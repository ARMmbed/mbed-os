/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "object_owners.h"

#include "nrf.h"
#include "nrf_peripherals.h"

#include <stdio.h>

#define SPI2C_INSTANCES SPI_COUNT

static void * nordic_spi2c_owners[SPI2C_INSTANCES] = { NULL, NULL, NULL };

/**
 * Brief       Set instance owner for the SPI/I2C peripheral.
 *
 * Parameter   instance  The instance.
 * Parameter   object    The object.
 */
void object_owner_spi2c_set(int instance, void *object)
{
    if (instance < SPI2C_INSTANCES) {
        nordic_spi2c_owners[instance] = object;
    }
}

/**
 * Brief       Get instance owner for the SPI/I2C peripheral.
 *
 * Parameter   instance  The instance.
 *
 * Return      Pointer to the object owning the instance.
 */
void * object_owner_spi2c_get(int instance)
{
    void *object = (void *) 0xFFFFFFFF;

    if (instance < SPI2C_INSTANCES) {
        object = nordic_spi2c_owners[instance];
    }

    return object;
}
