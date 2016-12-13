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
#ifndef __UVISOR_API_PAGE_ALLOCATOR_H__
#define __UVISOR_API_PAGE_ALLOCATOR_H__

#include "api/inc/uvisor_exports.h"
#include "api/inc/page_allocator_exports.h"
#include "api/inc/api.h"
#include <stdint.h>

UVISOR_EXTERN_C_BEGIN

/* Allocate a number of requested pages with the requested page size.
 * @param table.page_size[in]     Must be equal to the current page size
 * @param table.page_count[in]    The number of pages to be allocated
 * @param table.page_origins[out] Pointers to the page origins. The table must be large enough to hold page_count entries.
 * @returns Non-zero on failure with failure class `UVISOR_ERROR_CLASS_PAGE`. See `UVISOR_ERROR_PAGE_*`.
 */
static UVISOR_FORCEINLINE int uvisor_page_malloc(UvisorPageTable * const table)
{
    return uvisor_api.page_malloc(table);
}

/* Free the pages associated with the table, only if it passes validation.
 * @returns Non-zero on failure with failure class `UVISOR_ERROR_CLASS_PAGE`. See `UVISOR_ERROR_PAGE_*`.
 */
static UVISOR_FORCEINLINE int uvisor_page_free(const UvisorPageTable * const table)
{
    return uvisor_api.page_free(table);
}

/* @returns the active page size for one page. */
static UVISOR_FORCEINLINE uint32_t uvisor_get_page_size(void)
{
    return __uvisor_page_size;
}

UVISOR_EXTERN_C_END

#endif /* __UVISOR_API_PAGE_ALLOCATOR_H__ */
