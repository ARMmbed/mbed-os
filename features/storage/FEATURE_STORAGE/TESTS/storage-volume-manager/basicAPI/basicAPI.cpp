/*
 * Copyright (c) 2006-2016, ARM Limited, All Rights Reserved
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

#ifdef TARGET_LIKE_POSIX
#define AVOID_GREENTEA
#endif

#ifndef AVOID_GREENTEA
#include "greentea-client/test_env.h"
#endif
#include "utest/utest.h"
#include "unity/unity.h"

#include "storage-volume-manager/storage_volume_manager.h"
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

/* redefine tr_info() to a printf() equivalent to emit trace */
#define tr_info(...)                                ((void) 0)
#define mbed_trace_init(...)                        ((void) 0)
#define mbed_trace_config_set(...)                  ((void) 0)

#ifdef TARGET_LIKE_X86_LINUX_NATIVE
extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_RAM;
ARM_DRIVER_STORAGE *drv = &ARM_Driver_Storage_MTD_RAM;
#elif defined TARGET_LIKE_FRDM_K64F
extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F;
ARM_DRIVER_STORAGE *drv = &ARM_Driver_Storage_MTD_K64F;
#else
extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F;
ARM_DRIVER_STORAGE *drv = &ARM_Driver_Storage_MTD_K64F;
#endif

/* temporary buffer to hold data for testing. */
static const unsigned BUFFER_SIZE = 16384;
static uint8_t buffer[BUFFER_SIZE];

static int32_t callbackStatus;
static int32_t virtualVolumeCallbackStatus;

#ifndef AVOID_GREENTEA
// Custom setup handler required for proper Greentea support
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    // Call the default reporting function
    return greentea_test_setup_handler(number_of_cases);
}
#endif

/* used only for the initialization of the volume-manager. */
void initializeCallbackHandler(int32_t status)
{
    tr_info("in initializeCallbackHandler\r\n");
    Harness::validate_callback();
}

/* used only when accessing MTD directly (for verification) */
void mtdCallbackHandler(int32_t status, ARM_STORAGE_OPERATION operation)
{
    tr_info("in mtdCallbackHandler");
    callbackStatus = status;
    Harness::validate_callback();
}

/* the normal callback handler for the virtual volume */
void virtualMTDCallbackHandler(int32_t status, ARM_STORAGE_OPERATION operation)
{
    tr_info("in virtualMTDCallbackHandler");
    virtualVolumeCallbackStatus = status;
    Harness::validate_callback();
}

