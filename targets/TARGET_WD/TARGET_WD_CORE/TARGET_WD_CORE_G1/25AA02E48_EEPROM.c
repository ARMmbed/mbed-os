/*
 * 25AA02E48_EEPROM.h
 *
 * Created: 05.05.2015 09:31:06
 *  Author: patrick.frischmann
 */ 

#include <string.h>
#include "25AA02E48_EEPROM.h"
#include "hal/spi_api.h"
#include "PinNames.h"
#include "hal/gpio_api.h"

// EEPROM
#define INSTRUCTION_READ		0x03
#define NODE_ADDRESS_START		0xFA
#define DUMMY_DATA				0xFF

static uint8_t is_initialized = 0;
static gpio_t _cs;
static spi_t _spi;

void init(void) {
	gpio_init_out_ex(&_cs, SPI_CS2, 1);
	spi_init(&_spi, SPI_MOSI, SPI_MISO, SPI_SCK, NC);
	is_initialized = 1;
}

void select_chip(void) {
	gpio_write(&_cs, 0);
}

void deselect_chip(void) {
	gpio_write(&_cs, 1);
}

int eeprom_read(uint8_t address, uint8_t *buffer, uint8_t length) {
	
	if (!is_initialized) init();

	select_chip();
	if (-1 == spi_master_write(&_spi, INSTRUCTION_READ)) return -1;
	if (-1 == spi_master_write(&_spi, address)) return -1;
	for (int i = 0; i < length; i++) {
		int ret = spi_master_write(&_spi, DUMMY_DATA);
		if (ret < 0) return ret;
		
		*(buffer + i) = ret;
	}
	deselect_chip();
	
	return 0;
}

int eeprom_read_eui48_node_address(uint8_t *buffer) {
	return eeprom_read(NODE_ADDRESS_START, buffer, (size_t)(6U));
}

int eeprom_read_eui64_node_address(uint8_t *buffer) {
	uint8_t eui48[6];
	if(0 != eeprom_read_eui48_node_address(eui48)) return -1;
	
	// insert 0xFFFE between OUI and Extension Identifier
	uint64_t res =	(((uint64_t)eui48[0]) << 56) | 
					(((uint64_t)eui48[1]) << 48) | 
					(((uint64_t)eui48[2]) << 40) |
					(((uint64_t)(0xFF)) << 32) |
					(((uint64_t)(0xFE)) << 24) |
					(((uint64_t)eui48[3]) << 16) |
					(((uint64_t)eui48[4]) << 8) |
					((uint64_t)eui48[5]);
	memcpy(buffer, &res, sizeof(res));
	
	return 0;
}