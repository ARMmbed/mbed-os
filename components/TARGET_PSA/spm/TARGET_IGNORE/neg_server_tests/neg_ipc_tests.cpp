/* Copyright (c) 2017-2018 ARM Limited
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
#include "psa_neg_server_part1_ifs.h"
#include "psa_neg_server_part2_ifs.h"
#include "neg_tests.h"


#define MINOR_VER               5
#define CLIENT_RSP_BUF_SIZE     128
#define OFFSET_POS              1
#define INVALID_SID             (PART1_ROT_SRV1 + 30)
#define INVALID_MINOR           (MINOR_VER + 10)

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

static psa_handle_t negative_server_ipc_tests_connect(uint32_t sid, uint32_t minor_version)
{
    psa_handle_t handle = psa_connect(sid, minor_version);
    TEST_ASSERT_TRUE(handle > 0);
    return handle;
}

static void negative_server_ipc_tests_call(psa_handle_t handle,
                                           psa_invec_t *invec,
                                           size_t tx_len,
                                           psa_outvec_t *outvec,
                                           size_t rx_len
                                          )
{
    error_t status = PSA_SUCCESS;

    status = psa_call(handle, invec, tx_len, outvec, rx_len);

    TEST_ASSERT_EQUAL_INT(PSA_SUCCESS, status);
}

//Testing server interrupt mask contains only a subset of interrupt signal mask
void server_interrupt_mask_invalid()
{
    psa_connect(PART2_INT_MASK, MINOR_VER);

    TEST_FAIL_MESSAGE("server_interrupt_mask_invalid negative test failed at client side");
}

//Testing server get with msg NULL
void server_get_msg_null()
{
    psa_connect(PART2_GET_MSG_NULL, MINOR_VER);

    TEST_FAIL_MESSAGE("server_get_msg_null negative test failed at client side");
}

//Testing server get signum have more than one bit ON
void server_get_multiple_bit_signum()
{
    psa_connect(PART2_GET_SIGNUM_MULTIPLE_BIT, MINOR_VER);

    TEST_FAIL_MESSAGE("server_get_multiple_bit_signum negative test failed at client side");
}

//Testing server get signum is not a subset of current partition flags
void server_get_signum_not_subset()
{
    psa_connect(PART2_GET_SIGNUM_NOT_SUBSET, MINOR_VER);

    TEST_FAIL_MESSAGE("server_get_signum_not_subset negative test failed at client side");
}

//Testing server get signum flag is not active
void server_get_signum_not_active()
{
    psa_connect(PART2_GET_SIGNUM_NOT_ACTIVE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_get_signum_not_active negative test failed at client side");
}

//Testing server get signum flag twice
void server_get_signum_twice()
{
    psa_connect(PART2_GET_SIGNUM_TWICE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_get_signum_twice negative test failed at client side");
}

//Testing server read handle does not exist on the platform
void server_read_invalid_handle()
{
    uint32_t data = 52;
    psa_invec_t data_vec = {&data, sizeof(data)};
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_READ_INVALID_HANDLE, MINOR_VER);

    negative_server_ipc_tests_call(handle, &data_vec, 1, NULL, 0);

    TEST_FAIL_MESSAGE("server_read_invalid_handle negative test failed at client side");
}

//Testing server read handle is null (PSA_NULL_HANDLE)
void server_read_null_handle()
{
    uint32_t data = 52;
    psa_invec_t data_vec = {&data, sizeof(data)};
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_READ_NULL_HANDLE, MINOR_VER);

    negative_server_ipc_tests_call(handle, &data_vec, 1, NULL, 0);

    TEST_FAIL_MESSAGE("server_read_null_handle negative test failed at client side");
}

//Testing server read buffer is null
void server_read_null_buffer()
{
    uint32_t data = 52;
    psa_invec_t data_vec = {&data, sizeof(data)};
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_READ_NULL_BUFFER, MINOR_VER);

    negative_server_ipc_tests_call(handle, &data_vec, 1, NULL, 0);

    TEST_FAIL_MESSAGE("server_read_null_buffer negative test failed at client side");
}

//Testing server write buffer is null
void server_write_null_buffer()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_WRITE_BUFFER_NULL, MINOR_VER);

    uint8_t data[2] = {1, 0};
    psa_invec_t iovec_temp[PSA_MAX_IOVEC - 1] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };
    psa_outvec_t resp = { NULL, CLIENT_RSP_BUF_SIZE };

    negative_server_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, &resp, 1);

    TEST_FAIL_MESSAGE("server_write_null_buffer negative test failed at client side");
}

//Testing server write rx_buff is null
void server_write_rx_buff_null()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_WRITE_RX_BUFF_NULL, MINOR_VER);

    uint8_t data[2] = {1, 0};
    psa_invec_t iovec_temp[PSA_MAX_IOVEC] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };

    negative_server_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC, NULL, 0);

    TEST_FAIL_MESSAGE("server_write_rx_buff_null negative test failed at client side");
}

//Testing server write handle does not exist on the platform
void server_write_invalid_handle()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_WRITE_INVALID_HANDLE, MINOR_VER);

    uint8_t data[2] = {1, 0};
    psa_invec_t iovec_temp[PSA_MAX_IOVEC - 1] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };
    psa_outvec_t resp = { response_buf, CLIENT_RSP_BUF_SIZE };

    negative_server_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, &resp, 1);

    TEST_FAIL_MESSAGE("server_write_invalid_handle negative test failed at client side");
}

//Testing server write handle is null (PSA_NULL_HANDLE)
void server_write_null_handle()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_WRITE_NULL_HANDLE, MINOR_VER);

    uint8_t data[2] = {1, 0};
    psa_invec_t iovec_temp[PSA_MAX_IOVEC - 1] = {
        {data, sizeof(data)},
        {data, sizeof(data)},
        {data, sizeof(data)}
    };
    psa_outvec_t resp = { response_buf, CLIENT_RSP_BUF_SIZE };

    negative_server_ipc_tests_call(handle, iovec_temp, PSA_MAX_IOVEC - 1, &resp, 1);

    TEST_FAIL_MESSAGE("server_write_null_handle negative test failed at client side");
}

//Testing server reply with a null handle
void server_reply_null_handle()
{
    psa_connect(PART2_REPLY_NULL_HANDLE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_reply_null_handle negative test failed at client side");
}

//Testing server reply handle does not exist on the platform
void server_reply_invalid_handle()
{
    psa_connect(PART2_REPLY_INVALID_HANDLE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_reply_invalid_handle negative test failed at client side");
}

//Testing server reply invalid retval for connect
void server_reply_invalid_retval_connect()
{
    psa_connect(PART1_REPLY_INVALID_RETVAL_CONNECT, MINOR_VER);

    TEST_FAIL_MESSAGE("server_reply_invalid_retval_connect negative test failed at client side");
}

//Testing server reply invalid retval for call
void server_reply_invalid_retval_call()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART1_REPLY_INVALID_RETVAL_CALL, MINOR_VER);

    psa_call(handle, NULL, 0, NULL, 0);

    TEST_FAIL_MESSAGE("server_reply_invalid_retval_call negative test failed at client side");
}

//Testing server notify partition id doesnt exist
void server_notify_part_id_invalid()
{
    psa_connect(PART2_NOTIFY_PART_ID_INVALID, MINOR_VER);

    TEST_FAIL_MESSAGE("server_notify_part_id_invalid negative test failed at client side");
}

//Testing server psa_identity handle does not exist on the platform
void server_psa_identity_invalid_handle()
{
    psa_connect(PART2_IDENTITY_INVALID_HANDLE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_psa_identity_invalid_handle negative test failed at client side");
}

//Testing server psa_identity handle is null (PSA_NULL_HANDLE)
void server_psa_identity_null_handle()
{
    psa_connect(PART2_IDENTITY_NULL_HANDLE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_psa_identity_null_handle negative test failed at client side");
}

//Testing server psa_set_rhandle handle does not exist on the platform
void server_set_rhandle_invalid_handle()
{
    psa_connect(PART2_SET_RHANDLE_INVALID_HANDLE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_set_rhandle_invalid_handle negative test failed at client side");
}

//Testing server psa_set_rhandle handle is null (PSA_NULL_HANDLE)
void server_set_rhandle_null_handle()
{
    psa_connect(PART2_SET_RHANDLE_NULL_HANDLE, MINOR_VER);

    TEST_FAIL_MESSAGE("server_set_rhandle_null_handle negative test failed at client side");
}

void server_read_on_wraparound_msg_ptr()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_READ_WRAPAROUND, MINOR_VER);
    psa_call(handle, NULL, 0, NULL, 0);

    TEST_FAIL_MESSAGE("server_read_on_wraparound_msg_ptr negative test failed at client side");
}

void server_read_from_excess_invec()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_READ_EXCESS_INVEC, MINOR_VER);
    psa_call(handle, NULL, 0, NULL, 0);

    TEST_FAIL_MESSAGE("server_read_from_excess_invec negative test failed at client side");
}

void server_write_on_wraparound_msg_ptr()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_WRITE_WRAPAROUND, MINOR_VER);
    psa_call(handle, NULL, 0, NULL, 0);

    TEST_FAIL_MESSAGE("server_write_on_wraparound_msg_ptr negative test failed at client side");
}

void server_write_from_excess_outvec()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_WRITE_EXCESS_OUTVEC, MINOR_VER);
    psa_call(handle, NULL, 0, NULL, 0);

    TEST_FAIL_MESSAGE("server_write_from_excess_outvec negative test failed at client side");
}

void server_write_from_outvec_index_size_0()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART1_WRITE_OUTVEC_IX_SIZE_0, MINOR_VER);

    psa_outvec_t resp[2] = { {response_buf, CLIENT_RSP_BUF_SIZE},
        {response_buf, 0}
    };

    psa_call(handle, NULL, 0, resp, 2);

    TEST_FAIL_MESSAGE("server_write_from_outvec_index_size_0 negative test failed at client side");
}

void server_write_with_size_overflow()
{
    psa_handle_t handle = negative_server_ipc_tests_connect(PART2_WRITE_SIZE_OVERFLOW, MINOR_VER);
    psa_outvec_t resp = { response_buf, CLIENT_RSP_BUF_SIZE };
    psa_call(handle, NULL, 0, &resp, 1);
    TEST_FAIL_MESSAGE("server_write_with_size_overflow negative test failed at client side");
}

// Testing server psa_clear() when doorbell signal is not currently asserted
void server_clear_no_doorbell()
{
    psa_connect(PART1_CLEAR_NO_DOORBELL, MINOR_VER);

    TEST_FAIL_MESSAGE("server_clear_no_doorbell negative test failed at client side");
}

PSA_NEG_TEST(server_interrupt_mask_invalid)
PSA_NEG_TEST(server_get_msg_null)
PSA_NEG_TEST(server_get_multiple_bit_signum)
PSA_NEG_TEST(server_get_signum_not_subset)
PSA_NEG_TEST(server_get_signum_not_active)
PSA_NEG_TEST(server_get_signum_twice)
PSA_NEG_TEST(server_read_invalid_handle)
PSA_NEG_TEST(server_read_null_handle)
PSA_NEG_TEST(server_read_null_buffer)
PSA_NEG_TEST(server_write_null_buffer)
PSA_NEG_TEST(server_write_rx_buff_null)
PSA_NEG_TEST(server_write_invalid_handle)
PSA_NEG_TEST(server_write_null_handle)
PSA_NEG_TEST(server_reply_null_handle)
PSA_NEG_TEST(server_reply_invalid_handle)
PSA_NEG_TEST(server_reply_invalid_retval_connect)
PSA_NEG_TEST(server_reply_invalid_retval_call)
PSA_NEG_TEST(server_notify_part_id_invalid)
PSA_NEG_TEST(server_psa_identity_invalid_handle)
PSA_NEG_TEST(server_psa_identity_null_handle)
PSA_NEG_TEST(server_set_rhandle_invalid_handle)
PSA_NEG_TEST(server_set_rhandle_null_handle)
PSA_NEG_TEST(server_read_on_wraparound_msg_ptr)
PSA_NEG_TEST(server_read_from_excess_invec)
PSA_NEG_TEST(server_write_on_wraparound_msg_ptr)
PSA_NEG_TEST(server_write_with_size_overflow)
PSA_NEG_TEST(server_write_from_excess_outvec)
PSA_NEG_TEST(server_write_from_outvec_index_size_0)
PSA_NEG_TEST(server_clear_no_doorbell)

utest::v1::status_t spm_case_setup(const Case *const source, const size_t index_of_case)
{
    memset(response_buf, 0, sizeof(response_buf));
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t spm_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    spm_reboot();
    error_thrown = false;
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

#define SPM_UTEST_CASE(desc, test) Case(desc, spm_case_setup, PSA_NEG_TEST_NAME(test), spm_case_teardown)

// Test cases
Case cases[] = {
    SPM_UTEST_CASE("Testing server interrupt mask invalid", server_interrupt_mask_invalid),
    SPM_UTEST_CASE("Testing server get with msg NULL", server_get_msg_null),
    SPM_UTEST_CASE("Testing server get signum have more than one bit ON", server_get_multiple_bit_signum),
    SPM_UTEST_CASE("Testing server get signum flag is not a subset of current partition flags", server_get_signum_not_subset),
    SPM_UTEST_CASE("Testing server get signum flag is not active", server_get_signum_not_active),
    SPM_UTEST_CASE("Testing server get signum twice", server_get_signum_twice),
    SPM_UTEST_CASE("Testing server read handle does not exist on the platform", server_read_invalid_handle),
    SPM_UTEST_CASE("Testing server read handle is PSA_NULL_HANDLE", server_read_null_handle),
    SPM_UTEST_CASE("Testing server read buffer is NULL", server_read_null_buffer),
    SPM_UTEST_CASE("Testing server write buffer is NULL", server_write_null_buffer),
    SPM_UTEST_CASE("Testing server write rx_buff is null", server_write_rx_buff_null),
    SPM_UTEST_CASE("Testing server write handle does not exist on the platform", server_write_invalid_handle),
    SPM_UTEST_CASE("Testing server write handle is PSA_NULL_HANDLE", server_write_null_handle),
    SPM_UTEST_CASE("Testing server reply with PSA_NULL_HANDLE", server_reply_null_handle),
    SPM_UTEST_CASE("Testing server reply handle does not exist on the platform", server_reply_invalid_handle),
    SPM_UTEST_CASE("Testing server reply invalid retval for connect", server_reply_invalid_retval_connect),
    SPM_UTEST_CASE("Testing server reply invalid retval for call", server_reply_invalid_retval_call),
    SPM_UTEST_CASE("Testing server notify partition id does not exist", server_notify_part_id_invalid),
    SPM_UTEST_CASE("Testing server identify handle does not exist on the platform", server_psa_identity_invalid_handle),
    SPM_UTEST_CASE("Testing server identify handle is PSA_NULL_HANDLE", server_psa_identity_null_handle),
    SPM_UTEST_CASE("Testing server set_rhandle handle does not exist on the platform", server_set_rhandle_invalid_handle),
    SPM_UTEST_CASE("Testing server set_rhandle handle is PSA_NULL_HANDLE", server_set_rhandle_null_handle),
    SPM_UTEST_CASE("Testing server read on wrap around pointer", server_read_on_wraparound_msg_ptr),
    SPM_UTEST_CASE("Testing server read on excess invec index", server_read_from_excess_invec),
    SPM_UTEST_CASE("Testing server write on wrap around pointer", server_write_on_wraparound_msg_ptr),
    SPM_UTEST_CASE("Testing server write on excess out_vec index", server_write_from_excess_outvec),
    SPM_UTEST_CASE("Testing server write on out_vec index with size 0", server_write_from_outvec_index_size_0),
    SPM_UTEST_CASE("Testing server write with size overflow", server_write_with_size_overflow),
    SPM_UTEST_CASE("Testing server clear when doorbell signal is not asserted", server_clear_no_doorbell)
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
