#pragma once

#include "mbed.h"
#include "PinNames.h"

#define COMMAND_WRITE_INPUT_REGISTER_N				0x0
#define COMMAND_UPDATE_DAC_REGISTER_N				0x1
#define COMMAND_WRITE_INPUT_REGISTER_N_UPDATE_ALL	0x2
#define COMMAND_WRITE_AND_UPDATE_DAC_REGISTER_N		0x3
#define COMMAND_POWER_DOWN_N						0x4
#define COMMAND_POWER_DOWN_CHIP						0x5
#define COMMAND_SELECT_INTERNAL_REFERENCE			0x6
#define COMMAND_SELECT_EXTERNAL_REFERENCE			0x7
#define COMMAND_NO_OPERATION						0xF

#define ADDRESS_DAC_A			0x0
#define ADDRESS_DAC_B			0x1
#define ADDRESS_DAC_ALL			0xF

#define V_REF	2.5f;

class AnalogOutManager {
public:
	AnalogOutManager(PinName spiMosi = NC, PinName spiSck = NC, PinName spiCs = NC);
	~AnalogOutManager();
	
	void setValue(int outputIndex, int value);
	
private:
	
	void write(uint8_t cmd, uint8_t addr, uint16_t data);
	
	DigitalOut _pinCs;
	SPI _spi;
};

