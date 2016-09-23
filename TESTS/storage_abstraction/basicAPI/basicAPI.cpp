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

#if !DEVICE_STORAGE
    #error [NOT_SUPPORTED] Storage not supported for this target
#endif

#ifndef AVOID_GREENTEA
#include "greentea-client/test_env.h"
#endif
#include "utest/utest.h"
#include "unity/unity.h"

#include "storage_abstraction/Driver_Storage.h"

#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F;
ARM_DRIVER_STORAGE *drv = &ARM_Driver_Storage_MTD_K64F;

/* temporary buffer to hold data for testing. */
static const unsigned BUFFER_SIZE = 16384;
static uint8_t buffer[BUFFER_SIZE];

/* forward declaration */
void initializationCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation);

/*
 * Most tests need some basic initialization of the driver before proceeding
 * with their operations.
 */
static control_t preambleForBasicInitialization(void)
{
    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    int32_t rc = drv->Initialize(initializationCompleteCallback);
    TEST_ASSERT(rc >= ARM_DRIVER_OK);
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1,  capabilities.asynchronous_ops);
        return CaseTimeout(200) + CaseRepeatAll;
    } else {
        TEST_ASSERT(rc == 1);
        return CaseRepeatAll;
    }
}

template<typename T>
static void verifyBytePattern(uint64_t addr, size_t sizeofData, T bytePattern)
{
    /* we're limited by BUFFER_SIZE in how much we can verify in a single iteration;
     * the variable 'amountBeingVerified' captures the size being verified in each
     * iteration. */
    size_t amountBeingVerified = sizeofData;
    if (amountBeingVerified > BUFFER_SIZE) {
        amountBeingVerified = BUFFER_SIZE;
    }
    TEST_ASSERT((amountBeingVerified % sizeof(T)) == 0);

    while (sizeofData) {
        int32_t rc = drv->ReadData(addr, buffer, amountBeingVerified);
        TEST_ASSERT_EQUAL(amountBeingVerified, rc);
        for (size_t index = 0; index < amountBeingVerified / sizeof(T); index++) {
            // if (bytePattern != ((const T *)buffer)[index]) {
            //     printf("%u: expected %x, found %x\n", index, bytePattern, ((const T *)buffer)[index]);
            // }
            TEST_ASSERT_EQUAL(bytePattern, ((const T *)buffer)[index]);
        }

        sizeofData -= amountBeingVerified;
        addr       += amountBeingVerified;
    }
}

void test_getVersion()
{
    ARM_DRIVER_VERSION version = drv->GetVersion();

    TEST_ASSERT_EQUAL(version.api, ARM_STORAGE_API_VERSION);
    TEST_ASSERT_EQUAL(version.drv, ARM_DRIVER_VERSION_MAJOR_MINOR(1,00));
}

void test_getCapabilities()
{
    TEST_ASSERT(sizeof(ARM_STORAGE_CAPABILITIES) == sizeof(uint32_t));

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();
    TEST_ASSERT_EQUAL(0, capabilities.reserved);
}

void test_getInfo()
{
    ARM_STORAGE_INFO info = {};
    int32_t rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    TEST_ASSERT_EQUAL(0, info.security.reserved1);
    TEST_ASSERT_EQUAL(0, info.security.reserved2);
    TEST_ASSERT((info.program_cycles == ARM_STORAGE_PROGRAM_CYCLES_INFINITE) || (info.program_cycles > 0));
    TEST_ASSERT(info.total_storage > 0);
}

void initializationCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    printf("init complete callback\n");
    TEST_ASSERT_EQUAL(1, status);
    TEST_ASSERT_EQUAL(operation, ARM_STORAGE_OPERATION_INITIALIZE);

    Harness::validate_callback();
}

control_t test_initialize(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 3;
    printf("in test_initialize with call_count %u\n", call_count);

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    int32_t rc = drv->Initialize(initializationCompleteCallback);
    TEST_ASSERT(rc >= ARM_DRIVER_OK);
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return (call_count < REPEAT_INSTANCES) ? (CaseTimeout(200) + CaseRepeatAll) : (control_t) CaseNext;
    }

    TEST_ASSERT(rc == 1);
    return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
}

void uninitializationCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    printf("uninit complete callback\n");
    TEST_ASSERT_EQUAL(status, ARM_DRIVER_OK);
    TEST_ASSERT_EQUAL(operation, ARM_STORAGE_OPERATION_UNINITIALIZE);

    Harness::validate_callback();
}

control_t test_uninitialize(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 3;
    printf("in test_uninitialize with call_count %u\n", call_count);

    /* update the completion callback. */
    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    int32_t rc = drv->Uninitialize();
    if (call_count > 2) {
        /* the driver should return some error for repeated un-initialization. */
        TEST_ASSERT(rc < ARM_DRIVER_OK);
        return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
    }
    TEST_ASSERT(rc >= ARM_DRIVER_OK);
    if (rc == ARM_DRIVER_OK) {
        /* asynchronous operation */
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return CaseTimeout(200) + CaseRepeatAll;
    }

    /* synchronous operation */
    TEST_ASSERT(rc == 1);
    return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
}

void powerControlCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    printf("power control complete callback\n");
    TEST_ASSERT_EQUAL(status, ARM_DRIVER_OK);
    TEST_ASSERT_EQUAL(operation, ARM_STORAGE_OPERATION_POWER_CONTROL);

    Harness::validate_callback();
}

control_t test_powerControl(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 2;
    printf("in test_powerControl with call_count %u\n", call_count);

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    /* Update the completion callback to 'powerControlCompleteCallback'. */
    if (call_count == 2) {
        int32_t rc = drv->Initialize(powerControlCompleteCallback);
        TEST_ASSERT(rc == 1); /* Expect synchronous completion of initialization; the system must have been
                               * initialized by the previous iteration. */
    }

    int32_t rc = drv->PowerControl(ARM_POWER_FULL);
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
    } else {
        TEST_ASSERT(rc == 1);
        return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
    }
}

void readDataCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    printf("ReadData complete callback\n");
    TEST_ASSERT_EQUAL(status, ARM_DRIVER_OK);
    TEST_ASSERT_EQUAL(operation, ARM_STORAGE_OPERATION_READ_DATA);

    Harness::validate_callback();
}

control_t test_readData(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 5;
    printf("in test_readData with call_count %u\n", call_count);

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    /* Update the completion callback to 'readDataCompleteCallback'. */
    int32_t rc;
    if (call_count == 2) {
        rc = drv->Initialize(readDataCompleteCallback);
        TEST_ASSERT(rc == 1); /* Expect synchronous completion of initialization; the system must have been
                               * initialized by the previous iteration. */
    }

    /* Get the first block. */
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    TEST_ASSERT(firstBlock.size > 0);

    ARM_STORAGE_INFO info;
    rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    TEST_ASSERT(info.program_unit <= BUFFER_SIZE);
    TEST_ASSERT(firstBlock.size >= (REPEAT_INSTANCES - 1) * info.program_unit);

    /* choose an increasing address for each iteration. */
    uint64_t addr = firstBlock.addr + (call_count - 1) * info.program_unit;
    size_t sizeofData = info.program_unit;

    rc = drv->ReadData(addr, buffer, sizeofData);
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
    } else {
        TEST_ASSERT(rc > 0);
        return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
    }
}

void programDataCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    TEST_ASSERT(status >= 0);
    static unsigned programIteration = 0;

    static const uint32_t BYTE_PATTERN = 0xAA551122;
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));

    ARM_STORAGE_INFO info;
    int32_t rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    const uint64_t addr = firstBlock.addr + programIteration * firstBlock.attributes.erase_unit;
    size_t sizeofData = info.program_unit;
    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    TEST_ASSERT((operation == ARM_STORAGE_OPERATION_ERASE) || (operation == ARM_STORAGE_OPERATION_PROGRAM_DATA));
    if (operation == ARM_STORAGE_OPERATION_ERASE) {
        // printf("programming %u bytes at address %lu with pattern 0x%" PRIx32 "\n", sizeofData, (uint32_t)addr, BYTE_PATTERN);

        size_t sizeofData = info.program_unit;
        TEST_ASSERT(BUFFER_SIZE >= sizeofData);
        TEST_ASSERT((sizeofData % sizeof(uint32_t)) == 0);
        for (size_t index = 0; index < sizeofData / sizeof(uint32_t); index++) {
            ((uint32_t *)buffer)[index] = BYTE_PATTERN;
        }

        status = drv->ProgramData(addr, buffer, sizeofData);
        if (status < ARM_DRIVER_OK) {
            return; /* failure. this will trigger a timeout and cause test failure. */
        }
        if (status == ARM_DRIVER_OK) {
            TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
            return; /* We've successfully pended a programData operation; we'll have another
                     * invocation of this callback when programming completes. */
        }
    }

    /* We come here either because of completion for program-data or as a very
     * unlikely fall through from synchronous completion of program-data (above). */

