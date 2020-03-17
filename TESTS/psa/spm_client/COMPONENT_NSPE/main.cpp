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

#ifndef COMPONENT_PSA_SRV_IPC
#error [NOT_SUPPORTED] SPM tests can run only on SPM-enabled targets
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"

#if defined(TARGET_TFM)
#define MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS TFM_CONN_HANDLE_MAX_NUM
#define PSA_MAX_IOVEC 4
#endif

using namespace utest::v1;

#define MINOR_VER               0
#define DROP_CONN_MINOR_VER     5
#define CLIENT_RSP_BUF_SIZE     128
#define OFFSET_POS              1
#define INVALID_SID             0x00001A020


typedef struct th_struct {
    psa_handle_t handle;
    psa_invec *iovec_temp;
    uint8_t *expected;
    uint8_t expected_size;
} th_struct_t;

/* ------------------------------------- Functions ----------------------------------- */

static psa_handle_t client_ipc_tests_connect(uint32_t sid, uint32_t minor_version)
{
    psa_handle_t handle = psa_connect(sid, minor_version);

    TEST_ASSERT_TRUE(handle > 0);

    return handle;
}

static void client_ipc_tests_call(
    psa_handle_t handle,
    psa_invec *iovec_temp,
    size_t tx_len,
    size_t rx_len,
    uint8_t *expected,
    uint8_t expected_size
)
{
    psa_status_t status = PSA_SUCCESS;
    uint8_t *response_buf = (uint8_t *)malloc(CLIENT_RSP_BUF_SIZE * sizeof(uint8_t));
    memset(response_buf, 0, CLIENT_RSP_BUF_SIZE);
    psa_outvec resp = {NULL, rx_len};

    if (rx_len > 0) {
        resp.base = response_buf;
    }

    status = psa_call(handle,
                      (tx_len ? iovec_temp : NULL),
                      tx_len,
                      (rx_len ? &resp : NULL),
                      (rx_len ? 1 : 0)
                     );

    if (expected) {
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, response_buf, expected_size);
    }
    free(response_buf);
    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);
}

static void client_ipc_tests_close(psa_handle_t handle)
{
    psa_close(handle);
}

//Testing iovec 0 sent as NULL
void iovec_0_NULL()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    uint8_t expect_size = 5;
    uint8_t off = 2;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5};
    uint8_t expected_buff[] = {1, 2, 3, 4, 5};

    psa_invec iovec_temp[PSA_MAX_IOVEC - 1] = {
        {NULL, 0},
        {meta_iovec, sizeof(meta_iovec)},
        {buff1, sizeof(buff1)}
    };

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

//Testing iovec 1 sent as NULL
void iovec_1_NULL()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    uint8_t expect_size = 2;
    uint8_t off = 3;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5};
    uint8_t expected_buff[] = {2, 3};

    psa_invec iovec_temp[PSA_MAX_IOVEC - 1] = {{meta_iovec, sizeof(meta_iovec)},
        {NULL, 0},
        {buff1, sizeof(buff1)}
    };

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

//Testing iovec 2 sent as NULL
void iovec_2_NULL()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    uint8_t expect_size = 2;
    uint8_t off = 3;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5};
    uint8_t expected_buff[] = {2, 3};

    psa_invec iovec_temp[PSA_MAX_IOVEC - 1] = {{meta_iovec, sizeof(meta_iovec)},
        {buff1, sizeof(buff1)},
        {NULL, 0}
    };

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

// Testing in_vec[i] sent with size 0 and base not NULL
void in_vec_base_not_NULL_size_0()
{
    uint8_t     dummy_buff[]  = {1, 2, 3, 4, 5};
    psa_invec iovec_temp[1] = { {dummy_buff, 0} };

    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    client_ipc_tests_call(handle, iovec_temp, 1, 0, NULL, 0);

    client_ipc_tests_close(handle);
}

// Testing in_len is 0 but in_vec is not NULL
void in_len_0_in_vec_not_NULL()
{
    uint8_t     dummy_buff[]  = {1, 2, 3, 4, 5};
    psa_invec iovec_temp[1] = { {dummy_buff, sizeof(dummy_buff)} };

    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    client_ipc_tests_call(handle, iovec_temp, 0, 0, NULL, 0);

    client_ipc_tests_close(handle);
}

// Testing out_len is 0 but out_vec is not NULL
void out_len_0_outvec_not_NULL()
{
    psa_status_t status = PSA_SUCCESS;

    uint8_t      dummy_res[10]  = {0};
    psa_outvec outvec_temp[1] = {{dummy_res, sizeof(dummy_res)}};

    uint8_t dummy_buff[] = {1, 2, 3, 4, 5};

    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    psa_invec in_vec_temp[2] = { {dummy_buff, sizeof(dummy_buff)},
        {dummy_buff, sizeof(dummy_buff)}
    };

    status = psa_call(handle, in_vec_temp, 2, outvec_temp, 0);

    TEST_ASSERT_EQUAL_INT32(PSA_SUCCESS, status);

    client_ipc_tests_close(handle);
}

