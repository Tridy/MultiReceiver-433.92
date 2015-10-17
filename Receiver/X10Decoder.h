#ifndef _X10DECODER_h
#define _X10DECODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class X10Decoder
{
private:
	byte GetValueFromReceivedValuesRange(byte firstIndex, byte lastIndex);
	void SeparateLows(bool* readings);
	String BuildResult();
	bool _x10Values[22];
public:
	X10Decoder(byte id);
	String Decode(bool* readings);
};