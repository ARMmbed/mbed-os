/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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

#include <stdbool.h>
#include "PortNames.h"
#include "PeripheralNames.h"
#include "gpio_object.h"
#include "txz_gpio.h"
#include "txz_uart.h"
#include "txz_fuart.h"
#include "txz_tspi.h"
#include "txz_t32a.h"
#include "txz_cg.h"
#include "txz_driver_def.h"
#include "adc.h"
#include "txz_i2c_api.h"
#include "txz_i2c.h"
#include "txz_fuart_include.h"

#ifdef __cplusplus
extern "C" {
#endif

struct port_s {
    PortName port;
    uint32_t mask;
};

struct dac_s {
    DACName dac;
    TSB_DA_TypeDef *DACx;
};

struct serial_s {
    uint32_t index;
    uint32_t mode;
    TSB_UART_TypeDef *UARTx;
    TSB_FURT_TypeDef *FUARTx;
    uart_boudrate_t boud_obj;
    fuart_boudrate_t fboud_obj;
};

struct pwmout_s {
    PinName pin;
    uint32_t trailing_timing;
    uint32_t leading_timing;
    uint32_t divisor;
    float period;
    t32a_t p_t32a;
    uint32_t type;
};

struct spi_s {
    tspi_t p_obj;
    SPIName module;
    uint8_t bits;
};

struct gpio_irq_s {
    PortName port;
    uint8_t pin_num;
    uint32_t irq_id;
    CG_INTActiveState event;
    CG_INTSrc irq_src;
};

struct flash_s {
    int flash_inited;
};

struct analogin_s {
    PinName pin;
    adc_t p_adc;
    adc_channel_setting_t param;
    ADCName adc;
};

struct i2c_s {
    uint32_t index;
    int address;
    _i2c_t my_i2c;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
