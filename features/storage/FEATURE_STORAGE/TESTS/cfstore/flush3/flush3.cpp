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
 */

/** @file flush3.cpp Test cases to flush KVs in the CFSTORE using the drv->Flush() interface.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 *
 * CFSTORE flash-journal sync mode test for github issue Heap Corruption by Jenia Kogan:
 *      https://github.com/ARMmbed/configuration-store/issues/17
 * No evidence of heap corruption has been found but the test case put together to
 * investiage that problem did result in a bug being found, that being that
 * when the last attribute was deleted from CFSTORE, this was not properly committed to
 * flash, so that CFSTORE was re-initialised, the old attribute was read back from
 * flash into the store. This has now been fixed.
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

#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#include "cfstore_uvisor.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_flush3_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

#ifdef YOTTA_CFG_CFSTORE_UVISOR
/* Create the main box ACL list for the application.
 * The main ACL gets inherited by all the other boxes
 */
CFSTORE_UVISOR_MAIN_ACL(cfstore_acl_uvisor_box_flush3_g);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, cfstore_acl_uvisor_box_flush3_g);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

/// @cond CFSTORE_DOXYGEN_DISABLE
#ifdef CFSTORE_DEBUG
#define CFSTORE_FLUSH3_GREENTEA_TIMEOUT_S     1000
#else
#define CFSTORE_FLUSH3_GREENTEA_TIMEOUT_S     100
#endif
/// @endcond

/* used for sync mode build only */
#if defined STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS && STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS==0

#define CFSTORE_UNUSED_PARAM(param)      (void)(param)

int32_t cfstore_flush3_end(void)
{
    int32_t cfsStatus;
    ARM_CFSTORE_DRIVER *cfstoreDriver = &cfstore_driver;

    CFSTORE_DBGLOG("%s:IN\n", __func__);
    cfsStatus = cfstoreDriver->Uninitialize();
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("CFStore Finalization failed (err %ld)\n", cfsStatus);
        return ARM_DRIVER_ERROR;
    }
    CFSTORE_DBGLOG("%s:OUT:returning ARM_DRIVER_OK\n", __func__);
    return ARM_DRIVER_OK;
}


int32_t cfstore_flush3_delete_file(const char *fileDir, size_t maxFilePathSize, const char *fileName)
{
    int32_t cfsStatus;
    int32_t status = ARM_DRIVER_OK;

    ARM_CFSTORE_DRIVER *cfstoreDriver = &cfstore_driver;
    ARM_CFSTORE_FMODE flags;
    ARM_CFSTORE_HANDLE_INIT(hkey);

    CFSTORE_DBGLOG("%s:IN. File name %s\n", __func__, fileName);

    CFSTORE_UNUSED_PARAM(fileDir);
    CFSTORE_UNUSED_PARAM(maxFilePathSize);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Uninitialize Cfstore\n", __func__);
    TEST_ASSERT_MESSAGE(cfstoreDriver != NULL, cfstore_flush3_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Invalid file name\n", __func__);
    TEST_ASSERT_MESSAGE(fileName != NULL, cfstore_flush3_utest_msg_g);

    memset(&flags, 0, sizeof(flags));
    flags.write = true;
    cfsStatus = cfstoreDriver->Open(fileName, flags, hkey);
    if (cfsStatus == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
        /* printf added: modification to original code supplied by Jenia Kogan. */
        CFSTORE_DBGLOG("%s: cfsStatus == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND. returning success\n", __func__);
        return ARM_DRIVER_OK;
    }
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Open failed (err %ld)\n", cfsStatus);
        return ARM_DRIVER_ERROR;
    }
    cfsStatus = cfstoreDriver->Delete(hkey);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Failed deleting (%s) failed (err %ld)\n", fileName, cfsStatus);
        status = ARM_DRIVER_ERROR;
        goto out;
    }
out:
    cfsStatus = cfstoreDriver->Close(hkey);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Close failed (err %ld)\n", cfsStatus);
        return ARM_DRIVER_ERROR;
    }
    /* Flash-to-flash only on success */
    if (status == ARM_DRIVER_OK) {
        cfsStatus = cfstoreDriver->Flush();
        if(cfsStatus < ARM_DRIVER_OK){
            CFSTORE_DBGLOG("Flush to flash failed (err %ld)\n", cfsStatus);
            return ARM_DRIVER_ERROR;
        }
    }
    CFSTORE_DBGLOG("%s:OUT: status=%d\n", __func__, (int) status);
    return status;
}


