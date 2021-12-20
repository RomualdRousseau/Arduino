/**************************************************************************/
/*!
	@file     WiiChuck.h
	@author   Romuald Rousseau
	@license  BSD (see license.txt)

	Driver for the WII numchuck

	This is a library for the WII Numchuck

	@section  HISTORY

	v1.0 - First release
*/
/**************************************************************************/
#ifndef WiiChuck_h
#define WiiChuck_h

class WiiChuck
{
public:
	int16_t JX;
	int16_t JY;
	int16_t AX;
	int16_t AY;
	int16_t AZ;
	uint8_t BC;
	uint8_t BZ;

	WiiChuck();
	void begin();
	uint8_t ready();
};

#endif
