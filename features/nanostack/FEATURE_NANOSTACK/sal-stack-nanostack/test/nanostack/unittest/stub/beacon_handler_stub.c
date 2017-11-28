/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include <string.h>
#include "beacon_handler.h"
#include "mlme.h"


void beacon_received(int8_t if_id, const mlme_beacon_ind_t* data)
{
}

void beacon_optional_tlv_fields_skip(uint16_t *len, uint8_t **ptr, uint8_t offset)
{
}

int8_t mac_beacon_link_beacon_compare_rx_callback_set(int8_t interface_id,
    beacon_compare_rx_cb *beacon_compare_rx_cb_ptr)
{
    return -1;
}

int8_t mac_beacon_link_beacon_join_priority_tx_callback_set(int8_t interface_id,
    beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr)
{
    return -1;
}

void beacon_join_priority_update(int8_t interface_id)
{

}
