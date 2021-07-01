/*
 * Copyright (c) 2015-2017, Pelion and affiliates.
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
 * Thread CoAP address resolution client implementation.
 *
 * This client must be enabled for ROUTER, REED and non-sleepy HOST.
 *
 */


#ifndef THREAD_RESOLUTION_CLIENT_H_
#define THREAD_RESOLUTION_CLIENT_H_
#include <ns_types.h>

#define TRACE_GROUP_THREAD_RESOLUTION_CLIENT "TRsC"

/** Notification callback.
 *
 * callback to inform when unsolicited notification is received
 *
 *  \param interface_id interface id of thread instance
 *  \param ip_addr IP address from response
 *  \param loc_addr 16-bit "RLOC" address from response
 *  \param ml_eid 64-bit extended MAC address from response
 */
typedef void thread_resolution_client_notification_cb(int8_t interface_id, const uint8_t ip_addr[16], uint16_t loc_addr, const uint8_t ml_eid[8]);

/** Resolution response callback.
 *
 * callback to inform when response to resolution is received
 *
 *  \param interface_id interface id of thread instance
 *  \param status status of operation 0 success, -1 failed
 *  \param ip_addr IP address from response
 *  \param loc_addr 16-bit "RLOC" address from response
 *  \param last_transaction_time last transaction time from response; 0 if not present
 *  \param mleid Target MLEID
 *
 */
typedef void thread_resolution_client_response_cb(int8_t interface_id, int8_t status, const uint8_t ip_addr[16], uint16_t loc_addr, uint32_t last_transaction_time, uint8_t *mleid);

/** Error callback.
 *
 * callback to inform when address error is received
 *
 *  \param interface_id interface id of thread instance
 *  \param ip_addr target IP address from error
 *  \param ml_eid 64-bit ML-EID from error
 */
typedef void thread_resolution_client_error_cb(int8_t interface_id, const uint8_t ip_addr[16], const uint8_t ml_eid[8]);

#ifdef HAVE_THREAD_NEIGHBOR_DISCOVERY

/** Initialise thread resolution client.
 *
 * This instance needs to be initialised once for each thread network interface.
 * if only one thread instance is supported this is needed to call only once.
 *
 * \param interface_id interface id of this thread instance.
 */

void thread_resolution_client_init(int8_t interface_id);

/** Delete Thread resolution client.
 *
 * \param interface_id interface id of this thread instance.
 */
void thread_resolution_client_delete(int8_t interface_id);


/** Set callback pointer for notifications
 *
 *  \param interface_id interface id of this thread instance.
 *  \param notification_cb callback function
 */
void thread_resolution_client_set_notification_cb(int8_t interface_id, thread_resolution_client_notification_cb notification_cb);

/** Set callback pointer for errors
 *
 *  \param interface_id interface id of this thread instance.
 *  \param error_cb callback function
 */
void thread_resolution_client_set_error_cb(int8_t interface_id, thread_resolution_client_error_cb error_cb);

/** Initiate address resolution
 *
 * Intent is that this API should ultimately implement collation of multiple
 * replies and selection by time indicators, and it should also implement the
 * "retry delay" blockage.
 *
 * Initial implementation will just be a direct shim to CoAP, and return every
 * POSTed response.
 *
 * \note The callback sets a per-instance callback pointer; it's not possible
 * to have different callbacks for multiple ongoing resolutions. Indeed, there
 * is currently no transaction state.
 *
 *  \param interface_id interface id of this thread instance.
 *  \param ip_addr IP address to resolve
 *  \param resp_cb callback to inform result of the request.
 *
 * return 0 success (resolution in progress), -1 failure
 */
int thread_resolution_client_resolve(int8_t interface_id, uint8_t ip_addr[16], thread_resolution_client_response_cb *resp_cb);

/** Send an address error message
 *
 * \param interface_id interface id of this thread instance.
 * \param dest_ip_addr address to send error message (unicast or multicast)
 * \param target_ip_addr target IP address that is in error
 * \param ml_eid ML-EID for error report (routers with different ML-EID are in error)
 */
int thread_resolution_client_address_error(int8_t interface_id, const uint8_t dest_ip_addr[16], const uint8_t target_ip_addr[16], const uint8_t ml_eid[8]);

/** Periodic timer
 *
 * Called to maintain address query set.
 *
 * \param interface_id interface id of this thread instance.
 * \param seconds number of seconds elapsed since last call (usually 1)
 */
void thread_resolution_client_timer(int8_t interface_id, uint16_t seconds);

#else //HAVE_THREAD_NEIGHBOR_DISCOVERY

#define thread_resolution_client_init(interface_id)
#define thread_resolution_client_delete(interface_id)
#define thread_resolution_client_set_notification_cb(interface_id, notification_cb)
#define thread_resolution_client_set_error_cb(interface_id, error_cb)
#define thread_resolution_client_resolve(interface_id, ip_addr, resp_cb) (-1)
#define thread_resolution_client_address_error(interface_id, dest_ip_addr, target_ip_addr, ml_eid)
#define thread_resolution_client_timer(interface_id, seconds)

#endif //HAVE_THREAD_NEIGHBOR_DISCOVERY

#endif /* THREAD_RESOLUTION_CLIENT_H_ */
