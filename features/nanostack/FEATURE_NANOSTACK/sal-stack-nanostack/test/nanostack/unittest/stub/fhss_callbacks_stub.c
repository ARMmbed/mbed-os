/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "fhss_api.h"
#include "fhss_callbacks_stub.h"

fhss_callbacks_stub_def fhss_callbacks_stub;

uint16_t mac_read_tx_queue_sizes(const fhss_api_t *fhss_api, bool broadcast_queue)
{
    return fhss_callbacks_stub.uint8_value;
}

int mac_read_64bit_mac_address(const fhss_api_t *fhss_api, uint8_t *mac_address)
{
    return 0;
}

uint32_t mac_read_phy_datarate(const fhss_api_t *fhss_api)
{
    return 250000;
}

int mac_set_channel(const fhss_api_t *fhss_api, uint8_t channel_number)
{
    fhss_callbacks_stub.uint8_value = channel_number;
    return 0;
}

int mac_fhss_frame_tx(const fhss_api_t *fhss_api, int frame_type)
{
    return 0;
}

int mac_synch_lost(const fhss_api_t *fhss_api)
{
    return 0;
}

int mac_poll_tx_queue(const fhss_api_t *fhss_api)
{
    return 0;
}

int mac_broadcast_notification(const fhss_api_t *fhss_api, uint32_t broadcast_time)
{
    return 0;
}

int mac_get_coordinator_mac_address(const fhss_api_t *fhss_api, uint8_t *mac_address)
{
    if (!fhss_callbacks_stub.int8_value) {
        memcpy(mac_address, fhss_callbacks_stub.address, 8);
    }
    return fhss_callbacks_stub.int8_value;
}
