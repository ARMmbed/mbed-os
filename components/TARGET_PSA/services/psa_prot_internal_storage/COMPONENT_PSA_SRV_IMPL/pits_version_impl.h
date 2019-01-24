/* Copyright (c) 2018 ARM Limited
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

#ifndef __PITS_VER_IMPL_H__
#define __PITS_VER_IMPL_H__

#include "psa/internal_trusted_storage.h"

#ifdef   __cplusplus
extern "C"
{
#endif

#define ITS_VERSION_KEY "PSA_ITS_VERSION"  // ITS version entry identifier in TDBStore

typedef struct its_version {
    uint32_t major;
    uint32_t minor;
} its_version_t;

psa_its_status_t its_version_migrate(void *storage, const its_version_t *version);

#ifdef   __cplusplus
}
#endif

#endif // __PITS_VER_IMPL_H__
