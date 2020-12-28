/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <stddef.h>

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
};

struct analogin_s {
    ADCName adc;
};

struct i2c_s {
    uint32_t instance;
    uint8_t next_repeated_start;
    uint8_t issue_start;
};

struct spi_s {
    uint32_t instance;
    uint8_t bits;
    uint8_t ssel_num;
};

#if DEVICE_FLASH && !defined(TARGET_FLASH_CMSIS_ALGO)
struct flash_s {
    uint8_t dummy;
};
#endif

#if DEVICE_TRNG
struct trng_s {
    uint8_t dummy;
};
#endif

#if DEVICE_QSPI
struct qspi_s {
    uint32_t instance;
};
#endif

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
