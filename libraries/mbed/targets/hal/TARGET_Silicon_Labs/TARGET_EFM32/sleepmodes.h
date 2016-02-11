/***************************************************************************//**
 * @file sleepmodes.h
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

#ifndef MBED_SLEEPMODES_H
#define MBED_SLEEPMODES_H

#include "em_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* Blocks all sleepmodes below the one passed as argument
*
* @param minimumMode The lowest-power sleep mode which must remain active
*/
void blockSleepMode(sleepstate_enum minimumMode);

/*
* Unblocks a previously-blocked sleep mode
* 
*@param minimumMode The same sleepmode that was previously passed to blockSleepMode
*/
void unblockSleepMode(sleepstate_enum minimumMode);

#ifdef __cplusplus
}
#endif

#endif
