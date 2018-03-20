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
 * Thread CoAP management server implementation.
 *
 * This server must be enabled for all devices in thread network.
 * Normal operation allows
 * - Thread diagnostic API
 * - Joiner router forwarding.
 *
 * Leader mode
 * Following additional services are required from the leader of Thread network
 * - Router id assignment server.
 * - Thread commissioning registration support
 * - Thread management command support
 * - thread network data handling support
 */


#ifndef THREAD_MANAGEMENT_SERVER_H_
#define THREAD_MANAGEMENT_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ns_types.h>
#include "thread_management_if.h"

typedef struct thread_management_server_data {
    uint16_t joiner_router_port;
    uint16_t commissioner_port;
    bool joiner_router_enabled: 1;
} thread_management_server_data_t;


struct sn_coap_hdr_;

/** initialise CoAP thread management server
 * This instance needs to be initialised for all thread devices.
 *
 * When this is initialised this will enable the default management functionality for all the nodes in thread network,.
 *
 * /param interface_id interface id of this thread instance.
 */
int thread_management_server_init(int8_t interface_id);

/** Delete thread management services.
 *
 * When this is called it close selected service and free all allocated memory.
 *
 *  /param interface interface id of this thread instance.
 */
void thread_management_server_delete(int8_t interface_id);

/** initialise Joiner router module
 *
 *  This is called when commissioner becomes present in network.
 *  This will open the listening port for joiner device and starts forwarding traffic to
 *  the commissioner device.
 *
 * /param interface interface id of this thread instance.
 *
 */
int thread_management_server_joiner_router_init(int8_t interface_id);

/** De-initialise Joiner router module
 *
 * Closes joiner router module.
 *
 * /param interface interface id of this thread instance.
 *
 */
void thread_management_server_joiner_router_deinit(int8_t interface_id);
int8_t thread_management_server_interface_id_get(int8_t coap_service_id);

int thread_management_server_commisoner_data_get(int8_t interface_id, thread_management_server_data_t *server_data);

/**
 * Check if address can be found from network.
 *
 * \param interface_id
 * \param source_address
 *
 * \return true if address can be found from network, false otherwise.
 */
bool thread_management_server_source_address_check(int8_t interface_id, uint8_t source_address[16]);

/**
 * Handler for TMF GET-request messages:
 * -MGMT_GET (uri = /c/mg),
 * -MGMT_ACTIVE_GET (uri = /c/ag),
 * -MGMT_PENDING_GET (uri = /c/pg)
 *
 * \param interface_id
 * \param coap_service_id COAP service id
  * \param request_ptr COAP Request to handle
 *
 * \return 0 if response to request is send, negative value in case of error.
 */
int thread_management_server_tmf_get_request_handler(int8_t interface_id, int8_t coap_service_id, struct sn_coap_hdr_ *request_ptr);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_MANAGEMENT_SERVER_H_ */