control_t test_initialize(const size_t call_count)
{
    tr_info("test_initialize: called with call_count %lu", call_count);
    static StorageVolumeManager volumeManager;

    if (call_count == 1) {
        int32_t rc = volumeManager.initialize(drv, initializeCallbackHandler);
        TEST_ASSERT(rc >= ARM_DRIVER_OK);
        if (rc == ARM_DRIVER_OK) {
            TEST_ASSERT_EQUAL(1,  drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(200) + CaseRepeatAll;
        }

        /* synchronous completion */
        TEST_ASSERT(rc == 1);
    }

    TEST_ASSERT_EQUAL(true, volumeManager.isInitialized());
    TEST_ASSERT(volumeManager.getStorageInfo().total_storage > 0);
    for (size_t index = 0; index < MAX_VOLUMES; index++) {
        TEST_ASSERT_EQUAL(false, volumeManager.volumeAtIndex(index)->isAllocated());
    }

    return CaseNext;
}

template <uint64_t OFFSET>
control_t test_againstSingleVolumeAtOffset(const size_t call_count)
{
    tr_info("test_againstSingleVolumeAtOffset: called with call_count %lu", call_count);
    static StorageVolumeManager volumeManager;
    static StorageVolume *volumeP = NULL;
    static ARM_STORAGE_INFO info;
    static size_t sizeofDataOperation;

    const uint8_t PATTERN_FOR_PROGRAM_DATA = 0xAA;

    static enum {
        VOLUME_MANAGER_INITIALIZE = 1,
        BASIC_SYNCHRONOUS_API_TESTING,
        READ_DATA,
        ERASE,
        READ_AFTER_ERASE,
        PROGRAM_DATA,
        READ_AFTER_PROGRAM_DATA,
        VERIFY_PROGRAM_DATA,
        DISCONNECT_VOLUME_MANAGER_CALLBACK,
        READ_FROM_DRV_AFTER_PROGRAM_DATA,
        VERIFY_PROGRAM_DATA2,
    } state = VOLUME_MANAGER_INITIALIZE;
    tr_info("came in with state %u", state);

    int32_t rc;
    ARM_STORAGE_BLOCK firstBlock;
    rc = drv->GetNextBlock(NULL, &firstBlock);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));

    switch (state) {
        case VOLUME_MANAGER_INITIALIZE:
            rc = volumeManager.initialize(drv, initializeCallbackHandler);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1,  drv->GetCapabilities().asynchronous_ops);
                state = BASIC_SYNCHRONOUS_API_TESTING;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            /* synchronous completion */
            TEST_ASSERT(rc == 1);

            /* intentional fall-through */

        case BASIC_SYNCHRONOUS_API_TESTING:
            TEST_ASSERT_EQUAL(true, volumeManager.isInitialized());

            rc = drv->GetInfo(&info);
            TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
            TEST_ASSERT(info.total_storage > 0);

            { /* add volume */
                rc = volumeManager.addVolume(firstBlock.addr + OFFSET /*addr*/, info.total_storage - OFFSET /*size*/ , &volumeP);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(true, volumeManager.volumeAtIndex(0)->isAllocated());
                for (size_t index = 1; index < MAX_VOLUMES; index++) {
                    TEST_ASSERT_EQUAL(false, volumeManager.volumeAtIndex(index)->isAllocated());
                }
            }

            { /* GetVersion */
                TEST_ASSERT_EQUAL(drv->GetVersion().api, volumeP->GetVersion().api);
                TEST_ASSERT_EQUAL(drv->GetVersion().drv, volumeP->GetVersion().drv);
            }

            { /* GetCapabilities */
                TEST_ASSERT_EQUAL(drv->GetCapabilities().asynchronous_ops, volumeP->GetCapabilities().asynchronous_ops);
                TEST_ASSERT_EQUAL(drv->GetCapabilities().erase_all,        volumeP->GetCapabilities().erase_all);
            }

            { /* Initialize */
                rc = volumeP->Initialize(virtualMTDCallbackHandler);
                TEST_ASSERT_EQUAL(1, rc);
            }

            { /* GetStatus */
                ARM_STORAGE_STATUS status = volumeP->GetStatus();
                TEST_ASSERT_EQUAL(0, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
            }

            { /* GetInfo */
                ARM_STORAGE_INFO volumeInfo;
                rc = volumeP->GetInfo(&volumeInfo);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, volumeInfo.total_storage);
                TEST_ASSERT_EQUAL(info.program_unit,           volumeInfo.program_unit);
                TEST_ASSERT_EQUAL(info.optimal_program_unit,   volumeInfo.optimal_program_unit);
                TEST_ASSERT_EQUAL(info.program_cycles,         volumeInfo.program_cycles);
                TEST_ASSERT_EQUAL(info.erased_value,           volumeInfo.erased_value);
                TEST_ASSERT_EQUAL(info.memory_mapped,          volumeInfo.memory_mapped);
                TEST_ASSERT_EQUAL(info.programmability,        volumeInfo.programmability);
                TEST_ASSERT_EQUAL(info.retention_level,        volumeInfo.retention_level);
                TEST_ASSERT_EQUAL(info.reserved,               volumeInfo.reserved);
                TEST_ASSERT_EQUAL(0, memcmp(&info.security, &volumeInfo.security, sizeof(ARM_STORAGE_SECURITY_FEATURES)));
            }

            { /* resolve address */
                TEST_ASSERT_EQUAL(firstBlock.addr + OFFSET, volumeP->ResolveAddress(0));
                TEST_ASSERT_EQUAL(firstBlock.addr + OFFSET + info.total_storage, volumeP->ResolveAddress(info.total_storage));
                TEST_ASSERT_EQUAL(firstBlock.addr + OFFSET + info.total_storage / 2, volumeP->ResolveAddress(info.total_storage / 2));
            }

            { /* GetNextBlock */
                rc = volumeP->GetNextBlock(NULL, NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                ARM_STORAGE_BLOCK block;
                rc = volumeP->GetNextBlock(NULL, &block);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                TEST_ASSERT_EQUAL(0, block.addr);
                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, block.size);

                rc = volumeP->GetNextBlock(&block, NULL);
                TEST_ASSERT(rc < ARM_DRIVER_OK);
                rc = volumeP->GetNextBlock(&block, &block);
                TEST_ASSERT(rc < ARM_DRIVER_OK);
            }

            { /* GetBlock */
                ARM_STORAGE_BLOCK block;
                rc = volumeP->GetBlock(0, &block);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                TEST_ASSERT_EQUAL(0, block.addr);
                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, block.size);

                rc = volumeP->GetBlock((info.total_storage / 2), &block);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                TEST_ASSERT_EQUAL(0, block.addr);
                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, block.size);

                rc = volumeP->GetBlock(info.total_storage, &block);
                TEST_ASSERT(rc < ARM_DRIVER_OK);
            }

            state = READ_DATA;
            /* intentional fallthrough */

        case READ_DATA:
            sizeofDataOperation = ((info.total_storage - OFFSET) > BUFFER_SIZE) ? BUFFER_SIZE : (info.total_storage - OFFSET);
            TEST_ASSERT(sizeofDataOperation <= BUFFER_SIZE);

            /* ReadData */
            rc = volumeP->ReadData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volumeP->GetCapabilities().asynchronous_ops);
                state = ERASE;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case ERASE:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            /* Erase */
            rc = volumeP->Erase(0, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volumeP->GetCapabilities().asynchronous_ops);
                state = READ_AFTER_ERASE;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case READ_AFTER_ERASE:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            /* Read after Erase */
            rc = volumeP->ReadData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volumeP->GetCapabilities().asynchronous_ops);
                state = PROGRAM_DATA;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case PROGRAM_DATA:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);
            for (size_t index = 0; index < sizeofDataOperation; index++) {
                // if (bytePattern != (buffer)[index]) {
                //     tr_info("%u: expected %x, found %x", index, bytePattern, buffer[index]);
                // }
                TEST_ASSERT_EQUAL(info.erased_value ? (uint8_t)0xFF : (uint8_t)0, buffer[index]);
            }

            /* ProgramData */
            memset(buffer, PATTERN_FOR_PROGRAM_DATA, sizeofDataOperation);
            rc = volumeP->ProgramData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volumeP->GetCapabilities().asynchronous_ops);
                state = READ_AFTER_PROGRAM_DATA;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case READ_AFTER_PROGRAM_DATA:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            /* Read after Program */
            rc = volumeP->ReadData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volumeP->GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);
            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            /* intentional fallthrough */

        case DISCONNECT_VOLUME_MANAGER_CALLBACK:
            rc = drv->Initialize(mtdCallbackHandler);
            TEST_ASSERT_EQUAL(1, rc); /* expect synchronous completion */
            /* intentional fallthrough */

        case READ_FROM_DRV_AFTER_PROGRAM_DATA:
            /* Read after Program */
            rc = drv->ReadData(firstBlock.addr + OFFSET, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volumeP->GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            callbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, callbackStatus);
            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            break;

        default:
            TEST_ASSERT(false);
    }

    return CaseNext;
}

