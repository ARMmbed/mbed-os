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
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "multihoming_tests.h"

using namespace utest::v1;

namespace {
int result_ok;
int result_no_mem;
int result_dns_failure;
int result_exp_timeout;
}




// Callback used for asynchronous DNS result
void hostbyname_cb(void *data, nsapi_error_t result, SocketAddress *address)
{
    dns_application_data *app_data = static_cast<dns_application_data *>(data);
    app_data->result = result;
    if (address) {
        app_data->addr = *address;
    }
    app_data->semaphore->release();
    app_data->value_set = true;
}

void MULTIHOMING_ASYNCHRONOUS_DNS()
{
    rtos::Semaphore semaphore;
    dns_application_data data;
    data.semaphore = &semaphore;

    nsapi_dns_reset();

    for (unsigned int i = 0; i < MBED_CONF_APP_DNS_TEST_HOSTS_NUM; i++) {

        for (unsigned int interface_index = 0; interface_index < MBED_CONF_MULTIHOMING_MAX_INTERFACES_NUM; interface_index++) {
            NetworkInterface  *interface = get_interface(interface_index);
            if (interface == NULL) {
                continue;
            }

            for (int j = 0; j < interface_num; j++) {

                nsapi_error_t err = interface->gethostbyname_async(dns_test_hosts[i],
                                                                       mbed::Callback<void(nsapi_error_t, SocketAddress *)>(hostbyname_cb, (void *) &data), NSAPI_UNSPEC, interface_name[j]);
                TEST_ASSERT(err >= 0);

                semaphore.acquire();

                TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, data.result);
                printf("DNS: query  interface_name %s %d \n", interface_name[j], j);
                if (data.result == NSAPI_ERROR_OK) {
                    result_ok++;
                    printf("DNS: query OK \"%s\" => \"%s\"\n", dns_test_hosts[i], data.addr.get_ip_address());
                } else if (data.result == NSAPI_ERROR_DNS_FAILURE) {
                    result_dns_failure++;
                    printf("DNS: query \"%s\" => DNS failure\n", dns_test_hosts[i]);
                } else if (data.result == NSAPI_ERROR_TIMEOUT) {
                    result_exp_timeout++;
                    printf("DNS: query \"%s\" => timeout\n", dns_test_hosts[i]);
                } else if (data.result == NSAPI_ERROR_NO_MEMORY) {
                    result_no_mem++;
                    printf("DNS: query \"%s\" => no memory\n", dns_test_hosts[i]);
                } else {
                    printf("DNS: query \"%s\" => %d, unexpected answer\n", dns_test_hosts[i], data.result);
                    TEST_ASSERT(data.result == NSAPI_ERROR_OK || data.result == NSAPI_ERROR_NO_MEMORY || data.result == NSAPI_ERROR_DNS_FAILURE || data.result == NSAPI_ERROR_TIMEOUT);
                }

            }
        }



    }
}
