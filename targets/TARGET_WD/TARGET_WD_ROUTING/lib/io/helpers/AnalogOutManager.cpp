#include "AnalogOutManager.h"

#define LTC2632_DATA_SIZE_BYTES	3

AnalogOutManager::AnalogOutManager(PinName spiMosi, PinName spiSck, PinName spiCs) 
	: _spi(spiMosi, NC, spiSck), _pinCs(spiCs, 1) {}


AnalogOutManager::~AnalogOutManager() {}


/**
* \brief Set DAC value.
*
* \param cmd		Command for LTC2632-12 datagram (4 Bits).
* \param addr		Address for LTC2632-12 datagram (4 Bits).
* \param data		Data for LTC2632-12 datagram (12 Bits + 4 don't care Bits). Shifted within this function so don't care in outer scope.
*/
void AnalogOutManager::write(uint8_t cmd, uint8_t addr, uint16_t data) {
	
	this->_pinCs = 0;
	
	_spi.write((cmd << 4) | (addr << 0));
	_spi.write((uint8_t)(data >> 4));
	_spi.write((uint8_t)((data << 4) & 0xFF));
	
	this->_pinCs = 1;
	
}


void AnalogOutManager::setValue(int outputIndexIndex, int value) {
	
	// use internal reference
	this->write(COMMAND_SELECT_INTERNAL_REFERENCE, ADDRESS_DAC_ALL, 0x0000);
	
	this->write(COMMAND_WRITE_AND_UPDATE_DAC_REGISTER_N, (outputIndexIndex == 0 ? ADDRESS_DAC_A : ADDRESS_DAC_B), (uint16_t) value);
	
}