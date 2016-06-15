/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
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
