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
 * \file sn_coap_header.h
 *
 * \brief CoAP C-library User header interface header file
 */

#ifndef SN_COAP_HEADER_H_
#define SN_COAP_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Handle structure */
struct coap_s;

/* * * * * * * * * * * * * * */
/* * * * ENUMERATIONS  * * * */
/* * * * * * * * * * * * * * */

/**
 * \brief Enumeration for CoAP Version
 */
typedef enum coap_version_ {
    COAP_VERSION_1          = 0x40,
    COAP_VERSION_UNKNOWN    = 0xFF
} coap_version_e;

/**
 * \brief Enumeration for CoAP Message type, used in CoAP Header
 */
typedef enum sn_coap_msg_type_ {
    COAP_MSG_TYPE_CONFIRMABLE       = 0x00, /**< Reliable Request messages */
    COAP_MSG_TYPE_NON_CONFIRMABLE   = 0x10, /**< Non-reliable Request and Response messages */
    COAP_MSG_TYPE_ACKNOWLEDGEMENT   = 0x20, /**< Response to a Confirmable Request  */
    COAP_MSG_TYPE_RESET             = 0x30  /**< Answer a Bad Request */
} sn_coap_msg_type_e;

/**
 * \brief Enumeration for CoAP Message code, used in CoAP Header
 */
typedef enum sn_coap_msg_code_ {
    COAP_MSG_CODE_EMPTY                                 = 0,
    COAP_MSG_CODE_REQUEST_GET                           = 1,
    COAP_MSG_CODE_REQUEST_POST                          = 2,
    COAP_MSG_CODE_REQUEST_PUT                           = 3,
    COAP_MSG_CODE_REQUEST_DELETE                        = 4,

    COAP_MSG_CODE_RESPONSE_CREATED                      = 65,
    COAP_MSG_CODE_RESPONSE_DELETED                      = 66,
    COAP_MSG_CODE_RESPONSE_VALID                        = 67,
    COAP_MSG_CODE_RESPONSE_CHANGED                      = 68,
    COAP_MSG_CODE_RESPONSE_CONTENT                      = 69,
    COAP_MSG_CODE_RESPONSE_CONTINUE                     = 95,
    COAP_MSG_CODE_RESPONSE_BAD_REQUEST                  = 128,
    COAP_MSG_CODE_RESPONSE_UNAUTHORIZED                 = 129,
    COAP_MSG_CODE_RESPONSE_BAD_OPTION                   = 130,
    COAP_MSG_CODE_RESPONSE_FORBIDDEN                    = 131,
    COAP_MSG_CODE_RESPONSE_NOT_FOUND                    = 132,
    COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED           = 133,
    COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE               = 134,
    COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE    = 136,
    COAP_MSG_CODE_RESPONSE_PRECONDITION_FAILED          = 140,
    COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE     = 141,
    COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT   = 143,
    COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR        = 160,
    COAP_MSG_CODE_RESPONSE_NOT_IMPLEMENTED              = 161,
    COAP_MSG_CODE_RESPONSE_BAD_GATEWAY                  = 162,
    COAP_MSG_CODE_RESPONSE_SERVICE_UNAVAILABLE          = 163,
    COAP_MSG_CODE_RESPONSE_GATEWAY_TIMEOUT              = 164,
    COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED       = 165
} sn_coap_msg_code_e;

/**
 * \brief Enumeration for CoAP Option number, used in CoAP Header
 */
