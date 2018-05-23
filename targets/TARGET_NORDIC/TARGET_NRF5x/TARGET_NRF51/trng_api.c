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

#if (defined(DEVICE_TRNG) && defined(DEVICE_LPTICKER))

#include "hal/trng_api.h"
#include "hal/lp_ticker_api.h"

#include "nrf_drv_rng.h"

#define DEFAULT_TIMEOUT_US (1000*1000)

/* Macro for testing if the SoftDevice is active, regardless of whether the 
 * application is build with the SoftDevice or not.
 */
#if defined(SOFTDEVICE_PRESENT)
#include "nrf_sdm.h"
static uint8_t wrapper(void) {
    uint8_t softdevice_is_enabled;
    ret_code_t result = sd_softdevice_is_enabled(&softdevice_is_enabled);
    return ((result == NRF_SUCCESS) && (softdevice_is_enabled == 1));
}
#define NRF_HAL_SD_IS_ENABLED() wrapper()
#else
#define NRF_HAL_SD_IS_ENABLED() 0
#endif

/** Initialize the TRNG peripheral
 *
 * @param obj The TRNG object
 */
void trng_init(trng_t *obj)
{
    (void) obj;

    /* Initialize low power ticker. Used for timeouts. */
    static bool first_init = true;

    if (first_init) {
        first_init = false;
    }
}

/** Deinitialize the TRNG peripheral
 *
 * @param obj The TRNG object
 */
void trng_free(trng_t *obj)
{
    (void) obj;
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
    (void) obj;

    /* Use SDK RNG driver if SoftDevice is enabled. */
    if (NRF_HAL_SD_IS_ENABLED()) {

        /* Initialize driver once. */
        static bool nordic_driver_init = true;

        if (nordic_driver_init) {
            nordic_driver_init = false;
            nrf_drv_rng_init(NULL);
        }

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
    } else {

        /* Initialize low-level registers once. */
        static bool nordic_register_init = true;

        if (nordic_register_init) {
            nordic_register_init = false;

            /* Enable RNG */
            nrf_rng_error_correction_enable();
            nrf_rng_task_trigger(NRF_RNG_TASK_START);
        }

        /* Copy out one byte at a time. */
        size_t index = 0;
        for ( ;  index < length; index++) {

            /* Setup stop watch for timeout. */
            uint32_t start_us = lp_ticker_read();
            uint32_t now_us = start_us;

            /* Block until timeout or random numer is ready. */
            while (((now_us - start_us) < DEFAULT_TIMEOUT_US) && 
                   !nrf_rng_event_get(NRF_RNG_EVENT_VALRDY)) {
                now_us = lp_ticker_read();            
            }

            /* Abort if timeout was reached. */
            if ((now_us - start_us) >= DEFAULT_TIMEOUT_US) {
                break;
            } else {

                /* Read random byte and clear event in preparation for the next byte. */
                nrf_rng_event_clear(NRF_RNG_EVENT_VALRDY);
                output[index] = nrf_rng_random_value_get();
            }
        }

        /* Set output length with available bytes. */
        *output_length = index;
    }

    /* Set return value based on how many bytes was read. */
    return (*output_length == 0) ? -1 : 0;
}

#endif
