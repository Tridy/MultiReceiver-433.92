#include "X10Decoder.h"

X10Decoder::X10Decoder(byte mock)
{
}

String X10Decoder::Decode(bool* readings)
{
	SeparateLows(readings);
	return BuildResult();
}

void X10Decoder::SeparateLows(bool* readings)
{
	byte v = 0;
	for (size_t i = 0; i < 40; i += 2)
	{
		_x10Values[v++] = readings[i + 1] > 0;
	}
}

String X10Decoder::BuildResult()
{
	byte buttonIndex = GetValueFromReceivedValuesRange(10, 15);
	byte readButtonCode = GetValueFromReceivedValuesRange(1, 8);
	String btn = String(buttonIndex) + ":" + String(readButtonCode);
	return btn;
}

byte X10Decoder::GetValueFromReceivedValuesRange(byte firstIndex, byte lastIndex)
{
	byte value = 0;

	for (size_t i = firstIndex; i <= lastIndex; i++)
	{
		value = value | _x10Values[i];

		if (i < lastIndex)
		{
			value <<= 1;
		}
	}

	return value;
}
