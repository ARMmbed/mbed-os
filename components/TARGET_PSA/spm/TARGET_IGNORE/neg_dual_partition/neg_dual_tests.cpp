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
#include "psa_neg_dual_part1_ifs.h"
#include "psa_neg_dual_part2_ifs.h"
#include "neg_tests.h"

#define MINOR_VER 5

using namespace utest::v1;

Semaphore test_sem(0);
bool error_thrown = false;
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

void server_call_sid_without_extern_sid()
{
    psa_handle_t handle = psa_connect(PART1_CALL_NON_EXISTS_EXTERN_SID, MINOR_VER);
    TEST_ASSERT_TRUE(handle > 0);

    psa_call(handle, NULL, 0, NULL, 0);
    TEST_FAIL_MESSAGE("psa_call from SP to SID not listed as extern_sids didn't fail");
}

void server_call_sid_in_same_partition()
{
    psa_handle_t handle = psa_connect(PART2_CALL_INSIDE_PARTITION, MINOR_VER);
    TEST_ASSERT_TRUE(handle > 0);

    psa_call(handle, NULL, 0, NULL, 0);
    TEST_FAIL_MESSAGE("psa_call from SP to SID within same SP didn't fail");
}


PSA_NEG_TEST(server_call_sid_without_extern_sid)
PSA_NEG_TEST(server_call_sid_in_same_partition)

utest::v1::status_t spm_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    spm_reboot();
    error_thrown = false;
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

#define SPM_UTEST_CASE(desc, test) Case(desc, PSA_NEG_TEST_NAME(test), spm_case_teardown)

// Test cases
Case cases[] = {
    SPM_UTEST_CASE("Testing server calling sid without extern sid", server_call_sid_without_extern_sid),
    SPM_UTEST_CASE("Testing server calling sid within the same partition", server_call_sid_in_same_partition)
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
