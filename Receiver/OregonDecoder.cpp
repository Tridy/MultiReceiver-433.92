#include "OregonDecoder.h"

String _resultDecodedOregonMessage;

byte* _decodedOregonValues;
byte* _oregonValuesToBuildFrom;
byte _countToReverse;

OregonDecoder::OregonDecoder(byte mock)
{

}

String OregonDecoder::DecodeOregonValues(bool* valuesToDecode, byte count1)
{
	_resultDecodedOregonMessage = "N-A";

	byte decodedValues[84];
	bool doubleShortAsOne = true;

	byte c = 1;
	byte d = 0;

	while (c < count1)
	{
		if (valuesToDecode[c])
		{
			doubleShortAsOne = !doubleShortAsOne;
			decodedValues[d] = doubleShortAsOne;
			c++;
		}
		else
		{
			decodedValues[d] = doubleShortAsOne;
			c += 2;
		}

		d++;
	}

	_countToReverse = d;
	_decodedOregonValues = decodedValues;

	ReverseOregonValues();
	BuildOregonResult();

	return _resultDecodedOregonMessage;
}

void OregonDecoder::ReverseOregonValues()
{
	byte reversedValues[22];

	reversedValues[0] = 15;

	int i;

	Serial.println("");

	for (i = 1; i < _countToReverse; i++)
	{
		byte val = 0;

		int j;

		for (j = 3; j >= 0; j = j - 1)
		{
			bool bit = _decodedOregonValues[(i * 4) + j];
			val = val ^ bit;

			if (j > 0)
			{
				val = val << 1;
			}
		}

		reversedValues[i] = val;
	}

	_oregonValuesToBuildFrom = reversedValues;
}

void OregonDecoder::BuildOregonResult()
{
	
	byte checkSumResult = 0;

	int hh;
	for (hh = 0; hh < 16; hh++)
	{
		Serial.print(",");
		Serial.print(_oregonValuesToBuildFrom[hh]);
	}

	int cs;
	for (cs = 0; cs < 15; cs++)
	{
		checkSumResult += _oregonValuesToBuildFrom[cs];
	}

	String sensorId = GetHexValue(0) + GetHexValue(1) + GetHexValue(2) + GetHexValue(3);
	Serial.println("sensorId: " + sensorId);
	String channel = GetHexValue(4);
	Serial.println("channel:" + channel);
	String rollingCode = GetHexValue(5) + GetHexValue(6);
	Serial.println("rollingCode:" + rollingCode);
	String lowBattery = _oregonValuesToBuildFrom[7] == 1 ? "Yes" : "No";
	Serial.println("lowBattery:" + lowBattery);
	String temperature = (_oregonValuesToBuildFrom[11] == 0 ? "" : "-") + GetHexValue(10) + GetHexValue(9) + "." + GetHexValue(8);
	Serial.println("temperature:" + temperature);
	String humidity = GetHexValue(13) + GetHexValue(12);
	Serial.println("humidity:" + humidity);
	String checksum = GetHexValue(16) + GetHexValue(15);
	Serial.println("checksum:" + checksum);

	String combinedResult = String(
		"Sensor Id: " + sensorId +
		"\nChannel: " + channel +
		"\nRolling Code: " + rollingCode +
		"\nLow Battery: " + lowBattery +
		"\nTemperature: " + temperature +
		"\nHumidity: " + humidity +
		"\nChecksum: " + checksum);

	Serial.println("b9");
	Serial.println("RESULT: " + combinedResult);
	Serial.println("");

	_resultDecodedOregonMessage = combinedResult;
}

String OregonDecoder::GetHexValue(byte itemIndex)
{
	String hexStringValue = String(_oregonValuesToBuildFrom[itemIndex], HEX);

	Serial.print(itemIndex);
	Serial.print("=");
	Serial.println(hexStringValue);

	return hexStringValue;
}