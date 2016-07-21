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

#ifdef TARGET_LIKE_POSIX
#define AVOID_GREENTEA
#endif

#ifndef AVOID_GREENTEA
#include "greentea-client/test_env.h"
#endif
#include "utest/utest.h"
#include "unity/unity.h"

#include "flash-journal-strategy-sequential/flash_journal_strategy_sequential.h"
#include "flash-journal-strategy-sequential/flash_journal_private.h"
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

extern ARM_DRIVER_STORAGE ARM_Driver_Storage_(0);
ARM_DRIVER_STORAGE *drv = &ARM_Driver_Storage_(0);

FlashJournal_t      journal;

static const size_t BUFFER_SIZE = 8192;
static uint8_t      buffer[BUFFER_SIZE];

static const size_t SIZEOF_SMALL_WRITE = 8;
static const size_t SIZEOF_LARGE_WRITE = BUFFER_SIZE;
static int32_t      callbackStatus;

void callbackHandler(int32_t status, FlashJournal_OpCode_t cmd_code)
{
    callbackStatus = status;

    switch (cmd_code) {
        case FLASH_JOURNAL_OPCODE_INITIALIZE:
            // printf("journal_callbackHandler: callback for init with status %" PRId32 "\n", status);
            break;

        case FLASH_JOURNAL_OPCODE_READ_BLOB:
            // printf("journal_callbackHandler: callback for read with status %" PRId32 "\n", status);
            break;

        case FLASH_JOURNAL_OPCODE_LOG_BLOB:
            // printf("journal_callbackHandler: callback for log with status %" PRId32 "\n", status);
            break;

        case FLASH_JOURNAL_OPCODE_COMMIT:
            // printf("journal_callbackHandler: callback for commit with status %" PRId32 "\n", status);
            break;

        case FLASH_JOURNAL_OPCODE_RESET:
            // printf("journal_callbackHandler: callback for reset with status %" PRId32 "\n", status);
            break;

        default:
            // printf("journal_callbackHandler: callback for opcode %u with status %" PRId32 "\n", cmd_code, status);
            break;
    }
    Harness::validate_callback(); // Validate the callback
}

control_t test_initialize()
{
    int32_t rc = FlashJournal_initialize(&journal, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, callbackHandler);
    TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
    if (rc == JOURNAL_STATUS_OK) {
        return CaseTimeout(200);
    }

    /* ensure that something got written into the memory of journal_t */
    FlashJournal_t mockJournal;
    memset(&mockJournal, 0, sizeof(FlashJournal_t));
    TEST_ASSERT_NOT_EQUAL(0, memcmp(&mockJournal, &journal, sizeof(FlashJournal_t)));

    FlashJournal_Info_t info;
    rc = FlashJournal_getInfo(&journal, &info);
    TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
    TEST_ASSERT(info.capacity > 0);

    return CaseNext;
}

