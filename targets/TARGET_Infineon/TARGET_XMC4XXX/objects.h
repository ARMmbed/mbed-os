/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "cmsis.h"
#include "gpio_object.h"
#include "PeripheralNames.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************** Object's **/
#if DEVICE_SERIAL
struct serial_s {
    void *channel;
    uint32_t index;
    uint8_t dx0;
};
#endif

#if DEVICE_INTERRUPTIN
struct gpio_irq_s {
    void *eru_params;
    uint32_t id;
};
#endif

#if DEVICE_ANALOGOUT
struct dac_s {
    uint32_t dac;
};
#endif

#if DEVICE_ANALOGIN
struct analogin_s {
    void *group_ptr;
};
#endif

#if DEVICE_PWMOUT
struct pwmout_s {
    uint32_t pin;
    void *slice_ptr;
    void *module_ptr;
    uint8_t slice_nbr;
    uint8_t channel_nbr;
    uint8_t ccu_module;
    uint8_t prescaler;
    uint16_t period;
    double us_per_cycle;
    uint32_t ccu_freq;
    float duty_cycle;
};
#endif

#if DEVICE_I2C
struct i2c_s {
	void *channel;
	uint8_t dx0;
	uint8_t start_stop;
};
#endif

#if DEVICE_FLASH
struct flash_s {
    void *p;
};
#endif

#if DEVICE_SPI
struct spi_s {
    void *channel;
    uint8_t dx0;
    uint8_t idx;
};
#endif

#ifdef __cplusplus
}
#endif

#endif

/*EOF*/
