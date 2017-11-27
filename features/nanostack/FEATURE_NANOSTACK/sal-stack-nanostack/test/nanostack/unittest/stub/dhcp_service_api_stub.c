/*
 * Copyright (c) 2013-2016, Arm Limited and affiliates.
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
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "randLIB.h"
#include "socket_api.h"
#include "net_interface.h"
#include "common_functions.h"
#include "libDHCPv6/libDHCPv6.h"
#include "NWK_INTERFACE/Include/protocol.h" // just for protocol_core_monotonic_time

#include "dhcp_service_api.h"

#define TRACE_GROUP    "dhcp"

#define MAX_SERVERS 20

/* Fixed-point randomisation limits for randlib_randomise_base() - RFC 3315
 * says RAND is uniformly distributed between -0.1 and +0.1
 */
#define RAND1_LOW   0x7333 // 1 - 0.1; minimum for "1+RAND"
#define RAND1_HIGH  0x8CCD // 1 + 0.1; maximum for "1+RAND"

typedef struct {
    dhcp_service_receive_req_cb *recv_req_cb;
    uint16_t instance_id;
    int8_t interface_id;
    ns_list_link_t link;
} server_instance_t;
typedef NS_LIST_HEAD(server_instance_t, link) server_instance_list_t;

typedef struct {
    ns_address_t addr;
    dhcp_service_receive_resp_cb *recv_resp_cb;
    uint16_t instance_id;
    int8_t interface_id;
    int8_t socket;
    uint8_t options;
    void  *client_obj_ptr;
    uint32_t msg_tr_id;
    uint32_t message_tr_id;
    uint32_t first_transmit_time;
    uint16_t timeout;
    uint16_t timeout_init;
    uint16_t timeout_max;
    uint8_t retrans_max;
    uint8_t retrans;
    uint8_t *msg_ptr;
    uint16_t msg_len;
    ns_list_link_t link;
} msg_tr_t;
typedef NS_LIST_HEAD(msg_tr_t, link) tr_list_t;

typedef struct {
    ns_address_t src_address;
    server_instance_list_t srv_list;
    tr_list_t tr_list;
    int8_t dhcp_server_socket;
    int8_t dhcp_client_socket;
    int8_t dhcpv6_socket_service_tasklet;
} dhcp_service_class_t;

#define DHCPV6_SOCKET_SERVICE_TASKLET_INIT      1
#define DHCPV6_SOCKET_SERVICE_TIMER             2

#define DHCPV6_SOCKET_SERVICE_TIMER_ID          1

#define DHCPV6_SOCKET_TIMER_UPDATE_PERIOD_IN_MS 100

dhcp_service_class_t *dhcp_service = NULL;

void dhcp_service_send_message(msg_tr_t *msg_tr_ptr);

void DHCPv6_socket_service_tasklet(arm_event_s *event)
{
}

bool dhcp_service_allocate(void)
{
    return false;
}

msg_tr_t *dhcp_tr_find(uint32_t msg_tr_id)
{
    return NULL;
}

msg_tr_t *dhcp_tr_create(void)
{
    return NULL;
}

void dhcp_tr_delete(msg_tr_t *msg_ptr)
{
}

void dhcp_tr_set_retry_timers(msg_tr_t *msg_ptr, uint8_t msg_type)
{
}

server_instance_t *dhcp_service_client_find(uint16_t instance_id)
{
    return NULL;
}

void recv_dhcp_server_msg(void *cb_res)
{
}

void recv_dhcp_client_msg(void *cb_res)
{
}

uint16_t dhcp_service_init(int8_t interface_id, dhcp_instance_type_e instance_type, dhcp_service_receive_req_cb *receive_req_cb)
{
    return 0;
}

void dhcp_service_delete(uint16_t instance)
{
}

int dhcp_service_send_resp(uint32_t msg_tr_id, uint8_t options, uint8_t *msg_ptr, uint16_t msg_len)
{
    return 0;
}

uint32_t dhcp_service_send_req(uint16_t instance_id, uint8_t options, void *ptr, const uint8_t addr[static 16], uint8_t *msg_ptr, uint16_t msg_len, dhcp_service_receive_resp_cb *receive_resp_cb)
{
    return 0;
}

void dhcp_service_set_retry_timers(uint32_t msg_tr_id, uint16_t timeout_init, uint16_t timeout_max, uint8_t retrans_max)
{
}

void dhcp_service_req_remove(uint32_t msg_tr_id)
{
}

void dhcp_service_send_message(msg_tr_t *msg_tr_ptr)
{
}

bool dhcp_service_timer_tick(uint16_t ticks)
{
    return false;
}