control_t test_resetAndInitialize(const size_t call_count)
{
    int32_t rc;
    FlashJournal_Info_t info;
    SequentialFlashJournal_t *sequentialJournal = (SequentialFlashJournal_t *)&journal;

    static uint64_t previousCapacity;

    static enum {
        NEEDS_INITIAL_RESET,
        NEEDS_INITIALIZE_FOLLOWING_RESET,
        NEEDS_VERIFICATION_FOLLOWING_INITIALIZE,
    } state;

    printf("test_resetAndInitialize: entered with call_count %u\n", call_count);
    if (call_count == 1) {
        state = NEEDS_INITIAL_RESET;
    }

    switch (state) {
        case NEEDS_INITIAL_RESET:
            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT(info.capacity > 0);
            previousCapacity = info.capacity;

            printf("test_resetAndInitialize: calling reset()\n");
            rc = FlashJournal_reset(&journal);
            TEST_ASSERT_NOT_EQUAL(JOURNAL_STATUS_UNSUPPORTED, rc);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            state = NEEDS_INITIALIZE_FOLLOWING_RESET;
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(1000) + CaseRepeatAll;
            }
            TEST_ASSERT_EQUAL(1, rc); /* synchronous completion of reset() is expected to return 1 */

            /* fall through */
        case NEEDS_INITIALIZE_FOLLOWING_RESET:
            /* ensure that the journal has been re-initialized */
            TEST_ASSERT_EQUAL(0, sequentialJournal->nextSequenceNumber);
            TEST_ASSERT_EQUAL((uint32_t)-1, sequentialJournal->currentBlobIndex);
            TEST_ASSERT_EQUAL(SEQUENTIAL_JOURNAL_STATE_INITIALIZED, sequentialJournal->state);

            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT(info.capacity > 0);
            TEST_ASSERT_EQUAL(previousCapacity, info.capacity);
            TEST_ASSERT_EQUAL(0, info.sizeofJournaledBlob);

            /* attempt an initialize following reset() */
            printf("test_resetAndInitialize: calling initialize() after reset\n");
            rc = FlashJournal_initialize(&journal, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, callbackHandler);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            state = NEEDS_VERIFICATION_FOLLOWING_INITIALIZE;
            if (rc == JOURNAL_STATUS_OK) {
                return CaseTimeout(200);
            }
            TEST_ASSERT_EQUAL(1, rc); /* synchronous completion of initialize() is expected to return 1 */

            /* fall through */
        case NEEDS_VERIFICATION_FOLLOWING_INITIALIZE:
        default:
            printf("test_resetAndInitialize: verification\n");
            TEST_ASSERT_EQUAL(0, sequentialJournal->nextSequenceNumber);
            TEST_ASSERT_EQUAL((uint32_t)-1, sequentialJournal->currentBlobIndex);
            TEST_ASSERT_EQUAL(SEQUENTIAL_JOURNAL_STATE_INITIALIZED, sequentialJournal->state);

            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT(info.capacity > 0);
            TEST_ASSERT_EQUAL(previousCapacity, info.capacity);
            TEST_ASSERT_EQUAL(0, info.sizeofJournaledBlob);
            break;
    }

    return CaseNext;
}

control_t test_commitWithoutLogs(const size_t call_count)
{
    int32_t rc;

    printf("test_commitWithoutLogs: entered with call_count %u\n", call_count);

    switch (call_count) {
        case 1:
            /* initialize */
            rc = FlashJournal_initialize(&journal, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, callbackHandler);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                return CaseTimeout(200) + CaseRepeatAll;
            }
            TEST_ASSERT_EQUAL(1, rc); /* synchronous completion of initialize() is expected to return 1 */
            return CaseRepeatAll;

        case 2:
            rc = FlashJournal_commit(&journal);
            // printf("commit returned %" PRId32 "\r\n", rc);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }

            /* intentional fall through*/
            callbackStatus = rc;

        case 3:
            TEST_ASSERT_EQUAL(1, callbackStatus);
            break;
    }

    return CaseNext;
}

control_t test_logSmallWithoutCommit(const size_t call_count)
{
    int32_t rc;

    printf("test_logSmallWithoutCommit: entered with call_count %u\n", call_count);

    switch (call_count) {
        case 1:
            /* initialize */
            rc = FlashJournal_initialize(&journal, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, callbackHandler);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                return CaseTimeout(200) + CaseRepeatAll;
            }
            TEST_ASSERT_EQUAL(1, rc); /* synchronous completion of initialize() is expected to return 1 */
            return CaseRepeatAll;

        case 2:
            /* log without commit */
            memset(buffer, 0xAA, SIZEOF_SMALL_WRITE);
            rc = FlashJournal_log(&journal, buffer, SIZEOF_SMALL_WRITE);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }
            /* else, fall through to synchronous verification */

        default:
            rc = FlashJournal_read(&journal, buffer, SIZEOF_SMALL_WRITE);
            TEST_ASSERT(rc < JOURNAL_STATUS_OK);
            return CaseNext;
    }
}

