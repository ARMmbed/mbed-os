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

#include "fnet.h"
#include "fnet_mdns.h"
#include "fnet_mdns_stub.h"

// Test data
fnet_mdns_data_stub_t fnet_mdns_data_stub = {
    .fnet_mdns_descriptor = NULL,
    .fnet_mdns_service_descriptor = NULL,
    .announce_cnt = 0,
    .release_cnt = 0,
    .unregister_cnt = 0
};

// Test cases

fnet_mdns_desc_t fnet_mdns_init( struct fnet_mdns_params *params )
{
    if((params == 0) || (params->netif_desc == 0) || (params->name == 0))
    {
        return NULL;
    }

    return fnet_mdns_data_stub.fnet_mdns_descriptor;
}

void fnet_mdns_release(fnet_mdns_desc_t mdns_desc)
{
    fnet_mdns_data_stub.release_cnt++;
}

fnet_mdns_service_desc_t fnet_mdns_service_register(fnet_mdns_desc_t mdns_desc, const fnet_mdns_service_t *service)
{
     return fnet_mdns_data_stub.fnet_mdns_service_descriptor;
}

void fnet_mdns_service_unregister(fnet_mdns_service_desc_t service_desc)
{
    fnet_mdns_data_stub.unregister_cnt++;
}

void fnet_mdns_announce(fnet_mdns_desc_t mdns_desc)
{
    fnet_mdns_data_stub.announce_cnt++;
}

