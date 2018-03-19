/* Copyright (c) 2017 ARM Limited
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

#if !ENABLE_SPM
    #error [NOT_SUPPORTED] SPM is not supported on this platform
#endif


#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "spm_client.h"
#include "psa_client_common.h"

using namespace utest::v1;

#define MINOR_VER               0
#define CLIENT_RSP_BUF_SIZE     128
#define OFFSET_POS              1


typedef struct th_struct {
    psa_handle_t handle;
    iovec *iovec_temp;
    uint8_t *expected;
    uint8_t expected_size;
} th_struct;

/* ------------------------------------- Functions ----------------------------------- */

static psa_handle_t client_ipc_tests_connect(uint32_t sfid, uint32_t minor_version)
{
    psa_handle_t handle = psa_connect( sfid,
                                       minor_version
                                     );

    TEST_ASSERT_TRUE(handle > 0);

    return handle;
}

static void client_ipc_tests_call(
        psa_handle_t handle,
        iovec *iovec_temp,
        size_t tx_len,
        size_t rx_len,
        uint8_t *expected,
        uint8_t expected_size
        )
{
    error_t status = PSA_SUCCESS;
    uint8_t *response_buf = (uint8_t*)malloc(CLIENT_RSP_BUF_SIZE * sizeof(uint8_t));
    memset(response_buf, 0, CLIENT_RSP_BUF_SIZE);

    status = psa_call( handle,
                        (tx_len ? iovec_temp : NULL),
                        tx_len,
                        (rx_len ? response_buf : NULL),
                        rx_len
                    );

    if (expected) {
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, response_buf, expected_size);
    }
    free(response_buf);
    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);
}

static void client_ipc_tests_close(psa_handle_t handle)
{
    error_t status = PSA_SUCCESS;
    status = psa_close(handle);

    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);
}

//Testing iovec 0 sent as NULL
void iovec_0_NULL()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t expect_size = 5;
    uint8_t off = 2;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5};
    uint8_t expected_buff[] = {1, 2, 3, 4, 5};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{NULL, 0},
                                           {meta_iovec, sizeof(meta_iovec)},
                                           {buff1, sizeof(buff1)}};

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

//Testing iovec 1 sent as NULL
void iovec_1_NULL()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t expect_size = 2;
    uint8_t off = 3;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5};
    uint8_t expected_buff[] = {2, 3};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{meta_iovec, sizeof(meta_iovec)},
                                           {NULL, 0},
                                           {buff1, sizeof(buff1)}};

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

//Testing iovec 2 sent as NULL
void iovec_2_NULL()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t expect_size = 2;
    uint8_t off = 3;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5};
    uint8_t expected_buff[] = {2, 3};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{meta_iovec, sizeof(meta_iovec)},
                                           {buff1, sizeof(buff1)},
                                           {NULL, 0}};

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

//Testing rx_buff sent as NULL and rx_len as 0
void rx_buff_null()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t expect_size = 0, off = 2;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3, 4, 5}, buff2[] = {6};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{meta_iovec, sizeof(meta_iovec)},
                                           {buff1, sizeof(buff1)},
                                           {buff2, sizeof(buff2)}};

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, 0, NULL, 0);

    client_ipc_tests_close(handle);
}

//Testing tx_buff sent as NULL and tx_len as 0
void tx_buff_null()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    client_ipc_tests_call(handle, NULL, 0, CLIENT_RSP_BUF_SIZE, NULL, 0);

    client_ipc_tests_close(handle);
}

//Testing rx_buff and tx_null sent as NULL and rx_len and tx_len as 0
void rx_tx_null()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    client_ipc_tests_call(handle, NULL, 0, 0, NULL, 0);

    client_ipc_tests_close(handle);
}

