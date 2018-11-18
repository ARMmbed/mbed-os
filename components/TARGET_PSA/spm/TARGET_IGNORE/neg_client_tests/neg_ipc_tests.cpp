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

#include "cmsis_os2.h"
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "spm_client.h"
#include "psa_neg_client_part1_ifs.h"
#include "neg_tests.h"


#define MINOR_VER               5
#define CLIENT_RSP_BUF_SIZE     128
#define OFFSET_POS              1
#define INVALID_SID             (NEG_CLIENT_PART1_ROT_SRV1 + 30)
#define INVALID_MINOR           (MINOR_VER + 10)
#define INVALID_TX_LEN          (PSA_MAX_IOVEC + 1)


using namespace utest::v1;

Semaphore test_sem(0);
bool error_thrown = false;
uint8_t response_buf[CLIENT_RSP_BUF_SIZE];
extern "C" void spm_reboot(void);

void error(const char *format, ...)
{
    error_thrown = true;
    osStatus status = test_sem.release();
    MBED_ASSERT(status == osOK);
    PSA_UNUSED(status);
    while (1);
}

/* ------------------------------------- Functions ----------------------------------- */

static psa_handle_t negative_client_ipc_tests_connect(uint32_t sid, uint32_t minor_version)
{
    psa_handle_t handle = psa_connect(sid, minor_version);
    TEST_ASSERT_TRUE(handle > 0);
    return handle;
}

static void negative_client_ipc_tests_call(psa_handle_t handle,
                                           psa_invec_t *iovec_temp,
                                           size_t tx_len,
                                           size_t rx_len
                                          )
{
    error_t status = PSA_SUCCESS;
    memset(response_buf, 0, sizeof(response_buf));
    psa_outvec_t resp = { response_buf, rx_len };

    status = psa_call(handle, iovec_temp, tx_len, &resp, 1);

    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);
}

static void negative_client_ipc_tests_close(psa_handle_t handle)
{
    psa_close(handle);

    // Wait for psa_close to finish on server side
    osDelay(50);
}

//Testing client call with an invalid SID
void client_connect_invalid_sid()
{
    psa_connect(INVALID_SID,
                MINOR_VER
               );

    TEST_FAIL_MESSAGE("client_connect_invalid_sid negative test failed");
}

//Testing client connect version policy is RELAXED and minor version is bigger than the minimum version
void client_connect_invalid_pol_ver_RELAXED()
{
    psa_connect(NEG_CLIENT_PART1_ROT_SRV1,            //NEG_CLIENT_PART1_ROT_SRV1 should have RELAXED policy
                INVALID_MINOR
               );

    TEST_FAIL_MESSAGE("client_connect_invalid_pol_ver_RELAXED negative test failed");
}

//Testing client connect version policy is STRICT and requeted version is higher than the minimum version
void client_connect_invalid_high_pol_ver_STRICT()
{
    psa_connect(NEG_CLIENT_PART1_ROT_SRV2,            //NEG_CLIENT_PART1_ROT_SRV2 should have STRICT policy
                INVALID_MINOR
               );

    TEST_FAIL_MESSAGE("client_connect_invalid_high_pol_ver_STRICT negative test failed");
}

//Testing client connect version policy is STRICT and requeted version equales the minimum version
void client_connect_invalid_equal_pol_ver_STRICT()
{
    psa_connect(NEG_CLIENT_PART1_ROT_SRV2,            //NEG_CLIENT_PART1_ROT_SRV2 should have STRICT policy
                MINOR_VER
               );

    TEST_FAIL_MESSAGE("client_connect_invalid_equal_pol_ver_STRICT negative test failed");
}

