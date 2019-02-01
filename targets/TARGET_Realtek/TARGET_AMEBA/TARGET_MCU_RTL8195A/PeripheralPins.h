/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "cmsis.h"
#include "pinmap.h"

#ifdef __cplusplus
extern "C" {
#endif

// for spi_api.c
extern const PinMap PinMap_SSI_MOSI[];
extern const PinMap PinMap_SSI_MISO[];
extern const PinMap PinMap_SSI_SCLK[];
extern const PinMap PinMap_SSI_SSEL[];

// for serial_api.c
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];

// for pwmout_api.c
extern const PinMap PinMap_PWM[];

// for i2c_api.c
extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];

// for analogin_api.c
extern const PinMap PinMap_ADC[];

// for analogout_api.c
extern const PinMap PinMap_DAC[];

#ifdef __cplusplus
}
#endif

#endif

