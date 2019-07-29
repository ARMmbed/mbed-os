/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "tmpm46b_gpio.h"
#include "tmpm46b_uart.h"
#include "tmpm46b_fuart.h"
#include "tmpm46b_cg.h"
#include "tmpm46b_i2c.h"
#include "tmpm46b_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    uint32_t            mask;
    GPIO_Port           port;
    uint32_t            irq_id;
    CG_INTActiveState   event;
    CG_INTSrc           irq_src;
};

struct port_s {
    PortName port;
    uint32_t mask;
};

struct pwmout_s {
    PinName         pin;
    TSB_TB_TypeDef *channel;
    uint16_t        trailing_timing;
    uint16_t        leading_timing;
    uint16_t        divisor;
    float           period;
};

struct serial_s {
    PinName             pin;
    uint32_t            index;
    TSB_SC_TypeDef      *UARTx;
    TSB_FUART_TypeDef   *FUART;
    UART_InitTypeDef    uart_config;
    FUART_InitTypeDef   fuart_config;
    TSB_UART_TypeDef    *FUART_Reg;
};

struct analogin_s {
    PinName         pin;
    ADCName         adc;
    TSB_AD_TypeDef  *obj;
    ADC_AINx        channel;
};

struct i2c_s {
    uint32_t        index;
    int             address;
    IRQn_Type IRQn;
    TSB_I2C_TypeDef *i2c;
    I2C_InitTypeDef myi2c;
#ifdef DEVICE_I2C_ASYNCH
    uint32_t        event_mask;
    uint8_t         stop;
    uint8_t         state;
#endif
};

struct spi_s {
    TSB_SSP_TypeDef *spi;
    SPIName         module;
    uint8_t         bits;
    PinName         clk_pin;
    IRQn_Type       irqn;
#ifdef DEVICE_SPI_ASYNCH
    uint32_t        event_mask;
    uint8_t         state;
#endif
};

struct trng_s {
    bool trng_init;
};

struct flash_s {
    uint8_t dummy;
};
#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