template <uint8_t PATTERN>
control_t test_logSmallAndCommit(const size_t call_count)
{
    int32_t rc;

    printf("test_logSmallAndCommit: entered with call_count %u\n", call_count);

    switch (call_count) {
        case 1:
            memset(buffer, PATTERN, SIZEOF_SMALL_WRITE);
            rc = FlashJournal_log(&journal, buffer, SIZEOF_SMALL_WRITE);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }
            /* else, fall through to synchronous verification */

        case 2:
            rc = FlashJournal_commit(&journal);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }
            /* else, fall through to synchronous verification */

        case 3:
            {
                FlashJournal_Info_t info;
                rc = FlashJournal_getInfo(&journal, &info);
                TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
                TEST_ASSERT_EQUAL(SIZEOF_SMALL_WRITE, info.sizeofJournaledBlob);
            }

            rc = FlashJournal_read(&journal, buffer, SIZEOF_SMALL_WRITE);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }

            TEST_ASSERT_EQUAL(SIZEOF_SMALL_WRITE, rc);
            /* intentional fall-through */

        default:
            for (unsigned i = 0; i < SIZEOF_SMALL_WRITE; i++) {
                // printf("index %u value %x\n", i, buffer[i]);
                TEST_ASSERT_EQUAL(PATTERN, buffer[i]);
            }

            return CaseNext;
    }
}

control_t test_initializeAfterLogSmallAndCommit(const size_t call_count)
{
    int32_t rc;

    printf("test_initializeAfterLogSmallAndCommit: entered with call_count %u\n", call_count);

    if (call_count == 1) {
        rc = FlashJournal_initialize(&journal, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, callbackHandler);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            printf("asynchronous_ops for init\n");
            return CaseTimeout(200) + CaseRepeatAll;
        }
        TEST_ASSERT_EQUAL(1, rc); /* synchronous completion of initialize() is expected to return 1 */
    }

    FlashJournal_Info_t info;
    rc = FlashJournal_getInfo(&journal, &info);
    TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
    TEST_ASSERT_EQUAL(SIZEOF_SMALL_WRITE, info.sizeofJournaledBlob);

    return CaseNext;
}

control_t test_logLargeWithoutCommit(const size_t call_count)
{
    int32_t rc;

    printf("test_logLargeWithoutCommit: entered with call_count %u\n", call_count);

    switch (call_count) {
        case 1:
            rc = FlashJournal_initialize(&journal, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, callbackHandler);
            TEST_ASSERT(rc >= ARM_DRIVER_OK);
            if (rc == ARM_DRIVER_OK) {
                return CaseTimeout(200) + CaseRepeatAll;
            }
            TEST_ASSERT_EQUAL(1, rc); /* synchronous completion of initialize() is expected to return 1 */
            return CaseRepeatAll;

        case 2:
            memset(buffer, 0xAA, SIZEOF_LARGE_WRITE);
            rc = FlashJournal_log(&journal, buffer, SIZEOF_LARGE_WRITE);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(5000) + CaseRepeatAll;
            }
            /* intentional fall-through */

        case 3:
        default:
            rc = FlashJournal_read(&journal, buffer, SIZEOF_LARGE_WRITE);
            TEST_ASSERT(rc < JOURNAL_STATUS_OK);
            return CaseNext;
    }
}

