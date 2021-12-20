/**************************************************************************/
/*!
	@file     I2CDevices.h
	@author   Romuald Rousseau
	@license  BSD (see license.txt)

	Driver for the I2C devices

	This is a library to simplifies writing I2C device drivers

	@section  HISTORY

	v1.0  - First release
*/
/**************************************************************************/
#ifndef I2CDEVICES_h
#define I2CDEVICES_h

class I2CDevicesClass
{
public:
	void begin();
	
	uint8_t fast_read_register8(int addr, uint8_t reg);
	
	uint8_t read_register8(int addr, uint8_t reg);
	
	void write_register8(int addr, uint8_t reg, uint8_t value);
	
	int16_t read_register16(int addr, int16_t reg);
	
	void write_register16(int addr, uint8_t reg, int16_t value);
	
	uint8_t read_register_bit(int addr, uint8_t reg, uint8_t pos);
	
	void write_register_bit(int addr, uint8_t reg, uint8_t pos, uint8_t value);
	
	uint8_t read_register_bits(int addr, uint8_t reg, uint8_t mask, uint8_t pos);
	
	void write_register_bits(int addr, uint8_t reg, uint8_t mask, uint8_t pos, uint8_t value);
	
	void read_raw_buffer(int addr, int reg, uint8_t *buffer, int size);
	
	void write_raw_buffer(int addr, int reg, const uint8_t *buffer, int size);
};

static I2CDevicesClass I2CDevices;
#endif
