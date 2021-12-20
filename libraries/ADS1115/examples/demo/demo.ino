#include <Wire.h>
#include <ADS1115.h>

ADS1115 ads(ADS1115_ADDRESS0);

void setup()
{
	Serial.begin(19200);
	ads.begin();
}
 
void loop()
{
	if(ads.ready())
	{
		Serial.print(ads.analogRead(ADS1115_A0) >> 4, DEC);
		Serial.print("\t");

		Serial.print(ads.analogRead(ADS1115_A1) >> 4, DEC);
		Serial.print("\t");

		Serial.print(ads.analogRead(ADS1115_A2) >> 4, DEC);
		Serial.print("\t");

		Serial.print(ads.analogRead(ADS1115_A3) >> 4, DEC);
		Serial.print("\t");

		Serial.print("\r\n");
	}

	delay(10);
}


