#include "MultiReceiver.h"

MultiReceiver _multiReceiver(8);

void setup()
{
	Serial.begin(115200);
  Serial.println("MODULE STARTUP");
}
void loop()
{
	String value = _multiReceiver.ReceiveMulti();
	Serial.println(value);
  delay(200);
}
