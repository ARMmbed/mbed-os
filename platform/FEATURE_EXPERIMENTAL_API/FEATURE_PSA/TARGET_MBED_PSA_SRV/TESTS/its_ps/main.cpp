/*
* Copyright (c) 2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] ITS/PS test cases require RTOS to run.
#else

#ifndef TARGET_PSA
#error [NOT_SUPPORTED] ITS/PS tests can run only on PSA-enabled targets.
#else

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "psa/error.h"
#include "psa/storage_common.h"
#include "psa/internal_trusted_storage.h"
#include "psa/protected_storage.h"
#include "psa/lifecycle.h"
#include "KVMap.h"
#include "KVStore.h"
#include "kv_config.h"
#include "psa_storage_common_impl.h"
#include "DeviceKey.h"

using namespace utest::v1;
using namespace mbed;

#define TEST_BUFF_SIZE 16
#define STR_EXPAND(tok)                 #tok

typedef enum {
    its,
    ps
} storage_type_t;

extern "C" psa_status_t psa_ps_reset();

static psa_status_t set_func(storage_type_t stype, psa_storage_uid_t uid, size_t data_length,
                             const void *p_data, psa_storage_create_flags_t create_flags)
{
    return (stype == its) ?
           psa_its_set(uid, data_length, p_data, create_flags) :
           psa_ps_set(uid, data_length, p_data, create_flags);
}

static psa_status_t get_func(storage_type_t stype, psa_storage_uid_t uid, size_t data_offset,
                             size_t data_length, void *p_data, size_t *actual_length)
{
    return (stype == its) ?
           psa_its_get(uid, data_offset, data_length, p_data, actual_length) :
           psa_ps_get(uid, data_offset, data_length, p_data, actual_length);
}

static psa_status_t get_info_func(storage_type_t stype, psa_storage_uid_t uid,
                                  struct psa_storage_info_t *p_info)
{
    return (stype == its) ?
           psa_its_get_info(uid, p_info) :
           psa_ps_get_info(uid, p_info);
}

static psa_status_t remove_func(storage_type_t stype, psa_storage_uid_t uid)
{
    return (stype == its) ?
           psa_its_remove(uid) :
           psa_ps_remove(uid);
}


template <storage_type_t stype>
void pits_ps_test()
{
    psa_status_t status = PSA_SUCCESS;
    uint8_t write_buff[TEST_BUFF_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    uint8_t read_buff[TEST_BUFF_SIZE] = {0};
    size_t actual_size;
    psa_storage_create_flags_t flags;
    struct psa_storage_info_t info = {0, PSA_STORAGE_FLAG_WRITE_ONCE};
    memset(read_buff, 0, TEST_BUFF_SIZE);

    status = get_info_func(stype, 5, &info);
    TEST_ASSERT_EQUAL(PSA_ERROR_DOES_NOT_EXIST, status);

    status = set_func(stype, 5, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    status = get_info_func(stype, 5, &info);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(TEST_BUFF_SIZE, info.size);
    TEST_ASSERT_EQUAL(0, info.flags);

    status = get_func(stype, 5, 0, TEST_BUFF_SIZE, read_buff, &actual_size);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL_MEMORY(write_buff, read_buff, TEST_BUFF_SIZE);

    memset(read_buff, 0, TEST_BUFF_SIZE);
    status = get_func(stype, 5, 1, TEST_BUFF_SIZE, read_buff, &actual_size);
    TEST_ASSERT_NOT_EQUAL(PSA_SUCCESS, status);

    status = get_func(stype, 5, 1, TEST_BUFF_SIZE - 1, read_buff, &actual_size);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL_MEMORY(write_buff + 1, read_buff, TEST_BUFF_SIZE - 1);

    status = remove_func(stype, 5);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    status = get_info_func(stype, 5, &info);
    TEST_ASSERT_EQUAL(PSA_ERROR_DOES_NOT_EXIST, status);

    if (stype == its) {
        return;
    }

    mbed::KVMap &kv_map = mbed::KVMap::get_instance();
    mbed::KVStore *kvstore = kv_map.get_main_kv_instance(STR_EXPAND(MBED_CONF_STORAGE_DEFAULT_KV));
    uint32_t kv_get_flags;

    flags = PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION;
    status = set_func(stype, 6, TEST_BUFF_SIZE, write_buff, flags);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    status = get_info_func(stype, 6, &info);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(flags, info.flags);

    status = psa_storage_get_info_impl(kvstore, 1, 6, &info, &kv_get_flags);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(kv_get_flags, mbed::KVStore::REQUIRE_CONFIDENTIALITY_FLAG);

    flags = PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION | PSA_STORAGE_FLAG_NO_CONFIDENTIALITY | PSA_STORAGE_FLAG_WRITE_ONCE;
    status = set_func(stype, 6, TEST_BUFF_SIZE, write_buff, flags);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    status = get_info_func(stype, 6, &info);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(flags, info.flags);

    status = psa_storage_get_info_impl(kvstore, 1, 6, &info, &kv_get_flags);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(kv_get_flags, mbed::KVStore::WRITE_ONCE_FLAG);
}

template <storage_type_t stype>
void pits_ps_write_once_test()
{
    psa_status_t status = PSA_SUCCESS;
    uint8_t write_buff[TEST_BUFF_SIZE] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
    uint8_t read_buff[TEST_BUFF_SIZE] = {0};
    size_t actual_size;
    struct psa_storage_info_t info = {0, 0};

    status = get_info_func(stype, 5, &info);
    TEST_ASSERT_EQUAL(PSA_ERROR_DOES_NOT_EXIST, status);

    status = set_func(stype, 5, TEST_BUFF_SIZE, write_buff, PSA_STORAGE_FLAG_WRITE_ONCE);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);

    info.size = 0;
    info.flags = PSA_STORAGE_FLAG_WRITE_ONCE;
    status = get_info_func(stype, 5, &info);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(TEST_BUFF_SIZE, info.size);
    TEST_ASSERT_EQUAL(PSA_STORAGE_FLAG_WRITE_ONCE, info.flags);

    status = get_func(stype, 5, 0, TEST_BUFF_SIZE, read_buff, &actual_size);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(TEST_BUFF_SIZE, actual_size);
    TEST_ASSERT_EQUAL_MEMORY(write_buff, read_buff, TEST_BUFF_SIZE);

    status = set_func(stype, 5, TEST_BUFF_SIZE, write_buff, PSA_STORAGE_FLAG_WRITE_ONCE);
    TEST_ASSERT_EQUAL(PSA_ERROR_NOT_PERMITTED, status);

    status = set_func(stype, 5, TEST_BUFF_SIZE, write_buff, 0);
    TEST_ASSERT_EQUAL(PSA_ERROR_NOT_PERMITTED, status);

    status = remove_func(stype, 5);
    TEST_ASSERT_EQUAL(PSA_ERROR_NOT_PERMITTED, status);

    info.size = 0;
    info.flags = PSA_STORAGE_FLAG_WRITE_ONCE;
    status = get_info_func(stype, 5, &info);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    TEST_ASSERT_EQUAL(TEST_BUFF_SIZE, info.size);
    TEST_ASSERT_EQUAL(PSA_STORAGE_FLAG_WRITE_ONCE, info.flags);
}

utest::v1::status_t case_its_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    psa_status_t status;
    status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

template <storage_type_t stype>
utest::v1::status_t case_its_setup_handler(const Case *const source, const size_t index_of_case)
{
    psa_status_t status;
    if (stype == its) {
        status = mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
        TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    } else {
        status = psa_ps_reset();
        TEST_ASSERT_EQUAL(PSA_SUCCESS, status);
    }
#if DEVICEKEY_ENABLED
    DeviceKey::get_instance().generate_root_of_trust();
#endif
    return greentea_case_setup_handler(source, index_of_case);
}

Case cases[] = {
    Case("PSA prot internal storage - Basic", case_its_setup_handler<its>, pits_ps_test<its>, case_its_teardown_handler),
    Case("PSA prot internal storage - Write-once", case_its_setup_handler<its>, pits_ps_write_once_test<its>, case_its_teardown_handler),
#if COMPONENT_FLASHIAP
    Case("PSA protected storage - Basic", case_its_setup_handler<ps>, pits_ps_test<ps>),
    Case("PSA protected storage - Write-once", case_its_setup_handler<ps>, pits_ps_write_once_test<ps>)
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}

#endif // TARGET_PSA
#endif // !defined(MBED_CONF_RTOS_PRESENT)
