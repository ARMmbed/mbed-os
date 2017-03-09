/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
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