int32_t cfstore_flush3_read_file(const char *fileDir, size_t maxFilePathSize, const char *fileName, uint8_t *buff, size_t buffSize)
{
	int32_t cfsStatus;
	int32_t status = ARM_DRIVER_OK;

	ARM_CFSTORE_DRIVER *cfstoreDriver = &cfstore_driver;
	ARM_CFSTORE_FMODE flags;
	ARM_CFSTORE_SIZE readCount = buffSize;
	ARM_CFSTORE_HANDLE_INIT(hkey);

	CFSTORE_DBGLOG("%s:IN. File name %s, buffer %p, buffsize %d\n", __func__, fileName, buff, buffSize);

	CFSTORE_UNUSED_PARAM(fileDir);
	CFSTORE_UNUSED_PARAM(maxFilePathSize);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Uninitialize Cfstore\n", __func__);
    TEST_ASSERT_MESSAGE(cfstoreDriver != NULL, cfstore_flush3_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Invalid file name\n", __func__);
    TEST_ASSERT_MESSAGE(fileName != NULL, cfstore_flush3_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Invalid buff\n", __func__);
    TEST_ASSERT_MESSAGE(buff != NULL, cfstore_flush3_utest_msg_g);

	memset(&flags, 0, sizeof(flags));
	flags.read = true;

	cfsStatus = cfstoreDriver->Open(fileName, flags, hkey);
	if(cfsStatus == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND){
	    CFSTORE_DBGLOG("File (%s) not found (err %ld)\n", fileName, cfsStatus);
	    return ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND;

	}
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Open failed (err %ld)\n", cfsStatus);
        return ARM_DRIVER_ERROR;
    }
	cfsStatus = cfstoreDriver->Read(hkey, buff, &readCount);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Read failed (err %ld)\n", cfsStatus);
        status = ARM_DRIVER_ERROR;
        goto out;
    }
    if(readCount < buffSize){
        CFSTORE_DBGLOG("Read failed, amount is %zu while requested %zu\n", readCount, buffSize);
        status = ARM_DRIVER_ERROR;
        goto out;
    }
out:
	cfsStatus = cfstoreDriver->Close(hkey);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Close failed (err %ld)\n", cfsStatus);
        return ARM_DRIVER_ERROR;
    }
    CFSTORE_DBGLOG("%s:OUT: status=%d\n", __func__, (int) status);
	return status;
}