typedef enum sn_coap_option_numbers_ {
    COAP_OPTION_IF_MATCH        = 1,
    COAP_OPTION_URI_HOST        = 3,
    COAP_OPTION_ETAG            = 4,
    COAP_OPTION_IF_NONE_MATCH   = 5,
    COAP_OPTION_OBSERVE         = 6,
    COAP_OPTION_URI_PORT        = 7,
    COAP_OPTION_LOCATION_PATH   = 8,
    COAP_OPTION_URI_PATH        = 11,
    COAP_OPTION_CONTENT_FORMAT  = 12,
    COAP_OPTION_MAX_AGE         = 14,
    COAP_OPTION_URI_QUERY       = 15,
    COAP_OPTION_ACCEPT          = 17,
    COAP_OPTION_LOCATION_QUERY  = 20,
    COAP_OPTION_BLOCK2          = 23,
    COAP_OPTION_BLOCK1          = 27,
    COAP_OPTION_SIZE2           = 28,
    COAP_OPTION_PROXY_URI       = 35,
    COAP_OPTION_PROXY_SCHEME    = 39,
    COAP_OPTION_SIZE1           = 60
//  128 =   (Reserved)
//  132 =   (Reserved)
//  136 =   (Reserved)
} sn_coap_option_numbers_e;

/**
 * \brief Enumeration for CoAP Content Format codes
 */
typedef enum sn_coap_content_format_ {
    COAP_CT_NONE                = -1, // internal
    COAP_CT_TEXT_PLAIN          = 0,
    COAP_CT_LINK_FORMAT         = 40,
    COAP_CT_XML                 = 41,
    COAP_CT_OCTET_STREAM        = 42,
    COAP_CT_EXI                 = 47,
    COAP_CT_JSON                = 50,
    COAP_CT__MAX                = 0xffff
} sn_coap_content_format_e;

/**
 * \brief Enumeration for CoAP Observe option values
 *
 * draft-ietf-core-observe-16
 */
typedef enum sn_coap_observe_ {
    COAP_OBSERVE_NONE           = -1, // internal

    // Values for GET requests
    COAP_OBSERVE_REGISTER       = 0,
    COAP_OBSERVE_DEREGISTER     = 1,

    // In responses, value is a 24-bit opaque sequence number
    COAP_OBSERVE__MAX   = 0xffffff
} sn_coap_observe_e;

/**
 * \brief Enumeration for CoAP status, used in CoAP Header
 */
typedef enum sn_coap_status_ {
    COAP_STATUS_OK                             = 0, /**< Default value is OK */
    COAP_STATUS_PARSER_ERROR_IN_HEADER         = 1, /**< CoAP will send Reset message to invalid message sender */
    COAP_STATUS_PARSER_DUPLICATED_MSG          = 2, /**< CoAP will send Acknowledgement message to duplicated message sender */
    COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVING = 3, /**< User will get whole message after all message blocks received.
                                                         User must release messages with this status. */
    COAP_STATUS_PARSER_BLOCKWISE_ACK           = 4, /**< Acknowledgement for sent Blockwise message received */
    COAP_STATUS_PARSER_BLOCKWISE_MSG_REJECTED  = 5, /**< Blockwise message received but not supported by compiling switch */
    COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED  = 6, /**< Blockwise message fully received and returned to app.
                                                         User is responsible of freeing the data by calling sn_coap_protocol_block_remove() */
    COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED = 7, /**< When re-transmissions have been done and ACK not received, CoAP library calls
                                                         RX callback with this status */

    COAP_STATUS_BUILDER_BLOCK_SENDING_FAILED   = 8, /**< Blockwise message sending timeout.
                                                         The msg_id in sn_coap_hdr_s* parameter of RX callback is set to the same value
                                                         as in the first block sent, and parameter sn_nsdl_addr_s* is set as NULL.  */
    COAP_STATUS_BUILDER_BLOCK_SENDING_DONE     = 9  /**< Blockwise message sending, last block sent.
                                                         The msg_id in sn_coap_hdr_s* parameter of RX callback is set to the same value
                                                         as in the first block sent, and parameter sn_nsdl_addr_s* is set as NULL. */

} sn_coap_status_e;


/* * * * * * * * * * * * * */
/* * * * STRUCTURES  * * * */
/* * * * * * * * * * * * * */

/**
 * \brief Structure for CoAP Options
 */
