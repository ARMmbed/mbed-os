/***************************************************************************//**
 * @file rtc_api_HAL.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#ifndef MBED_RTC_API_HAL_H
#define MBED_RTC_API_HAL_H

#include <stdint.h>
#include "rtc_api.h"
#include "em_rtc.h"

#define RTC_CLOCKDIV 		cmuClkDiv_8
#define RTC_CLOCKDIV_INT	8
#define RTC_FREQ_SHIFT 		12


#define RTC_INIT_LPTIMER (1 << 1)
#define RTC_INIT_RTC     (1 << 0)

#ifdef __cplusplus
extern "C" {
#endif

/* Purpose of this file: extend rtc_api.h to include EFM-specific stuff*/
void rtc_set_comp0_handler(uint32_t handler);

void rtc_init_real(uint32_t flags);
void rtc_free_real(uint32_t flags);

#ifdef __cplusplus
}
#endif

#endif
