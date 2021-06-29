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
 * \file sn_coap_header_internal.h
 *
 * \brief Header file for CoAP Header part
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SN_COAP_HEADER_INTERNAL_H_
#define SN_COAP_HEADER_INTERNAL_H_


/* * * * * * * * * * * */
/* * * * DEFINES * * * */
/* * * * * * * * * * * */

#define COAP_VERSION                                COAP_VERSION_1 /* Tells which IETF CoAP specification version the CoAP message supports. */
/* This value is written to CoAP message header part. */

/* CoAP Header defines */
#define COAP_HEADER_LENGTH                          4   /* Fixed Header length of CoAP message as bytes */
#define COAP_HEADER_VERSION_MASK                    0xC0
#define COAP_HEADER_MSG_TYPE_MASK                   0x30
#define COAP_HEADER_TOKEN_LENGTH_MASK               0x0F
#define COAP_HEADER_MSG_ID_MSB_SHIFT                8

/* CoAP Options defines */
#define COAP_OPTIONS_OPTION_NUMBER_SHIFT            4

/* * * * * * * * * * * * * * */
/* * * * ENUMERATIONS  * * * */
/* * * * * * * * * * * * * * */

/* * * * * * * * * * * * * */
/* * * * STRUCTURES  * * * */
/* * * * * * * * * * * * * */

/**
 * \brief This structure is returned by sn_coap_exec() for sending
 */
typedef struct sn_nsdl_transmit_ {
    uint8_t                *packet_ptr;
    uint16_t                packet_len;
    sn_nsdl_addr_s          dst_addr_ptr;

    sn_nsdl_capab_e         protocol;

} sn_nsdl_transmit_s;

/* * * * * * * * * * * * * * * * * * * * * * */
/* * * * EXTERNAL FUNCTION PROTOTYPES  * * * */
/* * * * * * * * * * * * * * * * * * * * * * */
extern int8_t           sn_coap_header_validity_check(const sn_coap_hdr_s *src_coap_msg_ptr, coap_version_e coap_version);

#endif /* SN_COAP_HEADER_INTERNAL_H_ */

#ifdef __cplusplus
}
#endif