//Testing multiple subsequent calls to the same SFID
void multiple_call()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t expect_size = 2, off = 2;

    uint8_t meta_iovec[2] = {expect_size, off};
    uint8_t buff1[] = {1, 2, 3};
    uint8_t buff2[] = {4, 5, 6};
    uint8_t expected_buff[] = {1, 2};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{meta_iovec, sizeof(meta_iovec)},
                                           {buff1, sizeof(buff1)},
                                           {buff2, sizeof(buff2)}};

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    meta_iovec[1] = 3;              //off
    iovec_temp[0].iov_base = meta_iovec;
    expected_buff[0] = 2;
    expected_buff[1] = 3;

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    meta_iovec[1] = 4;              //off
    iovec_temp[0].iov_base = meta_iovec;
    expected_buff[0] = 3;
    expected_buff[1] = 4;

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, CLIENT_RSP_BUF_SIZE, expected_buff, sizeof(expected_buff));

    client_ipc_tests_close(handle);
}

static void set_struct(th_struct *thr_attr, psa_handle_t handle, iovec *iovec_temp, uint8_t *expect, uint8_t expected_size)
{
    thr_attr->handle = handle;
    thr_attr->iovec_temp = iovec_temp;
    thr_attr->expected = expect;
    thr_attr->expected_size = expected_size;
}

static void call_diff_handle(th_struct *thr_attr)
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    client_ipc_tests_call(handle,
                          thr_attr->iovec_temp,
                          PSA_MAX_INVEC_LEN,
                          CLIENT_RSP_BUF_SIZE,
                          thr_attr->expected,
                          thr_attr->expected_size);

    osDelay(10);

    client_ipc_tests_close(handle);
}

