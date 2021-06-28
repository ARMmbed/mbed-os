/*
 * Copyright (c) 2016-2021, Pelion and affiliates.
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

#ifndef KMP_API_H_
#define KMP_API_H_

/*
 * KMP API and KMP service module. KMP API offers key management security
 * protocols interface towards application.
 *
 * KMP service provides security protocols access to network, timing
 * services and callback services. Application must configure KMP service
 * before using KMP API.
 *
 */

typedef enum {
    KMP_TYPE_NONE = 0,

    IEEE_802_1X_MKA        = 1,
    RADIUS_IEEE_802_1X_MKA = 2,
    MSG_PROT               = 5,
    IEEE_802_11_4WH        = 6,
    IEEE_802_11_GKH        = 7,
    TLS_PROT               = 8,
    RADIUS_CLIENT_PROT     = 9,

    IEEE_802_1X_INITIAL_KEY = 10,

    IEEE_802_1X_MKA_KEY        = 11,
    RADIUS_IEEE_802_1X_MKA_KEY = 12,
    IEEE_802_11_4WH_KEY        = 16,
    IEEE_802_11_GKH_KEY        = 17
} kmp_type_e;

typedef enum {
    KMP_RESULT_OK = 0,                    // Successful
    KMP_RESULT_ERR_NO_MEM = -1,           // No memory
    KMP_RESULT_ERR_TX_NO_ACK = -2,        // No TX acknowledge was received
    KMP_RESULT_ERR_TX_UNSPEC = -3         // Other TX reason
} kmp_result_e;

typedef enum {
    KMP_TX_OK = 0,                         // Successful
    KMP_TX_ERR_TX_NO_ACK = -1,             // No acknowledge was received
    KMP_TX_ERR_UNSPEC = -2,                // Other reason
} kmp_tx_status_e;

// On message interface send, do not deallocate pdu buffer
#define MSG_IF_SEND_FLAG_NO_DEALLOC        0x01

typedef void kmp_sec_keys_t;
typedef struct sec_prot_s sec_prot_t;
typedef struct kmp_api_s kmp_api_t;
typedef struct kmp_service_s kmp_service_t;

struct ws_cfg_settings_s;

/**
 * kmp_api_create_request KMP-CREATE.request
 *
 * \param kmp instance
 * \param type protocol type
 * \param addr address
 * \param sec_keys security keys
 *
 */
void kmp_api_create_request(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr, kmp_sec_keys_t *sec_keys);

/**
 * kmp_api_create_confirm KMP-CREATE.confirm
 *
 * \param kmp instance
 * \param result ok or fail
 *
 */
typedef void kmp_api_create_confirm(kmp_api_t *kmp, kmp_result_e result);

/**
 * kmp_api_create_indication KMP-CREATE.indication
 *
 * \param kmp instance
 * \param type protocol type
 * \param addr address
 *
 */
typedef void kmp_api_create_indication(kmp_api_t *kmp, kmp_type_e type, kmp_addr_t *addr);

/**
 * kmp_api_create_response KMP-CREATE.response
 *
 * \param kmp instance
 * \param result ok or fail
 *
 */
void kmp_api_create_response(kmp_api_t *kmp, kmp_result_e result);

/**
 * kmp_api_finished_indication KMP-FINISHED.indication
 *
 * \param kmp instance
 * \param result ok or fail
 * \param sec_keys security keys
 *
 * \return TRUE authentication has been completed, FALSE authentication continues
 *
 */
typedef bool kmp_api_finished_indication(kmp_api_t *kmp, kmp_result_e result, kmp_sec_keys_t *sec_keys);

/**
 * kmp_api_finished will be called when KMP has finished and is ready for delete
 *
 * \param kmp instance
 *
 */
typedef void kmp_api_finished(kmp_api_t *kmp);

/**
 * kmp_api_create create KMP api
 *
 * \param service KMP service
 * \param type KMP type
 * \param msg_if_instance_id message interface instance identifier
 * \param sec_cfg security configuration
 *
 * \return KMP instance or NULL
 *
 */
kmp_api_t *kmp_api_create(kmp_service_t *service, kmp_type_e type, uint8_t msg_if_instance_id, sec_cfg_t *sec_cfg);

/**
 * kmp_api_start start KMP api
 *
 * \param kmp instance
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_api_start(kmp_api_t *kmp);

/**
 * kmp_api_create create KMP api
 *
 * \param kmp instance
 *
 */
void kmp_api_delete(kmp_api_t *kmp);

/**
 * kmp_api_type_get get KMP type
 *
 * \param kmp instance
 *
 * \return kmp_type_e KMP type
 *
 */
kmp_type_e kmp_api_type_get(kmp_api_t *kmp);

/**
 * kmp_api_type_get get receive disabled status
 *
 * \param kmp instance
 *
 * \return true/false true when receiving has been disabled
 *
 */
bool kmp_api_receive_disable(kmp_api_t *kmp);

/**
 * kmp_api_receive_check check if received message is for this KMP
 *
 * \param kmp instance
 * \param pdu pdu
 * \param size pdu size
 *
 * \return true/false true if message is for this KMP
 *
 */
