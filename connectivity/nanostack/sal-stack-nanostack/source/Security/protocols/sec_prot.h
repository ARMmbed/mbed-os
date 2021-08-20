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

#ifndef SEC_PROT_H_
#define SEC_PROT_H_

/*
 * Interface between KMP API and key management security protocols. Interface
 * provides abstraction for different security protocols for KMP API module.
 *
 * For security protocols it provides access to network, timing, callback
 * security keys and network address services.
 *
 */

typedef enum {
    SEC_RESULT_OK = 0,
    SEC_RESULT_ERR_NO_MEM = -1,
    SEC_RESULT_ERR_TX_NO_ACK = -2,
    SEC_RESULT_ERR_TX_UNSPEC = -3,
    SEC_RESULT_TIMEOUT = -4,
    SEC_RESULT_ERROR = -5,
    SEC_RESULT_CONF_ERROR = -6,
    SEC_RESULT_IGNORE
} sec_prot_result_e;

typedef enum {
    SEC_STATE_INIT = 0,
    SEC_STATE_CREATE_REQ,
    SEC_STATE_CREATE_RESP,
    SEC_STATE_CREATE_IND,
    SEC_STATE_FINISH,
    SEC_STATE_FINISHED,
    SEC_STATE_FIRST
} sec_prot_state_e;

typedef enum {
    SEC_PROT_TYPE_EAP_TLS = 0,
    SEC_PROT_TYPE_RADIUS_EAP_TLS,
    SEC_PROT_TYPE_TLS,
    SEC_PROT_TYPE_RADIUS_CLIENT
} sec_prot_type_e;

typedef enum {
    SEC_PROT_TX_OK = 0,                       // Successful
    SEC_PROT_TX_ERR_TX_NO_ACK = -1,           // No acknowledge was received
    SEC_PROT_TX_ERR_UNSPEC = -2,              // Other reason
} sec_prot_tx_status_e;

// On security protocol send, do not deallocate pdu buffer
#define SEC_PROT_SEND_FLAG_NO_DEALLOC         0x01

/**
 * sec_prot_create_request KMP-CREATE.request to security protocol
 *
 * \param prot protocol
 * \param sec_keys security keys
 *
 */
typedef void sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys);

/**
 * sec_prot_create_response KMP-CREATE.response from security protocol
 *
 * \param prot protocol
 * \param result result
 *
 */
typedef void sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result);

/**
 * sec_prot_create_confirm KMP-CREATE.confirm from security protocol
 *
 * \param prot protocol
 * \param result result
 *
 */
typedef void sec_prot_create_confirm(sec_prot_t *prot, sec_prot_result_e result);

/**
 * sec_prot_create_indication KMP-CREATE.indication from security protocol
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_create_indication(sec_prot_t *prot);

/**
 * sec_prot_finished_indication KMP-FINISHED.indication from security protocol
 *
 * \param prot protocol
 * \param result result
 * \param sec_keys security keys
 *
 * \return TRUE authentication has been completed, FALSE authentication continues
 *
 */
typedef bool sec_prot_finished_indication(sec_prot_t *prot, sec_prot_result_e result, sec_prot_keys_t *sec_keys);

/**
 * sec_prot_finished Security protocol has finished and is ready for delete
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_finished(sec_prot_t *prot);

/**
 * sec_prot_finished_send Security protocol finished send
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_finished_send(sec_prot_t *prot);

/**
 * sec_prot_receive receive a message
 *
 * \param prot protocol
 * \param pdu pdu
 * \param size pdu size
 * \param conn_number connection number
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);

/**
 * sec_prot_send send a message
 *
 * \param prot protocol
 * \param pdu pdu
 * \param size pdu size
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_send(sec_prot_t *prot, void *pdu, uint16_t size);

/**
 * sec_prot_conn_receive receive a message for a connection number
 *
 * \param prot protocol
 * \param pdu pdu
 * \param size pdu size
 * \param conn_number connection number
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_conn_receive(sec_prot_t *prot, void *pdu, uint16_t size, uint8_t conn_number);

/**
 * sec_prot_conn_send send a message for a connection number and/or with flags
 *
 * \param prot protocol
 * \param pdu pdu
 * \param size pdu size
 * \param conn_number connection number
 * \param flags flags
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_conn_send(sec_prot_t *prot, void *pdu, uint16_t size, uint8_t conn_number, uint8_t flags);

/**
 * sec_prot_tx_status_ind tx status indication
 *
 * \param prot protocol
 * \param tx_status tx status
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_tx_status_ind(sec_prot_t *prot, sec_prot_tx_status_e tx_status);

/**
 * sec_prot_delete delete the protocol data
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_delete(sec_prot_t *prot);

/**
 * sec_prot_state_machine protocol state machine
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_state_machine(sec_prot_t *prot);

/**
 * sec_prot_state_machine_call call protocol state machine
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_state_machine_call(sec_prot_t *prot);

/**
 * sec_prot_cont_timer_timeout cont timer timeout
 *
 * \param ticks timer ticks
 *
 */
typedef void sec_prot_cont_timer_timeout(uint16_t ticks);

/**
 * sec_prot_timer_start start timer
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_timer_start(sec_prot_t *prot);

/**
 * sec_prot_timer_stop stop timer
 *
 * \param prot protocol
 *
 */
typedef void sec_prot_timer_stop(sec_prot_t *prot);

