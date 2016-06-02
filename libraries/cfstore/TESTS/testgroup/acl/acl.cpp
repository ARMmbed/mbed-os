/** @file acl.cpp
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 * Test cases to use the access control list to control permissions to
 * KVs in the CFSTORE.
 */

#if defined __MBED__ && ! defined TOOLCHAIN_GCC_ARM

#include <stdio.h>
int main()
{
    printf("Not implemented for ARM toolchain\n");
}

#else


#include <stdio.h>
#include <string.h>

#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include <mbed-drivers/mbed.h>
#endif

#include "cfstore_config.h"
#include "../../../source/cfstore_test.h"
#include "../../../source/cfstore_debug.h"
#include <flash-abstraction/Driver_Common.h>
#include <configuration-store/configuration_store.h>
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "../../../source/cfstore_utest.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#include "cfstore_uvisor.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */
#include "acl_box1.h"
//todo: restore #include "acl_box2.h"

using namespace utest::v1;

char cfstore_acl_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];


#ifdef YOTTA_CFG_CFSTORE_UVISOR
/* Create the main box ACL list for the application.
 * The main ACL gets inherited by all the other boxes
 */
CFSTORE_UVISOR_MAIN_ACL(cfstore_acl_uvisor_box_acl_g);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, cfstore_acl_uvisor_box_acl_g);
/* Note, there is no namespace or box config for the main box, so
 * *** dont *** use the following macros here:
 *  UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore-test.acl")
 *  UVISOR_BOX_CONFIG(cfstore_acl, UVISOR_BOX_STACK_SIZE);
 * Rather, use them is specific boxes.
 */
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/*
static control_t cfstore_acl_test_99(const size_t call_count)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    cfstore_acl_box_1_test_02();
    return CaseNext;
}
*/

/* Overview of test
 * - acl has 3 uvisor boxes:
 * 	- main, which has access to KVs with "other" permissions set
 * 	- box1, which owns the "com.arm.mbed.cfstore-test.acl.box1" name space
 * 	- box2, which owns the "com.arm.mbed.cfstore-test.acl.box2" name space
 *
 * The aim of the tests are as follows:
 * - box1 uvisor context creates private RO attribute com.arm.mbed.cfstore-test.acl.box1.private-ro.
 * - box1 uvisor context creates private RW attribute com.arm.mbed.cfstore-test.acl.box1.private-rw.
 * - box1 uvisor context creates public RO attribute com.arm.mbed.cfstore-test.acl.box1.public-ro.
 * - box1 uvisor context creates public RW attribute com.arm.mbed.cfstore-test.acl.box1.public-rw.
 * - box2 uvisor context creates private RO attribute com.arm.mbed.cfstore-test.acl.box2.private-ro.
 * - box2 uvisor context creates private RW attribute com.arm.mbed.cfstore-test.acl.box2.private-rw.
 * - box2 uvisor context creates public RO attribute com.arm.mbed.cfstore-test.acl.box2.public-ro.
 * - box2 uvisor context creates public RW attribute com.arm.mbed.cfstore-test.acl.box1.public-rw.
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-ro, and verifies the open fails.
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-rw, and verifies the open fails.
 * - test: main uvisor context tries to read com.arm.mbed.cfstore-test.acl.box1.public-ro, and verifies the read succeeds.
 * - test: main uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box1.public-rw, and verifies the read/write succeeds.
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box2.private-ro, and verifies the open fails.
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box2.private-rw, and verifies the open fails.
 * - test: main uvisor context tries to read com.arm.mbed.cfstore-test.acl.box2.public-ro, and verifies the read succeeds.
 * - test: main uvisor context tries to create com.arm.mbed.cfstore-test.acl.box-main.public-rw, and verifies the create fails.
 * - test: main uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box2.public-rw, and verifies the read/write succeeds.
 * - test: box1 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box1.private-ro, and verifies the read succeeds.
 * - test: box1 uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box1.private-rw, and verifies the read/write succeed.
 * - test: box1 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box1.public-ro, and verifies the read succeeds.
 * - test: box1 uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box1.public-rw, and verifies the read/write succeeds.
 * - test: box1 uvisor context tries to open com.arm.mbed.cfstore-test.acl.box2.private-ro, and verifies the open fails.
 * - test: box1 uvisor context tries to open com.arm.mbed.cfstore-test.acl.box2.private-rw, and verifies the open fails.
 * - test: box1 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box2.public-ro, and verifies the read succeeds.
 * - test: box1 uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box2.public-rw, and verifies the read/write succeeds.
 * - test: box2 uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-ro, and verifies the open fails.
 * - test: box2 uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-rw, and verifies the open fails.
 * - test: box2 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box1.public-ro, and verifies the read succeeds.
 * - test: box2 uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box1.public-rw, and verifies the read/write succeeds.
 * - test: box2 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box2.private-ro, and verifies the read succeeds.
 * - test: box2 uvisor context tries to read/write com.arm.mbed.cfstore-test.ac2.box1.private-rw, and verifies the read/write succeed.
 * - test: box2 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box2.public-ro, and verifies the read succeeds.
 * - test: box2 uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box2.public-rw, and verifies the read/write succeeds.
 */
static cfstore_kv_data_t cfstore_acl_test_01_data[] = {
        CFSTORE_INIT_1_TABLE_MID_NODE,
        { NULL, NULL},
};


