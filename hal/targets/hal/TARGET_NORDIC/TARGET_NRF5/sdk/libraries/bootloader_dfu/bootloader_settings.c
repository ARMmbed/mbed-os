/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
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


#include "bootloader_settings.h"
#include <stdint.h>
#include <dfu_types.h>

#if defined ( __CC_ARM )

uint8_t  m_boot_settings[CODE_PAGE_SIZE]    __attribute__((at(BOOTLOADER_SETTINGS_ADDRESS))) __attribute__((used));                 /**< This variable reserves a codepage for bootloader specific settings, to ensure the compiler doesn't locate any code or variables at his location. */
uint32_t m_uicr_bootloader_start_address    __attribute__((at(NRF_UICR_BOOT_START_ADDRESS))) = BOOTLOADER_REGION_START;             /**< This variable makes the linker script write the bootloader start address to the UICR register. This value will be written in the HEX file and thus written to UICR when the bootloader is flashed into the chip. */

#elif defined ( __GNUC__ )

uint8_t m_boot_settings[CODE_PAGE_SIZE] __attribute__ ((section(".bootloaderSettings")));                                           /**< This variable reserves a codepage for bootloader specific settings, to ensure the compiler doesn't locate any code or variables at his location. */
volatile uint32_t m_uicr_bootloader_start_address  __attribute__ ((section(".uicrBootStartAddress"))) = BOOTLOADER_REGION_START;    /**< This variable ensures that the linker script will write the bootloader start address to the UICR register. This value will be written in the HEX file and thus written to UICR when the bootloader is flashed into the chip. */

#elif defined ( __ICCARM__ )

__no_init uint8_t m_boot_settings[CODE_PAGE_SIZE] @ BOOTLOADER_SETTINGS_ADDRESS;                                                    /**< This variable reserves a codepage for bootloader specific settings, to ensure the compiler doesn't locate any code or variables at his location. */
__root    const uint32_t m_uicr_bootloader_start_address @ NRF_UICR_BOOT_START_ADDRESS = BOOTLOADER_REGION_START;                   /**< This variable ensures that the linker script will write the bootloader start address to the UICR register. This value will be written in the HEX file and thus written to UICR when the bootloader is flashed into the chip. */

#endif

#if defined ( NRF52 )
#if defined ( __CC_ARM )

uint8_t m_mbr_params_page[CODE_PAGE_SIZE]   __attribute__((at(BOOTLOADER_MBR_PARAMS_PAGE_ADDRESS))) __attribute__((used));          /**< This variable reserves a codepage for mbr parameters, to ensure the compiler doesn't locate any code or variables at his location. */
uint32_t m_uicr_mbr_params_page_address     __attribute__((at(NRF_UICR_MBR_PARAMS_PAGE_ADDRESS))) 
                                                = BOOTLOADER_MBR_PARAMS_PAGE_ADDRESS;                                               /**< This variable makes the linker script write the mbr parameters page address to the UICR register. This value will be written in the HEX file and thus written to the UICR when the bootloader is flashed into the chip */

#elif defined (__GNUC__ )

uint8_t m_mbr_params_page[CODE_PAGE_SIZE]           __attribute__ ((section(".mbrParamsPage")));                                    /**< This variable reserves a codepage for mbr parameters, to ensure the compiler doesn't locate any code or variables at his location. */
volatile uint32_t m_uicr_mbr_params_page_address    __attribute__ ((section(".uicrMbrParamsPageAddress")))
                                                = BOOTLOADER_MBR_PARAMS_PAGE_ADDRESS;                                               /**< This variable makes the linker script write the mbr parameters page address to the UICR register. This value will be written in the HEX file and thus written to the UICR when the bootloader is flashed into the chip */

#elif defined (__ICCARM__ )

__no_init uint8_t m_mbr_params_page[CODE_PAGE_SIZE]     @ BOOTLOADER_MBR_PARAMS_PAGE_ADDRESS;                                       /**< This variable reserves a codepage for bootloader specific settings, to ensure the compiler doesn't locate any code or variables at his location. */
__root    const uint32_t m_uicr_mbr_params_page_address @ NRF_UICR_MBR_PARAMS_PAGE_ADDRESS = BOOTLOADER_MBR_PARAMS_PAGE_ADDRESS;    /**< This variable ensures that the linker script will write the bootloader start address to the UICR register. This value will be written in the HEX file and thus written to UICR when the bootloader is flashed into the chip. */

#endif
#endif //defined ( NRF52 )


void bootloader_util_settings_get(const bootloader_settings_t ** pp_bootloader_settings)
{
    // Read only pointer to bootloader settings in flash. 
    bootloader_settings_t const * const p_bootloader_settings =
        (bootloader_settings_t *)&m_boot_settings[0];        

    *pp_bootloader_settings = p_bootloader_settings;
}
