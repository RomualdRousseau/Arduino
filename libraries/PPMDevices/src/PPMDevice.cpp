/**************************************************************************/
/*!
    @file     PPMDevice.cpp
    @author   Romuald Rousseau
    @license  BSD (see license.txt)

    Driver for PPM device

    This is a library for PPM device

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/
#include "Arduino.h"

#include "PPMDevice.h"

const int PPMFreq_uS  = 22500;  // PPM frame length total in uS
const int PPMFixed_uS = 300;    // PPM frame padding low phase in uS

static volatile bool     PPM_HAL_ISHW;
static volatile bool     PPM_HAL_ISINV;
static volatile uint8_t* PPM_HAL_DDR;
static volatile uint8_t* PPM_HAL_PORT;
static volatile uint8_t  PPM_HAL_PIN;

static volatile int PPM_pulseScale_uS;

static volatile int PPM_array[PPM_MAX_CHANNELS + 1];

/**************************************************************************/
/*!
    @brief  Instantiates a new PPMDevice class w/appropriate properties
*/
/**************************************************************************/
PPMDevice::PPMDevice(int pin)
{
	// Virtualize HW access

	PPM_HAL_ISHW = (pin == PPM_HW_PIN);
	PPM_HAL_DDR = portModeRegister(digitalPinToPort(pin));
	PPM_HAL_PORT = portOutputRegister(digitalPinToPort(pin));
	PPM_HAL_PIN = digitalPinToBitMask(pin);

	if (PPM_HAL_ISHW) {
		PPM_pulseScale_uS = 1; // F_CPU / (8 * 1000000 * 2)
	} else {
		PPM_pulseScale_uS = 2; // F_CPU / (8 * 1000000)
	}

	// Initialize PPM channels with "good" initial values
	
	for (int i = 0; i < PPM_MAX_CHANNELS; i++) {
		PPM_array[i] = (PPM_MAX_PULSE_uS + PPM_MIN_PULSE_uS) / 2;
	}
	PPM_array[PPM_MAX_CHANNELS] = -1; // Mark end
}

/**************************************************************************/
/*!
    @brief  Sets up the HW
*/
/**************************************************************************/
void PPMDevice::begin(bool inverted) {
	PPM_HAL_ISINV = inverted;

	cli();
	
	// Set PPM output

	*PPM_HAL_DDR |= PPM_HAL_PIN;
	
	// Set timer1 for PPM signal
	
	TCCR1A = 0;
	TCCR1B = 0;
	
	if (PPM_HAL_ISHW) {

		// PWM mode with compare register interrupt
		
		TCCR1A |= !PPM_HAL_ISINV ? B00110000 : B00100000; // Use Compare Register B with inverted / non-inverted output
		TCCR1A |= (1 << WGM10);					          // PWM mode 3
		TCCR1B |= (1 << WGM13);
	} else {
	
		// Set right initial pin level
	
		if (!PPM_HAL_ISINV) {
			*PPM_HAL_PORT |= PPM_HAL_PIN;
		} else {
			*PPM_HAL_PORT &= ~PPM_HAL_PIN;
		}
		
		// CTC mode without compare register interrupt

		TCCR1B |= (1 << WGM12); // CTC mode
	}
	
	TCCR1B |= (1 << CS11); // 8 prescaler
	
	TCNT1 = 0;                               // Counter start
	OCR1A = PPMFreq_uS * PPM_pulseScale_uS;  // Counter top (22500us)
	OCR1B = PPMFixed_uS * PPM_pulseScale_uS; // Counter off time (300us) 
	
	TIMSK1 = (1 << OCIE1A); // Enable Compare Register A interrupts
	TIFR1  = (1 << OCIE1A);
		
	if (!PPM_HAL_ISHW) {
		TIMSK1 |= (1 << OCIE1B); // Enable Compare Register B interrupts
		TIFR1  |= (1 << OCIE1B);
	}
	
	sei(); 
}

/**************************************************************************/
/*!
    @brief  Write a value
*/
/**************************************************************************/
int PPMDevice::write(int channel, int value) {
	PPM_array[channel] = map(value, 0, 1023, PPM_MIN_PULSE_uS, PPM_MAX_PULSE_uS);
	return 1;
}

/**************************************************************************/
/*!
    @brief  Read a value
*/
/**************************************************************************/
int PPMDevice::read(int channel) {
	return map(PPM_array[channel], PPM_MIN_PULSE_uS, PPM_MAX_PULSE_uS, 0, 1023);
}

/**************************************************************************/
/*!
    @brief  Read a value
*/
/**************************************************************************/
int PPMDevice::readRaw(int channel) {
	return PPM_array[channel];
}

// **************************** BEGIN TIMER *******************************

ISR(TIMER1_COMPA_vect) {
	static volatile int *PPM_pointer = PPM_array;
	static volatile int ACC_PPM_len = 0;
	int curr_value = *(PPM_pointer++);
	
	if (!PPM_HAL_ISHW) {
		if (!PPM_HAL_ISINV) {
			*PPM_HAL_PORT &= ~PPM_HAL_PIN;
		} else {
			*PPM_HAL_PORT |= PPM_HAL_PIN;
		}
	}

	if (curr_value > -1) {
		OCR1A = curr_value * PPM_pulseScale_uS;
		ACC_PPM_len += curr_value;
	}
	else {
		OCR1A = (PPMFreq_uS - ACC_PPM_len) * PPM_pulseScale_uS;
		PPM_pointer = PPM_array;
		ACC_PPM_len = 0;
	}
}

ISR(TIMER1_COMPB_vect) {
	if (!PPM_HAL_ISHW) {
		if (!PPM_HAL_ISINV) {
			*PPM_HAL_PORT |= PPM_HAL_PIN;
		} else {
			*PPM_HAL_PORT &= ~PPM_HAL_PIN;
		}
	}
}
// ******************************* END TIMER *******************************

