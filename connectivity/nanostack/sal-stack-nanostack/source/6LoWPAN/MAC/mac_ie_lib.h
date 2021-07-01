/*
 * Copyright (c) 2018, Pelion and affiliates.
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

#ifndef MAC_IE_LIB_H_
#define MAC_IE_LIB_H_

struct mac_payload_IE_s;
struct mac_payload_IE_s;

/**
 * @brief struct mac_nested_payload_IE_t Mac Nested IE Payload information element structure for parsing or write operation
 */
typedef struct mac_nested_payload_IE_s {
    uint8_t *content_ptr;   /**< Content data */
    uint16_t length;        /**< Element length 0- 2047 when type_long true and for short 0- 255*/
    unsigned id: 7;         /**< Group ID 4-bit for long and 7 bit for short type */
    bool type_long: 1;       /**< True when Nested IE long format and false for short */
} mac_nested_payload_IE_t;

/** IE header element generic header write */
uint8_t *mac_ie_header_base_write(uint8_t *ptr, uint8_t type, uint16_t length);

/** IE payload element generic header write */
uint8_t *mac_ie_payload_base_write(uint8_t *ptr, uint8_t type, uint16_t length);

/** Nested IE long header write */
uint8_t *mac_ie_nested_ie_long_base_write(uint8_t *ptr, uint8_t sub_id, uint16_t length);

/** Nested IE short header write */
uint8_t *mac_ie_nested_ie_short_base_write(uint8_t *ptr, uint8_t sub_id, uint16_t length);

/** Payload IE discover for spesific group ID */
uint16_t mac_ie_payload_discover(uint8_t *payload_ptr, uint16_t length, struct mac_payload_IE_s *payload_ie);

/** Nested IE element discover inside parsed payload element */
uint16_t mac_ie_nested_discover(uint8_t *payload_ptr, uint16_t length, mac_nested_payload_IE_t *nested_ie);

/** Header IE elemnt discover */
uint8_t mac_ie_header_discover(uint8_t *header_ptr, uint16_t length, struct mac_header_IE_s *header_ie);

/** Header IE elemnt discover with sub id */
uint8_t mac_ie_header_sub_id_discover(uint8_t *header_ptr, uint16_t length, mac_header_IE_t *header_ie, uint8_t sub_id);

#endif /* MAC_IE_LIB_H_ */