bool kmp_api_receive_check(kmp_api_t *kmp, const void *pdu, uint16_t size);

/**
 * kmp_api_type_from_id_get get KMP type from KMP id
 *
 * \param kmp_id KMP identifier
 *
 * \return kmp_type_e KMP type
 *
 */
kmp_type_e kmp_api_type_from_id_get(uint8_t kmp_id);

/**
 * kmp_api_service_get get KMP service for KMP instance
 *
 * \param kmp instance
 *
 * \return service
 *
 */
kmp_service_t *kmp_api_service_get(kmp_api_t *kmp);

/**
 * kmp_api_data_set set application data
 *
 * \param kmp instance
 * \param data data
 *
 */
void kmp_api_data_set(kmp_api_t *kmp, void *data);

/**
 * kmp_api_data_get read application data
 *
 * \param kmp instance
 *
 * \return data
 *
 */
void *kmp_api_data_get(kmp_api_t *kmp);

/**
 * kmp_api_id_get get KMP instance identifier
 *
 * \param kmp instance
 *
 * \return instance identifier
 *
 */
uint8_t kmp_api_instance_id_get(kmp_api_t *kmp);

/**
 * kmp_api_addr_set set address
 *
 * \param kmp instance
 * \param addr addr
 *
 */
void kmp_api_addr_set(kmp_api_t *kmp, kmp_addr_t *addr);

/**
 * kmp_api_sec_keys_set set security keys
 *
 * \param kmp instance
 * \param sec_keys security keys
 *
 */
void kmp_api_sec_keys_set(kmp_api_t *kmp, kmp_sec_keys_t *sec_keys);

/**
 * kmp_api_cb_register registers api callbacks
 *
 * \param kmp instance
 * \param create_conf KMP-CREATE.confirm callback
 * \param create_ind KMP-CREATE.indication callback
 * \param finished_ind KMP-FINISHED.indication
 * \param finish KMP has finished and is ready for delete
 *
 */
void kmp_api_cb_register(kmp_api_t *kmp, kmp_api_create_confirm *create_conf, kmp_api_create_indication *create_ind, kmp_api_finished_indication *finished_ind, kmp_api_finished *finished);

/**
 * kmp_service_create creates KMP service
 *
 * \return service or NULL
 *
 */
kmp_service_t *kmp_service_create(void);

/**
 * kmp_service_delete deletes KMP service
 *
 * \param service KMP service
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_delete(kmp_service_t *service);

/**
 * kmp_service_incoming_ind Notifies application about incoming KMP frame
 *
 * \param service KMP service
 * \param instance_id instance identifier
 * \param type protocol type
 * \param addr address
 *
 * \return KMP instance or NULL
 *
 */
typedef kmp_api_t *kmp_service_incoming_ind(kmp_service_t *service, uint8_t instance_id, kmp_type_e type, const kmp_addr_t *addr, const void *pdu, uint16_t size);

/**
 * kmp_service_tx_status_ind Notifies application about TX status
 *
 * \param service KMP service
 * \param instance_id KMP instance identifier
 *
 * \return KMP instance or NULL
 *
 */
typedef kmp_api_t *kmp_service_tx_status_ind(kmp_service_t *service, uint8_t instance_id);

/**
 * kmp_service_addr_get gets addressing information related to KMP
 *
 * \param service KMP service
 * \param kmp KMP instance
 * \param local_addr Local address
 * \param remote_addr Remote address
 *
 */
typedef void kmp_service_addr_get(kmp_service_t *service, kmp_api_t *kmp, kmp_addr_t *local_addr, kmp_addr_t *remote_addr);

/**
 * kmp_service_ip_addr_get gets IP addressing information related to KMP
 *
 * \param service KMP service
 * \param kmp KMP instance
 * \param address IP address
 *
 */
typedef void kmp_service_ip_addr_get(kmp_service_t *service, kmp_api_t *kmp, uint8_t *address);

/**
 * kmp_service_api_get gets KMP API from KMP service
 *
 * \param service KMP service
 * \param kmp KMP instance
 * \param type protocol type
 *
 * \return KMP instance or NULL
 *
 */
typedef kmp_api_t *kmp_service_api_get(kmp_service_t *service, kmp_api_t *kmp, kmp_type_e type);