//Testing client call num of iovecs (tx_len + rx_len) is bigger than max value allowed
void client_call_invalid_iovecs_len()
{
    psa_handle_t handle = 0;

    handle = negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);

    uint8_t data[2] = {1, 0};

    psa_invec_t invec_temp[PSA_MAX_IOVEC] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    psa_outvec_t outvec_temp[1] = {
        {data, sizeof(data)}
    };

    // PSA_MAX_IOVEC invecs + 1 outvec
    psa_call(handle, invec_temp, PSA_MAX_IOVEC, outvec_temp, 1);

    TEST_FAIL_MESSAGE("client_call_invalid_iovecs_len negative test failed");
}

//Testing client call return buffer (rx_buff) is NULL and return buffer len is not 0
void client_call_rx_buff_null_rx_len_not_zero()
{
    psa_handle_t handle = 0;
    uint8_t data[2] = {1, 0};
    psa_invec_t iovec_temp[3] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    handle = negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);

    psa_call(handle, iovec_temp, 3, NULL, 1);

    TEST_FAIL_MESSAGE("client_call_rx_buff_null_rx_len_not_zero negative test failed");
}

//Testing client call iovecs pointer is NULL and num of iovecs is not 0
void client_call_iovecs_null_tx_len_not_zero()
{
    psa_handle_t handle = 0;

    handle = negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);
    memset(response_buf, 0, CLIENT_RSP_BUF_SIZE);
    psa_outvec_t resp = { response_buf, CLIENT_RSP_BUF_SIZE };

    psa_call(handle, NULL, 2, &resp, 1);

    TEST_FAIL_MESSAGE("client_call_iovecs_null_tx_len_not_zero negative test failed");
}

//Testing client call iovec base pointer is NULL and iovec size is not 0
void client_call_iovec_base_null_len_not_zero()
{
    negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);

    uint8_t data[2] = {1, 0};

    psa_invec_t iovec_temp[PSA_MAX_IOVEC - 1] = {
        {NULL, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    negative_client_ipc_tests_call(PSA_NULL_HANDLE, iovec_temp, PSA_MAX_IOVEC - 1, 0);

    TEST_FAIL_MESSAGE("client_call_iovec_base_null_len_not_zero negative test failed");
}

//Testing client call handle does not exist on the platform
void client_call_invalid_handle()
{
    psa_handle_t handle = 0, invalid_handle = 0;

    handle = negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);
    invalid_handle = handle + 10;

    uint8_t data[2] = {1, 0};

    psa_invec_t iovec_temp[PSA_MAX_IOVEC - 1] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    negative_client_ipc_tests_call(invalid_handle, iovec_temp, PSA_MAX_IOVEC - 1, 0);

    TEST_FAIL_MESSAGE("client_call_invalid_handle negative test failed");
}

//Testing client call handle is null (PSA_NULL_HANDLE)
void client_call_handle_is_null()
{
    negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);

    uint8_t data[2] = {1, 0};

    psa_invec_t iovec_temp[PSA_MAX_IOVEC - 1] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    negative_client_ipc_tests_call(PSA_NULL_HANDLE, iovec_temp, PSA_MAX_IOVEC - 1, 0);

    TEST_FAIL_MESSAGE("client_call_handle_is_null negative test failed");
}

//Testing client close handle does not exist on the platform
void client_close_invalid_handle()
{
    psa_handle_t handle = 0, invalid_handle = 0;

    handle = negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);
    invalid_handle = handle + 10;

    uint8_t data[2] = {1, 0};

    psa_invec_t iovec_temp[PSA_MAX_IOVEC - 1] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    negative_client_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, 0);

    negative_client_ipc_tests_close(invalid_handle);

    TEST_FAIL_MESSAGE("client_close_invalid_handle negative test failed");
}

void client_call_buffer_wrap_around()
{
    psa_handle_t handle = 0;
    psa_invec_t iovec_temp = { (void *)0x80000000, UINT32_MAX };

    handle = negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);
    psa_call(handle, &iovec_temp, 1, NULL, 0);

    TEST_FAIL_MESSAGE("client_call_buffer_wrap_around negative test failed");
}

void client_connect_not_allowed_from_nspe()
{
    psa_connect(NEG_CLIENT_PART1_ROT_SRV2, 5);

    TEST_FAIL_MESSAGE("client_connect_not_allowed_from_nspe negative test failed");
}

