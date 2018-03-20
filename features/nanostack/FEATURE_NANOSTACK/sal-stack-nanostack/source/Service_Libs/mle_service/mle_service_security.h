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

/*
 * \file mle_service_security.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef MLE_SERVICE_SECURITY_H_
#define MLE_SERVICE_SECURITY_H_
#include <ns_types.h>
#include "Service_Libs/mle_service/mle_service_api.h"

struct mle_security_header;

#define MLE_MAX_KEY_TABLE_SIZE 2

typedef struct mle_security_key {
    uint8_t aes_key[16]; /*!< Security key */
    uint8_t key_id; /*!< Key id */
    bool primary_key:1; /*!< define primary or secondary key */
    bool pending_primary:1; /*!< define key switch to primary when key is request or used first time when it is true */
    bool key_valid:1; /*!< Define that structure include valid data */
} mle_security_key_t;


typedef struct mle_security_components {
    mle_security_key_t mle_security_key_table[MLE_MAX_KEY_TABLE_SIZE]; /*!< Security key table */
    uint32_t security_frame_counter; /*!< Security frame counter */
    uint8_t sec_level; /*!< Security level for message*/
    mle_service_key_request_by_counter_cb * key_req; /*!< Security key request for 32-bit key source*/
    mle_service_security_notify_cb *security_notify; /*!< Security notify event call back*/
} mle_security_components_t;

/**
 * Registered mle service security class for given interface
 *
 * \param interface_id interface id
 *
 * \return 0 allocate ok
 * \return < 0 allocate fail
 */
int mle_service_security_instance_allocate(int8_t interface_id);

/**
 * Find registered service client class by given interface id
 *
 * \param interface_id interface id
 *
 * \return >0 Pointer to interface structure
 * \return < 0 Unknown interface id
 */
mle_security_components_t * mle_service_security_params_get(int8_t interface_id);

/**
 * Free mle service security class
 *
 * \param interface_id interface id
 *
 * \return 0 free ok
 * \return < 0 Unknown interface id
 */
int mle_service_security_instance_delete(int8_t interface_id);

/**
 * Init default settings to mle service security class
 *
 * Clean all keys from class
 *
 * \param sec_ptr security class pointer
 *
 */
void mle_service_security_parameters_init(mle_security_components_t *sec_ptr);

/**
 * Get default security key id
 *
 *
 * \param sec_ptr security class pointer
 *
 * \return key id
 */
uint8_t mle_service_security_get_default_key_id(mle_security_components_t *sec_ptr);


/**
 * Get security key class by given id
 *
 *
 * \param sec_ptr security class pointer
 * \param key_id key id
 *
 * \return >0 valid key class pointer
 * \retun NULL when key is unknown
 */
mle_security_key_t * mle_service_security_key_get(mle_security_components_t *sec_ptr, uint8_t key_id );

/**
 * Get / update security class mle security counter
 *
 *
 * \param sec_ptr security class pointer
 * \param increment true update counter false return current value
 *
 * \return security counter
 */
uint32_t mle_service_security_get_framecounter(mle_security_components_t *sec_ptr, bool increment);

/**
 * Get security key pointer
 *
 *
 * \param securityHeader pointer to mle message security header
 * \param sec_ptr security class pointer
 *
 * \return >0 Valid key pointer
 * \return NULL unknown key
 */
uint8_t *mle_service_security_get_key(struct mle_security_header *securityHeader, mle_security_components_t *sec_ptr, int8_t interfaceId);

/**
 * MLE security pending key trig by key id
 *
 * \param sec_ptr security class pointer
 * \param keyId pending key id
 *
 * \return true trig ok
 * \return false key id not valid
 */
bool mle_service_security_key_update_trig(uint8_t interface_id, mle_security_components_t *sec_ptr, uint8_t keyId);

/**
 * MLE security key update
 *
 * \param sec_ptr security class pointer
 * \param key pointer to 128-bit key
 * \param keyId key id
 * \param set_primary true Primary key false pending key
 *
 * \return true update ok
 * \return false update fail
 */
bool mle_service_security_key_set(mle_security_components_t *sec_ptr, const uint8_t *key, uint8_t keyId, bool set_primary);

/**
 * MLE security default key get
 *
 * \param sec_ptr security class pointer
 *
 * \return > 0 Pointer to default key
 * \return NULL no default key available
 */
uint8_t *mle_service_security_get_default_key(mle_security_components_t *sec_ptr);

/**
 * MLE security default key id get
 *
 * \param sec_ptr security class pointer
 *
 * \return > 0 Key id
 * \return 0 default key is not available
 */
uint8_t mle_service_security_get_next_key_id(mle_security_components_t *sec_ptr);

/**
 * MLE security Secondary key get
 *
 * \param sec_ptr security class pointer
 *
 * \return > 0 Key pointer
 * \return NULL key is not available
 */
uint8_t *mle_service_security_get_next_key(mle_security_components_t *sec_ptr);

#endif /* MLE_SERVICE_SECURITY_H_ */
