#pragma once

#include "AnalogInManager.h"

#define CURRENT_LOOP_INVALID_VALUE	0

class SensorCurrentLoopIn {
public:
	SensorCurrentLoopIn(AnalogInManager * analogInManager, int inputIndex);
	~SensorCurrentLoopIn();
	void start(void);
	
	float getMinRangeValue(void) { return _minRangeValue; };
	void setMinRangeValue(float value) { _minRangeValue = value; };
	float getMaxRangeValue(void) { return _maxRangeValue; };
	void setMaxRangeValue(float value) { _maxRangeValue = value; };
	float getCurrentCalibrationCoefficient(void) { return _currentCalibrationCoefficient; };
	void setCurrentCalibrationCoefficient(float value) { _currentCalibrationCoefficient = value; };
	
	float getMinMeasuredValue(void);
	float getMaxMeasuredValue(void);
	void resetMinAndMaxMeasuredValues(void);
	
	// tolerance in respect to mA value
	float getValueChangedTolerance(void);
	void setValueChangedTolerance(float value);
	
	void attach(Callback<void(uint16_t)> func);
	void detach(void);
	
	float getValue(void);
	
	float getMAD(void);
	
private:
	float _minRangeValue;
	float _maxRangeValue;
	float _currentCalibrationCoefficient;
	
	AnalogInManager * _analogInManager;
	int _inputIndex;
	
	float adc2mA(int adc);
	int mA2adc(float mA);
	
};

