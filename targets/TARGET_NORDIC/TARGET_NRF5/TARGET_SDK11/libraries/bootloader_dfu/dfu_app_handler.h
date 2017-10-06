/* 
 * Copyright (c) 2014 Nordic Semiconductor ASA
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

  
/** @file
 *
 * @defgroup nrf_dfu_app_handler DFU BLE packet handling in application
 * @{
 *
 * @brief Handling of DFU BLE packets in the application.
 *
 * @details This module implements the handling of DFU packets for switching 
 *          from an application to the bootloader and start DFU mode. The DFU
 *          packets are transmitted over BLE. 
 *          This module handles only the StartDFU packet, which allows a BLE 
 *          application to expose support for the DFU Service.
 *          The actual DFU Service runs in a dedicated environment after a BLE 
 *          disconnect and reset of the \nRFXX device. 
 *          The host must reconnect and continue the update procedure with 
 *          access to the full DFU Service.
 *
 * @note The application must propagate DFU events to this module by calling
 *       @ref dfu_app_on_dfu_evt from the @ref ble_dfu_evt_handler_t callback.
 */
 
#ifndef DFU_APP_HANDLER_H__
#define DFU_APP_HANDLER_H__

#include "ble_dfu.h"
#include "nrf_svc.h"
#include "bootloader_types.h"
#include "device_manager.h"

#define DFU_APP_ATT_TABLE_POS     0                     /**< Position for the ATT table changed setting. */
#define DFU_APP_ATT_TABLE_CHANGED 1                     /**< Value indicating that the ATT table might have changed. This value will be set in the application-specific context in Device Manager when entering DFU mode. */

/**@brief DFU application reset_prepare function. This function is a callback that allows the 
 *        application to prepare for an upcoming application reset. 
 */
typedef void (*dfu_app_reset_prepare_t)(void);

/**@brief   Function for handling events from the DFU Service. 
 *
 * @details The application must inject this function into the DFU Service or propagate DFU events 
 *          to the dfu_app_handler module by calling this function in the application-specific DFU event 
 *          handler.
 * 
 * @param[in] p_dfu  Pointer to the DFU Service structure to which the include event relates.
 * @param[in] p_evt  Pointer to the DFU event.
 */
void dfu_app_on_dfu_evt(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt);

/**@brief Function for registering a function to prepare a reset.
 *
 * @details The provided function is executed before resetting the system into bootloader/DFU
 *          mode. By registering this function, the caller is notified before the reset and can
 *          thus prepare the application for reset. For example, the application can gracefully
 *          disconnect any peers on BLE, turn of LEDS, ensure that all pending flash operations
 *          have completed, and so on.
 *
 * @param[in] reset_prepare_func  Function to be executed before a reset.
 */
void dfu_app_reset_prepare_set(dfu_app_reset_prepare_t reset_prepare_func);

/**@brief Function for setting the Device Manager application instance.
 *
 * @details This function allows to set the @ref dm_application_instance_t value that is returned by the 
 *          Device Manager when the application registers using @ref dm_register.
 *          If this function is not called, it is not be possible to share bonding information
 *          from the application to the bootloader/DFU when entering DFU mode.
 *
 * @param[in] app_instance Value for the application instance in use.
 */
void dfu_app_dm_appl_instance_set(dm_application_instance_t app_instance);

#endif // DFU_APP_HANDLER_H__

/** @} */
