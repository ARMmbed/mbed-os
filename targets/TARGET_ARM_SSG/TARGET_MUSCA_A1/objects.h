/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 Arm Limited
 *
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

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "platform_retarget_dev.h"
#include "gpio_object.h"

#ifdef __cplusplus
extern "C" {
#endif

struct serial_s {
    struct uart_pl011_dev_t *uart_dev;
    UARTName uart_index;                /* UART device number */
    IRQn_Type tx_irq;
    IRQn_Type rx_irq;
    IRQn_Type rx_timeout_irq;
};

#ifdef __cplusplus
}
#endif

#endif
