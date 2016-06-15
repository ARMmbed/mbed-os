/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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

 /**@file
 *
 * @defgroup nrf_bootloader_settings Bootloader settings API.
 * @{     
 *  
 * @brief Bootloader settings module interface.
 */
 
#ifndef BOOTLOADER_SETTINGS_H__
#define BOOTLOADER_SETTINGS_H__

#include <stdint.h>
#include "bootloader_types.h"

/**@brief Function for getting the bootloader settings.
 * 
 * @param[out] pp_bootloader_settings Bootloader settings. 
 */
void bootloader_util_settings_get(const bootloader_settings_t ** pp_bootloader_settings);

#endif // BOOTLOADER_SETTINGS_H__

/**@} */
