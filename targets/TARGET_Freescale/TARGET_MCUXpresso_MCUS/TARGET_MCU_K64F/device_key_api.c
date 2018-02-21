/*
 *
 *  Copyright (C) 2018, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*
 * Reference: "K64 Sub-Family Reference Manual, Rev. 2", chapter 12.2.19-12.2.22
 */

#if defined(DEVICE_DEVKEY)

#include <stdlib.h>
#include "fsl_common.h"
#include "fsl_clock.h"
#include "device_key_api.h"

/* Key length in bytes. This is platform dependent and can be
 * only 16 or 32 bytes long
 */
#define DEVICE_KEY_SIZE 16

int device_key_get_size_in_bytes()
{
    return DEVICE_KEY_SIZE;
}

/*  The function return the values stored in registers
 *  UIDH, UIDMH, UIDML and UIDL. Please notice: Part of this value
 *  is the device serial number therefore predictable and unsecured.
 *  This code is used as an example only.
 */
int device_key_get_value(uint32_t *output, size_t *size)
{
    if (*size < DEVICE_KEY_SIZE) {
        return DEVICEKEY_HAL_BUFFER_TOO_SMALL;
    }

    memset(output, 0, *size);

    *output++ = SIM->UIDH;
    *output++ = SIM->UIDML;
    *output++ = SIM->UIDMH;
    *output++ = SIM->UIDL;
    *size = sizeof(*output) * 4;

    return DEVICEKEY_HAL_SUCCESS;
}

#endif
