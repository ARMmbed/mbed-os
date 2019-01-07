/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#if DEVICE_TRNG
#if !defined(FEATURE_CRYPTOCELL310)
#include "hal/trng_api.h"
#include "hal/critical_section_api.h"

#include "nrf_drv_rng.h"

/* Keep track of instantiated FlashIAP objects. */
static int nordic_trng_counter = 0;

void trng_init(trng_t *obj)
{
    MBED_ASSERT(obj);

    /* Increment global counter. */
    nordic_trng_counter++;

    /* Initialize TRNG on first object only. */
    if (nordic_trng_counter == 1) {
        nrf_drv_rng_init(NULL);
    }
}

void trng_free(trng_t *obj)
{
    MBED_ASSERT(obj);

    /* Decrement global counter. */
    nordic_trng_counter--;

    /* Deinitialize TRNG when all objects have been freed. */
    if (nordic_trng_counter == 0) {
        nrf_drv_rng_uninit();
    }
}

/* Get random data from NRF5x TRNG peripheral.
 *
 * This implementation returns num of random bytes in range <1, length>.
 * For parameters description see trng_api.h file.
 */
int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(output);
    MBED_ASSERT(output_length);

    int result = 0;

    /* Return immediately if requested length is zero. */
    if (length != 0) {

        /* Query how many bytes are available. */
        uint8_t bytes_available;
        nrf_drv_rng_bytes_available(&bytes_available);

        /* If no bytes are cached, block until at least 1 byte is available. */
        if (bytes_available == 0) {
            nrf_drv_rng_block_rand(output, 1);
            *output_length = 1;
        } else {

            /* Get up to the requested number of bytes. */
            if (bytes_available > length) {
                bytes_available = length;
            }

            ret_code_t result = nrf_drv_rng_rand(output, bytes_available);

            /* Set output length with available bytes. */
            if (result == NRF_SUCCESS) {
                *output_length = bytes_available;
            } else {
                *output_length = 0;
            }
        }

        /* Set return value based on how many bytes were read. */
        result = (*output_length == 0) ? -1 : 0;
    }

    return result;
}
#endif/* !FEATURE_CRYPTOCELL310 */
#endif
