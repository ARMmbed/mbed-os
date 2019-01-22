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

#include <stddef.h>
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "TMPM3HQ.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t BaudRate;
    uint32_t DataBits;
    uint32_t StopBits;
    uint32_t Parity;
    uint32_t Mode;
    uint32_t FlowCtrl;
} uart_inittypedef_t;

struct port_s {
    PortName port;
    uint32_t mask;
};

typedef struct {
    uint8_t PinDATA;
    uint8_t PinCR;
    uint8_t PinFR[FRMAX];
    uint8_t PinOD;
    uint8_t PinPUP;
    uint8_t PinPDN;
    uint8_t PinIE;
} gpio_regtypedef_t;

typedef struct {
    __IO uint32_t DATA;
    __IO uint32_t CR;
    __IO uint32_t FR[FRMAX];
    uint32_t RESERVED0[1];
    __IO uint32_t OD;
    __IO uint32_t PUP;
    __IO uint32_t PDN;
    uint32_t RESERVED1;
    __IO uint32_t IE;
} TSB_Port_TypeDef;

struct serial_s {
    PinName pin;
    uint32_t index;
    TSB_UART_TypeDef * UARTx;
    uart_inittypedef_t uart_config;
};

struct analogin_s {
    PinName pin;
    ADCName adc;
    TSB_AD_TypeDef* obj;
};

struct dac_s {
    DACName dac;
    TSB_DA_TypeDef* handler;
};

struct pwmout_s {
    PinName pin;
    TSB_T32A_TypeDef * channel;
    uint16_t trailing_timing;
    uint16_t leading_timing;
    uint16_t divisor;
    float period;
};

struct i2c_s {
    uint32_t address;
    IRQn_Type IRQn;
    TSB_I2C_TypeDef *i2c;
};

struct spi_s {
    TSB_TSPI_TypeDef *spi;
    SPIName module;
    uint8_t bits;
};

extern const gpio_regtypedef_t GPIO_SFRs[];
extern const uint32_t GPIO_Base[];

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
