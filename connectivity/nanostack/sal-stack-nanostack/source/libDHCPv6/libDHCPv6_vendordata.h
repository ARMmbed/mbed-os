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

#ifndef LIBDHCPV6_VENDOR_DATA_H_
#define LIBDHCPV6_VENDOR_DATA_H_


/*ARM enterprise number used to identify ARM generated Vendor data options*/
#define ARM_ENTERPRISE_NUMBER 4128

/* ARM Defined vendor data option to distribute DNS query results through DHCP server
 * Format
 *
 * uint8_t address[16]
 * domain string nul terminated.
 *
 * multiple results must be in separated vendor option data fields
 * */
#define ARM_DHCP_VENDOR_DATA_DNS_QUERY_RESULT 297

/* DHCPv6 vendor options to distribute ARM vendor data*/

uint16_t net_dns_option_vendor_option_data_dns_query_length(char *domain);
uint8_t *net_dns_option_vendor_option_data_dns_query_write(uint8_t *ptr, uint8_t *address, char *domain);

uint16_t net_dns_option_vendor_option_data_get_next(uint8_t *ptr, uint16_t length, uint16_t *type);
uint16_t net_dns_option_vendor_option_data_dns_query_read(uint8_t *ptr, uint16_t length, uint8_t **address, char **domain);


#endif /* LIBDHCPV6_VENDOR_DATA_H_ */