template<uint8_t PATTERN>
control_t test_logLargeAndCommit(const size_t call_count)
{
    int32_t rc;

    printf("test_logLargeAndCommit: entered with call_count %u\n", call_count);

    switch (call_count) {
        case 1:
            memset(buffer, PATTERN, SIZEOF_LARGE_WRITE);
            rc = FlashJournal_log(&journal, buffer, SIZEOF_LARGE_WRITE);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }
            /* intentional fall-through */

        case 2:
            rc = FlashJournal_commit(&journal);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }
            /* intentional fall-through */

        case 3:
            {
                FlashJournal_Info_t info;
                rc = FlashJournal_getInfo(&journal, &info);
                TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
                TEST_ASSERT_EQUAL(SIZEOF_LARGE_WRITE, info.sizeofJournaledBlob);
            }

            rc = FlashJournal_read(&journal, buffer, SIZEOF_LARGE_WRITE);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }

            TEST_ASSERT_EQUAL(SIZEOF_LARGE_WRITE, rc);
            /* intentional fall-through */

        default:
            for (unsigned i = 0; i < SIZEOF_LARGE_WRITE; i++) {
                // printf("index %u value %x\n", i, buffer[i]);
                TEST_ASSERT_EQUAL(PATTERN, buffer[i]);
            }

            return CaseNext;
    }
}

control_t test_initializeAfterLogLargeAndCommit(const size_t call_count)
{
    int32_t rc;

    printf("test_initializeAfterLogLargeAndCommit: entered with call_count %u\n", call_count);

    if (call_count == 1) {
        rc = FlashJournal_initialize(&journal, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, callbackHandler);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            printf("test_initializeAfterLogLargeAndCommit: asynchronous_ops for init\n");
            return CaseTimeout(200) + CaseRepeatAll;
        }
        TEST_ASSERT_EQUAL(1, rc); /* synchronous completion of initialize() is expected to return 1 */
    }

    FlashJournal_Info_t info;
    rc = FlashJournal_getInfo(&journal, &info);
    TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
    TEST_ASSERT_EQUAL(SIZEOF_LARGE_WRITE, info.sizeofJournaledBlob);

    return CaseNext;
}

template<uint8_t PATTERN>
control_t test_logLargeAndReadSmallChunks(const size_t call_count)
{
    int32_t rc;

    printf("test_logLargeAndReadSmallChunks: entered with call_count %u\n", call_count);

    static const size_t SMALL_CHUNK_COUNT = 4;

    switch (call_count) {
        case 1:
            memset(buffer, PATTERN, SIZEOF_LARGE_WRITE);
            rc = FlashJournal_log(&journal, buffer, SIZEOF_LARGE_WRITE);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }
            /* intentional fall-through */

        case 2:
            rc = FlashJournal_commit(&journal);
            TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }
            /* intentional fall-through */

        case 3:
            {
                FlashJournal_Info_t info;
                rc = FlashJournal_getInfo(&journal, &info);
                TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
                TEST_ASSERT_EQUAL(SIZEOF_LARGE_WRITE, info.sizeofJournaledBlob);
            }
            /* intentional fall-through */

        default:
            break;
    }

    if (call_count > 3) {
        if (drv->GetCapabilities().asynchronous_ops) {
            if (callbackStatus == 0) {
                return CaseNext; /* termination condition */
            }
            TEST_ASSERT_EQUAL(SIZEOF_LARGE_WRITE / SMALL_CHUNK_COUNT, callbackStatus);
        }

        for (unsigned i = 0; i < SIZEOF_LARGE_WRITE / SMALL_CHUNK_COUNT; i++) {
            // printf("index %u value %x\n", i, buffer[i]);
            TEST_ASSERT_EQUAL(PATTERN, buffer[i]);
        }
    }

    while ((rc = FlashJournal_read(&journal, buffer, SIZEOF_LARGE_WRITE / SMALL_CHUNK_COUNT)) != JOURNAL_STATUS_EMPTY) {
        // printf("read returned %ld\n", rc);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }

        TEST_ASSERT_EQUAL(SIZEOF_LARGE_WRITE / SMALL_CHUNK_COUNT, rc);
        for (unsigned i = 0; i < SIZEOF_LARGE_WRITE / SMALL_CHUNK_COUNT; i++) {
            // printf("index %u value %x\n", i, buffer[i]);
            TEST_ASSERT_EQUAL(PATTERN, buffer[i]);
        }
    };

    return CaseNext;
}

