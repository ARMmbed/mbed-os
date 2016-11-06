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
#include "support_funcs.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

struct FormatInfo_t formatInfoSingleton;

int32_t mtdGetStartAddr(ARM_DRIVER_STORAGE *mtd, uint64_t *startAddrP)
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

/**
 * Check the sanity of a given slot
 * @param       journal
 * @param       slotOffset
 * @param [out] headSequenceNumberP
 *                  sequence number of the slot as read from the header.
 * @param [out] tailP
 *                  the tail of the slot
 * @return 1 if the slot is valid; i.e. if head and tail match, and if CRC32 agrees.
 */
int32_t slotIsSane(SequentialFlashJournal_t        *journal,
                   uint64_t                         slotOffset,
                   uint32_t                        *headSequenceNumberP,
                   SequentialFlashJournalLogTail_t *tailP)
{
    int32_t rc;
    ARM_DRIVER_STORAGE *mtd = journal->mtd;

    SequentialFlashJournalLogHead_t head;
    /* TODO: add support for asynchronous read */
    if (((rc = mtd->ReadData(slotOffset, &head, sizeof(SequentialFlashJournalLogHead_t))) < ARM_DRIVER_OK) ||
        (rc != sizeof(SequentialFlashJournalLogHead_t))) {
        if ((rc == ARM_DRIVER_OK) && (journal->mtdCapabilities.asynchronous_ops)) {
            return JOURNAL_STATUS_UNSUPPORTED;
        }

        return JOURNAL_STATUS_STORAGE_IO_ERROR;
    }

    /* compute the CRC32 of the header */
    flashJournalCrcReset();
    flashJournalCrcCummulative((const unsigned char *)&head, sizeof(SequentialFlashJournalLogHead_t));

    // printf("head->version: %lu\n", journal->initScan.head.version);
    // printf("head->magic: %lx\n", journal->initScan.head.magic);
    // printf("head->sequenceNumber: %lu\n", journal->initScan.head.sequenceNumber);
    // printf("head->reserved: %lu\n", journal->initScan.head.reserved);

    if (SEQUENTIAL_JOURNAL_VALID_HEAD(&head)) {
        *headSequenceNumberP = head.sequenceNumber;
        // printf("found valid header with sequenceNumber %" PRIu32 "\n", *headSequenceNumberP);

        uint64_t tailoffset = slotOffset
                              - ((slotOffset - SLOT_ADDRESS(journal, 0)) % journal->sizeofSlot)
                              + journal->sizeofSlot
                              - sizeof(SequentialFlashJournalLogTail_t);
        // printf("hoping to read a tail at offset %lu\n", (uint32_t)tailoffset);

        /* TODO: add support for asynchronous read */
        if (((rc = mtd->ReadData(tailoffset, tailP, sizeof(SequentialFlashJournalLogTail_t))) < ARM_DRIVER_OK) ||
            (rc != sizeof(SequentialFlashJournalLogTail_t))) {
            return JOURNAL_STATUS_STORAGE_IO_ERROR;
        }

        if (SEQUENTIAL_JOURNAL_VALID_TAIL(tailP) && (tailP->sequenceNumber == *headSequenceNumberP)) {
            // printf("found valid tail\n");

            /* iterate over the body of the slot computing CRC */
            #define CRC_CHUNK_SIZE 64
            uint8_t crcBuffer[CRC_CHUNK_SIZE];
            uint64_t bodyIndex = 0;
            uint64_t bodyOffset = slotOffset + sizeof(SequentialFlashJournalLogHead_t);
            while (bodyIndex < tailP->sizeofBlob) {
                size_t sizeofReadOperation;
                if ((tailP->sizeofBlob - bodyIndex) > CRC_CHUNK_SIZE) {
                    sizeofReadOperation = CRC_CHUNK_SIZE;
                } else {
                    sizeofReadOperation = (tailP->sizeofBlob - bodyIndex);
                }

                /* TODO: add support for asynchronous read */
                rc = mtd->ReadData(bodyOffset + bodyIndex, crcBuffer, sizeofReadOperation);
                if (rc != (int32_t)sizeofReadOperation) {
                    return JOURNAL_STATUS_STORAGE_IO_ERROR;
                }

                bodyIndex += sizeofReadOperation;
                flashJournalCrcCummulative(crcBuffer, sizeofReadOperation);
            }

            /* compute CRC32 over the tail */
            /* extract existing CRC32 from the tail. The CRC32 field in the tail needs to contain 0 before CRC32 can be computed over it. */
            uint32_t expectedCRC32 = tailP->crc32;
            tailP->crc32 = 0;

            uint32_t crc32 = flashJournalCrcCummulative((const unsigned char *)tailP, sizeof(SequentialFlashJournalLogTail_t));
            flashJournalCrcReset();
            // printf("expectedCRC32: 0x%x, computedCRC32: 0x%x\n", expectedCRC32, crc32);
            if (crc32 == expectedCRC32) {
                return 1;
            }
        }
    }

    return JOURNAL_STATUS_ERROR;
}

