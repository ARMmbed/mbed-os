/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
 *
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
#include "txz_adc.h"
#include "txz_i2c_api.h"
#include "txz_i2c.h"
#include "txz_fuart_include.h"

#ifdef __cplusplus
extern "C" {
#endif

struct port_s {
    uint32_t mask;
    PortName port;
};

struct dac_s {
    DACName dac;
    TSB_DA_TypeDef *DACx;
};

typedef struct {
    uint32_t BaudRate;
    uint32_t DataBits;
    uint32_t StopBits;
    uint32_t Parity;
    uint32_t Mode;
    uint32_t FlowCtrl;
} FUART_InitTypeDef;

struct serial_s {
    uint32_t index;
    uint32_t mode;
    uint8_t  is_using_fuart;
    TSB_UART_TypeDef *UARTx;
    TSB_FURT_TypeDef *FUARTx;
    uart_boudrate_t   boud_obj;
    FUART_InitTypeDef fuart_config;
};

struct pwmout_s {
    uint32_t divisor;
    uint32_t type;
    uint32_t trailing_timing;
    uint32_t leading_timing;
    float    period;
    t32a_t   p_t32a;
    PinName  pin;
};

struct spi_s {
    uint8_t   bits;
    tspi_t    p_obj;
    SPIName   module;
    PinName   clk_pin;
    IRQn_Type rxirqn;
    IRQn_Type txirqn;
    IRQn_Type errirqn;
#ifdef DEVICE_SPI_ASYNCH
    uint32_t  event_mask;
    uint8_t   state;
#endif
};

struct gpio_irq_s {
    PortName  port;
    uint8_t   pin_num;
    uint32_t  irq_id;
    CG_INTSrc irq_src;
    CG_INTActiveState event;
};

struct flash_s {
    int flash_inited;
};

struct analogin_s {
    adc_t   p_adc;
    PinName pin;
    ADCName adc;
    adc_channel_setting_t param;
};

struct i2c_s {
    uint8_t   is_master;
    uint32_t  index;
    IRQn_Type irqn;
    _i2c_t    my_i2c;
#if DEVICE_I2C_ASYNCH
    uint32_t  event_mask;
#endif
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
