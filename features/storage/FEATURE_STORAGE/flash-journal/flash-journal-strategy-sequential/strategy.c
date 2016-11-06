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

#include "flash-journal-strategy-sequential/flash_journal_crc.h"
#include "flash-journal-strategy-sequential/flash_journal_private.h"
#include "flash-journal-strategy-sequential/flash_journal_strategy_sequential.h"
#include "support_funcs.h"
#include <string.h>
#include <stdio.h>

SequentialFlashJournal_t *activeJournal;

/*
 * forward declarations of static-inline helper functions.
 */
static inline int32_t mtdGetTotalCapacity(ARM_DRIVER_STORAGE *mtd, uint64_t *capacityP);
static inline int32_t flashJournalStrategySequential_format_sanityChecks(ARM_DRIVER_STORAGE *mtd, uint32_t numSlots);
static inline int32_t flashJournalStrategySequential_read_sanityChecks(SequentialFlashJournal_t *journal, const void *blob, size_t sizeofBlob);
static inline int32_t flashJournalStrategySequential_log_sanityChecks(SequentialFlashJournal_t *journal, const void *blob, size_t sizeofBlob);
static inline int32_t flashJournalStrategySequential_commit_sanityChecks(SequentialFlashJournal_t *journal);


int32_t flashJournalStrategySequential_format(ARM_DRIVER_STORAGE      *mtd,
                                              uint32_t                 numSlots,
                                              FlashJournal_Callback_t  callback)
{
    int32_t rc;
    if ((rc = flashJournalStrategySequential_format_sanityChecks(mtd, numSlots)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    ARM_STORAGE_INFO mtdInfo;
    if (mtd->GetInfo(&mtdInfo) < ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    uint64_t mtdAddr;
    if (mtdGetStartAddr(mtd, &mtdAddr) < JOURNAL_STATUS_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }

    formatInfoSingleton.mtd            = mtd;
    formatInfoSingleton.mtdAddr        = mtdAddr;
    formatInfoSingleton.callback       = callback;
    formatInfoSingleton.mtdProgramUnit = mtdInfo.program_unit;

    if ((rc = setupSequentialJournalHeader(&formatInfoSingleton.header, mtd, mtdInfo.total_storage, numSlots)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    /* initialize MTD */
    rc = mtd->Initialize(formatHandler);
    if (rc < ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    } else if (rc == ARM_DRIVER_OK) {
        return JOURNAL_STATUS_OK; /* An asynchronous operation is pending; it will result in a completion callback
                                   * where the rest of processing will take place. */
    }
    if (rc != 1) {
        return JOURNAL_STATUS_STORAGE_API_ERROR; /* synchronous completion is expected to return 1 */
    }

    /* progress the rest of the create state-machine */
    return flashJournalStrategySequential_format_progress(ARM_DRIVER_OK, ARM_STORAGE_OPERATION_INITIALIZE);
}

/**
 * Validate a header at the start of the MTD.
 *
 * @param [in/out] headerP
 *                     Caller-allocated header which gets filled in during validation.

 * @return JOURNAL_STATUS_OK if the header is sane. As a side-effect, the memory
 *         pointed to by 'headerP' is initialized with the header.
 */
int32_t readAndVerifyJournalHeader(SequentialFlashJournal_t *journal, SequentialFlashJournalHeader_t *headerP)
{
    if (headerP == NULL) {
        return JOURNAL_STATUS_PARAMETER;
    }

    int32_t rc = journal->mtd->ReadData(journal->mtdStartOffset, headerP, sizeof(SequentialFlashJournalHeader_t));
    if (rc < ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_IO_ERROR;
    } else if (rc == ARM_DRIVER_OK) {
        ARM_STORAGE_CAPABILITIES mtdCaps = journal->mtd->GetCapabilities();
        if (!mtdCaps.asynchronous_ops) {
            return JOURNAL_STATUS_ERROR; /* asynchronous_ops must be set if MTD returns ARM_DRIVER_OK. */
        }

        return JOURNAL_STATUS_ERROR; /* TODO: handle init with pending asynchronous activity. */
    }

    if ((headerP->genericHeader.magic        != FLASH_JOURNAL_HEADER_MAGIC)             ||
        (headerP->genericHeader.version      != FLASH_JOURNAL_HEADER_VERSION)           ||
        (headerP->genericHeader.sizeofHeader != sizeof(SequentialFlashJournalHeader_t)) ||
        (headerP->magic                      != SEQUENTIAL_FLASH_JOURNAL_HEADER_MAGIC)  ||
        (headerP->version                    != SEQUENTIAL_FLASH_JOURNAL_HEADER_VERSION)) {
        return JOURNAL_STATUS_NOT_FORMATTED;
    }

    uint32_t expectedCRC = headerP->genericHeader.checksum;
    headerP->genericHeader.checksum = 0;
    flashJournalCrcReset();
    uint32_t computedCRC = flashJournalCrcCummulative((const unsigned char *)&headerP->genericHeader, sizeof(SequentialFlashJournalLogHead_t));
    if (computedCRC != expectedCRC) {
        //printf("readAndVerifyJournalHeader: checksum mismatch during header verification: expected = %u, computed = %u\n", (unsigned int) expectedCRC, (unsigned int) computedCRC);
        return JOURNAL_STATUS_METADATA_ERROR;
    }

    return JOURNAL_STATUS_OK;
}

int32_t flashJournalStrategySequential_initialize(FlashJournal_t           *_journal,
                                                  ARM_DRIVER_STORAGE       *mtd,
                                                  const FlashJournal_Ops_t *ops,
                                                  FlashJournal_Callback_t   callback)
{
    int32_t rc;

    /* initialize MTD */
    rc = mtd->Initialize(mtdHandler);
    if (rc < ARM_DRIVER_OK) {
        memset(_journal, 0, sizeof(FlashJournal_t));
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    if (rc == ARM_DRIVER_OK) {
        ARM_STORAGE_CAPABILITIES mtdCaps = mtd->GetCapabilities();
        if (!mtdCaps.asynchronous_ops) {
            return JOURNAL_STATUS_ERROR; /* asynchronous_ops must be set if MTD returns ARM_DRIVER_OK. */
        }

        return JOURNAL_STATUS_ERROR; /* TODO: handle init with pending asynchronous activity. */
    }

    SequentialFlashJournal_t *journal;
    activeJournal = journal    = (SequentialFlashJournal_t *)_journal;
    journal->state             = SEQUENTIAL_JOURNAL_STATE_NOT_INITIALIZED;
    journal->mtd               = mtd;

    /* Setup start address within MTD. */
    if ((rc = mtdGetStartAddr(journal->mtd, &journal->mtdStartOffset)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    /* fetch MTD's total capacity */
    uint64_t mtdCapacity;
    if ((rc = mtdGetTotalCapacity(mtd, &mtdCapacity)) != JOURNAL_STATUS_OK) {
        return rc;
    }
    ARM_STORAGE_INFO mtdInfo;
    if ((rc = mtd->GetInfo(&mtdInfo)) != ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }

    SequentialFlashJournalHeader_t journalHeader;
    if ((rc = readAndVerifyJournalHeader(journal, &journalHeader)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    /* initialize the journal structure */
    memcpy(&journal->ops, ops, sizeof(FlashJournal_Ops_t));
    journal->mtdCapabilities   = mtd->GetCapabilities(); /* fetch MTD's capabilities */

    journal->firstSlotOffset   = journalHeader.genericHeader.journalOffset;
    journal->numSlots          = journalHeader.numSlots;
    journal->sizeofSlot        = journalHeader.sizeofSlot;

    /* effective capacity */
    journal->info.capacity     = journal->sizeofSlot
                                 - roundUp_uint32(sizeof(SequentialFlashJournalLogHead_t), mtdInfo.program_unit)
                                 - roundUp_uint32(sizeof(SequentialFlashJournalLogTail_t), mtdInfo.program_unit);
    journal->info.program_unit = mtdInfo.program_unit;
    journal->callback          = callback;
    journal->prevCommand       = FLASH_JOURNAL_OPCODE_INITIALIZE;

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

    if (journal->prevCommand != FLASH_JOURNAL_OPCODE_READ_BLOB) {
        journal->read.logicalOffset = 0;
    }

    int32_t rc;
    if ((rc = flashJournalStrategySequential_read_sanityChecks(journal, blob, sizeofBlob)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    journal->read.blob       = blob;
    journal->read.sizeofBlob = sizeofBlob;

    if (journal->read.logicalOffset == 0) {
        { /* Establish the sanity of this slot before proceeding with the read. */
            uint32_t headSequenceNumber;
            SequentialFlashJournalLogTail_t tail;
            if (slotIsSane(journal,
                           SLOT_ADDRESS(journal, journal->currentBlobIndex),
                           &headSequenceNumber,
                           &tail) != 1) {
                /* TODO: rollback to an older slot. */
                return JOURNAL_STATUS_STORAGE_IO_ERROR;
            }
        }

        journal->read.mtdOffset = SLOT_ADDRESS(journal, journal->currentBlobIndex) + sizeof(SequentialFlashJournalLogHead_t);
    } else {
        /* journal->read.offset is already set from the previous read execution */
        // printf("flashJournalStrategySequential_read: continuing read of %lu from offset %lu\n", sizeofBlob, (uint32_t)journal->read.offset);
    }
    journal->read.dataBeingRead    = blob;
    journal->read.amountLeftToRead = ((journal->info.sizeofJournaledBlob - journal->read.logicalOffset) < sizeofBlob) ?
                                        (journal->info.sizeofJournaledBlob - journal->read.logicalOffset) : sizeofBlob;
    // printf("amount left to read %u\n", journal->read.amountLeftToRead);

    journal->state       = SEQUENTIAL_JOURNAL_STATE_READING;
    journal->prevCommand = FLASH_JOURNAL_OPCODE_READ_BLOB;
    return flashJournalStrategySequential_read_progress();
}

int32_t flashJournalStrategySequential_readFrom(FlashJournal_t *_journal, size_t offset, void *blob, size_t sizeofBlob)
{
    SequentialFlashJournal_t *journal;
    activeJournal = journal = (SequentialFlashJournal_t *)_journal;

    journal->read.logicalOffset = offset;
    int32_t rc;
    if ((rc = flashJournalStrategySequential_read_sanityChecks(journal, blob, sizeofBlob)) != JOURNAL_STATUS_OK) {
        return rc;
    }

    journal->read.blob             = blob;
    journal->read.sizeofBlob       = sizeofBlob;

    journal->read.mtdOffset        = SLOT_ADDRESS(journal, journal->currentBlobIndex) + sizeof(SequentialFlashJournalLogHead_t) + offset;

    journal->read.dataBeingRead    = blob;
    journal->read.amountLeftToRead = ((journal->info.sizeofJournaledBlob - journal->read.logicalOffset) < sizeofBlob) ?
                                        (journal->info.sizeofJournaledBlob - journal->read.logicalOffset) : sizeofBlob;
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
        /*
         * This is the first log in the sequence. We have to begin by identifying a new slot and erasing it.
         */

         /* choose the next slot */
        uint32_t logBlobIndex = journal->currentBlobIndex + 1;
        if (logBlobIndex == journal->numSlots) {
            logBlobIndex = 0;
        }

        /* setup an erase for the slot */
        journal->log.mtdEraseOffset = SLOT_ADDRESS(journal, logBlobIndex);
        journal->state              = SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE; /* start with erasing the log region */
        journal->prevCommand        = FLASH_JOURNAL_OPCODE_LOG_BLOB;
    } else {
        /* This is a continuation of an ongoing logging sequence. */
        journal->log.dataBeingLogged = blob;
        journal->log.amountLeftToLog = size;
    }

    /* progress the state machine for log() */
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
        /* the tail has already been setup during previous calls to log(); we can now include it in the crc32. */
        journal->log.tail.crc32 = flashJournalCrcCummulative((const unsigned char *)&journal->log.tail, sizeof(SequentialFlashJournalLogTail_t));
        flashJournalCrcReset();

        journal->log.mtdOffset       = journal->log.mtdTailOffset;
        journal->log.dataBeingLogged = (const uint8_t *)&journal->log.tail;
        journal->log.amountLeftToLog = sizeof(SequentialFlashJournalLogTail_t);
        journal->state               = SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL;
    } else {
        uint32_t logBlobIndex = journal->currentBlobIndex + 1;
        if (logBlobIndex == journal->numSlots) {
            logBlobIndex = 0;
        }
        journal->log.mtdEraseOffset = SLOT_ADDRESS(journal, logBlobIndex);
        journal->state              = SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE;
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

int32_t mtdGetTotalCapacity(ARM_DRIVER_STORAGE *mtd, uint64_t *capacityP)
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

int32_t flashJournalStrategySequential_format_sanityChecks(ARM_DRIVER_STORAGE *mtd, uint32_t numSlots)
{
    /*
     * basic parameter checking
     */
    if ((mtd == NULL) || (numSlots == 0)) {
        return JOURNAL_STATUS_PARAMETER;
    }

    ARM_STORAGE_INFO mtdInfo;
    if (mtd->GetInfo(&mtdInfo) < ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    if (mtdInfo.total_storage == 0) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }

    uint64_t mtdAddr;
    if (mtdGetStartAddr(mtd, &mtdAddr) < JOURNAL_STATUS_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    if (mtd->GetBlock(mtdAddr, NULL) < ARM_DRIVER_OK) { /* check validity of journal's start address */
        return JOURNAL_STATUS_PARAMETER;
    }
    if (mtd->GetBlock(mtdAddr + mtdInfo.total_storage - 1, NULL) < ARM_DRIVER_OK) { /* check validity of the journal's end address */
        return JOURNAL_STATUS_PARAMETER;
    }

    if ((mtdAddr % mtdInfo.program_unit) != 0) { /* ensure that the journal starts at a programmable unit */
        return JOURNAL_STATUS_PARAMETER;
    }
    if ((mtdAddr % LCM_OF_ALL_ERASE_UNITS) != 0) { /* ensure that the journal starts and ends at an erase-boundary */
        return JOURNAL_STATUS_PARAMETER;
    }

    return JOURNAL_STATUS_OK;
}

int32_t flashJournalStrategySequential_read_sanityChecks(SequentialFlashJournal_t *journal, const void *blob, size_t sizeofBlob)
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
    // printf("read sanity checks: logicalOffset = %lu, sizeofJournaledBlob = %lu\n", (uint32_t)journal->read.logicalOffset, (uint32_t)journal->info.sizeofJournaledBlob);
    if ((journal->info.sizeofJournaledBlob == 0) || (journal->read.logicalOffset >= journal->info.sizeofJournaledBlob)) {
        journal->read.logicalOffset = 0;
        return JOURNAL_STATUS_EMPTY;
    }

    return JOURNAL_STATUS_OK;
}

int32_t flashJournalStrategySequential_log_sanityChecks(SequentialFlashJournal_t *journal, const void *blob, size_t sizeofBlob)
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
        if (journal->log.mtdOffset + sizeofBlob > journal->log.mtdTailOffset) {
            return JOURNAL_STATUS_BOUNDED_CAPACITY; /* adding this log chunk would cause us to exceed capacity (write past the tail). */
        }
    }

    /* ensure that the request is at least as large as the minimum program unit */
    if (sizeofBlob < journal->info.program_unit) {
        return JOURNAL_STATUS_SMALL_LOG_REQUEST;
    }

    return JOURNAL_STATUS_OK;
}

int32_t flashJournalStrategySequential_commit_sanityChecks(SequentialFlashJournal_t *journal)
{
    if (journal == NULL) {
        return JOURNAL_STATUS_PARAMETER;
    }
    if (journal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY) {
        if (journal->prevCommand != FLASH_JOURNAL_OPCODE_LOG_BLOB) {
            return JOURNAL_STATUS_ERROR;
        }
        if ((journal->log.mtdOffset       == ARM_STORAGE_INVALID_OFFSET) ||
            (journal->log.mtdTailOffset   == ARM_STORAGE_INVALID_OFFSET) ||
            (journal->log.mtdTailOffset    < journal->log.mtdOffset)     ||
            (journal->log.tail.sizeofBlob == 0)                          ||
            (journal->log.tail.sizeofBlob  > journal->info.capacity)) {
            return JOURNAL_STATUS_ERROR; /* journal is in an un-expected state. */
        }
    }

    return JOURNAL_STATUS_OK;
}