int32_t cfstore_flush3_write_file(const char *fileDir, size_t maxFilePathSize, const char *fileName, const uint8_t *buff, size_t buffSize)
{
	int32_t cfsStatus;
	int32_t status = ARM_DRIVER_ERROR;

	ARM_CFSTORE_DRIVER *cfstoreDriver = &cfstore_driver;
	ARM_CFSTORE_SIZE writeCount = buffSize;
	ARM_CFSTORE_KEYDESC keyDesc;
	ARM_CFSTORE_HANDLE_INIT(hkey);

	CFSTORE_DBGLOG("%s:IN. File name %s, buffer %p, buffsize %d\n", __func__, fileName, buff, buffSize);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Uninitialize Cfstore\n", __func__);
    TEST_ASSERT_MESSAGE(cfstoreDriver != NULL, cfstore_flush3_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Invalid file name\n", __func__);
    TEST_ASSERT_MESSAGE(fileName != NULL, cfstore_flush3_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Invalid buff\n", __func__);
    TEST_ASSERT_MESSAGE(buff != NULL, cfstore_flush3_utest_msg_g);

	/* We always deleting the old file and recreating a new one to preserve simplicity */
	CFSTORE_DBGLOG("Before delete%s", "\n");

	/* Delete the old file */
	status = cfstore_flush3_delete_file(fileDir, maxFilePathSize, fileName);
    if(status != ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Failed deleting (%s)\n", fileName);
        return status;
    }

	CFSTORE_DBGLOG("After delete%s", "\n");
	/* Create a new file */
	memset(&keyDesc, 0, sizeof(keyDesc));
	keyDesc.drl = ARM_RETENTION_NVM;
	keyDesc.flags.read = true;
	keyDesc.flags.write = true;
	cfsStatus = cfstoreDriver->Create(fileName, buffSize, &keyDesc, hkey);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Fail creating (%s) key-store (%ld)\n", fileName, cfsStatus);
        return ARM_DRIVER_ERROR;
    }
	CFSTORE_DBGLOG("After create%s", "\n");
	cfsStatus = cfstoreDriver->Write(hkey, (const char *)buff, &writeCount);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Write failed (err %ld)\n", cfsStatus);
        status = ARM_DRIVER_ERROR;
        goto out;
    }
    if(writeCount != buffSize){
        CFSTORE_DBGLOG("Write failed, amount is %d while requested is %d\n", (int)writeCount, (int)buffSize);
        status = ARM_DRIVER_ERROR;
        goto out;
    }
	CFSTORE_DBGLOG("After write%s", "\n");
out:
	cfsStatus = cfstoreDriver->Close(hkey);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("Close failed (err %ld)\n", cfsStatus);
        return ARM_DRIVER_ERROR;
    }
	CFSTORE_DBGLOG("After close%s", "\n");
	/* Flash-to-flash only on success */
	if (status == ARM_DRIVER_OK) {
		cfsStatus = cfstoreDriver->Flush();
	    if(cfsStatus < ARM_DRIVER_OK){
	        CFSTORE_DBGLOG("Flush to flash failed(err %ld)\n", cfsStatus);
	        return ARM_DRIVER_ERROR;
	    }
		CFSTORE_DBGLOG("After flush%s", "\n");
	}		
    CFSTORE_DBGLOG("%s:OUT: status=%d\n", __func__, (int) status);
	return status;
}


int32_t cfstore_flush3_start(void)
{
	int32_t status = ARM_DRIVER_OK;
	int32_t cfsStatus;
	ARM_CFSTORE_DRIVER *cfstoreDriver = &cfstore_driver;
	ARM_CFSTORE_CAPABILITIES caps;

	CFSTORE_DBGLOG("%s:IN\n", __func__);

	/* Initialize configuration store */
	cfsStatus = cfstoreDriver->Initialize(NULL, NULL);
    if(cfsStatus < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("CFStore Initialization failed (err %lu)\n", cfsStatus);
        return ARM_DRIVER_ERROR;
    }
	/* Get capabilities */
	memset(&caps, 0, sizeof(caps));
	caps = cfstoreDriver->GetCapabilities();
    if(caps.asynchronous_ops == true){
        CFSTORE_DBGLOG("%s:Please configure CFstore to work in synchronous mode. This can be change in config.json file.\n", __func__);
        status = ARM_DRIVER_ERROR;
        goto out;
    }
    if(caps.uvisor_support_enabled == true){
        CFSTORE_DBGLOG("%s:Please enable uvisor spv box.\n", __func__);
        status = ARM_DRIVER_ERROR;
        goto out;
    }
	CFSTORE_DBGLOG("%s:OUT: returning ARM_DRIVER_OK\n", __func__);
	return ARM_DRIVER_OK; /* init succeeded */

out:
	/* init failed */
	(void) cfstore_flush3_end();
    CFSTORE_DBGLOG("%s:OUT: status=%d\n", __func__, (int) status);
	return status;
}

int32_t cfstore_flush3_check_data(uint8_t* data, int32_t len, uint8_t val)
{
    int i;
    for(i = 0; i < len; i++) {
        if(data[i] != val){
            /* found byte which doesnt have the expected data value */
            return ARM_DRIVER_ERROR;
        }
    }
    return ARM_DRIVER_OK;
}

