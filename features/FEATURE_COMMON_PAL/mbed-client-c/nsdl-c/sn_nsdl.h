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
* \file sn_nsdl.h
*
* \brief libNsdl generic header file
*
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SN_NSDL_H_
#define SN_NSDL_H_

/* * * Common * * */

#define SN_NSDL_SUCCESS  0
#define SN_NSDL_FAILURE (-1)

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

/**
 * \brief Address type of given address
 */
typedef enum sn_nsdl_addr_type_ {
    SN_NSDL_ADDRESS_TYPE_IPV6       = 0x01,         /**< Supported */
    SN_NSDL_ADDRESS_TYPE_IPV4       = 0x02,         /**< Supported */
    SN_NSDL_ADDRESS_TYPE_HOSTNAME   = 0x03,         /**< Unsupported */
    SN_NSDL_ADDRESS_TYPE_NONE       = 0xFF
} sn_nsdl_addr_type_e;


#define SN_NDSL_RESOURCE_NOT_REGISTERED 0
#define SN_NDSL_RESOURCE_REGISTERING    1
#define SN_NDSL_RESOURCE_REGISTERED     2


/* * * * * * * * * * * * * */
/* * * * STRUCTURES  * * * */
/* * * * * * * * * * * * * */

/**
 * \brief Address structure of Packet data
 */
typedef struct sn_nsdl_addr_ {
    uint8_t                 addr_len;
    sn_nsdl_addr_type_e     type;
    uint16_t                port;
    uint8_t                 *addr_ptr;
} sn_nsdl_addr_s;

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

#endif /* SN_NSDL_H_ */

#ifdef __cplusplus
}
#endif
