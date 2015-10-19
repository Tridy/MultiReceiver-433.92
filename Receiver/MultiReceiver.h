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
	void RearrangeReadings();
	void GetHighAndLowWithinLengthRanges();
	String ReceiveData();
	byte SimplifyByteValue(byte originalValue);
	bool IsSignalOregon(size_t positionIndex);
	bool IsSignalX10(size_t positionIndex);
	void ExtractSignalReadings(size_t positionIndex);
	void ClearExtractedValues();
public:
	MultiReceiver(byte pinId);
	String ReceiveMulti();
};