#ifndef __CC_ARM
    printf("verifying programmed sector at addr %lu\n", (uint32_t)addr);
#endif
    verifyBytePattern(addr, sizeofData, BYTE_PATTERN);
    ++programIteration;

    Harness::validate_callback();
}

control_t test_programDataUsingProgramUnit(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 5;
    printf("in test_programDataUsingProgramUnit with call_count %u\n", call_count);

    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    /* Get the first block. */
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    TEST_ASSERT(firstBlock.size > 0);

    ARM_STORAGE_INFO info;
    int32_t rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    TEST_ASSERT(info.program_unit <= firstBlock.attributes.erase_unit);
    TEST_ASSERT(firstBlock.size >= (REPEAT_INSTANCES - 1) * firstBlock.attributes.erase_unit);

    /* initialize the buffer to hold the pattern. */
    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    /* Update the completion callback to 'programDataCompleteCallback'. */
    if (call_count == 2) {
        int32_t rc = drv->Initialize(programDataCompleteCallback);
        TEST_ASSERT(rc == 1); /* Expect synchronous completion of initialization; the system must have been
                               * initialized by the previous iteration. */
    }

    /* choose an increasing address for each iteration. */
    uint64_t addr = firstBlock.addr + (call_count - 2) * firstBlock.attributes.erase_unit;

    /* erase the sector at 'addr' */
    printf("erasing sector at addr %lu\n", (uint32_t)addr);
    rc = drv->Erase(addr, firstBlock.attributes.erase_unit);
    TEST_ASSERT(rc >= 0);
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
    } else {
        TEST_ASSERT_EQUAL(firstBlock.attributes.erase_unit, rc);
        verifyBytePattern(addr, firstBlock.attributes.erase_unit, info.erased_value ? (uint8_t)0xFF : (uint8_t)0);

        static const uint32_t BYTE_PATTERN = 0xAA551122;
        size_t sizeofData = info.program_unit;
        TEST_ASSERT(BUFFER_SIZE >= sizeofData);
        TEST_ASSERT((sizeofData % sizeof(uint32_t)) == 0);
        for (size_t index = 0; index < sizeofData / sizeof(uint32_t); index++) {
            ((uint32_t *)buffer)[index] = BYTE_PATTERN;
        }

        /* program the sector at addr */
        // printf("programming %u bytes at address %lu with pattern 0x%" PRIx32 "\n", sizeofData, (uint32_t)addr, BYTE_PATTERN);
        rc = drv->ProgramData((uint32_t)addr, buffer, sizeofData);
        if (rc == ARM_DRIVER_OK) {
            TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
            return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
        } else {
            TEST_ASSERT(rc > 0);

            printf("verifying programmed sector at addr %lu\n", (uint32_t)addr);
            verifyBytePattern(addr, sizeofData, BYTE_PATTERN);

            return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
        }
    }
}

void programDataOptimalCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    TEST_ASSERT(status >= 0);
    static unsigned programIteration = 0;

    static const uint8_t BYTE_PATTERN = 0xAA;
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    const uint64_t addr = firstBlock.addr + programIteration * firstBlock.attributes.erase_unit;

    ARM_STORAGE_INFO info;
    int32_t rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    size_t sizeofData = info.optimal_program_unit;
    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    TEST_ASSERT((operation == ARM_STORAGE_OPERATION_ERASE) || (operation == ARM_STORAGE_OPERATION_PROGRAM_DATA));
    if (operation == ARM_STORAGE_OPERATION_ERASE) {
#ifndef __CC_ARM
        printf("programming %u bytes at address %lu with pattern 0x%x\n", sizeofData, (uint32_t)addr, BYTE_PATTERN);
#endif
        size_t sizeofData = info.optimal_program_unit;
        TEST_ASSERT(BUFFER_SIZE >= sizeofData);
        memset(buffer, BYTE_PATTERN, sizeofData);

        status = drv->ProgramData(addr, buffer, sizeofData);
        if (status < ARM_DRIVER_OK) {
            return; /* failure. this will trigger a timeout and cause test failure. */
        }
        if (status == ARM_DRIVER_OK) {
            TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
            return; /* We've successfully pended a programData operation; we'll have another
                     * invocation of this callback when programming completes. */
        }
    }

    /* We come here either because of completion for program-data or as a very
     * unlikely fall through from synchronous completion of program-data (above). */

#ifndef __CC_ARM
    printf("verifying programmed sector at addr %lu\n", (uint32_t)addr);
#endif
    verifyBytePattern(addr, sizeofData, BYTE_PATTERN);
    ++programIteration;

    Harness::validate_callback();
}

control_t test_programDataUsingOptimalProgramUnit(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 5;
    printf("in test_programDataUsingOptimalProgramUnit with call_count %u\n", call_count);

    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    /* Get the first block. */
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    TEST_ASSERT(firstBlock.size > 0);

    ARM_STORAGE_INFO info;
    int32_t rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    TEST_ASSERT(info.optimal_program_unit <= firstBlock.attributes.erase_unit);
    TEST_ASSERT(firstBlock.size >= (REPEAT_INSTANCES - 1) * firstBlock.attributes.erase_unit);

    /* initialize the buffer to hold the pattern. */
    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    /* Update the completion callback to 'programDataCompleteCallback'. */
    if (call_count == 2) {
        int32_t rc = drv->Initialize(programDataOptimalCompleteCallback);
        TEST_ASSERT(rc == 1); /* Expect synchronous completion of initialization; the system must have been
                               * initialized by the previous iteration. */
    }

    /* choose an increasing address for each iteration. */
    uint64_t addr = firstBlock.addr + (call_count - 2) * firstBlock.attributes.erase_unit;

    /* erase the sector at 'addr' */
    printf("erasing sector at addr %lu\n", (uint32_t)addr);
    rc = drv->Erase(addr, firstBlock.attributes.erase_unit);
    TEST_ASSERT(rc >= 0);
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
    } else {
        TEST_ASSERT_EQUAL(firstBlock.attributes.erase_unit, rc);
        verifyBytePattern(addr, firstBlock.attributes.erase_unit, info.erased_value ? (uint8_t)0xFF : (uint8_t)0);

        static const uint8_t BYTE_PATTERN = 0xAA;
        size_t sizeofData = info.optimal_program_unit;
        TEST_ASSERT(BUFFER_SIZE >= sizeofData);
        memset(buffer, BYTE_PATTERN, sizeofData);

        /* program the sector at addr */
        printf("programming %u bytes at address %lu with pattern 0x%x\n", sizeofData, (uint32_t)addr, BYTE_PATTERN);
        rc = drv->ProgramData((uint32_t)addr, buffer, sizeofData);
        if (rc == ARM_DRIVER_OK) {
            TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
            return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
        } else {
            TEST_ASSERT_EQUAL(sizeofData, rc);

            printf("verifying programmed sector at addr %lu\n", (uint32_t)addr);
            verifyBytePattern(addr, sizeofData, BYTE_PATTERN);

            return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
        }
    }
}

