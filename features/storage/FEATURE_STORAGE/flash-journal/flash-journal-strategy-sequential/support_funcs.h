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

#ifndef __FLASH_JOURNAL_SEQUENTIAL_STRATEGY_SUPPORT_FUNCTIONS_H__
#define __FLASH_JOURNAL_SEQUENTIAL_STRATEGY_SUPPORT_FUNCTIONS_H__

#include "flash-journal-strategy-sequential/flash_journal_private.h"
#include "flash-journal-strategy-sequential/flash_journal_strategy_sequential.h"

/* The following singleton captures the state of the format machine. Format is
 * handled differently because it executes even before a journal exists (or a
 * Journal_t can be initialized. */
extern struct FormatInfo_t {
    ARM_DRIVER_STORAGE             *mtd;
    SequentialFlashJournalHeader_t  header;
    FlashJournal_Callback_t         callback;
    uint64_t                        mtdAddr;
    uint32_t                        mtdProgramUnit;
} formatInfoSingleton;

extern SequentialFlashJournal_t *activeJournal;

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
                   SequentialFlashJournalLogTail_t *tailP);

int32_t mtdGetStartAddr(ARM_DRIVER_STORAGE *mtd, uint64_t *startAddrP);
int32_t setupSequentialJournalHeader(SequentialFlashJournalHeader_t *headerP, ARM_DRIVER_STORAGE *mtd, uint64_t totalSize, uint32_t numSlots);
int32_t discoverLatestLoggedBlob(SequentialFlashJournal_t *journal);

/**
 * Progress the state machine for the 'format' operation. This method can also be called from an interrupt handler.
 * @return  < JOURNAL_STATUS_OK for error
 *          = JOURNAL_STATUS_OK to signal pending asynchronous activity
 *          > JOURNAL_STATUS_OK for completion
 */
int32_t flashJournalStrategySequential_format_progress(int32_t status, ARM_STORAGE_OPERATION operationWhichJustFinshed);

/**
 * Progress the state machine for the 'log' operation. This method can also be called from an interrupt handler.
 * @return  < JOURNAL_STATUS_OK for error
 *          = JOURNAL_STATUS_OK to signal pending asynchronous activity
 *          > JOURNAL_STATUS_OK for completion
 */
int32_t flashJournalStrategySequential_log_progress(void);

int32_t flashJournalStrategySequential_reset_progress(void);
int32_t flashJournalStrategySequential_read_progress(void);

void    mtdHandler(int32_t status, ARM_STORAGE_OPERATION operation);
void    formatHandler(int32_t status, ARM_STORAGE_OPERATION operation);

#endif /*__FLASH_JOURNAL_SEQUENTIAL_STRATEGY_SUPPORT_FUNCTIONS_H__*/
