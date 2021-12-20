/**************************************************************************/
/*!
    @file     SBUSDevice.cpp
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

/**************************************************************************/
/*!
    @brief  Instantiates a new SBUSDevice class w/appropriate properties
*/
/**************************************************************************/
SBUSDeviceClass::SBUSDeviceClass() {

	// Prepare and ensure "good" initial values

	for(int i = 0; i < SBUS_MAX_CHANNELS; i++) {
		_channels[i] = (SBUS_MIN_PULSE_uS + SBUS_MAX_PULSE_uS) / 2;
	}
}

/**************************************************************************/
/*!
    @brief  Sets up the HW
*/
/**************************************************************************/
void SBUSDeviceClass::begin() {
	Serial.begin(100000, SERIAL_8E2);
	while (!Serial) {
	; // wait for serial port to connect. Needed for Native USB only
	}
}

int SBUSDeviceClass::available() {
	if (!Serial.available()) {
		return 0;
	}
		
	byte header;
	Serial.readBytes(&header, 1);
	if(header != 0x0F || !_read_one_packet()) {
		return 0;
	}
	
	return 1;
}

/**************************************************************************/
/*!
    @brief  Read a SBUS channel
*/
/**************************************************************************/
int SBUSDeviceClass::read(int channel) {
	return map(_channels[channel], SBUS_MIN_PULSE_uS, SBUS_MAX_PULSE_uS, 0, 1023);
}

/**************************************************************************/
/*!
    @brief  Read SBUS flag
*/
/**************************************************************************/
int SBUSDeviceClass::getFlag() {
	return _flag;
}

bool SBUSDeviceClass::_read_one_packet() {
  byte _payload[24];
  
  if (Serial.readBytes(_payload, 24) != 24) {
    return false;
  }
  
  if (_payload[23] != 0x00) {
    return false;
  }

  _channels[0]  = (uint16_t) ((_payload[0]    |_payload[1] <<8)                   & 0x07FF);
  _channels[1]  = (uint16_t) ((_payload[1]>>3 |_payload[2] <<5)                   & 0x07FF);
  _channels[2]  = (uint16_t) ((_payload[2]>>6 |_payload[3] <<2 |_payload[4]<<10)  & 0x07FF);
  _channels[3]  = (uint16_t) ((_payload[4]>>1 |_payload[5] <<7)                   & 0x07FF);
  _channels[4]  = (uint16_t) ((_payload[5]>>4 |_payload[6] <<4)                   & 0x07FF);
  _channels[5]  = (uint16_t) ((_payload[6]>>7 |_payload[7] <<1 |_payload[8]<<9)   & 0x07FF);
  _channels[6]  = (uint16_t) ((_payload[8]>>2 |_payload[9] <<6)                   & 0x07FF);
  _channels[7]  = (uint16_t) ((_payload[9]>>5 |_payload[10]<<3)                   & 0x07FF);
  _channels[8]  = (uint16_t) ((_payload[11]   |_payload[12]<<8)                   & 0x07FF);
  _channels[9]  = (uint16_t) ((_payload[12]>>3|_payload[13]<<5)                   & 0x07FF);
  _channels[10] = (uint16_t) ((_payload[13]>>6|_payload[14]<<2 |_payload[15]<<10) & 0x07FF);
  _channels[11] = (uint16_t) ((_payload[15]>>1|_payload[16]<<7)                   & 0x07FF);
  _channels[12] = (uint16_t) ((_payload[16]>>4|_payload[17]<<4)                   & 0x07FF);
  _channels[13] = (uint16_t) ((_payload[17]>>7|_payload[18]<<1 |_payload[19]<<9)  & 0x07FF);
  _channels[14] = (uint16_t) ((_payload[19]>>2|_payload[20]<<6)                   & 0x07FF);
  _channels[15] = (uint16_t) ((_payload[20]>>5|_payload[21]<<3)                   & 0x07FF);

  _flag = _payload[22];

  return true;
}