template<uint8_t PATTERN, size_t SIZEOF_READS>
control_t test_readLargeInSmallOddChunks(const size_t call_count)
{
    int32_t rc;

    printf("test_readLargeInSmallOddChunks<0x%02x, %u>: entered with call_count %u\n", PATTERN, SIZEOF_READS, call_count);

    if (call_count == 1) {
        FlashJournal_Info_t info;
        rc = FlashJournal_getInfo(&journal, &info);
        TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
        TEST_ASSERT_EQUAL(SIZEOF_LARGE_WRITE, info.sizeofJournaledBlob);
        TEST_ASSERT(SIZEOF_READS < info.sizeofJournaledBlob);
    } else {
        if (drv->GetCapabilities().asynchronous_ops) {
            if (callbackStatus == 0) {
                return CaseNext; /* termination condition */
            }
            TEST_ASSERT_EQUAL(SIZEOF_READS, callbackStatus);
        }

        for (unsigned i = 0; i < SIZEOF_READS; i++) {
            // printf("index %u value %x\n", i, buffer[i]);
            TEST_ASSERT_EQUAL(PATTERN, buffer[i]);
        }
    }

    while ((rc = FlashJournal_read(&journal, buffer, SIZEOF_READS)) != JOURNAL_STATUS_EMPTY) {
        // printf("read returned %ld\n", rc);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }

        TEST_ASSERT(rc <= (int32_t)SIZEOF_READS);
        for (unsigned i = 0; i < (unsigned)rc; i++) {
            // printf("index %u value %x\n", i, buffer[i]);
            TEST_ASSERT_EQUAL(PATTERN, buffer[i]);
        }
    };

    return CaseNext;
}

template<size_t SIZEOF_ODD_CHUNK, size_t N_WRITES>
control_t test_logSeveralOddSizedChunks(size_t call_count)
{
    TEST_ASSERT(N_WRITES >= 1);

    int32_t rc;

    static const uint8_t PATTERN = 0xAA;
    static size_t totalDataLogged = 0;

    printf("test_logSeveralOddSizedChunks<%u, %u>: entered with call_count %u\n", SIZEOF_ODD_CHUNK, N_WRITES, call_count);
    TEST_ASSERT(SIZEOF_ODD_CHUNK <= BUFFER_SIZE);

    /* check the status of the previous asynchronous operation */
    if ((call_count > 1) && (call_count <= (N_WRITES + 1))) {
        TEST_ASSERT((callbackStatus >= JOURNAL_STATUS_OK) || (callbackStatus == JOURNAL_STATUS_SMALL_LOG_REQUEST));
        if (callbackStatus == JOURNAL_STATUS_SMALL_LOG_REQUEST) {
            FlashJournal_Info_t info;
            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT(SIZEOF_ODD_CHUNK < info.program_unit);
            printf("test_logSeveralOddSizedChunks: RETURNING CaseNext\n");
            return CaseNext;
        }

        size_t sizeofLoggedData = callbackStatus;
        TEST_ASSERT((size_t)sizeofLoggedData <= SIZEOF_ODD_CHUNK);
        if (sizeofLoggedData < SIZEOF_ODD_CHUNK) {
            FlashJournal_Info_t info;
            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT((sizeofLoggedData % info.program_unit) == 0);
        }
        totalDataLogged += sizeofLoggedData;
    }

    while (call_count <= N_WRITES) {
        printf("test_logSeveralOddSizedChunks: iteration with call_count %u\n", call_count);
        memset(buffer, PATTERN, SIZEOF_ODD_CHUNK);
        rc = FlashJournal_log(&journal, buffer, SIZEOF_ODD_CHUNK);
        // printf("test_logSeveralOddSizedChunks: called FlashJournal_log(): rc = %" PRId32 "\n", rc);
        TEST_ASSERT((rc >= JOURNAL_STATUS_OK) || (rc == JOURNAL_STATUS_SMALL_LOG_REQUEST));
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }

        if (rc == JOURNAL_STATUS_SMALL_LOG_REQUEST) {
            FlashJournal_Info_t info;
            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT(SIZEOF_ODD_CHUNK < info.program_unit);
            return CaseNext;
        }

        size_t sizeofLoggedData = rc;
        TEST_ASSERT(sizeofLoggedData <= SIZEOF_ODD_CHUNK); /* the amount actually written is expected to be less than the original */
        if (sizeofLoggedData < SIZEOF_ODD_CHUNK) {
            FlashJournal_Info_t info;
            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT((sizeofLoggedData % info.program_unit) == 0);
        }

        totalDataLogged += sizeofLoggedData;
        ++call_count; /* simulate CaseRepeatAll for the synchronous case */
    }

    if (call_count == (N_WRITES + 1)) {
        rc = FlashJournal_commit(&journal);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }

        callbackStatus = rc;
    }

    TEST_ASSERT_EQUAL(1, callbackStatus);
    {
        FlashJournal_Info_t info;
        rc = FlashJournal_getInfo(&journal, &info);
        TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
        TEST_ASSERT_EQUAL(totalDataLogged, info.sizeofJournaledBlob);
    }

    return CaseNext;
}

