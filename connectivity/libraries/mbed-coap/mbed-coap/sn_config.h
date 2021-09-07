/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
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

#ifndef SN_CONFIG_H
#define SN_CONFIG_H

#ifdef MBED_CLIENT_USER_CONFIG_FILE
#include MBED_CLIENT_USER_CONFIG_FILE
#endif

#ifdef MBED_CLOUD_CLIENT_USER_CONFIG_FILE
#include MBED_CLOUD_CLIENT_USER_CONFIG_FILE
#endif

#ifdef MBED_CLOUD_CLIENT_CONFIGURATION_ENABLED
#include "mbed-cloud-client/MbedCloudClientConfig.h"
#endif

/**
* \brief Configuration options (set of defines and values)
*
*  This lists set of compile-time options that needs to be used to enable
*  or disable features selectively, and set the values for the mandatory
*  parameters.
*/

/**
 * \def SN_COAP_DUPLICATION_MAX_MSGS_COUNT
 * \brief For Message duplication detection
 * Init value for the maximum count of messages to be stored for duplication detection
 * Setting of this value to 0 will disable duplication check, also reduce use of ROM memory
 * Default is set to 0.
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_DUPLICATION_MAX_MSGS_COUNT
#define SN_COAP_DUPLICATION_MAX_MSGS_COUNT MBED_CONF_MBED_CLIENT_SN_COAP_DUPLICATION_MAX_MSGS_COUNT
#endif

#ifndef SN_COAP_DUPLICATION_MAX_MSGS_COUNT
#define SN_COAP_DUPLICATION_MAX_MSGS_COUNT              0
#endif

/**
 * \def SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
 * \brief For Message blockwising
 * Init value for the maximum payload size to be sent and received at one blockwise message
 * Setting of this value to 0 with SN_COAP_BLOCKWISE_ENABLED will disable this feature, and
 * also reduce use of ROM memory.
 * Note: This define is common for both received and sent Blockwise messages
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#define SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE MBED_CONF_MBED_CLIENT_SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#endif

#ifndef SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
#define SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE              0  /**< Must be 2^x and x is at least 4. Suitable values: 0, 16, 32, 64, 128, 256, 512 and 1024 */
#endif

/**
 * \def SN_COAP_CONSTANT_NEEDED_SIZE
 * \brief Avoid needed size calculations
 * If this is defined, sn_coap_builder_calc_needed_packet_data_size always returns that value,
 * saving a lot of calculation code, at the cost of outgoing TX buffers being oversized, and
 * with danger of them being undersized.
 *
 * sn_coap_builder_payload_build does not have any size input to limit its output, so it is
 * always wise for users to assert that it has not output more than the size returned by
 * sn_coap_builder_calc_needed_packet_size, whether this option is defined or not.
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_CONSTANT_NEEDED_SIZE
#define SN_COAP_CONSTANT_NEEDED_SIZE MBED_CONF_MBED_CLIENT_SN_COAP_CONSTANT_NEEDED_SIZE
#endif

//#define SN_COAP_CONSTANT_NEEDED_SIZE                  1024

/**
 * \def SN_COAP_DISABLE_RESENDINGS
 * \brief Disables resending feature. Resending feature should not be needed
 * when using CoAP with TCP transport for example. By default resendings are
 * enabled. Set to 1 to disable.
 */
#ifdef SN_COAP_DISABLE_RESENDINGS
#define ENABLE_RESENDINGS                               0   /** Disable resendings **/
#else
#define ENABLE_RESENDINGS                               1   /**< Enable / Disable resending from library in building */
#endif

/**
 * \def SN_COAP_RESENDING_QUEUE_SIZE_MSGS
 * \brief Sets the number of messages stored
 * in the resending queue. Default is 2
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_MSGS
#define SN_COAP_RESENDING_QUEUE_SIZE_MSGS MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_MSGS
#endif

#ifndef SN_COAP_RESENDING_QUEUE_SIZE_MSGS
#define SN_COAP_RESENDING_QUEUE_SIZE_MSGS               2   /**< Default re-sending queue size - defines how many messages can be stored. Setting this to 0 disables feature */
#endif

