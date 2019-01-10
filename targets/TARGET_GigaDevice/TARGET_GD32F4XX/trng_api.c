/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
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

/** Initialize the TRNG peripheral
 *
 * @param obj The TRNG object
 */
void trng_init(trng_t *obj)
{
    (void)obj;
    /* TRNG module clock enable */
    rcu_periph_clock_enable(RCU_TRNG);

    /* TRNG registers reset */
    trng_deinit();
    trng_enable();
}

/** Deinitialize the TRNG peripheral
 *
 * @param obj The TRNG object
 */
void trng_free(trng_t *obj)
{
    (void)obj;
    trng_disable();
}

/** Get random data from TRNG peripheral
 *
 * @param obj The TRNG object
 * @param output The pointer to an output array
 * @param length The size of output data, to avoid buffer overwrite
 * @param output_length The length of generated data
 * @return 0 success, -1 fail
 */
int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int reval = 0;
    volatile uint8_t tmp[4];
    uint32_t timeout = 0;

    (void)obj;
    *output_length = 0;

    while ((*output_length < length) && (0 == reval)) {
        while (0 == trng_flag_get(TRNG_FLAG_DRDY) && timeout < 0x1FFFF) {
            timeout++;
        }
        if (timeout == 0x1FFFF) {
            reval = -1;
            break;
        }
        *(uint32_t *)tmp = trng_get_true_random_data();
        for (uint8_t i = 0; (i < 4) && (*output_length < length) ; i++) {
            *output++ = tmp[i];
            *output_length += 1;
            tmp[i] = 0;
        }
    }

    /* Determines the status error current and error current */
    if ((1 == trng_flag_get(TRNG_FLAG_CECS)) || (1 == trng_flag_get(TRNG_FLAG_SECS))) {
        reval = -1;
    }

    return reval;
}
#endif /* DEVICE_TRNG */
