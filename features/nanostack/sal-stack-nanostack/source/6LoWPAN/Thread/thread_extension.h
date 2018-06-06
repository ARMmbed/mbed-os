/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
 * \brief Thread extensions for network bootstrap.
 */

#ifndef THREAD_EXTENSION_H_
#define THREAD_EXTENSION_H_

#ifdef __cplusplus
extern "C" {
#endif

struct discovery_response_list;

#define thread_extension_allocate(cur)
#define thread_extension_free(cur)
#define thread_extension_init(interface_id,coap_service_id)
#define thread_extension_network_prefix_get(interface_id,options_ptr,prefix_ptr,prefix_len) (-1)
#define thread_extension_network_data_process(cur)
#define thread_extension_primary_bbr_get(cur,addr_ptr,seq_ptr,timer1_ptr, timer2_ptr) (-1)
#define thread_extension_address_registration(interface,addr,child_mac64)
#define thread_extension_address_generate(cur)
#define thread_extension_aloc_generate(cur)
#define thread_extension_aloc_map(cur, addr16) false
#define thread_extension_mcast_subscrition_change(interface, group, added)
#define thread_extension_route_set(cur)
#define thread_extension_activate(cur)
#define thread_extension_enabled(cur) (false)
#define thread_extension_version_check(version) (false)
#define thread_extension_discover_response_read(nwk_info, discover_response_tlv, data_ptr, data_len)
#define thread_extension_discover_response_tlv_write(data, version, extension_bit) (data)
#define thread_extension_joiner_router_init(interface_id)
#define thread_extension_joining_enabled(interface_id) false
#define thread_extension_discover_response_len(cur) 0
#define thread_extension_discover_response_write(cur, ptr) (ptr)

#ifdef __cplusplus
}
#endif

#endif /* THREAD_EXTENSION_H_ */
