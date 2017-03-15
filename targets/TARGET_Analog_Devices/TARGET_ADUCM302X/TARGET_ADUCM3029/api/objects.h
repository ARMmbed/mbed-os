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

#include "cmsis.h"
#include "PeripheralNames.h"
#include "PinNames.h"
//#include "target_config.h"
#include "gpio_object.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Add your custom header content here */
struct gpio_irq_s {
    unsigned int id;
    PinName pinname;
};

struct sleep_s {
    int temp;
};

struct serial_s {
    int index;
};

struct trng_s {
    uint8_t dummy;
};

struct spi_s {
    uint32_t instance;
};

#include "gpio_object.h"


#ifdef __cplusplus
}
#endif

#endif
