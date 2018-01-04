#ifndef ACTORDIGITALOUT_H
#define ACTORDIGITALOUT_H

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "mbed.h"
#include "PinNames.h"

/******************************************************
___________________DECLARATION_________________________
******************************************************/
class ActorDigitalOut {
public:
	
	ActorDigitalOut(PinName pin, int value = 0) : _digitalOut(pin, value), _value(value){};
	~ActorDigitalOut(){};
	
	int getValue(void) { return _value; };
	void setValue(int value);
	
private:
	DigitalOut _digitalOut;
	int _value;
	
};

#endif