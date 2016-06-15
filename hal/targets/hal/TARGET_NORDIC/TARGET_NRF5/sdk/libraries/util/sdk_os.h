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
 * @defgroup sdk_os SDK OS Abstraction
 * @ingroup experimental_api
 * @details In order to made SDK modules independent of use of an embedded OS, and permit
 *          application with varied task architecture, SDK abstracts the OS specific
 *          elements here in order to make all other modules agnostic to the OS or task
 *          architecture.
 * @{
 */

#ifndef SDK_OS_H__
#define SDK_OS_H__

#define SDK_MUTEX_DEFINE(X)
#define SDK_MUTEX_INIT(X)
#define SDK_MUTEX_LOCK(X)
#define SDK_MUTEX_UNLOCK(X)

/**
 * @defgroup os_data_type Data types.
 */
 
/** @} */
/** @endcond */
#endif // SDK_OS_H__

