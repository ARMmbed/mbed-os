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

#include "cyabs_rtos.h" /* For semaphores */
#include "whd.h"


#ifndef INCLUDED_WHD_WIFI_P2P_H
#define INCLUDED_WHD_WIFI_P2P_H


#ifdef __cplusplus
extern "C"
{
#endif

/** @cond !ADDTHIS*/

/**
 * Set whether the p2p GO is up or not
 * @param is_up   : specify whether the p2p GO is up currently or not
 */
extern void whd_wifi_p2p_set_go_is_up(whd_driver_t whd_driver, whd_bool_t is_up);

/**
 * @return WHD_TRUE if the P2P GO is currently up
 */
extern whd_bool_t whd_wifi_p2p_is_go_up(whd_driver_t whd_driver);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_WIFI_P2P_H */

