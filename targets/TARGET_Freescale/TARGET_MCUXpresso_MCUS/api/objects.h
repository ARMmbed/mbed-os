/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#if DEVICE_SPI_ASYNCH
#include "fsl_dspi_edma.h"
#endif
#if DEVICE_SERIAL_ASYNCH
#include "fsl_uart_edma.h"
#endif
#include "fsl_flash.h"
#include "dma_api_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    uint32_t port;
    uint32_t pin;
    uint32_t ch;
};

struct port_s {
    PortName port;
    uint32_t mask;
};

struct pwmout_s {
    PWMName pwm_name;
};

struct serial_s {
    int index;
#if DEVICE_SERIAL_ASYNCH
    uint8_t txstate;
    uint8_t rxstate;
    uint32_t events;
    uart_handle_t uart_transfer_handle;
    uart_edma_handle_t uart_dma_handle;
    dma_options_t uartDmaTx;
    dma_options_t uartDmaRx;
#endif
};

struct analogin_s {
    ADCName adc;
};

struct i2c_s {
    uint32_t instance;
    uint8_t next_repeated_start;
};

struct spi_s {
    uint32_t instance;
    uint8_t bits;
#if DEVICE_SPI_ASYNCH
    status_t status;
    dspi_master_handle_t spi_master_handle;
    dspi_master_edma_handle_t spi_dma_master_handle;
    dma_options_t spiDmaMasterRx;
    dma_options_t spiDmaMasterTx;
    dma_options_t spiDmaMasterIntermediary;
#endif
};

struct dac_s {
    DACName dac;
};

struct trng_s {
    uint8_t dummy;
};

struct flash_s {
    flash_config_t flash_config;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
