/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef PANA_EAP_HEADER_H_
#define PANA_EAP_HEADER_H_

// Codes
#define EAP_REQ                         0x01
#define EAP_RESPONSE                    0x02
#define EAP_SUCCESS                     0x03
#define EAP_FAILURE                     0x04

// Request/Response Types
#define EAP_IDENTITY                    1
#define EAP_TLS                         13
#define EAP_TTLS                        21

#define EAP_TLS_START                   0x20
#define EAP_TLS_MORE_FRAGMENTS          0x40
#define EAP_TLS_FRAGMENT_LENGTH         0x80

#define EAP_MTU_SIZE                    520
#define EAP_MTU_FRAG_SIZE               296

/****************************************
 * EAP BASE For EAP_REQ & EAP_RESPONSE
 *
 * Code     Identifier  Length  Type
 * 8-bit    8-bit       16-bit  8-bit
 *
 */

/****************************************
 * EAP BASE For EAP_SUCCESS & EAP_FAILURE
 *
 * Code     Identifier  Length
 * 8-bit    8-bit       16-bit
 *
 */

typedef struct {
    uint16_t length;
    uint8_t eap_code;
    uint8_t id_seq;
    uint8_t type;
    uint8_t *data_ptr;
} eap_header_t;

typedef struct {
    uint8_t *data_ptr;
    uint8_t eap_tls_flags;
    uint16_t tls_frame_length;
    uint16_t tls_length;
} eap_tls_header_t;

bool eap_header_parse(uint8_t *data_ptr, uint16_t length, eap_header_t *header);
uint8_t eap_header_size(uint8_t eap_code);
uint8_t *eap_header_build(uint8_t *ptr, uint16_t data_length, uint8_t eap_code, uint8_t id_seq, uint8_t type);
bool eap_tls_header_parse(uint8_t *eap_data_ptr, uint16_t eap_datalength, eap_tls_header_t *header);
uint8_t eap_tls_header_size(uint16_t eap_tls_flags);
uint8_t *eap_tls_header_build(uint8_t *ptr, uint16_t eap_tls_flags, uint16_t frame_length);


#endif /* PANA_EAP_HEADER_H_ */
