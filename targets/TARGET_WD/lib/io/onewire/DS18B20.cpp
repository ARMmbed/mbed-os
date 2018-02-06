#include "DS18B20.h"
#include <utility>
#include "wd_logging.h"

#define DS18B20_POWER_PARASITE	0x00
#define DS18B20_POWER_EXTERN	0x01

#define DS18B20_COMMAND_CONVERT_T				0x44
#define DS18B20_COMMAND_WRITE_SCRATCHPAD		0x4E
#define DS18B20_COMMAND_READ_SCRATCHPAD			0xBE
#define DS18B20_COMMAND_COPY_SCRATCHPAD			0x48
#define DS18B20_COMMAND_RECALL_E2				0xB8

#define DS18B20_COPYSP_DELAY    10 /* ms */
#define DS18B20_SP_SIZE			8	// size without crc
#define DS18B20_TH_REG			2
#define DS18B20_TL_REG			3
#define DS18B20_RES_REG			4

// conversion times in ms
#define DS18B20_TCONV_12BIT     750
#define DS18B20_TCONV_11BIT     DS18B20_TCONV_12_BIT/2
#define DS18B20_TCONV_10BIT     DS18B20_TCONV_12_BIT/4
#define DS18B20_TCONV_9BIT      DS18B20_TCONV_12_BIT/8

#define NUM_RETRIES				3

static void donothing(uint64_t id) {}

DS18B20::DS18B20(OneWire * oneWire, uint measurementIntervalSeconds)
	: _sensorCount(0), _sensorAddedCallback(donothing), _sensorRemovedCallback(donothing) {
	
	this->_oneWire = oneWire;
	_ticker = new Ticker();
	events::EventQueue * eq = mbed_event_queue();
	this->_ticker->attach(eq->event(callback(this, &DS18B20::collectMeasurement)), (float)(measurementIntervalSeconds));	
		
}


DS18B20::~DS18B20() {}

OW_STATUS_CODE DS18B20::enumerateSensors(void) {
	
	char sensorIds[OW_MAXSENSORS][OW_ROMCODE_SIZE] = {0};
	int retry_count = NUM_RETRIES;
	
	// start by searching for sensors
	OW_STATUS_CODE res;
	do {
		res = this->_oneWire->ow_search_sensors(&this->_sensorCount, &sensorIds[0][0], DS18B20_FAMILY_CODE);
		if (res == OW_OK && this->_sensorCount > 0) break;
		wait_ms(5);
	} while (--retry_count > 0);
	
	// return if search was unsuccessful
	if (res != OW_OK) return res;
	
	// clear our sensor map and queue notification(s) if no sensors were found
	if (this->_sensorCount == 0) {
		
		for(map<uint64_t, DS18B20MeasurementBuffer>::const_iterator it = this->_mSensors.begin(); it != this->_mSensors.end(); it++) {
			this->_sensorRemovedCallback.call(it->first);
		}
		
		this->_mSensors.clear();
		return OW_OK;
	}
	
	/* sync detected sensors with map */
	
	// remove sensors no longer available on OneWire bus
	for(map<uint64_t, DS18B20MeasurementBuffer>::const_iterator it = this->_mSensors.begin(); it != this->_mSensors.end(); it++) {
		
		bool stillPresent = false;
		for (int i = 0; i<this->_sensorCount; i++){
			if (it->first == this->transformId(sensorIds[i])) {
				stillPresent = true;
				break;
			}
		}
		
		if (!stillPresent) {
			this->_sensorRemovedCallback.call(it->first);
			this->_mSensors.erase(it->first);
		}
	}
	
	// iterate through all detected sensors and sync with map
	for (int i = 0; i<this->_sensorCount; i++){
		
		uint64_t id = this->transformId(sensorIds[i]);
		
		// add sensor if it is not yet in sensor map
		if(this->_mSensors.find(id) == this->_mSensors.end()) {
			
			this->_mSensors.insert(make_pair<uint64_t, DS18B20MeasurementBuffer>(uint64_t(id), DS18B20MeasurementBuffer(DS18B20_MEASUREMENT_BUFFER_SIZE)));
			this->_sensorAddedCallback.call(id);
		}
		
	}
	
	return OW_OK;
		
}

float DS18B20::getValue(uint64_t id) {
	
	if(this->_mSensors.find(id) == this->_mSensors.end()) {
		return DS18B20_INVALID_VALUE;
	} else {
		return this->_mSensors[id].get();
	}
	
}

void DS18B20::setMeasurementInterval(uint measurementIntervalSeconds) {
	this->_ticker->detach();
	this->_ticker->attach(callback(this, &DS18B20::collectMeasurement), (float)(measurementIntervalSeconds));
}

void DS18B20::attachSensorAddedCallback(Callback<void(uint64_t)> cb) {
	
	if (cb){
		events::EventQueue * eq = mbed_event_queue();
		this->_sensorAddedCallback= eq->event(cb);
	} else {
		this->_sensorAddedCallback = donothing;
	}
	
}

void DS18B20::attachSensorRemovedCallback(Callback<void(uint64_t)> cb) {
	
	if (cb){
		events::EventQueue * eq = mbed_event_queue();
		this->_sensorRemovedCallback = eq->event(cb);
	} else {
		this->_sensorRemovedCallback = donothing;
	}
	
}

