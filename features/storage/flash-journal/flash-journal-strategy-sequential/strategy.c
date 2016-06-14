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

#include "flash-journal-strategy-sequential/flash_journal_private.h"
#include "flash-journal-strategy-sequential/flash_journal_strategy_sequential.h"
#include "support_funcs.h"
#include <string.h>
#include <stdio.h>

SequentialFlashJournal_t *activeJournal;

/* forward declarations */
void mtdHandler(int32_t status, ARM_STORAGE_OPERATION operation);

static inline int32_t mtdGetTotalCapacity(ARM_DRIVER_STORAGE *mtd, uint64_t *capacityP)
{
    /* fetch MTD's INFO */
    ARM_STORAGE_INFO mtdInfo;
    int32_t rc = mtd->GetInfo(&mtdInfo);
    if (rc != ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    *capacityP = mtdInfo.total_storage;

    return JOURNAL_STATUS_OK;
}

static inline int32_t flashJournalStrategySequential_read_sanityChecks(SequentialFlashJournal_t *journal, const void *blob, size_t sizeofBlob)
{
    if ((journal == NULL) || (blob == NULL) || (sizeofBlob == 0)) {
        return JOURNAL_STATUS_PARAMETER;
    }
    if ((journal->state == SEQUENTIAL_JOURNAL_STATE_NOT_INITIALIZED) || (journal->state == SEQUENTIAL_JOURNAL_STATE_INIT_SCANNING_LOG_HEADERS)) {
        return JOURNAL_STATUS_NOT_INITIALIZED;
    }
    if (journal->state != SEQUENTIAL_JOURNAL_STATE_INITIALIZED) {
        return JOURNAL_STATUS_ERROR; /* journal is in an un-expected state. */
    }
    // printf("read sanity checks: totalDataRead = %lu, sizeofJournaledBlob = %lu\n", (uint32_t)journal->read.totalDataRead, (uint32_t)journal->info.sizeofJournaledBlob);
    if ((journal->info.sizeofJournaledBlob == 0) || (journal->read.totalDataRead == journal->info.sizeofJournaledBlob)) {
        journal->read.totalDataRead = 0;
        return JOURNAL_STATUS_EMPTY;
    }

    return JOURNAL_STATUS_OK;
}

static inline int32_t flashJournalStrategySequential_log_sanityChecks(SequentialFlashJournal_t *journal, const void *blob, size_t sizeofBlob)
{
    if ((journal == NULL) || (blob == NULL) || (sizeofBlob == 0)) {
        return JOURNAL_STATUS_PARAMETER;
    }
    if ((journal->state == SEQUENTIAL_JOURNAL_STATE_NOT_INITIALIZED) || (journal->state == SEQUENTIAL_JOURNAL_STATE_INIT_SCANNING_LOG_HEADERS)) {
        return JOURNAL_STATUS_NOT_INITIALIZED;
    }
    if ((journal->state != SEQUENTIAL_JOURNAL_STATE_INITIALIZED) && (journal->state != SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY)) {
        return JOURNAL_STATUS_ERROR; /* journal is in an un-expected state. */
    }
    if (journal->state == SEQUENTIAL_JOURNAL_STATE_INITIALIZED) {
        if (sizeofBlob > journal->info.capacity) {
            return JOURNAL_STATUS_BOUNDED_CAPACITY; /* adding this log chunk would cause us to exceed capacity (write past the tail). */
        }
    } else if (journal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY) {
        if (journal->log.offset + sizeofBlob > journal->log.tailOffset) {
            return JOURNAL_STATUS_BOUNDED_CAPACITY; /* adding this log chunk would cause us to exceed capacity (write past the tail). */
        }
    }

    return JOURNAL_STATUS_OK;
}

static inline int32_t flashJournalStrategySequential_commit_sanityChecks(SequentialFlashJournal_t *journal)
{
    if (journal == NULL) {
        return JOURNAL_STATUS_PARAMETER;
    }
    if (journal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY) {
        if (journal->prevCommand != FLASH_JOURNAL_OPCODE_LOG_BLOB) {
            return JOURNAL_STATUS_ERROR;
        }
        if ((journal->log.offset          == ARM_STORAGE_INVALID_OFFSET) ||
            (journal->log.tailOffset      == ARM_STORAGE_INVALID_OFFSET) ||
            (journal->log.tailOffset       < journal->log.offset)        ||
            (journal->log.tail.sizeofBlob == 0)                          ||
            (journal->log.tail.sizeofBlob  > journal->info.capacity)) {
            return JOURNAL_STATUS_ERROR; /* journal is in an un-expected state. */
        }
    }

    return JOURNAL_STATUS_OK;
}

int32_t flashJournalStrategySequential_initialize(FlashJournal_t           *_journal,
                                                  ARM_DRIVER_STORAGE       *mtd,
                                                  const FlashJournal_Ops_t *ops,
                                                  FlashJournal_Callback_t   callback)
{
    int32_t rc;

    SequentialFlashJournal_t *journal;
    activeJournal = journal = (SequentialFlashJournal_t *)_journal;
    journal->state          = SEQUENTIAL_JOURNAL_STATE_NOT_INITIALIZED;

    /* fetch MTD's total capacity */
    uint64_t mtdCapacity;
    if ((rc = mtdGetTotalCapacity(mtd, &mtdCapacity)) != JOURNAL_STATUS_OK) {
        return rc;
    }
    ARM_STORAGE_INFO mtdInfo;
    if ((rc = mtd->GetInfo(&mtdInfo)) != ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }

    /* initialize the journal structure */
    memcpy(&journal->ops, ops, sizeof(FlashJournal_Ops_t));
    journal->mtd               = mtd;
    journal->mtdCapabilities   = mtd->GetCapabilities(); /* fetch MTD's capabilities */
    journal->sequentialSkip    = mtdCapacity / SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS;
    journal->info.capacity     = journal->sequentialSkip - (sizeof(SequentialFlashJournalLogHead_t) + sizeof(SequentialFlashJournalLogTail_t)); /* effective capacity */
    journal->info.program_unit = mtdInfo.program_unit;
    journal->callback          = callback;
    journal->prevCommand       = FLASH_JOURNAL_OPCODE_INITIALIZE;

    /* initialize MTD */
    ARM_STORAGE_CAPABILITIES mtdCaps = mtd->GetCapabilities();
    rc = mtd->Initialize(mtdHandler);
    if (rc < ARM_DRIVER_OK) {
        memset(journal, 0, sizeof(FlashJournal_t));
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    if ((mtdCaps.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
        return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
    }

    if ((rc = discoverLatestLoggedBlob(journal)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    return 1; /* synchronous completion */
}

FlashJournal_Status_t flashJournalStrategySequential_getInfo(FlashJournal_t *_journal, FlashJournal_Info_t *infoP)
{
    SequentialFlashJournal_t *journal;
    activeJournal = journal = (SequentialFlashJournal_t *)_journal;

    memcpy(infoP, &journal->info, sizeof(FlashJournal_Info_t));
    return JOURNAL_STATUS_OK;
}

int32_t flashJournalStrategySequential_read(FlashJournal_t *_journal, void *blob, size_t sizeofBlob)
{
    SequentialFlashJournal_t *journal;
    activeJournal = journal = (SequentialFlashJournal_t *)_journal;

    int32_t rc;
    if ((rc = flashJournalStrategySequential_read_sanityChecks(journal, blob, sizeofBlob)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    journal->read.blob       = blob;
    journal->read.sizeofBlob = sizeofBlob;

    if ((journal->prevCommand != FLASH_JOURNAL_OPCODE_READ_BLOB) || (journal->read.totalDataRead == 0)) {
        journal->read.offset        = journal->mtdStartOffset
                                      + (journal->currentBlobIndex * journal->sequentialSkip)
                                      + sizeof(SequentialFlashJournalLogHead_t);
        journal->read.totalDataRead = 0;
    } else {
        /* journal->read.offset is already set from the previous read execution */
        // printf("flashJournalStrategySequential_read: continuing read of %lu from offset %lu\n", sizeofBlob, (uint32_t)journal->read.offset);
    }
    journal->read.dataBeingRead    = blob;
    journal->read.amountLeftToRead = ((journal->info.sizeofJournaledBlob - journal->read.totalDataRead) < sizeofBlob) ?
                                        (journal->info.sizeofJournaledBlob - journal->read.totalDataRead) : sizeofBlob;
    // printf("amount left to read %u\n", journal->read.amountLeftToRead);

    journal->state       = SEQUENTIAL_JOURNAL_STATE_READING;
    journal->prevCommand = FLASH_JOURNAL_OPCODE_READ_BLOB;
    return flashJournalStrategySequential_read_progress();
}

int32_t flashJournalStrategySequential_log(FlashJournal_t *_journal, const void *blob, size_t size)
{
    SequentialFlashJournal_t *journal;
    activeJournal = journal = (SequentialFlashJournal_t *)_journal;

    int32_t rc;
    if ((rc = flashJournalStrategySequential_log_sanityChecks(journal, blob, size)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    journal->log.blob       = blob;
    journal->log.sizeofBlob = size;

    if (journal->prevCommand != FLASH_JOURNAL_OPCODE_LOG_BLOB) {
        uint32_t logBlobIndex = journal->currentBlobIndex + 1;
        if (logBlobIndex == SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS) {
            logBlobIndex = 0;
        }
        journal->log.eraseOffset = journal->mtdStartOffset + (logBlobIndex * journal->sequentialSkip);

        /* ensure that the request is at least as large as the minimum program unit */
        if (size < journal->info.program_unit) {
            return JOURNAL_STATUS_SMALL_LOG_REQUEST;
        }

        journal->state       = SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE; /* start with erasing the log region */
        journal->prevCommand = FLASH_JOURNAL_OPCODE_LOG_BLOB;
    } else {
        journal->log.dataBeingLogged = blob;
        journal->log.amountLeftToLog = size;
    }

    return flashJournalStrategySequential_log_progress();
}

int32_t flashJournalStrategySequential_commit(FlashJournal_t *_journal)
{
    SequentialFlashJournal_t *journal;
    activeJournal = journal = (SequentialFlashJournal_t *)_journal;

    int32_t rc;
    if ((rc = flashJournalStrategySequential_commit_sanityChecks(journal)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    if (journal->prevCommand == FLASH_JOURNAL_OPCODE_LOG_BLOB) {
        journal->log.offset          = journal->log.tailOffset;
        journal->log.dataBeingLogged = (const uint8_t *)&journal->log.tail;
        journal->log.amountLeftToLog = sizeof(SequentialFlashJournalLogTail_t);
        journal->state               = SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL;
    } else {
        uint32_t logBlobIndex = journal->currentBlobIndex + 1;
        if (logBlobIndex == SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS) {
            logBlobIndex = 0;
        }
        journal->log.eraseOffset = journal->mtdStartOffset + (logBlobIndex * journal->sequentialSkip);
        journal->state           = SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE;
    }

    journal->prevCommand = FLASH_JOURNAL_OPCODE_COMMIT;
    return flashJournalStrategySequential_log_progress();
}

int32_t flashJournalStrategySequential_reset(FlashJournal_t *_journal)
{
    SequentialFlashJournal_t *journal;
    activeJournal = journal = (SequentialFlashJournal_t *)_journal;

    journal->state = SEQUENTIAL_JOURNAL_STATE_RESETING;

    journal->prevCommand = FLASH_JOURNAL_OPCODE_RESET;
    return flashJournalStrategySequential_reset_progress();
}