template <uint64_t OFFSET>
control_t test_againstSingleCStorageAtOffset(const size_t call_count)
{
    tr_info("test_againstSingleCStorageAtOffset: called with call_count %lu", call_count);
    static StorageVolumeManager volumeManager;
    static _ARM_DRIVER_STORAGE mtd = {};
    static ARM_STORAGE_INFO info;
    static size_t sizeofDataOperation;

    const uint8_t PATTERN_FOR_PROGRAM_DATA = 0xAA;

    static enum {
        VOLUME_MANAGER_INITIALIZE = 1,
        BASIC_SYNCHRONOUS_API_TESTING,
        READ_DATA,
        ERASE,
        READ_AFTER_ERASE,
        PROGRAM_DATA,
        READ_AFTER_PROGRAM_DATA,
        VERIFY_PROGRAM_DATA,
        DISCONNECT_VOLUME_MANAGER_CALLBACK,
        READ_FROM_DRV_AFTER_PROGRAM_DATA,
        VERIFY_PROGRAM_DATA2,
    } state = VOLUME_MANAGER_INITIALIZE;
    tr_info("came in with state %u", state);

    int32_t rc;
    rc = drv->GetNextBlock(NULL, NULL);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
    ARM_STORAGE_BLOCK firstBlock;
    rc = drv->GetNextBlock(NULL, &firstBlock);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));

    switch (state) {
        case VOLUME_MANAGER_INITIALIZE:
            rc = volumeManager.initialize(drv, initializeCallbackHandler);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1,  drv->GetCapabilities().asynchronous_ops);
                state = BASIC_SYNCHRONOUS_API_TESTING;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            /* synchronous completion */
            TEST_ASSERT(rc == 1);

            /* intentional fall-through */

        case BASIC_SYNCHRONOUS_API_TESTING:
            TEST_ASSERT_EQUAL(true, volumeManager.isInitialized());

            rc = drv->GetInfo(&info);
            TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
            TEST_ASSERT(info.total_storage > 0);

            { /* add volume */
                rc = volumeManager.addVolume_C(firstBlock.addr + OFFSET /*addr*/, info.total_storage - OFFSET /*size*/ , &mtd);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(true, volumeManager.volumeAtIndex(0)->isAllocated());
                for (size_t index = 1; index < MAX_VOLUMES; index++) {
                    TEST_ASSERT_EQUAL(false, volumeManager.volumeAtIndex(index)->isAllocated());
                }
            }

            { /* GetVersion */
                TEST_ASSERT_EQUAL(drv->GetVersion().api, mtd.GetVersion().api);
                TEST_ASSERT_EQUAL(drv->GetVersion().drv, mtd.GetVersion().drv);
            }

            { /* GetCapabilities */
                TEST_ASSERT_EQUAL(drv->GetCapabilities().asynchronous_ops, mtd.GetCapabilities().asynchronous_ops);
                TEST_ASSERT_EQUAL(drv->GetCapabilities().erase_all,        mtd.GetCapabilities().erase_all);
            }

            { /* Initialize */
                rc = mtd.Initialize(virtualMTDCallbackHandler);
                TEST_ASSERT_EQUAL(1, rc);
            }

            { /* GetStatus */
                ARM_STORAGE_STATUS status = mtd.GetStatus();
                TEST_ASSERT_EQUAL(0, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
            }

            { /* GetInfo */
                ARM_STORAGE_INFO volumeInfo;
                rc = mtd.GetInfo(&volumeInfo);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, volumeInfo.total_storage);
                TEST_ASSERT_EQUAL(info.program_unit,           volumeInfo.program_unit);
                TEST_ASSERT_EQUAL(info.optimal_program_unit,   volumeInfo.optimal_program_unit);
                TEST_ASSERT_EQUAL(info.program_cycles,         volumeInfo.program_cycles);
                TEST_ASSERT_EQUAL(info.erased_value,           volumeInfo.erased_value);
                TEST_ASSERT_EQUAL(info.memory_mapped,          volumeInfo.memory_mapped);
                TEST_ASSERT_EQUAL(info.programmability,        volumeInfo.programmability);
                TEST_ASSERT_EQUAL(info.retention_level,        volumeInfo.retention_level);
                TEST_ASSERT_EQUAL(info.reserved,               volumeInfo.reserved);
                TEST_ASSERT_EQUAL(0, memcmp(&info.security, &volumeInfo.security, sizeof(ARM_STORAGE_SECURITY_FEATURES)));
            }

            { /* resolve address */
                TEST_ASSERT_EQUAL(firstBlock.addr + OFFSET, mtd.ResolveAddress(0));
                TEST_ASSERT_EQUAL(firstBlock.addr + OFFSET + info.total_storage, mtd.ResolveAddress(info.total_storage));
                TEST_ASSERT_EQUAL(firstBlock.addr + OFFSET + info.total_storage / 2, mtd.ResolveAddress(info.total_storage / 2));
            }

            { /* GetNextBlock */
                rc = mtd.GetNextBlock(NULL, NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                ARM_STORAGE_BLOCK block;
                rc = mtd.GetNextBlock(NULL, &block);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                TEST_ASSERT_EQUAL(0, block.addr);
                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, block.size);

                rc = mtd.GetNextBlock(&block, NULL);
                TEST_ASSERT(rc < ARM_DRIVER_OK);
                rc = mtd.GetNextBlock(&block, &block);
                TEST_ASSERT(rc < ARM_DRIVER_OK);
            }

            { /* GetBlock */
                rc = mtd.GetBlock(0, NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                ARM_STORAGE_BLOCK block;
                rc = mtd.GetBlock(0, &block);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                TEST_ASSERT_EQUAL(0, block.addr);
                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, block.size);

                rc = mtd.GetBlock((info.total_storage / 2), NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                rc = mtd.GetBlock((info.total_storage / 2), &block);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                TEST_ASSERT_EQUAL(0, block.addr);
                TEST_ASSERT_EQUAL(info.total_storage - OFFSET, block.size);

                rc = mtd.GetBlock(info.total_storage, NULL);
                TEST_ASSERT(rc < ARM_DRIVER_OK);
                rc = mtd.GetBlock(info.total_storage, &block);
                TEST_ASSERT(rc < ARM_DRIVER_OK);
            }

            state = READ_DATA;
            /* intentional fallthrough */

        case READ_DATA:
            sizeofDataOperation = ((info.total_storage - OFFSET) > BUFFER_SIZE) ? BUFFER_SIZE : (info.total_storage - OFFSET);
            TEST_ASSERT(sizeofDataOperation <= BUFFER_SIZE);

            /* ReadData */
            rc = mtd.ReadData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd.GetCapabilities().asynchronous_ops);
                state = ERASE;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case ERASE:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            /* Erase */
            rc = mtd.Erase(0, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd.GetCapabilities().asynchronous_ops);
                state = READ_AFTER_ERASE;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case READ_AFTER_ERASE:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            /* Read after Erase */
            rc = mtd.ReadData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd.GetCapabilities().asynchronous_ops);
                state = PROGRAM_DATA;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case PROGRAM_DATA:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);
            for (size_t index = 0; index < sizeofDataOperation; index++) {
                // if (bytePattern != (buffer)[index]) {
                //     tr_info("%u: expected %x, found %x", index, bytePattern, buffer[index]);
                // }
                TEST_ASSERT_EQUAL(info.erased_value ? (uint8_t)0xFF : (uint8_t)0, buffer[index]);
            }

            /* ProgramData */
            memset(buffer, PATTERN_FOR_PROGRAM_DATA, sizeofDataOperation);
            rc = mtd.ProgramData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd.GetCapabilities().asynchronous_ops);
                state = READ_AFTER_PROGRAM_DATA;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case READ_AFTER_PROGRAM_DATA:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            /* Read after Program */
            rc = mtd.ReadData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd.GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);
            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            /* intentional fallthrough */

        case DISCONNECT_VOLUME_MANAGER_CALLBACK:
            rc = drv->Initialize(mtdCallbackHandler);
            TEST_ASSERT_EQUAL(1, rc); /* expect synchronous completion */
            /* intentional fallthrough */

        case READ_FROM_DRV_AFTER_PROGRAM_DATA:
            /* Read after Program */
            rc = drv->ReadData(firstBlock.addr + OFFSET, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd.GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            callbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, callbackStatus);
            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            break;

        default:
            TEST_ASSERT(false);
    }

    return CaseNext;
}

