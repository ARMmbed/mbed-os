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

