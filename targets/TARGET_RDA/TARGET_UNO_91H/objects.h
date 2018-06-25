/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "gpio_object.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    uint16_t ch;
    uint8_t  flagR;
    uint8_t  flagF;
};

struct port_s {
    PortName port;
    uint32_t mask;
    __IO uint32_t *reg_out;
    __I  uint32_t *reg_in;
    __IO uint32_t *reg_dir;
};

struct pwmout_s {
    uint32_t channel;
    uint32_t *base_clk_ptr;
    uint32_t period_ticks;
    uint32_t pulsewidth_ticks;
    uint32_t period_ticks_max;
    uint32_t period_ticks_min;
    PinName  pin;
    __IO uint32_t *CFGR;
};

struct serial_s {
    RDA_UART_TypeDef *uart;
    int index;
};

struct i2s_s {
    RDA_I2S_TypeDef *i2s;
};

struct wdt_s {
    RDA_WDT_TypeDef *wdt;
};

struct analogin_s {
    ADCName adc;
};

struct gpadc_s {
    GPADCName ch;
    PinName   pin;
};

#if 0
struct dac_s {
    DACName dac;
};
#endif

struct i2c_s {
    RDA_I2C0_TypeDef *i2c;
};

struct spi_s {
    RDA_SPI_TypeDef *spi;
    uint8_t bit_ofst[2];
};

struct trng_s {
    uint8_t dma_clk_flag;
    uint8_t byte_idx;
    uint32_t trng_data0;
};

#ifdef __cplusplus
}
#endif

#endif
