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

#include "support_funcs.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

static inline int32_t mtdGetStartAddr(ARM_DRIVER_STORAGE *mtd, uint64_t *startAddrP)
{
    ARM_STORAGE_BLOCK mtdBlock;
    if ((mtd->GetNextBlock(NULL, &mtdBlock)) != ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    if (!ARM_STORAGE_VALID_BLOCK(&mtdBlock)) {
        return JOURNAL_STATUS_ERROR;
    }

    *startAddrP = mtdBlock.addr;
    return JOURNAL_STATUS_OK;
}

int32_t discoverLatestLoggedBlob(SequentialFlashJournal_t *journal)
{
    int32_t rc;

    /* reset top level journal metadata prior to scanning headers. */
    journal->nextSequenceNumber       = SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER; /* we are currently unaware of previously written blobs */
    journal->currentBlobIndex         = SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS;
    journal->info.sizeofJournaledBlob = 0;

    /* begin header-scan from the first block of the MTD */
    ARM_DRIVER_STORAGE *mtd = journal->mtd;
    if ((rc = mtdGetStartAddr(journal->mtd, &journal->mtdStartOffset)) != JOURNAL_STATUS_OK) {
        return rc;
    }
    journal->initScan.currentOffset = journal->mtdStartOffset;
    journal->state                  = SEQUENTIAL_JOURNAL_STATE_INIT_SCANNING_LOG_HEADERS;

    // printf("start of init scan\n");
    for (unsigned blobIndex = 0;
         blobIndex < SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS;
         blobIndex++, journal->initScan.currentOffset += journal->sequentialSkip) {
        // printf("blob index %u\n", blobIndex);
        /* TODO: it is possible that the header structure spans multiple blocks, needing multiple reads. */

        if (((rc = mtd->ReadData(journal->initScan.currentOffset, &journal->initScan.head, sizeof(SequentialFlashJournalLogHead_t))) < ARM_DRIVER_OK) ||
            (rc != sizeof(SequentialFlashJournalLogHead_t))) {
            /* TODO: add support for asynchronous scan */
            if ((rc == ARM_DRIVER_OK) && (journal->mtdCapabilities.asynchronous_ops)) {
                return JOURNAL_STATUS_UNSUPPORTED;
            }

            return JOURNAL_STATUS_STORAGE_IO_ERROR;
        }
        // printf("head->version: %lu\n", journal->initScan.head.version);
        // printf("head->magic: %lx\n", journal->initScan.head.magic);
        // printf("head->sequenceNumber: %lu\n", journal->initScan.head.sequenceNumber);
        // printf("head->reserved: %lu\n", journal->initScan.head.reserved);

        if (SEQUENTIAL_JOURNAL_VALID_HEAD(&journal->initScan.head)) {
            journal->initScan.headSequenceNumber = journal->initScan.head.sequenceNumber;
            // printf("found valid header with sequenceNumber %lu\n", journal->initScan.headSequenceNumber);

            uint64_t tailoffset = journal->initScan.currentOffset
                                  - ((journal->initScan.currentOffset - journal->mtdStartOffset) % journal->sequentialSkip)
                                  + journal->sequentialSkip
                                  - sizeof(SequentialFlashJournalLogTail_t);

            // printf("hoping to read a tail at offset %lu\n", (uint32_t)tailoffset);
            if (((rc = mtd->ReadData(tailoffset, &journal->initScan.tail, sizeof(SequentialFlashJournalLogTail_t))) < ARM_DRIVER_OK) ||
                (rc != sizeof(SequentialFlashJournalLogTail_t))) {
                return JOURNAL_STATUS_STORAGE_IO_ERROR;
            }

            if (SEQUENTIAL_JOURNAL_VALID_TAIL(&journal->initScan.tail) &&
                (journal->initScan.tail.sequenceNumber == journal->initScan.headSequenceNumber)) {
                // printf("found valid blob with sequence number %lu\n", journal->initScan.headSequenceNumber);
                uint32_t nextSequenceNumber = journal->initScan.headSequenceNumber + 1;
                if (nextSequenceNumber == SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER) {
                    nextSequenceNumber = 0;
                }

                if ((journal->nextSequenceNumber == SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER) ||
                    /* We take advantage of properties of unsigned arithmetic in the following
                     * expression.
                     *
                     * We want to calculate if (nextSequenceNumber > journal->nextSequenceNumber),
                     * instead we use the expression ((nextSequenceNumber - journal->nextSequenceNumber) > 0)
                     * to take wraparounds into account.
                     */
                    ((int32_t)(nextSequenceNumber - journal->nextSequenceNumber) > 0)) {
                    journal->currentBlobIndex         = blobIndex;
                    journal->nextSequenceNumber       = nextSequenceNumber;
                    journal->info.sizeofJournaledBlob = journal->initScan.tail.sizeofBlob;
                    // printf("discoverLatestLoggedBlob: index %lu, sizeofBlob: %lu, nextSequenceNumber: %lu\n",
                    //        journal->currentBlobIndex, (uint32_t)journal->info.sizeofJournaledBlob, journal->nextSequenceNumber);
                }
            }
        }
    }
    // printf("finished init scan\n");

    /* Handle the case where our scan hasn't yielded any results. */
    if (journal->nextSequenceNumber == SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER) {
        // printf("discoverLatestLoggedBlob: initializing to defaults\n");
        journal->currentBlobIndex   = (uint32_t)-1; /* to be incremented to 0 during the first attempt to log(). */
        journal->nextSequenceNumber = 0;

        /* setup info.current_program_unit */
        ARM_STORAGE_BLOCK storageBlock;
        if ((rc = journal->mtd->GetBlock(journal->mtdStartOffset, &storageBlock)) != ARM_DRIVER_OK) {
            return JOURNAL_STATUS_STORAGE_API_ERROR;
        }
    }

    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;

    return JOURNAL_STATUS_OK;
}

int32_t flashJournalStrategySequential_reset_progress(void)
{
    int32_t rc;
    SequentialFlashJournal_t *journal = activeJournal;

    if (journal->mtdCapabilities.erase_all) {
        if ((rc = journal->mtd->EraseAll()) < ARM_DRIVER_OK) {
            journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
            return JOURNAL_STATUS_STORAGE_IO_ERROR;
        }
        if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
            return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
        }
        /* else we fall through to handle synchronous completion */
    } else {
        if ((rc = journal->mtd->Erase(journal->mtdStartOffset, SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS * journal->sequentialSkip)) < ARM_DRIVER_OK) {
            journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
            return JOURNAL_STATUS_STORAGE_IO_ERROR;
        }
        if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
            printf("eturning JOURNAL_STATUS_OK\n");
            return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
        }
        /* else we fall through to handle synchronous completion */
    }

    journal->nextSequenceNumber       = 0;
    journal->currentBlobIndex         = (uint32_t)-1;
    journal->info.sizeofJournaledBlob = 0;
    journal->state                    = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;
    return 1;
}

