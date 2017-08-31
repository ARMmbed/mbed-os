/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
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
