/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_EXPORT_TABLE_EXPORTS_H__
#define __UVISOR_API_EXPORT_TABLE_EXPORTS_H__

#include "rt_OsEventObserver.h"
#include <stdint.h>

/* If this magic doesn't match what you get in a TUvisorExportTable, then you
 * didn't find a TUvisorExportTable and all bets are off as to what will be
 * contained in what you found. */
#define UVISOR_EXPORT_MAGIC 0x5C9411B4

/* This is the export table API version. If this version doesn't match what you
 * get in TUvisorExportTable, then you need a different header file to
 * understand the TUvisorExportTable. */
#define UVISOR_EXPORT_VERSION 0

typedef struct {
    /* magic and version must be present as the first two elements in this
     * table so that across various versions of the table layout, the table can
     * be interpreted correctly. */
    uint32_t magic;
    uint32_t version;

    OsEventObserver os_event_observer;

    /* This must be the last element of the table so that uvisor-input.S can
     * export the size statically. */
    uint32_t size;
} TUvisorExportTable;

#endif
