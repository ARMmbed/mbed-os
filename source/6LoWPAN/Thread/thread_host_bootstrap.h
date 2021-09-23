/*
 * Copyright (c) 2015-2018, Pelion and affiliates.
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

/*
 * \file thread_host_bootstrap.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef THREAD_HOST_BOOTSTRAP_H_
#define THREAD_HOST_BOOTSTRAP_H_

#include "Service_Libs/mle_service/mle_service_api.h"
#include "MLE/mle.h"
struct protocol_interface_info_entry;
struct mle_security_header;

void thread_network_attach_start(struct protocol_interface_info_entry *cur);
bool thread_host_bootstrap_child_update(struct protocol_interface_info_entry *cur, const uint8_t *mac64);
int thread_host_bootstrap_child_update_negative_response(protocol_interface_info_entry_t *cur, uint8_t *dstAddress, mle_tlv_info_t *challenge);
void thread_child_set_default_route(struct protocol_interface_info_entry *cur);

void thread_mle_parent_discover_receive_cb(int8_t interface_id, mle_message_t *mle_msg, struct mle_security_header *security_headers);

void thread_endevice_synch_start(struct protocol_interface_info_entry *cur);

#endif /* THREAD_HOST_BOOTSTRAP_H_ */
