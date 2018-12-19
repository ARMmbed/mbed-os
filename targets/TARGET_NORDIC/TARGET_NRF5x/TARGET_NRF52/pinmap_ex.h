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

#ifndef PINMAP_EX_H
#define PINMAP_EX_H

#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Data structure for pre-allocated I2C instances. */
typedef struct {
    PinName sda;
    PinName scl;
    int instance;
} PinMapI2C;

extern const PinMapI2C PinMap_I2C[];

/* Data structure for pre-allocated SPI instances. */
typedef struct {
    PinName mosi;
    PinName miso;
    PinName clk;
    int instance;
} PinMapSPI;

extern const PinMapSPI PinMap_SPI[];

/* Data structure for pre-allocated PWM instances. */
typedef struct {
    PinName pwm;
    int instance;
} PinMapPWM;

extern const PinMapPWM PinMap_PWM[];

/* Data structure for pre-allocated UART instances. */
typedef struct {
    PinName tx;
    PinName rx;
    int instance;
} PinMapUART;

extern const PinMapUART PinMap_UART[];

/**
 * @brief      Find hardware instance for the provided I2C pins.
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
 * @param[in]  sda   sda pin.
 * @param[in]  scl   scl pin.
 *
 * @return     Hardware instance associated with provided pins.
 */
int pin_instance_i2c(PinName sda, PinName scl);

/**
 * @brief      Find hardware instance for the provided SPI pins.
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
 * @param[in]  mosi   mosi pin.
 * @param[in]  miso   miso pin.
 * @param[in]  clk    clk pin.
 *
 * @return     Hardware instance associated with provided pins.
 */
int pin_instance_spi(PinName mosi, PinName miso, PinName clk);

/**
 * @brief      Find hardware instance for the provided PWM pins.
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
 * @param[in]  pwm    pwm pin.
 *
 * @return     Hardware instance associated with provided pins.
 */
int pin_instance_pwm(PinName pwm);

/**
 * @brief      Find hardware instance for the provided UART pins.
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
 * @param[in]  tx     tx pin.
 * @param[in]  rx     rx pin.
 *
 * @return     Hardware instance associated with provided pins.
 */
int pin_instance_uart(PinName tx, PinName rx);

#ifdef __cplusplus
}
#endif

#endif // PINMAP_EX_H
