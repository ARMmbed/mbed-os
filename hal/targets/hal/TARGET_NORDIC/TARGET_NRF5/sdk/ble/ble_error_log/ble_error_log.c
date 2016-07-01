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


#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <nrf.h>
#include "ble_error_log.h"
#include "app_util.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "pstorage.h"


// Made static to avoid the error_log to go on the stack.
static ble_error_log_data_t m_ble_error_log;  /**< . */
//lint -esym(526,__Vectors)
extern uint32_t * __Vectors;  /**< The initialization vector holds the address to __initial_sp that will be used when fetching the stack. */

static void fetch_stack(ble_error_log_data_t * error_log)
{
    uint32_t * p_stack;
    uint32_t * initial_sp;
    uint32_t   length;

    initial_sp = (uint32_t *) __Vectors;
    p_stack    = (uint32_t *) GET_SP();

    length = ((uint32_t) initial_sp) - ((uint32_t) p_stack);
    memcpy(error_log->stack_info,
           p_stack,
           (length > STACK_DUMP_LENGTH) ? STACK_DUMP_LENGTH : length);
}

uint32_t ble_error_log_write(uint32_t err_code, const uint8_t * p_message, uint16_t line_number)
{
    m_ble_error_log.failure     = true;
    m_ble_error_log.err_code    = err_code;
    m_ble_error_log.line_number = line_number;

    strncpy((char *)m_ble_error_log.message, (const char *)p_message, ERROR_MESSAGE_LENGTH - 1);
    m_ble_error_log.message[ERROR_MESSAGE_LENGTH - 1] = '\0';

    fetch_stack(&m_ble_error_log);

    // Write to flash removed, to be redone.

    return NRF_SUCCESS;
}
