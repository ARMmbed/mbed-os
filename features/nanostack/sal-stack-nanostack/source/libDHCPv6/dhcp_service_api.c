/*
 * Copyright (c) 2013-2018, Arm Limited and affiliates.
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
#ifdef HAVE_DHCPV6
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
    dhcp_instance_type_e instance_type;
    ns_list_link_t link;
} server_instance_t;
typedef NS_LIST_HEAD(server_instance_t, link) server_instance_list_t;


typedef struct {
    uint16_t instance_id;
    int8_t interface_id;
    uint8_t server_address[16];
    bool    relay_activated;
    ns_list_link_t link;
} relay_instance_t;
typedef NS_LIST_HEAD(relay_instance_t, link) relay_instance_list_t;

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
    uint8_t *relay_start;
    ns_list_link_t link;
} msg_tr_t;
typedef NS_LIST_HEAD(msg_tr_t, link) tr_list_t;

typedef struct {
    ns_address_t src_address;
    server_instance_list_t srv_list;
    relay_instance_list_t relay_list;
    tr_list_t tr_list;
    int8_t dhcp_server_socket;
    int8_t dhcp_client_socket;
    int8_t dhcp_relay_socket;
    int8_t dhcpv6_socket_service_tasklet;
} dhcp_service_class_t;

#define DHCPV6_SOCKET_SERVICE_TASKLET_INIT      1
#define DHCPV6_SOCKET_SERVICE_TIMER             2

#define DHCPV6_SOCKET_SERVICE_TIMER_ID          1

#define DHCPV6_SOCKET_TIMER_UPDATE_PERIOD_IN_MS 100

dhcp_service_class_t *dhcp_service = NULL;
static bool dhcpv6_socket_timeout_timer_active = false;

void dhcp_service_send_message(msg_tr_t *msg_tr_ptr);

void DHCPv6_socket_service_tasklet(arm_event_s *event)
{
    if (event->event_type == DHCPV6_SOCKET_SERVICE_TASKLET_INIT) {
        //We should define peridiocally timer service!!
        eventOS_event_timer_request(DHCPV6_SOCKET_SERVICE_TIMER_ID, DHCPV6_SOCKET_SERVICE_TIMER, dhcp_service->dhcpv6_socket_service_tasklet, DHCPV6_SOCKET_TIMER_UPDATE_PERIOD_IN_MS);
        dhcpv6_socket_timeout_timer_active = true;
    } else if (event->event_type == DHCPV6_SOCKET_SERVICE_TIMER) {

        if (dhcp_service_timer_tick(1)) {
            dhcpv6_socket_timeout_timer_active = true;
            eventOS_event_timer_request(DHCPV6_SOCKET_SERVICE_TIMER_ID, DHCPV6_SOCKET_SERVICE_TIMER, dhcp_service->dhcpv6_socket_service_tasklet, DHCPV6_SOCKET_TIMER_UPDATE_PERIOD_IN_MS);
        } else {
            dhcpv6_socket_timeout_timer_active = false;
        }
    }
}

bool dhcp_service_allocate(void)
{
    bool retVal = false;
    if (dhcp_service == NULL) {
        dhcp_service = ns_dyn_mem_alloc(sizeof(dhcp_service_class_t));
        if (dhcp_service) {
            ns_list_init(&dhcp_service->srv_list);
            ns_list_init(&dhcp_service->relay_list);
            ns_list_init(&dhcp_service->tr_list);
            dhcp_service->dhcp_client_socket = -1;
            dhcp_service->dhcp_server_socket = -1;
            dhcp_service->dhcp_relay_socket = -1;
            dhcp_service->dhcpv6_socket_service_tasklet = eventOS_event_handler_create(DHCPv6_socket_service_tasklet, DHCPV6_SOCKET_SERVICE_TASKLET_INIT);
            if (dhcp_service->dhcpv6_socket_service_tasklet < 0) {
                ns_dyn_mem_free(dhcp_service);
                dhcp_service = NULL;
            } else {
                retVal = true;
            }
        }
    } else {
        retVal = true;
    }
    return retVal;
}

/*Subclass instances*/
msg_tr_t *dhcp_tr_find(uint32_t msg_tr_id)
{
    msg_tr_t *result = NULL;
    ns_list_foreach(msg_tr_t, cur_ptr, &dhcp_service->tr_list) {
        if (cur_ptr->msg_tr_id == msg_tr_id) {
            result = cur_ptr;
        }
    }
    return result;
}


