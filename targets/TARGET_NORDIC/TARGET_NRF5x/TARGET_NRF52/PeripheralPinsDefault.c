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

/* Default mapping between I2C pins and I2C instance.
 * Can be overwritten by user.
 */
MBED_WEAK const PinMapI2C PinMap_I2C[1] = {
    { NC, NC, NC }
};

/* Default mapping between SPI pins and SPI instance.
 * Can be overwritten by user.
 */
MBED_WEAK const PinMapSPI PinMap_SPI[1] = {
    { NC, NC, NC, NC }
};

/* Default mapping between PWM pins and PWM instance.
 * Can be overwritten by user.
 */
MBED_WEAK const PinMapPWM PinMap_PWM[1] = {
    { NC, NC }
};

/* Default mapping between UART pins and UART instance.
 * Can be overwritten by user.
 */
MBED_WEAK const PinMapUART PinMap_UART[1] = {
    { NC, NC, NC }
};
