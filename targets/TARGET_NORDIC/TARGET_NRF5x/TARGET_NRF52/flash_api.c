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

#if DEVICE_FLASH && DEVICE_LPTICKER

#include "hal/flash_api.h"
#include "hal/lp_ticker_api.h"

#include "nrf_fstorage.h"

#if defined(SOFTDEVICE_PRESENT)
#include "nrf_fstorage_sd.h"
#else
#include "nrf_fstorage_nvmc.h"
#endif

#include <stdbool.h>

#define PAGE_ERASE_TIMEOUT_US (200 * 1000)  // Max. value from datasheet: 89.7 ms
#define WORD_SIZE_IN_BYTES 4

NRF_FSTORAGE_DEF(static nrf_fstorage_t nordic_fstorage) = { 0 };

int32_t flash_init(flash_t *obj)
{
    (void)(obj);

    ret_code_t result = NRF_SUCCESS;

    /* Only initialize once. */
    static bool do_init = true;

    if (do_init) {
        do_init = false;

        /* Set instance to cover the whole flash. */
        nordic_fstorage.p_flash_info    = NULL;
        nordic_fstorage.evt_handler     = NULL;
        nordic_fstorage.start_addr      = 0;
        nordic_fstorage.end_addr        = NRF_FICR->CODESIZE * NRF_FICR->CODEPAGESIZE;

        /* Initialize either SoftDevice API or NVMC API. 
         * SoftDevice API should work both when the SoftDevice is enabled or disabled.
         * NVMC API is used when the SoftDevice is not present.
         */
#if defined(SOFTDEVICE_PRESENT)
        result = nrf_fstorage_init(&nordic_fstorage, &nrf_fstorage_sd, NULL);
#else
        result = nrf_fstorage_init(&nordic_fstorage, &nrf_fstorage_nvmc, NULL);
#endif

    }

    /* Convert Nordic SDK error code to mbed HAL. */
    return (result == NRF_SUCCESS) ? 0 : -1;
}

int32_t flash_free(flash_t *obj)
{
    (void)(obj);

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    (void)(obj);

    ret_code_t result = NRF_ERROR_NO_MEM;

    /* Setup stop watch for timeout. */
    uint32_t start_us = lp_ticker_read();
    uint32_t now_us = start_us;

    /* Retry if flash is busy until timeout is reached. */
    while (((now_us - start_us) < PAGE_ERASE_TIMEOUT_US) &&
           (result == NRF_ERROR_NO_MEM)) {

        /* Map mbed HAL call to Nordic fstorage call. */
        result = nrf_fstorage_erase(&nordic_fstorage, address, 1, NULL);

        /* Read timeout timer. */
        now_us = lp_ticker_read();
    }

    /* Convert Nordic SDK error code to mbed HAL. */
    return (result == NRF_SUCCESS) ? 0 : -1;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    (void)(obj);

    /* Check that data pointer is not NULL. */
    ret_code_t result = NRF_ERROR_NULL;

    if (data) {

        /* nrf_fstorage_write only accepts word aligned input buffers.
         * Cast data pointer to word pointer and read word into temporary word variable
         * which should be word aligned. This should be safe on Cortex-m4 which is able
         * to load unaligned data.
         */
        const uint32_t *data_word = (const uint32_t *) data;

        /* Loop through input buffer 4 bytes at a time. */
        for (size_t index = 0; index < (size / WORD_SIZE_IN_BYTES); index++) {

            /* Load 4 bytes into temporary variable. */
            uint32_t word = data_word[index];

            /* Setup stop watch for timeout. */
            uint32_t start_us = lp_ticker_read();
            uint32_t now_us = start_us;

            /* Retry if flash is busy until timeout is reached. */
            do {
                /* Write one word at a time. */
                result = nrf_fstorage_write(&nordic_fstorage, 
                                            address + (WORD_SIZE_IN_BYTES * index), 
                                            &word, 
                                            WORD_SIZE_IN_BYTES, 
                                            NULL);

                /* Read timeout timer. */
                now_us = lp_ticker_read();            

            /* Loop if queue is full or until time runs out. */
            } while (((now_us - start_us) < PAGE_ERASE_TIMEOUT_US) &&
                     (result == NRF_ERROR_NO_MEM));

            /* Break loop if write command wasn't accepted. */
            if (result != NRF_SUCCESS) {
                break;
            }
        }
    }

    /* Convert Nordic SDK error code to mbed HAL. */
    return (result == NRF_SUCCESS) ? 0 : -1;
}

uint32_t flash_get_size(const flash_t *obj)
{
    (void)(obj);

    return nordic_fstorage.end_addr - nordic_fstorage.start_addr;
    ;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    (void)(obj);

    /* Check if passed address is in flash area. Note end_addr is outside valid range. */
    if ((address >= nordic_fstorage.start_addr) && (address < nordic_fstorage.end_addr)) {
        return nordic_fstorage.p_flash_info->erase_unit;
    }

    /* Return invalid size if request is outisde flash area. */
    return MBED_FLASH_INVALID_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    (void)(obj);

    /* Return minimum writeable page size. */
    return WORD_SIZE_IN_BYTES;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return 0;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