msg_tr_t *dhcp_tr_create(void)
{
    uint32_t tr_id;
    msg_tr_t *msg_ptr = NULL;
    msg_ptr = ns_dyn_mem_temporary_alloc(sizeof(msg_tr_t));
    if (msg_ptr == NULL) {
        return NULL;
    }

    memset(msg_ptr, 0, sizeof(msg_tr_t));
    msg_ptr->msg_ptr = NULL;
    msg_ptr->recv_resp_cb = NULL;

    tr_id = randLIB_get_32bit() & 0xffffff;// 24 bits for random
    // Ensure a unique non-zero transaction id for each transaction
    while (tr_id == 0 || dhcp_tr_find(tr_id) != NULL) {
        tr_id = (tr_id + 1) & 0xffffff;
    }
    msg_ptr->msg_tr_id = tr_id;
    ns_list_add_to_start(&dhcp_service->tr_list, msg_ptr);
    return msg_ptr;
}

void dhcp_tr_delete(msg_tr_t *msg_ptr)
{
    if (msg_ptr != NULL) {
        ns_list_remove(&dhcp_service->tr_list, msg_ptr);
        ns_dyn_mem_free(msg_ptr->msg_ptr);
        ns_dyn_mem_free(msg_ptr);
    }
    return;
}

void dhcp_tr_set_retry_timers(msg_tr_t *msg_ptr, uint8_t msg_type)
{
    if (msg_ptr != NULL) {
        if (msg_type == DHCPV6_SOLICATION_TYPE) {
            msg_ptr->timeout_init = SOL_TIMEOUT;
            msg_ptr->timeout_max = SOL_MAX_RT;
            msg_ptr->retrans_max = 0;
        } else if (msg_type == DHCPV6_RENEW_TYPE) {
            msg_ptr->timeout_init = REN_TIMEOUT;
            msg_ptr->timeout_max = REN_MAX_RT;
            msg_ptr->retrans_max = 0;
        } else if (msg_type == DHCPV6_LEASEQUERY_TYPE) {
            msg_ptr->timeout_init = LQ_TIMEOUT;
            msg_ptr->timeout_max = LQ_MAX_RT;
            msg_ptr->retrans_max = LQ_MAX_RC;
        } else {
            msg_ptr->timeout_init = REL_TIMEOUT;
            msg_ptr->timeout_max = 0;
            msg_ptr->retrans_max = REL_MAX_RC;
        }

        // Convert from seconds to 1/10s ticks, with initial randomisation factor
        msg_ptr->timeout_init = randLIB_randomise_base(msg_ptr->timeout_init * 10, RAND1_LOW, RAND1_HIGH);
        msg_ptr->timeout_max *= 10;

        msg_ptr->timeout = msg_ptr->timeout_init;
        if (!dhcpv6_socket_timeout_timer_active) {
            eventOS_event_timer_request(DHCPV6_SOCKET_SERVICE_TIMER_ID, DHCPV6_SOCKET_SERVICE_TIMER, dhcp_service->dhcpv6_socket_service_tasklet, DHCPV6_SOCKET_TIMER_UPDATE_PERIOD_IN_MS);
            dhcpv6_socket_timeout_timer_active = true;
        }
    }
    return;
}

server_instance_t *dhcp_service_client_find(uint16_t instance_id)
{
    server_instance_t *result = NULL;
    ns_list_foreach(server_instance_t, cur_ptr, &dhcp_service->srv_list) {
        if (cur_ptr->instance_id == instance_id) {
            result = cur_ptr;
        }
    }
    return result;
}


static uint16_t dhcp_service_relay_interface_get(int8_t  interface_id)
{
    ns_list_foreach(server_instance_t, cur_ptr, &dhcp_service->srv_list) {
        if (cur_ptr->interface_id == interface_id && cur_ptr->instance_type == DHCP_INTANCE_RELAY_AGENT) {
            return cur_ptr->instance_id;
        }
    }

    return 0;
}



static relay_instance_t *dhcp_service_relay_find(uint16_t instance_id)
{
    relay_instance_t *result = NULL;
    ns_list_foreach(relay_instance_t, cur_ptr, &dhcp_service->relay_list) {
        if (cur_ptr->instance_id == instance_id) {
            result = cur_ptr;
        }
    }
    return result;
}

static relay_instance_t *dhcp_service_relay_interface(int8_t  interface_id)
{
    relay_instance_t *result = NULL;
    ns_list_foreach(relay_instance_t, cur_ptr, &dhcp_service->relay_list) {
        if (cur_ptr->interface_id == interface_id) {
            result = cur_ptr;
        }
    }
    return result;
}


