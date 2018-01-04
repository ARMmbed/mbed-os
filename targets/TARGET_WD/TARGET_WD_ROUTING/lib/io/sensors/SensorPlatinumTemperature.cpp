#include "SensorPlatinumTemperature.h"
#include <cmath>

SensorPlatinumTemperature::SensorPlatinumTemperature(AnalogInManager * analogInManager, int inputIndex, PTType ptType)
	: _inputIndex(inputIndex), _minRangeValue(-50.0f), _maxRangeValue(150.0f), _unit(SensorPlatinumTemperature::CELSIUS), _ptType(ptType) {

	this->_analogInManager = analogInManager;
	this->setValueChangedTolerance();
}

SensorPlatinumTemperature::~SensorPlatinumTemperature() {

	delete this->_analogInManager;
	this->_analogInManager = NULL;
	
}

float SensorPlatinumTemperature::adc2temperature(uint16_t adc) {
	
	uint64_t rv = this->_ptType == SensorPlatinumTemperature::PT1000 ? 2700 : 3300;
	
	int32_t resistance = (uint64_t)((uint64_t)(rv * (uint64_t)adc) / (uint64_t)((uint64_t)0x10000 - (uint64_t)adc));
	resistance *=  512;
	
	int64_t tx100 = (((int64_t)13300 * resistance) - (int64_t)6809600000) / 0x40000;
	
	float result = (float)tx100 / 100.0f;
	
	if (this->_ptType == SensorPlatinumTemperature::PT100) {
		result /= 19.0f;
	}
	
	if (this->_unit == SensorPlatinumTemperature::FAHRENHEIT) {
		result = ((9.0f / 5.0f) * result) + 32.0f;
	}
	
	return result;
	
}

uint16_t SensorPlatinumTemperature::temperature2adc(float temp) {

	int64_t resistance = ((int64_t)(262144 * temp) + (int64_t)68096000) / 133;
	int64_t rv = this->_ptType == SensorPlatinumTemperature::PT1000 ? 2700 : 3300;
	uint16_t adc = ((int64_t)(65536 * resistance)) / ((int64_t)512 * rv + resistance);
	return adc;

}

float SensorPlatinumTemperature::getValueChangedTolerance(void) {

	return this->adc2temperature(
		this->temperature2adc(0) +
		this->_analogInManager->getValueChangedTolerance(this->_inputIndex)
	);

}

void SensorPlatinumTemperature::setValueChangedTolerance(float value /*= PT_VALUE_CHANGED_TOLERANCE*/) {

	uint16_t adc_temp_zero = this->temperature2adc(0);
	uint16_t adc_temp = this->temperature2adc(abs(value));

	this->_analogInManager->setValueChangedTolerance(this->_inputIndex, adc_temp-adc_temp_zero);
	
}

void SensorPlatinumTemperature::attach(Callback<void(uint16_t)> func) {

	if (func) {
		this->_analogInManager->attach(this->_inputIndex, func);
	}

}

void SensorPlatinumTemperature::detach(void) {
	
	this->_analogInManager->detach(this->_inputIndex);

}

float SensorPlatinumTemperature::getMinMeasuredValue(void) {

	return this->adc2temperature(this->_analogInManager->getMinValue(this->_inputIndex));

}

float SensorPlatinumTemperature::getMaxMeasuredValue(void) {

	return this->adc2temperature(this->_analogInManager->getMaxValue(this->_inputIndex));

}

void SensorPlatinumTemperature::resetMinAndMaxMeasuredValues(void) {

	this->_analogInManager->resetMinAndMaxValues(this->_inputIndex);

}

float SensorPlatinumTemperature::getValue(void) {
	
	float conversion_result = this->adc2temperature(this->_analogInManager->getValue(this->_inputIndex));
	
	if (conversion_result < this->_minRangeValue || conversion_result > this->_maxRangeValue) {
		return PT_INVALID_VALUE;
	} else {
		return conversion_result;
	}
	
}