/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "nsapi_dns.h"
#include "mbed_trace.h"

#define TRACE_GROUP "GRNT"

#ifndef MULTIHOMING_TESTS_H
#define MULTIHOMING_TESTS_H

#define DNS_TEST_HOST_LEN 40
#define INTERFACE_NAME_LEN 6

#ifndef MBED_CONF_MULTIHOMING_MAX_INTERFACES_NUM
#define MBED_CONF_MULTIHOMING_MAX_INTERFACES_NUM    2
#endif

#ifndef MBED_CONF_APP_DNS_TEST_HOSTS_NUM
#define MBED_CONF_APP_DNS_TEST_HOSTS_NUM 12
#endif

// Hostnames for testing against
// Both lists must have A and AAAA records
#ifndef MBED_CONF_APP_DNS_TEST_HOSTS
#define MBED_CONF_APP_DNS_TEST_HOSTS {"google.com", "youtube.com", "facebook.com", "wikipedia.org", "yahoo.com", "instagram.com", "ipv6ready.org", "wireshark.org", "bbc.co.uk", "cnn.com", "www.flickr.com", "www.mozilla.org"}
#endif


#define ETH_INTERFACE  0
#define WIFI_INTERFACE 1


struct dns_application_data {
    rtos::Semaphore *semaphore;
    nsapi_error_t result;
    SocketAddress addr;
    nsapi_error_t req_result;
    bool value_set;
};


extern char interface_name[MBED_CONF_MULTIHOMING_MAX_INTERFACES_NUM][INTERFACE_NAME_LEN];
extern int  interface_num;

const char dns_test_hosts[MBED_CONF_APP_DNS_TEST_HOSTS_NUM][DNS_TEST_HOST_LEN] = MBED_CONF_APP_DNS_TEST_HOSTS;


NetworkInterface *get_interface(int interface_index);
void drop_bad_packets(UDPSocket &sock, int orig_timeout);
void fill_tx_buffer_ascii(char *buff, size_t len);

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
extern mbed_stats_socket_t udp_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
int fetch_stats(void);
#endif

/*
 * Test cases
 */
void MULTIHOMING_SYNCHRONOUS_DNS();
void MULTIHOMING_ASYNCHRONOUS_DNS();
void MULTIHOMING_UDPSOCKET_ECHOTEST();
void MULTIHOMING_UDPSOCKET_ECHOTEST_NONBLOCK();

#endif //UDP_TESTS_H