/**
 * \def DEFAULT_RESPONSE_TIMEOUT
 * \brief Sets the CoAP re-send interval in seconds.
 * By default is 10 seconds.
 */

#ifdef MBED_CONF_MBED_CLIENT_DEFAULT_RESPONSE_TIMEOUT
#define DEFAULT_RESPONSE_TIMEOUT MBED_CONF_MBED_CLIENT_DEFAULT_RESPONSE_TIMEOUT
#endif

#ifndef DEFAULT_RESPONSE_TIMEOUT
#define DEFAULT_RESPONSE_TIMEOUT                        10  /**< Default re-sending timeout as seconds */
#endif

/**
 * \def SN_COAP_RESENDING_QUEUE_SIZE_BYTES
 * \brief Sets the size of the re-sending buffer.
 * Setting this to 0 disables this feature.
 * By default, this feature is disabled.
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_BYTES
#define SN_COAP_RESENDING_QUEUE_SIZE_BYTES MBED_CONF_MBED_CLIENT_SN_COAP_RESENDING_QUEUE_SIZE_BYTES
#endif

#ifndef SN_COAP_RESENDING_QUEUE_SIZE_BYTES
#define SN_COAP_RESENDING_QUEUE_SIZE_BYTES              0   /**< Default re-sending queue size - defines size of the re-sending buffer. Setting this to 0 disables feature */
#endif

/**
 * \def SN_COAP_MAX_INCOMING_MESSAGE_SIZE
 * \brief Sets the maximum size (in bytes) that
 * mbed Client will allow to be handled while
 * receiving big payload in blockwise mode.
 * Application can set this value based on their
 * available storage capability.
 * By default, maximum size is UINT16_MAX, 65535 bytes.
 */
#ifndef SN_COAP_MAX_INCOMING_MESSAGE_SIZE
#define SN_COAP_MAX_INCOMING_MESSAGE_SIZE               UINT16_MAX
#endif

/**
 * \def SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE
 * \brief Sets the maximum payload size allowed before blockwising the message.
 * This option should only be used when using TCP and TLS as transport
 * with known maximum fragment size. This optimizes the number of messages
 * if it is possible to send larger than 1kB messages without blockwise transfer.
 * If payload length is larger than SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE
 * it will be sent using blockwise transfer.
 * By default, this feature is disabled, 0 disables the feature, set to positive
 * value larger than SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE to enable.
 * Note that value should be less than transport layer maximum fragment size.
 * Note that value has no effect if blockwise transfer is disabled.
 */
#ifndef SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE
#define SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE           0
#endif

/**
 * \def SN_COAP_BLOCKWISE_ENABLED
 * \brief Enables the blockwise functionality in CoAP library also when blockwise payload
 * size is set to '0' in  SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE.
 */
#ifndef SN_COAP_BLOCKWISE_ENABLED
#define SN_COAP_BLOCKWISE_ENABLED                       0  /**< Enable blockwise */
#endif

/**
 * \def SN_COAP_RESENDING_MAX_COUNT
 * \brief Defines how many times CoAP library tries to re-send the CoAP packet.
 * By default value is 3.
 */
#ifdef MBED_CONF_MBED_CLIENT_RECONNECTION_COUNT
#define SN_COAP_RESENDING_MAX_COUNT MBED_CONF_MBED_CLIENT_RECONNECTION_COUNT
#endif

#ifndef SN_COAP_RESENDING_MAX_COUNT
#define SN_COAP_RESENDING_MAX_COUNT                     3
#endif

/**
 * \def SN_COAP_MAX_ALLOWED_RESENDING_COUNT
 * \brief Maximum allowed count of re-sending that can be set at runtime via
 * 'sn_coap_protocol_set_retransmission_parameters()' API.
 * By default value is 6.
 */
#ifndef SN_COAP_MAX_ALLOWED_RESENDING_COUNT
#define SN_COAP_MAX_ALLOWED_RESENDING_COUNT             6   /**< Maximum allowed count of re-sending */
#endif

/**
 * \def SN_COAP_MAX_ALLOWED_RESPONSE_TIMEOUT
 * \brief Maximum allowed re-send interval in seconds that can be set at runtime via
 * 'sn_coap_protocol_set_retransmission_parameters()' API.
 * By default value is 40.
 */