int32_t setupSequentialJournalHeader(SequentialFlashJournalHeader_t *headerP, ARM_DRIVER_STORAGE *mtd, uint64_t totalSize, uint32_t numSlots)
{
    ARM_STORAGE_INFO mtdInfo;
    if (mtd->GetInfo(&mtdInfo) < ARM_DRIVER_OK) {
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }

    headerP->genericHeader.magic        = FLASH_JOURNAL_HEADER_MAGIC;
    headerP->genericHeader.version      = FLASH_JOURNAL_HEADER_VERSION;
    headerP->genericHeader.sizeofHeader = sizeof(SequentialFlashJournalHeader_t);

    /* Determine 'journalOffset'.
     * Constraint: journal header should start and terminate at an erase-boundary
     * (so that slot-0 can be erased independently), and also a program-unit boundary.
     */
    headerP->genericHeader.journalOffset = roundUp_uint32(headerP->genericHeader.sizeofHeader, LCM_OF_ALL_ERASE_UNITS);
    if ((headerP->genericHeader.journalOffset % mtdInfo.program_unit) != 0) {
        //printf("setupSequentialJournalHeader: journalOffset is not a multiple of MTD's program_unit\r\n");
        return JOURNAL_STATUS_PARAMETER;
    }

    headerP->magic    = SEQUENTIAL_FLASH_JOURNAL_HEADER_MAGIC;
    headerP->version  = SEQUENTIAL_FLASH_JOURNAL_HEADER_VERSION;
    headerP->numSlots = numSlots;

    /* Determine 'sizeofSlot'.
     * Constraint: slot-size should be a multiple of the erase-units of all involved storage blocks.
     */
    uint64_t spaceAvailableForSlots = totalSize - headerP->genericHeader.journalOffset;
    headerP->sizeofSlot = roundDown_uint32(spaceAvailableForSlots / numSlots, LCM_OF_ALL_ERASE_UNITS);
    if (headerP->sizeofSlot == 0) {
        //printf("setupSequentialJournalHeader: not enough space to create %" PRIu32 " slots\r\n", numSlots);
        return JOURNAL_STATUS_PARAMETER;
    }

    headerP->genericHeader.totalSize = headerP->genericHeader.journalOffset + (headerP->sizeofSlot * numSlots);
    //printf("setupSequentialJournalHeader: header size = %" PRIu32 ", journalOffset = %" PRIu32 ", sizeofSlot = %" PRIu32 ", totalSize = %lu\n", headerP->genericHeader.sizeofHeader, headerP->genericHeader.journalOffset, headerP->sizeofSlot, (uint32_t)headerP->genericHeader.totalSize);

    /* compute checksum over the entire header */
    headerP->genericHeader.checksum = 0;
    flashJournalCrcReset();
    headerP->genericHeader.checksum = flashJournalCrcCummulative((const unsigned char *)&headerP->genericHeader, sizeof(SequentialFlashJournalLogHead_t));

    return JOURNAL_STATUS_OK;
}

