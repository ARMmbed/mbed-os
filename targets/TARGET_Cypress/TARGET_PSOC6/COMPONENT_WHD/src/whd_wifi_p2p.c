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

#include "whd_ap.h"
#include "whd_chip_constants.h"
#include "whd_debug.h"
#include "whd_events_int.h"
#include "whd_sdpcm.h"
#include "whd_thread_internal.h"
#include "whd_utils.h"
#include "whd_wifi_api.h"
#include "whd_wifi_p2p.h"


/******************************************************
* @cond       Constants
******************************************************/

whd_bool_t whd_wifi_p2p_is_go_up(whd_driver_t whd_driver)
{
    return whd_driver->internal_info.whd_wifi_p2p_go_is_up;
}

void whd_wifi_p2p_set_go_is_up(whd_driver_t whd_driver, whd_bool_t is_up)
{
    if (whd_driver->internal_info.whd_wifi_p2p_go_is_up != is_up)
    {
        whd_driver->internal_info.whd_wifi_p2p_go_is_up = is_up;
    }
}

