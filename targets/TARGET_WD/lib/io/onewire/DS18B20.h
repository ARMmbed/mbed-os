#pragma once

#include <map>
#include <iterator>
#include "mbed.h"
#include "OneWire.h"
#include "MeasurementBuffer.h"

#define DS18B20_INVALID_VALUE				-999.99
#define DS18B20_FAMILY_CODE					0x28
#define DS18B20_MEASUREMENT_BUFFER_SIZE		9
#define DS18B20_MEASUREMENT_INTERVAL_S		5

class DS18B20 {
public:
	
	DS18B20(OneWire * oneWire, uint measurementIntervalSeconds = DS18B20_MEASUREMENT_INTERVAL_S);
	~DS18B20();
	
	int getSensorCount(void) { return _sensorCount; };
	void getSensorIds(uint64_t * buffer);
	
	OW_STATUS_CODE enumerateSensors(void);
	
	float getValue(uint64_t id);
	
	void setMeasurementInterval(uint measurementIntervalSeconds);
	void attachSensorAddedCallback(Callback<void(uint64_t)> cb);
	void attachSensorRemovedCallback(Callback<void(uint64_t)> cb);
	
private:
	
	enum TemperatureResolution {
		Resolution_09_BIT = 0x1F,
		Resolution_10_BIT = 0x3F,
		Resolution_11_BIT = 0x5F,
		Resolution_12_BIT = 0x7F
	};
	
	OneWire * _oneWire;
	Callback<void(uint64_t)> _sensorAddedCallback;
	Callback<void(uint64_t)> _sensorRemovedCallback;
	
	Ticker * _ticker;
	
	typedef MeasurementBuffer<float> DS18B20MeasurementBuffer;
	map<uint64_t, DS18B20MeasurementBuffer> _mSensors;
	int _sensorCount;
	
	uint64_t transformId(char romId[]);
	void retrieveId(uint64_t inId, char * outId);
	OW_STATUS_CODE convertTemperature(void);
	OW_STATUS_CODE readMeasurements(void);
	OW_STATUS_CODE setTemperatureResolution(TemperatureResolution resolution = Resolution_10_BIT); 
	
	void collectMeasurement(void);
};

