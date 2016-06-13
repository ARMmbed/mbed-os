/***************************************************************************//**
 * @file sleepmodes.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
