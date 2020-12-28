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
 * \file thread_net_config_api.h
 * \brief Public API to handle the Thread network services and configuration.
 */

#ifndef _THREAD_NET_CONFIG_API_H_
#define _THREAD_NET_CONFIG_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

/**
  * \brief function callback type for nd_data_request.
  *
  * \param inteface_id Network interface ID where request was made.
  * \param status 0 when response is received from destination, -1 otherwise.
  * \param data_ptr ND_data options encoded according to RFC6106. Is NULL if destination was unreachable or didn't have the requested data.
  * \param data_len Length of data in bytes.
 */
typedef void thread_net_config_nd_data_req_cb(int8_t interface_id, int8_t status, uint8_t *data_ptr, uint16_t data_len);

/**
 * \brief Request ND options (as in RFC6106) from given destination.
 * Response data will be provided in callback function.
 *
 * \param interface_id network interface ID.
 * \param destination IPv6 address where request is sent.
 * \param options requested option type identifiers according to RFC6106.
 * \param options_len number of options requested.
 * \param callback Function that will be called once information is available.
 *
 * \return 0 on success. A callback will be called with/without response data.
 * \return <0 in error cases. Callback will not be called.
 */
int thread_net_config_nd_data_request(int8_t interface_id, const uint8_t destination[16], const uint8_t *options, uint8_t options_len, thread_net_config_nd_data_req_cb *callback);

#ifdef __cplusplus
}
#endif
#endif /* _THREAD_NET_CONFIG_API_H_ */
