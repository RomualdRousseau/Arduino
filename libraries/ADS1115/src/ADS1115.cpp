/**************************************************************************/
/*!
	@file     ADS1115.cpp
	@author   Romuald Rousseau
	@license  BSD (see license.txt)

	Driver for the ADS1115 ADC

	This is a library for the ADS1115 breakout

	@section  HISTORY

	v1.0 - First release
*/
/**************************************************************************/
#include "Arduino.h"
#include "Wire.h"
#include "ADS1115.h"

/**************************************************************************/
/*!
	@brief  Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
	Wire.beginTransmission(i2cAddress);
	Wire.write((uint8_t) reg);
	Wire.write((uint8_t) (value >> 8));
	Wire.write((uint8_t) (value & 0xFF));
	Wire.endTransmission();
}

/**************************************************************************/
/*!
	@brief  Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
	Wire.beginTransmission(i2cAddress);
	Wire.write((uint8_t) ADS1115_REG_POINTER_CONVERT);
	Wire.endTransmission();
	Wire.requestFrom(i2cAddress, (uint8_t) 2);
	return (((uint8_t) Wire.read()) << 8) | ((uint8_t) Wire.read());
}

/**************************************************************************/
/*!
	@brief  Instantiates a new ADS1115 class w/appropriate properties
*/
/**************************************************************************/
ADS1115::ADS1115(uint8_t i2cAddress)
{
	m_i2cAddress = i2cAddress;
	m_state = 0;
	m_values[0] = 0;
	m_values[1] = 0;
	m_values[2] = 0;
	m_values[3] = 0;
}

/**************************************************************************/
/*!
	@brief  Sets up the HW (reads initial values, etc.)
*/
/**************************************************************************/
void ADS1115::begin()
{
	Wire.begin();
	while(!ready())
	{
		delay(ADS1115_CONVERSIONDELAY);
	}
}

/**************************************************************************/
/*!
	@brief  Checks if ADC is ready
*/
/**************************************************************************/
uint8_t ADS1115::ready()
{
	if((m_state & 1) == 0)
	{
		uint16_t config = ADS1115_REG_CONFIG_CQUE_NONE	| // Disable the comparator (default val)
					ADS1115_REG_CONFIG_CLAT_NONLAT  	| // Non-latching (default val)
					ADS1115_REG_CONFIG_CPOL_ACTVLOW		| // Alert/Rdy active low   (default val)
					ADS1115_REG_CONFIG_CMODE_TRAD   	| // Traditional comparator (default val)
					ADS1115_REG_CONFIG_DR_2400SPS   	| // 2400 samples per second
					ADS1115_REG_CONFIG_MODE_SINGLE		| // Single-shot mode (default)
					ADS1115_REG_CONFIG_PGA_6_144V		| // +/-6.144V range = Gain 2/3
					ADS1115_REG_CONFIG_OS_SINGLE;		  // Write: Set to start a single-conversion

		switch (m_state)
		{
		case (0):
		  config |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
		  break;
		case (2):
		  config |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
		  break;
		case (4):
		  config |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
		  break;
		case (6):
		  config |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
		  break;
		}
		writeRegister(m_i2cAddress, ADS1115_REG_POINTER_CONFIG, config);
		m_state++;
		m_time = millis();
	}
	else if((millis() - m_time) > ADS1115_CONVERSIONDELAY)
	{
		m_values[(m_state - 1) >> 1] = readRegister(m_i2cAddress, ADS1115_REG_POINTER_CONVERT);
		if(++m_state == 8)
		{
			m_state = 0;
			return true;
		}
	}
	return false;
}

/**************************************************************************/
/*!
	@brief  Read values
*/
/**************************************************************************/
uint16_t ADS1115::analogRead(uint8_t channel)
{
	if(channel > 3)
	{
		return 0;
	}
	return m_values[channel];
}