control_t test_multipleWritesFollowedByCommitFollowedByMultipleReads(const size_t call_count)
{
    int32_t rc;

    static const uint8_t PATTERN      = 0xAA;
    static const size_t  N_WRITES     = 4;
    static const size_t  N_READS      = N_WRITES;
    static const size_t  SIZEOF_WRITE = BUFFER_SIZE / N_WRITES;
    static const size_t  SIZEOF_READ  = BUFFER_SIZE / N_READS;

    printf("test_multipleWritesFollowedByCommitFollowedByMultipleReads: entered with call_count %u\n", call_count);

    if (call_count <= N_WRITES) {
        printf("writing pattern %02x\n", PATTERN ^ call_count);
        memset(buffer, (PATTERN ^ call_count), SIZEOF_WRITE);
        rc = FlashJournal_log(&journal, buffer, SIZEOF_WRITE);
        // printf("test_multipleWritesFollowedByCommitFollowedByMultipleReads: log returned %" PRId32 "\n", rc);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }
        TEST_ASSERT_EQUAL(SIZEOF_WRITE, rc);
        return CaseRepeatAll;
    } else if (call_count == (N_WRITES + 1)) {
        rc = FlashJournal_commit(&journal);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }
        // printf("test_multipleWritesFollowedByCommitFollowedByMultipleReads: commit returned %" PRId32 "\n", rc);
        callbackStatus = rc; /* pass forward the return value so that the next iteration can check callbackStatus */
        return CaseRepeatAll;
    } else if (call_count < (N_WRITES + 1 + N_READS + 1)) {
        unsigned readIteration = call_count - (N_WRITES + 1);
        printf("test_multipleWritesFollowedByCommitFollowedByMultipleReads: read iteration %u\n", readIteration);
        if (call_count == (N_WRITES + 1 /* commit */ + 1 /* first iteration after commit */)) {
            TEST_ASSERT_EQUAL(1, callbackStatus);

            FlashJournal_Info_t info;
            rc = FlashJournal_getInfo(&journal, &info);
            TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
            TEST_ASSERT_EQUAL(BUFFER_SIZE, info.sizeofJournaledBlob);
        } else {
            TEST_ASSERT_EQUAL(SIZEOF_READ, callbackStatus);
            for (unsigned i = 0; i < SIZEOF_READ; i++) {
                // printf("test_multipleWritesFollowedByCommitFollowedByMultipleReads: index %u value %x\n", i, buffer[i]);
                TEST_ASSERT_EQUAL(PATTERN ^ (readIteration - 1), buffer[i]);
            }
        }

        while ((rc = FlashJournal_read(&journal, buffer, SIZEOF_READ)) != JOURNAL_STATUS_EMPTY) {
            // printf("test_multipleWritesFollowedByCommitFollowedByMultipleReads: read returned %ld\n", rc);
            TEST_ASSERT((rc == JOURNAL_STATUS_OK) || (rc == SIZEOF_READ));
            if (rc == JOURNAL_STATUS_OK) {
                TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
                return CaseTimeout(500) + CaseRepeatAll;
            }

            TEST_ASSERT_EQUAL(SIZEOF_READ, rc);
            printf("test_multipleWritesFollowedByCommitFollowedByMultipleReads: checking for pattern %02x\n", PATTERN ^ readIteration);
            for (unsigned i = 0; i < SIZEOF_READ; i++) {
                // printf("index %u value %x\n", i, buffer[i]);
                TEST_ASSERT_EQUAL(PATTERN ^ readIteration, buffer[i]);
            }
            ++readIteration;
        };
        TEST_ASSERT_EQUAL(N_READS + 1, readIteration);
    }

    return CaseNext;
}

