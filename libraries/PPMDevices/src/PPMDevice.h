/**************************************************************************/
/*!
	@file     PPMDevice.h
	@author   Romuald Rousseau
	@license  BSD (see license.txt)

	Driver for PPM device

	This is a library to write PPM signals

	@section  HISTORY

	v1.0  - First release
*/
/**************************************************************************/
#ifndef PPMDEVICE_h
#define PPMDEVICE_h

#define PPM_MAX_CHANNELS	8

#define PPM_MIN_PULSE_uS	1000
#define PPM_MAX_PULSE_uS	2000

#if defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
#define PPM_HW_PIN 12
#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#define PPM_HW_PIN 10
#endif

class PPMDevice
{
public:
	PPMDevice(int pin = PPM_HW_PIN);

	void begin(bool inverted = false);
	
	int write(int channel, int value);
	
	int read(int channel);
	
	int readRaw(int channel);
};

#endif
