#include "OregonReceiver.h"
#include "OregonDecoder.h"

OregonDecoder _oregonDecoder(0);
byte _oregonCounter = 0;
const int MAX_READINGS_COUNT = 65;

OregonReceiver::OregonReceiver(int pinId)
{
	_oregonPinId = pinId;
}

String OregonReceiver::ReceiveOregon()
{
	ReceiveSignal();
	return _receivedOregonResult;
}

void OregonReceiver::ReceiveSignal()
{
	ReadValues();
	_receivedOregonResult = _oregonDecoder.DecodeOregonValues(_readOregonValues, _oregonCounter);
}

void OregonReceiver::ReadValues()
{
	_oregonCounter = 0;
	unsigned long low = 0;
	unsigned long lastLow = micros();
	unsigned long high = 0;
	bool vals[200];

	while (low < 10000 && _oregonCounter < MAX_READINGS_COUNT)
	{
		low = pulseIn(_oregonPinId, LOW, 1000000);

		high = micros() - (lastLow + low);
		lastLow = micros();

		vals[_oregonCounter] = high > 800;
		vals[_oregonCounter + 1] = low > 800;

		_oregonCounter += 2;
	}

	_readOregonValues = vals;
}