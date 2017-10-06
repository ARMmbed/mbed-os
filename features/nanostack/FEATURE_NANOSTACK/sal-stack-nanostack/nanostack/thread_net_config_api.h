/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
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