OW_STATUS_CODE DS18B20::convertTemperature(void) {
	// Remark: The power-on reset value of the temperature register is +85C.
	
	if (this->_oneWire->ow_reset() == OW_OK) {
		this->_oneWire->ow_command(DS18B20_COMMAND_CONVERT_T, NULL);
		// 1-Wire bus must be switched to the strong pullup within 10us (max) after a Convert T
		this->_oneWire->ow_enable_strong_pullup();
		wait_ms(DS18B20_TCONV_12BIT); // todo timer with callback?
		this->_oneWire->ow_disable_strong_pullup();
		return OW_OK;
		
	} else { // no device present
		return OW_ERROR_NO_DEVICE;
	}
	
}

OW_STATUS_CODE DS18B20::readMeasurements(void) {
	
	for(map<uint64_t, DS18B20MeasurementBuffer>::const_iterator it = this->_mSensors.begin(); it != this->_mSensors.end(); it++) {
		
		char sp[DS18B20_SP_SIZE] = {};
		int reading;
		float result;
		char id[OW_ROMCODE_SIZE] = {};
		
		this->retrieveId(it->first, id);
		
		this->_oneWire->ow_reset();
		this->_oneWire->ow_command(DS18B20_COMMAND_READ_SCRATCHPAD, id);
		if (this->_oneWire->ow_read_bytes_with_crc_8(&sp[0], DS18B20_SP_SIZE) != OW_OK) {
			(it->second).add(DS18B20_INVALID_VALUE);
		} else {
			reading = (sp[1] << 8) + sp[0];
	
			if (reading & 0x8000) { // negative degrees C
				reading = 0-((reading ^ 0xffff) + 1); // 2's comp then convert to signed int
			}
	
			result = (float)reading / 16.0f;
			
			(it->second).add(result);
		}
	}
	
}

void DS18B20::collectMeasurement(void) {
	
	this->convertTemperature();
	this->readMeasurements();
	
}

uint64_t DS18B20::transformId(char romId[]) {
	
	return	(((uint64_t)romId[0]) << 56) | 
			(((uint64_t)romId[1]) << 48) | 
			(((uint64_t)romId[2]) << 40) |
			(((uint64_t)romId[3]) << 32) |
			(((uint64_t)romId[4]) << 24) |
			(((uint64_t)romId[5]) << 16) |
			(((uint64_t)romId[6]) << 8) |
			((uint64_t)romId[7]);
	
}

void DS18B20::retrieveId(uint64_t inId, char * outId) {
	
	for (int i=0; i<OW_ROMCODE_SIZE; i++) {
		outId[i] = (char) (inId >> (8 * (OW_ROMCODE_SIZE-1-i)));
	}
	
}

OW_STATUS_CODE DS18B20::setTemperatureResolution(TemperatureResolution resolution) {
	
	for(map<uint64_t, DS18B20MeasurementBuffer>::const_iterator it = this->_mSensors.begin(); it != this->_mSensors.end(); it++) {
		
		char sp[DS18B20_SP_SIZE] = {};
		char id[OW_ROMCODE_SIZE] = {};
		OW_STATUS_CODE res;
		
		this->retrieveId(it->first, id);
		
		// get current T_h and T_l values
		int retry_count = NUM_RETRIES;
		do {
			this->_oneWire->ow_reset();
			this->_oneWire->ow_command(DS18B20_COMMAND_READ_SCRATCHPAD, id);
			res = this->_oneWire->ow_read_bytes_with_crc_8(sp, DS18B20_SP_SIZE);
			if (res == OW_OK) break;
			wait_ms(5);
		} while (--retry_count > 0);
		if (res != OW_OK) return res;
		
		// skip if current setting matches desired resolution
		if (sp[DS18B20_RES_REG] == resolution) continue;
		
		// else write scratchpad values
		char t_h = sp[DS18B20_TH_REG];
		char t_l = sp[DS18B20_TL_REG];
		
		this->_oneWire->ow_reset();
		this->_oneWire->ow_command(DS18B20_COMMAND_WRITE_SCRATCHPAD, id);
		this->_oneWire->ow_write_byte(t_h);
		this->_oneWire->ow_write_byte(t_l);
		this->_oneWire->ow_write_byte(resolution);
		
		// verify configuration register changes
		retry_count = NUM_RETRIES;
		do {
			this->_oneWire->ow_reset();
			this->_oneWire->ow_command(DS18B20_COMMAND_READ_SCRATCHPAD, id);
			res = this->_oneWire->ow_read_bytes_with_crc_8(sp, DS18B20_SP_SIZE);
			if (res == OW_OK) break;
			wait_ms(5);
		} while (--retry_count > 0);
		if (res != OW_OK) return res;
		
		if (sp[DS18B20_RES_REG] != resolution) return OW_ERROR;
	}
	
	return OW_OK;
}

void DS18B20::getSensorIds(uint64_t * buffer) {
	
	for(map<uint64_t, DS18B20MeasurementBuffer>::const_iterator it = this->_mSensors.begin(); it != this->_mSensors.end(); it++) {
	
		(*buffer++) = it->first;
		
	}
	
}