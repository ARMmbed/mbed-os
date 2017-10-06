/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BTLE_H_
#define _BTLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common.h"

#include "ble_srv_common.h"
#include "headers/nrf_ble.h"

error_t     btle_init(void);

// flag indicating if events have been signaled or not
// It is used by processEvents and signalEventsToProcess
// signalEventsToProcess raise the flag and processEvents
// clears it.
extern bool isEventsSignaled;

#ifdef __cplusplus
}
#endif

#endif // ifndef _BTLE_H_
