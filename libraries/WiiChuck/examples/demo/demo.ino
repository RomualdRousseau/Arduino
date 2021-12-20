#include <Wire.h>
#include <WiiChuck.h>

WiiNunChuck NunC;

void setup()
{
	Serial.begin(19200);
	NunC.begin();
}

void loop()
{
	if(NunC.ready())
	{
		Serial.print (NunC.JX, DEC);
		Serial.print ("\t");

		Serial.print (NunC.JY, DEC);
		Serial.print ("\t");

		Serial.print (NunC.AX, DEC);
		Serial.print ("\t");

		Serial.print (NunC.AY, DEC);

		Serial.print ("\t");

		Serial.print (NunC.AZ, DEC);
		Serial.print ("\t");

		Serial.print (NunC.BZ, DEC);
		Serial.print ("\t");

		Serial.print (NunC.BC, DEC);
		Serial.print ("\t");

		Serial.print ("\r\n");
	}
	delay(10);
}