void client_call_excess_outvec()
{
    psa_handle_t handle = 0;
    uint8_t data[2] = {1, 0};
    psa_outvec_t iovec_temp[PSA_MAX_IOVEC + 1] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    handle = negative_client_ipc_tests_connect(NEG_CLIENT_PART1_ROT_SRV1, MINOR_VER);
    psa_call(handle, NULL, 0, iovec_temp, PSA_MAX_IOVEC + 1);

    TEST_FAIL_MESSAGE("client_call_excess_outvec negative test failed");
}

PSA_NEG_TEST(client_connect_invalid_sid)
PSA_NEG_TEST(client_connect_invalid_pol_ver_RELAXED)
PSA_NEG_TEST(client_connect_invalid_high_pol_ver_STRICT)
PSA_NEG_TEST(client_connect_invalid_equal_pol_ver_STRICT)
PSA_NEG_TEST(client_call_invalid_iovecs_len)
PSA_NEG_TEST(client_call_rx_buff_null_rx_len_not_zero)
PSA_NEG_TEST(client_call_iovecs_null_tx_len_not_zero)
PSA_NEG_TEST(client_call_iovec_base_null_len_not_zero)
PSA_NEG_TEST(client_call_invalid_handle)
PSA_NEG_TEST(client_call_handle_is_null)
PSA_NEG_TEST(client_close_invalid_handle)
PSA_NEG_TEST(client_call_buffer_wrap_around)
PSA_NEG_TEST(client_connect_not_allowed_from_nspe)
PSA_NEG_TEST(client_call_excess_outvec)

utest::v1::status_t spm_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    spm_reboot();
    error_thrown = false;
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

#define SPM_UTEST_CASE(desc, test) Case(desc, PSA_NEG_TEST_NAME(test), spm_case_teardown)

// Test cases
Case cases[] = {
    SPM_UTEST_CASE("Testing client connect invalid sid", client_connect_invalid_sid),
    SPM_UTEST_CASE("Testing client connect version policy RELAXED invalid minor", client_connect_invalid_pol_ver_RELAXED),
    SPM_UTEST_CASE("Testing client connect version policy STRICT high minor", client_connect_invalid_high_pol_ver_STRICT),
    SPM_UTEST_CASE("Testing client connect version policy STRICT equal minor", client_connect_invalid_equal_pol_ver_STRICT),
    SPM_UTEST_CASE("Testing client call invalid num of iovecs (tx_len + rx_len)", client_call_invalid_iovecs_len),
    SPM_UTEST_CASE("Testing client call rx_buff is NULL rx_len is not 0", client_call_rx_buff_null_rx_len_not_zero),
    SPM_UTEST_CASE("Testing client call iovecs is NULL tx_len is not 0", client_call_iovecs_null_tx_len_not_zero),
    SPM_UTEST_CASE("Testing client call iovec base NULL while iovec len not 0", client_call_iovec_base_null_len_not_zero),
    SPM_UTEST_CASE("Testing client call handle does not exist", client_call_invalid_handle),
    SPM_UTEST_CASE("Testing client call handle is PSA_NULL_HANDLE", client_call_handle_is_null),
    SPM_UTEST_CASE("Testing client close handle does not exist", client_close_invalid_handle),
    SPM_UTEST_CASE("Testing client call with buffer wrap-around", client_call_buffer_wrap_around),
    SPM_UTEST_CASE("Testing client connect to non-NSPE ROT_SRV", client_connect_not_allowed_from_nspe),
    SPM_UTEST_CASE("Testing client call with too much outvec's", client_call_excess_outvec)
};

utest::v1::status_t spm_setup(const size_t number_of_cases)
{
#ifndef NO_GREENTEA
    GREENTEA_SETUP(60, "default_auto");
#endif
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(spm_setup, cases);

int main()
{
    Harness::run(specification);
    return 0;
}
