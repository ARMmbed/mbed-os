/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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

#if DEVICE_TRNG

#include "trng_api.h"

/*!
 * @brief       Initialize the TRNG peripheral
 *
 * @param       obj The TRNG object
 *
 * @retval      None
 */
void trng_init(trng_t *obj)
{
    (void)obj;
    /* TRNG module clock enable */
    RCM_EnableAHB2PeriphClock(RCM_AHB2_PERIPH_RNG);

    /* TRNG registers reset */
    RNG_Reset();
    RNG_Enable();
}

/*!
 * @brief       Deinitialize the TRNG peripheral
 *
 * @param       obj The TRNG object
 *
 * @retval      None
 */
void trng_free(trng_t *obj)
{
    (void)obj;

    RNG_Disable();
}

/*!
 * @brief       Get random data from TRNG peripheral
 *
 * @param       obj The TRNG object
 *
 * @param       output The pointer to an output array
 *
 * @param       length The size of output data, to avoid buffer overwrite
 *
 * @param       output_length The length of generated data
 *
 * @retval      0 success, -1 fail
 */
int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int value = 0;
    volatile uint8_t temp[4];
    uint32_t timeout = 0;

    (void)obj;

    *output_length = 0;

    while ((*output_length < length) && (value == 0))
    {
        while ((RNG_ReadStatusFlag(RNG_FLAG_DATARDY) == RESET) && timeout < 0x1ffff)
        {
            timeout++;
        }
        if (timeout == 0x1ffff)
        {
            value = -1;
            break;
        }

        *(uint32_t *)temp = RNG_ReadRandomNumber();
        for (uint8_t i = 0; (i < 4) && (*output_length < length); i++)
        {
            *output++ = temp[i];
            *output_length += 1;
            temp[i] = 0;
        }
    }

    if ((RNG_ReadStatusFlag(RNG_FLAG_CLKERCSTS) != RESET) || (RNG_ReadStatusFlag(RNG_FLAG_FSCSTS) != RESET))
    {
        value = -1;
    }

    return value;
}
#endif /* DEVICE_TRNG */
