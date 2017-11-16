/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
 * \file net_fhss.h
 * \brief FHSS API
 *
 */

#ifndef NET_FHSS_H_
#define NET_FHSS_H_
#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates FHSS API instance which will be registered to software MAC.
 * @param fhss_configuration Basic FHSS configuration.
 * @param fhss_timer FHSS platform timer interface and configuration.
 * @param fhss_statistics FHSS statistics storage.
 * @return New FHSS instance if successful, NULL otherwise.
 */
extern fhss_api_t *ns_fhss_create(const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics);

/**
 * @brief Set synchronization time configuration for FHSS network. Should be only called from Border router.
 * @param fhss_api FHSS instance.
 * @param fhss_synch_configuration FHSS synchronization time configuration.
 * @return 0 on success, -1 on fail.
 */
extern int ns_fhss_configuration_set(fhss_api_t *fhss_api, const fhss_synch_configuration_t *fhss_synch_configuration);

/**
 * @brief Deletes a FHSS API instance and removes it from software MAC.
 * @param fhss_api FHSS instance.
 * @return 0 on success, -1 on fail.
 */
extern int ns_fhss_delete(fhss_api_t *fhss_api);


#ifdef __cplusplus
}
#endif

#endif /* NET_FHSS_H_ */
