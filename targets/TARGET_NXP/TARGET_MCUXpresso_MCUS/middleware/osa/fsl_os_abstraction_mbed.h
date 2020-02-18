/*! *********************************************************************************
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#if !defined(__FSL_OS_ABSTRACTION_MBED_H__)
#define __FSL_OS_ABSTRACTION_MBED_H__

#include "mbed_critical.h"

/*!
 * @addtogroup os_abstraction_free_rtos
 * @{
 */

/*******************************************************************************
 * Declarations
 ******************************************************************************/
/*! @brief Constant to pass as timeout value in order to wait indefinitely. */
#define OSA_WAIT_FOREVER 0xFFFFFFFFU

/*! @brief OSA's time range in millisecond, OSA time wraps if exceeds this value. */
#define FSL_OSA_TIME_RANGE 0xFFFFFFFFU

#endif // __FSL_OS_ABSTRACTION_MBED_H__