template <uint64_t OFFSET1, uint64_t SIZE1, uint64_t OFFSET2, uint64_t SIZE2>
control_t test_concurrentAccessFromTwoVolumes(const size_t call_count)
{
    tr_info("test_concurrentAccessFromTwoVolumes: called with call_count %lu", call_count);

    if (MAX_VOLUMES <= 1) {
        return CaseNext;
    }

    static StorageVolumeManager volumeManager;
    static StorageVolume *volume1P = NULL;
    static StorageVolume *volume2P = NULL;
    static ARM_STORAGE_INFO info;
    static size_t sizeofDataOperation;

    const uint8_t PATTERN_FOR_PROGRAM_DATA = 0xAA;

    static enum {
        VOLUME_MANAGER_INITIALIZE = 1,
        ADD_VOLUMES,
        ERASE1,
        PROGRAM_DATA1,
        ERASE2,
        PROGRAM_DATA2,
        DISCONNECT_VOLUME_MANAGER_CALLBACK,
        READ_FROM_DRV_AFTER_PROGRAM_DATA1,
        VERIFY_PROGRAM_DATA1,
        READ_FROM_DRV_AFTER_PROGRAM_DATA2,
        VERIFY_PROGRAM_DATA2,
    } state = VOLUME_MANAGER_INITIALIZE;
    tr_info("came in with state %u", state);

    int32_t rc;
    switch (state) {
        case VOLUME_MANAGER_INITIALIZE:
            rc = volumeManager.initialize(drv, initializeCallbackHandler);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1,  drv->GetCapabilities().asynchronous_ops);
                state = ADD_VOLUMES;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            /* synchronous completion */
            TEST_ASSERT(rc == 1);

            /* intentional fall-through */

        case ADD_VOLUMES:
            TEST_ASSERT_EQUAL(true, volumeManager.isInitialized());

            rc = drv->GetInfo(&info);
            TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
            TEST_ASSERT(info.total_storage > 0);

            { /* add volume1 */
                rc = drv->GetBlock(OFFSET1, NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                ARM_STORAGE_BLOCK block1;
                rc = drv->GetBlock(OFFSET1, &block1);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                rc = drv->GetBlock(OFFSET1 + SIZE1 - 1, NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                rc = drv->GetBlock(OFFSET1 + SIZE1 - 1, &block1);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                rc = volumeManager.addVolume(OFFSET1 /*addr*/, SIZE1 /*size*/ , &volume1P);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(true, volumeManager.volumeAtIndex(0)->isAllocated());
                for (size_t index = 1; index < MAX_VOLUMES; index++) {
                    TEST_ASSERT_EQUAL(false, volumeManager.volumeAtIndex(index)->isAllocated());
                }

                { /* Initialize */
                    rc = volume1P->Initialize(virtualMTDCallbackHandler);
                    TEST_ASSERT_EQUAL(1, rc);
                }
            }
            { /* add volume2 */
                rc = drv->GetBlock(OFFSET2, NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                ARM_STORAGE_BLOCK block2;
                rc = drv->GetBlock(OFFSET2, &block2);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                rc = drv->GetBlock(OFFSET2 + SIZE2 - 2, NULL);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                rc = drv->GetBlock(OFFSET2 + SIZE2 - 2, &block2);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                rc = volumeManager.addVolume(OFFSET2 /*addr*/, SIZE2 /*size*/ , &volume2P);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(true, volumeManager.volumeAtIndex(1)->isAllocated());
                for (size_t index = 2; index < MAX_VOLUMES; index++) {
                    TEST_ASSERT_EQUAL(false, volumeManager.volumeAtIndex(index)->isAllocated());
                }

                { /* Initialize */
                    rc = volume2P->Initialize(virtualMTDCallbackHandler);
                    TEST_ASSERT_EQUAL(1, rc);
                }
            }

            sizeofDataOperation = (SIZE1 > BUFFER_SIZE) ? BUFFER_SIZE : SIZE1;
            sizeofDataOperation = (SIZE2 > sizeofDataOperation) ? sizeofDataOperation : SIZE2;
            TEST_ASSERT((sizeofDataOperation > 0) && (sizeofDataOperation <= BUFFER_SIZE));
            memset(buffer, PATTERN_FOR_PROGRAM_DATA, sizeofDataOperation);

            /* intentional fall-through */

        case ERASE1:
            rc = volume1P->Erase(0, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volume1P->GetCapabilities().asynchronous_ops);
                state = PROGRAM_DATA1;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case PROGRAM_DATA1:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            tr_info("PROGRAM_DATA1");
            rc = volume1P->ProgramData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volume1P->GetCapabilities().asynchronous_ops);

                ARM_STORAGE_STATUS status;
                status = drv->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = volume1P->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = volume2P->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);

                rc = volume2P->ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = volume1P->ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = volume1P->ReadData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = volume1P->Erase(0, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);

                state = ERASE2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case ERASE2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            rc = volume2P->Erase(0, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volume2P->GetCapabilities().asynchronous_ops);
                state = PROGRAM_DATA2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case PROGRAM_DATA2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            tr_info("PROGRAM_DATA2");
            rc = volume2P->ProgramData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, volume2P->GetCapabilities().asynchronous_ops);

                ARM_STORAGE_STATUS status;
                status = drv->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = volume2P->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = volume1P->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);

                rc = volume1P->ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = volume2P->ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = volume2P->ReadData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = volume2P->Erase(0, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);

                state = DISCONNECT_VOLUME_MANAGER_CALLBACK;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case DISCONNECT_VOLUME_MANAGER_CALLBACK:
            tr_info("DISCONNECT_VOLUME_MANAGER_CALLBACK");
            rc = drv->Initialize(mtdCallbackHandler);
            TEST_ASSERT_EQUAL(1, rc); /* expect synchronous completion */
            /* intentional fallthrough */

        case READ_FROM_DRV_AFTER_PROGRAM_DATA1:
            tr_info("verifying state");
            /* Read after Program */
            rc = drv->ReadData(OFFSET1, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA1;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA1:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            /* intentional fallthrough */

        case READ_FROM_DRV_AFTER_PROGRAM_DATA2:
            /* Read after Program */
            rc = drv->ReadData(OFFSET2, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            break;

        default:
            TEST_ASSERT(false);
    }

    return CaseNext;
}

