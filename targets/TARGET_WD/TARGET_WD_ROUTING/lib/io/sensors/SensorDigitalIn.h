#ifndef SENSORDIGITALIN_H
#define SENSORDIGITALIN_H

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "mbed.h"
#include "PinNames.h"
#include "MeasurementBuffer.h"
#include "IOEventQueue.h"
#include "ResettableTimeout.h"

/******************************************************
___________________DECLARATION_________________________
******************************************************/
class SensorDigitalIn {
public:
	enum EdgeSelection {
        Falling = 1,
        Rising = 2,
		None	// Polling instead of interrupt
    };
	
	SensorDigitalIn(PinName pin, EdgeSelection edgeSelection = Rising, uint16_t instanceMetadata = 0);
	~SensorDigitalIn();
	
	void attach(mbed::Callback<void(uint16_t)> func);
	void detach(void);
	int getValue(void) { return _value; };
	int getCount(void) { return _edgeCounter; };
	void resetCount(void) { _edgeCounter = 0; };
		
	float getPulseDuration(void);
	int getPulseDurationFilterSize(void) { return _pulseDurationFilterSize; };
	void setPulseDurationFilterSize(int size);
	
private:
	void setValue(int value);
	void onSamplingTick(void);
	void onEdge(void);
	void onPulseDurationResetTimeout(void);
	
	DigitalIn * _din;
	EdgeSelection _edgeSelection;
	Callback<void(uint16_t)> _irq;
	uint16_t _instanceMetadata;
	int _value;
	int _edgeCounter;
	Timer * _pulseDurationTimer;
	ResettableTimeout * _pulseDurationResetTimout;
	MeasurementBuffer<int> * _pulseDurationBuffer;
	MeasurementBuffer<uint8_t> * _samplingBuffer;
	IOEventQueue * _eq;
	int _pulseDurationOffset;
	int _pulseDurationFilterSize;
};

#endif