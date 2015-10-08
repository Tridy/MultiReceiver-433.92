#include "MultiReceiver.h"

MultiReceiver _multiReceiver(8);

void setup()
{

}
void loop()
{
	String value = _multiReceiver.ReceiveMulti();
	Serial.println(value);
	delay(1000);
}