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

#include "cmsis_os2.h"
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "spm_client.h"
#include "psa_client_common.h"
#include "neg_tests.h"


#define MINOR_VER               5
#define CLIENT_RSP_BUF_SIZE     128
#define OFFSET_POS              1
#define INVALID_SFID            (PART1_SF1 + 30)
#define INVALID_MINOR           (MINOR_VER + 10)
#define INVALID_TX_LEN          (PSA_MAX_INVEC_LEN + 1)


using namespace utest::v1;

Semaphore test_sem(0);
bool error_thrown = false;
uint8_t response_buf[CLIENT_RSP_BUF_SIZE];
extern "C" void spm_reboot(void);

void error(const char* format, ...)
{
    error_thrown = true;
    osStatus status = test_sem.release();
    MBED_ASSERT(status == osOK);
    while(1);
    PSA_UNUSED(status);
}

/* ------------------------------------- Functions ----------------------------------- */

static psa_handle_t client_ipc_tests_connect(uint32_t sfid, uint32_t minor_version)
{
    psa_handle_t handle = psa_connect( sfid,
                                       minor_version
                                     );

    TEST_ASSERT_TRUE(handle > 0);

    return handle;
}

static void client_ipc_tests_call( psa_handle_t handle,
                                   iovec *iovec_temp,
                                   size_t tx_len,
                                   size_t rx_len
                                 )
{
    error_t status = PSA_SUCCESS;
    memset(response_buf, 0, sizeof(response_buf));

    status = psa_call( handle,
                       iovec_temp,
                       tx_len,
                       response_buf,
                       rx_len
                     );

    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);
}

static void client_ipc_tests_close(psa_handle_t handle)
{
    error_t status = PSA_SUCCESS;
    status = psa_close(handle);

    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);
}

//Testing client call with an invalid SFID
void client_connect_invalid_sfid()
{
    psa_connect( INVALID_SFID,
                 MINOR_VER
               );

    TEST_FAIL_MESSAGE("client_connect_invalid_sfid negative test failed");
}

//Testing client connect version policy is RELAXED and minor version is bigger than the minimum version
void client_connect_invalid_pol_ver_relaxed()
{
    psa_connect( PART1_SF1,           //PART1_SF1 should have relaxed policy
                 INVALID_MINOR
               );

    TEST_FAIL_MESSAGE("client_connect_invalid_pol_ver_relaxed negative test failed");
}

//Testing client connect version policy is STRICT and minor version is different than the minimum version
void client_connect_invalid_pol_ver_strict()
{
    psa_connect( PART1_SF2,           //PART1_SF2 should have strict policy
                 INVALID_MINOR
               );

    TEST_FAIL_MESSAGE("client_connect_invalid_pol_ver_strict negative test failed");
}

//Testing client call num of iovec (tx_len) is bigger than max value allowed
void client_call_invalid_tx_len()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t data[2] = {1, 0};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{data, sizeof(data)},
                                           {data, sizeof(data)},
                                           {data, sizeof(data)}};

    client_ipc_tests_call(handle, iovec_temp, INVALID_TX_LEN, CLIENT_RSP_BUF_SIZE);

    TEST_FAIL_MESSAGE("client_call_invalid_tx_len negative test failed");
}

//Testing client call return buffer (rx_buff) is NULL and return buffer len is not 0
void client_call_rx_buff_null_rx_len_not_zero()
{
    psa_handle_t handle = 0;
    size_t rx_len = 1;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t data[2] = {1, 0};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{data, sizeof(data)},
                                           {data, sizeof(data)},
                                           {data, sizeof(data)}};

    psa_call( handle,
              iovec_temp,
              PSA_MAX_INVEC_LEN,
              NULL,
              rx_len
            );

    TEST_FAIL_MESSAGE("client_call_rx_buff_null_rx_len_not_zero negative test failed");
}

//Testing client call iovecs pointer is NULL and num of iovecs is not 0
void client_call_iovecs_null_tx_len_not_zero()
{
    psa_handle_t handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t response_buf[CLIENT_RSP_BUF_SIZE] = {0};

    psa_call( handle,
              NULL,
              PSA_MAX_INVEC_LEN,
              response_buf,
              CLIENT_RSP_BUF_SIZE
            );

    TEST_FAIL_MESSAGE("client_call_iovecs_null_tx_len_not_zero negative test failed");
}

//Testing client call iovec base
void client_call_iovec_base_null_len_not_zero()
{
    client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t data[2] = {1, 0};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{NULL, sizeof(data)},
                                           {data, sizeof(data)},
                                           {data, sizeof(data)}};

    client_ipc_tests_call(PSA_NULL_HANDLE, iovec_temp, PSA_MAX_INVEC_LEN, 0);

    TEST_FAIL_MESSAGE("client_call_iovec_base_null_len_not_zero negative test failed");
}

