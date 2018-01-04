#pragma once

#include "AnalogOutManager.h"

class ActorCurrentLoopOut {
public:
	ActorCurrentLoopOut(AnalogOutManager * analogOutManager, int outputIndex);
	~ActorCurrentLoopOut();
	
	float getMinRangeValue(void) { return _minRangeValue; };
	void setMinRangeValue(float value) { _minRangeValue = value; };
	float getMaxRangeValue(void) { return _maxRangeValue; };
	void setMaxRangeValue(float value) { _maxRangeValue = value; };
	float getCurrentCalibrationCoefficient(void) { return _currentCalibrationCoefficient; };
	void setCurrentCalibrationCoefficient(float value) { _currentCalibrationCoefficient = value; };
	
	float getValue(void) { return _value; };
	void setValue(float value);
	
private:
	
	float _minRangeValue;
	float _maxRangeValue;
	float _currentCalibrationCoefficient;

	AnalogOutManager * _analogOutManager;
	int _outputIndex;
	
	float _value;
	
};

