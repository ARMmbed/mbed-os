/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
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

 #ifndef MBED_CONF_APP_CONNECT_STATEMENT
     #error [NOT_SUPPORTED] No network configuration found for this target.
 #endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include MBED_CONF_APP_HEADER_FILE

using namespace utest::v1;

// Hostname for testing against
// Must have A and AAAA records
#ifndef MBED_DNS_TEST_HOST
#define MBED_DNS_TEST_HOST "connector.mbed.com"
#endif


// Address info from stack
const char *ip_literal;
nsapi_version_t ip_pref;
const char *ip_pref_repr;

// Network setup
NetworkInterface *net;

void net_bringup() {
    net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;
    TEST_ASSERT_EQUAL(0, err);
    printf("MBED: Connected to network\n");
    printf("MBED: IP Address: %s\n", net->get_ip_address());

    ip_literal = net->get_ip_address();
    ip_pref = SocketAddress(ip_literal).get_ip_version();
    ip_pref_repr = (ip_pref == NSAPI_IPv4) ? "ipv4" :
                   (ip_pref == NSAPI_IPv6) ? "ipv6" : "unspec";
}


// DNS tests
void test_dns_query() {
    SocketAddress addr;
    int err = net->gethostbyname(MBED_DNS_TEST_HOST, &addr);
    printf("DNS: query \"%s\" => \"%s\"\n",
            MBED_DNS_TEST_HOST, addr.get_ip_address());

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT((bool)addr);
    TEST_ASSERT(strlen(addr.get_ip_address()) > 1);
}

void test_dns_query_pref() {
    SocketAddress addr;
    int err = net->gethostbyname(MBED_DNS_TEST_HOST, &addr, ip_pref);
    printf("DNS: query %s \"%s\" => \"%s\"\n",
            ip_pref_repr, MBED_DNS_TEST_HOST, addr.get_ip_address());

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT((bool)addr);
    TEST_ASSERT(strlen(addr.get_ip_address()) > 1);
    TEST_ASSERT_EQUAL(ip_pref, addr.get_ip_version());
}

void test_dns_literal() {
    SocketAddress addr;
    int err = net->gethostbyname(ip_literal, &addr);
    printf("DNS: literal \"%s\" => \"%s\"\n",
            ip_literal, addr.get_ip_address());

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT((bool)addr);
    TEST_ASSERT(strlen(addr.get_ip_address()) > 1);
    TEST_ASSERT(strcmp(ip_literal, addr.get_ip_address()) == 0);
}

void test_dns_literal_pref() {
    SocketAddress addr;
    int err = net->gethostbyname(ip_literal, &addr, ip_pref);
    printf("DNS: literal %s \"%s\" => \"%s\"\n",
            ip_pref_repr, ip_literal, addr.get_ip_address());

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT((bool)addr);
    TEST_ASSERT(strlen(addr.get_ip_address()) > 1);
    TEST_ASSERT_EQUAL(ip_pref, addr.get_ip_version());
    TEST_ASSERT(strcmp(ip_literal, addr.get_ip_address()) == 0);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(120, "default_auto");
    net_bringup();
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("DNS query",               test_dns_query),
    Case("DNS preference query",    test_dns_query_pref),
    Case("DNS literal",             test_dns_literal),
    Case("DNS preference literal",  test_dns_literal_pref),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
