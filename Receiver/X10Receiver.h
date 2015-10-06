// X10Receiver.h

#ifndef _X10RECEIVER_h
#define _X10RECEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class X10Receiver
{
private:
	byte _pinId;
	unsigned long _timing;
	int _counter;
	void ResetVariables();
	void ReadLows();
	byte GetValueFromReceivedValuesRange(byte firstIndex, byte lastIndex);
	String Decode();
public:
	X10Receiver(byte pinId);
	String Receive();
};