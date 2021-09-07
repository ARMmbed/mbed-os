/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef __LP_TICKER_DEFINES_H
#define __LP_TICKER_DEFINES_H

#define LP_TICKER_PERIOD_NUM    1000000

#if MBED_CONF_TARGET_LPTICKER_LPTIM

#if MBED_CONF_TARGET_LSE_AVAILABLE
#define LP_TICKER_PERIOD_DEN    (LSE_VALUE / MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK)
#else
#define LP_TICKER_PERIOD_DEN    (LSI_VALUE / MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK)
#endif
#define LP_TICKER_MASK          0xFFFF      // 16 bits

#else // MBED_CONF_TARGET_LPTICKER_LPTIM == 0 => Use RTC WAKEUP timer

#if MBED_CONF_TARGET_LSE_AVAILABLE
#define LP_TICKER_PERIOD_DEN    (LSE_VALUE / 4) // RTC_CLOCK / RTC_WAKEUPCLOCK_RTCCLK_DIV4
#else
#define LP_TICKER_PERIOD_DEN    (LSI_VALUE / 4) // RTC_CLOCK / RTC_WAKEUPCLOCK_RTCCLK_DIV4
#endif
#define LP_TICKER_MASK          0xFFFFFFFF  // 32 bits

#endif // MBED_CONF_TARGET_LPTICKER_LPTIM

#endif // __LP_TICKER_DEFINES_H
