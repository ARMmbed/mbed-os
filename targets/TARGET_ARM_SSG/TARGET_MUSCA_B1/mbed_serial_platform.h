/*
 * Copyright (c) 2018-2019 Arm Limited
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

#ifndef __MBED_SERIAL_PLATFORM_H__
#define __MBED_SERIAL_PLATFORM_H__

#include "serial_api.h"

#ifdef __cplusplus
extern "C" {
#endif

struct uart_irq_t {
    uart_irq_handler handler;
    uint32_t id;
};

/* Handlers registered */
extern struct uart_irq_t uart_irq[];

void mbed_uart_platform_init(serial_t *obj, UARTName uart);

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL_PLATFORM_H__ */
