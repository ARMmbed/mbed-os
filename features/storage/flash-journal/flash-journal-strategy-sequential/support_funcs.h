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

extern SequentialFlashJournal_t *activeJournal;

int32_t discoverLatestLoggedBlob(SequentialFlashJournal_t *journal);
int32_t flashJournalStrategySequential_reset_progress(void);
int32_t flashJournalStrategySequential_read_progress(void);
int32_t flashJournalStrategySequential_log_progress(void);
void    mtdHandler(int32_t status, ARM_STORAGE_OPERATION operation);

#endif /*__FLASH_JOURNAL_SEQUENTIAL_STRATEGY_SUPPORT_FUNCTIONS_H__*/
