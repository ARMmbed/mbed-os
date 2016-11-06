#include "AT30TSE75X.h"

AT30TSE75X::AT30TSE75X(PinName sda, PinName scl) : at30tse75x_i2c(sda, scl), resolution(AT30TSE_CONFIG_RES_9_bit) {
    at30tse75x_i2c.frequency(10000);
}
AT30TSE75X::~AT30TSE75X() { }

float AT30TSE75X::read_temp() {
	uint8_t bufferwrite[1];
	uint8_t buffer[2];
	float temperature;
	uint16_t data;
	buffer[0] = 0;
	buffer[1] = 0;

	at30tse75x_i2c.read((AT30TSE_TEMPERATURE_TWI_ADDR << 1 ),(char *)buffer,AT30TSE_TEMPERATURE_REG_SIZE,false);


	data = (buffer[0] << 8) | buffer[1];
	int8_t sign = 1;

	/* Check if negative and clear sign bit. */
	if (data & (1 << 15)) {
		sign *= -1;
		data &= ~(1 << 15);
	}

	/* Convert to temperature. */
	switch (this->resolution) {
	case AT30TSE_CONFIG_RES_9_bit:
		data = (data >> 7);
		(temperature) = data * sign * 0.5;
		break;

	case AT30TSE_CONFIG_RES_10_bit:
		data = (data >> 6);
		(temperature) = data * sign * 0.25;
		break;

	case AT30TSE_CONFIG_RES_11_bit:
		data = (data >> 5);
		(temperature) = data * sign * 0.125;
		break;

	case AT30TSE_CONFIG_RES_12_bit:
		data = (data >> 4);
		(temperature) = data * sign * 0.0625;
		break;

	default:
		break;
	}
	return temperature;
}

uint8_t AT30TSE75X:: read_eeprom(uint8_t *data, uint8_t length, uint8_t word_addr, uint8_t page) {	
	uint8_t buff[20];
	buff[0]  = (word_addr & 0x0F) | ((0x0F & page) << 4);
		
	at30tse75x_i2c.write((int)(AT30TSE_EEPROM_TWI_ADDR << 1),(char *)buff,1,false);
	return at30tse75x_i2c.read((int)(AT30TSE_EEPROM_TWI_ADDR << 1),(char *)data,length,false);	
}

uint8_t AT30TSE75X:: write_eeprom(uint8_t *data, uint8_t length, uint8_t word_addr, uint8_t page)
{
	uint8_t buff[length+1];;
	buff[0] = (word_addr & 0x0F) | ((0x0F & page) << 4);
	for(int i = 0; i < length; i++)
	{
		buff[i+1] = *data++;
	}
	return 	at30tse75x_i2c.write((int)(AT30TSE_EEPROM_TWI_ADDR << 1),(char *)buff,(length + 1),false);
}
