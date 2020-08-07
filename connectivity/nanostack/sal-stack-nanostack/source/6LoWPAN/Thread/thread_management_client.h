/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Thread CoAP management client implementation.
 *
 * This client must be enabled for ROUTER, REED and HOST that want to publish network data.
 *
 * HOST
 * - sending own network data to thread leader
 *
 * REED
 * - Router id assignment client.
 */


#ifndef THREAD_MANAGEMENT_CLIENT_H_
#define THREAD_MANAGEMENT_CLIENT_H_

#include <ns_types.h>
#include "thread_management_if.h"
#include "net_interface.h"

#define TRACE_GROUP_THREAD_MANAGEMENT_CLIENT "TMFs"

/** Initialise thread management client.
 *
 * This instance needs to bee initialised once for each thread network interface.
 * if only one thread instance is supported this is needed to call only once.
 *
 * /param interface interface id of this thread instance.
 *
 */

void thread_management_client_init(int8_t interface_id);

/** Delete dhcp thread dhcp client.
 *
 * When this is called all addressed assigned by this module are removed from stack.
 */
void thread_management_client_delete(int8_t interface_id);

/** Router id handler callback.
 *
 * callback to inform when new router id is received from leader.
 *
 *  /param status status of operation 0 success, -1 not accepted
 *  /param router_rloc router rloc16 address assigned to this device. 0xfffe if failure.
 *  /param router_mask_ptr Router id mask of current thread network.
 *
 */
typedef void thread_management_client_router_id_cb(int8_t interface_id, int8_t status, uint16_t router_rloc, const uint8_t router_mask_ptr[9]);

/** request router id from the leader of network.
 *
 *  /param mac extented mac address of requested device.
 *  /param router_id old router id of this device, 0xffff if new address requested.
 *  /param id_cb callback to inform result of the request.
 *  /param status See thread 1.1.0 specification, chapter 5.19.5 for possible values.
 *
 * return positive value indicates COAP message ID of the request, zero or negative value indicates failure
 */
int thread_management_client_router_id_get(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb, uint8_t status);

/** release router id from the leader of network.
 *
 *  /param mac extented mac address of requested device.
 *  /param router_id router id of this device.
 *  /param id_cb callback to inform result of the request when calling this status is -1 and address is 0xfffe.
 *
 * return 0 success other values failure
 */
int thread_management_client_router_id_release(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb);

/** Network data set response callback.
 *
 * callback to inform if network data was set to leader.
 *
 * /param status status of operation 0 success, -1 failure from leader received
 * /param data_ptr pointer to network data TLV that leader accepted.
 * /param data_len length of network data.
 *
 */
typedef void thread_management_client_network_data_set_cb(int8_t interface_id, int8_t status, uint8_t *data_ptr, uint16_t data_len);

/** send network data to leader of the network
 *
 * /param interface interface id of this thread instance.
 * /param data_ptr pointer to network data TLV to publish for leader of the network.
 * /param data_len length of network data.
 * /param set_cb callback to see if setting of network data was success. can be NULL.
 *
 *return 0 success other values failure
 */
int thread_management_client_network_data_register(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len, thread_management_client_network_data_set_cb *set_cb);

/** Send network data clear for leader to clean network data related to this rloc16.
 *
 * /param interface interface id of this thread instance.
 * /param rloc16 rloc16 of device which network data is unregistered.
 *
 *return 0 success other values failure
 */
int thread_management_client_network_data_unregister(int8_t interface_id, uint16_t rloc16);

/** Send neighbor discovery data request to given destination. Requested options will be:
 *  -Recursive DNS Server Option (25) and,
 *  -DNS Search List Option (31).
 *
 * /param interface interface id of this thread instance.
 * /param destination address where message is sent.
 * /param options requested nd data options according to RFC6106
 * /param options_len number of options requested
 * /param set_cb callback to be called when a response arrives.
 *
 *return 0 success other values failure
 */
int thread_management_client_neighbor_discovery_data_request(int8_t interface_id, const uint8_t destination[16],  const uint8_t *options, uint8_t options_len, thread_management_client_network_data_set_cb *set_cb);

/** Get interface_id of based on coap service_id
 *
 *
 * /param service_id coap service id.
 *
 *return interface_id of thread instance if successful and -1 for failure
 */
int thread_management_client_get_interface_id_by_service_id(int8_t service_id);

/** send active configuration dataset get for parent
 *
 * /param interface_id interface id of this thread instance.
 * /param dst_address destination address
 *
 *return 0 success other values failure
 */
int8_t thread_management_client_active_get(int8_t interface_id, uint8_t *dst_address);

/** send active configuration dataset set for parent
 *
 * /param interface_id interface id of this thread instance.
 * /param dst_address destination address
 *
 *return 0 success other values failure
 */
int8_t thread_management_client_active_set(int8_t interface_id, uint8_t *dst_address);

/** send pending configuration dataset get query for parent
 *
 * /param interface_id interface id of this thread instance.
 * /param dst_address destination address
 *
 *return 0 success other values failure
 */
int8_t thread_management_client_pending_get(int8_t interface_id, uint8_t *dst_address);

/** send next pending configuration dataset
 *
 * /param interface_id interface id of this thread instance.
 * /param dst_address destination address
 *
 *return 0 success other values failure
 */
int8_t thread_management_client_pending_set(int8_t interface_id, uint8_t *dst_address);

/** Send application provisioning message
 *
 * /param interface_id interface id of this thread instance.
 * /param dst_address destination address
 * /param port destination port
 *
 *return 0 success other values failure
 */
int thread_management_client_provision_request(int8_t interface_id, uint8_t *dst_address, uint16_t port);

/** Send out Address Notification proactively.
 *
 *  \param interface_id interface id of this Thread instance.
 *  \param address The target address added to the interface.
 *  \param rloc RLOC responsible for the target address.
 *  \param ml_eid Mesh-local EID value.
 *  \param dst_addr The address to send the notification to.
 */
void thread_management_client_proactive_an(int8_t interface_id, const uint8_t address[16], const uint16_t rloc, const uint8_t ml_eid[8], const uint8_t dst_addr[16]);

/** Delete COAP message .
 *
 * Delete COAP message that is sent to COAP service.
 *
 *  \param interface_id interface id of this Thread instance.
 *  \param coap_message_id COAP message to be deleted.
 */
void thread_management_client_coap_message_delete(int8_t interface_id, uint16_t coap_message_id);

/** Clear old partition data.
 *
 * Clear data related to old partition, like pending COAP transactions.
 *
 *  \param interface_id interface id of this Thread instance.
 */
void thread_management_client_old_partition_data_clean(int8_t interface_id);

#ifdef HAVE_THREAD_V2
/** Send address notification message.
 *
 * Sends address notification message
 *
 *  \param interface_id interface id of this Thread instance.
 */
void thread_management_client_addr_ntf_send(int8_t interface_id, uint8_t *destination_address, const uint8_t *addr_data_ptr, uint8_t bbr_status);
/** Send multicast registration message
 *
 * include all the addresses that are registered to the bbr
 *
 *  \param interface_id interface id of this Thread instance.
 */
int thread_management_client_mlr_req_send(int8_t interface_id, const uint8_t br_addr[16], const uint8_t *address, uint8_t address_len);
#else

#define thread_management_client_addr_ntf_send(interface_id, destination_address, addr_data_ptr, bbr_status) (void(0))
#define thread_management_client_mlr_req_send(interface_id, br_addr, address, address_len) (void(0))
#endif


#endif /* THREAD_MANAGEMENT_CLIENT_H_ */