int32_t discoverLatestLoggedBlob(SequentialFlashJournal_t *journal)
{
    /* reset top level journal metadata prior to scanning headers. */
    journal->nextSequenceNumber       = SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER; /* we are currently unaware of previously written blobs */
    journal->currentBlobIndex         = journal->numSlots;
    journal->info.sizeofJournaledBlob = 0;

    /* begin header-scan from the first block of the MTD */
    journal->initScan.currentOffset   = SLOT_ADDRESS(journal, 0);
    journal->state                    = SEQUENTIAL_JOURNAL_STATE_INIT_SCANNING_LOG_HEADERS;

    // printf("discoverLatestLoggedBlob: start of init scan\n");
    for (unsigned blobIndex = 0;
         blobIndex < journal->numSlots;
         blobIndex++, journal->initScan.currentOffset += journal->sizeofSlot) {
        // printf("discoverLatestLoggedBlob: blob index %u\n", blobIndex);
        /* TODO: it is possible that the header structure spans multiple blocks, needing multiple reads. */

        if (slotIsSane(journal,
                       journal->initScan.currentOffset,
                       &journal->initScan.headSequenceNumber,
                       &journal->initScan.tail) == 1) {
            // printf("found valid blob with sequence number %lu\n", journal->initScan.headSequenceNumber);
            uint32_t nextSequenceNumber = journal->initScan.headSequenceNumber + 1;
            if (nextSequenceNumber == SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER) {
                nextSequenceNumber = 0;
            }

            /* Have we found the best of the slots seen so far? */
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
    // printf("discoverLatestLoggedBlob: finished init scan\n");

    /* Handle the case where our scan hasn't yielded any results. */
    if (journal->nextSequenceNumber == SEQUENTIAL_FLASH_JOURNAL_INVALD_NEXT_SEQUENCE_NUMBER) {
        // printf("discoverLatestLoggedBlob: initializing to defaults\n");
        journal->currentBlobIndex   = (uint32_t)-1; /* to be incremented to 0 during the first attempt to log(). */
        journal->nextSequenceNumber = 0;
    }

    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;
    return JOURNAL_STATUS_OK;
}

/**
 * Progress the state machine for the 'format' operation. This method can also be called from an interrupt handler.
 * @return  < JOURNAL_STATUS_OK for error
 *          = JOURNAL_STATUS_OK to signal pending asynchronous activity
 *          > JOURNAL_STATUS_OK for completion
 */
int32_t flashJournalStrategySequential_format_progress(int32_t status, ARM_STORAGE_OPERATION operationWhichJustFinshed)
{
    int32_t rc;
    size_t sizeofWrite = roundUp_uint32(formatInfoSingleton.header.genericHeader.sizeofHeader, formatInfoSingleton.mtdProgramUnit);
    size_t sizeofErase = roundUp_uint32(formatInfoSingleton.header.genericHeader.sizeofHeader, LCM_OF_ALL_ERASE_UNITS);
    switch (operationWhichJustFinshed) {
        case ARM_STORAGE_OPERATION_INITIALIZE:
            if (status != ARM_DRIVER_OK) {
                return JOURNAL_STATUS_STORAGE_API_ERROR;
            }

            // printf("erasing %u bytes from offset %u\n", roundUp_uint32(header.genericHeader.sizeofHeader, mtdInfo.program_unit), mtdAddr);
            rc = (formatInfoSingleton.mtd)->Erase(formatInfoSingleton.mtdAddr, sizeofErase);
            if (rc < ARM_DRIVER_OK) {
                if (rc == ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE) {
                    return JOURNAL_STATUS_STORAGE_RUNTIME_OR_INTEGRITY_FAILURE;
                } else {
                    return JOURNAL_STATUS_STORAGE_IO_ERROR;
                }
            } else if (rc == ARM_DRIVER_OK) {
                return JOURNAL_STATUS_OK; /* An asynchronous operation is pending; it will result in a completion callback
                                           * where the rest of processing will take place. */
            }
            /* handle synchronous completion of programData */
            status = rc;

            /* intentional fall-through */

        case ARM_STORAGE_OPERATION_ERASE:
            if (status != (int32_t)sizeofErase) {
                return JOURNAL_STATUS_STORAGE_IO_ERROR;
            }

            // printf("calling ProgramData at address %u for %u bytes\n",
            //        formatInfoSingleton.mtdAddr, roundUp_uint32(formatInfoSingleton.header.genericHeader.sizeofHeader, formatInfoSingleton.mtdProgramUnit));
            rc = (formatInfoSingleton.mtd)->ProgramData(formatInfoSingleton.mtdAddr, &(formatInfoSingleton.header), sizeofWrite);
            if (rc < ARM_DRIVER_OK) {
                if (rc == ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE) {
                    return JOURNAL_STATUS_STORAGE_RUNTIME_OR_INTEGRITY_FAILURE;
                } else {
                    return JOURNAL_STATUS_STORAGE_IO_ERROR;
                }
            } else if (rc == ARM_DRIVER_OK) {
                return JOURNAL_STATUS_OK; /* An asynchronous operation is pending; it will result in a completion callback
                                           * where the rest of processing will take place. */
            }
            /* handle synchronous completion of programData */
            status = rc;

            /* intentional fall-through */

        case ARM_STORAGE_OPERATION_PROGRAM_DATA:
            if (status != (int32_t)sizeofWrite) {
                return JOURNAL_STATUS_STORAGE_IO_ERROR;
            }

            return 1; /* acknowledge the completion of create */

        default:
            return JOURNAL_STATUS_STORAGE_API_ERROR; /* we don't expect to be here */
    }
}

int32_t flashJournalStrategySequential_reset_progress(void)
{
    int32_t rc;
    SequentialFlashJournal_t *journal = activeJournal;

    if ((rc = journal->mtd->Erase(SLOT_ADDRESS(journal, 0), journal->numSlots * journal->sizeofSlot)) < ARM_DRIVER_OK) {
        journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
        if (rc == ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE) {
            return JOURNAL_STATUS_STORAGE_RUNTIME_OR_INTEGRITY_FAILURE;
        } else {
            return JOURNAL_STATUS_STORAGE_IO_ERROR;
        }
    }
    if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
        //printf("eturning JOURNAL_STATUS_OK\n");
        return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
    }
    /* else we fall through to handle synchronous completion */

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
        ((rc = journal->mtd->GetBlock(journal->read.mtdOffset, &storageBlock)) != ARM_DRIVER_OK)) {
        journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
        return JOURNAL_STATUS_STORAGE_API_ERROR;
    }
    uint64_t storageBlockAvailableCapacity = storageBlock.size - (journal->read.mtdOffset - storageBlock.addr);

    while (journal->read.amountLeftToRead) {
        while (!storageBlockAvailableCapacity) {
            if ((rc = journal->mtd->GetNextBlock(&storageBlock, &storageBlock)) < ARM_DRIVER_OK) {
                journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                return JOURNAL_STATUS_ERROR; /* We ran out of storage blocks. Journal is in an un-expected state. */
            }
            journal->read.mtdOffset       =  storageBlock.addr; /* This should not be necessary since we assume
                                                                 * storage map manages a contiguous address space. */
            storageBlockAvailableCapacity = storageBlock.size;
        }

        /* compute the transfer size for this iteration. */
        uint32_t xfer = (journal->read.amountLeftToRead < storageBlockAvailableCapacity) ?
                            journal->read.amountLeftToRead : storageBlockAvailableCapacity;

        /* perform the IO */
        //printf("reading %lu bytes at offset %lu\n", xfer, (uint32_t)journal->read.mtdOffset);
        rc = journal->mtd->ReadData(journal->read.mtdOffset, journal->read.dataBeingRead, xfer);
        if (rc < ARM_DRIVER_OK) {
            journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
            return JOURNAL_STATUS_STORAGE_IO_ERROR;
        }
        if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
            return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
        } else {
            /* synchronous completion. 'rc' contains the actual number of bytes transferred. */
            journal->read.mtdOffset        += rc;
            journal->read.amountLeftToRead -= rc;
            journal->read.dataBeingRead    += rc;
            journal->read.logicalOffset    += rc;
        }
    }

    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;
    return (journal->read.dataBeingRead - journal->read.blob);
}