void test_eraseWithInvalidParameters(void)
{
    int32_t rc;

    rc = drv->Erase(0, 0);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);

    /* operate before the start of the first block. */
    ARM_STORAGE_BLOCK block;
    rc = drv->GetNextBlock(NULL, &block); /* get the first block */
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&block));
    TEST_ASSERT(block.size > 0);
    rc = drv->Erase(block.addr - 1, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);

    /* operate at an address past the end of the last block */
    uint64_t endAddr = block.addr + block.size;
    for (; ARM_STORAGE_VALID_BLOCK(&block); drv->GetNextBlock(&block, &block)) {
        endAddr = block.addr + block.size;
    }
    rc = drv->Erase(endAddr + 1, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);

    ARM_STORAGE_INFO info;
    rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    drv->GetNextBlock(NULL, &block); /* get the first block */
    TEST_ASSERT(block.size >= block.attributes.erase_unit);
    TEST_ASSERT((block.size % block.attributes.erase_unit) == 0);

    rc = drv->Erase(block.addr + 1, block.attributes.erase_unit);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->Erase(block.addr, block.attributes.erase_unit - 1);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->Erase(block.addr, block.attributes.erase_unit + 1);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->Erase(block.addr, block.attributes.erase_unit / 2);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
}

template<size_t ERASE_UNITS_PER_ITERATION>
void eraseCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    static unsigned eraseIteration = 0;
#ifndef __CC_ARM
    printf("erase<%u> complete callback: iteration %u\n", ERASE_UNITS_PER_ITERATION, eraseIteration);
#endif
    TEST_ASSERT_EQUAL(operation, ARM_STORAGE_OPERATION_ERASE);

    /* test that the actual sector has been erased */
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    TEST_ASSERT_EQUAL(ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit, status);

    const uint64_t addr = firstBlock.addr + eraseIteration * ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit;
    ++eraseIteration;

    ARM_STORAGE_INFO info;
    int32_t rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    //printf("testing erased sector at addr %lu", (uint32_t)addr);
    verifyBytePattern(addr, ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit, info.erased_value ? (uint8_t)0xFF : (uint8_t)0);

    Harness::validate_callback();
}

template <size_t ERASE_UNITS_PER_ITERATION>
control_t test_erase(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 5;
    printf("in test_erase<%u> with call_count %u\n", ERASE_UNITS_PER_ITERATION, call_count);

    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    /* Get the first block. */
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    TEST_ASSERT(firstBlock.size > 0);
    if (firstBlock.size < ((call_count - 1) * ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit)) {
        printf("firstBlock isn't large enough to support instance %u of test_erase<%u>\n", call_count, ERASE_UNITS_PER_ITERATION);
        return CaseNext;
    }

    /* Update the completion callback to 'eraseCompleteCallback'. */
    if (call_count == 2) {
        int32_t rc = drv->Initialize(eraseCompleteCallback<ERASE_UNITS_PER_ITERATION>);
        TEST_ASSERT(rc == 1); /* Expect synchronous completion of initialization; the system must have been
                               * initialized by the previous iteration. */
    }

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    /* choose an increasing address for each iteration. */
    uint64_t addr = firstBlock.addr + (call_count - 2) * ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit;

    printf("erasing %lu bytes at addr %lu\n", (ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit), (uint32_t)addr);
    int32_t rc = drv->Erase(addr, ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit);
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
    } else {
        TEST_ASSERT_EQUAL(ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit, rc);

        ARM_STORAGE_INFO info;
        rc = drv->GetInfo(&info);
        TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

        /* test that the actual sector has been erased */
        printf("testing erased sector at addr %lu\n", (uint32_t)addr);
        verifyBytePattern(addr, ERASE_UNITS_PER_ITERATION * firstBlock.attributes.erase_unit, (uint8_t)0xFF);

        return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
    }
}

void eraseChipCompleteCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
#ifndef __CC_ARM
    printf("eraseChip complete callback\n");
#endif
    TEST_ASSERT_EQUAL(status, ARM_DRIVER_OK);
    TEST_ASSERT_EQUAL(operation, ARM_STORAGE_OPERATION_ERASE_ALL);

    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    uint64_t addr = firstBlock.addr;

    /* test that the flash has been erased */
#ifndef __CC_ARM
    printf("testing erased chip\n");
#endif
    unsigned index = 0;
    static const unsigned MAX_VERIFY_ITERATIONS = 5;
    while ((index < MAX_VERIFY_ITERATIONS) && (addr < (firstBlock.addr + firstBlock.size))) {
        // printf("testing erased chip at addr %lu\n", (uint32_t)addr);
        verifyBytePattern(addr, firstBlock.attributes.erase_unit, (uint8_t)0xFF);

        index++;
        addr += firstBlock.attributes.erase_unit;
    }

    Harness::validate_callback();
}

