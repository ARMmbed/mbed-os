/*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component:   Watchdog
 * File:        cb_watchdog.h
 *
 * Description: Functionality for watchdog and reset.
 *-------------------------------------------------------------------------*/
#ifndef _CB_WATCHDOG_H_
#define _CB_WATCHDOG_H_

#include "cb_comdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/

/*===========================================================================
 * TYPES
 *=========================================================================*/


/*===========================================================================
 * FUNCTIONS
 *=========================================================================*/

/**
* Put watchdog in a defined state.
*/
void cbWD_init(void);

/**
* Resets the CPU.
*/
void cbWD_systemReset(void);

/**
* Enables watchdog. Watchdog needs to be polled using cbWD_poll() with
* shorter intervals then specified by timeInMilliseconds.
*
* @param timeInMilliseconds Watchdog timeout in milliseconds.
*/
void cbWD_enable(cb_uint32 timeInMilliseconds);

/**
* Poll the watchdog timer. This must be done with shorter intervalls
* than the time in cbWD_enable().
*/
void cbWD_poll(void);

#ifdef __cplusplus
}
#endif

#endif /* _CB_WATCHDOG_H_ */
