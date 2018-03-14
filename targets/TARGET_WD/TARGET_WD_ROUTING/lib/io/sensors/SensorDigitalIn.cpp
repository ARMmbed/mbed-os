/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "SensorDigitalIn.h"
#include "wd_logging.h"

/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define DEFAULT_PULSE_DURATION_FILTER_SIZE		9
#define PULSE_DURATION_RESET_TIMEOUT_FACTOR		3
#define SAMPLING_BUFFER_FILTER_SIZE				3
#define SAMPLING_INTERVAL_MS					15

/******************************************************
___________________IMPLEMENTATION______________________
******************************************************/
static void donothing(uint16_t instanceId) {}

SensorDigitalIn::SensorDigitalIn(PinName pin, EdgeSelection edgeSelection, uint16_t instanceMetadata)
	: _instanceMetadata(instanceMetadata), 
    _eq(&IOEventQueue::getInstance()), 
	_edgeSelection(edgeSelection),
	_value(0), 
	_pulseDurationOffset(0),
	_pulseDurationFilterSize(DEFAULT_PULSE_DURATION_FILTER_SIZE),
	_edgeCounter(0) {
	
	_irq = donothing;

	// set initial value
	_din = new DigitalIn(pin);
	this->setValue(_din->read());

	// prepare buffers
	this->_pulseDurationBuffer = new MeasurementBuffer<int>(_pulseDurationFilterSize);
	this->_pulseDurationBuffer->clear();
	_samplingBuffer = new MeasurementBuffer<uint8_t>(SAMPLING_BUFFER_FILTER_SIZE);
	_samplingBuffer->clear();

	// schedule sampling
	_eq->call_every(SAMPLING_INTERVAL_MS, this, &SensorDigitalIn::onSamplingTick);

	// prepare timers
	_pulseDurationTimer = new Timer();
	this->_pulseDurationTimer->start();
	events::EventQueue * eq = mbed_event_queue();
	this->_pulseDurationResetTimout = new ResettableTimeout(eq->event(callback(this, &SensorDigitalIn::onPulseDurationResetTimeout)), 5000000); // 5 seconds default timeout

}

SensorDigitalIn::~SensorDigitalIn(){}

void SensorDigitalIn::onEdge() {
	
	this->_edgeCounter ++;
	int durationUs = this->_pulseDurationTimer->read_us();
	this->_pulseDurationBuffer->add(durationUs);
	this->_pulseDurationTimer->reset();
	timestamp_t resetTimeout = durationUs * PULSE_DURATION_RESET_TIMEOUT_FACTOR;
	if (resetTimeout < 10000000) resetTimeout = 10000000;	// limit to 10 sec min.
	if (resetTimeout > 60000000) resetTimeout = 60000000;	// limit to 60 sec max.
	this->_pulseDurationResetTimout->reset(resetTimeout);

}

void SensorDigitalIn::onSamplingTick(void) {
	
	_samplingBuffer->add((uint8_t)_din->read());
	
	int value = _samplingBuffer->sum() > (SAMPLING_BUFFER_FILTER_SIZE/2) ? 1 : 0;
	if ((value > _value && _edgeSelection == Rising) ||
		(value < _value && _edgeSelection == Falling)) {
		onEdge();
	}

	this->setValue(value);
	
}

void SensorDigitalIn::onPulseDurationResetTimeout(void) {
	
	this->_pulseDurationBuffer->clear();

}

void SensorDigitalIn::attach(mbed::Callback<void(uint16_t)> func) {
	
	if (func){
		events::EventQueue * eq = mbed_event_queue();
		_irq = eq->event(func);
	} else {
		_irq = donothing;
	}
	
}

void SensorDigitalIn::detach(void) {
	
	_irq = donothing;
	
}

float SensorDigitalIn::getPulseDuration(void) {
	return ((float) this->_pulseDurationBuffer->get()) / 1000000.0f;
}

void SensorDigitalIn::setPulseDurationFilterSize(int size) {
	this->_pulseDurationFilterSize = size;
	delete this->_pulseDurationBuffer;
	this->_pulseDurationBuffer = new MeasurementBuffer<int>(_pulseDurationFilterSize);
	this->_pulseDurationBuffer->clear();
}

void SensorDigitalIn::setValue(int value) {
	
	int prev = this->_value;
	this->_value = value;
	
	if (value != prev) _irq.call(this->_instanceMetadata);
	
}