/**
 * Progress the state machine for the 'log' operation. This method can also be called from an interrupt handler.
 * @return  < JOURNAL_STATUS_OK for error
 *          = JOURNAL_STATUS_OK to signal pending asynchronous activity
 *          > JOURNAL_STATUS_OK for completion
 */
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
    if (blobIndexBeingLogged == journal->numSlots) {
        blobIndexBeingLogged = 0;
    }

    while (true) {
        int32_t rc;

        if (journal->state == SEQUENTIAL_JOURNAL_STATE_LOGGING_ERASE) {
            uint64_t amountLeftToErase = SLOT_ADDRESS(journal, blobIndexBeingLogged + 1) - journal->log.mtdEraseOffset;
            // printf("journal state: erasing; offset %lu [size %lu]\n",
            //        (uint32_t)journal->log.eraseOffset, (uint32_t)amountLeftToErase);
            while (amountLeftToErase) {
                if ((rc = journal->mtd->Erase(journal->log.mtdEraseOffset, amountLeftToErase)) < ARM_DRIVER_OK) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    if (rc == ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE) {
                        return JOURNAL_STATUS_STORAGE_RUNTIME_OR_INTEGRITY_FAILURE;
                    } else {
                        return JOURNAL_STATUS_ERROR;
                    }
                }
                if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
                    return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
                } else {
                    /* synchronous completion. */
                    journal->log.mtdEraseOffset += rc;
                    amountLeftToErase           -= rc;
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
                if ((rc = journal->mtd->GetBlock(journal->log.mtdOffset, &storageBlock)) != ARM_DRIVER_OK) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    return JOURNAL_STATUS_STORAGE_API_ERROR;
                }
                if ((journal->log.mtdOffset - storageBlock.addr) % journal->info.program_unit) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    return JOURNAL_STATUS_ERROR; /* Program offset doesn't align with info.program_unit. This would result in an IO error if attempted. */
                }

                uint32_t xfer = journal->log.amountLeftToLog;
                xfer -= xfer % journal->info.program_unit; /* align transfer-size with program_unit. */

                /* perform the IO */
                // printf("programming %lu bytes at offset %lu\n", xfer, (uint32_t)journal->log.mtdOffset);
                rc = journal->mtd->ProgramData(journal->log.mtdOffset, journal->log.dataBeingLogged, xfer);
                if (rc < ARM_DRIVER_OK) {
                    journal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                    if (rc == ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE) {
                        return JOURNAL_STATUS_STORAGE_RUNTIME_OR_INTEGRITY_FAILURE;
                    } else {
                        return JOURNAL_STATUS_STORAGE_IO_ERROR;
                    }
                }
                if ((journal->mtdCapabilities.asynchronous_ops) && (rc == ARM_DRIVER_OK)) {
                    return JOURNAL_STATUS_OK; /* we've got pending asynchronous activity. */
                } else {
                    /* synchronous completion. 'rc' contains the actual number of bytes transferred. */
                    journal->log.mtdOffset       += rc;
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
                journal->log.mtdOffset           = SLOT_ADDRESS(journal, blobIndexBeingLogged);
                journal->log.head.version        = SEQUENTIAL_FLASH_JOURNAL_VERSION;
                journal->log.head.magic          = SEQUENTIAL_FLASH_JOURNAL_MAGIC;
                journal->log.head.sequenceNumber = journal->nextSequenceNumber;
                journal->log.head.reserved       = 0;
                journal->log.dataBeingLogged     = (const uint8_t *)&journal->log.head;
                journal->log.amountLeftToLog     = sizeof(SequentialFlashJournalLogHead_t);
                // printf("newstate: program HEAD; amount to log %u\n", journal->log.amountLeftToLog);
                break;

            case SEQUENTIAL_JOURNAL_STATE_LOGGING_HEAD: /* we've finished writing the head */
                /* compute CRC32 on the header */
                flashJournalCrcReset();
                flashJournalCrcCummulative((const unsigned char *)&journal->log.head, sizeof(SequentialFlashJournalLogHead_t));

                /* switch to writing the body */

                /* Prepare for the tail to be written out at a later time.
                 * This will only be done once Commit() is called. */
                journal->log.mtdTailOffset       = SLOT_ADDRESS(journal, blobIndexBeingLogged + 1) - sizeof(SequentialFlashJournalLogTail_t);

                journal->log.tail.magic          = SEQUENTIAL_FLASH_JOURNAL_MAGIC;
                journal->log.tail.sequenceNumber = journal->nextSequenceNumber;
                journal->log.tail.sizeofBlob     = 0; /* we'll update this as we complete our writes. */
                journal->log.tail.crc32          = 0;

                if (journal->prevCommand == FLASH_JOURNAL_OPCODE_COMMIT) {
                    /* This branch is taken only when commit() is called without any preceding log() operations. */
                    journal->log.tail.crc32      = flashJournalCrcCummulative((const unsigned char *)&journal->log.tail, sizeof(SequentialFlashJournalLogTail_t));
                    flashJournalCrcReset();

                    journal->state               = SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL;
                    journal->log.dataBeingLogged = (const uint8_t *)&journal->log.tail;
                    journal->log.amountLeftToLog = sizeof(SequentialFlashJournalLogTail_t);
                    journal->log.mtdOffset       = journal->log.mtdTailOffset;
                    // printf("newstate: program TAIL at offset %lu\r\n", (uint32_t)journal->log.mtdOffset);
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
                    uint32_t amountOfDataLogged = (journal->log.dataBeingLogged - journal->log.blob);
                    flashJournalCrcCummulative(journal->log.blob, amountOfDataLogged); /* compute CRC32 on logged data */
                    return amountOfDataLogged;
                }

            case SEQUENTIAL_JOURNAL_STATE_LOGGING_TAIL:
                // printf("crc32 of slot: 0x%x\n", journal->log.tail.crc32);

                journal->info.sizeofJournaledBlob = journal->log.tail.sizeofBlob;
                journal->state                    = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state to allow further operations */

                ++journal->currentBlobIndex;
                if (journal->currentBlobIndex == journal->numSlots) {
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

void formatHandler(int32_t status, ARM_STORAGE_OPERATION operation)
{
    if (status < ARM_DRIVER_OK) {
        if (formatInfoSingleton.callback) {
            formatInfoSingleton.callback(status, FLASH_JOURNAL_OPCODE_FORMAT);
        }
        return;
    }

    int32_t rc = flashJournalStrategySequential_format_progress(status, operation);
    if (rc != JOURNAL_STATUS_OK) {
        if (formatInfoSingleton.callback) {
            formatInfoSingleton.callback(rc, FLASH_JOURNAL_OPCODE_FORMAT);
        }
    }
}

void mtdHandler(int32_t status, ARM_STORAGE_OPERATION operation)
{
    int32_t rc;

    if (status < ARM_DRIVER_OK) {
        /* Map integrity failures reported by the Storage driver appropriately. */
        if (status == ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE) {
            status = JOURNAL_STATUS_STORAGE_RUNTIME_OR_INTEGRITY_FAILURE;
        } else {
            status = JOURNAL_STATUS_STORAGE_IO_ERROR;
        }

        // printf("journal mtdHandler: received error status %ld\n", status);
        switch (activeJournal->state) {
            case SEQUENTIAL_JOURNAL_STATE_NOT_INITIALIZED:
            case SEQUENTIAL_JOURNAL_STATE_INIT_SCANNING_LOG_HEADERS:
                if (activeJournal->callback) {
                    activeJournal->callback(status, FLASH_JOURNAL_OPCODE_INITIALIZE);
                }
                break;

            case SEQUENTIAL_JOURNAL_STATE_RESETING:
                activeJournal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED; /* reset state */
                if (activeJournal->callback) {
                    activeJournal->callback(status, FLASH_JOURNAL_OPCODE_RESET);
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
                    activeJournal->callback(status, FLASH_JOURNAL_OPCODE_LOG_BLOB);
                }
                break;
            case SEQUENTIAL_JOURNAL_STATE_READING:
                /* reset journal state to allow further operation. */
                activeJournal->state = SEQUENTIAL_JOURNAL_STATE_INITIALIZED;

                if (activeJournal->callback) {
                    activeJournal->callback(status, FLASH_JOURNAL_OPCODE_READ_BLOB);
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

                activeJournal->log.mtdEraseOffset += status;

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
            // printf("journal mtdHandler: PROGRAM_DATA: received status of %ld\n", status);
            rc = status;
            activeJournal->log.mtdOffset       += rc;
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
            //printf("mtdHandler: unknown operation %u\n", operation);
            break;
    }
}
