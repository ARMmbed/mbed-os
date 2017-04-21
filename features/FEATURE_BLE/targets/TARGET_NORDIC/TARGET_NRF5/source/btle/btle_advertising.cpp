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
#include "common/common.h"

#include "ble_advdata.h"
#include "btle.h"

/**************************************************************************/
/*!
    @brief      Starts the advertising process

    @returns
*/
/**************************************************************************/
error_t btle_advertising_start(void)
{
    ble_gap_adv_params_t adv_para = {0};

    /* Set the default advertising parameters */
    adv_para.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_para.p_peer_addr = NULL; /* Undirected advertising */
    adv_para.fp          = BLE_GAP_ADV_FP_ANY;
    adv_para.p_whitelist = NULL;
    adv_para.interval    = (CFG_GAP_ADV_INTERVAL_MS * 8) / 5; /* Advertising
                                                               * interval in
                                                               * units of 0.625
                                                               * ms */
    adv_para.timeout     = CFG_GAP_ADV_TIMEOUT_S;

    ASSERT_STATUS( sd_ble_gap_adv_start(&adv_para));

    return ERROR_NONE;
}
