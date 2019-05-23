/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

#ifdef HAVE_THREAD_V2

void thread_extension_allocate(protocol_interface_info_entry_t *cur);
void thread_extension_free(protocol_interface_info_entry_t *cur);
void thread_extension_init(int8_t interface_id, int8_t coap_service_id);
void thread_extension_mtd_service_register(protocol_interface_info_entry_t *cur);
void thread_extension_network_data_process(struct protocol_interface_info_entry *cur);
int thread_extension_primary_bbr_get(struct protocol_interface_info_entry *cur, uint8_t *addr_ptr, uint8_t *seq_ptr, uint32_t *timer1_ptr, uint32_t *timer2_ptr);
void thread_extension_address_registration(struct protocol_interface_info_entry *interface, const uint8_t *addr, const uint8_t *child_mac64, bool refresh_child_entry, bool duplicate_child_detected);
void thread_extension_child_address_registration_response_process(struct protocol_interface_info_entry *interface);
void thread_extension_dua_address_generate(protocol_interface_info_entry_t *cur, const uint8_t *domain_prefix, uint8_t domain_prefix_len);
void thread_extension_aloc_generate(struct protocol_interface_info_entry *cur);
bool thread_extension_aloc_map(protocol_interface_info_entry_t *cur, uint16_t *addr16);
void thread_extension_mcast_subscrition_change(protocol_interface_info_entry_t *interface);
void thread_extension_address_registration_trigger(protocol_interface_info_entry_t *interface);
bool thread_extension_enabled(protocol_interface_info_entry_t *cur);
bool thread_extension_version_check(uint8_t version);
void thread_extension_discover_response_read(struct discovery_response_list *nwk_info, uint16_t discover_response_tlv, uint8_t *data_ptr, uint16_t data_len);
void thread_extension_discover_response_tlv_write(uint16_t *data, uint8_t version, uint16_t securityPolicy);
int thread_extension_service_init(protocol_interface_info_entry_t *cur);
void thread_extension_addr_ntf_send(struct protocol_interface_info_entry *cur, uint8_t *destination_address, const uint8_t *addr_data_ptr, uint8_t bbr_status);
#ifdef HAVE_THREAD_ROUTER
bool thread_extension_joining_enabled(int8_t interface_id);
bool thread_extension_is_reed_upgrade_allowed(protocol_interface_info_entry_t *cur);
uint8_t thread_extension_discover_response_len(protocol_interface_info_entry_t *cur);
uint8_t *thread_extension_discover_response_write(protocol_interface_info_entry_t *cur, uint8_t *ptr);

#else
#define thread_extension_joining_enabled(interface_id) (false)
#define thread_extension_is_reed_upgrade_allowed(cur) (true)
#define thread_extension_discover_response_len(cur) (0)
#define thread_extension_discover_response_write(cur, ptr) (ptr)
#endif //HAVE_THREAD_ROUTER
#else

#define thread_extension_allocate(cur) ((void) 0)
#define thread_extension_free(cur) ((void) 0)
#define thread_extension_init(interface_id,coap_service_id) ((void) 0)
#define thread_extension_network_data_process(cur) ((void) 0)
#define thread_extension_primary_bbr_get(cur,addr_ptr,seq_ptr,timer1_ptr, timer2_ptr) (-1)
#define thread_extension_address_registration(interface,addr,child_mac64,refresh_child_entry,duplicate_child_detected) ((void) 0)
#define thread_extension_child_address_registration_response_process(interface) ((void) 0)
#define thread_extension_aloc_generate(cur) ((void) 0)
#define thread_extension_aloc_map(cur, addr16) (false)
#define thread_extension_mcast_subscrition_change(interface) ((void) 0)
#define thread_extension_enabled(cur) (false)
#define thread_extension_is_reed_upgrade_allowed(cur) (true)
#define thread_extension_version_check(version) (false)
#define thread_extension_discover_response_read(nwk_info, discover_response_tlv, data_ptr, data_len) ((void) 0)
#define thread_extension_discover_response_tlv_write(data, version, securityPolicy) ((void) 0)
#define thread_extension_service_init(cur) ((void) 0)
#define thread_extension_joining_enabled(interface_id) (false)
#define thread_extension_discover_response_len(cur) (0)
#define thread_extension_discover_response_write(cur, ptr) (ptr)
#define thread_extension_addr_ntf_send(cur,destination_address,addr_data_ptr,bbr_status) ((void) 0)
#define thread_extension_dua_address_generate(cur, domain_prefix, domain_prefix_len) ((void) 0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* THREAD_EXTENSION_H_ */
