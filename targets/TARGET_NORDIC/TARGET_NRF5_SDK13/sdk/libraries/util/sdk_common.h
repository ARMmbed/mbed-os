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
 * @ingroup experimental_api
 * @defgroup sdk_common SDK Common Header
 * @breif All common headers needed for SDK examples will be included here so that application
 *       developer does not have to include headers on him/herself.
 * @{
 */

#ifndef SDK_COMMON_H__
#define SDK_COMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "nordic_common.h"
#include "compiler_abstraction.h"
#include "sdk_os.h"
#include "sdk_errors.h"
#include "app_util.h"

/**@brief Macro for verifying that the module is initialized. It will cause the function to return
 *        if not.
 *
 * @param[in] param  The variable to check if is NULL.
 */
#ifndef DISABLE_PARAM_CHECK
#define VERIFY_PARAM_NOT_NULL(param)                \
do                                                  \
{                                                   \
    if (param == NULL)                              \
    {                                               \
        return NRF_ERROR_NULL;                      \
    }                                               \
} while(0)
#else
#define VERIFY_PARAM_NOT_NULL()
#endif /* DISABLE_PARAM_CHECK */


/**@brief Macro for verifying that the module is initialized. It will cause the function to return
 *        if not.
 *
 * @param[in] param  The variable to check if is NULL.
 */
#ifndef DISABLE_PARAM_CHECK
#define VERIFY_PARAM_NOT_NULL_VOID(param)           \
do                                                  \
{                                                   \
    if (param == NULL)                              \
    {                                               \
        return;                                     \
    }                                               \
} while(0)
#else
#define VERIFY_PARAM_NOT_NULL_VOID()
#endif /* DISABLE_PARAM_CHECK */


/**@brief Macro for verifying that a function returned NRF_SUCCESS. Will return the err code
 * if not.
 *
 * @param[in] err_code The error code to check.
 */
#ifndef DISABLE_PARAM_CHECK
#define VERIFY_SUCCESS(err_code)            \
do                                          \
{                                           \
    if (err_code != NRF_SUCCESS)            \
    {                                       \
        return err_code;                    \
    }                                       \
} while(0)
#else
#define VERIFY_SUCCESS()
#endif /* DISABLE_PARAM_CHECK */


/**@brief Macro for verifying that a function returned NRF_SUCCESS. Will return if not.
 *
 * @param[in] err_code The error code to check.
 */
#ifndef DISABLE_PARAM_CHECK
#define VERIFY_SUCCESS_VOID(err_code)       \
do                                          \
{                                           \
    if (err_code != NRF_SUCCESS)            \
    {                                       \
        return;                             \
    }                                       \
} while(0)
#else
#define VERIFY_SUCCESS_VOID()
#endif /* DISABLE_PARAM_CHECK */


/**@brief Macro for verifying statement to be true. Will return err_code if not.
*
* @param[in]   statement   Statement to test.
* @param[in]   err_code     Error value to return if test was invalid.
*
* @retval      err_code if test fails.
*/
#define VERIFY_TRUE(statement, err_code)    \
do                                          \
{                                           \
    if (!(statement))                       \
    {                                       \
        return err_code;                    \
    }                                       \
} while(0)


/**@brief Macro for verifying statement to be true. Will return if not.
*
* @param[in]   statement   Statement to test.
*/
#define VERIFY_TRUE_VOID(statement)         \
do                                          \
{                                           \
    if (!(statement))                       \
    {                                       \
        return;                             \
    }                                       \
} while(0)


/**@brief Macro for verifying statement to be false. Will return err_code if not.
*
* @param[in]   statement   Statement to test.
* @param[in]   err_code     Error value to return if test was invalid.
*
* @retval      err_code if test fails.
*/
#define VERIFY_FALSE(statement, err_code)   \
do                                          \
{                                           \
    if ((statement))                        \
    {                                       \
        return err_code;                    \
    }                                       \
} while(0)


/**@brief Macro for verifying statement to be false. Will return if not.
*
* @param[in]   statement    Statement to test.
*/
#define VERIFY_FALSE_VOID(statement)        \
do                                          \
{                                           \
    if ((statement))                        \
    {                                       \
        return;                             \
    }                                       \
} while(0)

/** @} */
/** @endcond */
#endif // SDK_COMMON_H__

