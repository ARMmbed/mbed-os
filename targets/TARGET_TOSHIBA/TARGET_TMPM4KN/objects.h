/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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
#include "txzp_gpio.h"
#include "txzp_uart.h"
#include "txzp_tspi.h"
#include "txzp_t32a.h"
#include "txzp_cg.h"
#include "txzp_driver_def.h"
#include "txzp_adc.h"
#include "txzp_i2c_api.h"
#include "txzp_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

struct port_s {
    uint32_t mask;
    PortName port;
};


struct serial_s {
    uint32_t index;
    uint32_t mode;
    TSB_UART_TypeDef *UARTx;
    uart_boudrate_t   boud_obj;
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
    uint8_t bits;
    tspi_t  p_obj;
    SPIName module;
    PinName   clk_pin;
    PinName   ssel_pin;
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

struct analogin_s {
    adc_t   p_adc;
    PinName pin;
    ADCName adc;
    adc_channel_setting_t param;
};

struct i2c_s {
    int      address;
    uint32_t index;
    _i2c_t   my_i2c;
};

struct flash_s {
    int flash_inited;
};

#include "gpio_object.h"

#define HAL_CRC_IS_SUPPORTED(polynomial, width) (((width) == 16 && (polynomial) == 0x1021) || \
                                                 ((width) == 32 && (polynomial) == 0x04C11DB7))

#ifdef __cplusplus
}
#endif

#endif