typedef struct sn_coap_options_list_ {
    uint8_t         etag_len;           /**< 1-8 bytes. Repeatable */
    unsigned int    use_size1:1;
    unsigned int    use_size2:1;

    uint16_t    proxy_uri_len;      /**< 1-1034 bytes. */
    uint16_t    uri_host_len;       /**< 1-255 bytes. */
    uint16_t    location_path_len;  /**< 0-255 bytes. Repeatable */
    uint16_t    location_query_len; /**< 0-255 bytes. Repeatable */
    uint16_t    uri_query_len;      /**< 1-255 bytes. Repeatable */

    sn_coap_content_format_e accept; /**< Value 0-65535. COAP_CT_NONE if not used */

    uint32_t    max_age;            /**< Value in seconds (default is 60) */
    uint32_t    size1;              /**< 0-4 bytes. */
    uint32_t    size2;              /**< 0-4 bytes. */
    int32_t     uri_port;           /**< Value 0-65535. -1 if not used */
    int32_t     observe;            /**< Value 0-0xffffff. -1 if not used */
    int32_t     block1;             /**< Value 0-0xffffff. -1 if not used. Not for user */
    int32_t     block2;             /**< Value 0-0xffffff. -1 if not used. Not for user */

    uint8_t    *proxy_uri_ptr;      /**< Must be set to NULL if not used */
    uint8_t    *etag_ptr;           /**< Must be set to NULL if not used */
    uint8_t    *uri_host_ptr;       /**< Must be set to NULL if not used */
    uint8_t    *location_path_ptr;  /**< Must be set to NULL if not used */
    uint8_t    *location_query_ptr; /**< Must be set to NULL if not used */
    uint8_t    *uri_query_ptr;      /**< Must be set to NULL if not used */
} sn_coap_options_list_s;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !!! Main CoAP message struct !!! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \brief Main CoAP message struct
 */
typedef struct sn_coap_hdr_ {
    uint8_t                 token_len;          /**< 1-8 bytes. */

    sn_coap_status_e        coap_status;        /**< Used for telling to User special cases when parsing message */
    sn_coap_msg_code_e      msg_code;           /**< Empty: 0; Requests: 1-31; Responses: 64-191 */

    sn_coap_msg_type_e      msg_type;           /**< Confirmable, Non-Confirmable, Acknowledgement or Reset */
    sn_coap_content_format_e content_format;    /**< Set to COAP_CT_NONE if not used */

    uint16_t                msg_id;             /**< Message ID. Parser sets parsed message ID, builder sets message ID of built coap message */
    uint16_t                uri_path_len;       /**< 0-255 bytes. Repeatable. */
    uint16_t                payload_len;        /**< Must be set to zero if not used */

    uint8_t                *token_ptr;          /**< Must be set to NULL if not used */
    uint8_t                *uri_path_ptr;       /**< Must be set to NULL if not used. E.g: temp1/temp2 */
    uint8_t                *payload_ptr;        /**< Must be set to NULL if not used */

    /* Here are not so often used Options */
    sn_coap_options_list_s *options_list_ptr;   /**< Must be set to NULL if not used */
} sn_coap_hdr_s;

/* * * * * * * * * * * * * * */
/* * * * ENUMERATIONS  * * * */
/* * * * * * * * * * * * * * */


/**
 * \brief Used protocol
 */
typedef enum sn_nsdl_capab_ {
    SN_NSDL_PROTOCOL_HTTP           = 0x01,         /**< Unsupported */
    SN_NSDL_PROTOCOL_HTTPS          = 0x02,         /**< Unsupported */
    SN_NSDL_PROTOCOL_COAP           = 0x04          /**< Supported */
} sn_nsdl_capab_e;

/* * * * * * * * * * * * * */
/* * * * STRUCTURES  * * * */
/* * * * * * * * * * * * * */


/**
 * \brief Used for creating manually registration message with sn_coap_register()
 */
