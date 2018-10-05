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


#include "ble_debug_assert_handler.h"
#include <string.h>
#include "nrf.h"
#include "ble_error_log.h"
#include "nordic_common.h"

#define MAX_LENGTH_FILENAME 128  /**< Max length of filename to copy for the debug error handlier. */


// WARNING - DO NOT USE THIS FUNCTION IN END PRODUCT. - WARNING
// WARNING -         FOR DEBUG PURPOSES ONLY.         - WARNING
void ble_debug_assert_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // Copying parameters to static variables because parameters may not be accessible in debugger.
    static volatile uint8_t  s_file_name[MAX_LENGTH_FILENAME];
    static volatile uint16_t s_line_num;
    static volatile uint32_t s_error_code;

    strncpy((char *)s_file_name, (const char *)p_file_name, MAX_LENGTH_FILENAME - 1);
    s_file_name[MAX_LENGTH_FILENAME - 1] = '\0';
    s_line_num                           = line_num;
    s_error_code                         = error_code;
    UNUSED_VARIABLE(s_file_name);
    UNUSED_VARIABLE(s_line_num);
    UNUSED_VARIABLE(s_error_code);

    // WARNING: The PRIMASK register is set to disable ALL interrups during writing the error log.
    // 
    // Do not use __disable_irq() in normal operation.
    __disable_irq();

    // This function will write error code, filename, and line number to the flash.
    // In addition, the Cortex-M0 stack memory will also be written to the flash.
    //(void) ble_error_log_write(error_code, p_file_name, line_num);

    // For debug purposes, this function never returns.
    // Attach a debugger for tracing the error cause.
    for (;;)
    {
        // Do nothing.
    }
}