control_t test_eraseAll(const size_t call_count)
{
    static const unsigned REPEAT_INSTANCES = 5;
    printf("in test_eraseAll with call_count %u\n", call_count);

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();
    if (!capabilities.erase_all) {
        printf("chip erase not supported on this flash\n");
        return CaseNext;
    }

    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    /* Update the completion callback to 'eraseChipCompleteCallback'. */
    if (call_count == 2) {
        int32_t rc = drv->Initialize(eraseChipCompleteCallback);
        TEST_ASSERT(rc == 1); /* Expect synchronous completion of initialization; the system must have been
                               * initialized by the previous iteration. */
    }

    /* Get the first block. */
    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    TEST_ASSERT(firstBlock.size > 0);
    uint64_t addr = firstBlock.addr;
    printf("erasing chip\n");

    int32_t rc = drv->EraseAll();
    if (rc == ARM_DRIVER_OK) {
        TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
        return (call_count < REPEAT_INSTANCES) ? CaseTimeout(200) + CaseRepeatAll: CaseTimeout(200);
    } else {
        TEST_ASSERT(rc == 1);

        /* test that the flash has been erased */
        unsigned index = 0;
        static const unsigned MAX_VERIFY_ITERATIONS = 5;
        while ((index < MAX_VERIFY_ITERATIONS) && (addr < (firstBlock.addr + firstBlock.size))) {
            //printf("testing erased chip at addr %lu", (uint32_t)addr);
            ARM_STORAGE_INFO info;
            rc = drv->GetInfo(&info);
            TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

            verifyBytePattern(addr, firstBlock.attributes.erase_unit, info.erased_value ? (uint8_t)0xFF : (uint8_t)0);

            index++;
            addr += firstBlock.attributes.erase_unit;
        }

        return (call_count < REPEAT_INSTANCES) ? CaseRepeatAll : CaseNext;
    }
}

void test_programDataWithInvalidParameters(void)
{
    int32_t rc;

    rc = drv->ProgramData(0, NULL, 0);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->ProgramData(0, buffer, 0);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->ProgramData(0, NULL, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);

    /* operate before the start of the first block. */
    ARM_STORAGE_BLOCK block;
    rc = drv->GetNextBlock(NULL, &block); /* get the first block */
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&block));
    TEST_ASSERT(block.size > 0);
    rc = drv->ProgramData(block.addr - 1, buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);

    /* operate at an address past the end of the last block */
    uint64_t endAddr = block.addr + block.size;
    for (; ARM_STORAGE_VALID_BLOCK(&block); drv->GetNextBlock(&block, &block)) {
        endAddr = block.addr + block.size;
    }
    rc = drv->ProgramData(endAddr + 1, buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);

    ARM_STORAGE_INFO info;
    rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);
    if (info.program_unit <= 1) {
        return; /* if program_unit is 1 (or 0), we can't proceed with any alignment tests */
    }

    drv->GetNextBlock(NULL, &block); /* get the first block */

    TEST_ASSERT(block.size >= info.program_unit);

    rc = drv->ProgramData(block.addr + 1, buffer, info.program_unit);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->ProgramData(block.addr, buffer, info.program_unit - 1);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->ProgramData(block.addr, buffer, info.program_unit + 1);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
    rc = drv->ProgramData(block.addr, buffer, info.program_unit / 2);
    TEST_ASSERT_EQUAL(ARM_DRIVER_ERROR_PARAMETER, rc);
}