void recv_dhcp_server_msg(void *cb_res)
{
    socket_callback_t *sckt_data;
    server_instance_t *srv_ptr = NULL;
    msg_tr_t *msg_tr_ptr;
    uint8_t *msg_ptr, *allocated_ptr;
    uint16_t msg_len;
    dhcpv6_relay_msg_t relay_msg;

    sckt_data = cb_res;

    if (sckt_data->event_type != SOCKET_DATA || sckt_data->d_len < 4) {
        return;
    }
    tr_debug("dhcp Server recv request");
    msg_tr_ptr = dhcp_tr_create();
    msg_ptr = ns_dyn_mem_temporary_alloc(sckt_data->d_len);
    allocated_ptr = msg_ptr;
    if (msg_ptr == NULL || msg_tr_ptr == NULL) {
        // read actual message
        tr_error("Out of resources");
        goto cleanup;
    }
    msg_len = socket_read(sckt_data->socket_id, &msg_tr_ptr->addr, msg_ptr, sckt_data->d_len);

    uint8_t msg_type = *msg_ptr;
    if (msg_type == DHCPV6_RELAY_FORWARD) {
        if (!libdhcpv6_relay_msg_read(msg_ptr, msg_len, &relay_msg)) {
            tr_error("Relay forward not correct");
            goto cleanup;
        }
        //Update Source and data
        msg_tr_ptr->relay_start = msg_ptr;
        memcpy(msg_tr_ptr->addr.address, relay_msg.peer_address, 16);
        msg_ptr = relay_msg.relay_options.msg_ptr;
        msg_len = relay_msg.relay_options.len;
        msg_type = *msg_ptr;


    } else if (msg_type == DHCPV6_RELAY_REPLY) {
        tr_error("Relay reply drop at server");
        goto cleanup;
    }

    //TODO use real function from lib also call validity check
    msg_tr_ptr->message_tr_id = common_read_24_bit(&msg_ptr[1]);

    if (0 != libdhcpv6_message_malformed_check(msg_ptr, msg_len)) {
        tr_error("Malformed packet");
        goto cleanup;
    }
    msg_tr_ptr->socket = sckt_data->socket_id;
    // call all receivers until found.
    ns_list_foreach(server_instance_t, cur_ptr, &dhcp_service->srv_list) {
        if (cur_ptr->interface_id == sckt_data->interface_id && cur_ptr->recv_req_cb != NULL) {
            msg_tr_ptr->instance_id = cur_ptr->instance_id;
            msg_tr_ptr->interface_id = sckt_data->interface_id;
            if ((RET_MSG_ACCEPTED ==
                    cur_ptr->recv_req_cb(cur_ptr->instance_id, msg_tr_ptr->msg_tr_id, msg_type, msg_ptr + 4, msg_len - 4))) {
                // should not modify pointers but library requires.
                msg_tr_ptr = NULL;
                srv_ptr = cur_ptr;
                break;
            }
        }
    }

cleanup:
    dhcp_tr_delete(msg_tr_ptr);
    ns_dyn_mem_free(allocated_ptr);
    if (srv_ptr == NULL) {
        //no owner found
        tr_warn("No handler for this message found");
    }

    return;
}

