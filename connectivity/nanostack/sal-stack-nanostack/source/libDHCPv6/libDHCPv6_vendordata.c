/*
 * Copyright (c) 2020, Pelion and affiliates.
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

uint16_t net_vendor_option_current_time_length(void)
{
    return 4 + 3 * sizeof(uint32_t);
}

uint8_t *net_vendor_option_current_time_write(uint8_t *ptr, int32_t era, uint32_t offset, uint32_t fraction)
{
    /*
     * Time value written using NTP time format
     * Following rfc5905
     */

    ptr = common_write_16_bit(ARM_DHCP_VENDOR_DATA_NETWORK_TIME, ptr);
    ptr = common_write_16_bit(3 * sizeof(uint32_t), ptr);
    ptr = common_write_32_bit((uint32_t)era, ptr);
    ptr = common_write_32_bit(offset, ptr);
    ptr = common_write_32_bit(fraction, ptr);
    return ptr;
}

uint8_t *net_vendor_option_current_time_read(uint8_t *ptr, uint16_t length, int32_t *era, uint32_t *offset, uint32_t *fraction)
{
    /*
     * Time value written using NTP time format
     * Following rfc5905
     */
    uint16_t option_len;

    if (length < net_vendor_option_current_time_length()) {
        // Corrupted as there is no room for all fields
        return 0;
    }

    if (common_read_16_bit(ptr) != ARM_DHCP_VENDOR_DATA_NETWORK_TIME) {
        return 0;
    }

    option_len = common_read_16_bit(ptr + 2);
    ptr += 2 * sizeof(uint16_t);

    if (option_len < 3 * sizeof(uint32_t)) {
        // Corrupted as not enough room for fields
        return 0;
    }

    if (era) {
        *era = (int32_t)common_read_32_bit(ptr);
    }

    if (offset) {
        *offset = common_read_32_bit(ptr + sizeof(uint32_t));
    }

    if (fraction) {
        *fraction = common_read_32_bit(ptr + sizeof(uint32_t) + sizeof(uint32_t));
    }

    return ptr;
}

uint16_t net_vendor_option_time_configuration_length(void)
{
    return 4 + 1 * sizeof(uint64_t) + 2 * sizeof(int16_t) + 1 * sizeof(uint16_t);
}

uint8_t *net_vendor_option_time_configuration_write(uint8_t *ptr, uint64_t timestamp, int16_t timezone, int16_t deviation, uint16_t status)
{

    ptr = common_write_16_bit(ARM_DHCP_VENDOR_DATA_TIME_CONFIGURATION, ptr);
    ptr = common_write_16_bit(1 * sizeof(uint64_t) + 2 * sizeof(int16_t) + 1 * sizeof(uint16_t), ptr);
    ptr = common_write_16_bit(status, ptr);
    ptr = common_write_64_bit(timestamp, ptr);
    ptr = common_write_16_bit((uint16_t)deviation, ptr);
    ptr = common_write_16_bit((uint16_t)timezone, ptr);
    return ptr;
}

uint8_t *net_vendor_option_time_configuration_read(uint8_t *ptr, uint16_t length, uint64_t *timestamp, int16_t *timezone, int16_t *deviation, uint16_t *status)
{
    uint16_t option_len;

    if (length < net_vendor_option_time_configuration_length()) {
        // Corrupted as there is no room for all fields
        return 0;
    }

    if (common_read_16_bit(ptr) != ARM_DHCP_VENDOR_DATA_TIME_CONFIGURATION) {
        return 0;
    }

    option_len = common_read_16_bit(ptr + sizeof(uint16_t));
    ptr += 2 * sizeof(uint16_t);

    if (option_len < 1 * sizeof(uint64_t) + 2 * sizeof(int16_t) + 1 * sizeof(uint16_t)) {
        // Corrupted as not enough room for fields
        return 0;
    }

    if (status) {
        *status = (uint16_t)common_read_16_bit(ptr);
    }

    if (timestamp) {
        *timestamp = common_read_64_bit(ptr + sizeof(uint16_t));
    }

    if (deviation) {
        *deviation = (int16_t)common_read_16_bit(ptr + sizeof(uint16_t) + sizeof(uint64_t));
    }

    if (timezone) {
        *timezone = (int16_t)common_read_16_bit(ptr + sizeof(uint16_t) + sizeof(uint64_t) + sizeof(uint16_t));
    }

    return ptr;
}
