/* Copyright (c) 2017-2019 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __TFM_PARTITION_DEFS_INC__
#define __TFM_PARTITION_DEFS_INC__

#ifdef TFM_PSA_API
#define ITS_ID (TFM_SP_BASE + 0)
#endif

#ifdef TFM_PSA_API
#define PLATFORM_ID (TFM_SP_BASE + 1)
#endif

#ifdef TFM_PSA_API
#define CRYPTO_SRV_ID (TFM_SP_BASE + 2)
#endif

#define TFM_MAX_USER_PARTITIONS (3)

#endif /* __TFM_PARTITION_DEFS_INC__ */