//Testing rx_buff sent as NULL and rx_len as 0
void rx_buff_null()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    uint8_t expect_size = 0, off = 2;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5}, buff2[] = {6};

    psa_invec iovec_temp[PSA_MAX_IOVEC - 1] = {{meta_iovec, sizeof(meta_iovec)},
        {buff1, sizeof(buff1)},
        {buff2, sizeof(buff2)}
    };

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, 0, NULL, 0);

    client_ipc_tests_close(handle);
}

//Testing tx_buff sent as NULL and tx_len as 0
void tx_buff_null()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    client_ipc_tests_call(handle, NULL, 0, CLIENT_RSP_BUF_SIZE, NULL, 0);

    client_ipc_tests_close(handle);
}

//Testing rx_buff and tx_null sent as NULL and rx_len and tx_len as 0
void rx_tx_null()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    client_ipc_tests_call(handle, NULL, 0, 0, NULL, 0);

    client_ipc_tests_close(handle);
}

//Testing multiple subsequent calls to the same SID
void multiple_call()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    uint8_t expect_size = 2, off = 2;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3};
    uint8_t buff2[] = {4, 5, 6};
    uint8_t expected_buff[] = {1, 2};

    psa_invec iovec_temp[PSA_MAX_IOVEC - 1] = {{meta_iovec, sizeof(meta_iovec)},
        {buff1, sizeof(buff1)},
        {buff2, sizeof(buff2)}
    };

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    meta_iovec[1] = 3;              //off
    iovec_temp[0].base = meta_iovec;
    expected_buff[0] = 2;
    expected_buff[1] = 3;

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    meta_iovec[1] = 4;              //off
    iovec_temp[0].base = meta_iovec;
    expected_buff[0] = 3;
    expected_buff[1] = 4;

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

static void set_struct(th_struct_t *thr_attr, psa_handle_t handle, psa_invec *iovec_temp, uint8_t *expect, uint8_t expected_size)
{
    thr_attr->handle = handle;
    thr_attr->iovec_temp = iovec_temp;
    thr_attr->expected = expect;
    thr_attr->expected_size = expected_size;
}

static void call_diff_handle(th_struct_t *thr_attr)
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);

    client_ipc_tests_call(handle,
                          thr_attr->iovec_temp,
                          PSA_MAX_IOVEC - 1,
                          CLIENT_RSP_BUF_SIZE,
                          thr_attr->expected,
                          thr_attr->expected_size);

    osDelay(10);

    client_ipc_tests_close(handle);
}

//Testing multiple parallel calls to the same SID with different handles
void multi_thread_diff_handles()
{
    Thread T1(osPriorityNormal, 512);
    Thread T2(osPriorityNormal, 512);
    Thread T3(osPriorityNormal, 512);

    th_struct_t thr_attr[] = {{0}, {0}, {0}};

    uint8_t meta_iovec_1[] = { 2,            //expect_size
                               2             //off
                             };
    uint8_t buff1[] = {1, 2, 3};
    uint8_t buff2[] = {4, 5, 6};
    uint8_t expected_buff_1[] = {1, 2};

    psa_invec iovec_temp_1[PSA_MAX_IOVEC - 1] = {{meta_iovec_1, sizeof(meta_iovec_1)},
        {buff1, sizeof(buff1)},
        {buff2, sizeof(buff2)}
    };

    set_struct(&thr_attr[0], 0, iovec_temp_1, expected_buff_1, sizeof(expected_buff_1));
    osStatus err = T1.start(callback(call_diff_handle, (th_struct_t *)&thr_attr[0]));
    if (err) {
        TEST_FAIL_MESSAGE("creating thread failed!");
    }

    uint8_t meta_iovec_2[] = { 2,            //expect_size
                               3             //off
                             };
    uint8_t expected_buff_2[] = {2, 3};

    psa_invec iovec_temp_2[PSA_MAX_IOVEC - 1] = {{meta_iovec_2, sizeof(meta_iovec_2)},
        {buff1, sizeof(buff1)},
        {buff2, sizeof(buff2)}
    };
    set_struct(&thr_attr[1], 0, iovec_temp_2, expected_buff_2, sizeof(expected_buff_2));
    err = T2.start(callback(call_diff_handle, (th_struct_t *)&thr_attr[1]));
    if (err) {
        TEST_FAIL_MESSAGE("creating thread failed!");
    }

    uint8_t meta_iovec_3[] = { 2,            //expect_size
                               4             //off
                             };
    uint8_t expected_buff_3[] = {3, 4};

    psa_invec iovec_temp_3[PSA_MAX_IOVEC - 1] = {{meta_iovec_3, sizeof(meta_iovec_3)},
        {buff1, sizeof(buff1)},
        {buff2, sizeof(buff2)}
    };

    set_struct(&thr_attr[2], 0, iovec_temp_3, expected_buff_3, sizeof(expected_buff_3));
    err = T3.start(callback(call_diff_handle, (th_struct_t *)&thr_attr[2]));
    if (err) {
        TEST_FAIL_MESSAGE("creating thread failed!");
    }

    err = T1.join();
    if (err) {
        TEST_FAIL_MESSAGE("joining thread failed!");
    }
    err = T2.join();
    if (err) {
        TEST_FAIL_MESSAGE("joining thread failed!");
    }
    err = T3.join();
    if (err) {
        TEST_FAIL_MESSAGE("joining thread failed!");
    }

}


