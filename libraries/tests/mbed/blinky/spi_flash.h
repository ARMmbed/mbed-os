#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include <stdbool.h>
#include <stdint.h>

bool spi_flash_init(void);
void spi_flash_get_MACAddress(uint8_t *);
bool spi_flash_powerDown(void);

#endif /* SPI_FLASH_H */