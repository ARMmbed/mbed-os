/*
 * Copyright (c) 2017-2019, Arm Limited and affiliates.
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

#ifndef THREAD_CCM_H_
#define THREAD_CCM_H_


#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_THREAD_V2
void thread_ccm_free(protocol_interface_info_entry_t *cur);

int thread_ccm_device_certificate_set(protocol_interface_info_entry_t *cur, const unsigned char *device_certificate_ptr, uint16_t device_certificate_len, const unsigned char *priv_key_ptr, uint16_t priv_key_len);

int thread_ccm_network_certificate_set(protocol_interface_info_entry_t *cur, const unsigned char *domain_certificate_ptr, uint16_t domain_certificate_len);

int thread_ccm_network_private_key_set(protocol_interface_info_entry_t *cur, const unsigned char *priv_key_ptr, uint16_t priv_key_len);

bool thread_ccm_network_certificate_available(protocol_interface_info_entry_t *cur);

int thread_ccm_thread_name_set(protocol_interface_info_entry_t *cur, char thread_name[16]);

int thread_ccm_commission_start(protocol_interface_info_entry_t *cur, uint8_t parent_address[16], discovery_response_list_t *nwk_info, thread_commission_done_cb *done_cb);

discovery_response_list_t *thread_ccm_network_select(protocol_interface_info_entry_t *cur, thread_nwk_discovery_response_list_t *discover_response);

uint8_t thread_ccm_thread_name_length_get(protocol_interface_info_entry_t *cur);

uint8_t *thread_ccm_thread_name_ptr_get(protocol_interface_info_entry_t *cur);

int thread_ccm_network_certificate_enable(protocol_interface_info_entry_t *cur, int8_t coap_service_id);

int thread_ccm_reenrollment_start(protocol_interface_info_entry_t *cur, int8_t service_id, uint8_t *pbbr_addr);

#else
#define thread_ccm_free(cur);
#define thread_ccm_device_certificate_set(cur, device_certificate_ptr, device_certificate_len, priv_key_ptr, priv_key_len) (-1)
#define thread_ccm_network_certificate_set(cur, domain_certificate_ptr, domain_certificate_len) (-1)
#define thread_ccm_network_private_key_set(cur, priv_key_ptr, priv_key_len) (-1)
#define thread_ccm_network_certificate_available(cur) (false)
#define thread_ccm_thread_name_set(cur, thread_name) (-1)
#define thread_ccm_commission_start(cur, parent_address, port, done_cb) (-1)
#define thread_ccm_network_select(cur, discovery_response) (NULL)
#define thread_ccm_thread_name_length_get(cur) (0)
#define thread_ccm_thread_name_ptr_get(cur) (NULL)
#define thread_ccm_network_certificate_enable(cur, coap_service_id) (NULL)
#define thread_ccm_reenrollment_start(cur, service_id, pbbr_addr) (-1)
#endif

#ifdef __cplusplus
}
#endif
#endif /* THREAD_CCM_H_ */
