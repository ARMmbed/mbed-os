/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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

#include "nrf_drv_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct serial_s {
};

struct spi_s {
    nrf_drv_spi_t *p_spi;
    volatile bool busy;
    bool async_mode;
};

struct port_s {
    __IO uint32_t *reg_cnf;
    __IO uint32_t *reg_out;
    __I  uint32_t *reg_in;
    PortName port;
    uint32_t mask;
};

struct pwmout_s {
    PWMName pwm_name;
    PinName pin;
    uint8_t pwm_channel;
    void *  pwm_struct;
};

struct i2c_s {
    uint8_t twi_idx;
};

struct analogin_s {
    ADCName adc;
    uint8_t adc_pin;
};

struct gpio_irq_s {
    uint32_t ch;
};

struct sleep_s {
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
