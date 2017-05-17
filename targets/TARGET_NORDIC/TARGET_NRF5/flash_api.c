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

#include "flash_api.h"
#include "nrf_nvmc.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"

#if DEVICE_FLASH

int32_t flash_init(flash_t *obj)
{
    (void)(obj);
    uint8_t sd_enabled;
    if ((sd_softdevice_is_enabled(&sd_enabled) == NRF_SUCCESS) && sd_enabled == 1) {
        return -1;
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
    uint8_t sd_enabled;
    if ((sd_softdevice_is_enabled(&sd_enabled) == NRF_SUCCESS) && sd_enabled == 1) {
        return -1;
    }
    nrf_nvmc_page_erase(address);
    return 0;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint8_t sd_enabled;
    if ((sd_softdevice_is_enabled(&sd_enabled) == NRF_SUCCESS) && sd_enabled == 1) {
        return -1;
    }
    /* We will use *_words function to speed up flashing code. Word means 32bit -> 4B
     * or sizeof(uint32_t).
     */
    nrf_nvmc_write_words(address, (const uint32_t *) data, (size / sizeof(uint32_t)));
    return 0;
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
    return NRF_FICR->CODEPAGESIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return 0;
}

#endif

/** @}*/
