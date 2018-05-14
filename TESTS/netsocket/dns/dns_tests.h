/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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

#ifndef DNS_TESTS_H
#define DNS_TESTS_H

#ifndef MBED_CONF_APP_DNS_SIMULT_QUERIES
#define MBED_CONF_APP_DNS_SIMULT_QUERIES   5
#endif

#ifndef MBED_CONF_NSAPI_DNS_CACHE_SIZE
#define MBED_CONF_NSAPI_DNS_CACHE_SIZE     3
#endif

// Hostnames for testing against
// Both lists must have A and AAAA records
#ifndef MBED_CONF_APP_DNS_TEST_HOSTS
#define MBED_CONF_APP_DNS_TEST_HOSTS {"google.com", "youtube.com", "facebook.com", "wikipedia.org", "yahoo.com", "instagram.com"}
#endif

#ifndef MBED_CONF_APP_DNS_TEST_HOSTS_SECOND
#define MBED_CONF_APP_DNS_TEST_HOSTS_SECOND {"ipv6ready.org", "wireshark.org", "bbc.co.uk", "cnn.com", "www.flickr.com", "www.mozilla.org"}
#endif

#ifndef MBED_CONF_APP_DNS_TEST_HOSTS_NUM
#define MBED_CONF_APP_DNS_TEST_HOSTS_NUM 6
#endif

#define DNS_TEST_HOST_LEN 40

struct dns_application_data {
    rtos::Semaphore *semaphore;
    nsapi_error_t result;
    SocketAddress addr;
    nsapi_error_t req_result;
    bool value_set;
};

extern const char dns_test_hosts[MBED_CONF_APP_DNS_TEST_HOSTS_NUM][DNS_TEST_HOST_LEN];
extern const char dns_test_hosts_second[MBED_CONF_APP_DNS_TEST_HOSTS_NUM][DNS_TEST_HOST_LEN];

/*
 * Utility functions
 */
NetworkInterface *get_interface();
void hostbyname_cb(void *data, nsapi_error_t result, SocketAddress *address);
void do_asynchronous_gethostbyname(const char hosts[][DNS_TEST_HOST_LEN], unsigned int op_count, int *exp_ok, int *exp_no_mem, int *exp_dns_failure, int *exp_timeout);

/*
 * Test cases
 */
void ASYNCHRONOUS_DNS();
void ASYNCHRONOUS_DNS_SIMULTANEOUS();
void ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE();
void ASYNCHRONOUS_DNS_CACHE();
void ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC();
void ASYNCHRONOUS_DNS_CANCEL();
void ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE();
void ASYNCHRONOUS_DNS_INVALID_HOST();
void ASYNCHRONOUS_DNS_TIMEOUTS();
void ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT();

#endif
