/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef INCLUDED_WHD_THREAD_INTERNAL_H
#define INCLUDED_WHD_THREAD_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "whd_debug.h"
#include "whd_int.h"

/******************************************************
*                      Macros
******************************************************/

#define WHD_WLAN_KEEP_AWAKE(whd_driver) \
    do { \
        whd_result_t verify_result; \
        whd_driver->internal_info.whd_wlan_status.keep_wlan_awake++; \
        verify_result = whd_ensure_wlan_bus_is_up(whd_driver); \
        whd_assert("Could not bring bus up", (verify_result == WHD_SUCCESS) ); \
    } while (0)
#define WHD_WLAN_LET_SLEEP(whd_driver) \
    do { \
        whd_driver->internal_info.whd_wlan_status.keep_wlan_awake--; \
        if (whd_driver->internal_info.whd_wlan_status.keep_wlan_awake == 0) \
            whd_thread_notify(whd_driver); \
    } while (0)
#define WHD_WLAN_MAY_SLEEP() \
    ( (whd_driver->internal_info.whd_wlan_status.keep_wlan_awake == 0) && \
      (whd_driver->internal_info.whd_wlan_status.state == WLAN_UP) )

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_THREAD_INTERNAL_H */

