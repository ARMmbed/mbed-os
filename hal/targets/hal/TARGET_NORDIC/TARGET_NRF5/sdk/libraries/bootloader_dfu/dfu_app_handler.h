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
