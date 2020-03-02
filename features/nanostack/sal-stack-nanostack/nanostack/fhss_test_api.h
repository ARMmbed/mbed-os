/*
 * Copyright (c) 2020, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file fhss_test_api.h
 * \brief
 */

#ifndef FHSS_TEST_API_H
#define FHSS_TEST_API_H


#ifdef __cplusplus
extern "C" {
#endif

/**
  * \brief Set optimal packet length
  *
  * \param fhss_api FHSS instance.
  * \param packet_length Optimal packet length
  *
  * \return  0 Success
  * \return -1 Failure
  */
int8_t fhss_set_optimal_packet_length(const fhss_api_t *fhss_api, uint16_t packet_length);

#ifdef __cplusplus
}
#endif

#endif // FHSS_TEST_API_H