typedef struct registration_info_ {
    uint8_t endpoint_len;
    uint8_t endpoint_type_len;
    uint16_t links_len;

    uint8_t *endpoint_ptr;          /**< Endpoint name */
    uint8_t *endpoint_type_ptr;     /**< Endpoint type */
    uint8_t *links_ptr;             /**< Resource registration string */
} registration_info_t;


/**
 * \brief Address type of given address
 */
typedef enum sn_nsdl_addr_type_ {
    SN_NSDL_ADDRESS_TYPE_IPV6       = 0x01,         /**< Supported */
    SN_NSDL_ADDRESS_TYPE_IPV4       = 0x02,         /**< Supported */
    SN_NSDL_ADDRESS_TYPE_HOSTNAME   = 0x03,         /**< Unsupported */
    SN_NSDL_ADDRESS_TYPE_NONE       = 0xFF
} sn_nsdl_addr_type_e;

/**
 * \brief Address structure of Packet data
 */
typedef struct sn_nsdl_addr_ {
    uint8_t                 addr_len;
    sn_nsdl_addr_type_e     type;
    uint16_t                port;
    uint8_t                 *addr_ptr;
} sn_nsdl_addr_s;


/* * * * * * * * * * * * * * * * * * * * * * */
/* * * * EXTERNAL FUNCTION PROTOTYPES  * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
/**
 * \fn sn_coap_hdr_s *sn_coap_parser(struct coap_s *handle, uint16_t packet_data_len, uint8_t *packet_data_ptr, coap_version_e *coap_version_ptr)
 *
 * \brief Parses CoAP message from given Packet data
 *
 * \param *handle Pointer to CoAP library handle
 *
 * \param packet_data_len is length of given Packet data to be parsed to CoAP message
 *
 * \param *packet_data_ptr is source for Packet data to be parsed to CoAP message
 *
 * \param *coap_version_ptr is destination for parsed CoAP specification version
 *
 * \return Return value is pointer to parsed CoAP message.\n
 *         In following failure cases NULL is returned:\n
 *          -Failure in given pointer (= NULL)\n
 *          -Failure in memory allocation (malloc() returns NULL)
 */
extern sn_coap_hdr_s *sn_coap_parser(struct coap_s *handle, uint16_t packet_data_len, uint8_t *packet_data_ptr, coap_version_e *coap_version_ptr);

/**
 * \fn void sn_coap_parser_release_allocated_coap_msg_mem(struct coap_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr)
 *
 * \brief Releases memory of given CoAP message
 *
 *        Note!!! Does not release Payload part
 *
 * \param *handle Pointer to CoAP library handle
 *
 * \param *freed_coap_msg_ptr is pointer to released CoAP message
 */
extern void sn_coap_parser_release_allocated_coap_msg_mem(struct coap_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr);

/**
 * \fn int16_t sn_coap_builder(uint8_t *dst_packet_data_ptr, sn_coap_hdr_s *src_coap_msg_ptr)
 *
 * \brief Builds an outgoing message buffer from a CoAP header structure.
 *
 * \param *dst_packet_data_ptr is pointer to allocated destination to built CoAP packet
 *
 * \param *src_coap_msg_ptr is pointer to source structure for building Packet data
 *
 * \return Return value is byte count of built Packet data. In failure cases:\n
 *          -1 = Failure in given CoAP header structure\n
 *          -2 = Failure in given pointer (= NULL)
 */
extern int16_t sn_coap_builder(uint8_t *dst_packet_data_ptr, const sn_coap_hdr_s *src_coap_msg_ptr);

/**
 * \fn uint16_t sn_coap_builder_calc_needed_packet_data_size(sn_coap_hdr_s *src_coap_msg_ptr)
 *
 * \brief Calculates needed Packet data memory size for given CoAP message
 *
 * \param *src_coap_msg_ptr is pointer to data which needed Packet
 *          data length is calculated
 *
 * \return Return value is count of needed memory as bytes for build Packet data
 *          Null if failed
 */
extern uint16_t sn_coap_builder_calc_needed_packet_data_size(const sn_coap_hdr_s *src_coap_msg_ptr);