/**
 * sec_prot_timer_timeout timer timeout
 *
 * \param prot protocol
 * \param ticks timer ticks
 *
 */
typedef void sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);

/**
 * sec_prot_shared_comp_timer_timeout shared component timer timeout
 *
 * \param ticks timer ticks
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_shared_comp_timer_timeout(uint16_t ticks);

/**
 * sec_prot_shared_comp_delete shared component delete
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_shared_comp_delete(void);

typedef struct {
    sec_prot_shared_comp_timer_timeout *timeout;
    sec_prot_shared_comp_delete *delete;
} shared_comp_data_t;

/**
 * sec_prot_shared_comp_add add shared component
 *
 * \param prot protocol
 * \param data shared component data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_shared_comp_add(sec_prot_t *prot, shared_comp_data_t *data);

/**
 * sec_prot_shared_comp_remove remove shared component
 *
 * \param prot protocol
 * \param data shared component data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t sec_prot_shared_comp_remove(sec_prot_t *prot, shared_comp_data_t *data);

/**
 * sec_prot_eui64_addr_get gets EUI-64 addresses
 *
 * \param prot protocol
 * \param local_eui64 local EUI-64
 * \param remote_eui64 remote EUI-64
 *
 */
typedef void sec_prot_eui64_addr_get(sec_prot_t *prot, uint8_t *local_eui64, uint8_t *remote_eui64);

/**
 * sec_prot_ip_addr_get gets IP address for security protocol
 *
 * \param prot protocol
 * \param address IP address
 *
 */
typedef void sec_prot_ip_addr_get(sec_prot_t *prot, uint8_t *address);

/**
 * sec_prot_by_type_get gets security protocol
 *
 * \param prot protocol
 * \param type security protocol type
 *
 * \return security protocol or NULL
 *
 */
typedef sec_prot_t *sec_prot_by_type_get(sec_prot_t *prot, uint8_t type);

/**
 * sec_prot_receive_disable disables receiving of messages
 *
 * \param prot protocol
 *
 * \return security protocol or NULL
 *
 */
typedef void sec_prot_receive_disable(sec_prot_t *prot);

/**
 * sec_prot_receive_check check if received message is for this protocol
 *
 * \param prot protocol
 * \param pdu pdu
 * \param size pdu size
 *
 * \return < 0 message is not for this protocol
 * \return >= 0 message is for this protocol
 *
 */
typedef int8_t sec_prot_receive_check(sec_prot_t *prot, const void *pdu, uint16_t size);

typedef struct sec_prot_int_data_s sec_prot_int_data_t;

// Security protocol data
struct sec_prot_s {
    sec_prot_create_request       *create_req;           /**< Create request */
    sec_prot_create_response      *create_resp;          /**< Create response */

    sec_prot_create_confirm       *create_conf;          /**< Create confirm */
    sec_prot_create_indication    *create_ind;           /**< Create indication */
    sec_prot_finished_indication  *finished_ind;         /**< Finished indication */
    sec_prot_finished             *finished;             /**< Finished i.e. ready to be deleted */
    sec_prot_finished_send        *finished_send;        /**< Send finished */

    sec_prot_send                 *send;                 /**< Protocol send */
    sec_prot_receive              *receive;              /**< Protocol receive */
    sec_prot_conn_send            *conn_send;            /**< Protocol connection send */
    sec_prot_conn_receive         *conn_receive;         /**< Protocol connection receive */
    sec_prot_receive              *receive_peer;         /**< Protocol receive from peer (used by peer protocol for send) */
    sec_prot_delete               *peer_deleted;         /**< Protocol peer has been deleted (notifies that peer no longer exists */

    sec_prot_tx_status_ind        *tx_status_ind;        /**< TX status indication */

    sec_prot_delete               *delete;               /**< Protocol delete */

    sec_prot_state_machine_call   *state_machine_call;   /**< Call state machine */
    sec_prot_state_machine        *state_machine;        /**< Protocol state machine */

    sec_prot_timer_start          *timer_start;          /**< Start timer */
    sec_prot_timer_stop           *timer_stop;           /**< Stop timer */
    sec_prot_timer_timeout        *timer_timeout;        /**< Timer timeout */

    sec_prot_shared_comp_add      *shared_comp_add;      /**< Shared component add */
    sec_prot_shared_comp_remove   *shared_comp_remove;   /**< Shared component remove */

    sec_prot_eui64_addr_get       *addr_get;             /**< Gets EUI-64 addresses */
    sec_prot_ip_addr_get          *ip_addr_get;          /**< Gets IP address */
    sec_prot_by_type_get          *type_get;             /**< Gets security protocol by type */
    sec_prot_receive_disable      *receive_disable;      /**< Disable receiving of messages */
    sec_prot_receive_check        *receive_check;        /**< Check if messages is for this protocol */

    sec_prot_keys_t               *sec_keys;             /**< Security keys storage pointer */
    sec_cfg_t                     *sec_cfg;              /**< Security configuration configuration pointer */
    uint8_t                       header_size;           /**< Header size */
    uint8_t                       receive_peer_hdr_size; /**< Receive from peer header size */
    uint8_t                       number_of_conn;        /**< Number of connections */
    uint8_t                       msg_if_instance_id;    /**< Message interface instance identifier */
    sec_prot_int_data_t           *data;                 /**< Protocol internal data */
};

#endif /* SEC_PROT_H_ */