/* @brief   test case to recreate the transactions documented in the
 *          issue 17 trace log.
 *
 * the test case was created by recreating the entries found in the
 * log report grepped from the trace log attached to githug issue 17:
 *  20160622_1321_grep_pv_configuration_store_issue_17_heap_corruption.txt
 *
 * This is what the test is intended to do:
 *  - A (setup)
 *  - cfstore_flush3_start
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 220) to create KV
 *  - cfstore_flush3_end
 *  - B (setup?)
 *  - cfstore_flush3_start
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta)
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.0)
 *  - cfstore_flush3_end
 *  - C (delete everything)
 *  - cfstore_flush3_start
 *  - cfstore_flush3_delete_file(com.arm.mbed.spv.sst.node.0). this will work as the KV is present
 *  - cfstore_flush3_delete_file(com.arm.mbed.spv.sst.node.x) where x = {1..29}
 *  - cfstore_flush3_delete_file(name com.arm.mbed.spv.sst.meta).
 *      - this should work as the KV is present
 *      - however, its is seen to fail in the trace log for the issue.
 *  - cfstore_flush3_end
 *  - D
 *  - cfstore_flush3_start
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta). readd 2280 bytes and check correct
 *      - this should fail as the kv has been deleted.
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 220) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV
 *  - cfstore_flush3_end
 *  - E
 *  - run C-D again, to delete everything
 *  - F
 *  - cfstore_flush3_start
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta). readd 2280 bytes and check correct
 *      - this should fail as the kv has been deleted.
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 220) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 816) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.1, len: 217) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 818) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV
 *  - cfstore_flush3_end
 *  - G
 *  - cfstore_flush3_start
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta) 2280 bytes should be read
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.0) 818 bytes should be read
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.1) 217 bytes should be read
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.1) 217 bytes should be read
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.1) 217 bytes should be read
 *  - cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.1) 217 bytes should be read
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.2, len: 235) to create KV
 *  - cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV
 *  - cfstore_flush3_end
 *
 *  The issue reported heap corruption. To this end, a heap buffer
 *  is allocated and checked as a canary against heap corruption
 */

#define CFSTORE_FLUSH3_TEST_DATA_CHAR      'A'         /* 0b1010 bit pattern */
#define CFSTORE_FLUSH3_HEAP_DATA_CHAR      'Z'
#define CFSTORE_FLUSH3_TEST_DATA_BUF_LEN   3000        /* 2280 was the largest buffer used*/

#ifdef TARGET_K64F
#define CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN   87500       /* alloc a heap buffer to use up some memory: value tuned to K64F */
#else
#define CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN   1000        /* alloc a heap buffer to use up some memory (minimal for non-k64f targets until tuned */
#endif /* TARGET_K64F */