void recv_dhcp_relay_msg(void *cb_res)
{
    socket_callback_t *sckt_data;
    uint16_t msg_len;

    sckt_data = cb_res;

    if (sckt_data->event_type != SOCKET_DATA || sckt_data->d_len < 4) {
        return;
    }

    protocol_interface_info_entry_t *interface_ptr = protocol_stack_interface_info_get_by_id(sckt_data->interface_id);

    relay_instance_t *relay_srv = dhcp_service_relay_interface(sckt_data->interface_id);

    if (!interface_ptr || !relay_srv || !relay_srv->relay_activated) {
        return;
    }
    ns_address_t src_address;

    uint8_t relay_frame[DHCPV6_RELAY_LENGTH + 4];
    ns_iovec_t msg_iov[2];
    msg_iov[0].iov_base = relay_frame;
    msg_iov[0].iov_len = 34;
    msg_iov[1].iov_base = ns_dyn_mem_temporary_alloc(sckt_data->d_len);
    msg_iov[1].iov_len = sckt_data->d_len;
    if (msg_iov[1].iov_base == NULL) {
        // read actual message
        tr_error("Out of resources");
        goto cleanup;
    }

    ns_msghdr_t msghdr;
    //Set messages name buffer
    msghdr.msg_name = &src_address;
    msghdr.msg_namelen = sizeof(src_address);
    msghdr.msg_iov = &msg_iov[1];
    msghdr.msg_iovlen = 1;
    msghdr.msg_control = NULL;
    msghdr.msg_controllen = 0;

    msg_len = socket_recvmsg(sckt_data->socket_id, &msghdr, NS_MSG_LEGACY0);


    tr_debug("dhcp Relay recv msg");

    //Parse type
    uint8_t *ptr = msg_iov[1].iov_base;
    uint8_t msg_type = *ptr;


    if (msg_type == DHCPV6_RELAY_FORWARD) {
        tr_error("Drop not supported DHCPv6 forward at Agent");
        goto cleanup;

    } else if (msg_type == DHCPV6_RELAY_REPLY) {
        //Parse and validate Relay
        dhcpv6_relay_msg_t relay_msg;
        if (!libdhcpv6_relay_msg_read(ptr, msg_len, &relay_msg)) {
            tr_error("Not valid relay");
            goto cleanup;
        }
        if (0 != libdhcpv6_message_malformed_check(relay_msg.relay_options.msg_ptr, relay_msg.relay_options.len)) {
            tr_error("Malformed packet");
            goto cleanup;
        }
        //Copy DST address
        memcpy(src_address.address, relay_msg.peer_address, 16);
        src_address.type = ADDRESS_IPV6;
        src_address.identifier = DHCPV6_CLIENT_PORT;
        msghdr.msg_iov = &msg_iov[0];
        msghdr.msg_iovlen = 1;
        msg_iov[0].iov_base = relay_msg.relay_options.msg_ptr;
        msg_iov[0].iov_len = relay_msg.relay_options.len;
        tr_debug("Forward Original relay msg to client");

    } else {
        if (0 != libdhcpv6_message_malformed_check(ptr, msg_len)) {
            tr_error("Malformed packet");
            goto cleanup;
        }
        uint8_t gp_address[16];
        //Get blobal address from interface
        if (addr_interface_select_source(interface_ptr, gp_address, relay_srv->server_address, 0) != 0) {
            // No global prefix available
            tr_error("No GP address");
            goto cleanup;
        }

        //Build
        libdhcpv6_dhcp_relay_msg_write(relay_frame, DHCPV6_RELAY_FORWARD, 0, src_address.address, gp_address);
        libdhcpv6_dhcp_option_header_write(relay_frame + 34, msg_len);

        //Copy DST address
        memcpy(src_address.address, relay_srv->server_address, 16);
        src_address.type = ADDRESS_IPV6;
        src_address.identifier = DHCPV6_SERVER_PORT;
        //ADD relay frame vector front of original data
        msghdr.msg_iov = &msg_iov[0];
        msghdr.msg_iovlen = 2;
        msg_iov[0].iov_base = relay_frame;
        msg_iov[0].iov_len = 38;
        msg_iov[1].iov_len = msg_len;
        tr_debug("Forward Client msg to server");
    }
    socket_sendmsg(sckt_data->socket_id, &msghdr, NS_MSG_LEGACY0);
cleanup:
    ns_dyn_mem_free(msg_iov[1].iov_base);

    return;
}

void recv_dhcp_client_msg(void *cb_res)
{
    ns_address_t address;
    socket_callback_t *sckt_data;
    msg_tr_t *msg_tr_ptr = NULL;
    uint8_t *msg_ptr = NULL;
    int16_t msg_len = 0;
    uint_fast24_t tr_id = 0;
    int retVal = RET_MSG_ACCEPTED;

    sckt_data = cb_res;

    if (sckt_data->event_type != SOCKET_DATA || sckt_data->d_len < 4) {
        return;
    }
    tr_debug("dhcp recv response message");
    // read actual message
    msg_ptr = ns_dyn_mem_temporary_alloc(sckt_data->d_len);

    if (msg_ptr == NULL) {
        tr_error("Out of memory");
        goto cleanup;
    }
    msg_len = socket_read(sckt_data->socket_id, &address, msg_ptr, sckt_data->d_len);

    tr_id = common_read_24_bit(&msg_ptr[1]);
    msg_tr_ptr = dhcp_tr_find(tr_id);

    if (msg_tr_ptr == NULL) {
        tr_error("invalid tr id");
        goto cleanup;
    }
    if (0 != libdhcpv6_message_malformed_check(msg_ptr, msg_len)) {
        msg_tr_ptr->recv_resp_cb(msg_tr_ptr->instance_id, msg_tr_ptr->client_obj_ptr, 0, NULL, 0);
        tr_error("Malformed packet");
        goto cleanup;
    }
    // read msg tr id from message and find transaction. and then instance
    // TODO use real function from dhcp lib

    if (msg_tr_ptr != NULL && msg_tr_ptr->recv_resp_cb) {
        // call receive callback should not modify pointers but library requires
        retVal = msg_tr_ptr->recv_resp_cb(msg_tr_ptr->instance_id, msg_tr_ptr->client_obj_ptr, *msg_ptr, msg_ptr + 4, msg_len - 4);
    } else {
        tr_error("no receiver for this message found");
    }

cleanup:
    ns_dyn_mem_free(msg_ptr);
    if (retVal != RET_MSG_WAIT_ANOTHER) {
        //Transaction is not killed yet
        dhcp_tr_delete(dhcp_tr_find(tr_id));
    }
    return ;
}

