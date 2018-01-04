#pragma once

#include "AnalogInManager.h"

#define PT_INVALID_VALUE				-999.99f
#define PT_VALUE_CHANGED_TOLERANCE		0.5f

class SensorPlatinumTemperature {
public:
	enum PTUnit {
		CELSIUS,
		FAHRENHEIT
	};
	
	enum PTType {
		PT100,
		PT1000
	};
	
	SensorPlatinumTemperature(AnalogInManager * analogInManager, int inputIndex, PTType ptType);
	~SensorPlatinumTemperature();
	
	float getMinRangeValue(void) { return _minRangeValue; };
	void setMinRangeValue(float value) { _minRangeValue = value; };
	float getMaxRangeValue(void) { return _maxRangeValue; };
	void setMaxRangeValue(float value) { _maxRangeValue = value; };
	PTUnit getUnit(void) { return _unit; };
	void setUnit(PTUnit value) { _unit = value; };
	PTType getType(void) { return _ptType; };
	
	float getValueChangedTolerance(void);
	void setValueChangedTolerance(float value = PT_VALUE_CHANGED_TOLERANCE);

	void attach(Callback<void(uint16_t)> func);
	void detach(void);

	float getMinMeasuredValue(void);
	float getMaxMeasuredValue(void);
	void resetMinAndMaxMeasuredValues(void);

	float getValue(void);
	
private:
	float adc2temperature(uint16_t adc);
	uint16_t temperature2adc(float temp);
	
	float _minRangeValue; // TODO
	float _maxRangeValue; // TODO
	PTUnit _unit;
	PTType _ptType;
	
	AnalogInManager * _analogInManager;
	int _inputIndex;
};

