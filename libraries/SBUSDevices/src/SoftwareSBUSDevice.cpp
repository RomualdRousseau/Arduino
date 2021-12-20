/**************************************************************************/
/*!
    @file     SoftwareSBUSDevice.cpp
    @author   Romuald Rousseau
    @license  BSD (see license.txt)

    Driver for SBUS device

    This is a library to read SBUS signals

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/
#include "Arduino.h"

#include "SBUSDevice.h"

#define SBUS_PAYLOAD_COUNT 4

volatile static uint8_t* SBUS_HAL_DDR;
volatile static uint8_t* SBUS_HAL_PORTW;
volatile static uint8_t* SBUS_HAL_PORTR;
volatile static uint8_t* SBUS_HAL_PCMSK;
volatile static uint8_t SBUS_HAL_PCIE;
volatile static uint8_t SBUS_HAL_PCINT;
volatile static uint8_t SBUS_HAL_PIN;

volatile static uint8_t SBUS_payload[SBUS_PAYLOAD_COUNT][25];
volatile static uint8_t SBUS_prev_payload_idx;
volatile static uint8_t SBUS_curr_payload_idx;

/**************************************************************************/
/*!
    @brief  Instantiates a new SoftwareSBUSDevice class w/appropriate properties
*/
/**************************************************************************/
SoftwareSBUSDevice::SoftwareSBUSDevice(int pin) {

	// Virtualize HW access

	SBUS_HAL_DDR = portModeRegister(digitalPinToPort(pin));
	SBUS_HAL_PORTW = portOutputRegister(digitalPinToPort(pin));
	SBUS_HAL_PORTR = portInputRegister(digitalPinToPort(pin));
	SBUS_HAL_PCMSK = digitalPinToPCMSK(pin);
	SBUS_HAL_PCIE = (1 << digitalPinToPCICRbit(pin));
	SBUS_HAL_PCINT = (1 << digitalPinToPCMSKbit(pin));
	SBUS_HAL_PIN = digitalPinToBitMask(pin);
	
	// Prepare SBUS_payload ring structure and ensure "good" initial values

	SBUS_prev_payload_idx = 0;
	SBUS_curr_payload_idx = 0;

	for(int i = 0; i < SBUS_MAX_CHANNELS; i++) {
		_channels[i] = (SBUS_MIN_PULSE_uS + SBUS_MAX_PULSE_uS) / 2;
	}
}

/**************************************************************************/
/*!
    @brief  Sets up the HW
*/
/**************************************************************************/
void SoftwareSBUSDevice::begin() {
	cli();

	// Set SBUS input

	*SBUS_HAL_DDR &= ~SBUS_HAL_PIN;      // Input pin
	*SBUS_HAL_PORTW |= SBUS_HAL_PIN;      // Pull-up
	*SBUS_HAL_PCMSK |= SBUS_HAL_PCINT; // Enable interrupt change on SBUS_HAL_PIN
	PCICR |= SBUS_HAL_PCIE;          // Enable interrupt for the group X

	// Set timer2 for SBUS signal

	TCCR2A = 1 << WGM21;          // CTC Mode
	TCCR2B = 1 << CS20;           // No prescaling
	TCNT2 = 0;                    // Counter start
	OCR2A = (F_CPU / 100000) - 1; // Counter top (~10us)

	sei();
}

