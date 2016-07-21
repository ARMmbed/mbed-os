/*
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
 */

/** @file flash_set.cpp Test tool to set flash to some data
 */

#include "mbed.h"
#include "cfstore_config.h"
#include "Driver_Common.h"
#include "cfstore_debug.h"
#include "cfstore_test.h"
#include "configuration_store.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "cfstore_utest.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#include "cfstore_uvisor.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

#ifdef YOTTA_CFG_CFSTORE_UVISOR
/* Create the main box ACL list for the application.
 * The main ACL gets inherited by all the other boxes
 */
CFSTORE_UVISOR_MAIN_ACL(cfstore_acl_uvisor_box_flash_set_g);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, cfstore_acl_uvisor_box_flash_set_g);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */



/**
 * @brief   add ~50 KVs and store them in flash
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t cfstore_flash_set_test_01_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_FMODE flags;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_CAPABILITIES caps = cfstore_driver.GetCapabilities();

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&flags, 0, sizeof(flags));

    CFSTORE_LOG("caps.asynchronous_ops : %d\n", (int) caps.asynchronous_ops);
    ret = cfstore_test_init_1();
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to write data to falsh.", __func__);
    }
    ret = drv->Flush();
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Flush() call failed (ret=%d).\r\n", __func__, (int) ret);
    }
#ifdef CFSTORE_DEBUG
    ret = cfstore_test_dump();
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("Error: failed to dump CFSTORE contents%s", "\n");
    }
#endif /* CFSTORE_DEBUG */
    ret = drv->Uninitialize();
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("Error: failed to Uninitialize() CFSTORE%s", "\n");
    }
    return CaseNext;
}

/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FLASH_SET_test_01_start", cfstore_utest_default_start),
        Case("FLASH_SET_test_01_end", cfstore_flash_set_test_01_end),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
