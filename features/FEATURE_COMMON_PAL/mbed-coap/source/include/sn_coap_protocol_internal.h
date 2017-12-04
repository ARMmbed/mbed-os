/*
 * Copyright (c) 2011-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file sn_coap_protocol_internal.h
 *
 * \brief Header file for CoAP Protocol part
 *
 */

#ifndef SN_COAP_PROTOCOL_INTERNAL_H_
#define SN_COAP_PROTOCOL_INTERNAL_H_

#include "ns_list.h"
#include "sn_coap_header_internal.h"
#include "mbed-coap/sn_config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sn_coap_hdr_;

/* * * * * * * * * * * */
/* * * * DEFINES * * * */
/* * * * * * * * * * * */

/* * For Message resending * */
#define ENABLE_RESENDINGS                               1   /**< Enable / Disable resending from library in building */

#define SN_COAP_RESENDING_MAX_COUNT                     3   /**< Default number of re-sendings  */

#ifdef YOTTA_CFG_COAP_RESENDING_QUEUE_SIZE_MSGS
#define SN_COAP_RESENDING_QUEUE_SIZE_MSGS YOTTA_CFG_COAP_RESENDING_QUEUE_SIZE_MSGS
#elif defined MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_MSGS
#define SN_COAP_RESENDING_QUEUE_SIZE_MSGS MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_MSGS
#endif

#ifndef SN_COAP_RESENDING_QUEUE_SIZE_MSGS
#define SN_COAP_RESENDING_QUEUE_SIZE_MSGS               2   /**< Default re-sending queue size - defines how many messages can be stored. Setting this to 0 disables feature */
#endif

#ifdef YOTTA_CFG_COAP_RESENDING_QUEUE_SIZE_BYTES
#define SN_COAP_RESENDING_QUEUE_SIZE_BYTES YOTTA_CFG_COAP_RESENDING_QUEUE_SIZE_BYTES
#elif defined MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_BYTES
#define SN_COAP_RESENDING_QUEUE_SIZE_BYTES MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_BYTES
#endif

#ifndef SN_COAP_RESENDING_QUEUE_SIZE_BYTES
#define SN_COAP_RESENDING_QUEUE_SIZE_BYTES              0   /**< Default re-sending queue size - defines size of the re-sending buffer. Setting this to 0 disables feature */
#endif

#define DEFAULT_RESPONSE_TIMEOUT                        10  /**< Default re-sending timeout as seconds */

/* These parameters sets maximum values application can set with API */
#define SN_COAP_MAX_ALLOWED_RESENDING_COUNT             6   /**< Maximum allowed count of re-sending */
#define SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_MSGS    6   /**< Maximum allowed number of saved re-sending messages */
#define SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_BYTES   512 /**< Maximum allowed size of re-sending buffer */
#define SN_COAP_MAX_ALLOWED_RESPONSE_TIMEOUT            40  /**< Maximum allowed re-sending timeout */

#define RESPONSE_RANDOM_FACTOR                          1.5   /**< Resending random factor, value is specified in IETF CoAP specification */

/* * For Message duplication detecting * */

/* Init value for the maximum count of messages to be stored for duplication detection          */
/* Setting of this value to 0 will disable duplication check, also reduce use of ROM memory     */

// Keep the old flag to maintain backward compatibility
#ifndef SN_COAP_DUPLICATION_MAX_MSGS_COUNT
#define SN_COAP_DUPLICATION_MAX_MSGS_COUNT              0
#endif

#ifdef YOTTA_CFG_COAP_DUPLICATION_MAX_MSGS_COUNT
#define SN_COAP_DUPLICATION_MAX_MSGS_COUNT YOTTA_CFG_COAP_DUPLICATION_MAX_MSGS_COUNT
#elif defined MBED_CONF_MBED_CLIENT_SN_COAP_DUPLICATION_MAX_MSGS_COUNT
#define SN_COAP_DUPLICATION_MAX_MSGS_COUNT MBED_CONF_MBED_CLIENT_SN_COAP_DUPLICATION_MAX_MSGS_COUNT
#endif



/* Maximum allowed number of saved messages for duplicate searching */
#define SN_COAP_MAX_ALLOWED_DUPLICATION_MESSAGE_COUNT   6

/* Maximum time in seconds of messages to be stored for duplication detection */
#define SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED    60 /* RESPONSE_TIMEOUT * RESPONSE_RANDOM_FACTOR * (2 ^ MAX_RETRANSMIT - 1) + the expected maximum round trip time */

/* * For Message blockwising * */

/* Init value for the maximum payload size to be sent and received at one blockwise message                         */
/* Setting of this value to 0 will disable this feature, and also reduce use of ROM memory                          */
/* Note: This define is common for both received and sent Blockwise messages                                        */

#ifdef YOTTA_CFG_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#define SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE YOTTA_CFG_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#elif defined MBED_CONF_MBED_CLIENT_SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#define SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE MBED_CONF_MBED_CLIENT_SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#endif

#ifndef SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#define SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE          0  /**< Must be 2^x and x is at least 4. Suitable values: 0, 16, 32, 64, 128, 256, 512 and 1024 */
#endif

#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED
#define SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED MBED_CONF_MBED_CLIENT_SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED
#endif