static control_t cfstore_flush3_test_01(const size_t call_count)
{
    int32_t i;
    int32_t ret = ARM_DRIVER_ERROR;
    const char* kv_name_root = "com.arm.mbed.spv.sst";
    const char* kv_name_node= "node";
    const char* kv_name_meta= "meta";
    char kv_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t data[CFSTORE_FLUSH3_TEST_DATA_BUF_LEN];
    void* heap_buf = NULL;

    (void) call_count;
    heap_buf = malloc(CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  malloc() failed .\n", __func__);
    TEST_ASSERT_MESSAGE(heap_buf != NULL, cfstore_flush3_utest_msg_g);
    memset(heap_buf, CFSTORE_FLUSH3_HEAP_DATA_CHAR, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN);

    CFSTORE_DBGLOG("%s: - A (setup)\n", __func__);
    CFSTORE_DBGLOG("%s: - cfstore_flush3_start\n", __func__);
    ret = cfstore_flush3_start();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  A.1 cfstore_flush3_start() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV\n", __func__);
    memset(data, CFSTORE_FLUSH3_TEST_DATA_CHAR, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, kv_name_meta);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  A.2 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 220) to create KV\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 220);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  A.3 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - cfstore_flush3_end\n", __func__);
    ret = cfstore_flush3_end();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  A.4 cfstore_flush3_end() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: check for heap corruption\n", __func__);
    ret = cfstore_flush3_check_data((uint8_t*) heap_buf, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN, CFSTORE_FLUSH3_HEAP_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  A.5 cfstore_flush3_check_data() failed for heap (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - B\n", __func__);
    CFSTORE_DBGLOG("%s: - cfstore_flush3_start\n", __func__);
    ret = cfstore_flush3_start();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  B.1 cfstore_flush3_start() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta). read 2280\n", __func__);
    memset(data, 0, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_read_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  B.2 cfstore_flush3_read_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    ret = cfstore_flush3_check_data(data, 2280, CFSTORE_FLUSH3_TEST_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  B.3 cfstore_flush3_check_data() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.0). read 220\n", __func__);
    memset(data, 0, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_read_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, data, 220);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  B.4 cfstore_flush3_read_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    ret = cfstore_flush3_check_data(data, 220, CFSTORE_FLUSH3_TEST_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  B.5 cfstore_flush3_check_data() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - cfstore_flush3_end\n", __func__);
    ret = cfstore_flush3_end();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  B.6 cfstore_flush3_end() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: check for heap corruption\n", __func__);
    ret = cfstore_flush3_check_data((uint8_t*) heap_buf, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN, CFSTORE_FLUSH3_HEAP_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  B.7 cfstore_flush3_check_data() failed for heap (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - C (delete everything)\n", __func__);
    CFSTORE_DBGLOG("%s: - cfstore_flush3_start\n", __func__);
    ret = cfstore_flush3_start();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  C.1 cfstore_flush3_start() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_delete_file(com.arm.mbed.spv.sst.node.0). this will work as the KV is present\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_delete_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  C.2 cfstore_flush3_delete_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_delete_file(com.arm.mbed.spv.sst.node.x) where x = {1..29}, each of which should fail\n", __func__);
    for(i = 1; i <= 29; i++){
        snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.%d", (char*) kv_name_root, (char*) kv_name_node, (int) i);
        ret = cfstore_flush3_delete_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  C.%d cfstore_flush3_delete_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) i+2, (int) ret, kv_name);
        /* The delete operations are expected to fail as the keys dont exist
         * but cfstore_flush3_delete_file() returns ARM_DRIVER_OK when key isnt found, so cant test the return code.
         */
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);
    }

    CFSTORE_DBGLOG("%s: cfstore_flush3_delete_file(name com.arm.mbed.spv.sst.meta). this is expected to succeed as the KV is present\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_delete_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  C.32 cfstore_flush3_delete_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

#ifdef CFSTORE_DEBUG
    CFSTORE_DBGLOG("%s: cfstore_test_dump: dump here contents of CFSTORE so we know whats present\n", __func__);
    ret = cfstore_test_dump();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.1.1 cfstore_test_dump failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);
#endif  /* CFSTORE_DEBUG */

    CFSTORE_DBGLOG("%s: - cfstore_flush3_end\n", __func__);
    ret = cfstore_flush3_end();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  C.33 cfstore_flush3_end() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: check for heap corruption\n", __func__);
    ret = cfstore_flush3_check_data((uint8_t*) heap_buf, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN, CFSTORE_FLUSH3_HEAP_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  C.34 cfstore_flush3_check_data() failed for heap (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - D\n", __func__);
    CFSTORE_DBGLOG("%s: - cfstore_flush3_start\n", __func__);
    ret = cfstore_flush3_start();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.1 cfstore_flush3_start() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

#ifdef CFSTORE_DEBUG
    CFSTORE_DBGLOG("%s: cfstore_test_dump: dump here contents of CFSTORE so we know whats present\n", __func__);
    ret = cfstore_test_dump();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.1.1 cfstore_test_dump failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);
#endif  /* CFSTORE_DEBUG */

    CFSTORE_DBGLOG("%s: cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta). this should fail as the kv has been deleted.\n", __func__);
    memset(data, 0, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_read_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.2 cfstore_flush3_read_file() succeeded when expected to fail (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 220) to create KV.\n", __func__);
    memset(data, CFSTORE_FLUSH3_TEST_DATA_CHAR, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 220);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.3 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV.\n", __func__);
    memset(data, CFSTORE_FLUSH3_TEST_DATA_CHAR, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.4 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - cfstore_flush3_end\n", __func__);
    ret = cfstore_flush3_end();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.5 cfstore_flush3_end() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: check for heap corruption\n", __func__);
    ret = cfstore_flush3_check_data((uint8_t*) heap_buf, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN, CFSTORE_FLUSH3_HEAP_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  D.6 cfstore_flush3_check_data() failed for heap (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - E: run C-D again, to delete everything\n", __func__);
    CFSTORE_DBGLOG("%s: - cfstore_flush3_start\n", __func__);
    ret = cfstore_flush3_start();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  E.1 cfstore_flush3_start() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_delete_file(com.arm.mbed.spv.sst.node.0). this will work as the KV is present.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_delete_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  E.2 cfstore_flush3_delete_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_delete_file(com.arm.mbed.spv.sst.node.x) where x = {1..29}, each of which should fail.\n", __func__);
    for(i = 1; i <= 29; i++){
        snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.%d", (char*) kv_name_root, (char*) kv_name_node, (int) i);
        ret = cfstore_flush3_delete_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  E.%d cfstore_flush3_delete_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) i+2, (int) ret, kv_name);
        /* The delete operations are expected to fail as the keys dont exist
         * but cfstore_flush3_delete_file() returns ARM_DRIVER_OK when key isnt found, so cant test the return code.
         */
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);
    }

    CFSTORE_DBGLOG("%s: cfstore_flush3_delete_file(name com.arm.mbed.spv.sst.meta). this is expected to succeed as the KV is present.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_delete_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  E.32 cfstore_flush3_delete_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - cfstore_flush3_end\n", __func__);
    ret = cfstore_flush3_end();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  E.33 cfstore_flush3_end() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: check for heap corruption\n", __func__);
    ret = cfstore_flush3_check_data((uint8_t*) heap_buf, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN, CFSTORE_FLUSH3_HEAP_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  E.34 cfstore_flush3_check_data() failed for heap (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - F\n", __func__);
    CFSTORE_DBGLOG("%s: - cfstore_flush3_start\n", __func__);
    ret = cfstore_flush3_start();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.1 cfstore_flush3_start() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta). this should fail as the kv has been deleted.\n", __func__);
    memset(data, 0, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_read_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.2 cfstore_flush3_read_file() succeeded when expected to fail (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    memset(data, CFSTORE_FLUSH3_TEST_DATA_CHAR, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    /* cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 220) to create KV */
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 220);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.3 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.4 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 816) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 816);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.5 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.6 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.1, len: 217) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.1", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 217);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.7 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.8 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.0, len: 818) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 818);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.9 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.10 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - cfstore_flush3_end\n", __func__);
    ret = cfstore_flush3_end();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.11 cfstore_flush3_end() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: check for heap corruption\n", __func__);
    ret = cfstore_flush3_check_data((uint8_t*) heap_buf, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN, CFSTORE_FLUSH3_HEAP_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.12 cfstore_flush3_check_data() failed for heap (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - G\n", __func__);
    CFSTORE_DBGLOG("%s: - cfstore_flush3_start\n", __func__);
    ret = cfstore_flush3_start();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  G.1 cfstore_flush3_start() failed (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_read_file(com.arm.mbed.spv.sst.meta) 2280 bytes should be read.\n", __func__);
    memset(data, 0, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_read_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  G.2 cfstore_flush3_read_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    ret = cfstore_flush3_check_data(data, 2280, CFSTORE_FLUSH3_TEST_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  G.3 cfstore_flush3_check_data() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.0) 818 bytes should be read.\n", __func__);
    memset(data, 0, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_read_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, data, 818);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  G.4 cfstore_flush3_read_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    ret = cfstore_flush3_check_data(data, 818, CFSTORE_FLUSH3_TEST_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  G.5 cfstore_flush3_check_data() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_read_file(com.arm.mbed.spv.sst.node.1) 217 bytes should be read. repeat 4 times.\n", __func__);
    for(i = 0; i < 4; i++){
        memset(data, 0, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
        snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.1", (char*) kv_name_root, (char*) kv_name_node);
        ret = cfstore_flush3_read_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, data, 217);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  G.%d.1 cfstore_flush3_read_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) i+6, (int) ret, kv_name);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

        ret = cfstore_flush3_check_data(data, 217, CFSTORE_FLUSH3_TEST_DATA_CHAR);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  G.%d.2 cfstore_flush3_check_data() failed (ret=%d, kv_name=%s).\n", __func__, (int) i+6, (int) ret, kv_name);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);
    }

    memset(data, CFSTORE_FLUSH3_TEST_DATA_CHAR, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.node.2, len: 235) to create KV.\n", __func__);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s.0", (char*) kv_name_root, (char*) kv_name_node);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 235);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.3 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: cfstore_flush3_write_file(com.arm.mbed.spv.sst.meta, len: 2280) to create KV.\n", __func__);
    memset(data, CFSTORE_FLUSH3_TEST_DATA_CHAR, CFSTORE_FLUSH3_TEST_DATA_BUF_LEN);
    snprintf(kv_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%s", (char*) kv_name_root, (char*) kv_name_meta);
    ret = cfstore_flush3_write_file(NULL, CFSTORE_KEY_NAME_MAX_LENGTH, (const char*) kv_name, (const uint8_t*) data, 2280);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  A.2 cfstore_flush3_write_file() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: - cfstore_flush3_end\n", __func__);
    ret = cfstore_flush3_end();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  F.11 cfstore_flush3_end() failed (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s: check for heap corruption\n", __func__);
    ret = cfstore_flush3_check_data((uint8_t*) heap_buf, CFSTORE_FLUSH3_TEST_HEAP_BUF_LEN, CFSTORE_FLUSH3_HEAP_DATA_CHAR);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:  A.5 cfstore_flush3_check_data() failed for heap (ret=%d, kv_name=%s).\n", __func__, (int) ret, kv_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    if(heap_buf){
        free(heap_buf);
    }

    return CaseNext;
}


/* @brief   simple flush test */
static control_t cfstore_flush3_test_02(const size_t call_count)
{
    int32_t cfsStatus = ARM_DRIVER_ERROR;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_FMODE flags;
    ARM_CFSTORE_SIZE len = strlen("key0");
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    memset(&flags, 0, sizeof(flags));

    CFSTORE_DBGLOG("%s:Initialize()\n", __func__);
    cfsStatus = drv->Initialize(NULL, NULL);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s:Create()\n", __func__);
    cfsStatus = drv->Create("key0", len, &kdesc, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    len = strlen("some-value");
    CFSTORE_DBGLOG("%s:Write()\n", __func__);
    cfsStatus = drv->Write(hkey, "some-value", &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s:Close()\n", __func__);
    cfsStatus = drv->Close(hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s:Flush()\n", __func__);
    cfsStatus = drv->Flush();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s:Open()\n", __func__);
    cfsStatus = drv->Open("key0", flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s:Delete()\n", __func__);
    cfsStatus = drv->Delete(hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s:Close()\n", __func__);
    cfsStatus = drv->Close(hkey);       ///////  <--- cfsStatus = ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    CFSTORE_DBGLOG("%s:got status = %d\n", __func__, (int) cfsStatus);

    CFSTORE_DBGLOG("%s:Uninitialize()\n", __func__);
    cfsStatus = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error:%d:cfsStatus=%d", __func__, __LINE__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);

    return CaseNext;
}

#endif /* STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS && STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS==0 */


static control_t cfstore_flush3_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_CAPABILITIES caps;;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    (void) call_count;

    /* initialise the context */
    caps = drv->GetCapabilities();
    CFSTORE_LOG("%s:INITIALIZING: caps.asynchronous_ops=%lu\n", __func__, caps.asynchronous_ops);
    if(caps.asynchronous_ops == 1){
        /* This is a sync mode only test. If this test is not built for sync mode, then skip testing return true
         * This means the test will conveniently pass when run in CI as part of async mode testing */
        CFSTORE_LOG("*** Skipping test as binary built for flash journal async mode, and this test is sync-only%s", "\n");
        return CaseNext;
    }
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush3_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush3_utest_msg_g);
    return CaseNext;
}

/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(CFSTORE_FLUSH3_GREENTEA_TIMEOUT_S, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FLUSH3_test_00", cfstore_flush3_test_00),
#if defined STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS && STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS==0
        Case("FLUSH3_test_01", cfstore_flush3_test_01),
        Case("FLUSH3_test_02", cfstore_flush3_test_02),
#endif // STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

/* mbedosV3++*/
int main()
{
    return !Harness::run(specification);
}
/// @cond

