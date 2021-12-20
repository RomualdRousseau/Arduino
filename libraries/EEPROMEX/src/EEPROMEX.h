/**************************************************************************/
/*!
	@file     EEPROMEX.h

	This is a extention library for EEPROM

	@section  HISTORY

	v1.0  - First release
*/
/**************************************************************************/
#ifndef EEPROMEX_h
#define EEPROMEX_h

#include <EEPROM.h>

class EEPROMEXClass
{
public:
	template<class T> int put(int ee, const T& value)
	{
		const byte* p = (const byte*)(const void*)&value;
		int i;
		for (i = 0; i < sizeof(value); i++)
		{
			EEPROM.write(ee++, *p++);
		}
		return i;
	}

	template<class T> int get(int ee, T& value)
	{
		byte* p = (byte*)(void*)&value;
		int i;
		for (i = 0; i < sizeof(value); i++)
		{
			*p++ = EEPROM.read(ee++);
		}
		return i;
	}
};

extern EEPROMEXClass EEPROMEX;
#endif
