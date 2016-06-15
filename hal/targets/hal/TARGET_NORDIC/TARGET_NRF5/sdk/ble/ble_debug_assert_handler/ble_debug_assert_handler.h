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

/** @file
 *
 * @defgroup ble_debug_assert_handler Assert handler for debug purposes
 * @{
 * @ingroup ble_sdk_lib
 * @brief Module for handling of assert during application development when debugging.
 *
 * @details This module may be used during development of an application to facilitate debugging.
 *          It contains a function to write file name, line number and the Stack Memory to flash.
 *          This module is ONLY for debugging purposes and must never be used in final product.
 *
 */
 
#ifndef BLE_DEBUG_ASSERT_HANDLER_H__
#define BLE_DEBUG_ASSERT_HANDLER_H__

#include <stdint.h>
 
/**@brief Function for handling the Debug assert, which can be called from an error handler. 
 *        To be used only for debugging purposes.
 *
 *@details This code will copy the filename and line number into local variables for them to always
 *         be accessible in Keil debugger. The function will also write the ARM Cortex-M0 stack 
 *         memory into flash where it can be retrieved and manually un-winded in order to 
 *         back-trace the location where the error ocured.<br>
 * @warning <b>ALL INTERRUPTS WILL BE DISABLED.</b>
 * 
 * @note    This function will never return but loop forever for debug purposes.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the original handler is called.
 * @param[in] p_file_name Pointer to the file name. 
 */
void ble_debug_assert_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);

#endif /* BLE_DEBUG_ASSERT_HANDLER_H__ */


/** @} */
