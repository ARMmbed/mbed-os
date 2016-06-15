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
 
/**@file
 *
 * @defgroup nrf_bootloader Bootloader API.
 * @{     
 *
 * @brief Bootloader module interface.
 */

#ifndef BOOTLOADER_H__
#define BOOTLOADER_H__

#include <stdbool.h>
#include <stdint.h>
#include "bootloader_types.h"
#include <dfu_types.h>

/**@brief Function for initializing the Bootloader.
 * 
 * @retval     NRF_SUCCESS If bootloader was succesfully initialized. 
 */
uint32_t bootloader_init(void);

/**@brief Function for validating application region in flash.
 * 
 * @param[in]  app_addr      Address to the region in flash where the application is stored.
 * 
 * @retval     true          If Application region is valid.
 * @retval     false         If Application region is not valid.
 */
bool bootloader_app_is_valid(uint32_t app_addr);

/**@brief Function for starting the Device Firmware Update.
 * 
 * @retval     NRF_SUCCESS If new application image was successfully transferred.
 */
uint32_t bootloader_dfu_start(void);

/**@brief Function for exiting bootloader and booting into application.
 *
 * @details This function will disable SoftDevice and all interrupts before jumping to application.
 *          The SoftDevice vector table base for interrupt forwarding will be set the application
 *          address.
 *
 * @param[in]  app_addr      Address to the region where the application is stored.
 */
void bootloader_app_start(uint32_t app_addr);

/**@brief Function for retrieving the bootloader settings.
 *
 * @param[out] p_settings    A copy of the current bootloader settings is returned in the structure
 *                           provided.
 */
void bootloader_settings_get(bootloader_settings_t * const p_settings);

/**@brief Function for processing DFU status update.
 *
 * @param[in]  update_status DFU update status.
 */
void bootloader_dfu_update_process(dfu_update_status_t update_status);

/**@brief Function getting state of SoftDevice update in progress.
 *        After a successfull SoftDevice transfer the system restarts in orderto disable SoftDevice
 *        and complete the update.
 *
 * @retval     true          A SoftDevice update is in progress. This indicates that second stage 
 *                           of a SoftDevice update procedure can be initiated.
 * @retval     false         No SoftDevice update is in progress.
 */
bool bootloader_dfu_sd_in_progress(void);

/**@brief Function for continuing the Device Firmware Update of a SoftDevice.
 * 
 * @retval     NRF_SUCCESS If the final stage of SoftDevice update was successful. 
 */
uint32_t bootloader_dfu_sd_update_continue(void);

/**@brief Function for finalizing the Device Firmware Update of a SoftDevice.
 * 
 * @retval     NRF_SUCCESS If the final stage of SoftDevice update was successful. 
 */
uint32_t bootloader_dfu_sd_update_finalize(void);

#endif // BOOTLOADER_H__

/**@} */
