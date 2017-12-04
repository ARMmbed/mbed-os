/***************************************************************************//**
 * @file objects.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "PinNames.h"
#include "PeripheralNames.h"
#include "PortNames.h"
#include "em_i2c.h"
#include "em_dma.h"
#include "em_cmu.h"
#include "dma_api_HAL.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName pin;
    PinMode mode;
    PinDirection dir;
} gpio_t;

#if DEVICE_ANALOGIN
struct analogin_s {
    ADC_TypeDef *adc;
    uint32_t channel;
};
#endif

#if DEVICE_ANALOGOUT
struct dac_s {
    DAC_TypeDef *dac;
    uint32_t channel;
};
#endif

#if DEVICE_I2C
struct i2c_s {
    I2C_TypeDef *i2c;
    uint32_t location;
#if DEVICE_I2C_ASYNCH
    uint32_t events;
    I2C_TransferSeq_TypeDef xfer;
#endif
};
#endif

#if DEVICE_PORTOUT
struct port_s {
    PortName port;
    uint32_t mask;
    PinDirection dir;
};
#endif

#if DEVICE_PWMOUT
struct pwmout_s {
    //Channel on TIMER
    uint32_t channel;
    PinName pin;
};
#endif

#if DEVICE_INTERRUPTIN
struct gpio_irq_s {
    PinName pin;
    uint8_t risingEdge;
    uint8_t fallingEdge;
};
#endif

#if DEVICE_SERIAL
struct serial_s {
    union {
        USART_TypeDef *uart;
        LEUART_TypeDef *leuart;
    } periph;
#ifndef _SILICON_LABS_32B_PLATFORM_2
    uint32_t location;
#else
    uint32_t location_tx;
    uint32_t location_rx;
#endif
    PinName rx_pin;
    PinName tx_pin;
#if DEVICE_SERIAL_ASYNCH
    uint32_t events;
    DMA_OPTIONS_t dmaOptionsTX;
    DMA_OPTIONS_t dmaOptionsRX;
#endif
    uint32_t sleep_blocked;
};
#endif

#if DEVICE_SPI
struct spi_s {
    USART_TypeDef *spi;
    uint32_t location;
    uint32_t route;
    uint8_t bits;
    uint8_t master;
#if DEVICE_SPI_ASYNCH
    uint32_t event;
    DMA_OPTIONS_t dmaOptionsTX;
    DMA_OPTIONS_t dmaOptionsRX;
#endif
};
#endif

#if DEVICE_RTC
struct lp_timer_s {
    uint32_t start;
    uint32_t stop;
};
#endif

#if DEVICE_SLEEP
#define NUM_SLEEP_MODES 5
typedef enum {
    EM0 = 0,
    EM1 = 1,
    EM2 = 2,
    EM3 = 3,
    EM4 = 4
} sleepstate_enum;
#endif

#if DEVICE_FLASH
struct flash_s {
    MSC_TypeDef *msc;
};
#endif

#if DEVICE_TRNG
struct trng_s {
    TRNG_TypeDef *instance;
};
#endif

#ifdef __cplusplus
}
#endif

#endif
