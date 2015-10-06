#include "OregonReceiver.h"
#include "OregonDecoder.h"

const int MaxReadingsCount = 65;

OregonReceiver::OregonReceiver(int pinId)
{
	ResetVariables();
	_pinId = pinId;
}

String OregonReceiver::Receive()
{
	ResetVariables();
	ReceiveSignal();
	return _receivedResult;
}

void OregonReceiver::ResetVariables()
{
	_counter = 0;
	_timing = 0;
}

void OregonReceiver::ReceiveSignal()
{
	ReadValues();
	Serial.println("READ=" + String(_counter));
	_receivedResult = OregonDecoder().DecodeValues(_readValues, _counter);
}

void OregonReceiver::ReadValues()
{
	unsigned long low = 0;
	unsigned long lastLow = micros();
	unsigned long high = 0;

	while (low < 10000 && _counter < MaxReadingsCount)
	{
		low = pulseIn(_pinId, LOW, 1000000);

		high = micros() - (lastLow + low);
		lastLow = micros();

		_readValues[_counter] = high > 800;
		_readValues[_counter + 1] = low > 800;

		_counter += 2;
	}
}