#ifndef SN_COAP_MAX_ALLOWED_RESPONSE_TIMEOUT
#define SN_COAP_MAX_ALLOWED_RESPONSE_TIMEOUT            40  /**< Maximum allowed re-sending timeout */
#endif

/**
 * \def SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_MSGS
 * \brief Maximum allowed count of messages that can be stored into resend buffer at runtime via
 * 'sn_coap_protocol_set_retransmission_buffer()' API.
 * By default value is 6.
 */
#ifndef SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_MSGS
#define SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_MSGS    6   /**< Maximum allowed number of saved re-sending messages */
#endif

/**
 * \def SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_BYTES
 * \brief Maximum size of messages in bytes that can be stored into resend buffer at runtime via
 * 'sn_coap_protocol_set_retransmission_buffer()' API.
 * By default value is 512.
 */
#ifndef SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_BYTES
#define SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_BYTES   512 /**< Maximum allowed size of re-sending buffer */
#endif

/**
 * \def SN_COAP_MAX_ALLOWED_DUPLICATION_MESSAGE_COUNT
 * \brief Maximum allowed number of saved messages for message duplicate searching
 * that can be set via 'sn_coap_protocol_set_duplicate_buffer_size' API.
 * By default value is 6.
 */
#ifndef SN_COAP_MAX_ALLOWED_DUPLICATION_MESSAGE_COUNT
#define SN_COAP_MAX_ALLOWED_DUPLICATION_MESSAGE_COUNT   6
#endif

/**
 * \def SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED
 * \brief Maximum time in seconds howe long message is kept for duplicate detection.
 * By default 60 seconds.
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED
#define SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED MBED_CONF_MBED_CLIENT_SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED
#endif

#ifndef SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED
#define SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED    300 /** RESPONSE_TIMEOUT * RESPONSE_RANDOM_FACTOR * (2 ^ MAX_RETRANSMIT - 1) + the expected maximum round trip time **/
#endif

/**
 * \def SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED
 * \brief Maximum time in seconds how long (messages and payload) are be stored for blockwising.
 * Longer time will increase the memory consumption in lossy networks.
 * By default 60 seconds.
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED
#define SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED MBED_CONF_MBED_CLIENT_SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED
#endif

#ifndef SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED
#define SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED      300 /**< Maximum time in seconds of data (messages and payload) to be stored for blockwising */
#endif

/**
 * \def SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE
 * \brief Maximum size of blockwise message that can be received.
 * By default 65535 bytes.
 */
#ifdef MBED_CONF_MBED_CLIENT_SN_COAP_MAX_INCOMING_MESSAGE_SIZE
#define SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE MBED_CONF_MBED_CLIENT_SN_COAP_MAX_INCOMING_MESSAGE_SIZE
#endif

#ifndef SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE
#define SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE     UINT16_MAX
#endif

/**
 * \def SN_COAP_BLOCKWISE_INTERNAL_BLOCK_2_HANDLING_ENABLED
 * \brief A size optimization switch, which removes the blockwise Block2 response if set to 0.
 * handling code which is typically overridden by a call of "sn_coap_protocol_handle_block2_response_internally(coap, false);".
 * By default the code is there, so the override can be reversed by "sn_coap_protocol_handle_block2_response_internally(coap, true)".
 */
#ifndef SN_COAP_BLOCKWISE_INTERNAL_BLOCK_2_HANDLING_ENABLED
#define SN_COAP_BLOCKWISE_INTERNAL_BLOCK_2_HANDLING_ENABLED  1
#endif

/**
 * \def SN_COAP_REDUCE_BLOCKWISE_HEAP_FOOTPRINT
 * \brief A heap optimization switch, which removes unnecessary copy of the blockwise data.
 * If enabled, application must NOT free the payload when it gets the COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED status.
 * Application must call sn_coap_protocol_block_remove() instead.
 */
#ifndef SN_COAP_REDUCE_BLOCKWISE_HEAP_FOOTPRINT
#define SN_COAP_REDUCE_BLOCKWISE_HEAP_FOOTPRINT              0   /**< Disabled by default */
#endif

#endif // SN_CONFIG_H