int32_t flashJournalStrategySequential_read_progress(void)
{
    SequentialFlashJournal_t *journal = activeJournal;

    // printf("flashJournalStrategySequential_read_progress\n");
    if (journal->state != SEQUENTIAL_JOURNAL_STATE_READING) {
        return JOURNAL_STATUS_ERROR; /* journal is in an un-expected state. */
    }

    int32_t rc;
    ARM_STORAGE_BLOCK storageBlock;

    if ((journal->read.amountLeftToRead) &&
        ((rc = journal->mtd->GetBlock(journal->read.offset, &storageBlock)) != ARM_DRIVER_OK)) {
        journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    uint64_t storageBlockAvailableCapacity = storageBlock.size - (journal->read.offset - storageBlock.addr);

    while (journal->read.amountLeftToRead) {
        while (!storageBlockAvailableCapacity) {
            if ((rc = journal->mtd->GetNextBlock(&storageBlock, &storageBlock)) < ARM_DRIVER_OK) {
                journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                return JOURNAL_STATUS_ERROR; /* We ran out of storage blocks. Journal is in an un-expected state. */
            }
            journal->read.offset          =  storageBlock.addr; /* This should not be necessary since we assume
                                                                 * storage map manages a contiguous address space. */
            storageBlockAvailableCapacity = storageBlock.size;
        }

        /* compute the transfer size for this iteration. */
        uint32_t xfer = (journal->read.amountLeftToRead < storageBlockAvailableCapacity) ?
                            journal->read.amountLeftToRead : storageBlockAvailableCapacity;

        /* perform the IO */
        //printf("reading %lu bytes at offset %lu\n", xfer, (uint32_t)journal->read.offset);
        rc = journal->mtd->ReadData(journal->read.offset, journal->read.dataBeingRead, xfer);
        if (rc < ARM_DRIVER_OK) {
            journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
            return JOURNAL_STATUS_STORAGE_IO_ERROR;
        }
        if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
            return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
        } else {
            /* synchronous completion. 'rc' contains the actual number of bytes transferred. */
            journal->read.offset           += rc;
            journal->read.amountLeftToRead -= rc;
            journal->read.dataBeingRead    += rc;
            journal->read.totalDataRead    += rc;
        }
    }

    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;
    return (journal->read.dataBeingRead - journal->read.blob);
}

