#include "OregonDecoder.h"

OregonDecoder::OregonDecoder()
{
	
}

String OregonDecoder::DecodeValues(bool readValues[], byte count)
{
	_counter = count;

	bool doubleShortAsOne = true;

	byte c = 1;
	byte d = 0;

	Serial.println("DECODING-");

	while (c < _counter)
	{
		Serial.print("d");

		if (readValues[c])
		{
			doubleShortAsOne = !doubleShortAsOne;
			_decodedValues[d] = doubleShortAsOne;
			c++;
		}
		else
		{
			_decodedValues[d] = doubleShortAsOne;
			c += 2;
		}

		d++;
	}

	Serial.println("");


	ReverseValues();
	BuildResult();

	Serial.println("DECODE: " + _result);

	return _result;
}

void OregonDecoder::ReverseValues()
{
	Serial.println("REVERSING-");

	_reversedValues[0] = 15;

	int i;

	for (i = 1; i < 20; i++)
	{
		Serial.print("r");

		byte val = 0;

		int j;

		for (j = 3; j >= 0; j = j - 1)
		{
			bool bit = _decodedValues[(i * 4) + j];
			val = val ^ bit;

			if (j > 0)
			{
				val = val << 1;
			}
		}

		_reversedValues[i] = val;

		Serial.println("i:" + String((int)i));
		Serial.println("i Value:" + String((int)val));
	}

	Serial.println("");

}

void OregonDecoder::BuildResult()
{
	Serial.println("BUILDING-");


	int cs;
	byte checkSumResult = 0;

	for (cs = 0; cs < 15; cs++)
	{
		Serial.print("s");
		checkSumResult += _reversedValues[cs];
	}

	Serial.println("");

	String sensorId = GetHexValue(0) + GetHexValue(1) + GetHexValue(2) + GetHexValue(3);
	Serial.println("sensorId: " + sensorId);
	String channel = GetHexValue(4);
	Serial.println("channel:" + channel);
	String rollingCode = GetHexValue(5) + GetHexValue(6);
	Serial.println("rollingCode:" + rollingCode);
	String lowBattery = _reversedValues[7] == 1 ? "Yes" : "No";
	Serial.println("lowBattery:" + lowBattery);
	String temperature = (_reversedValues[11] == 0 ? "" : "-") + GetHexValue(10) + GetHexValue(9) + "." + GetHexValue(8);
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
	_result = combinedResult;
}

String OregonDecoder::GetHexValue(byte index)
{
	return String(_reversedValues[index], HEX);
}