#include "MultiReceiver.h"
#include "X10Decoder.h"
#include "OregonDecoder.h"


const byte MAX_READINGS = 180;
const byte MIN_SIGNAL_LENGTH = 24;

const unsigned long MIN_LONG_PAUSE = 100000;
const unsigned long MAX_LONG_PAUSE = 210000;

byte _readings[MAX_READINGS];
byte _orderedReadings[MAX_READINGS];
bool _extractedValues[MAX_READINGS];

byte _breakIndex;

X10Decoder _x10Decoder(0);
OregonDecoder _oregonDecoder(0);

MultiReceiver::MultiReceiver(byte pinId)
{
	_pinId = pinId;
}

String MultiReceiver::ReceiveMulti()
{
	pinMode(_pinId, INPUT);

	ResetVariables();
	WaitForLongLow();
	RearrangeReadings();
	return ReceiveData();
}

void MultiReceiver::ResetVariables()
{
	_previousLowLength = 0;
	_highLength = 0;
	_breakIndex = 0;

	for (size_t i = 0; i < MAX_READINGS; i++)
	{
		_readings[i] = 0;
	}
}

void MultiReceiver::WaitForLongLow()
{
	_breakIndex = 0;
	unsigned long t2 = 0;
	unsigned long low = 0;
	unsigned long high = 0;

	while (low < MIN_LONG_PAUSE || low > MAX_LONG_PAUSE)
	{
		low = pulseIn(_pinId, LOW, 1000000);
		high = (micros() - low) - t2;
		t2 = micros();
		_readings[_breakIndex] = high / 100;
		_readings[_breakIndex + 1] = low / 100;
		_breakIndex += 2;

		if (_breakIndex >= MAX_READINGS)
		{
			_breakIndex = 0;
		}
	}
}

void MultiReceiver::RearrangeReadings()
{
	byte r = 0;
	byte i1;

	for (i1 = _breakIndex; i1 < MAX_READINGS; i1 = i1 + 2)
	{
		_orderedReadings[r] = SimplifyByteValue(_readings[i1]);
		_orderedReadings[r + 1] = SimplifyByteValue(_readings[i1 + 1]);
		r = r + 2;
	}

	byte i2;
	for (i2 = 0; i2 <= _breakIndex; i2 = i2 + 2)
	{
		_orderedReadings[r] = SimplifyByteValue(_readings[i2]);
		_orderedReadings[r + 1] = SimplifyByteValue(_readings[i2 + 1]);
		r = r + 2;
	}
}

byte MultiReceiver::SimplifyByteValue(byte originalValue)
{
	if (originalValue < 7)
	{
		return 1;
	}

	if (originalValue < 20)
	{
		return 2;
	}

	return 3;
}

String MultiReceiver::ReceiveData()
{
	for (size_t rd = MAX_READINGS - MIN_SIGNAL_LENGTH; rd > 0; rd = rd -2)
	{
		if (IsSignalOregon(rd))
		{
			return _oregonDecoder.Decode(_extractedValues);
		}

		if (IsSignalX10(rd))
		{
			return _x10Decoder.Decode(_extractedValues);
		}
	}

	return "ERROR";
}

bool MultiReceiver::IsSignalX10(size_t positionIndex)
{
	const byte INTRO_LENGTH = 4;

	if (positionIndex > MAX_READINGS - INTRO_LENGTH)
	{
		return false;
	}

	bool isMatch = _orderedReadings[positionIndex] == 1 &&
		_orderedReadings[positionIndex + 1] == 3 &&
		_orderedReadings[positionIndex + 2] == 3 &&
		_orderedReadings[positionIndex + 3] == 1;

	if (!isMatch)
	{
		return false;
	}

	ExtractSignalReadings(positionIndex + INTRO_LENGTH);

	return true;
}

bool MultiReceiver::IsSignalOregon(size_t positionIndex)
{
	const byte INTRO_LENGTH = 6;

	if (positionIndex < 1)
	{
		return false;
	}

  if (positionIndex > MAX_READINGS - INTRO_LENGTH)
  {
    return false;
  }
  
	bool isMatch = _orderedReadings[positionIndex] == 1 &&
		_orderedReadings[positionIndex + 1] == 2 &&
		_orderedReadings[positionIndex + 2] == 2 &&
		_orderedReadings[positionIndex + 3] == 2 &&
		_orderedReadings[positionIndex + 4] == 2 &&
		_orderedReadings[positionIndex + 5] == 1;

	if (!isMatch)
	{
		return false;
	}

	ExtractSignalReadings(positionIndex + INTRO_LENGTH -1);

	return true;
}

void MultiReceiver::ExtractSignalReadings(size_t positionIndex)
{
	for (size_t i = positionIndex; i < MAX_READINGS; i++)
	{
		_extractedValues[i - positionIndex] = _orderedReadings[i] > 1;
	}
}
