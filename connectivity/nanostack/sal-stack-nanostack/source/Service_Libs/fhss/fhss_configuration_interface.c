/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include "Service_Libs/fhss/fhss_common.h"
#include "Service_Libs/fhss/fhss_ws.h"
#include "Service_Libs/fhss/fhss_statistics.h"
#include "ns_trace.h"

#define TRACE_GROUP "fhss"

fhss_api_t *ns_fhss_create(const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics)
{
    fhss_api_t *this = ns_dyn_mem_alloc(sizeof(fhss_api_t));
    if (!this) {
        return NULL;
    }
    // Create FHSS object
    fhss_structure_t *fhss_struct = fhss_enable(this, fhss_configuration, fhss_timer, fhss_statistics);
    if (!fhss_struct) {
        tr_err("Failed to enable FHSS");
        ns_dyn_mem_free(this);
        return NULL;
    }
    fhss_set_callbacks(fhss_struct);
    return this;
}

fhss_api_t *ns_fhss_ws_create(const fhss_ws_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer)
{
    fhss_api_t *this = ns_dyn_mem_alloc(sizeof(fhss_api_t));
    if (!this) {
        return NULL;
    }
    // Create FHSS object
    fhss_structure_t *fhss_struct = fhss_ws_enable(this, fhss_configuration, fhss_timer);
    if (!fhss_struct) {
        tr_err("Failed to enable FHSS");
        ns_dyn_mem_free(this);
        return NULL;
    }
    fhss_ws_set_callbacks(fhss_struct);
    return this;
}

int ns_fhss_ws_set_parent(const fhss_api_t *fhss_api, const uint8_t eui64[8], const broadcast_timing_info_t *bc_timing_info, const bool force_synch)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure || !eui64 || !bc_timing_info) {
        return -1;
    }
    return fhss_ws_set_parent(fhss_structure, eui64, bc_timing_info, force_synch);
}

int ns_fhss_ws_remove_parent(const fhss_api_t *fhss_api, const uint8_t eui64[8])
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure || !eui64) {
        return -1;
    }
    return fhss_ws_remove_parent(fhss_structure, eui64);
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

int ns_fhss_set_neighbor_info_fp(const fhss_api_t *fhss_api, fhss_get_neighbor_info *get_neighbor_info)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure || !fhss_structure->ws) {
        return -1;
    }
    fhss_structure->ws->get_neighbor_info = get_neighbor_info;
    return 0;
}

const fhss_ws_configuration_t *ns_fhss_ws_configuration_get(const fhss_api_t *fhss_api)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure || !fhss_structure->ws) {
        return NULL;
    }
    return &fhss_structure->ws->fhss_configuration;
}

int ns_fhss_ws_configuration_set(const fhss_api_t *fhss_api, const fhss_ws_configuration_t *fhss_configuration)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure || !fhss_structure->ws) {
        return -1;
    }
    return fhss_ws_configuration_set(fhss_structure, fhss_configuration);
}

int ns_fhss_ws_set_hop_count(const fhss_api_t *fhss_api, const uint8_t hop_count)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure || !fhss_structure->ws) {
        return -1;
    }
    return fhss_ws_set_hop_count(fhss_structure, hop_count);
}

int ns_fhss_statistics_start(const fhss_api_t *fhss_api, fhss_statistics_t *fhss_statistics)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return -1;
    }
    return fhss_statistics_start(fhss_structure, fhss_statistics);
}
