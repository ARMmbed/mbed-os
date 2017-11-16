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


int8_t net_nvm_wpan_params_storage_enable(int8_t interface_id, wpan_params_updated *nvm_update_cb, wpan_params_get *nvm_get_cb)
{
    return 0;
}


int8_t net_nvm_wpan_params_storage_disable(int8_t interface_id)
{
    return 0;
}


