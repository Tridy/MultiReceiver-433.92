#include "OregonDecoder.h"

OregonDecoder::OregonDecoder(byte mock)
{

}

String OregonDecoder::Decode(bool* valuesToDecode)
{
	_resultDecodedOregonMessage = "N-A";
	bool doubleShortAsOne = true;

	byte c = 0;
	byte d = 0;

	while (c < 180)
	{
		if (valuesToDecode[c])
		{
			doubleShortAsOne = !doubleShortAsOne;
			_oregonValues[d] = doubleShortAsOne;
			c++;
		}
		else
		{
			_oregonValues[d] = doubleShortAsOne;
			c += 2;
		}

		d++;
	}

	_countToReverse = d;

	ClearOregonValues();
	ReverseOregonValues();
	BuildOregonResult();
	return _resultDecodedOregonMessage;
}

void OregonDecoder::ClearOregonValues()
{
	for (byte i = 0; i < 22; i++)
	{
		_oregonValuesToBuildFrom[i] = 0;
	}
}

void OregonDecoder::ReverseOregonValues()
{
	_oregonValuesToBuildFrom[0] = 15;

	for (byte i = 1; i < _countToReverse; i++)
	{
		byte val = 0;

		int j;

		for (j = 3; j >= 0; j = j - 1)
		{
			bool bit = _oregonValues[(i * 4) + j];
			val = val ^ bit;

			if (j > 0)
			{
				val = val << 1;
			}
		}

		_oregonValuesToBuildFrom[i] = val;
	}
}

void OregonDecoder::BuildOregonResult()
{
	int checkSumResult = 0;

	for (byte cs = 0; cs < 15; cs++)
	{
		checkSumResult += _oregonValuesToBuildFrom[cs];
	}

	String sensorId = GetHexValue(0) + GetHexValue(1) + GetHexValue(2) + GetHexValue(3);
	String channel = GetHexValue(4);
	String rollingCode = GetHexValue(5) + GetHexValue(6);
	String lowBattery = _oregonValuesToBuildFrom[7] == 1 ? "Yes" : "No";
	String temperature = (_oregonValuesToBuildFrom[11] == 0 ? "" : "-") + GetHexValue(10) + GetHexValue(9) + "." + GetHexValue(8);
	String humidity = GetHexValue(13) + GetHexValue(12);
	String checksum = GetHexValue(16) + GetHexValue(15);

	String combinedResult = String(sensorId + "," + 
		channel + "," + 
		rollingCode + "," + 
		lowBattery + "," + 
		temperature + "," + 
		humidity + "," + 
		checksum + "," + 
		String(checkSumResult, HEX));

	_resultDecodedOregonMessage = combinedResult;
}

String OregonDecoder::GetHexValue(int itemIndex)
{
	return String(_oregonValuesToBuildFrom[itemIndex], HEX);
}
