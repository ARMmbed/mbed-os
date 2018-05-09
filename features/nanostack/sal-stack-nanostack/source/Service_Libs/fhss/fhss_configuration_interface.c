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
#include "ns_types.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "net_fhss.h"
#include "nsdynmemLIB.h"
#include "Service_Libs/fhss/fhss.h"
#include "Service_Libs/fhss/fhss_mac_interface.h"
#include "ns_trace.h"

#define TRACE_GROUP "fhss"


fhss_api_t *ns_fhss_create(const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics)
{
    fhss_api_t *this = ns_dyn_mem_alloc(sizeof(fhss_api_t));
    if (!this) {
        return NULL;
    }
    // Create FHSS object
    int8_t retval = fhss_enable(this, fhss_configuration, fhss_timer, fhss_statistics);
    if (retval) {
        tr_err("Failed to enable FHSS, return code: %i", retval);
        ns_dyn_mem_free(this);
        return NULL;
    }
    this->is_broadcast_channel = &fhss_is_broadcast_channel_cb;
    this->use_broadcast_queue = &fhss_use_broadcast_queue_cb;
    this->tx_handle = &fhss_tx_handle_cb;
    this->check_tx_conditions = &fhss_check_tx_conditions_cb;
    this->receive_frame = &fhss_receive_frame_cb;
    this->data_tx_done = &fhss_data_tx_done_cb;
    this->data_tx_fail = &fhss_data_tx_fail_cb;
    this->synch_state_set = &fhss_synch_state_set_cb;
    this->read_timestamp = &fhss_read_timestamp_cb;
    this->get_retry_period = &fhss_get_retry_period_cb;
    this->init_callbacks = &fhss_init_callbacks_cb;
    return this;
}

int ns_fhss_delete(fhss_api_t *fhss_api)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return -1;
    }
    if (fhss_disable(fhss_structure)) {
        return -1;
    }
    ns_dyn_mem_free(fhss_api);
    fhss_api = NULL;
    return 0;
}

int ns_fhss_configuration_set(fhss_api_t *fhss_api, const fhss_synch_configuration_t *fhss_synch_configuration)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return -1;
    }
    return fhss_set_synch_configuration(fhss_structure, fhss_synch_configuration);
}