template <uint64_t OFFSET1, uint64_t SIZE1, uint64_t OFFSET2, uint64_t SIZE2>
control_t test_concurrentAccessFromTwoCStorageDevices(const size_t call_count)
{
    tr_info("test_concurrentAccessFromTwoCStorageDevices: called with call_count %lu", call_count);

    if (MAX_VOLUMES <= 1) {
        return CaseNext;
    }

    static StorageVolumeManager volumeManager;
    static _ARM_DRIVER_STORAGE mtd1 = {};
    static _ARM_DRIVER_STORAGE mtd2 = {};
    static ARM_STORAGE_INFO info;
    static size_t sizeofDataOperation;

    const uint8_t PATTERN_FOR_PROGRAM_DATA = 0xAA;

    static enum {
        VOLUME_MANAGER_INITIALIZE = 1,
        ADD_VOLUMES,
        ERASE1,
        PROGRAM_DATA1,
        ERASE2,
        PROGRAM_DATA2,
        DISCONNECT_VOLUME_MANAGER_CALLBACK,
        READ_FROM_DRV_AFTER_PROGRAM_DATA1,
        VERIFY_PROGRAM_DATA1,
        READ_FROM_DRV_AFTER_PROGRAM_DATA2,
        VERIFY_PROGRAM_DATA2,
    } state = VOLUME_MANAGER_INITIALIZE;
    tr_info("came in with state %u", state);

    int32_t rc;
    switch (state) {
        case VOLUME_MANAGER_INITIALIZE:
            rc = volumeManager.initialize(drv, initializeCallbackHandler);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1,  drv->GetCapabilities().asynchronous_ops);
                state = ADD_VOLUMES;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            /* synchronous completion */
            TEST_ASSERT(rc == 1);

            /* intentional fall-through */

        case ADD_VOLUMES:
            TEST_ASSERT_EQUAL(true, volumeManager.isInitialized());

            rc = drv->GetInfo(&info);
            TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
            TEST_ASSERT(info.total_storage > 0);

            { /* add C_Storage device 1 */
                ARM_STORAGE_BLOCK block1;
                rc = drv->GetBlock(OFFSET1, &block1);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                rc = drv->GetBlock(OFFSET1 + SIZE1 - 1, &block1);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                rc = volumeManager.addVolume_C(OFFSET1 /*addr*/, SIZE1 /*size*/ , &mtd1);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(true, volumeManager.volumeAtIndex(0)->isAllocated());
                for (size_t index = 1; index < MAX_VOLUMES; index++) {
                    TEST_ASSERT_EQUAL(false, volumeManager.volumeAtIndex(index)->isAllocated());
                }

                { /* Initialize */
                    rc = mtd1.Initialize(virtualMTDCallbackHandler);
                    TEST_ASSERT_EQUAL(1, rc);
                }
            }
            { /* add C_Storage device 2 */
                ARM_STORAGE_BLOCK block2;
                rc = drv->GetBlock(OFFSET2, &block2);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
                rc = drv->GetBlock(OFFSET2 + SIZE2 - 2, &block2);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                rc = volumeManager.addVolume_C(OFFSET2 /*addr*/, SIZE2 /*size*/ , &mtd2);
                TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

                TEST_ASSERT_EQUAL(true, volumeManager.volumeAtIndex(1)->isAllocated());
                for (size_t index = 2; index < MAX_VOLUMES; index++) {
                    TEST_ASSERT_EQUAL(false, volumeManager.volumeAtIndex(index)->isAllocated());
                }

                { /* Initialize */
                    rc = mtd2.Initialize(virtualMTDCallbackHandler);
                    TEST_ASSERT_EQUAL(1, rc);
                }
            }

            sizeofDataOperation = (SIZE1 > BUFFER_SIZE) ? BUFFER_SIZE : SIZE1;
            sizeofDataOperation = (SIZE2 > sizeofDataOperation) ? sizeofDataOperation : SIZE2;
            TEST_ASSERT((sizeofDataOperation > 0) && (sizeofDataOperation <= BUFFER_SIZE));
            memset(buffer, PATTERN_FOR_PROGRAM_DATA, sizeofDataOperation);

            /* intentional fall-through */

        case ERASE1:
            rc = mtd1.Erase(0, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd1.GetCapabilities().asynchronous_ops);
                state = PROGRAM_DATA1;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case PROGRAM_DATA1:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            tr_info("PROGRAM_DATA1");
            rc = mtd1.ProgramData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd1.GetCapabilities().asynchronous_ops);

                ARM_STORAGE_STATUS status;
                status = drv->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = mtd1.GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = mtd2.GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);

                rc = mtd2.ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = mtd1.ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = mtd1.ReadData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = mtd1.Erase(0, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);

                state = ERASE2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case ERASE2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            rc = mtd2.Erase(0, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd2.GetCapabilities().asynchronous_ops);
                state = PROGRAM_DATA2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case PROGRAM_DATA2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            tr_info("PROGRAM_DATA2");
            rc = mtd2.ProgramData(0, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, mtd2.GetCapabilities().asynchronous_ops);

                ARM_STORAGE_STATUS status;
                status = drv->GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = mtd2.GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);
                status = mtd1.GetStatus();
                TEST_ASSERT_EQUAL(1, status.busy);
                TEST_ASSERT_EQUAL(0, status.error);

                rc = mtd1.ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = mtd2.ProgramData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = mtd2.ReadData(0, buffer, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);
                rc = mtd2.Erase(0, sizeofDataOperation);
                TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_BUSY, rc);

                state = DISCONNECT_VOLUME_MANAGER_CALLBACK;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case DISCONNECT_VOLUME_MANAGER_CALLBACK:
            tr_info("DISCONNECT_VOLUME_MANAGER_CALLBACK");
            rc = drv->Initialize(mtdCallbackHandler);
            TEST_ASSERT_EQUAL(1, rc); /* expect synchronous completion */
            /* intentional fallthrough */

        case READ_FROM_DRV_AFTER_PROGRAM_DATA1:
            tr_info("verifying state");
            /* Read after Program */
            rc = drv->ReadData(OFFSET1, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA1;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA1:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            /* intentional fallthrough */

        case READ_FROM_DRV_AFTER_PROGRAM_DATA2:
            /* Read after Program */
            rc = drv->ReadData(OFFSET2, buffer, sizeofDataOperation);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                state = VERIFY_PROGRAM_DATA2;
                return CaseTimeout(200) + CaseRepeatAll;
            }

            virtualVolumeCallbackStatus = rc;
            /* intentional fallthrough */

        case VERIFY_PROGRAM_DATA2:
            TEST_ASSERT_EQUAL(sizeofDataOperation, virtualVolumeCallbackStatus);

            for (uint32_t index = 0; index < sizeofDataOperation; index++) {
                if ((buffer)[index] != PATTERN_FOR_PROGRAM_DATA) {
                    tr_info("%s:%u: %" PRIu32 ": expected 0x%02x, found 0x%02x", __FUNCTION__, __LINE__, index, PATTERN_FOR_PROGRAM_DATA, buffer[index]);
                }
                TEST_ASSERT_EQUAL(PATTERN_FOR_PROGRAM_DATA, buffer[index]);
            }
            break;

        default:
            TEST_ASSERT(false);
    }

    return CaseNext;
}

