/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S5JS100_RTOS_H__
#define __S5JS100_RTOS_H__
#define enable_icache()     SCB_EnableICache()
#define enable_dcache()     SCB_EnableDCache()
#define disable_icache()    SCB_DisableICache()
#define disable_dcache()    SCB_DisableDCache()

/*
 * I-CACHE operations
 */
#define invalidate_icache() SCB_InvalidateICache()

/*
 * D-CACHE operations
 */
#define invalidate_dcache()                         SCB_InvalidateDCache()
#define invalidate_dcache_by_addr(addr,size)        SCB_InvalidateDCache_by_Addr(addr, size)
#define clean_dcache()                              SCB_CleanDCache()
#define clean_dcache_by_addr(addr, size)            SCB_CleanDCache_by_Addr(addr, size)
#define clean_invalidate_dcache()                   SCB_CleanInvalidateDCache()
#define clean_invalidate_dcache_by_addr(addr, size) SCB_CleanInvalidateDCache_by_Addr(addr, size)


#endif
