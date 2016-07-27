/* 
 * Copyright (c) 2012 Nordic Semiconductor ASA
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
 * @defgroup ble_error_log_module Error Log Module
 * @{
 * @ingroup ble_sdk_lib
 * @brief Module for writing error and stack to flash memory.
 *
 * @details It contains functions for writing an error code, line number, filename/message and
 *          the stack to the flash during an error, e.g. in the assert handler.
 *
 */
#ifndef BLE_ERROR_LOG_H__
#define BLE_ERROR_LOG_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble_flash.h"

#define ERROR_MESSAGE_LENGTH  128                                /**< Length of error message to stored. */
#define STACK_DUMP_LENGTH     256                                /**< Length of stack to be stored at max: 64 entries of 4 bytes each. */
#define FLASH_PAGE_ERROR_LOG  (BLE_FLASH_PAGE_END - 2)           /**< Address in flash where stack trace can be stored. */

/**@brief Error Log Data structure.
 *
 * @details The structure contains the error, message/filename, line number as well as the current
 *          stack, at the time where an error occured.
 */
typedef struct
{
    uint16_t                  failure;                           /**< Indication that a major failure has occurred during last execution of the application. */
    uint16_t                  line_number;                       /**< Line number indicating at which line the failure occurred. */
    uint32_t                  err_code;                          /**< Error code when failure occurred. */
    uint8_t                   message[ERROR_MESSAGE_LENGTH];     /**< Will just use the first 128 bytes of filename to store for debugging purposes. */
    uint32_t                  stack_info[STACK_DUMP_LENGTH / 4]; /**< Will contain stack information, can be manually unwinded for debug purposes. */
} ble_error_log_data_t;


/**@brief Function for writing the file name/message, line number, and current program stack
 *        to flash.
 * 
 * @note This function will force the writing to flash, and disregard any radio communication.
 *       USE THIS FUNCTION WITH CARE.
 *
 * @param[in]   err_code    Error code to be logged.
 * @param[in]   p_message   Message to be written to the flash together with stack dump, usually
 *                          the file name where the error occured.
 * @param[in]   line_number Line number where the error occured.
 *
 * @return      NRF_SUCCESS on successful writing of the error log.
 *
 */
uint32_t ble_error_log_write(uint32_t err_code, const uint8_t * p_message, uint16_t line_number);


#endif /* BLE_ERROR_LOG_H__ */

/** @} */
