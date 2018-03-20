/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
 * Thread CoAP address resolution server implementation.
 *
 * This server must be enabled for all routers and non-sleepy end devices.
 *
 * Routers respond on behalf of their sleepy children.
 */


#ifndef THREAD_RESOLUTION_SERVER_H_
#define THREAD_RESOLUTION_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ns_types.h>

/** Address resolution request.
 *
 * This callback is called when a device is requesting address resolution.
 *
 * \param interface_id interface id of this thread instance.
 * \param[in] target_addr target IP address being resolved
 * \param[out] addr_out pointer to write resulting RLOC16 address
 * \param[out] proxy write as true if responding for a child device
 * \param[out] last_transaction_time last transaction time for response (if proxy is true)
 * \param[out] ML-EID for target address
 *
 * /return return 0 for success, negative if unable to respond.
 */
typedef int thread_resolution_server_addr_query_cb(int8_t interface_id, const uint8_t target_addr[static 16], uint16_t *addr_out, bool *proxy, uint32_t *last_transaction_time, uint8_t *mleid_ptr);

#ifdef HAVE_THREAD_NEIGHBOR_DISCOVERY

/** initialise CoAP thread resolution server
 * This instance needs to be initialised for all thread devices.
 *
 * When this is initialised this will enable the default management functionality for all the nodes in thread network,.
 *
 * \param interface interface id of this thread instance.
 *
 */
int thread_resolution_server_init(int8_t interface_id, thread_resolution_server_addr_query_cb *query_cb);

/** Delete thread resolution services.
 *
 * When this is called it close selected service and free all allocated memory.
 *
 *  \param interface interface id of this thread instance.
 */
void thread_resolution_server_delete(int8_t interface_id);

#else //HAVE_THREAD_NEIGHBOR_DISCOVERY

#define thread_resolution_server_init(interface_id, query_cb)
#define thread_resolution_server_delete(interface_id)

#endif  //HAVE_THREAD_NEIGHBOR_DISCOVERY

#ifdef __cplusplus
}
#endif

#endif /* THREAD_RESOLUTION_SERVER_H_ */
