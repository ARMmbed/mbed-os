/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
#include "ns_list.h"
#include "ns_trace.h"
#include "socket_api.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_eapol_relay_lib.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wsrl"

int8_t ws_eapol_relay_lib_send_to_relay(const uint8_t socket_id, const uint8_t *eui_64, const ns_address_t *dest_addr, const void *data, uint16_t data_len)
{
    ns_address_t addr = *dest_addr;

    ns_iovec_t msg_iov[2];
    ns_msghdr_t msghdr;
    //Set messages name buffer
    msghdr.msg_name = &addr;
    msghdr.msg_namelen = sizeof(addr);
    msghdr.msg_iov = &msg_iov[0];
    msghdr.msg_iovlen = 2;
    msghdr.msg_control = NULL;
    msghdr.msg_controllen = 0;
    msg_iov[0].iov_base = (void *)eui_64;
    msg_iov[0].iov_len = 8;
    msg_iov[1].iov_base = (void *)data;
    msg_iov[1].iov_len = data_len;
    socket_sendmsg(socket_id, &msghdr, NS_MSG_LEGACY0);
    return 0;
}

#endif /* HAVE_WS */