uint16_t dhcp_service_init(int8_t interface_id, dhcp_instance_type_e instance_type, dhcp_service_receive_req_cb *receive_req_cb)
{
    uint16_t id = 1;
    server_instance_t *srv_ptr;

    if (!dhcp_service_allocate()) {
        tr_error("dhcp Sockets data base alloc fail");
        return 0;
    }
    if (instance_type == DHCP_INSTANCE_SERVER && dhcp_service->dhcp_server_socket < 0) {
        if (dhcp_service->dhcp_relay_socket >= 0) {
            tr_error("dhcp Server socket can't open because Agent open already");
        }
        dhcp_service->dhcp_server_socket = socket_open(SOCKET_UDP, DHCPV6_SERVER_PORT, recv_dhcp_server_msg);
    }

    if (instance_type == DHCP_INTANCE_RELAY_AGENT && dhcp_service->dhcp_relay_socket < 0) {
        if (dhcp_service->dhcp_server_socket >= 0) {
            tr_error("dhcp Relay agent can't open because server open already");
        }
        dhcp_service->dhcp_relay_socket = socket_open(SOCKET_UDP, DHCPV6_SERVER_PORT, recv_dhcp_relay_msg);
    }

    if (instance_type == DHCP_INSTANCE_CLIENT && dhcp_service->dhcp_client_socket < 0) {
        dhcp_service->dhcp_client_socket = socket_open(SOCKET_UDP, DHCPV6_CLIENT_PORT, recv_dhcp_client_msg);
    }
    if (instance_type == DHCP_INSTANCE_SERVER && dhcp_service->dhcp_server_socket < 0) {
        tr_error("No sockets available for DHCP server");
        return 0;
    }
    if (instance_type == DHCP_INSTANCE_CLIENT && dhcp_service->dhcp_client_socket < 0) {
        tr_error("No sockets available for DHCP client");
        return 0;
    }

    if (instance_type == DHCP_INTANCE_RELAY_AGENT) {
        if (dhcp_service->dhcp_relay_socket < 0) {
            tr_error("No sockets available for DHCP server");
        }

        uint16_t temp_id = dhcp_service_relay_interface_get(interface_id);
        if (temp_id) {
            return temp_id;
        }
    }

    for (; id < MAX_SERVERS; id++) {
        if (dhcp_service_client_find(id) == NULL) {
            break;
        }
    }
    srv_ptr = ns_dyn_mem_alloc(sizeof(server_instance_t));
    if (id == MAX_SERVERS || srv_ptr == NULL) {
        tr_error("Out of server instances");
        ns_dyn_mem_free(srv_ptr);
        return 0;
    }

    if (instance_type == DHCP_INTANCE_RELAY_AGENT) {
        //Allocate Realay Agent
        relay_instance_t *relay_srv = ns_dyn_mem_alloc(sizeof(relay_instance_t));
        if (!relay_srv) {
            tr_error("Out of realy instances");
            ns_dyn_mem_free(srv_ptr);
            return 0;
        }
        ns_list_add_to_start(&dhcp_service->relay_list, relay_srv);
        relay_srv->instance_id = id;
        relay_srv->interface_id = interface_id;
        relay_srv->relay_activated = false;

    }

    ns_list_add_to_start(&dhcp_service->srv_list, srv_ptr);
    srv_ptr->instance_id = id;
    srv_ptr->instance_type = instance_type;
    srv_ptr->interface_id = interface_id;
    srv_ptr->recv_req_cb = receive_req_cb;
    return srv_ptr->instance_id;
}

void dhcp_service_relay_instance_enable(uint16_t instance, uint8_t *server_address)
{
    relay_instance_t *relay_srv = dhcp_service_relay_find(instance);
    if (relay_srv) {
        relay_srv->relay_activated = true;
        memcpy(relay_srv->server_address, server_address, 16);
    }
}