control_t test_failedSmallWriteFollowedByPaddedWrite(const size_t call_count)
{
    int32_t rc;

    static const uint8_t PATTERN = 0xAA;

    printf("test_failedSmallWriteFollowedByPaddedWrite: entered with call_count %u\n", call_count);

    FlashJournal_Info_t info;
    rc = FlashJournal_getInfo(&journal, &info);
    TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
    TEST_ASSERT(info.program_unit >= 1);
    if (info.program_unit == 1) {
        return CaseNext;
    }

    static const size_t SMALL_CONSTANT = 8 * info.program_unit;
    static const size_t SIZEOF_WRITE   = (info.program_unit - 1) + SMALL_CONSTANT;
    TEST_ASSERT(SIZEOF_WRITE <= BUFFER_SIZE);

    memset(buffer, PATTERN, SIZEOF_WRITE);

    if (call_count == 1) {
        rc = FlashJournal_log(&journal, buffer, SIZEOF_WRITE);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }
        TEST_ASSERT_EQUAL(SMALL_CONSTANT, rc);
        callbackStatus = rc;
        return CaseRepeatAll;
    } else if (call_count == 2) {
        TEST_ASSERT_EQUAL(SMALL_CONSTANT, callbackStatus);
        rc = FlashJournal_log(&journal, buffer, SIZEOF_WRITE - callbackStatus);
        TEST_ASSERT_EQUAL(JOURNAL_STATUS_SMALL_LOG_REQUEST, rc);

        rc = FlashJournal_getInfo(&journal, &info);
        TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
        TEST_ASSERT(info.program_unit >= 1);
        TEST_ASSERT(info.program_unit <= BUFFER_SIZE);

        rc = FlashJournal_log(&journal, buffer, info.program_unit);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }
        TEST_ASSERT_EQUAL(info.program_unit, rc);
        callbackStatus = rc;
        return CaseRepeatAll;
    } else if (call_count == 3) {
        rc = FlashJournal_commit(&journal);
        TEST_ASSERT(rc >= JOURNAL_STATUS_OK);
        if (rc == JOURNAL_STATUS_OK) {
            TEST_ASSERT_EQUAL(1, drv->GetCapabilities().asynchronous_ops);
            return CaseTimeout(500) + CaseRepeatAll;
        }
        TEST_ASSERT_EQUAL(1, rc);
        callbackStatus = rc;
        return CaseRepeatAll;
    } else {
        TEST_ASSERT_EQUAL(1, callbackStatus);

        rc = FlashJournal_getInfo(&journal, &info);
        TEST_ASSERT_EQUAL(JOURNAL_STATUS_OK, rc);
        TEST_ASSERT_EQUAL((SIZEOF_WRITE + 1), info.sizeofJournaledBlob);
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
    Case("initialize",                                  test_initialize),
    Case("reset and initialize1",                       test_resetAndInitialize),

    Case("log small item without commit",               test_logSmallWithoutCommit),
    Case("reset and initialize2",                       test_resetAndInitialize),

    Case("commit without logs",                         test_commitWithoutLogs),
    Case("initialize",                                  test_initialize),

    /* log small item, and reinitialize */
    Case("log small item and commit1",                  test_logSmallAndCommit<0xAA>),
    Case("initialize after small log and commit1",      test_initializeAfterLogSmallAndCommit),
    Case("log small item and commit2",                  test_logSmallAndCommit<0x11>),
    Case("initialize after small log and commit2",      test_initializeAfterLogSmallAndCommit),
    Case("log small item and commit3",                  test_logSmallAndCommit<0x22>),
    Case("initialize after small log and commit3",      test_initializeAfterLogSmallAndCommit),
    Case("log small item and commit4",                  test_logSmallAndCommit<0x55>),
    Case("initialize after small log and commit4",      test_initializeAfterLogSmallAndCommit),
    Case("log small item and commit5",                  test_logSmallAndCommit<0xAB>),
    Case("initialize after small log and commit5",      test_initializeAfterLogSmallAndCommit),
    Case("reset and initialize3",                       test_resetAndInitialize),

    Case("log large item without commit",               test_logLargeWithoutCommit),

    /* initialize, log large item, and reinitialize */
    Case("initialize2",                                 test_initialize),
    Case("reset and initialize4",                       test_resetAndInitialize),
    Case("log large item and commit1",                  test_logLargeAndCommit<0xAA>),
    Case("initialize after large log and commit1",      test_initializeAfterLogLargeAndCommit),
    Case("log large item and commit2",                  test_logLargeAndCommit<0x55>),
    Case("initialize after large log and commit2",      test_initializeAfterLogLargeAndCommit),
    Case("log large item and commit3",                  test_logLargeAndCommit<0x11>),
    Case("initialize after large log and commit3",      test_initializeAfterLogLargeAndCommit),
    Case("log large item and commit4",                  test_logLargeAndCommit<0xAB>),
    Case("initialize after large log and commit4",      test_initializeAfterLogLargeAndCommit),
    Case("log large item and commit5",                  test_logLargeAndCommit<0x22>),
    Case("initialize after large log and commit5",      test_initializeAfterLogLargeAndCommit),
    Case("reset and initialize5",                       test_resetAndInitialize),

    Case("log large item and read smaller chunks",      test_logLargeAndReadSmallChunks<0xAA>),
    Case("read large item in small, odd-sized chunks1", test_readLargeInSmallOddChunks<0xAA, ((BUFFER_SIZE / 2) - 1)>),
    Case("read large item in small, odd-sized chunks2", test_readLargeInSmallOddChunks<0xAA, 255>),
    Case("read large item in small, odd-sized chunks3", test_readLargeInSmallOddChunks<0xAA, 1021>),
    Case("read large item in small, odd-sized chunks4", test_readLargeInSmallOddChunks<0xAA, 2401>),

    /* log odd-sized blocks which wouldn't align with program_unit at the tail */
    Case("initialize3",                                 test_initialize),
    Case("log odd-sized chunk",                         test_logSeveralOddSizedChunks<1, 1>),
    Case("log odd-sized chunk",                         test_logSeveralOddSizedChunks<101, 11>),
    Case("log odd-sized chunk",                         test_logSeveralOddSizedChunks<1217, 4>),
    Case("log odd-sized chunk",                         test_logSeveralOddSizedChunks<2402, 5>),
    Case("log odd-sized chunk",                         test_logSeveralOddSizedChunks<4803, 3>),
    Case("log odd-sized chunk",                         test_logSeveralOddSizedChunks<(BUFFER_SIZE-1), 7>),

    Case("initialize4",                                 test_initialize),
    Case("multiple writes, commit, multiple reads",     test_multipleWritesFollowedByCommitFollowedByMultipleReads),

    Case("failed small write followed by padded write", test_failedSmallWriteFollowedByPaddedWrite),

    Case("reset and initialize6",                       test_resetAndInitialize),
    // Case("uninitialize", test_uninitialize),
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
