/*
 * Copyright (c) 2018, 2021, Pelion and affiliates.
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

#ifndef MPX_API_H_
#define MPX_API_H_
#include "mac_mcps.h"
struct mcps_data_req_s;
struct mcps_data_conf_s;
struct mcps_data_ind_s;
struct mcps_purge_s;

typedef struct mpx_api_s mpx_api_t;

/**
 * @brief mpx_data_request MPX_DATA request with user ID
 * @param api API to handle the request
 * @param data MCPS-DATA.request specific values
 * @param user_id MPX user ID
 * @param priority priority level
 *
 */
typedef void mpx_data_request(const mpx_api_t *api, const struct mcps_data_req_s *data, uint16_t user_id, mac_data_priority_t priority);

/**
 * @brief mpx_data_queue_clean clean MPX user data
 * @param api API to handle the request
 * @param purge MCPS-purge request
 * @param user_id MPX user ID
 * @return 0 if purge requst was OK, non-zero otherwise
 *
 */
typedef uint8_t mpx_data_purge_request(const mpx_api_t *api, struct mcps_purge_s *purge, uint16_t user_id);

/**
 * @brief mpx_data_confirm MPX-DATA confirm is called as a response to MPX-DATA request
 * @param api The API which handled the response
 * @param data MCPS-DATA.confirm specific values
 * @param user_id MPX user ID
 */
typedef void mpx_data_confirm(const mpx_api_t *api, const struct mcps_data_conf_s *data);

/**
 * @brief mpx_data_indication MPX-DATA confirm is called as a response to MPX-DATA request
 * @param api The API which handled the response
 * @param data MCPS-DATA.indication specific values
 * @param user_id MPX user ID
 */
typedef void mpx_data_indication(const mpx_api_t *api, const struct mcps_data_ind_s *data);

/**
 * @brief mpx_header_size_get Function for request MPX user head room size
 * @param api The API which handled the response
 * @param user_id MPX user ID
 *
 * @return >0 Head room size in bytes
 * @return 0 When Unknown User Id
 */
typedef uint16_t mpx_header_size_get(const mpx_api_t *api, uint16_t user_id);

/**
 * @brief mpx_data_cb_register MPX-DATA confirm cb register by user
 * @param api The API which handled the response
 * @param confirm_cb MPX Data Confirm call back
 * @param indication_cb MPX Data indication
 * @param user_id MPX user ID
 *
 * @return 0 register OK
 * @return -1 Unknown User ID
 */
typedef int8_t mpx_data_cb_register(const mpx_api_t *api, mpx_data_confirm *confirm_cb, mpx_data_indication *indication_cb, uint16_t user_id);

/**
 * @brief mpx_eui64_purge_request Purge EUI-64 related data from MPX
 * @param api The API which handled the response
 * @param eui64 EUI-64 to purge
 *
 */
typedef void mpx_eui64_purge_request(const mpx_api_t *api, const uint8_t *eui64);

/**
 * \brief mpx_high_priority_mode_set Enable/Disable MPX high priority mode for TX process
 * @param api The API which handled the response
 * @param enable_mode True for enable High Priority mode, False disable
 *
 */
typedef void mpx_high_priority_mode_set(const mpx_api_t *api, bool enable_mode);

/**
 * \brief Struct mpx_api_s defines functions for MPX user for register call backs and send data.
 */
struct mpx_api_s {
    mpx_data_request *mpx_data_request;             /**< MPX data request. */
    mpx_data_purge_request *mpx_data_purge;            /**< MPX data Purge. */
    mpx_header_size_get *mpx_headroom_size_get;     /**< MPX headroom size get in bytes. */
    mpx_data_cb_register *mpx_user_registration;    /**< MPX User cb registration must be call before enable to send or RX data*/
    mpx_eui64_purge_request *mpx_eui64_purge;       /**< MPX Purge EUI-64 related data */
    mpx_high_priority_mode_set *mpx_priority_mode_set; /**< MPX request to enable / disable High Priority mode */
};


#endif /* MPX_API_H_ */