uint8_t *dhcp_service_relay_global_addres_get(uint16_t instance)
{
    relay_instance_t *relay_srv = dhcp_service_relay_find(instance);
    if (!relay_srv || !relay_srv->relay_activated) {
        return NULL;
    }

    return relay_srv->server_address;
}

void dhcp_service_delete(uint16_t instance)
{
    server_instance_t *srv_ptr;
    if (dhcp_service == NULL) {
        return;
    }
    srv_ptr = dhcp_service_client_find(instance);
    //TODO delete all transactions
    if (srv_ptr != NULL) {
        ns_list_remove(&dhcp_service->srv_list, srv_ptr);
        if (srv_ptr->instance_type == DHCP_INTANCE_RELAY_AGENT) {
            //Free relay service
            relay_instance_t *relay = dhcp_service_relay_find(instance);
            if (relay) {
                ns_list_remove(&dhcp_service->relay_list, relay);
                ns_dyn_mem_free(relay);
            }
        }
        ns_dyn_mem_free(srv_ptr);

    }
    ns_list_foreach_safe(msg_tr_t, cur_ptr, &dhcp_service->tr_list) {
        if (cur_ptr->instance_id == instance) {
            dhcp_tr_delete(cur_ptr);
        }
    }

    int8_t server_instances = 0, client_instances = 0, relay_instances = 0;

    ns_list_foreach(server_instance_t, srv, &dhcp_service->srv_list) {
        if (srv->instance_type == DHCP_INSTANCE_SERVER) {
            ++server_instances;
        } else if (srv->instance_type == DHCP_INSTANCE_CLIENT) {
            ++client_instances;
        } else if (srv->instance_type == DHCP_INTANCE_RELAY_AGENT) {
            ++relay_instances;
        }
    }

    if ((server_instances == 0 && relay_instances == 0) && dhcp_service->dhcp_server_socket > -1) {
        socket_close(dhcp_service->dhcp_server_socket);
        dhcp_service->dhcp_server_socket = -1;
    }

    if (client_instances == 0 && dhcp_service->dhcp_client_socket > -1) {
        socket_close(dhcp_service->dhcp_client_socket);
        dhcp_service->dhcp_client_socket = -1;
    }
    return;
}

int dhcp_service_send_resp(uint32_t msg_tr_id, uint8_t options, uint8_t *msg_ptr, uint16_t msg_len)
{
    msg_tr_t *msg_tr_ptr;
    server_instance_t *srv_instance;
    msg_tr_ptr = dhcp_tr_find(msg_tr_id);
    if (msg_tr_ptr == NULL) {
        tr_error("msg_tr_id not found");
        return -1;
    }
    srv_instance = dhcp_service_client_find(msg_tr_ptr->instance_id);
    if (srv_instance == NULL) {
        tr_error("Srv Instance not found");
        return -1;
    }
    ns_dyn_mem_free(msg_tr_ptr->msg_ptr);

    msg_tr_ptr->msg_ptr = msg_ptr;
    msg_tr_ptr->msg_len = msg_len;
    msg_tr_ptr->options = options;
    // set the received transaction id to message.
    common_write_24_bit(msg_tr_ptr->message_tr_id, &msg_tr_ptr->msg_ptr[1]);

    dhcp_service_send_message(msg_tr_ptr);
    msg_tr_ptr->msg_ptr = NULL; // pointer is the responsibility of client
    dhcp_tr_delete(msg_tr_ptr);
    return 0;
}
uint32_t dhcp_service_send_req(uint16_t instance_id, uint8_t options, void *ptr, const uint8_t addr[static 16], uint8_t *msg_ptr, uint16_t msg_len, dhcp_service_receive_resp_cb *receive_resp_cb)
{
    msg_tr_t *msg_tr_ptr;
    server_instance_t *srv_ptr;
    srv_ptr = dhcp_service_client_find(instance_id);
    msg_tr_ptr = dhcp_tr_create();

    if (msg_tr_ptr == NULL || srv_ptr == NULL || msg_ptr == NULL || receive_resp_cb == NULL || msg_len < 5) {
        tr_error("Request sending failed");
        return 0;
    }

    msg_tr_ptr->msg_ptr = msg_ptr;
    msg_tr_ptr->msg_len = msg_len;
    msg_tr_ptr->options = options;
    msg_tr_ptr->client_obj_ptr = ptr;
    memcpy(msg_tr_ptr->addr.address, addr, 16);
    msg_tr_ptr->addr.identifier = DHCPV6_SERVER_PORT;
    msg_tr_ptr->addr.type = ADDRESS_IPV6;
    msg_tr_ptr->interface_id = srv_ptr->interface_id;
    msg_tr_ptr->instance_id = instance_id;
    msg_tr_ptr->socket = dhcp_service->dhcp_client_socket;
    msg_tr_ptr->recv_resp_cb = receive_resp_cb;
    msg_tr_ptr->first_transmit_time = protocol_core_monotonic_time;
    dhcp_tr_set_retry_timers(msg_tr_ptr, msg_tr_ptr->msg_ptr[0]);
    common_write_24_bit(msg_tr_ptr->msg_tr_id, &msg_tr_ptr->msg_ptr[1]);

    dhcp_service_send_message(msg_tr_ptr);
    return msg_tr_ptr->msg_tr_id;
}

