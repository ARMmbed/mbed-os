/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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
 * \file mle_service_interface.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef MLE_SERVICE_INTERFACE_H_
#define MLE_SERVICE_INTERFACE_H_
#include <ns_types.h>
#include "ns_list.h"

#include "Service_Libs/mle_service/mle_service_api.h"

typedef struct {
    mle_service_receive_cb *recv_cb; /*!< Interface message handler */
    mle_service_receive_security_bypass_cb *recv_security_bypass_cb; /*!< Interface message security bypass handler */
    void *interface_ptr;
    uint8_t mac64[8]; /*!< Interface MAC 64 used for Security nonce and LL64 address define */
    int8_t interface_id; /*!< Interface Id */
    uint8_t challenge_length; /*!< Define MLE challenge length */
    uint16_t URT; /*!< Unicast Retransmission timeout */
    uint16_t MRT; /*!< Multicast Retransmission timeout */
    uint8_t MRC; /*!< Maximum retransmission count */
    int16_t token_bucket; /*!< Token bucket */
    uint16_t token_bucket_ticks; /*!< Token bucket ticks */
    uint8_t token_bucket_size; /*!< MLE token bucket size */
    uint8_t token_bucket_rate; /*!< Rate in which tokens are added to bucket (0.1 seconds * rate) */
    uint8_t token_bucket_count; /*!< Count of tokens added to bucket at a time */
    ns_list_link_t link;
} service_instance_t;

typedef NS_LIST_HEAD(service_instance_t, link) service_instance_list_t;

/**
 * Find registered service client class
 *
 * \param interface_id interface id
 *
 * \return >0 Pointer to interface structure
 * \return < 0 Unknown interface id
 */
service_instance_t *mle_service_interface_find(int8_t interface_id);

/**
 * Allocate or return current registered service client class
 *
 * \param interface_id interface id
 *
 * \return >0 Pointer to interface structure
 * \return < 0 Allocate fail
 */
service_instance_t *mle_service_interface_get(int8_t interface_id);

/**
 * Delete and free service
 */
int mle_service_interface_delete(int8_t interface_id);
/**
 * Check is all interfaced freed
 */
bool mle_service_interface_list_empty(void);

/**
 *  MLE service interface timer call back
 *
 *  This function will be called when timer is updated
 *
 * \param ticks
 *
 * \return true Keep timer active
 * \return false Stop timer
 *
 */
typedef bool (mle_service_interface_timer_cb)(uint16_t ticks, service_instance_t *cur_ptr);

/**
 * MLE service interface timeout handling
 *
 * \param ticks timer ticks
 * \param timeout_cb callback called ticks are updated
 *
 * \return true Keep timer active
 * \return false Stop timer
 */
bool mle_service_interface_timer_tick(uint16_t ticks, mle_service_interface_timer_cb *timeout_cb);

#endif /* MLE_SERVICE_INTERFACE_H_ */
