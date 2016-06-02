/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
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

#include <stdio.h>
#include <stdlib.h>     /* for NULL for box_config.h */
#include <string.h>
#include <inttypes.h>
#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include <mbed-drivers/mbed.h>
#endif
#include "cfstore_config.h"
#include <flash-abstraction/Driver_Common.h>
#include "../../../source/cfstore_test.h"
#include "../../../source/cfstore_debug.h"
#include <configuration-store/configuration_store.h>
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */
#include "acl_box2.h"
//todo: restore #include "acl_box1.h"

using namespace utest::v1;

char cfstore_acl_box_2_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];


/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore-test.acl.box2");
UVISOR_BOX_CONFIG(cfstore_acl_box2, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


#ifdef noCodeUse

cfstore_kv_data_t cfstore_acl_box_2_test_01_data[] = {
    { "com.arm.mbed.cfstore-test.acl.box2.private-ro", "box_2_ro_private_data"},
    { "com.arm.mbed.cfstore-test.acl.box2.private-rw", "box_2_rw_private_data"},
    { "com.arm.mbed.cfstore-test.acl.box2.public-ro", "box_2_ro_public_data"},
    { "com.arm.mbed.cfstore-test.acl.box2.public-rw", "box_2_rw_public_data"},
    { NULL, NULL},
};

/* table of acl permissions for the above keys */
static ARM_CFSTORE_ACCESS_CONTROL_LIST cfstore_acl_box_2_test_01_table[] = {
    /* owner_R,     owner_W,    owner_X,    other_R,    other_W,    other_X,    reserved */
    {  true,        false,      false,      false,      false,      false,      0},
    {  true,        true,       false,      false,      false,      false,      0},
    {  true,        true,       false,      true,       false,      false,      0},
    {  true,        true,       false,      true,       true,       false,      0},
    {  false,       false,      false,      false,      false,      false,      0},

};

/* @brief   setup the config store attributes with specified permissions.
 *
 * - box2 uvisor context creates private RO attribute com.arm.mbed.cfstore-test.acl.box2.private-ro.
 * - box2 uvisor context creates private RW attribute com.arm.mbed.cfstore-test.acl.box2.private-rw.
 * - box2 uvisor context creates public RO attribute com.arm.mbed.cfstore-test.acl.box2.public-ro.
 * - box2 uvisor context creates public RW attribute com.arm.mbed.cfstore-test.acl.box2.public-rw.
 */
UVISOR_EXTERN int __cfstore_acl_box_2_test_02(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_KEYDESC kdesc;
    cfstore_kv_data_t* node = NULL;
    ARM_CFSTORE_ACCESS_CONTROL_LIST* acl_node = NULL;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    node = cfstore_acl_box_2_test_01_data;
    acl_node = cfstore_acl_box_2_test_01_table;
    while(node->key_name != NULL)
    {
        len = strlen(node->value);
        memset(&kdesc, 0, sizeof(kdesc));
        kdesc.drl = ARM_RETENTION_NVM;
        kdesc.acl.perm_owner_read = acl_node->perm_owner_read;
        kdesc.acl.perm_owner_write = acl_node->perm_owner_write;
        kdesc.acl.perm_owner_read = acl_node->perm_owner_read;
        kdesc.acl.perm_owner_execute = acl_node->perm_owner_execute;
        kdesc.acl.perm_other_read = acl_node->perm_other_read;
        kdesc.acl.perm_other_write = acl_node->perm_other_write;
        kdesc.acl.perm_other_execute = acl_node->perm_other_execute;

        ret = cfstore_test_create(node->key_name, (char*) node->value, &len, &kdesc);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_acl_box_2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create kv (key_name=%s.\n", __func__, node->key_name);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_acl_box_2_utest_msg_g);
        CFSTORE_LOG("Created KV successfully (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);
        node++;
        acl_node++;
    }
    return ret;
}

/* @brief   setup the config store attributes with specified permissions.
 * todo: implement the following list of tests
 * - test: box2 uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-ro, and verifies the open fails.
 * - test: box2 uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-rw, and verifies the open fails.
 * - test: box2 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box1.public-ro, and verifies the read succeeds.
 * - test: box2 uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box1.public-rw, and verifies the read/write succeeds.
 * - test: box2 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box2.private-ro, and verifies the read succeeds.
 * - test: box2 uvisor context tries to read/write com.arm.mbed.cfstore-test.ac2.box1.private-rw, and verifies the read/write succeed.
 * - test: box2 uvisor context tries to read com.arm.mbed.cfstore-test.acl.box2.public-ro, and verifies the read succeeds.
 * - test: box2 uvisor context tries to read/write com.arm.mbed.cfstore-test.acl.box2.public-rw, and verifies the read/write succeeds.
 */
UVISOR_EXTERN int32_t __cfstore_acl_box_2_test_04(void)
{
    //int32_t ret = ARM_DRIVER_ERROR;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    /* todo: do this test first
     * - test: box2 uvisor context tries to open com.arm.mbed.cfstore-test.acl.box1.private-rw, and verifies the open fails.*/
    // todo: restore return ret;
    // temporary measure
    return ARM_DRIVER_OK;
}


#ifdef YOTTA_CFG_CFSTORE_UVISOR

/* uvisor enabled: call function via sec gw */
int32_t cfstore_acl_box_2_test_02(void)
{
    /* do the test as box 1 */
    return secure_gateway(cfstore_acl_box2, __cfstore_acl_box_2_test_02);
}

int32_t cfstore_acl_box_2_test_04(void)
{
    /* do the test as box 1 */
    return secure_gateway(cfstore_acl_box2, __cfstore_acl_box_2_test_04);
}

#else

/* uvisor disabled: call function without sec gw */
int32_t cfstore_acl_box_2_test_02(void)
{
    /* do the test as box 1 */
    return __cfstore_acl_box_2_test_02();
}

int32_t cfstore_acl_box_2_test_04(void)
{
    /* do the test as box 1 */
    return __cfstore_acl_box_2_test_04();
}

#endif // noCodeUse


#endif /* YOTTA_CFG_CFSTORE_UVISOR */
