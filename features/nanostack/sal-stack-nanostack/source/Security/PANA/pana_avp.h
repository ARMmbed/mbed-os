/*
 * Copyright (c) 2017-2019, Pelion and affiliates.
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

#ifndef PANA_AVP_H_
#define PANA_AVP_H_

#define PANA_EAP_VENDOR_FLAG 0x8000

#define AVP_AUTHENCY_CODE               1
#define AVP_EAP_PAYLOAD_CODE            2
#define AVP_INTEGRIRTY_ALGORYTHM_CODE   3
#define AVP_KEY_ID_CODE                 4
#define AVP_NONCE_CODE                  5
#define AVP_PRF_ALGORYTHM_CODE          6
#define AVP_RESULT_CODE                 7
#define AVP_SESSION_LIFETIME_CODE       8
/**
 * Pana relay AVP's
 */
#define AVP_PAC_INFO_CODE               10
#define AVP_RELAY_MSG_CODE              11

/**
 * Define Key delivery method
 */
#define AVP_KEY_WRAP_ALG_CODE           12
/**
 * Key delivery AVP's which are encrypted at message
 */
#define AVP_ENCRYPT_ALGORITHM_CODE      13


/****************************************
 * Pana AVP Base
 *
 * Code     Flags   Length  RESErved
 * 16-bit   16-bit  16-bit  16-bit
 *
 */

typedef struct {
    uint32_t vendor_id;
    uint16_t code;
    uint16_t flags;
    uint16_t len;
    uint8_t *avp_ptr;
} pana_avp_t;

uint8_t *pana_avp_base_write(uint8_t avp_type, uint16_t length, uint8_t *dptr, uint16_t flags, uint32_t vendor_id);

uint8_t *pana_avp_32_bit_write(uint8_t avp_type, uint32_t value, uint8_t *dptr);
/**
 * Write AVP header and data also if value is not NULL
 *
 * Add automatically padding's for word size 4
 *
 * \return pointer end of message
 */
uint8_t *pana_avp_write_n_bytes(uint16_t avp_type, uint16_t length, const uint8_t *value, uint8_t *dptr);

/**
 * Write AVP header with vendor id and data also if value is not NULL
 *
 * Add automatically padding's for word size 4
 *
 * \return pointer end of message
 */
uint8_t *pana_avp_vendor_id_write_n_bytes(uint16_t avp_type, uint16_t length, const uint8_t *value, uint8_t *dptr, uint32_t vendor_id);

bool pana_avp_discover(uint8_t *dptr, uint16_t data_len, pana_avp_t *avp);

#endif /* PANA_AVP_H_ */
