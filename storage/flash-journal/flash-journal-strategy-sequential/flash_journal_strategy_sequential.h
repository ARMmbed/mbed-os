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

int32_t               flashJournalStrategySequential_initialize(FlashJournal_t           *journal,
                                                                ARM_DRIVER_STORAGE       *mtd,
                                                                const FlashJournal_Ops_t *ops,
                                                                FlashJournal_Callback_t   callback);
FlashJournal_Status_t flashJournalStrategySequential_getInfo(FlashJournal_t *journal, FlashJournal_Info_t *info);
int32_t               flashJournalStrategySequential_read(FlashJournal_t *journal, void *blob, size_t n);
int32_t               flashJournalStrategySequential_log(FlashJournal_t *journal, const void *blob, size_t n);
int32_t               flashJournalStrategySequential_commit(FlashJournal_t *journal);
int32_t               flashJournalStrategySequential_reset(FlashJournal_t *journal);

static const FlashJournal_Ops_t FLASH_JOURNAL_STRATEGY_SEQUENTIAL = {
    flashJournalStrategySequential_initialize,
    flashJournalStrategySequential_getInfo,
    flashJournalStrategySequential_read,
    flashJournalStrategySequential_log,
    flashJournalStrategySequential_commit,
    flashJournalStrategySequential_reset
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __FLASH_JOURNAL_STRATEGY_SEQUENTIAL_H__ */
