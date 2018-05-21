/*
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "nrf_drv_spi.h"
#include "nrf_twi.h"

#include "nrf_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_uart.h"

#if defined(FEATURE_CRYPTOCELL310)
#include "objects_cryptocell.h"
#else
struct trng_s {
    uint32_t placeholder;
};
#endif

struct serial_s {
    int                 instance;
    uint32_t            tx;
    uint32_t            rx;
    uint32_t            cts;
    uint32_t            rts;
    nrf_uart_hwfc_t     hwfc;
    nrf_uart_parity_t   parity;
    nrf_uart_baudrate_t baudrate;
    uint32_t            context;
    uint32_t            handler;
    uint32_t            mask;
    uint32_t            event;
    bool                update;
#if DEVICE_SERIAL_ASYNCH
    bool                rx_asynch;
    uint32_t            tx_handler;
    uint32_t            tx_mask;
    uint32_t            tx_event;
    uint32_t            rx_handler;
    uint32_t            rx_mask;
    uint32_t            rx_event;
#endif
};

struct spi_s {
    int instance;
    PinName cs;
    nrf_drv_spi_config_t config;
    bool update;

#if DEVICE_SPI_ASYNCH
    uint32_t handler;
    uint32_t mask;
    uint32_t event;
#endif
};

struct port_s {
    PortName port;
    uint32_t mask;
};

struct pwmout_s {
    int instance;
    PinName pin;
    nrf_pwm_values_common_t pulse;
    uint16_t period;
    float percent;
    nrf_pwm_sequence_t sequence;
};

struct i2c_s {
    int instance;
    PinName sda;
    PinName scl;
    nrf_twi_frequency_t frequency;
    int state;
    int mode;
    bool update;

#if DEVICE_I2C_ASYNCH
    uint32_t handler;
    uint32_t mask;
    uint32_t event;
#endif
};

struct analogin_s {
    uint8_t channel;
};

struct gpio_irq_s {
    uint32_t ch;
};

struct flash_s {
    uint32_t placeholder;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
