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
#include <string.h>
#include "nsdynmemLIB.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss_mac_interface_stub.h"
#include "fhss_config_stub.h"

fhss_config_stub_def fhss_config_stub;

fhss_api_t *ns_fhss_create(const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer)
{
    fhss_config_stub.fhss_api_ptr.is_broadcast_channel = &fhss_is_broadcast_channel_cb;
    fhss_config_stub.fhss_api_ptr.use_broadcast_queue = &fhss_use_broadcast_queue_cb;
    fhss_config_stub.fhss_api_ptr.tx_handle = &fhss_tx_handle_cb;
    fhss_config_stub.fhss_api_ptr.check_tx_conditions = &fhss_check_tx_conditions_cb;
    fhss_config_stub.fhss_api_ptr.receive_frame = &fhss_receive_frame_cb;
    fhss_config_stub.fhss_api_ptr.data_tx_done = &fhss_data_tx_done_cb;
    fhss_config_stub.fhss_api_ptr.data_tx_fail = &fhss_data_tx_fail_cb;
    fhss_config_stub.fhss_api_ptr.synch_state_set = &fhss_synch_state_set_cb;
    fhss_config_stub.fhss_api_ptr.read_timestamp = &fhss_read_timestamp_cb;
    fhss_config_stub.fhss_api_ptr.get_retry_period = &fhss_get_retry_period_cb;
    fhss_config_stub.fhss_api_ptr.init_callbacks = &fhss_init_callbacks_cb;
    return &fhss_config_stub.fhss_api_ptr;
}
