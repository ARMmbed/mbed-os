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


/** @cond */
/**@file
 *
 * @ingroup sdk_util
 * @defgroup sdk_common_macros SDK Common Header
 * @breif Macros for parameter checking and similar tasks
 * @{
 */

#ifndef SDK_MACROS_H__
#define SDK_MACROS_H__

/**@brief Macro for verifying that the module is initialized. It will cause the function to return
 *        @ref NRF_ERROR_INVALID_STATE if not.
 */
#ifdef DISABLE_PARAM_CHECK
#define VERIFY_MODULE_INITIALIZED()
#else
#ifdef MODULE_INITIALIZED
#define VERIFY_MODULE_INITIALIZED()                 \
do                                          \
{                                           \
    if (!MODULE_INITIALIZED)                \
    {                                       \
        return NRF_ERROR_INVALID_STATE;     \
    }                                       \
} while(0)
#else
#define VERIFY_MODULE_INITIALIZED()
#endif /* MODULE_INITIALIZED  */
#endif /* DISABLE_PARAM_CHECK */


/**@brief Macro for verifying that the module is initialized. It will cause the function to return
 *        if not.
 */
#ifdef DISABLE_PARAM_CHECK
#define VERIFY_MODULE_INITIALIZED_VOID()
#else
#ifdef MODULE_INITIALIZED
#define VERIFY_MODULE_INITIALIZED_VOID()            \
do                                                  \
{                                                   \
    if (!MODULE_INITIALIZED)                        \
    {                                               \
        return;                                     \
    }                                               \
} while(0)
#else
#define VERIFY_MODULE_INITIALIZED_VOID()
#endif /* MODULE_INITIALIZED  */
#endif /* DISABLE_PARAM_CHECK */




/** @} */
/** @endcond */
#endif // SDK_MACROS_H__

