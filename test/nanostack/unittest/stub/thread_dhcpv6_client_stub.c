/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#include <ns_types.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "common_functions.h"


#include "dhcp_service_api.h"
#include "thread_dhcpv6_client.h"
#include "libDHCPv6/libDHCPv6.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"


typedef struct {
    thread_dhcp_client_global_adress_cb *global_address_cb;
    uint16_t service_instance;
    uint8_t libDhcp_instance;
    int8_t interface;
} thread_dhcp_client_class_t;

thread_dhcp_client_class_t dhcp_client;

extern uint8_t *dhcpv6_vendor_code_parse_thread_router_id(uint8_t *ptr, uint16_t data_length);
void thread_dhcpv6_client_set_address(int8_t interface_id, dhcpv6_client_server_data_t *srv_data_ptr);


void thread_dhcp_client_init(int8_t interface)
{
}

void thread_dhcp_client_delete(int8_t interface)
{
}

void thread_dhcpv6_client_send_error_cb(dhcpv6_client_server_data_t *srv_data_ptr)
{
}


/* solication responce received for either global address or routter id assignment */
int dhcp_solicit_resp_cb(uint16_t instance_id, void *ptr, uint8_t msg_name,  uint8_t *msg_ptr, uint16_t msg_len)
{
    return 0;
}

int thread_dhcp_client_get_global_address(int8_t interface, uint8_t dhcp_addr[__static 16], uint8_t prefix[__static 16], uint8_t mac64[__static 8], thread_dhcp_client_global_adress_cb *error_cb)
{
    return 0;
}

void thread_dhcp_client_global_address_renew(int8_t interface)
{
}

void thread_dhcp_client_global_address_delete(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16])
{
}

void thread_dhcpv6_renew(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason)
{
}

void thread_dhcpv6_client_set_address(int8_t interface_id, dhcpv6_client_server_data_t *srv_data_ptr)
{
}

uint8_t *dhcpv6_vendor_code_parse_thread_router_id(uint8_t *ptr, uint16_t data_length)
{
    return NULL;
}
