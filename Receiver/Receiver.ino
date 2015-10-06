#include "MultiReceiver.h"
void setup()
{
	
}
void loop()
{
	String value = MultiReceiver(8).Receive();
	Serial.println(value);
	delay(1000);
}