void dhcp_service_set_retry_timers(uint32_t msg_tr_id, uint16_t timeout_init, uint16_t timeout_max, uint8_t retrans_max)
{
    msg_tr_t *msg_tr_ptr;
    msg_tr_ptr = dhcp_tr_find(msg_tr_id);

    if (msg_tr_ptr != NULL) {
        msg_tr_ptr->timeout_init = randLIB_randomise_base(timeout_init * 10, RAND1_LOW, RAND1_HIGH);
        msg_tr_ptr->timeout = msg_tr_ptr->timeout_init;
        msg_tr_ptr->timeout_max = timeout_max * 10;
        msg_tr_ptr->retrans_max = retrans_max;
    }
    return;
}

void dhcp_service_update_server_address(uint32_t msg_tr_id, uint8_t *server_address)
{
    msg_tr_t *msg_tr_ptr;
    msg_tr_ptr = dhcp_tr_find(msg_tr_id);

    if (msg_tr_ptr != NULL) {
        memcpy(msg_tr_ptr->addr.address, server_address, 16);
    }
}

void dhcp_service_req_remove(uint32_t msg_tr_id)
{
    if (dhcp_service) {
        dhcp_tr_delete(dhcp_tr_find(msg_tr_id));
    }
    return;
}

void dhcp_service_send_message(msg_tr_t *msg_tr_ptr)
{
    int8_t retval;
    int16_t multicast_hop_limit = -1;
    const uint32_t address_pref = SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;
    dhcp_options_msg_t elapsed_time;

    if (libdhcpv6_message_option_discover((msg_tr_ptr->msg_ptr + 4), (msg_tr_ptr->msg_len - 4), DHCPV6_ELAPSED_TIME_OPTION, &elapsed_time) == 0 &&
            elapsed_time.len == 2) {
        uint32_t t = protocol_core_monotonic_time - msg_tr_ptr->first_transmit_time; // time in 1/10s ticks
        uint16_t cs;
        if (t > 0xffff / 10) {
            cs = 0xffff;
        } else {
            cs = (uint16_t) t * 10;
        }
        common_write_16_bit(cs, elapsed_time.msg_ptr);
    }

    if ((msg_tr_ptr->options & TX_OPT_USE_SHORT_ADDR) == TX_OPT_USE_SHORT_ADDR) {
        socket_setsockopt(msg_tr_ptr->socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &address_pref, sizeof address_pref);
    }
    if ((msg_tr_ptr->options & TX_OPT_MULTICAST_HOP_LIMIT_64) == TX_OPT_MULTICAST_HOP_LIMIT_64) {
        multicast_hop_limit = 64;
    }
    socket_setsockopt(msg_tr_ptr->socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &multicast_hop_limit, sizeof multicast_hop_limit);
    socket_setsockopt(msg_tr_ptr->socket, SOCKET_IPPROTO_IPV6, SOCKET_INTERFACE_SELECT, &msg_tr_ptr->interface_id, sizeof(int8_t));

    if (msg_tr_ptr->relay_start) {
        //Build Relay Reply only server do this
        ns_iovec_t data_vector[2];
        ns_msghdr_t msghdr;
        memcpy(msg_tr_ptr->addr.address, msg_tr_ptr->relay_start + 2, 16);
        msg_tr_ptr->addr.identifier = DHCPV6_SERVER_PORT;
        //SET IOV vectors
        //Relay Reply
        data_vector[0].iov_base = (void *) msg_tr_ptr->relay_start;
        data_vector[0].iov_len = DHCPV6_RELAY_LENGTH + 4;
        //DHCPV normal message vector
        data_vector[1].iov_base = (void *) msg_tr_ptr->msg_ptr;
        data_vector[1].iov_len = msg_tr_ptr->msg_len;

        //Set message name
        msghdr.msg_name = (void *) &msg_tr_ptr->addr;
        msghdr.msg_namelen = sizeof(ns_address_t);
        msghdr.msg_iov = &data_vector[0];
        msghdr.msg_iovlen = 2;
        //No ancillary data
        msghdr.msg_control = NULL;
        msghdr.msg_controllen = 0;

        uint8_t *ptr = msg_tr_ptr->relay_start;
        *ptr = DHCPV6_RELAY_REPLY;
        libdhcpv6_dhcp_option_header_write(ptr + 34, msg_tr_ptr->msg_len);
        retval = socket_sendmsg(msg_tr_ptr->socket, &msghdr, NS_MSG_LEGACY0);

    } else {
        retval = socket_sendto(msg_tr_ptr->socket, &msg_tr_ptr->addr, msg_tr_ptr->msg_ptr, msg_tr_ptr->msg_len);
    }
    if (retval != 0) {
        tr_warn("dhcp service socket_sendto fails: %i", retval);
    } else {
        tr_debug("dhcp service socket_sendto %s", trace_ipv6(msg_tr_ptr->addr.address));
    }
}
bool dhcp_service_timer_tick(uint16_t ticks)
{
    bool activeTimerNeed = false;
    ns_list_foreach_safe(msg_tr_t, cur_ptr, &dhcp_service->tr_list) {
        if (cur_ptr->timeout == 0) {
            continue;
        }

        if (cur_ptr->timeout <= ticks) {
            activeTimerNeed = true;
            cur_ptr->retrans++;
            if (cur_ptr->retrans_max != 0 && cur_ptr->retrans >= cur_ptr->retrans_max) {
                // retransmission count exceeded.
                cur_ptr->recv_resp_cb(cur_ptr->instance_id, cur_ptr->client_obj_ptr, 0, NULL, 0);
                dhcp_tr_delete(cur_ptr);
                continue;
            }
            dhcp_service_send_message(cur_ptr);
            // RFC 3315 says:
            //     RT = 2*RTprev + RAND*RTprev,
            // We calculate this as
            //     RT = RTprev + (1+RAND)*RTprev
            cur_ptr->timeout = cur_ptr->timeout_init + randLIB_randomise_base(cur_ptr->timeout_init, RAND1_LOW, RAND1_HIGH);
            // Catch 16-bit integer overflow
            if (cur_ptr->timeout < cur_ptr->timeout_init) {
                cur_ptr->timeout = 0xFFFF;
            }
            // Check against MRT
            if (cur_ptr->timeout_max != 0 && cur_ptr->timeout > cur_ptr->timeout_max) {
                cur_ptr->timeout = randLIB_randomise_base(cur_ptr->timeout_max, RAND1_LOW, RAND1_HIGH);
            }
            cur_ptr->timeout_init = cur_ptr->timeout;
        } else {
            cur_ptr->timeout -= ticks;
            activeTimerNeed = true;
        }
    }
    return activeTimerNeed;
}
#else
uint16_t dhcp_service_init(int8_t interface_id, dhcp_instance_type_e instance_type, dhcp_service_receive_req_cb *receive_req_cb)
{
    (void)interface_id;
    (void)instance_type;
    (void)receive_req_cb;
    return 0;
}

