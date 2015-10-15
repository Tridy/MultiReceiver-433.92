#include "OregonDecoder.h"



OregonDecoder::OregonDecoder(byte mock)
{

}

String OregonDecoder::DecodeOregonValues(bool* valuesToDecode, byte count1)
{
	_resultDecodedOregonMessage = "N-A";

	bool doubleShortAsOne = true;

	byte c = 1;
	byte d = 0;

	while (c < count1)
	{
		if (valuesToDecode[c])
		{
			doubleShortAsOne = !doubleShortAsOne;
			_decodedOregonValues[d] = doubleShortAsOne;
			c++;
		}
		else
		{
			_decodedOregonValues[d] = doubleShortAsOne;
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
	for (size_t i = 0; i < 22; i++)
	{
		_oregonValuesToBuildFrom[i] = 0;
	}
}

void OregonDecoder::ReverseOregonValues()
{
	_oregonValuesToBuildFrom[0] = 15;

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

		_oregonValuesToBuildFrom[i] = val;
	}
}

void OregonDecoder::BuildOregonResult()
{
	
	int checkSumResult = 0;

	int hh;
	for (hh = 0; hh < 16; hh++)
	{
		Serial.print(",");
		Serial.print(_oregonValuesToBuildFrom[hh]);
	}

	Serial.println("");

	int cs;
	for (cs = 0; cs < 15; cs++)
	{
		checkSumResult += _oregonValuesToBuildFrom[cs];
	}

	String sensorId = GetHexValue(0) + GetHexValue(1) + GetHexValue(2) + GetHexValue(3);
	Serial.print("sensorId: ");
	Serial.println(sensorId);
	String channel = GetHexValue(4);
	Serial.print("channel:");
	Serial.println(channel);
	String rollingCode = GetHexValue(5) + GetHexValue(6);
	Serial.print("rollingCode:");
	Serial.println(rollingCode);
	String lowBattery = _oregonValuesToBuildFrom[7] == 1 ? "Yes" : "No";
	Serial.print("lowBattery:");
	Serial.println(lowBattery);
	String temperature = (_oregonValuesToBuildFrom[11] == 0 ? "" : "-") + GetHexValue(10) + GetHexValue(9) + "." + GetHexValue(8);
	Serial.print("temperature:");
	Serial.println(temperature);
	String humidity = GetHexValue(13) + GetHexValue(12);
	Serial.print("humidity:");
	Serial.println(humidity);
	String checksum = GetHexValue(16) + GetHexValue(15);
	Serial.print("checksum:");
	Serial.println(checksum);

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

String OregonDecoder::GetHexValue(int itemIndex)
{
	int decimalValue = _oregonValuesToBuildFrom[itemIndex];
	Serial.print("DEC=");
	Serial.print(decimalValue);
	String blah = String(15, HEX);
	Serial.print(", [");
	Serial.print(itemIndex);
	Serial.print("] HEX VALUE: ");
	Serial.println(blah);
	return blah;
}