//Testing exceeding num of max channels allowed by psa_connect
void exceed_num_of_max_channels()
{
    int i = 0;
    psa_handle_t handle[MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS + 1] = {0};

    for (i = 0; i < MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS + 1; i++) {
        if (i != MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS) {
            handle[i] = client_ipc_tests_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);
        } else {
            handle[i] = psa_connect(CLIENT_TESTS_PART1_ROT_SRV1, MINOR_VER);
            TEST_ASSERT_EQUAL_INT32(PSA_CONNECTION_REFUSED, handle[i]);
        }
    }

    for (i = 0; i < MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS; i++) {
        client_ipc_tests_close(handle[i]);
    }
}

void client_close_null_handle()
{
    client_ipc_tests_close(PSA_NULL_HANDLE);
}

void drop_connection()
{
    psa_handle_t handle = client_ipc_tests_connect(CLIENT_TESTS_PART1_DROP_CONN, DROP_CONN_MINOR_VER);
    psa_status_t status = psa_call(handle, NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL_INT(PSA_DROP_CONNECTION, status);

    status = PSA_SUCCESS;
    status = psa_call(handle, NULL, 0, NULL, 0);
    TEST_ASSERT_EQUAL_INT(PSA_DROP_CONNECTION, status);

    client_ipc_tests_close(handle);
}

void verify_psa_framework_version()
{
    uint32_t ff_version = psa_framework_version();
    TEST_ASSERT_EQUAL_INT(PSA_FRAMEWORK_VERSION, ff_version);
}

void psa_version_existing()
{
    uint32_t rot_version = psa_version(CLIENT_TESTS_PART1_DROP_CONN);
    TEST_ASSERT_EQUAL_INT(DROP_CONN_MINOR_VER, rot_version);
}

void psa_version_non_existing()
{
    uint32_t rot_version = psa_version(INVALID_SID);
    TEST_ASSERT_EQUAL_INT(PSA_VERSION_NONE, rot_version);
}

void psa_version_secure_access_only()
{
    uint32_t rot_version = psa_version(CLIENT_TESTS_PART1_SECURE_CLIENTS_ONLY);
    TEST_ASSERT_EQUAL_INT(PSA_VERSION_NONE, rot_version);
}


// Test cases
Case cases[] = {
    Case("Testing client iovec_0_NULL", iovec_0_NULL),
    Case("Testing client iovec_1_NULL", iovec_1_NULL),
    Case("Testing client iovec_2_NULL", iovec_2_NULL),
    Case("Testing client in_vec 0 base not NULL size 0", in_vec_base_not_NULL_size_0),
    Case("Testing client in_len 0 in_vec not NULL", in_len_0_in_vec_not_NULL),
    Case("Testing client out_len is 0 but out_vec is not NULL", out_len_0_outvec_not_NULL),
    Case("Testing client rx_buff_null", rx_buff_null),
    Case("Testing client tx_buff_null", tx_buff_null),
    Case("Testing client rx_tx_null", rx_tx_null),
    Case("Testing client multiple_call from a single thread", multiple_call),
    Case("Testing client close on NULL handle", client_close_null_handle),
    Case("Testing DROP_CONNECTION State", drop_connection),
    Case("Testing client psa_framework_version() API", verify_psa_framework_version),
    Case("Testing client psa_version() API on existing SID", psa_version_existing),
    Case("Testing client psa_version() API on non-existing SID", psa_version_non_existing),
    Case("Testing client psa_version() API to a service that is not NSPE callable", psa_version_secure_access_only),
    Case("Testing client multiple calls on different channels to the same SID", multi_thread_diff_handles),
};

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    // Setup Greentea using a reasonable timeout in seconds
    GREENTEA_SETUP(60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Specification specification(test_setup, cases);

int main()
{
    Harness::run(specification);
    return 0;
}

#endif // COMPONENT_PSA_SRV_IPC
