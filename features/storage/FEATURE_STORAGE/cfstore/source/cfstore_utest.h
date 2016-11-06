/** @file cfstore_test.h
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
 * Header file for test support data structures and function API.
 */
#ifndef __CFSTORE_UTEST_H
#define __CFSTORE_UTEST_H

#include "cfstore_debug.h"
#include "cfstore_test.h"
#include <inttypes.h>

using namespace utest::v1;

void cfstore_utest_default_callback(int32_t status, ARM_CFSTORE_OPCODE cmd_code, void *client_context, ARM_CFSTORE_HANDLE handle)
{
    (void) status;
    (void) client_context;
    (void) handle;

    CFSTORE_FENTRYLOG("%s:entered: status=%d, cmd_code=%d (%s) handle=%p\n", __func__, (int) status, (int) cmd_code, cfstore_test_opcode_str[cmd_code], handle);
    switch(cmd_code)
    {
    case CFSTORE_OPCODE_INITIALIZE:
    case CFSTORE_OPCODE_FLUSH:
    case CFSTORE_OPCODE_UNINITIALIZE:
    case CFSTORE_OPCODE_CLOSE:
    case CFSTORE_OPCODE_CREATE:
    case CFSTORE_OPCODE_DELETE:
    case CFSTORE_OPCODE_FIND:
    case CFSTORE_OPCODE_GET_KEY_NAME:
    case CFSTORE_OPCODE_GET_STATUS:
    case CFSTORE_OPCODE_GET_VALUE_LEN:
    case CFSTORE_OPCODE_OPEN:
    case CFSTORE_OPCODE_POWER_CONTROL:
    case CFSTORE_OPCODE_READ:
    case CFSTORE_OPCODE_RSEEK:
    case CFSTORE_OPCODE_WRITE:
    default:
        CFSTORE_DBGLOG("%s:debug: received asynchronous notification for opcode=%d (%s)", __func__, cmd_code, cmd_code < CFSTORE_OPCODE_MAX ? cfstore_test_opcode_str[cmd_code] : "unknown");
    }
    CFSTORE_DBGLOG("%s:about to validate callback\n", __func__);
    Harness::validate_callback();
    return;
}


static control_t cfstore_utest_default_start(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    char cfstore_utest_msg[CFSTORE_UTEST_MSG_BUF_SIZE];

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    ret = drv->Initialize(cfstore_utest_default_callback, NULL);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialize CFSTORE (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg);
    return CaseTimeout(CFSTORE_UTEST_DEFAULT_TIMEOUT_MS);
}

#endif /* __CFSTORE_UTEST_H */
