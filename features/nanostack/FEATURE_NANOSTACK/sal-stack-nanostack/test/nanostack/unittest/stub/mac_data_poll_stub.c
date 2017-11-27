/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#include <inttypes.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/MAC/mac_data_poll.h"

void mac_data_poll_init_protocol_poll(struct protocol_interface_info_entry *rf_mac_setup)
{

}
void mac_data_poll_disable(struct protocol_interface_info_entry *cur)
{

}

uint32_t mac_data_poll_host_poll_time_max(struct protocol_interface_info_entry *cur)
{
    return 0;
}

void mac_data_poll_protocol_poll_mode_decrement(struct protocol_interface_info_entry *cur)
{

}

void mac_data_poll_protocol_poll_mode_disable(struct protocol_interface_info_entry *cur)
{

}

void mac_data_poll_enable_check(struct protocol_interface_info_entry *cur)
{

}

int8_t mac_data_poll_host_mode_get(struct protocol_interface_info_entry *cur, net_host_mode_t *mode)
{
    return 0;
}

void mac_data_poll_cb_run(int8_t interface_id)
{

}

int8_t mac_data_poll_host_mode_set(struct protocol_interface_info_entry *cur, net_host_mode_t mode, uint32_t poll_time)
{
    return 0;
}

void mac_data_poll_init(struct protocol_interface_info_entry *cur)
{

}

void mac_data_poll_radio_disable_check(struct protocol_interface_rf_mac_setup *rf_mac_setup)
{

}

uint32_t mac_data_poll_get_max_sleep_period(struct protocol_interface_info_entry *cur)
{
    return 0;
}

uint32_t mac_data_poll_host_timeout(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void mac_poll_timer_trig(uint32_t poll_time, protocol_interface_info_entry_t *cur)
{

}

void mac_mlme_poll_confirm(protocol_interface_info_entry_t *cur, const mlme_poll_conf_t *confirm)
{

}
