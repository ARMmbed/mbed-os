#include "AnalogInManager.h"
#include <cmath>

#define ADC_MAX_VALUE	0xFFFF

static void donothing(uint16_t instanceId) {}

AnalogInManager::AnalogInManager(int inputCount, PinName muxSel0, PinName muxSel1, PinName muxSel2, PinName spiMiso, PinName spiSck, PinName spiCs)
	:	_inputCount(inputCount), _currentInputSelection(1), _currentValue(0), _valueChangedTolerance(0), _minValue(0), _maxValue(0),
		_muxSel0(muxSel0, 0), _muxSel1(muxSel1, 0), _muxSel2(muxSel2, 0), 
		_pinCs(spiCs, 1), _spi(NC, spiMiso, spiSck) {
	
	// allocate memory for dynamic buffers
	this->_measurementBuffers = new AINMeasurementBuffer[inputCount](AIN_MEASUREMENT_BUFFER_SIZE);
	this->_currentValue = new uint16_t[inputCount]();
	this->_minValue = new uint16_t[inputCount]();
	this->_maxValue = new uint16_t[inputCount]();
	this->_valueChangedTolerance = new int[inputCount]();
	this->_irq = new Callback<void(uint16_t)>[inputCount]();
		
	for (int i = 0; i < inputCount; i++) {
        this->_irq[i] = donothing;
		this->_valueChangedTolerance[i] = AIN_DEFAULT_VALUE_CHANGED_TOLERANCE;
		this->_minValue[i] = ADC_MAX_VALUE;
		this->_maxValue[i] = 0;
    }
		
}

void AnalogInManager::start(void) {
	this->read(); // initiate first conversion
	
	// start measurement-thread
	this->_measurementThread.start(mbed::Callback<void()>(this, &AnalogInManager::measurement_entry));
}

AnalogInManager::~AnalogInManager() {
	
	// free allocated memory for dynamic buffers
	delete [] this->_measurementBuffers;
	delete [] this->_currentValue;
	delete [] this->_minValue;
	delete [] this->_maxValue;
	delete [] this->_valueChangedTolerance;
	delete [] this->_irq;
	_measurementThread.terminate();
}


void AnalogInManager::attach(int inputIndex, Callback<void(uint16_t)> func) {
	
	if (inputIndex < 0 || inputIndex >= this->_inputCount) return;
	
	if (func){
		this->_irq[inputIndex] = func;
	} else {
		this->_irq[inputIndex] = donothing;
	}
	
}


void AnalogInManager::detach(int inputIndex) {
	
	if (inputIndex < 0 || inputIndex >= this->_inputCount) return;

	this->_irq[inputIndex] = donothing;
	
}


int AnalogInManager::getValue(int inputIndex) {
	
	if (inputIndex < 0 || inputIndex >= this->_inputCount) return 0;
	
	return this->_currentValue[inputIndex];
	
}


void AnalogInManager::selectNextInput(void) {
	
	_muxSel0 = !_muxSel0;
	
	if (_currentInputSelection % 2 == 0) _muxSel1 = !_muxSel1;
	
	if (_currentInputSelection % 4 == 0) _muxSel2 = !_muxSel2;
	
	if (++_currentInputSelection > this->_inputCount) { // reset
		_currentInputSelection = 1;
		_muxSel0 = 0;
		_muxSel1 = 0;
		_muxSel2 = 0;
	}
	
	wd_log_debug("AIN MUX select %d %d %d -> input %d", _muxSel2.read(), _muxSel1.read(), _muxSel0.read(), _currentInputSelection);
	
}


uint16_t AnalogInManager::read(void) {
	
	this->_pinCs = 0;
	
	int hb = _spi.write(0xFF);
	int lb = _spi.write(0xFF);
	
	this->_pinCs = 1;
		
	return (((uint16_t) hb) << 8) | ((uint16_t) lb);
	
}


void AnalogInManager::measurement_entry(void) {

	while(true){
	
		if (!_mutex.trylock()) {
			continue;
		}
		
		this->measurement_step();
	
		_mutex.unlock();
	
		Thread::wait(AIN_MEASUREMENT_INTERVAL_MS);
	
	}

}

void AnalogInManager::measurement_step(void) {
	
	// save current input selection (adc should already hold result of input value conversion, i.e. value of previously selected input)
	int inputIndex = _currentInputSelection-1;	
	
	// already select next input as the adc immediately enters the capture state after a read operation
	this->selectNextInput();
	
	// wait for propagation delay
	//	wait_ms(1);	// time is below 2 ns so we can savely skip this step
	
	// read last captured value and enter capture state afterwards
	uint16_t adc_val = this->read();
	_measurementBuffers[inputIndex].add(adc_val);
	wd_log_debug("AIN read -> input %d, value %d", inputIndex, adc_val);
	
	// detect value change
	uint16_t previousValue = this->_currentValue[inputIndex];
	this->_currentValue[inputIndex] = this->_measurementBuffers[inputIndex].get();
	
	if (this->_currentValue[inputIndex] > this->_maxValue[inputIndex])
		this->_maxValue[inputIndex] = this->_currentValue[inputIndex];
	
	if (this->_currentValue[inputIndex] < this->_minValue[inputIndex])
		this->_minValue[inputIndex] = this->_currentValue[inputIndex];
	
	if (abs(previousValue - this->_currentValue[inputIndex]) > this->_valueChangedTolerance[inputIndex]){
		events::EventQueue * eq = mbed_highprio_event_queue();
		eq->call(this->_irq[inputIndex], inputIndex);
	}
	
}

int AnalogInManager::getMAD(int inputIndex) {
	
	if (inputIndex < 0 || inputIndex >= this->_inputCount) return 0;
	
	return this->_measurementBuffers[inputIndex].MAD();
	
}

void AnalogInManager::resetMinAndMaxValues(int inputIndex) {
	
	if (inputIndex < 0 || inputIndex >= this->_inputCount) return;

	this->_minValue[inputIndex] = ADC_MAX_VALUE;
	this->_maxValue[inputIndex] = 0;

}