// Specify all your test cases here
Case cases[] = {
    Case("initialize",                                    test_initialize),
    Case("Against a single volume at offset",             test_againstSingleVolumeAtOffset<0>),
    Case("Against a single volume at offset",             test_againstSingleVolumeAtOffset<4096>),
    Case("Against a single volume at offset",             test_againstSingleVolumeAtOffset<8192>),
    Case("Against a single volume at offset",             test_againstSingleVolumeAtOffset<65536>),
    Case("Against a single C_Storage at offset",          test_againstSingleCStorageAtOffset<0>),
    Case("Against a single C_Storage at offset",          test_againstSingleCStorageAtOffset<4096>),
    Case("Against a single C_Storage at offset",          test_againstSingleCStorageAtOffset<8192>),
    Case("Against a single C_Storage at offset",          test_againstSingleCStorageAtOffset<65536>),

    /* note: the following tests are unportable in the sense that they require the underlying storage device to support certain address ranges. */
    Case("Concurrent accesss from two volumes",           test_concurrentAccessFromTwoVolumes<512*1024, 128*1024, (512+128)*1024, 128*1024>),
    Case("Concurrent accesss from two volumes",           test_concurrentAccessFromTwoVolumes<512*1024, 128*1024, (512+128)*1024, 128*1024>),
    Case("Concurrent accesss from two volumes",           test_concurrentAccessFromTwoVolumes<512*1024, 128*1024, (512+256)*1024, 128*1024>),
    Case("Concurrent accesss from two volumes",           test_concurrentAccessFromTwoVolumes<512*1024, 128*1024, (512+384)*1024, 128*1024>),
    Case("Concurrent accesss from two C_Storage devices", test_concurrentAccessFromTwoCStorageDevices<512*1024, 128*1024, (512+128)*1024, 128*1024>),
    Case("Concurrent accesss from two C_Storage devices", test_concurrentAccessFromTwoCStorageDevices<512*1024, 128*1024, (512+256)*1024, 128*1024>),
    Case("Concurrent accesss from two C_Storage devices", test_concurrentAccessFromTwoCStorageDevices<512*1024, 128*1024, (512+384)*1024, 128*1024>),
};

// Declare your test specification with a custom setup handler
#ifndef AVOID_GREENTEA
Specification specification(greentea_setup, cases);
#else
Specification specification([](const size_t) {return STATUS_CONTINUE;}, cases);
#endif

int main(int argc, char** argv)
{
    mbed_trace_init();       // initialize the trace library
    mbed_trace_config_set(TRACE_MODE_COLOR | TRACE_ACTIVE_LEVEL_INFO | TRACE_CARRIAGE_RETURN);

    // Run the test specification
    Harness::run(specification);
}
