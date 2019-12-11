/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/*
 * This file contains some bitband alias helper macros,
 * so that the source code is more readable.
 */

#ifndef GPIO_MSP432_H
#define GPIO_MSP432_H

#include <stdint.h>
#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

// GPIO register offsets
#define REG_IN   0x00
#define REG_OUT  0x02
#define REG_DIR  0x04
#define REG_REN  0x06
#define REG_SEL0 0x0a
#define REG_SEL1 0x0c
#define REG_IES  0x18
#define REG_IE   0x1a
#define REG_IFG  0x1c

/*
 *  Bit-Band base addresses of the 10 GPIO ports
 */
extern __IO uint8_t *gpio_bitband_base[10];

/*
 * Port base addresses of the 10 GPIO ports
 */
extern __IO uint8_t *port_base[10];

/*
 * Macro to calculate the bitband base address of a single pin.
 * (within the IN-register, which has an offset of 0)
 * port range: 0..10, corresponding to P1...P10,PJ
 * pin  range: 0..7 , corresponding to pin index
 */
#define PIN_BASE(port,pin) ((__IO uint8_t *) (gpio_bitband_base[port] + (pin<<2) ))

/*
 * Macro to calculate the adrdess of a pin within a given register.
 * base: pin base address
 * reg: offset to a specific register (REG_xxx)
 */
#define PIN_REG(base,reg) (*(base + (reg<<5)))

/*
 * Macro to calculate the adrdess of a port register.
 * base: port base address
 * reg:  offset to a specific register (REG_xxx)
 */
#define PORT_REG(base,reg) (base + reg)

#ifdef __cplusplus
}
#endif

#endif