/**
 * kmp_service_cb_register registers service callbacks
 *
 * \param service KMP service
 * \param incoming_ind incoming message callback
 * \param tx_status tx status callback
 * \param addr_get gets addressing information callback
 * \param ip_addr_get gets IP addressing information callback
 * \param api_get gets KMP API from KMP service
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_cb_register(kmp_service_t *service, kmp_service_incoming_ind *incoming_ind, kmp_service_tx_status_ind *tx_status_ind, kmp_service_addr_get *addr_get, kmp_service_ip_addr_get *ip_addr_get, kmp_service_api_get *api_get);

/**
 * kmp_service_msg_if_receive receive a message
 *
 * \param service KMP service
 * \param instance_id instance identifier
 * \param type protocol type
 * \param addr address
 * \param pdu pdu
 * \param size pdu size
 * \param conn_number connection number (0 for default)
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_msg_if_receive(kmp_service_t *service, uint8_t instance_id, kmp_type_e kmp_id, const kmp_addr_t *addr, void *pdu, uint16_t size, uint8_t conn_number);

/**
 * kmp_service_msg_if_send send a message
 *
 * \param service KMP service
 * \param instance_id instance identifier
 * \param type protocol type
 * \param addr address
 * \param pdu pdu
 * \param size pdu size
 * \param tx_identifier TX identifier
 * \param conn_number connection number (0 for default)
 * \param flags flags
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_msg_if_send(kmp_service_t *service, uint8_t instance_id, kmp_type_e type, const kmp_addr_t *addr, void *pdu, uint16_t size, uint8_t tx_identifier, uint8_t conn_number, uint8_t flags);

/**
 * kmp_service_msg_if_register registers message interface
 *
 * \param service KMP service
 * \param instance_id message interface instance identifier
 * \param send KMP PDU send callback
 * \param header_size header size
 * \param number_of_conn number of connections
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_msg_if_register(kmp_service_t *service, uint8_t instance_id, kmp_service_msg_if_send *send, uint8_t header_size, uint8_t number_of_conn);

/**
 * kmp_service_tx_status tx status indication
 *
 * \param service KMP service
 * \param tx_status tx status
 * \param tx_identifier tx identifier
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_tx_status_indication(kmp_service_t *service, kmp_tx_status_e tx_status, uint8_t tx_identifier);

/**
 * kmp_sec_prot_size security protocol data size
 *
 * \return size
 *
 */
typedef uint16_t kmp_sec_prot_size(void);

/**
 * kmp_sec_prot_init security protocol init callback
 *
 * \param prot protocol data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_sec_prot_init(sec_prot_t *prot);

/**
 * kmp_service_sec_protocol_register register a security protocol to KMP service
 *
 * \param service KMP service
 * \param type protocol type
 * \param size size callback
 * \param init init callback
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_sec_protocol_register(kmp_service_t *service, kmp_type_e type, kmp_sec_prot_size *size, kmp_sec_prot_init *init);

/**
 * kmp_service_sec_protocol_unregister unregister a security protocol from KMP service
 *
 * \param service KMP service
 * \param type protocol type
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_sec_protocol_unregister(kmp_service_t *service, kmp_type_e type);

/**
 * kmp_service_timer_if_timeout timer timeout
 *
 * \param service KMP instance
 * \param ticks Ticks
 *
 */
void kmp_service_timer_if_timeout(kmp_api_t *kmp, uint16_t ticks);

/**
 * kmp_service_timer_if_start timer start callback
 *
 * \param service KMP service
 * \param kmp KMP instance
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_timer_if_start(kmp_service_t *service, kmp_api_t *kmp);

/**
 * kmp_service_timer_if_stop timer stop callback
 *
 * \param service KMP service
 * \param kmp KMP instance
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_timer_if_stop(kmp_service_t *service, kmp_api_t *kmp);

/**
 * kmp_service_timer_if_register register a timer interface to KMP service
 *
 * \param service KMP service
 * \param start timer start callback
 * \param stop timer stop callback
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_timer_if_register(kmp_service_t *service, kmp_service_timer_if_start start, kmp_service_timer_if_stop stop);

/**
 * kmp_service_shared_comp_timer_timeout shared component timer timeout
 *
 * \param ticks timer ticks
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_shared_comp_timer_timeout(uint16_t ticks);

/**
 * kmp_service_shared_comp_delete shared component delete
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_shared_comp_delete(void);

typedef struct {
    kmp_service_shared_comp_timer_timeout *timeout;
    kmp_service_shared_comp_delete *delete;
} kmp_shared_comp_t;

/**
 * kmp_service_shared_comp_add add shared component
 *
 * \param service KMP service
 * \param data shared component data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_shared_comp_add(kmp_service_t *service, kmp_shared_comp_t *data);

/**
 * kmp_service_shared_comp_remove remove shared component
 *
 * \param service KMP service
 * \param data shared component data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_shared_comp_remove(kmp_service_t *service, kmp_shared_comp_t *data);

/**
 * kmp_service_shared_comp_if_register register a continuous timer interface to KMP service
 *
 * \param service KMP service
 * \param add  shared component
 * \param remove remove shared component
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_shared_comp_if_register(kmp_service_t *service, kmp_service_shared_comp_add add, kmp_service_shared_comp_remove remove);

/**
 * kmp_service_event_if_event event callback
 *
 * \param service KMP service
 * \param data opaque callback data
 *
 */
void kmp_service_event_if_event(kmp_service_t *service, void *data);

/**
 * kmp_service_event_if_event_send event send callback
 *
 * \param service KMP service
 * \param data opaque callback data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t kmp_service_event_if_event_send(kmp_service_t *service, void *data);

/**
 * kmp_service_event_if_register register an event interface to KMP service
 *
 * \param service KMP service
 * \param send send event
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_service_event_if_register(kmp_service_t *service, kmp_service_event_if_event_send send);

#endif /* KMP_API_H_ */
