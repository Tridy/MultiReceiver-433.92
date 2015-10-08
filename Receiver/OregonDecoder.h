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
	void ReverseOregonValues();
	void BuildOregonResult();
	String GetHexValue(byte itemIndex);
};