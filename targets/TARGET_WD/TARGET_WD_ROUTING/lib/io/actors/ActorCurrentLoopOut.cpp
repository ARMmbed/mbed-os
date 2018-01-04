#include "ActorCurrentLoopOut.h"


ActorCurrentLoopOut::ActorCurrentLoopOut(AnalogOutManager * analogOutManager, int outputIndex) 
	: _outputIndex(outputIndex), _minRangeValue(3.8f), _maxRangeValue(20.5f), _currentCalibrationCoefficient(5.549f), _value(3.8f) {

	this->_analogOutManager = analogOutManager;
}


ActorCurrentLoopOut::~ActorCurrentLoopOut() {}


/**
* \brief Set value.
*
* \param value		Value in 4 - 20 mA.
*/
void ActorCurrentLoopOut::setValue(float value) {
	
	if (value < _minRangeValue) {
		value = _minRangeValue;
	} else if (value > _maxRangeValue) {
		value = _maxRangeValue;
	}
	
	this->_value = value;
		
	float dac_value = (value * 1000.0f) / this->_currentCalibrationCoefficient;
		
	_analogOutManager->setValue(this->_outputIndex, (int)dac_value);
	
}