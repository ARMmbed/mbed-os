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

#ifndef __FLASH_JOURNAL_STRATEGY_SEQUENTIAL_H__
#define __FLASH_JOURNAL_STRATEGY_SEQUENTIAL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "flash-journal/flash_journal.h"

/**
 * Create/format a sequential flash journal at a given offset within a storage
 * device and with a given slot-cardinality.
 *
 * This function must be called *once* for each incarnation of a sequential
 * journal.
 *
 * @param[in] mtd
 *              The underlying Storage driver.
 *
 * @param[in] numSlots
 *              Number of slots in the sequential journal. Each slot holds a header, blob-payload, and a tail.
 *
 * @param[in] callback
 *                Caller-defined callback to be invoked upon command completion
 *                in case the storage device executes operations asynchronously.
 *                Use a NULL pointer when no callback signals are required.
 *
 * @note: this is an asynchronous operation, but it can finish
 * synchronously if the underlying MTD supports that.
 *
 * @return
 *   The function executes in the following ways:
 *   - When the operation is asynchronous, the function only starts the
 *     initialization and control returns to the caller with an
 *     JOURNAL_STATUS_OK before the actual completion of the operation (or with
 *     an appropriate error code in case of failure). When the operation is
 *     completed the command callback is invoked with 1 passed in as the
 *     'status' parameter of the callback. In case of errors, the completion
 *     callback is invoked with an error status.
 *   - When the operation is executed by the journal in a blocking (i.e.
 *     synchronous) manner, control returns to the caller only upon the actual
 *     completion of the operation or the discovery of a failure condition. In
 *     this case, the function returns 1 to signal successful synchronous
 *     completion or an appropriate error code, and no further
 *     invocation of the completion callback should be expected at a later time.
 *
 * Here's a code snippet to suggest how this API might be used by callers:
 * \code
 *     ASSERT(JOURNAL_STATUS_OK == 0); // this is a precondition; it doesn't need to be put in code
 *     int32_t returnValue = flashJournalStrategySequential_format(MTD, numSlots, callbackHandler);
 *     if (returnValue < JOURNAL_STATUS_OK) {
 *         // handle error
 *     } else if (returnValue == JOURNAL_STATUS_OK) {
 *         ASSERT(MTD->GetCapabilities().asynchronous_ops == 1);
 *         // handle early return from asynchronous execution
 *     } else {
 *         ASSERT(returnValue == 1);
 *         // handle synchronous completion
 *     }
 * \endcode
 *
 *     +-------------------------------+    ^
 *     |                               |    |
 *     |  Journal Header               |    |
 *     |  starts with generic header   |    |
 *     |  followed by specific header  |    |
 *     |                               |    |   multiple of program_unit
 *     +-------------------------------+    |   and erase-boundary
 *     +-------------------------------+    |
 *     |                               |    |
 *     |   padding to allow alignment  |    |
 *     |                               |    |
 *     +-------------------------------+    v
 *     +-------------------------------+
 *     | +---------------------------+ |    ^
 *     | |  slot header              | |    |
 *     | |  aligned with program_unit| |    |
 *     | +---------------------------+ |    |     slot 0
 *     |                               |    |     aligned with LCM of all erase boundaries
 *     |                               |    |
 *     |                               |    |
 *     |                               |    |
 *     |          BLOB0                |    |
 *     |                               |    |
 *     |                               |    |
 *     | +---------------------------+ |    |
 *     | |  slot tail                | |    |
 *     | |  aligned with program_unit| |    |
 *     | +---------------------------+ |    |
 *     +-------------------------------+    v
 *     +-------------------------------+
 *     | +---------------------------+ |    ^
 *     | |  slot header              | |    |
 *     | |  aligned with program_unit| |    |
 *     | +---------------------------+ |    |     slot 1
 *     |                               |    |     aligned with LCM of all erase boundaries
 *     |          BLOB1                |    |
 *     |                               |    |
 *     .                               .    .
 *     .                               .    .
 *
 *     .                               .    .
 *     .          BLOB(N-1)            .    .
 *     |                               |    |
 *     | +---------------------------+ |    |     slot 'N - 1'
 *     | |  slot tail                | |    |     aligned with LCM of all erase boundaries
 *     | |  aligned with program_unit| |    |
 *     | +---------------------------+ |    |
 *     +-------------------------------+    v
 */
int32_t               flashJournalStrategySequential_format(ARM_DRIVER_STORAGE      *mtd,
                                                            uint32_t                 numSlots,
                                                            FlashJournal_Callback_t  callback);

int32_t               flashJournalStrategySequential_initialize(FlashJournal_t           *journal,
                                                                ARM_DRIVER_STORAGE       *mtd,
                                                                const FlashJournal_Ops_t *ops,
                                                                FlashJournal_Callback_t   callback);
FlashJournal_Status_t flashJournalStrategySequential_getInfo(FlashJournal_t *journal, FlashJournal_Info_t *info);
int32_t               flashJournalStrategySequential_read(FlashJournal_t *journal, void *blob, size_t n);
int32_t               flashJournalStrategySequential_readFrom(FlashJournal_t *journal, size_t offset, void *blob, size_t n);
int32_t               flashJournalStrategySequential_log(FlashJournal_t *journal, const void *blob, size_t n);
int32_t               flashJournalStrategySequential_commit(FlashJournal_t *journal);
int32_t               flashJournalStrategySequential_reset(FlashJournal_t *journal);

static const FlashJournal_Ops_t FLASH_JOURNAL_STRATEGY_SEQUENTIAL = {
    flashJournalStrategySequential_initialize,
    flashJournalStrategySequential_getInfo,
    flashJournalStrategySequential_read,
    flashJournalStrategySequential_readFrom,
    flashJournalStrategySequential_log,
    flashJournalStrategySequential_commit,
    flashJournalStrategySequential_reset
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __FLASH_JOURNAL_STRATEGY_SEQUENTIAL_H__ */