#ifndef SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED
#define SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED      60 /**< Maximum time in seconds of data (messages and payload) to be stored for blockwising */
#endif

#ifdef YOTTA_CFG_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE
#define SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE YOTTA_CFG_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE
#elif defined MBED_CONF_MBED_CLIENT_SN_COAP_MAX_INCOMING_MESSAGE_SIZE
#define SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE MBED_CONF_MBED_CLIENT_SN_COAP_MAX_INCOMING_MESSAGE_SIZE
#endif

#ifndef SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE
#define SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE UINT16_MAX
#endif

/* * For Option handling * */
#define COAP_OPTION_MAX_AGE_DEFAULT                 60 /**< Default value of Max-Age if option not present */
#define COAP_OPTION_URI_PORT_NONE                   (-1) /**< Internal value to represent no Uri-Port option */
#define COAP_OPTION_BLOCK_NONE                      (-1) /**< Internal value to represent no Block1/2 option */


#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */
int8_t prepare_blockwise_message(struct coap_s *handle, struct sn_coap_hdr_ *coap_hdr_ptr);
#endif

/* Structure which is stored to Linked list for message sending purposes */
typedef struct coap_send_msg_ {
    uint8_t             resending_counter;  /* Tells how many times message is still tried to resend */
    uint32_t            resending_time;     /* Tells next resending time */

    sn_nsdl_transmit_s *send_msg_ptr;

    struct coap_s       *coap;              /* CoAP library handle */
    void                *param;             /* Extra parameter that will be passed to TX/RX callback functions */

    ns_list_link_t      link;
} coap_send_msg_s;

typedef NS_LIST_HEAD(coap_send_msg_s, link) coap_send_msg_list_t;

/* Structure which is stored to Linked list for message duplication detection purposes */
typedef struct coap_duplication_info_ {
    uint32_t            timestamp; /* Tells when duplication information is stored to Linked list */
    uint16_t            msg_id;
    uint16_t            packet_len;
    uint8_t             *packet_ptr;
    struct coap_s       *coap;  /* CoAP library handle */
    sn_nsdl_addr_s      *address;
    void                *param;
    ns_list_link_t      link;
} coap_duplication_info_s;

typedef NS_LIST_HEAD(coap_duplication_info_s, link) coap_duplication_info_list_t;

/* Structure which is stored to Linked list for blockwise messages sending purposes */
typedef struct coap_blockwise_msg_ {
    uint32_t            timestamp;  /* Tells when Blockwise message is stored to Linked list */

    sn_coap_hdr_s       *coap_msg_ptr;
    struct coap_s       *coap;      /* CoAP library handle */

    ns_list_link_t     link;
} coap_blockwise_msg_s;

typedef NS_LIST_HEAD(coap_blockwise_msg_s, link) coap_blockwise_msg_list_t;

/* Structure which is stored to Linked list for blockwise messages receiving purposes */
typedef struct coap_blockwise_payload_ {
    uint32_t            timestamp; /* Tells when Payload is stored to Linked list */

    uint8_t             addr_len;
    uint8_t             *addr_ptr;
    uint16_t            port;
    uint32_t            block_number;

    uint16_t            payload_len;
    uint8_t             *payload_ptr;
    struct coap_s       *coap;  /* CoAP library handle */

    ns_list_link_t     link;
} coap_blockwise_payload_s;

typedef NS_LIST_HEAD(coap_blockwise_payload_s, link) coap_blockwise_payload_list_t;

struct coap_s {
    void *(*sn_coap_protocol_malloc)(uint16_t);
    void (*sn_coap_protocol_free)(void *);

    uint8_t (*sn_coap_tx_callback)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *);
    int8_t (*sn_coap_rx_callback)(sn_coap_hdr_s *, sn_nsdl_addr_s *, void *);

    #if ENABLE_RESENDINGS /* If Message resending is not used at all, this part of code will not be compiled */
        coap_send_msg_list_t linked_list_resent_msgs; /* Active resending messages are stored to this Linked list */
        uint16_t count_resent_msgs;
    #endif

    #if SN_COAP_DUPLICATION_MAX_MSGS_COUNT /* If Message duplication detection is not used at all, this part of code will not be compiled */
        coap_duplication_info_list_t  linked_list_duplication_msgs; /* Messages for duplicated messages detection is stored to this Linked list */
        uint16_t                      count_duplication_msgs;
    #endif

    #if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwise is not used at all, this part of code will not be compiled */
        coap_blockwise_msg_list_t     linked_list_blockwise_sent_msgs; /* Blockwise message to to be sent is stored to this Linked list */
        coap_blockwise_payload_list_t linked_list_blockwise_received_payloads; /* Blockwise payload to to be received is stored to this Linked list */
    #endif

    uint32_t system_time;    /* System time seconds */
    uint16_t sn_coap_block_data_size;
    uint8_t sn_coap_resending_queue_msgs;
    uint32_t sn_coap_resending_queue_bytes;
    uint8_t sn_coap_resending_count;
    uint8_t sn_coap_resending_intervall;
    uint8_t sn_coap_duplication_buffer_size;
};

#ifdef __cplusplus
}
#endif

#endif /* SN_COAP_PROTOCOL_INTERNAL_H_ */

