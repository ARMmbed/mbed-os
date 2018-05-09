/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef THREAD_EXTENSION_BOOTSTRAP_H_
#define THREAD_EXTENSION_BOOTSTRAP_H_


#ifdef __cplusplus
extern "C" {
#endif

#define thread_extension_bootstrap_free(cur);
#define thread_extension_bootstrap_device_certificate_set(cur, device_certificate_ptr, device_certificate_len, priv_key_ptr, priv_key_len) (-1)
#define thread_extension_bootstrap_network_certificate_set(cur, domain_certificate_ptr, domain_certificate_len) (-1)
#define thread_extension_bootstrap_network_private_key_set(cur, priv_key_ptr, priv_key_len) (-1)
#define thread_extension_bootstrap_thread_name_set(cur, thread_name) (-1)
#define thread_extension_bootstrap_commission_start(cur, parent_address, port, done_cb) (-1)
#define thread_extension_bootstrap_network_select(cur, discovery_response) (NULL)
#define thread_extension_bootstrap_thread_name_length_get(cur) (0)
#define thread_extension_bootstrap_thread_name_ptr_get(cur) (NULL)
#define thread_extension_bootstrap_network_certificate_enable(cur, coap_service_id) (NULL)

#ifdef __cplusplus
}
#endif
#endif /* THREAD_EXTENSION_BOOTSTRAP_H_ */
