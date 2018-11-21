/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
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

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "mbed_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gpio_s gpio_t;

struct gpio_s {
    uint32_t mask;
    PinName  pin;
    __IO uint32_t gpio_periph;
};

struct gpio_irq_s {
    IRQn_Type irq_n;
    uint32_t irq_index;
    uint32_t event;
    PinName pin;
};

struct port_s {
    PortName port;
    uint32_t mask;
    PinDirection direction;
    __IO uint32_t *reg_in;
    __IO uint32_t *reg_out;
};

struct analogin_s {
    ADCName adc;
    PinName pin;
    uint8_t channel;
};

#if DEVICE_ANALOGOUT
struct dac_s {
    DACName dac;
    PinName pin;
    uint32_t channel;
};
#endif

struct can_s {
    CANName can;
    int index;
};

struct pwmout_s {
    PWMName pwm;
    uint32_t cnt_unit;
    uint8_t ch;
};

struct serial_s {
    /* basic information */
    UARTName uart;
    int     index;
    PinName pin_tx;
    PinName pin_rx;

    /* configure information */
    uint32_t baudrate;
    uint32_t databits;
    uint32_t stopbits;
    uint32_t parity;

    /* operating parameters */
    uint16_t        rx_size;
    uint8_t         *tx_buffer_ptr;
    uint8_t         *rx_buffer_ptr;
    __IO uint16_t   tx_count;
    __IO uint16_t   rx_count;

    __IO uint32_t   error_code;
    __IO operation_state_enum  tx_state;
    __IO operation_state_enum  rx_state;

#if DEVICE_SERIAL_ASYNCH
    uint32_t events;
#endif
#if DEVICE_SERIAL_FC
    uint32_t hw_flow_ctl;
    PinName pin_rts;
    PinName pin_cts;
#endif
};

struct spi_s {
    spi_parameter_struct spi_struct;
    IRQn_Type spi_irq;
    SPIName spi;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    PinName pin_ssel;
};

struct i2c_s {
    /* basic information */
    I2CName i2c;
    uint8_t index;
    PinName sda;
    PinName scl;
    int     i2c_inited;         /* flag used to indicate whether the i2c has been initialized */

    /* configure information */
    int      freq;              /* i2c frequence */
    uint32_t addr_bit_mode;     /* 7 bits or 10 bits */
    uint32_t slave_addr0;
    uint32_t slave_addr1;
    uint16_t transfer_size;
    uint8_t *buffer_pointer;

    /* operating parameters */
    __IO operation_state_enum state;
    __IO i2c_mode_enum mode;
    __IO uint32_t previous_state_mode;
    __IO uint32_t i2c_target_dev_addr;
    __IO uint32_t event_count;
    __IO uint32_t transfer_count;
    __IO uint32_t transfer_option;
    __IO uint32_t error_code;

    /* I2C DMA information */
    uint32_t tx_dma_periph;
    dma_channel_enum tx_dma_channel;
    uint32_t rx_dma_periph;
    dma_channel_enum rx_dma_channel;

    IRQn_Type event_i2cIRQ;
    IRQn_Type error_i2cIRQ;
    uint32_t global_trans_option;
    volatile uint8_t event;

#if DEVICE_I2CSLAVE
    uint8_t slave;
    volatile uint8_t pending_slave_tx_master_rx;
    volatile uint8_t pending_slave_rx_maxter_tx;
#endif

#if DEVICE_I2C_ASYNCH
    uint32_t address;
    uint8_t stop;
    uint8_t available_events;
#endif

};
#if DEVICE_FLASH
struct flash_s {
    /*  nothing to be stored for now */
    uint32_t dummy;
};
#endif
#ifdef __cplusplus
}
#endif

#endif