int SoftwareSBUSDevice::available() {
	if (SBUS_prev_payload_idx == SBUS_curr_payload_idx) {
		return 0;
	}

	// Decode and normalize SBUS_payload

	uint8_t* p = &SBUS_payload[SBUS_prev_payload_idx][0];
	_channels[0]  = (uint16_t) ((p[0]    |p[1] <<8)            & 0x07FF);
	_channels[1]  = (uint16_t) ((p[1]>>3 |p[2] <<5)            & 0x07FF);
	_channels[2]  = (uint16_t) ((p[2]>>6 |p[3] <<2 |p[4]<<10)  & 0x07FF);
	_channels[3]  = (uint16_t) ((p[4]>>1 |p[5] <<7)            & 0x07FF);
	_channels[4]  = (uint16_t) ((p[5]>>4 |p[6] <<4)            & 0x07FF);
	_channels[5]  = (uint16_t) ((p[6]>>7 |p[7] <<1 |p[8]<<9)   & 0x07FF);
	_channels[6]  = (uint16_t) ((p[8]>>2 |p[9] <<6)            & 0x07FF);
	_channels[7]  = (uint16_t) ((p[9]>>5 |p[10]<<3)            & 0x07FF);
	_channels[8]  = (uint16_t) ((p[11]   |p[12]<<8)            & 0x07FF);
	_channels[9]  = (uint16_t) ((p[12]>>3|p[13]<<5)            & 0x07FF);
	_channels[10] = (uint16_t) ((p[13]>>6|p[14]<<2 |p[15]<<10) & 0x07FF);
	_channels[11] = (uint16_t) ((p[15]>>1|p[16]<<7)            & 0x07FF);
	_channels[12] = (uint16_t) ((p[16]>>4|p[17]<<4)            & 0x07FF);
	_channels[13] = (uint16_t) ((p[17]>>7|p[18]<<1 |p[19]<<9)  & 0x07FF);
	_channels[14] = (uint16_t) ((p[19]>>2|p[20]<<6)            & 0x07FF);
	_channels[15] = (uint16_t) ((p[20]>>5|p[21]<<3)            & 0x07FF);

	SBUS_prev_payload_idx = SBUS_curr_payload_idx;

	return 1;
}

/**************************************************************************/
/*!
    @brief  Read a SBUS channel
*/
/**************************************************************************/
int SoftwareSBUSDevice::read(int channel) {
	return map(_channels[channel], SBUS_MIN_PULSE_uS, SBUS_MAX_PULSE_uS, 0, 1023);
}

/**************************************************************************/
/*!
    @brief  Read SBUS flag
*/
/**************************************************************************/
int SoftwareSBUSDevice::getFlag() {
	return _flag;
}

// *********************** BEGIN INTERRUPT **************************
static void handleSBUSInt() {
	volatile static int SBUS_payload_state = 0;
	
	if (!(*SBUS_HAL_PORTR & SBUS_HAL_PIN)) {
		return;
	}

	// Disable SBUS interrupt

	PCICR &= ~SBUS_HAL_PCIE;

	// Reset timer2

	TCNT2 = 0;
	TIFR2 |= 1 << OCF2A;

	// Read 9 bits (8 data bits + 1 parity bit)

	byte data = 0;
	byte parity = 0xFF;
	for (int i = 0; i < 9; i++) {

		// Delay timer2

		sei();
		while (!(TIFR2 & (1 << OCF2A)));
		cli();
		TIFR2 |= 1 << OCF2A;

		// Read one bit and calculate parity

		if (*SBUS_HAL_PORTR & SBUS_HAL_PIN) {
			data |= 1 << i;
		} else {
			parity = ~parity;
		}
	}

	// Check parity and process data

	if (!parity) {

    	// Ignore current SBUS_payload if parity error
    
    	SBUS_payload_state = 0;
    
	} else {
    	data = ~data;
    	
    	switch (SBUS_payload_state) {
			case 0:

				// Check header signature and continue or ignore
					
				if (data == 0x0F) {
					SBUS_payload_state = 1;
				}
				
				break;
				
			case 24:

				// Check footer signature and get ready new SBUS_payload or ignore

				if (data == 0x00) {
					SBUS_prev_payload_idx = SBUS_curr_payload_idx;
					SBUS_curr_payload_idx = (SBUS_curr_payload_idx + 1) % SBUS_PAYLOAD_COUNT;
				}
				
				SBUS_payload_state = 0;
				break;
			  
			default:
				SBUS_payload[SBUS_curr_payload_idx][SBUS_payload_state - 1] = data;	
				SBUS_payload_state++;
		}
	}

	// Enable SBUS interrupt

	PCICR |= SBUS_HAL_PCIE;
}

ISR(PCINT0_vect) {
	handleSBUSInt();
}

ISR(PCINT1_vect) {
	handleSBUSInt();
}

ISR(PCINT2_vect) {
	handleSBUSInt();
}
// *********************** END INTERRUPT **************************