template <size_t N_UNITS>
void programDataWithMultipleProgramUnitsCallback(int32_t status, ARM_STORAGE_OPERATION operation)
{
    TEST_ASSERT(status >= ARM_DRIVER_OK);

    ARM_STORAGE_BLOCK firstBlock;
    drv->GetNextBlock(NULL, &firstBlock); /* get first block */
    TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
    TEST_ASSERT(firstBlock.size > 0);

    ARM_STORAGE_INFO info;
    int32_t rc = drv->GetInfo(&info);
    TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

    ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

    size_t rangeNeededForTest = (N_UNITS * info.program_unit);
    /* round-up range to the nearest erase_unit */
    rangeNeededForTest = ((rangeNeededForTest + firstBlock.attributes.erase_unit - 1) / firstBlock.attributes.erase_unit) * firstBlock.attributes.erase_unit;

    static const uint32_t BYTE_PATTERN = 0xABCDEF00;

    if (operation == ARM_STORAGE_OPERATION_ERASE) {
        TEST_ASSERT_EQUAL(rangeNeededForTest, status);
        TEST_ASSERT((N_UNITS * info.program_unit) <= BUFFER_SIZE);

        /* setup byte pattern in buffer */
        if (info.program_unit >= sizeof(BYTE_PATTERN)) {
            for (size_t index = 0; index < ((N_UNITS * info.program_unit) / sizeof(BYTE_PATTERN)); index++) {
                ((uint32_t *)buffer)[index] = BYTE_PATTERN;
            }
        } else {
           for (size_t index = 0; index < ((N_UNITS * info.program_unit)); index++) {
               buffer[index] = ((const uint8_t *)&BYTE_PATTERN)[0];
           }
        }

#ifndef __CC_ARM
        printf("Callback: programming %lu bytes at address %lu with pattern 0x%lx\n", (N_UNITS * info.program_unit), (uint32_t)firstBlock.addr, BYTE_PATTERN);
#endif
        rc = drv->ProgramData(firstBlock.addr, buffer, (N_UNITS * info.program_unit));
        TEST_ASSERT(rc >= ARM_DRIVER_OK);
        if (rc == ARM_DRIVER_OK) {
            TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
            return; /* We've successfully pended a programData operation; we'll have another
                     * invocation of this callback when programming completes. */
        }

        status = rc;
    }

    TEST_ASSERT_EQUAL((N_UNITS * info.program_unit), status);

#ifndef __CC_ARM
    printf("Callback: verifying programmed sector at addr %lu\n", (uint32_t)firstBlock.addr);
#endif
    if (info.program_unit >= sizeof(BYTE_PATTERN)) {
        verifyBytePattern(firstBlock.addr, (N_UNITS * info.program_unit), BYTE_PATTERN);
    } else {
        verifyBytePattern(firstBlock.addr, (N_UNITS * info.program_unit), ((const uint8_t *)&BYTE_PATTERN)[0]);
    }

    Harness::validate_callback();
}