//Testing client call handle does not exist on the platform
void client_call_invalid_handle()
{
    psa_handle_t handle = 0, invalid_handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);
    invalid_handle = handle + 10;

    uint8_t data[2] = {1, 0};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{data, sizeof(data)},
                                           {data, sizeof(data)},
                                           {data, sizeof(data)}};

    client_ipc_tests_call(invalid_handle, iovec_temp, PSA_MAX_INVEC_LEN, 0);

    TEST_FAIL_MESSAGE("client_call_invalid_handle negative test failed");
}

//Testing client call handle is null (PSA_NULL_HANDLE)
void client_call_handle_is_null()
{
    client_ipc_tests_connect(PART1_SF1, MINOR_VER);

    uint8_t data[2] = {1, 0};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{data, sizeof(data)},
                                           {data, sizeof(data)},
                                           {data, sizeof(data)}};

    client_ipc_tests_call(PSA_NULL_HANDLE, iovec_temp, PSA_MAX_INVEC_LEN, 0);

    TEST_FAIL_MESSAGE("client_call_handle_is_null negative test failed");
}

//Testing client close handle does not exist on the platform
void client_close_invalid_handle()
{
    psa_handle_t handle = 0, invalid_handle = 0;

    handle = client_ipc_tests_connect(PART1_SF1, MINOR_VER);
    invalid_handle = handle + 10;

    uint8_t data[2] = {1, 0};

    iovec iovec_temp[PSA_MAX_INVEC_LEN] = {{data, sizeof(data)},
                                           {data, sizeof(data)},
                                           {data, sizeof(data)}};

    client_ipc_tests_call(handle, iovec_temp, PSA_MAX_INVEC_LEN, 0);

    client_ipc_tests_close(invalid_handle);

    TEST_FAIL_MESSAGE("client_close_invalid_handle negative test failed");
}


PSA_NEG_TEST(client_connect_invalid_sfid)
PSA_NEG_TEST(client_connect_invalid_pol_ver_relaxed)
PSA_NEG_TEST(client_connect_invalid_pol_ver_strict)
PSA_NEG_TEST(client_call_invalid_tx_len)
PSA_NEG_TEST(client_call_rx_buff_null_rx_len_not_zero)
PSA_NEG_TEST(client_call_iovecs_null_tx_len_not_zero)
PSA_NEG_TEST(client_call_iovec_base_null_len_not_zero)
PSA_NEG_TEST(client_call_invalid_handle)
PSA_NEG_TEST(client_call_handle_is_null)
PSA_NEG_TEST(client_close_invalid_handle)

utest::v1::status_t spm_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    spm_reboot();
    error_thrown = false;
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

// Test cases
Case cases[] = {
    Case("Testing client connect invalid sfid", PSA_NEG_TEST_NAME(client_connect_invalid_sfid), spm_case_teardown),
    Case("Testing client connect version policy relaxed invalid minor", PSA_NEG_TEST_NAME(client_connect_invalid_pol_ver_relaxed), spm_case_teardown),
    Case("Testing client connect version policy strict invalid minor", PSA_NEG_TEST_NAME(client_connect_invalid_pol_ver_strict), spm_case_teardown),
    Case("Testing client call invalid tx_len", PSA_NEG_TEST_NAME(client_call_invalid_tx_len), spm_case_teardown),
    Case("Testing client call rx_buff is NULL rx_len is not 0", PSA_NEG_TEST_NAME(client_call_rx_buff_null_rx_len_not_zero), spm_case_teardown),
    Case("Testing client call iovecs is NULL tx_len is not 0", PSA_NEG_TEST_NAME(client_call_iovecs_null_tx_len_not_zero), spm_case_teardown),
    Case("Testing client call iovec base NULL while iovec len not 0", PSA_NEG_TEST_NAME(client_call_iovec_base_null_len_not_zero), spm_case_teardown),
    Case("Testing client call handle does not exist", PSA_NEG_TEST_NAME(client_call_invalid_handle), spm_case_teardown),
    Case("Testing client call handle is PSA_NULL_HANDLE", PSA_NEG_TEST_NAME(client_call_handle_is_null), spm_case_teardown),
    Case("Testing client close handle does not exist", PSA_NEG_TEST_NAME(client_close_invalid_handle), spm_case_teardown)
};

utest::v1::status_t spm_setup(const size_t number_of_cases)
{
#ifndef NO_GREENTEA
    GREENTEA_SETUP(20, "default_auto");
#endif
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(spm_setup, cases);

int main()
{
    !Harness::run(specification);
    return 0;
}
