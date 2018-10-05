/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#ifndef MAC_RESPONSE_HANDLER_H
#define MAC_RESPONSE_HANDLER_H

#include "mac_api.h"

struct mcps_data_ind_s;
struct mcps_data_conf_t;

void mcps_data_confirm_handler( const mac_api_t* api, const struct mcps_data_conf_s *data );

void mcps_data_indication_handler( const mac_api_t* api, const struct mcps_data_ind_s *data );

void mcps_purge_confirm_handler( const mac_api_t* api, mcps_purge_conf_t *data );

void mlme_confirm_handler( const mac_api_t* api, mlme_primitive id, const void *data );

void mlme_indication_handler( const mac_api_t* api, mlme_primitive id, const void *data );

#endif // MAC_RESPONSE_HANDLER_H
