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

#ifdef __cplusplus
extern "C" {
#endif

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


#ifdef __cplusplus
}
#endif

#endif /* BLE_DEBUG_ASSERT_HANDLER_H__ */


/** @} */