int32_t flashJournalStrategySequential_log_progress(void)
{
    SequentialFlashJournal_t *journal = activeJournal;

    if ((journal->state != SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE) &&
        (journal->state != SEQUENTIAL_JOURNAL_STATE_LOGGING_HEAD)  &&
        (journal->state != SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY)  &&
        (journal->state != SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL)) {
        return JOURNAL_STATUS_ERROR; /* journal is in an un-expected state. */
    }

    uint32_t blobIndexBeingLogged = journal->currentBlobIndex + 1;
    if (blobIndexBeingLogged == SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS) {
        blobIndexBeingLogged = 0;
    }

    while (true) {
        int32_t rc;

        if (journal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE) {
            uint64_t amountLeftToErase = journal->mtdStartOffset
                                         + (blobIndexBeingLogged + 1) * journal->sequentialSkip
                                         - journal->log.eraseOffset;
            // printf("journal state: erasing; offset %lu [size %lu]\n",
            //        (uint32_t)journal->log.eraseOffset, (uint32_t)amountLeftToErase);
            while (amountLeftToErase) {
                if ((rc = journal->mtd->Erase(journal->log.eraseOffset, amountLeftToErase)) < ARM_DRIVER_OK) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    return JOURNAL_STATUS_ERROR; /* We ran out of storage blocks. Journal is in an un-expected state. */
                }
                if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
                    return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
                } else {
                    /* synchronous completion. */
                    journal->log.eraseOffset += rc;
                    amountLeftToErase        -= rc;
                }
            }
        } else {
            ARM_STORAGE_BLOCK storageBlock;

            /* find the available capacity in the current storage block */
            while (journal->log.amountLeftToLog) {
                if (journal->log.amountLeftToLog < journal->info.program_unit) {
                    /* We cannot log any smaller than info.program_unit. 'xfer'
                     * amount of data would remain unlogged. We'll break out of this loop and report
                     * the amount actually logged. */
                    break;
                }

                /* check for alignment of next log offset with program_unit */
                if ((rc = journal->mtd->GetBlock(journal->log.offset, &storageBlock)) != ARM_DRIVER_OK) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    return JOURNAL_STATUS_STORAGE_API_ERROR;
                }
                if ((journal->log.offset - storageBlock.addr) % journal->info.program_unit) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    return JOURNAL_STATUS_ERROR; /* Program offset doesn't align with info.program_unit. This would result in an IO error if attempted. */
                }

                uint32_t xfer = journal->log.amountLeftToLog;
                xfer -= xfer % journal->info.program_unit; /* align transfer-size with program_unit. */

                /* perform the IO */
                // printf("programming %lu bytes at offset %lu\n", xfer, (uint32_t)journal->log.offset);
                rc = journal->mtd->ProgramData(journal->log.offset, journal->log.dataBeingLogged, xfer);
                if (rc < ARM_DRIVER_OK) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    return JOURNAL_STATUS_STORAGE_IO_ERROR;
                }
                if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
                    return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
                } else {
                    /* synchronous completion. 'rc' contains the actual number of bytes transferred. */
                    journal->log.offset          += rc;
                    journal->log.amountLeftToLog -= rc;
                    journal->log.dataBeingLogged += rc;
                    if (journal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY) {
                        journal->log.tail.sizeofBlob += rc;
                    }
                }
            } /* while (journal->log.amountLeftToLog) */
        }

        // printf("flashJournalStrategySequential_log_progress: state switch\n");

        /* state transition */
        switch (journal->state) {
            case SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE:
                journal->state                   = SEQUENTIAL_JOURNAL_STATE_LOGGING_HEAD;
                journal->log.offset              = journal->mtdStartOffset + blobIndexBeingLogged * journal->sequentialSkip;
                journal->log.head.version        = SEQUENTIAL_FLASH_JOURNAL_VERSION;
                journal->log.head.magic          = SEQUENTIAL_FLASH_JOURNAL_MAGIC;
                journal->log.head.sequenceNumber = journal->nextSequenceNumber;
                journal->log.head.reserved       = 0;
                journal->log.dataBeingLogged     = (const uint8_t *)&journal->log.head;
                journal->log.amountLeftToLog     = sizeof(SequentialFlashJournalLogHead_t);
                // printf("newstate: program HEAD; amount to log %u\n", journal->log.amountLeftToLog);
                break;

            case SEQUENTIAL_JOURNAL_STATE_LOGGING_HEAD: /* switch to writing the body */
                /* Prepare for the tail to be written out at a later time.
                 * This will only be done once Commit() is called. */
                journal->log.tailOffset          = journal->mtdStartOffset
                                                   + (blobIndexBeingLogged + 1) * journal->sequentialSkip
                                                   - sizeof(SequentialFlashJournalLogTail_t);
                journal->log.tail.magic          = SEQUENTIAL_FLASH_JOURNAL_MAGIC;
                journal->log.tail.sequenceNumber = journal->nextSequenceNumber;
                journal->log.tail.sizeofBlob     = 0; /* we'll update this as we complete our writes. */

                if (journal->prevCommand == FLASH_JOURNAL_OPCODE_COMMIT) {
                    /* This branch is taken only when commit() is called without any preceding log() operations. */
                    journal->state               = SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL;
                    journal->log.dataBeingLogged = (const uint8_t *)&journal->log.tail;
                    journal->log.amountLeftToLog = sizeof(SequentialFlashJournalLogTail_t);
                    journal->log.offset          = journal->log.tailOffset;
                    // printf("newstate: program TAIL at offset %lu\r\n", (uint32_t)journal->log.offset);
                } else {
                    journal->state                   = SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY;
                    journal->log.dataBeingLogged     = journal->log.blob;
                    journal->log.amountLeftToLog     = journal->log.sizeofBlob;
                    // printf("newstate: program BODY; amount to log %u\n", journal->log.amountLeftToLog);
                }
                break;

            case SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY:
                // printf("finished logging BODY; amount logged %u\n", journal->log.dataBeingLogged - journal->log.blob);
                if (journal->log.dataBeingLogged == journal->log.blob) {
                    return JOURNAL_STATUS_SMALL_LOG_REQUEST;
                } else {
                    return (journal->log.dataBeingLogged - journal->log.blob);
                }

            case SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL:
                journal->info.sizeofJournaledBlob = journal->log.tail.sizeofBlob;
                journal->state                    = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state to allow further operations */

                ++journal->currentBlobIndex;
                if (journal->currentBlobIndex == SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS) {
                    journal->currentBlobIndex = 0;
                }
                // printf("currentBlobIndex: %lu\n", journal->currentBlobIndex);

                /* increment next sequence number */
                ++journal->nextSequenceNumber;
                if (journal->nextSequenceNumber == SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER) {
                    ++journal->nextSequenceNumber;
                }
                // printf("nextSequenceNumber %lu\n", journal->nextSequenceNumber);

                return 1; /* commit returns 1 upon completion. */

            default:
                journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;
                return JOURNAL_STATUS_ERROR;
        }
    }
}

