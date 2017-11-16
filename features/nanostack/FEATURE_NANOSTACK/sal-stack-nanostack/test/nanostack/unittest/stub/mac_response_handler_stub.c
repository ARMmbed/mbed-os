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
#include "mlme.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Core/include/address.h"
#include "Core/include/socket.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_response_handler.h"
#include "MLE/mle.h"

#include "6LoWPAN/MAC/mac_data_poll.h"


#define TRACE_GROUP "MRsH"

void mcps_data_confirm_handler( const mac_api_t* api, const mcps_data_conf_t *data )
{
}

void mcps_data_indication_handler( const mac_api_t* api, const mcps_data_ind_t *data_ind )
{
}

void mcps_purge_confirm_handler( const mac_api_t* api, mcps_purge_conf_t *data )
{
}

void mlme_confirm_handler( const mac_api_t* api, mlme_primitive id, const void *data )
{
}

void mlme_indication_handler( const mac_api_t* api, mlme_primitive id, const void *data )
{
}

bool mac_data_is_broadcast_addr(const sockaddr_t *addr)
{
    return false;
}