/**
 * @brief	basic check to see if cfstore-uvisor integration is working i.e. that
 * 			secure_gateway() calls can change from the insecure to the secure
 * 			context. This test is a copy of the open() test_02 i.e.:
 * 			test to open() a pre-existing key and try to write it, which should fail
 *          as by default pre-existing keys are opened read-only
 *
 * Basic open test which does the following:
 * - creates KV with default rw perms and writes some data to the value blob.
 * - closes the newly created KV.
 * - opens the KV with the default permissions (read-only)
 * - tries to write the KV data which should fail because KV was not opened with write flag set.
 * - closes the opened key
 *
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret < ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
static control_t cfstore_acl_test_01_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    // dont set any flags to get default settings
    memset(&flags, 0, sizeof(flags));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(cfstore_acl_test_01_data[0].value);
    ret = cfstore_test_create(cfstore_acl_test_01_data[0].key_name, (char*) cfstore_acl_test_01_data[0].value, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store (ret=%" PRId32 ").\n", __func__, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);

    // by default, owner of key opens with read-only permissions
    ret = drv->Open(cfstore_acl_test_01_data[0].key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (key_name=\"%s\")(ret=%" PRId32 ")\n", __func__, cfstore_acl_test_01_data[0].key_name, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);

    len = strlen(cfstore_acl_test_01_data[0].value);
    ret = drv->Write(hkey, cfstore_acl_test_01_data[0].value, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: call to Write() succeeded when should have failed (key_name=\"%s\")(ret=%" PRId32 ").\n", __func__, cfstore_acl_test_01_data[0].key_name, ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_READ_ONLY, cfstore_acl_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);
    return CaseNext;
}


/**
 * @brief call the boxes to perform creation of KV's in acl_box_1/2 in respective namespaces
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret < ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
/*
static control_t cfstore_acl_test_02_setup_kvs(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    ret = cfstore_acl_box_1_test_02();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: acl_box_1 failed to create attributes (ret=%" PRId32 ").\n", __func__, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);

    ret = cfstore_acl_box_2_test_02();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: acl_box_2 failed to create attributes (ret=%" PRId32 ").\n", __func__, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);

    return CaseNext;
}
*/

/**
 * @brief   acl_box_1 tests
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret < ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
/*
static control_t cfstore_acl_test_03(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    ret = cfstore_acl_box_1_test_03();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: acl_box_1 failed access tests (ret=%" PRId32 ").\n", __func__, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);
    return CaseNext;
}
*/
/**
 * @brief   acl_box_2 tests
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret < ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
/*
static control_t cfstore_acl_test_04(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    ret = cfstore_acl_box_2_test_04();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: acl_box_2 failed access tests (ret=%" PRId32 ").\n", __func__, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);
    return CaseNext;
}
*/
/**
 * @brief   main box tests
 * todo: implement this list of tests
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-ro, and verifies the open fails.
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-rw, and verifies the open fails.
 * - test: main uvisor context tries to read com.arm.mbed.cfstore-test.acl.box1.public-ro, and verifies the read succeeds.
 * - test: main uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box1.public-rw, and verifies the read/write succeeds.
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box2.private-ro, and verifies the open fails.
 * - test: main uvisor context tries to open com.arm.mbed.cfstore-test.acl.box2.private-rw, and verifies the open fails.
 * - test: main uvisor context tries to read com.arm.mbed.cfstore-test.acl.box2.public-ro, and verifies the read succeeds.
 * - test: main uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box2.public-rw, and verifies the read/write succeeds.
 * - test: main uvisor context tries to create com.arm.mbed.cfstore-test.acl.box-main.public-rw, and verifies the create fails.
 *
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret < ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
/*
static control_t cfstore_acl_test_05(const size_t call_count)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    return CaseNext;
}
*/
/**
 * @brief
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret < ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
/*
static control_t cfstore_acl_test_deinit(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_utest_msg_g);
    return CaseNext;
}
*/

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    // Call the default reporting function-
    GREENTEA_SETUP(CFSTORE_MBED_HOSTTEST_TIMEOUT, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
//        Case("CFSTORE_ACL_test_99", cfstore_acl_test_99),
        Case("CFSTORE_ACL_test_01_start", cfstore_utest_default_start),
        Case("CFSTORE_ACL_test_01_end", cfstore_acl_test_01_end),
        /*
        Case("CFSTORE_ACL_test_01_start", cfstore_utest_default_start),
        Case("CFSTORE_ACL_test_01_end", cfstore_acl_test_01_end),
        Case("CFSTORE_ACL_test_02_start", cfstore_utest_default_start),
        Case("CFSTORE_ACL_test_02_setup_KVs", cfstore_acl_test_02_setup_kvs),
        Case("CFSTORE_ACL_test_03_acl_box_1", cfstore_acl_test_03),
        Case("CFSTORE_ACL_test_04_acl_box_2", cfstore_acl_test_04),
        Case("CFSTORE_ACL_test_05_main_box", cfstore_acl_test_05),
        Case("CFSTORE_ACL_test_06_end", cfstore_acl_test_deinit),
        */
};


/* Declare your test specification with a custom setup handler*/
Specification specification(greentea_setup, cases);

#if defined CFSTORE_CONFIG_MBED_OS_VERSION && CFSTORE_CONFIG_MBED_OS_VERSION == 3
/* mbedosV3*/
void app_start(int argc __unused, char** argv __unused)
{
    /* Run the test specification */
    Harness::run(specification);
}
#endif /* CFSTORE_CONFIG_MBED_OS_VERSION == 3 */

#if defined CFSTORE_CONFIG_MBED_OS_VERSION && CFSTORE_CONFIG_MBED_OS_VERSION == 4
/* mbedosV3++*/
int main()
{
    return !Harness::run(specification);
}
#endif /* CFSTORE_CONFIG_MBED_OS_VERSION == 4 */


#endif // __MBED__ && ! defined TOOLCHAIN_GCC_ARM
