#ifndef AT30TSE75X_H
#define AT30TSE75X_H

#include "mbed.h"

#define AT30TSE_TEMPERATURE_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_TEMP | \
(BOARD_AT30TSE_DEVICE_ADDR & 0x07))

/* AT30TSE75x Device Type ID for Temperature Sensor: 0b1001xxx */
#define AT30TSE75X_DEVICE_TYPE_ID_TEMP    0x48
/* AT30TSE75x Device Type ID for EEPROM: 0b1010xxx */
#define AT30TSE75X_DEVICE_TYPE_ID_EEPROM  0x50

#define BOARD_AT30TSE_DEVICE_ADDR  0x07 /* 0b111 */ /*CAn be varied according to the condition of A0, A1, A2*/

#define AT30TSE_CONFIG_RES_9_bit    0
#define AT30TSE_CONFIG_RES_10_bit   1
#define AT30TSE_CONFIG_RES_11_bit   2
#define AT30TSE_CONFIG_RES_12_bit   3

#define AT30TSE_TEMPERATURE_REG_SIZE    2

#if (BOARD_USING_AT30TSE != AT30TSE752) && \
(BOARD_USING_AT30TSE != AT30TSE754) && \
(BOARD_USING_AT30TSE != AT30TSE758) && \
(BOARD_USING_AT30TSE != AT30TS75)
/* Using AT30TSE758 in Default */
#define BOARD_USING_AT30TSE  AT30TSE758
#endif

/* Setting the AT30TSE75x Temperature Sensor Address */
#define AT30TSE_TEMPERATURE_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_TEMP | \
(BOARD_AT30TSE_DEVICE_ADDR & 0x07))

#define AT30TSE752                      1
#define AT30TSE754                      2
#define AT30TSE758                      3
#define AT30TS75                        4

#if BOARD_USING_AT30TSE == AT30TSE752
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
(BOARD_AT30TSE_DEVICE_ADDR & 0x07))
#elif BOARD_USING_AT30TSE == AT30TSE754
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
(BOARD_AT30TSE_DEVICE_ADDR & 0x06))
#elif BOARD_USING_AT30TSE == AT30TSE758
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
(BOARD_AT30TSE_DEVICE_ADDR & 0x04))
#elif BOARD_USING_AT30TSE != AT30TS75
/* Using AT30TSE758 in Default */
#define AT30TSE_EEPROM_TWI_ADDR  (AT30TSE75X_DEVICE_TYPE_ID_EEPROM | \
(BOARD_AT30TSE_DEVICE_ADDR & 0x04))
#endif

class AT30TSE75X
{
public:
  AT30TSE75X(PinName sda, PinName scl);
  ~AT30TSE75X();

  float read_temp();
  uint8_t read_eeprom(uint8_t *data, uint8_t length, uint8_t word_addr, uint8_t page);
  uint8_t write_eeprom(uint8_t *data, uint8_t length, uint8_t word_addr, uint8_t page);
  
  I2C at30tse75x_i2c;
private:
  uint8_t resolution = AT30TSE_CONFIG_RES_9_bit;

};

#endif
