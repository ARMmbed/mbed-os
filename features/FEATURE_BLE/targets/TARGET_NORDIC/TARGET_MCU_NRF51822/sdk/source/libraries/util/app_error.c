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

/** @file
 *
 * @defgroup app_error Common application error handler
 * @{
 * @ingroup app_common
 *
 * @brief Common application error handler.
 */

#include "nrf.h"
#include "app_error.h"
#include "compiler_abstraction.h"
#include "nordic_common.h"
#ifdef DEBUG
#include "bsp.h"

/* global error variables - in order to prevent removal by optimizers */
uint32_t m_error_code;
uint32_t m_line_num;
const uint8_t * m_p_file_name;
#endif

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 *
 * Function is implemented as weak so that it can be overwritten by custom application error handler
 * when needed.
 */

/*lint -save -e14 */
__WEAK void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // On assert, the system can only recover with a reset.
#ifndef DEBUG
    NVIC_SystemReset();
#else

#ifdef BSP_DEFINES_ONLY 
    LEDS_ON(LEDS_MASK);
#else
    UNUSED_VARIABLE(bsp_indication_set(BSP_INDICATE_FATAL_ERROR));
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Uncomment the line below to use.
    //ble_debug_assert_handler(error_code, line_num, p_file_name);
#endif // BSP_DEFINES_ONLY

    // The following variable helps Keil keep the call stack visible, in addition, it can be set to
    // 0 in the debugger to continue executing code after the error check.
    volatile bool loop = true;
    UNUSED_VARIABLE(loop);

    m_error_code = error_code;
    m_line_num = line_num;
    m_p_file_name = p_file_name;

    UNUSED_VARIABLE(m_error_code);
    UNUSED_VARIABLE(m_line_num);
    UNUSED_VARIABLE(m_p_file_name);
    __disable_irq();

    while(loop);
#endif // DEBUG
}
/*lint -restore */