void mtdHandler(int32_t status, ARM_STORAGE_OPERATION operation)
{
    int32_t rc;

    if (status < ARM_DRIVER_OK) {
        printf("mtdHandler: received error status %" PRId32 "\n", status);
        switch (activeJournal->state) {
            case SEQUENTIAL_JOURNAL_STATE_NOT_INITIALIZED:
            case SEQUENTIAL_JOURNAL_STATE_INIT_SCANNING_LOG_HEADERS:
                if (activeJournal->callback) {
                    activeJournal->callback(JOURNAL_STATUS_STORAGE_IO_ERROR, FLASH_JOURNAL_OPCODE_INITIALIZE);
                }
                break;

            case SEQUENTIAL_JOURNAL_STATE_RESETING:
                activeJournal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                if (activeJournal->callback) {
                    activeJournal->callback(JOURNAL_STATUS_STORAGE_IO_ERROR, FLASH_JOURNAL_OPCODE_RESET);
                }
                break;

            case SEQUENTIAL_JOURNAL_STATE_INITIALIZED:
            case SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE:
            case SEQUENTIAL_JOURNAL_STATE_LOGGING_HEAD:
            case SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY:
            case SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL:
                /* reset journal state to allow further operation. */
                activeJournal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;

                if (activeJournal->callback) {
                    activeJournal->callback(JOURNAL_STATUS_STORAGE_IO_ERROR, FLASH_JOURNAL_OPCODE_LOG_BLOB);
                }
                break;
            case SEQUENTIAL_JOURNAL_STATE_READING:
                /* reset journal state to allow further operation. */
                activeJournal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;

                if (activeJournal->callback) {
                    activeJournal->callback(JOURNAL_STATUS_STORAGE_IO_ERROR, FLASH_JOURNAL_OPCODE_READ_BLOB);
                }
                break;
        }

        return;
    }

    switch (operation) {
        case ARM_STORAGE_OPERATION_INITIALIZE:
            if (activeJournal->callback) {
                activeJournal->callback(JOURNAL_STATUS_OK, FLASH_JOURNAL_OPCODE_INITIALIZE);
            }
            break;

        case ARM_STORAGE_OPERATION_ERASE_ALL:
            if (activeJournal->state == SEQUENTIAL_JOURNAL_STATE_RESETING) {
                activeJournal->nextSequenceNumber       = 0;
                activeJournal->currentBlobIndex         = (uint32_t)-1;
                activeJournal->info.sizeofJournaledBlob = 0;
                activeJournal->state                    = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;
                if (activeJournal->callback) {
                    activeJournal->callback(JOURNAL_STATUS_OK, FLASH_JOURNAL_OPCODE_RESET);
                }
            }
            break;

        case ARM_STORAGE_OPERATION_ERASE:
            if (activeJournal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE) {
                if (status <= ARM_DRIVER_OK) {
                    if (activeJournal->callback) {
                        activeJournal->callback(JOURNAL_STATUS_STORAGE_API_ERROR, FLASH_JOURNAL_OPCODE_LOG_BLOB);
                    }
                    return;
                }

                activeJournal->log.eraseOffset += status;

                if ((rc = flashJournalStrategySequential_log_progress()) != JOURNAL_STATUS_OK) {
                    activeJournal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    if (activeJournal->callback) {
                        activeJournal->callback(rc, FLASH_JOURNAL_OPCODE_LOG_BLOB);
                    }
                    return;
                }
            } else if (activeJournal->state == SEQUENTIAL_JOURNAL_STATE_RESETING) {
                activeJournal->nextSequenceNumber       = 0;
                activeJournal->currentBlobIndex         = (uint32_t)-1;
                activeJournal->info.sizeofJournaledBlob = 0;
                activeJournal->state                    = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;
                if (activeJournal->callback) {
                    activeJournal->callback(JOURNAL_STATUS_OK, FLASH_JOURNAL_OPCODE_RESET);
                }
            }
            break;

        case ARM_STORAGE_OPERATION_PROGRAM_DATA:
            // printf("PROGRAM_DATA: received status of %ld\n", status);
            rc = status;
            activeJournal->log.offset          += rc;
            activeJournal->log.amountLeftToLog -= rc;
            activeJournal->log.dataBeingLogged += rc;
            if (activeJournal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_BODY) {
                activeJournal->log.tail.sizeofBlob += rc;
            }

            if ((rc = flashJournalStrategySequential_log_progress()) < JOURNAL_STATUS_OK) {
                activeJournal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                if (activeJournal->callback) {
                    activeJournal->callback(rc,
                                            (activeJournal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL) ?
                                                FLASH_JOURNAL_OPCODE_COMMIT : FLASH_JOURNAL_OPCODE_LOG_BLOB);
                }
                return;
            }
            if ((rc == JOURNAL_STATUS_OK) && (activeJournal->log.amountLeftToLog > 0)) {
                return; /* we've got pending asynchronous activity */
            }
            if (activeJournal->callback) {
                activeJournal->callback(rc, (activeJournal->state == SEQUENTIAL_JOURNAL_STATE_INITIALIZED) ?
                                                FLASH_JOURNAL_OPCODE_COMMIT : FLASH_JOURNAL_OPCODE_LOG_BLOB);
            }
            break;

        default:
            printf("mtdHandler: unknown operation %u\n", operation);
            break;
    }
}
