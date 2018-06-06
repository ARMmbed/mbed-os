/* 
 * Copyright (c) 2006 Nordic Semiconductor ASA
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
 * @brief Utilities for verifying program logic
 */

#ifndef NRF_ASSERT_H_
#define NRF_ASSERT_H_

#include <stdint.h>
#include "nrf.h"
#include "app_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(NDEBUG) && !defined(DEBUG_NRF_USER)
#define DEBUG_NRF_USER
#endif

#if defined(DEBUG_NRF) || defined(DEBUG_NRF_USER)

/** @brief Function for handling assertions.
 *
 *
 * @note
 * This function is called when an assertion has triggered.
 *
 *
 * @post
 * All hardware is put into an idle non-emitting state (in particular the radio is highly
 * important to switch off since the radio might be in a state that makes it send
 * packets continiously while a typical final infinit ASSERT loop is executing).
 *
 *
 * @param line_num The line number where the assertion is called
 * @param file_name Pointer to the file name
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t *file_name);

/*lint -emacro(506, ASSERT) */ /* Suppress "Constant value Boolean */
/*lint -emacro(774, ASSERT) */ /* Suppress "Boolean within 'if' always evaluates to True" */ \

/** @brief Function for checking intended for production code.
 *
 * Check passes if "expr" evaluates to true. */
#define ASSERT(expr) \
if (expr)                                                                     \
{                                                                             \
}                                                                             \
else                                                                          \
{                                                                             \
    assert_nrf_callback((uint16_t)__LINE__, (uint8_t *)__FILE__);             \
}
#else
#ifndef ASSERT
#define ASSERT(expr) //!< Assert empty when disabled
__WEAK void assert_nrf_callback(uint16_t line_num, const uint8_t *file_name);
#endif
#endif /* defined(DEBUG_NRF) || defined(DEBUG_NRF_USER) */

#ifdef __cplusplus
}
#endif

#endif /* NRF_ASSERT_H_ */
