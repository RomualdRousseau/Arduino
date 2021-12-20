/**************************************************************************/
/*!
	@file     WiiChuck.cpp
	@author   Romuald Rousseau
	@license  BSD (see license.txt)

	Driver for the WII numchuck

	This is a library for the WII Numchuck

	@section  HISTORY

	v1.0 - First release
*/
/**************************************************************************/
#include "Arduino.h"
#include "Wire.h"
#include "WiiChuck.h"

#define POWER_VIA_PORT_C2_C3		1

#define WII_TELEGRAM_LEN			((byte)6)
#define WII_NUNCHUCK_TWI_ADR		((byte)0x52)

static uint8_t outbuf[WII_TELEGRAM_LEN];

WiiChuck::WiiChuck()
{
	JX = 0;
	JY = 0;

	BZ = LOW;
	BC = LOW;

	AX = 0;
	AY = 0;
	AZ = 0;
}

void WiiChuck::begin()
{
#ifdef POWER_VIA_PORT_C2_C3
	PORTC &=~ _BV(PORTC2);
	PORTC |= _BV(PORTC3);
	DDRC |= _BV(PORTC2) | _BV(PORTC3);
	delay(100);
#endif

	Wire.begin();

	byte rc = 1;
	do
	{
		Wire.beginTransmission(WII_NUNCHUCK_TWI_ADR);
		Wire.write(0xF0);
		Wire.write(0x55);
		if(Wire.endTransmission() == 0)
		{
			Wire.beginTransmission(WII_NUNCHUCK_TWI_ADR);
			Wire.write(0xFB);
			Wire.write(0x00);
			if(Wire.endTransmission() == 0)
			{
				rc = 0;
			}
		}
	}
	while(rc != 0);
}

uint8_t WiiChuck::ready()
{
	byte i;

	Wire.requestFrom(WII_NUNCHUCK_TWI_ADR, WII_TELEGRAM_LEN);
	for (i = 0; (i < WII_TELEGRAM_LEN) && Wire.available(); i++)
	{
		outbuf[i] = Wire.read();
	}
	while(Wire.available())
	{
		Wire.read();
	}

	Wire.beginTransmission(WII_NUNCHUCK_TWI_ADR);
	Wire.write(0x00);
	Wire.endTransmission();

	if(i >= WII_TELEGRAM_LEN)
	{
		JX = outbuf[0] << 2;
		JY = outbuf[1] << 2;

		BZ = ((outbuf[5] >> 0) & 1) ? LOW : HIGH;
		BC = ((outbuf[5] >> 1) & 1) ? LOW : HIGH;

		AX = (map(outbuf[2] << 2, 256, 768, 0, 255) | (((outbuf[5] >> 2) & 1) << 1) | ((outbuf[5] >> 3) & 1)) << 2;
		AY = (map(outbuf[3] << 2, 256, 768, 0, 255) | (((outbuf[5] >> 4) & 1) << 1) | ((outbuf[5] >> 5) & 1)) << 2;
		AZ = (map(outbuf[4] << 2, 256, 768, 0, 255) | (((outbuf[5] >> 6) & 1) << 1) | ((outbuf[5] >> 7) & 1)) << 2;
		return true;
	}
	else
	{
		return false;
	}
}