template<size_t N_UNITS>
control_t test_programDataWithMultipleProgramUnits(const size_t call_count)
{
    int32_t rc;
    printf("in test_programDataWithMultipleProgramUnits<%u> with call_count %u\n", N_UNITS, call_count);

    if (call_count == 1) {
        /* Achieve basic initialization for the driver before anything else. */
        return preambleForBasicInitialization();
    }

    /* Update the completion callback to 'programDataWithMultipleProgramUnitsCallback'. */
    if (call_count == 2) {
        rc = drv->Initialize(programDataWithMultipleProgramUnitsCallback<N_UNITS>);
        TEST_ASSERT(rc == 1); /* Expect synchronous completion of initialization; the system must have been
                               * initialized by the previous iteration. */

        ARM_STORAGE_BLOCK firstBlock;
        drv->GetNextBlock(NULL, &firstBlock); /* get first block */
        TEST_ASSERT(ARM_STORAGE_VALID_BLOCK(&firstBlock));
        TEST_ASSERT(firstBlock.size > 0);

        ARM_STORAGE_INFO info;
        int32_t rc = drv->GetInfo(&info);
        TEST_ASSERT_EQUAL(ARM_DRIVER_OK, rc);

        ARM_STORAGE_CAPABILITIES capabilities = drv->GetCapabilities();

        size_t rangeNeededForTest = (N_UNITS * info.program_unit);
        /* round-up range to the nearest erase_unit */
        rangeNeededForTest = ((rangeNeededForTest + firstBlock.attributes.erase_unit - 1) / firstBlock.attributes.erase_unit) * firstBlock.attributes.erase_unit;
        if (firstBlock.size < rangeNeededForTest) {
            printf("first block not large enough; rangeNeededForTest: %u\n", rangeNeededForTest);
            return CaseNext; /* first block isn't large enough for the intended operation */
        }

        if (rangeNeededForTest > BUFFER_SIZE) {
            printf("buffer (%u) not large enough; rangeNeededForTest: %u\n", BUFFER_SIZE, rangeNeededForTest);
            return CaseNext;
        }

        // printf("erasing %u bytes at addr %lu\n", rangeNeededForTest, (uint32_t)firstBlock.addr);
        rc = drv->Erase(firstBlock.addr, rangeNeededForTest);
        TEST_ASSERT(rc >= 0);
        if (rc == ARM_DRIVER_OK) {
            TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
            return CaseTimeout(500);
        } else {
            TEST_ASSERT_EQUAL(rangeNeededForTest, rc);

            /* setup byte pattern in buffer */
            static const uint32_t BYTE_PATTERN = 0xABCDEF00;
            if (info.program_unit >= sizeof(BYTE_PATTERN)) {
                for (size_t index = 0; index < ((N_UNITS * info.program_unit) / sizeof(BYTE_PATTERN)); index++) {
                    ((uint32_t *)buffer)[index] = BYTE_PATTERN;
                }
            } else {
               for (size_t index = 0; index < ((N_UNITS * info.program_unit)); index++) {
                   buffer[index] = ((const uint8_t *)&BYTE_PATTERN)[0];
               }
            }

            printf("programming %lu bytes at address %lu with pattern 0x%lx\n", (N_UNITS * info.program_unit), (uint32_t)firstBlock.addr, BYTE_PATTERN);
            rc = drv->ProgramData(firstBlock.addr, buffer, (N_UNITS * info.program_unit));
            TEST_ASSERT(rc >= 0);
            if (rc == ARM_DRIVER_OK) {
                TEST_ASSERT_EQUAL(1, capabilities.asynchronous_ops);
                return CaseTimeout(500);
            } else {
                TEST_ASSERT_EQUAL((N_UNITS * info.program_unit), rc);

                printf("verifying programmed sector at addr %lu\n", (uint32_t)firstBlock.addr);
                if (info.program_unit >= sizeof(BYTE_PATTERN)) {
                    verifyBytePattern(firstBlock.addr, (N_UNITS * info.program_unit), BYTE_PATTERN);
                } else {
                    verifyBytePattern(firstBlock.addr, (N_UNITS * info.program_unit), ((const uint8_t *)&BYTE_PATTERN)[0]);
                }

                return CaseNext;
            }
        }
    }

    return CaseNext;
}

#ifndef AVOID_GREENTEA
// Custom setup handler required for proper Greentea support
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    // Call the default reporting function
    return greentea_test_setup_handler(number_of_cases);
}
#else
status_t default_setup(const size_t)
{
    return STATUS_CONTINUE;
}
#endif

// Specify all your test cases here
Case cases[] = {
    Case("get version",                              test_getVersion),
    Case("get capabilities",                         test_getCapabilities),
    Case("get info",                                 test_getInfo),
    Case("initialize",                               test_initialize),
    Case("uninitialize",                             test_uninitialize),
    Case("power control",                            test_powerControl),
    Case("erase all",                                test_eraseAll),
    Case("read data",                                test_readData),
    Case("erase with invalid parameters",            test_eraseWithInvalidParameters),
    Case("erase single unit",                        test_erase<1>),
    Case("erase two units",                          test_erase<2>),
    Case("erase four units",                         test_erase<4>),
    Case("erase eight units",                        test_erase<8>),
    Case("program data with invalid parameters",     test_programDataWithInvalidParameters),
    Case("program data using program_unit",          test_programDataUsingProgramUnit),
    Case("program data using optimal_program_unit",  test_programDataUsingOptimalProgramUnit),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<1>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<2>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<7>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<8>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<9>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<31>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<32>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<33>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<127>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<128>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<129>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<1023>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<1024>),
    Case("program data with multiple program units", test_programDataWithMultipleProgramUnits<1025>),
};

// Declare your test specification with a custom setup handler
#ifndef AVOID_GREENTEA
Specification specification(greentea_setup, cases);
#else
Specification specification(default_setup, cases);
#endif

int main(int argc, char** argv)
{
    // Run the test specification
    Harness::run(specification);
}
