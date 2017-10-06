/***************************************************************************//**
 * @file trng_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "trng_api.h"
#include "sl_trng.h"

#if defined(DEVICE_TRNG)
static bool is_trng_enabled = false;

void trng_init(trng_t *obj)
{
    /* Use TRNG0 for all HW entropy collection */
    obj->instance = TRNG0;
    
    if(!is_trng_enabled) {
        sl_trng_init(obj->instance);
        is_trng_enabled = true;
    }
}

void trng_free(trng_t *obj)
{
    /* Don't turn off the TRNG to avoid clearing its FIFO */
    (void) obj;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int ret = sl_trng_poll(obj->instance,
                           output,
                           length,
                           output_length);
    return (ret == 0) ? 0 : -1;
}
#endif /* DEVICE_TRNG */
