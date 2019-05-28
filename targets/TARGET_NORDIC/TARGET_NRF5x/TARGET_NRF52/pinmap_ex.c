/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited and affiliates.
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
#define NORDIC_PWM_COUNT PWM_COUNT
#define NORDIC_UART_COUNT UARTE_COUNT

/* Define which instance to return when there are no free instances left.
 * The Mbed HAL API doesn't provide a way for signaling initialization errors
 * so we return the default value. Any instance conflicts must be handled
 * by the driver implementation.
 */
#define DEFAULT_I2C_INSTANCE 0 // SPI counts down, choose instance furthest away
#define DEFAULT_SPI_INSTANCE (NORDIC_SPI_COUNT - 1) // I2C counts up, choose instance furthers away
#define DEFAULT_PWM_INSTANCE (NORDIC_PWM_COUNT - 1)
#define DEFAULT_UART_INSTANCE (NORDIC_UART_COUNT - 1)


/***
 *       _____ _____ _____ ___   _____
 *      / ____|  __ \_   _|__ \ / ____|
 *     | (___ | |__) || |    ) | |
 *      \___ \|  ___/ | |   / /| |
 *      ____) | |    _| |_ / /_| |____
 *     |_____/|_|   |_____|____|\_____|
 *
 *
 */

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


/***
 *      _______          ____  __
 *     |  __ \ \        / /  \/  |
 *     | |__) \ \  /\  / /| \  / |
 *     |  ___/ \ \/  \/ / | |\/| |
 *     | |      \  /\  /  | |  | |
 *     |_|       \/  \/   |_|  |_|
 *
 *
 */

/* Internal data structure to keep track of allocated instances.
 */
static PinName nordic_internal_pwm[NORDIC_PWM_COUNT] = {
    NC,
    NC,
    NC,
#if (NORDIC_PWM_COUNT == 4)
    NC,
#endif
};

/**
 * Brief       Find hardware instance for the provided PWM pin.
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
 * Parameter   pwm   pwm pin.
 *
 * Return      Hardware instance associated with provided pins.
 */
int pin_instance_pwm(PinName pwm)
{
    int instance = NC;

    /* Search pin map for pre-allocated instance */
    for (size_t index = 0; (PinMap_PWM[index].pwm != NC); index++) {

        /* Compare pins to entry. */
        if (PinMap_PWM[index].pwm == pwm) {

            DEBUG_PRINTF("found: %d %d\r\n", pwm, PinMap_PWM[index].instance);

            /* Instance found, save result. */
            instance = PinMap_PWM[index].instance;

            /* Lock out entry in map. */
            nordic_internal_pwm[instance] = pwm;
            break;
        }
    }

    /* No instance was found in static map. */
    if (instance == NC) {

        /* Search dynamic map for entry. */
        for (size_t index = 0; index < NORDIC_PWM_COUNT; index++) {

            /* Pins match previous dynamic allocation, return instance. */
            if (nordic_internal_pwm[index] == pwm) {

                instance = index;
                break;
            }
        }
    }

    /* No instance was found in dynamic map. */
    if (instance == NC) {

        /* Search dynamic map for empty slot. */
        for (size_t index = 0; index < NORDIC_PWM_COUNT; index++) {

            /* Empty slot found, reserve slot by storing pins. */
            if (nordic_internal_pwm[index] == NC) {

                nordic_internal_pwm[index] = pwm;

                instance = index;
                break;
            }
        }
    }

#if defined(DEFAULT_PWM_INSTANCE)
    /* Exhausted all options. Return default value. */
    if (instance == NC) {
        instance = DEFAULT_PWM_INSTANCE;
    }
#endif

    DEBUG_PRINTF("PWM: %d %d\r\n", pwm, instance);

    return instance;
}


/***
 *      _    _         _____ _______
 *     | |  | |  /\   |  __ \__   __|
 *     | |  | | /  \  | |__) | | |
 *     | |  | |/ /\ \ |  _  /  | |
 *     | |__| / ____ \| | \ \  | |
 *      \____/_/    \_\_|  \_\ |_|
 *
 *
 */

/* Internal data structure to keep track of allocated instances.
 */
typedef struct {
    PinName tx;
    PinName rx;
} uart_t;

static uart_t nordic_internal_uart[NORDIC_UART_COUNT] = {
    { NC, NC },
#if (NORDIC_UART_COUNT == 2)
    { NC, NC }
#endif
};

/**
 * Brief       Find hardware instance for the provided UART pin.
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
 * Parameter   tx    tx pin.
 * Parameter   rx    rx pin.
 *
 * Return      Hardware instance associated with provided pins.
 */
int pin_instance_uart(PinName tx, PinName rx)
{
    int instance = NC;

    /* Search pin map for pre-allocated instance */
    for (size_t index = 0; ((PinMap_UART[index].tx != NC) && (PinMap_UART[index].rx != NC)); index++) {

        /* Compare pins to entry. */
        if ((PinMap_UART[index].tx == tx) && (PinMap_UART[index].rx == rx)) {

            DEBUG_PRINTF("found: %d %d %d\r\n", tx, rx, PinMap_UART[index].instance);

            /* Instance found, save result. */
            instance = PinMap_UART[index].instance;

            /* Lock out entry in map. */
            nordic_internal_uart[instance].tx = tx;
            nordic_internal_uart[instance].rx = rx;
            break;
        }
    }

    /* No instance was found in static map. */
    if (instance == NC) {

        /* Search dynamic map for entry. */
        for (size_t index = 0; index < NORDIC_UART_COUNT; index++) {

            /* Pins match previous dynamic allocation, return instance. */
            if ((nordic_internal_uart[index].tx == tx) && (nordic_internal_uart[index].rx == rx)) {

                instance = index;
                break;
            }
        }
    }

    /* No instance was found in dynamic map. */
    if (instance == NC) {

        /* Search dynamic map for empty slot. */
        for (size_t index = 0; index < NORDIC_UART_COUNT; index++) {

            /* Empty slot found, reserve slot by storing pins. */
            if ((nordic_internal_uart[index].tx == NC) && (nordic_internal_uart[index].rx == NC)) {

                nordic_internal_uart[index].tx = tx;
                nordic_internal_uart[index].rx = rx;

                instance = index;
                break;
            }
        }
    }

#if defined(DEFAULT_UART_INSTANCE)
    /* Exhausted all options. Return default value. */
    if (instance == NC) {
        instance = DEFAULT_UART_INSTANCE;
    }
#endif

    DEBUG_PRINTF("UART: %d %d %d\r\n", tx, rx, instance);

    return instance;
}