//Testing multiple parallel calls to the same SFID with different handles
void multi_thread_diff_handles()
{
    Thread T1, T2, T3;
    th_struct thr_attr[] = {{0}, {0}, {0}};

    uint8_t meta_iovec_1[] = { 2,            //expect_size
                               2             //off
                             };
    uint8_t buff1[] = {1, 2, 3};
    uint8_t buff2[] = {4, 5, 6};
    uint8_t expected_buff_1[] = {1, 2};

    iovec iovec_temp_1[PSA_MAX_INVEC_LEN] = {{meta_iovec_1, sizeof(meta_iovec_1)},
                                           {buff1, sizeof(buff1)},
                                           {buff2, sizeof(buff2)}};

    set_struct(&thr_attr[0], 0, iovec_temp_1, expected_buff_1, sizeof(expected_buff_1));
    osStatus err = T1.start(callback(call_diff_handle, (th_struct *)&thr_attr[0]));
    if (err) {
       TEST_FAIL_MESSAGE("creating thread failed!");
    }

    uint8_t meta_iovec_2[] = { 2,            //expect_size
                               3             //off
                             };
    uint8_t expected_buff_2[] = {2, 3};

    iovec iovec_temp_2[PSA_MAX_INVEC_LEN] = {{meta_iovec_2, sizeof(meta_iovec_2)},
                                           {buff1, sizeof(buff1)},
                                           {buff2, sizeof(buff2)}};
    set_struct(&thr_attr[1], 0, iovec_temp_2, expected_buff_2, sizeof(expected_buff_2));
    err = T2.start(callback(call_diff_handle, (th_struct *)&thr_attr[1]));
    if (err) {
       TEST_FAIL_MESSAGE("creating thread failed!");
    }

    uint8_t meta_iovec_3[] = { 2,            //expect_size
                               4             //off
                             };
    uint8_t expected_buff_3[] = {3, 4};

    iovec iovec_temp_3[PSA_MAX_INVEC_LEN] = {{meta_iovec_3, sizeof(meta_iovec_3)},
                                           {buff1, sizeof(buff1)},
                                           {buff2, sizeof(buff2)}};

    set_struct(&thr_attr[2], 0, iovec_temp_3, expected_buff_3, sizeof(expected_buff_3));
    err = T3.start(callback(call_diff_handle, (th_struct *)&thr_attr[2]));
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

static void call_same_handle(th_struct *thr_attr)
{
    client_ipc_tests_call(thr_attr->handle,
                          thr_attr->iovec_temp,
                          PSA_MAX_INVEC_LEN,
                          CLIENT_RSP_BUF_SIZE,
                          thr_attr->expected,
                          thr_attr->expected_size);
    osDelay(10);
}

//Testing multiple parallel calls to the same SFID with the same handle
void multi_thread_same_handle()
{
    Thread T1, T2, T3;
    th_struct thr_attr[] = {{0}, {0}, {0}};

    psa_handle_t handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t meta_iovec_1[] = { 2,            //expect_size
                               2             //off
                             };
    uint8_t buff1[] = {1, 2, 3};
    uint8_t buff2[] = {4, 5, 6};
    uint8_t expected_buff_1[] = {1, 2};

    iovec iovec_temp_1[PSA_MAX_INVEC_LEN] = {{meta_iovec_1, sizeof(meta_iovec_1)},
                                           {buff1, sizeof(buff1)},
                                           {buff2, sizeof(buff2)}};

    set_struct(&thr_attr[0], handle, iovec_temp_1, expected_buff_1, sizeof(expected_buff_1));
    osStatus err = T1.start(callback(call_same_handle, (th_struct *)&thr_attr[0]));
    if (err) {
       TEST_FAIL_MESSAGE("creating thread failed!");
    }

    uint8_t meta_iovec_2[] = { 2,            //expect_size
                               3             //off
                             };
    uint8_t expected_buff_2[] = {2, 3};

    iovec iovec_temp_2[PSA_MAX_INVEC_LEN] = {{meta_iovec_2, sizeof(meta_iovec_2)},
                                            {buff1, sizeof(buff1)},
                                            {buff2, sizeof(buff2)}};
    set_struct(&thr_attr[1], handle, iovec_temp_2, expected_buff_2, sizeof(expected_buff_2));
    err = T2.start(callback(call_same_handle, (th_struct *)&thr_attr[1]));
    if (err) {
       TEST_FAIL_MESSAGE("creating thread failed!");
    }

    uint8_t meta_iovec_3[] = { 2,            //expect_size
                               4             //off
                             };
    uint8_t expected_buff_3[] = {3, 4};

    iovec iovec_temp_3[PSA_MAX_INVEC_LEN] = {{meta_iovec_3, sizeof(meta_iovec_3)},
                                            {buff1, sizeof(buff1)},
                                            {buff2, sizeof(buff2)}};
    set_struct(&thr_attr[2], handle, iovec_temp_3, expected_buff_3, sizeof(expected_buff_3));
    err = T3.start(callback(call_same_handle, (th_struct *)&thr_attr[2]));
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

    client_ipc_tests_close(handle);
}

//Testing exceeding num of max channels allowed by psa_connect
void exceed_num_of_max_channels()
{
    int i = 0;
    psa_handle_t handle[MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS + 1] = {0};

    for (i = 0; i < MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS + 1; i++) {
        if (i != MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS) {
            handle[i] = client_ipc_tests_connect(PART1_SF1, MINOR_VER);
        }
        else {
            handle[i] = psa_connect(PART1_SF1, MINOR_VER);
            TEST_ASSERT_EQUAL_INT32(PSA_CONNECTION_REFUSED_BUSY, handle[i]);
        }
    }

    for (i = 0; i < MBED_CONF_SPM_IPC_MAX_NUM_OF_CHANNELS; i++) {
        client_ipc_tests_close(handle[i]);
    }
}


 // Test cases
Case cases[] = {
    Case("Testing client iovec_0_NULL", iovec_0_NULL),
    Case("Testing client iovec_1_NULL", iovec_1_NULL),
    Case("Testing client iovec_2_NULL", iovec_2_NULL),
    Case("Testing client rx_buff_null", rx_buff_null),
    Case("Testing client tx_buff_null", tx_buff_null),
    Case("Testing client rx_tx_null", rx_tx_null),
    Case("Testing client multiple_call from a single thread", multiple_call),
    Case("Testing client multiple calls on different channels to the same SFID", multi_thread_diff_handles),
    Case("Testing client multiple calls on the same channel to the same SFID", multi_thread_same_handle),
    Case("Testing client exceed num of max channels allowed", exceed_num_of_max_channels),
};

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    // Setup Greentea using a reasonable timeout in seconds
#ifndef NO_GREENTEA
    GREENTEA_SETUP(20, "default_auto");
#endif
    return verbose_test_setup_handler(number_of_cases);
}

Specification specification(test_setup, cases);

int main()
{
    !Harness::run(specification);
    return 0;
}
