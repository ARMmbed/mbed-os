/** @file find.cpp
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
 * Test cases to find KVs in the CFSTORE using the drv->Find() interface.
 */
#if defined __MBED__ && ! defined TOOLCHAIN_GCC_ARM


#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include "mbed-drivers/mbed.h"
#endif

#include "cfstore_config.h"
#include "Driver_Common.h"
#include "cfstore_debug.h"
#include "cfstore_test.h"
#include "configuration-store/configuration_store.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#include "cfstore_uvisor.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

static control_t cfstore_find2_test_00(const size_t call_count)
{
    (void) call_count;
    printf("Not implemented for ARM toolchain\n");
    return CaseNext;
}


utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FIND2_test_00", cfstore_find2_test_00),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}



#else


#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include "mbed-drivers/mbed.h"
#endif

#include "cfstore_config.h"
#include "cfstore_test.h"
#include "cfstore_debug.h"
#include "Driver_Common.h"
#include "configuration-store/configuration_store.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_find2_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.find2.box1");
UVISOR_BOX_CONFIG(cfstore_find2_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


extern ARM_CFSTORE_DRIVER cfstore_driver;

#ifdef TARGET_LIKE_FRDM_K64F_GCC
#define CFSTORE_FIND_MBED_HOSTTEST_TIMEOUT 60
#endif

void cfstore_find2_callback(int32_t status, ARM_CFSTORE_OPCODE cmd_code, void *client_context, ARM_CFSTORE_HANDLE handle)
{
    (void) status;
    (void) cmd_code;
    (void) client_context;
    (void) handle;
    return;
}

/* report whether built/configured for flash sync or async mode */
static control_t cfstore_find2_test_00(const size_t call_count)
{
    (void) call_count;
    CFSTORE_LOG("INITIALIZING: caps.asynchronous_ops=%lu\n", cfstore_driver.GetCapabilities().asynchronous_ops);
    return CaseNext;
}


static control_t cfstore_find2_test_01(const size_t call_count)
{
	char keyBuffer[128] = "com.arm.mbed.manifest-manager.root.AQAAAAAAAAA-.manifest";
	int32_t rc;
	ARM_CFSTORE_HANDLE_INIT(hkey);
	ARM_CFSTORE_HANDLE_INIT(prev);

	// Initialize the config store
	(void) call_count;
	cfstore_driver.Initialize(cfstore_find2_callback, NULL);
	cfstore_driver.PowerControl(ARM_POWER_FULL);

	// Find the target key
	rc = cfstore_driver.Find(keyBuffer, prev, hkey);

	// If the target key was not found
	if (rc == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
		ARM_CFSTORE_KEYDESC kdesc = {
			.acl = {
				.perm_owner_read = 1,
				.perm_owner_write = 1,
				.perm_owner_execute = 0,
				.perm_other_read = 1,
				.perm_other_write = 0,
				.perm_other_execute = 0,
				/* added initialisers */
                .reserved = 0
			}, .drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE,
			.security = {
				.acls = 1,
				.rollback_protection = 0,
				.tamper_proof = 0,
				.internal_flash = 0,
				/* added initialisers */
                .reserved1 = 0,
                .software_attacks = 0,
                .board_level_attacks = 0,
                .chip_level_attacks = 0,
                .side_channel_attacks = 0,
                .reserved2 = 0
			},
			.flags = {
				.continuous = 0,
				.lazy_flush = 1,
				.flush_on_close = 1,
				.read = 0,
				.write = 1,
				.execute = 0,
				.storage_detect = 1,
                /* added initialisers */
                .reserved = 0
			}
		};

		// Create the target key
		rc = cfstore_driver.Create(keyBuffer, 191, &kdesc, hkey);
	}
	return CaseNext;
}


/* fixed version of brendans code */
#define CFSTORE_FIND2_TEST_02_VALUE_SIZE 191
static control_t cfstore_find2_test_02(const size_t call_count)
{
	char keyBuffer[128] = "com.arm.mbed.manifest-manager.root.AQAAAAAAAAA-.manifest";

	int32_t rc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_HANDLE_INIT(prev);
    ARM_CFSTORE_SIZE length;
    char value[CFSTORE_FIND2_TEST_02_VALUE_SIZE];

    // Initialize the config store
    (void) call_count;
    cfstore_driver.Initialize(NULL, NULL); /* non-null client_callback not supported for MBED_V_0_1_x */
    cfstore_driver.PowerControl(ARM_POWER_FULL);
    memset(value, 0, 191);

    // Find the target key
    rc = cfstore_driver.Find(keyBuffer, prev, hkey);

    // If the target key was not found
    if (rc == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
        ARM_CFSTORE_KEYDESC kdesc = {
            .acl = {
                .perm_owner_read = 1,
                .perm_owner_write = 1,
                .perm_owner_execute = 0,
                .perm_other_read = 1,
                .perm_other_write = 0,
                .perm_other_execute = 0,
                .reserved = 0
            }, .drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE,	/* DATA_RETENTION_NVM not supported for MBED_V_0_1_x */
            .security = {
                .acls = 0, /* protection against internal software attacks using ACLs not supported for MBED_V_0_1_x */
                .rollback_protection = 0,
                .tamper_proof = 0,
                .internal_flash = 0,
                .reserved1 = 0,
                .software_attacks = 0,
                .board_level_attacks = 0,
                .chip_level_attacks = 0,
                .side_channel_attacks = 0,
                .reserved2 = 0
            },
            .flags = {
                .continuous = 0,
                .lazy_flush = 0, 		/* lazy flush not supported for MBED_V_0_1_x */
                .flush_on_close = 0,	/* flush on close not supported for MBED_V_0_1_x */
                .read = 0,
                .write = 1,
                .execute = 0,
                .storage_detect = 0,    /* storage detect supported for MBED_V_0_1_x */
                /* added initialisers */
                .reserved = 0

            }
        };

        // Create the target key
        rc = cfstore_driver.Create(keyBuffer, 191, &kdesc, hkey);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%sError: failed to create key\n", __func__);
        TEST_ASSERT_MESSAGE(rc >= ARM_DRIVER_OK, cfstore_find2_utest_msg_g);

        strncpy(value, "MyValueData", CFSTORE_FIND2_TEST_02_VALUE_SIZE);
		length = strlen(value);
		rc = cfstore_driver.Write(hkey, value, &length);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%sError: failed to write key\n", __func__);
        TEST_ASSERT_MESSAGE(rc >= ARM_DRIVER_OK, cfstore_find2_utest_msg_g);
		printf("Success!\n");
    }
    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(400, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FIND2_test_00", cfstore_find2_test_00),
        Case("FIND2_test_01", cfstore_find2_test_01),
        Case("FIND2_test_02", cfstore_find2_test_02),
};


/* Declare your test specification with a custom setup handler */
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
