/* Copyright (c) 2017-2018 ARM Limited
 *
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

#ifndef __UVISOR_MBED_SPM_SERVER_TESTS_H__
#define __UVISOR_MBED_SPM_SERVER_TESTS_H__

typedef enum {
    START_TEST = 1,
    GET_TEST_RESULT = 2
} test_action_t;

typedef struct factorial_data {
    uint32_t count;
    uint32_t val;
} factorial_data_t;

typedef psa_status_t (*psa_test_server_side_func)(psa_status_t *);
#define PSA_TEST_ERROR (-1L)
#define PSA_TEST_CLIENT_NAME(name) psa_test_client_side_ ## name
#define PSA_TEST_SERVER_NAME(name) psa_test_server_side_ ## name

#define PSA_TEST_CLIENT(name) void PSA_TEST_CLIENT_NAME(name) (void)
#define PSA_TEST_SERVER(name) psa_status_t PSA_TEST_SERVER_NAME(name) (psa_status_t *status_ptr)

#define PSA_TEST(name) \
    PSA_TEST_CLIENT(name); \
    PSA_TEST_SERVER(name); \


PSA_TEST(identity_during_connect)
PSA_TEST(identity_during_call)
PSA_TEST(get_msg_twice)
PSA_TEST(msg_size_assertion)
PSA_TEST(reject_connection)
PSA_TEST(read_at_outofboud_offset)
PSA_TEST(msg_read_truncation)
PSA_TEST(skip_zero)
PSA_TEST(skip_some)
PSA_TEST(skip_more_than_left)
PSA_TEST(rhandle_factorial)
PSA_TEST(cross_partition_call)
PSA_TEST(doorbell_test)
#endif /* __UVISOR_MBED_SPM_SERVER_TESTS_H__ */
