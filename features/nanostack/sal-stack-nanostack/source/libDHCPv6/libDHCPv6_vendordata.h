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

/* ARM Defined vendor data option to distribute network time through DHCP server
 * Format. This can be used to syncronice the times of Wi-SUN devices to match the
 * time of border router.
 *
 * Following rfc5905 date format.
 *  +-------------+------------+-----+---------------+------------------+
 *  | Date        | MJD        | NTP | NTP Timestamp | Epoch            |
 *  |             |            | Era | Era Offset    |                  |
 *  +-------------+------------+-----+---------------+------------------+
 *  | 1 Jan -4712 | -2,400,001 | -49 | 1,795,583,104 | 1st day Julian   |
 *  | 1 Jan -1    | -679,306   | -14 | 139,775,744   | 2 BCE            |
 *  | 1 Jan 0     | -678,491   | -14 | 171,311,744   | 1 BCE            |
 *  | 1 Jan 1     | -678,575   | -14 | 202,939,144   | 1 CE             |
 *  | 4 Oct 1582  | -100,851   | -3  | 2,873,647,488 | Last day Julian  |
 *  | 15 Oct 1582 | -100,840   | -3  | 2,874,597,888 | First day        |
 *  |             |            |     |               | Gregorian        |
 *  | 31 Dec 1899 | 15019      | -1  | 4,294,880,896 | Last day NTP Era |
 *  |             |            |     |               | -1               |
 *  | 1 Jan 1900  | 15020      | 0   | 0             | First day NTP    |
 *  |             |            |     |               | Era 0            |
 *  | 1 Jan 1970  | 40,587     | 0   | 2,208,988,800 | First day UNIX   |
 *  | 1 Jan 1972  | 41,317     | 0   | 2,272,060,800 | First day UTC    |
 *  | 31 Dec 1999 | 51,543     | 0   | 3,155,587,200 | Last day 20th    |
 *  |             |            |     |               | Century          |
 *  | 8 Feb 2036  | 64,731     | 1   | 63,104        | First day NTP    |
 *  |             |            |     |               | Era 1            |
 *  +-------------+------------+-----+---------------+------------------+
 * int32_t Era
 * uint32_t offset
 * uint32_t fraction
 *
 * */
#define ARM_DHCP_VENDOR_DATA_NETWORK_TIME 298

/* DHCPv6 vendor options to distribute ARM vendor data*/

uint16_t net_dns_option_vendor_option_data_dns_query_length(char *domain);
uint8_t *net_dns_option_vendor_option_data_dns_query_write(uint8_t *ptr, uint8_t *address, char *domain);

uint16_t net_dns_option_vendor_option_data_get_next(uint8_t *ptr, uint16_t length, uint16_t *type);
uint16_t net_dns_option_vendor_option_data_dns_query_read(uint8_t *ptr, uint16_t length, uint8_t **address, char **domain);

uint16_t net_vendor_option_current_time_length(void);
uint8_t *net_vendor_option_current_time_write(uint8_t *ptr, int32_t era, uint32_t offset, uint32_t fraction);
uint8_t *net_vendor_option_current_time_read(uint8_t *ptr, uint16_t length, int32_t *era, uint32_t *offset, uint32_t *fraction);


#endif /* LIBDHCPV6_VENDOR_DATA_H_ */
