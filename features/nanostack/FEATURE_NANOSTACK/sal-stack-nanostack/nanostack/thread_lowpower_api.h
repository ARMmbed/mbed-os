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

/**
 * \file thread_lowpower_api.h
 * \brief Nanostack Thread lowpower probe implementation. Responsible for tasks like
 * request link metrics by sending data requests.
 */

#ifndef THREAD_LOWPOWER_API_H_
#define THREAD_LOWPOWER_API_H_

#include <ns_types.h>

/** \brief callback function that returns the received metrics
 *
 * \param destination_address address to which the lowpower query is sent
 * \param interface_id interface_id of destination
 * \param metrics_ptr Pointer to metrics
 * \param metrics_len Length of metrics_ptr
 *
 * \return metrics_ptr if response received, NULL if no response is received
 * \return metrics_len length of the response received, 0 if NULL response is received
 *
 */
typedef int (thread_lowpower_resp_cb)(uint8_t *destination_address, int8_t interface_id, uint8_t *metrics_ptr, uint16_t metrics_len);

/** \brief Send lowpower data request for requesting low power metrics
 *
 * \param interface_id Interface ID of the Thread network.
 * \param address destination ll64 address
 * \param series_id id of the query or series id
 * \param series_flags query type 0 for single probe
 * \param timeout Timeout for the query 0 for single probe
 * \param metrics_ptr array of metrics to be measured
 * \param metrics_len length of the metrics array
 * \param response_cb callback function called to return values after execution
 *
 * \return 0 if data request successfully initiated.
 *
 */
int thread_lowpower_metrics_management_request_send(int8_t interface_id, uint8_t *address, uint8_t series_id, uint8_t series_flags, uint16_t timeout,  uint8_t *metrics_ptr, uint8_t metrics_len, thread_lowpower_resp_cb response_cb);

/** \brief DEPRECATED Send lowpower data request for requesting low power metrics
 *
 * use thread_lowpower_metrics_management_request_send instead
 * \param interface_id Interface ID of the Thread network.
 * \param destination_address destination ll64 address
 * \param metrics_ptr array of metrics to be measured
 * \param metrics_len length of the metrics array
 * \param response_cb callback function called to return values after execution
 *
 * \return 0 if data request successfully initiated.
 *
 */
int thread_lowpower_test_probe_send(int8_t interface_id, uint8_t *destination_address, uint8_t *metrics_ptr, uint8_t metrics_len, thread_lowpower_resp_cb response_cb);

#endif /*THREAD_LOWPOWER_API_H_ */
