/**************************************************************************/
/*!
	@file     I2CDevices.cpp
	@author   Romuald Rousseau
	@license  BSD (see license.txt)

	Driver for the I2C devices

	This is a library to simplifies writing I2C device drivers

	@section  HISTORY

	v1.0  - First release
*/
/**************************************************************************/
#include "Arduino.h"
#include "Wire.h"

#include "I2CDevices.h"

static bool i2c_devices_initialized = false;

void I2CDevicesClass::begin()
{
	if(!i2c_devices_initialized)
	{
		i2c_devices_initialized = true;
		Wire.begin();
	}
}

uint8_t I2CDevicesClass::fast_read_register8(int addr, uint8_t reg)
{
	uint8_t value;

	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.endTransmission(false);

	Wire.requestFrom(addr, 1, true);
	value = Wire.read();

	return value;
}

uint8_t I2CDevicesClass::read_register8(int addr, uint8_t reg)
{
	uint8_t value;

	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.endTransmission(false);

	Wire.requestFrom(addr, 1, true);
	while(!Wire.available()) {};
	value = Wire.read();

	return value;
}

void I2CDevicesClass::write_register8(int addr, uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission(true);
}

int16_t I2CDevicesClass::read_register16(int addr, int16_t reg)
{
	int8_t value[2];
	read_raw_buffer(addr, reg, value, 2);
	return value[0] << 8 | value[1];
}

void I2CDevicesClass::write_register16(int addr, uint8_t reg, int16_t value)
{
	int8_t swapped_value[2] = {value >> 8, value};
	write_raw_buffer(addr, reg, swapped_value, 2);
}

uint8_t I2CDevicesClass::read_register_bit(int addr, uint8_t reg, uint8_t pos)
{
	uint8_t value = read_register8(addr, reg) & (1 << pos);
	return value >> pos;
}

void I2CDevicesClass::write_register_bit(int addr, uint8_t reg, uint8_t pos, uint8_t value)
{
	uint8_t value_org = read_register8(addr, reg) & ~(1 << pos);
	write_register8(addr, reg, value_org | (value << pos));
}

uint8_t I2CDevicesClass::read_register_bits(int addr, uint8_t reg, uint8_t mask, uint8_t pos)
{
	uint8_t value = read_register8(addr, reg) & (mask << pos);
	return value >> pos;
}

void I2CDevicesClass::write_register_bits(int addr, uint8_t reg, uint8_t mask, uint8_t pos, uint8_t value)
{
	uint8_t value_org = read_register8(addr, reg) & ~(mask << pos);
	write_register8(addr, reg, value_org | (value << pos));
}

void I2CDevicesClass::read_raw_buffer(int addr, int reg, uint8_t *buffer, int size)
{
	int i = 0;

	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.endTransmission(false);

	Wire.requestFrom(addr, size, true);
	while(Wire.available() && i < size)
	{
		buffer[i++] = Wire.read();
	}
}

void I2CDevicesClass::write_raw_buffer(int addr, int reg, const uint8_t *buffer, int size)
{
	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.write(buffer, size);
	Wire.endTransmission(true);
}
