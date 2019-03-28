/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "i2c_def.h"
#include "spi_def.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    uint32_t ch;
};

struct port_s {
    __IO uint32_t *reg_dir;
    __IO uint32_t *reg_dirclr;
    __IO uint32_t *reg_out;
    __IO  uint32_t *reg_in;
    PortName port;
    uint32_t mask;
};

struct serial_s {
    void *uart;
    int index;
};

struct i2c_s {
    I2C_TypeDef *i2c;
    uint16_t last_xfer_address;
	unsigned int xfer_speed;
	unsigned int mode;
	unsigned int base;
	unsigned int timeout;
	unsigned int slave_addr;
	unsigned int addrlen;
	unsigned int master;
	int retries;
	unsigned char initialized;
	int clock;
};

struct spi_s {
    SPI_TypeDef *spi;
	unsigned int freqid;
	unsigned int nbits;
	unsigned int base;
	unsigned int mode;
	unsigned int frequency;
	unsigned int actual;
};

struct analogin_s {
    ADCName adc;
    PinName  pin;
    uint32_t pin_number;
    __IO uint32_t address;
};

struct flash_s {
	uint32_t dummy;
};

struct trng_s {
	uint32_t dummy;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
