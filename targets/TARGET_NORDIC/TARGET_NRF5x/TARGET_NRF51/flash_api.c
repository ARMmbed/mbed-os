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

#if (defined(DEVICE_FLASH) && defined(DEVICE_LPTICKER))

#include "hal/flash_api.h"
#include "hal/lp_ticker_api.h"

#include "nrf_drv_common.h"
#include "nrf_nvmc.h"
#include "nrf_soc.h"

#define WORD_WRITE_TIMEOUT_US (1 * 1000)    // Max. value from datasheet: 338 us
#define PAGE_ERASE_TIMEOUT_US (200 * 1000)  // Max. value from datasheet: 89.7 ms

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

int32_t flash_init(flash_t *obj)
{
    (void)(obj);

    /* Initialize low power ticker. Used for timeouts. */
    static bool first_init = true;

    if (first_init) {
        first_init = false;
    }

    return 0;
}

int32_t flash_free(flash_t *obj)
{
    (void)(obj);
    
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    (void)(obj);

    /* Return value defaults to error. */
    uint32_t result = NRF_ERROR_BUSY;

    if (NRF_HAL_SD_IS_ENABLED()) {

        /* Convert address to page number. */
        uint32_t page_number = address / NRF_FICR->CODEPAGESIZE;

        /* Setup stop watch for timeout. */
        uint32_t start_us = lp_ticker_read();
        uint32_t now_us = start_us;

        /* Retry if flash is busy until timeout is reached. */
        while (((now_us - start_us) < PAGE_ERASE_TIMEOUT_US) &&
               (result == NRF_ERROR_BUSY)) {

            result = sd_flash_page_erase(page_number);

            /* Read timeout timer. */
            now_us = lp_ticker_read();            
        }

    } else {

        /* Raw API doesn't return error code, assume success. */
        nrf_nvmc_page_erase(address);
        result = NRF_SUCCESS;
    }

    /* Convert Nordic error code to mbed HAL error code. */
    return (result == NRF_SUCCESS) ? 0 : -1;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    (void)(obj);

    /* Return value defaults to error. */
    uint32_t result = NRF_ERROR_BUSY;

    /* Convert size to words. */
    uint32_t words = size / sizeof(uint32_t);

    if (NRF_HAL_SD_IS_ENABLED()) {

        /* Setup stop watch for timeout. */
        uint32_t start_us = lp_ticker_read();
        uint32_t now_us = start_us;

        /* Retry if flash is busy until timeout is reached. */
        while (((now_us - start_us) < (words * WORD_WRITE_TIMEOUT_US)) &&
               (result == NRF_ERROR_BUSY)) {

            result = sd_flash_write((uint32_t *) address, (const uint32_t *) data, words);

            /* Read timeout timer. */
            now_us = lp_ticker_read();            
        }

    } else {
        /* We will use *_words function to speed up flashing code. Word means 32bit -> 4B
         * or sizeof(uint32_t).
         */
        nrf_nvmc_write_words(address, (const uint32_t *) data, words);
        result = NRF_SUCCESS;
    }

    /* Convert Nordic error code to mbed HAL error code. */
    return (result == NRF_SUCCESS) ? 0 : -1;
}

uint32_t flash_get_size(const flash_t *obj)
{
    (void)(obj);

    /* Just count flash size. */
    return NRF_FICR->CODESIZE * NRF_FICR->CODEPAGESIZE;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    (void)(obj);

    /* Test if passed address is in flash space. */
    if (address < flash_get_size(obj)) {
        return NRF_FICR->CODEPAGESIZE;
    }

    /* Something goes wrong, return invalid size error code. */
    return MBED_FLASH_INVALID_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    (void)(obj);

    /* Return minimum writeable size. Note that this is different from the erase page size. */
    return 4;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    (void)(obj);

    return 0;
}

#endif

/** @}*/
