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
	String Decode(bool *readValues);

private:
	String _resultDecodedOregonMessage;
	bool _oregonValues[90];
	int _oregonValuesToBuildFrom[22];
	byte _countToReverse;

	void ClearOregonValues();
	void ReverseOregonValues();
	void BuildOregonResult();
	String GetHexValue(int itemIndex);
};