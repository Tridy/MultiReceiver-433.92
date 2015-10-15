#ifndef _OREGONDECODER_h
#define _OREGONDECODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class OregonDecoder
{
public:
	OregonDecoder(byte mock);
	String DecodeOregonValues(bool *readValues, byte count1);

private:
	String _resultDecodedOregonMessage;
	byte _decodedOregonValues[84];
	int _oregonValuesToBuildFrom[22];
	byte _countToReverse;
	void ClearOregonValues();
	void ReverseOregonValues();
	void BuildOregonResult();
	String GetHexValue(int itemIndex);
};