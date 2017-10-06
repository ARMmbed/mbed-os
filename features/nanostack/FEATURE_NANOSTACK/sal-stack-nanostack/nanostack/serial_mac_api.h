/*
 * Copyright (c) 2016-2017 ARM Limited. All rights reserved.
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

/** \file serial_mac_api.h
 * \brief Serial MAC API.
 */

#ifndef SERIAL_MAC_API_H_
#define SERIAL_MAC_API_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

struct virtual_data_req_s;

typedef struct serial_mac_api_s serial_mac_api_t;

/**
 * Create serial MAC
 * @param serial_driver_id PHY driver ID.
 * @return Serial MAC callback structure on success.
 * @return NULL on failure.
 */
extern serial_mac_api_t *serial_mac_create(int8_t serial_driver_id);

/**
 * @brief data_request data request call
 * @param api API to handle the request
 * @param data Data to be sent
 * @param data_length Length of the data
 */
typedef int8_t data_request(const serial_mac_api_t* api, const uint8_t *data_ptr, uint16_t data_length);


/**
 * @brief data_indication Data indication is called when MAC layer has received data
 * @param api The API which handled the response
 * @param data Data to be sent
 * @param data_length Length of the data
 */
typedef void data_indication(const serial_mac_api_t *api, const uint8_t *data_ptr, uint16_t data_length);

/**
 * * @brief serial_mac_api_initialize Initialises Serial MAC layer into use
 * @param api API to initialize
 * @param ind_cb Callback to call when data is received.
 * @return 0 if success, -1 if api is invalid
 */
typedef int8_t serial_mac_api_initialize(serial_mac_api_t *api, data_indication *ind_cb);

/**
 * @brief serial_mac_virtual_initialize Initialises MAC to use virtual driver
 * @param api API to start using virtual driver
 * @param driver_id Virtual driver ID. Must be valid.
 * @return 0 if success, -1 if api or driver is invalid
 */
typedef int8_t serial_mac_virtual_initialize(const serial_mac_api_t *api, int8_t driver_id);

/**
 * Serial MAC callback structure.
 */
struct serial_mac_api_s {
    serial_mac_api_initialize   *mac_initialize;            /**< Inititilize data callback */
    serial_mac_virtual_initialize * virtual_initilize;      /**< Enable bridge to virtual driver */
    data_request                *data_request_cb;           /**< Data request callback */
    data_indication             *data_ind_cb;               /**< Data indication callback */
};

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_MAC_API_H_ */
