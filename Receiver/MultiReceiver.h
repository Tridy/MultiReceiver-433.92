// MultiReceiver.h

#ifndef _MULTIRECEIVER_h
#define _MULTIRECEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class MultiReceiver
{
private:
	byte _pinId;
	unsigned long _highLength;
	unsigned long _previousLowLength;
	byte _signalIntroType;
	void ResetVariables();
	void WaitForLongLow();
	void GetHighAndLowWithinLengthRanges();
	String ReceiveData();
public:
	MultiReceiver(byte pinId);
	String Receive();
};