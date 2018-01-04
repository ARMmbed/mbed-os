#include "SensorCurrentLoopIn.h"

SensorCurrentLoopIn::SensorCurrentLoopIn(AnalogInManager * analogInManager, int inputIndex)
	: _inputIndex(inputIndex), _minRangeValue(3.8f), _maxRangeValue(20.5f), _currentCalibrationCoefficient(0.3357f) {

	this->_analogInManager = analogInManager;
}

SensorCurrentLoopIn::~SensorCurrentLoopIn() {

	delete this->_analogInManager;
	this->_analogInManager = NULL;
	
}

void SensorCurrentLoopIn::start(void) {
	this->_analogInManager->start();
}

float SensorCurrentLoopIn::getValue(void) {
	
	float conversion_result = this->adc2mA(this->_analogInManager->getValue(this->_inputIndex));
	
	if (conversion_result < _minRangeValue || conversion_result > _maxRangeValue) {
		return CURRENT_LOOP_INVALID_VALUE;
	} else {
		return conversion_result;
	}
	
}

void SensorCurrentLoopIn::attach(Callback<void(uint16_t)> func) {
	
	if (func){
		this->_analogInManager->attach(this->_inputIndex, func);
	}
	
}

void SensorCurrentLoopIn::detach(void) {
	
	this->_analogInManager->detach(this->_inputIndex);
	
}

float SensorCurrentLoopIn::getMinMeasuredValue(void) {

	return this->adc2mA(this->_analogInManager->getMinValue(this->_inputIndex));

}

float SensorCurrentLoopIn::getMaxMeasuredValue(void) {

	return this->adc2mA(this->_analogInManager->getMaxValue(this->_inputIndex));

}

void SensorCurrentLoopIn::resetMinAndMaxMeasuredValues(void) {

	this->_analogInManager->resetMinAndMaxValues(this->_inputIndex);

}

float SensorCurrentLoopIn::getValueChangedTolerance(void) {
	
	return this->adc2mA(this->_analogInManager->getValueChangedTolerance(this->_inputIndex));
	
}

void SensorCurrentLoopIn::setValueChangedTolerance(float value) {
	
	this->_analogInManager->setValueChangedTolerance(this->_inputIndex, this->mA2adc(value));
	
}

float SensorCurrentLoopIn::getMAD(void) {
	
	return this->adc2mA(this->_analogInManager->getMAD(this->_inputIndex));
	
}


float SensorCurrentLoopIn::adc2mA(int adc) {
	
	return ((float)adc) * this->_currentCalibrationCoefficient / 1000.0f;
	
}


int SensorCurrentLoopIn::mA2adc(float mA) {
	
	return (int)(mA * 1000.0f / this->_currentCalibrationCoefficient); 
	
}