/**
 * \fn int16_t sn_coap_builder_2(uint8_t *dst_packet_data_ptr, sn_coap_hdr_s *src_coap_msg_ptr,  uint16_t blockwise_size)
 *
 * \brief Builds an outgoing message buffer from a CoAP header structure.
 *
 * \param *dst_packet_data_ptr is pointer to allocated destination to built CoAP packet
 *
 * \param *src_coap_msg_ptr is pointer to source structure for building Packet data
 *
 * \param blockwise_payload_size Blockwise message maximum payload size
 *
 * \return Return value is byte count of built Packet data. In failure cases:\n
 *          -1 = Failure in given CoAP header structure\n
 *          -2 = Failure in given pointer (= NULL)
 */
extern int16_t sn_coap_builder_2(uint8_t *dst_packet_data_ptr, const sn_coap_hdr_s *src_coap_msg_ptr, uint16_t blockwise_payload_size);

/**
 * \fn uint16_t sn_coap_builder_calc_needed_packet_data_size_2(sn_coap_hdr_s *src_coap_msg_ptr, uint16_t blockwise_payload_size)
 *
 * \brief Calculates needed Packet data memory size for given CoAP message
 *
 * \param *src_coap_msg_ptr is pointer to data which needed Packet
 *          data length is calculated
 * \param blockwise_payload_size Blockwise message maximum payload size
 *
 * \return Return value is count of needed memory as bytes for build Packet data
 *          Null if failed
 */
extern uint16_t sn_coap_builder_calc_needed_packet_data_size_2(const sn_coap_hdr_s *src_coap_msg_ptr, uint16_t blockwise_payload_size);

/**
 * \fn sn_coap_hdr_s *sn_coap_build_response(struct coap_s *handle, sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code)
 *
 * \brief Prepares generic response packet from a request packet. This function allocates memory for the resulting sn_coap_hdr_s
 *
 * \param *handle Pointer to CoAP library handle
 * \param *coap_packet_ptr The request packet pointer
 * \param msg_code response messages code
 *
 * \return *coap_packet_ptr The allocated and pre-filled response packet pointer
 *          NULL    Error in parsing the request
 *
 */
extern sn_coap_hdr_s *sn_coap_build_response(struct coap_s *handle, const sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code);

/**
 * \brief Initialise a message structure to empty
 *
 * \param *coap_msg_ptr is pointer to CoAP message to initialise
 *
 * \return Return value is pointer passed in
 */
extern sn_coap_hdr_s *sn_coap_parser_init_message(sn_coap_hdr_s *coap_msg_ptr);

/**
 * \brief Allocate an empty message structure
 *
 * \param *handle Pointer to CoAP library handle
 *
 * \return Return value is pointer to an empty CoAP message.\n
 *         In following failure cases NULL is returned:\n
 *          -Failure in given pointer (= NULL)\n
 *          -Failure in memory allocation (malloc() returns NULL)
 */
extern sn_coap_hdr_s *sn_coap_parser_alloc_message(struct coap_s *handle);

/**
 * \brief Allocates and initializes options list structure
 *
 * \param *handle Pointer to CoAP library handle
 * \param *coap_msg_ptr is pointer to CoAP message that will contain the options
 *
 * If the message already has a pointer to an option structure, that pointer
 * is returned, rather than a new structure being allocated.
 *
 * \return Return value is pointer to the CoAP options structure.\n
 *         In following failure cases NULL is returned:\n
 *          -Failure in given pointer (= NULL)\n
 *          -Failure in memory allocation (malloc() returns NULL)
 */
extern sn_coap_options_list_s *sn_coap_parser_alloc_options(struct coap_s *handle, sn_coap_hdr_s *coap_msg_ptr);

extern sn_coap_hdr_s *sn_coap_parser_alloc_message_with_options(struct coap_s *handle);

#ifdef __cplusplus
}
#endif

#endif /* SN_COAP_HEADER_H_ */
