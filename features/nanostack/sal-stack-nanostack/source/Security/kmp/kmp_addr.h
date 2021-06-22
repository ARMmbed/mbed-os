/*
 * Copyright (c) 2016-2019, Pelion and affiliates.
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

#ifndef KMP_ADDR_H_
#define KMP_ADDR_H_

typedef enum {
    KMP_ADDR_EUI_64 = 0,
    KMP_ADDR_EUI_64_AND_IP
} kmp_addr_e;

typedef struct {
    uint8_t type;
    uint8_t eui_64[8];
    address_t relay_address;
    uint16_t port;
} kmp_addr_t;

/**
 * kmp_address_init initializes address
 *
 * \param type address type
 * \param addr address
 * \param eui_64 EUI-64
 *
 */
void kmp_address_init(kmp_addr_e type, kmp_addr_t *addr, const uint8_t *eui_64);


/**
 * kmp_address_eui_64_get get EUI-64
 *
 * \param addr address
 *
 * \return EUI-64
 *
 */
const uint8_t *kmp_address_eui_64_get(const kmp_addr_t *addr);

/**
 * kmp_address_ip_get get IP address
 *
 * \param addr address
 *
 * \return IP address
 *
 */
const uint8_t *kmp_address_ip_get(const kmp_addr_t *addr);


/**
 * kmp_address_eui_64_set set EUI-64
 *
 * \param addr address
 * \param ip_addr EUI-64
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_address_eui_64_set(kmp_addr_t *addr, const uint8_t *eui64);

/**
 * kmp_address_copy copies address
 *
 * \param to_addr address to copy to
 * \param from_addr address to copy from
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kmp_address_copy(kmp_addr_t *to_addr, const kmp_addr_t *from_addr);

#endif /* KMP_ADDR_H_ */
