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

#include "pinmap_ex.h"

#include "mbed_toolchain.h"
#include "nrf.h"
#include "nrf_peripherals.h"

#include <stdio.h>

#if 0
#define DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while(0)
#else
#define DEBUG_PRINTF(...) {}
#endif

/* Define number of instances */
#define NORDIC_TWI_COUNT TWI_COUNT
#define NORDIC_SPI_COUNT SPI_COUNT

/* Define which instance to return when there are no free instances left.
 * The Mbed HAL API doesn't provide a way for signaling initialization errors
 * so we return the default value. Any instance conflicts must be handled
 * by the driver implementation.
 */
#define DEFAULT_I2C_INSTANCE 0 // SPI counts down, choose instance furthest away
#define DEFAULT_SPI_INSTANCE (SPI_COUNT - 1) // I2C counts up, choose instance furthers away

/* Internal data structure shared between SPI and I2C to keep track of allocated
 * instances. The data structure is shared to reflect the hardware sharing.
 */
typedef struct {
    PinName locaction0;
    PinName locaction1;
    PinName locaction2;
} spi2c_t;

static spi2c_t nordic_internal_spi2c[3] = {
    {NC, NC, NC},
    {NC, NC, NC},
    {NC, NC, NC},
};

/**
 * Brief       Find hardware instance for the provided I2C pins.
 *
 *             The function will search the PeripheralPin map for a pre-allocated
 *             assignment. If none is found the allocation map will be searched
 *             to see if the same pins have been assigned an instance before.
 *
 *             If no assignement is found and there is an empty slot left in the
 *             map, the pins are stored in the map and the hardware instance is
 *             returned.
 *
 *             If no free instances are available, the default instance is returned.
 *
 * Parameter   sda   sda pin.
 * Parameter   scl   scl pin.
 *
 * Return      Hardware instance associated with provided pins.
 */
int pin_instance_i2c(PinName sda, PinName scl)
{
    int instance = NC;

    /* Search pin map for pre-allocated instance */
    for (size_t index = 0;
         !((PinMap_I2C[index].sda == NC) &&
           (PinMap_I2C[index].scl == NC));
         index++) {

        /* Compare pins to entry. */
        if ((PinMap_I2C[index].sda == sda) &&
            (PinMap_I2C[index].scl == scl)) {

            DEBUG_PRINTF("found: %d %d %d\r\n", sda, scl, PinMap_I2C[index].instance);

            /* Instance found, save result. */
            instance = PinMap_I2C[index].instance;

            /* Lock out entry in map to prevent SPI from grabbing it. */
            nordic_internal_spi2c[instance].locaction0 = sda;
            nordic_internal_spi2c[instance].locaction1 = scl;
            nordic_internal_spi2c[instance].locaction2 = NC;
            break;
        }
    }

    /* No instance was found in static map. */
    if (instance == NC) {

        /* Search dynamic map for entry. */
        for (size_t index = 0; index < NORDIC_TWI_COUNT; index++) {

            /* Pins match previous dynamic allocation, return instance. */
            if ((nordic_internal_spi2c[index].locaction0 == sda) &&
                (nordic_internal_spi2c[index].locaction1 == scl) &&
                (nordic_internal_spi2c[index].locaction2 == NC)) {

                instance = index;
                break;
            }
        }
    }

    /* No instance was found in dynamic map. */
    if (instance == NC) {

        /* Search dynamic map for empty slot. */
        for (size_t index = 0; index < NORDIC_TWI_COUNT; index++) {

            /* Empty slot found, reserve slot by storing pins. */
            if ((nordic_internal_spi2c[index].locaction0 == NC) &&
                (nordic_internal_spi2c[index].locaction1 == NC) &&
                (nordic_internal_spi2c[index].locaction2 == NC)) {

                nordic_internal_spi2c[index].locaction0 = sda;
                nordic_internal_spi2c[index].locaction1 = scl;

                instance = index;
                break;
            }
        }
    }

#if defined(DEFAULT_I2C_INSTANCE)
    /* Exhausted all options. Return default value. */
    if (instance == NC) {
        instance = DEFAULT_I2C_INSTANCE;
    }
#endif

    DEBUG_PRINTF("I2C: %d %d %d\r\n", sda, scl, instance);

    return instance;
}

/**
 * Brief       Find hardware instance for the provided SPI pins.
 *
 *             The function will search the PeripheralPin map for a pre-allocated
 *             assignment. If none is found the allocation map will be searched
 *             to see if the same pins have been assigned an instance before.
 *
 *             If no assignement is found and there is an empty slot left in the
 *             map, the pins are stored in the map and the hardware instance is
 *             returned.
 *
 *             If no free instances are available, the default instance is returned.
 *
 * Parameter   mosi   mosi pin.
 * Parameter   miso   miso pin.
 * Parameter   clk    clk pin.
 *
 * Return      Hardware instance associated with provided pins.
 */
int pin_instance_spi(PinName mosi, PinName miso, PinName clk)
{
    int instance = NC;

    /* Search pin map for pre-allocated instance */
    for (size_t index = 0;
         !((PinMap_SPI[index].mosi == NC) &&
           (PinMap_SPI[index].miso == NC) &&
           (PinMap_SPI[index].clk  == NC));
         index++) {

        DEBUG_PRINTF("search: %d %d %d\r\n", PinMap_SPI[index].mosi, PinMap_SPI[index].miso, PinMap_SPI[index].clk);

        /* Compare pins to entry. */
        if ((PinMap_SPI[index].mosi == mosi) &&
            (PinMap_SPI[index].miso == miso) &&
            (PinMap_SPI[index].clk  == clk)) {

            DEBUG_PRINTF("found: %d %d %d %d\r\n", mosi, miso, clk, PinMap_SPI[index].instance);

            /* Foung instance, save result. */
            instance = PinMap_SPI[index].instance;

            /* Lock out entry in map to prevent I2C from grabbing it. */
            nordic_internal_spi2c[instance].locaction0 = mosi;
            nordic_internal_spi2c[instance].locaction1 = miso;
            nordic_internal_spi2c[instance].locaction2 = clk;
            break;
        }
    }

    /* No instance was found in static map. */
    if (instance == NC) {

        /* Search dynamic map for entry. */
        for (int index = NORDIC_SPI_COUNT - 1; index > -1; index--) {
            if ((nordic_internal_spi2c[index].locaction0 == mosi) &&
                (nordic_internal_spi2c[index].locaction1 == miso) &&
                (nordic_internal_spi2c[index].locaction2 == clk)) {

                instance = index;
                break;
            }
        }
    }

    /* No instance was found in dynamic map. */
    if (instance == NC) {

        /* Search dynamic map for empty slot. */
        for (int index = NORDIC_SPI_COUNT - 1; index > -1; index--) {

            /* Empty slot found, reserve slot by storing pins. */
            if ((nordic_internal_spi2c[index].locaction0 == NC) &&
                (nordic_internal_spi2c[index].locaction1 == NC) &&
                (nordic_internal_spi2c[index].locaction2 == NC)) {

                nordic_internal_spi2c[index].locaction0 = mosi;
                nordic_internal_spi2c[index].locaction1 = miso;
                nordic_internal_spi2c[index].locaction2 = clk;

                instance = index;
                break;
            }
        }
    }

#if defined(DEFAULT_SPI_INSTANCE)
    /* Exhausted all options. Return default value. */
    if (instance == NC) {
        instance = DEFAULT_SPI_INSTANCE;
    }
#endif

    DEBUG_PRINTF("SPI: %d %d %d %d\r\n", mosi, miso, clk, instance);

    return instance;
}
