/**************************************************************************/
/*!
    @file     SBUSDevice.h
    @author   Romuald Rousseau
    @license  BSD (see license.txt)

    Driver for SBUS device

    This is a library to read SBUS signals

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/
#ifndef SBUSDEVICE_h
#define SBUSDEVICE_h

#define SBUS_MAX_CHANNELS		16
#define SBUS_FLAG_SIGNAL_LOSS	4
#define SBUS_FLAG_FAILSAFE    	8

#define SBUS_MIN_PULSE_uS		172
#define SBUS_MAX_PULSE_uS		1811

class SBUSDeviceClass
{
public:
	SBUSDeviceClass();

	void begin();

	int available();

	int read(int channel);

	int getFlag();

private:
	bool _read_one_packet();
	  
	uint16_t _channels[SBUS_MAX_CHANNELS];
	uint8_t _flag;
};
static SBUSDeviceClass SBUSDevice;

class SoftwareSBUSDevice
{
public:
	SoftwareSBUSDevice(int pin = 2);

	void begin();

	int available();

	int read(int channel);

	int getFlag();

private:
	uint16_t _channels[SBUS_MAX_CHANNELS];
	uint8_t _flag;
};

#endif

