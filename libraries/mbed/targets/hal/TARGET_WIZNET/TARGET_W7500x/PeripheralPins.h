#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "pinmap.h"
#include "PeripheralNames.h"


//*** ADC ***
extern const PinMap PinMap_ADC[];

//*** SERIAL ***
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];

//*** I2C ***
extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];


//*** SPI ***

extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SSEL[];
#endif
