/*
 * Copyright (c) 2018-2019 ARM Limited
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

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef MIN
extern int MIN(/*@sef@*/ int x, /*@sef@*/ int y);  /* LINT : This tells lint that  the parameter must be side-effect free. i.e. evaluation does not change any values (since it is being evaulated more than once */
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif /* ifndef MIN */

#define FX_IPTYPE_IPV4                  (0)
#define FX_IPTYPE_IPV6                  (1)

typedef union {
    uint32_t addr;
    uint8_t addrs[4];
} cy_ip_addr_v4_t;

typedef struct {
    uint32_t addr[4];
} cy_ip_addr_v6_t;

typedef struct {
    uint8_t type;
    union {
        cy_ip_addr_v4_t addrv4;
        cy_ip_addr_v6_t addrv6;
    };
} cy_ip_addr_t;

/**
 * Structure for storing a MAC address (Wi-Fi Media Access Control address).
 */
typedef struct {
    uint8_t octet[6]; /**< Unique 6-byte MAC address */
} cy_mac_addr_t;

/**
 * Converts a unsigned long int to a decimal string
 *
 * @param value[in]      : The unsigned long to be converted
 * @param output[out]    : The buffer which will receive the decimal string
 * @param min_length[in] : the minimum number of characters to output (zero padding will apply if required).
 * @param max_length[in] : the maximum number of characters to output (up to 10 ). There must be space for terminating NULL.
 *
 * @note: A terminating NULL is added. Wnsure that there is space in the buffer for this.
 *
 * @return the number of characters returned (excluding terminating null)
 *
 */
uint8_t unsigned_to_decimal_string(uint32_t value, char *output, uint8_t min_length, uint8_t max_length);

/**
 *  Convert a IPv4 address to a string
 *
 *  @note: String is 16 bytes including terminating null
 *
 * @param[out] buffer       : Buffer which will recieve the IPv4 string
 * @param[in]  ipv4_address : IPv4 address to convert
 */
void ipv4_to_string(char buffer[16], uint32_t ipv4_address);

/**
 *  Convert a IPv4 address to a string
 *
 *  @note: String is 16 bytes including terminating null
 *
 * @param[in] buffer         : Buffer which has the IPv4 string
 * @return  ipv4_address (0 on failure)
 */
uint32_t string_to_ipv4(const char *buffer);

#if defined(__cplusplus)
}
#endif
