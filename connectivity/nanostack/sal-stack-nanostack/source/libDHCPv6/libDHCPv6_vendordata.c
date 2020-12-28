/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "ns_types.h"
#include "ns_trace.h"
#include <string.h>
#include "common_functions.h"
#include "libDHCPv6.h"
#include "libDHCPv6_vendordata.h"

#define TRACE_GROUP "vend"


/* DHCPv6 vendor options to distribute ARM vendor data*/

uint16_t net_dns_option_vendor_option_data_dns_query_length(char *domain)
{
    return 4 + 16 + strlen(domain) + 1;
}

uint8_t *net_dns_option_vendor_option_data_dns_query_write(uint8_t *ptr, uint8_t *address, char *domain)
{
    int domain_len = strlen(domain);
    int length = 16 + domain_len + 1;

    ptr = common_write_16_bit(ARM_DHCP_VENDOR_DATA_DNS_QUERY_RESULT, ptr);
    ptr = common_write_16_bit(length, ptr);
    memcpy(ptr, address, 16);
    ptr += 16;
    memcpy(ptr, domain, domain_len + 1);
    ptr += domain_len + 1;
    return ptr;
}

uint16_t net_dns_option_vendor_option_data_get_next(uint8_t *ptr, uint16_t length, uint16_t *type)
{
    uint16_t option_len;

    if (length < 4) {
        // Corrupted
        return 0;
    }
    if (type) {
        *type = common_read_16_bit(ptr);
    }

    option_len = common_read_16_bit(ptr + 2);
    if (option_len + 4 > length) {
        // Corrupted
        return 0;
    }

    return option_len + 4;
}

uint16_t net_dns_option_vendor_option_data_dns_query_read(uint8_t *ptr, uint16_t length, uint8_t **address, char **domain)
{
    uint16_t option_len;
    option_len = common_read_16_bit(ptr + 2);

    if (length < 4 + 16 + 1) {
        // Corrupted as there is no room for all fields
        return 0;
    }
    if (option_len < 17) {
        // Corrupted as not enough room in field
        return 0;
    }
    if (*(ptr + 4 + option_len - 1) != 0) {
        // Not nul terminated string for domain
        return 0;
    }

    if (common_read_16_bit(ptr) != ARM_DHCP_VENDOR_DATA_DNS_QUERY_RESULT) {
        return 0;
    }

    if (address) {
        *address = ptr + 4;
    }
    if (domain) {
        *domain = (char *)(ptr + 4 + 16);
    }
    return option_len;
}
