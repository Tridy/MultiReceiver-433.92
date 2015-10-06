#include "MultiReceiver.h"
#include "OregonReceiver.h"
#include "X10Receiver.h"

const byte REMOTE_CONTROL = 1;
const byte WEATHER_STATION = 2;
const byte NONE = 0;


MultiReceiver::MultiReceiver(byte pinId)
{
	_pinId = pinId;
}

String MultiReceiver::Receive()
{
	pinMode(_pinId, INPUT);
	ResetVariables();
	WaitForLongLow();
	return ReceiveData();
}

void MultiReceiver::ResetVariables()
{
	_previousLowLength = 0;
	_highLength = 0;
	_signalIntroType = NONE;
}

void MultiReceiver::WaitForLongLow()
{
	unsigned long t2 = 0;
	unsigned long lowLength = 0;
	_signalIntroType = NONE;

	while (_signalIntroType == NONE)
	{
		_previousLowLength = lowLength;
		lowLength = pulseIn(_pinId, LOW, 1000000);
		_highLength = (micros() - lowLength) - t2;
		t2 = micros();
		GetHighAndLowWithinLengthRanges();
	}

	// Serial.print("HIGH=" + String(_highLength)  + ", LOW=" + String(_previousLowLength));
}

void MultiReceiver::GetHighAndLowWithinLengthRanges()
{
	if (_highLength > 950 && _highLength < 1050 && _previousLowLength > 950 && _previousLowLength < 1050)
	{
		_signalIntroType = WEATHER_STATION;
		Serial.println("WS");
		//Serial.print("HIGH=" + String(_highLength)  + ", LOW=" + String(_previousLowLength));
	}
	else if (_highLength > 3800 && _highLength < 3900 && _previousLowLength > 3550 && _previousLowLength < 3650)
	{
		_signalIntroType = REMOTE_CONTROL;
		// Serial.println("RC");
	}
}

String MultiReceiver::ReceiveData()
{
	// Serial.println("R");
	if (_signalIntroType == REMOTE_CONTROL)
	{
		return X10Receiver(_pinId).Receive();
		// return "REMOTE";
	}
	else if (_signalIntroType == WEATHER_STATION)
	{
		//Serial.println("O.R.");
		// return OregonReceiver(_pinId).Receive();
		return "OREGON MOCK";
	}

	return "NONE";
}