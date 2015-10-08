#ifndef _OREGONRECEIVER_h
#define _OREGONRECEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class OregonReceiver
{

public:
	OregonReceiver(int pinId);
	String ReceiveOregon();

private:
	String _receivedOregonResult;
	byte _oregonPinId;
	byte _oregonCounter;
	bool* _readOregonValues;
	void ReceiveSignal();
	void ReadValues();
};