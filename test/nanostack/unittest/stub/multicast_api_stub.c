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
#include "ip6string.h"
#include "ns_trace.h"
#include "randLIB.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_sha256.h"
#include "socket_api.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/mld.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "common_functions.h"

#include "RPL/rpl_control.h"


int8_t multicast_fwd_set_forwarding(int8_t interface_id, bool enable)
{
    return 0;
}

int8_t multicast_fwd_set_proxy_upstream(int8_t interface_id)
{
    return 0;
}
