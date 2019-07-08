/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "port_api.h"
#include "cy_gpio.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

#ifdef __cplusplus
extern "C" {
#endif

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    obj->port = Cy_GPIO_PortToAddr(port);
    obj->mask = (uint8_t)mask;
    uint32_t driveMode = dir == PIN_INPUT ? CY_GPIO_DM_HIGHZ : CY_GPIO_DM_STRONG;
    for (int i = 0; i < 8; i++) {
        if (0 != (mask & (1 << i))) {
            Cy_GPIO_Pin_FastInit(obj->port, i, driveMode, 0, HSIOM_SEL_GPIO);
        }
    }
}

void port_mode(port_t *obj, PinMode mode)
{
    for (int i = 0; i < 8; i++) {
        if (0 != (obj->mask & (1 << i))) {
            uint32_t origMode = Cy_GPIO_GetDrivemode(obj->port, i);
            Cy_GPIO_SetDrivemode(obj->port, i, (origMode & CY_GPIO_DM_HIGHZ) | mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    for (int i = 0; i < 8; i++) {
        if (0 != (obj->mask & (1 << i))) {
            uint32_t origMode = Cy_GPIO_GetDrivemode(obj->port, i);
            Cy_GPIO_SetDrivemode(obj->port, i, origMode == PIN_OUTPUT
                                 ? (origMode & ~CY_GPIO_DM_HIGHZ)
                                 : (origMode | CY_GPIO_DM_HIGHZ));
        }
    }
}

void port_write(port_t *obj, int value)
{
    obj->port->OUT_SET = value & obj->mask;
    obj->port->OUT_CLR = (~value) & obj->mask;
}

int port_read(port_t *obj)
{
    return obj->port->IN & obj->mask;
}

#ifdef __cplusplus
}
#endif
#endif
