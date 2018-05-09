/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "ns_types.h"
#include "eventOS_event.h"
#include "ns_trace.h"
#include "string.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/NVM/nwk_nvm.h"
#include "Service_Libs/mle_service/mle_service_api.h"

#define TRACE_GROUP "nNVM"

#define COUNTER_NVM_UPDATE_INCREMENT 1000

static wpan_nvm_params_t  *nwk_nvm_params_get_cb(nwk_wpan_nvm_api_t *api, uint16_t pan_id);

static void nwk_nvm_params_update_cb(nwk_wpan_nvm_api_t *api, bool if_down_call);

int8_t net_nvm_wpan_params_storage_enable(int8_t interface_id, wpan_params_updated *nvm_update_cb, wpan_params_get *nvm_get_cb)
{

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !nvm_update_cb || !nvm_get_cb) {
        return -1;
    }

    if (cur->nwk_wpan_nvm_api) {
        return 0;
    }
    cur->nwk_wpan_nvm_api = ns_dyn_mem_alloc(sizeof(nwk_wpan_nvm_api_t));
    if (!cur->nwk_wpan_nvm_api) {
        return -2;
    }

    cur->nwk_wpan_nvm_api->interface = cur;
    cur->nwk_wpan_nvm_api->params.pan_id = 0xffff;
    cur->nwk_wpan_nvm_api->NVM_GET = nvm_get_cb;
    cur->nwk_wpan_nvm_api->NVM_PUSH = nvm_update_cb;
    cur->nwk_wpan_nvm_api->nvm_params_get_cb = nwk_nvm_params_get_cb;
    cur->nwk_wpan_nvm_api->nvm_params_update_cb = nwk_nvm_params_update_cb;

    return 0;
}


int8_t net_nvm_wpan_params_storage_disable(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->nwk_wpan_nvm_api) {
        return -1;
    }
    ns_dyn_mem_free(cur->nwk_wpan_nvm_api);
    cur->nwk_wpan_nvm_api = NULL;
    return 0;

}

int8_t net_nvm_wpan_params_storage_reset(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->nwk_wpan_nvm_api) {
        return -1;
    }
    cur->nwk_wpan_nvm_api->params.pan_id = 0xffff;
    return 0;
}


static wpan_nvm_params_t  *nwk_nvm_params_get_cb(nwk_wpan_nvm_api_t *api, uint16_t pan_id)
{
    wpan_nvm_params_t temp_params;
    if (api->params.pan_id == 0xffff || api->params.pan_id != pan_id) {
        temp_params.pan_id = pan_id;
        if (api->NVM_GET(&temp_params)) {
            temp_params.mac_security_frame_counter += COUNTER_NVM_UPDATE_INCREMENT;
            temp_params.mle_securit_counter += COUNTER_NVM_UPDATE_INCREMENT;

        } else {
            temp_params.mac_security_frame_counter = 0;
            temp_params.mle_securit_counter = 0;

        }
        //SET Init values
        api->NVM_PUSH(&temp_params);
        api->params = temp_params;
    }
    return &api->params;
}

static void nwk_nvm_params_update_cb(nwk_wpan_nvm_api_t *api, bool if_down_call)
{
    uint32_t mac_counter;
    uint32_t mlme_counter;
    if (api->params.pan_id == 0xffff) {
        return;
    }

    //Read first current values and compare different to last read opration
    if (mac_helper_link_frame_counter_read(api->interface->id, &mac_counter) != 0) {
        return;
    }
    mlme_counter = mle_service_security_get_frame_counter(api->interface->id);

    if (if_down_call) {
        api->params.mac_security_frame_counter = mac_counter;
        api->params.mle_securit_counter = mlme_counter;
        return;
    }

    bool push_new_data = false;
    if (api->params.mac_security_frame_counter < mac_counter) {
        if (mac_counter - api->params.mac_security_frame_counter > COUNTER_NVM_UPDATE_INCREMENT - 50  ) {
            push_new_data = true;
            api->params.mac_security_frame_counter = mac_counter;
        }
    }

    if (api->params.mle_securit_counter < mlme_counter) {
        if (mlme_counter - api->params.mle_securit_counter > COUNTER_NVM_UPDATE_INCREMENT - 50 ) {
            push_new_data = true;
            api->params.mle_securit_counter = mlme_counter;
        }
    }

    if (push_new_data) {
        api->NVM_PUSH(&api->params);
    }
}