void dhcp_service_delete(uint16_t instance)
{
    (void)instance;
}

void dhcp_service_relay_instance_enable(uint16_t instance, uint8_t *server_address)
{
    (void)instance;
    (void)server_address;
}

int dhcp_service_send_resp(uint32_t msg_tr_id, uint8_t options, uint8_t *msg_ptr, uint16_t msg_len)
{
    (void)msg_tr_id;
    (void)options;
    (void)msg_ptr;
    (void)msg_len;
    return -1;
}

uint32_t dhcp_service_send_req(uint16_t instance_id, uint8_t options, void *ptr, const uint8_t addr[static 16], uint8_t *msg_ptr, uint16_t msg_len, dhcp_service_receive_resp_cb *receive_resp_cb)
{
    (void)instance_id;
    (void)options;
    (void)ptr;
    (void)addr;
    (void)msg_ptr;
    (void)msg_len;
    (void)receive_resp_cb;
    return 0;
}

void dhcp_service_set_retry_timers(uint32_t msg_tr_id, uint16_t timeout_init, uint16_t timeout_max, uint8_t retrans_max)
{
    (void)msg_tr_id;
    (void)timeout_init;
    (void)timeout_max;
    (void)retrans_max;
}
void dhcp_service_req_remove(uint32_t msg_tr_id)
{
    (void)msg_tr_id;
}

bool dhcp_service_timer_tick(uint16_t ticks)
{
    (void)ticks;
    return false;
}

#endif
