/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
    CMSDK_UART_TypeDef *uart;
    int index;
};

struct i2c_s {
    MPS2_I2C_TypeDef *i2c;
};

struct tsc_s {
    MPS2_I2C_TypeDef *tsc;
};

struct audio_s {
    MPS2_I2S_TypeDef *audio_I2S;
        MPS2_I2C_TypeDef *audio_I2C;
};


struct spi_s {
    MPS2_SSP_TypeDef *spi;
};

struct clcd_s {
    MPS2_SSP_TypeDef *clcd;
};

struct analogin_s {
    ADCName adc;
        MPS2_SSP_TypeDef *adc_spi;
    PinName  pin;
        uint32_t pin_number;
    __IO uint32_t address;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
