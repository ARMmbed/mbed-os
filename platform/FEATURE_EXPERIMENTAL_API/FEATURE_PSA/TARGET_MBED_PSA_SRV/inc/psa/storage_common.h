/*  Copyright (C) 2019, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/** @file
@brief This file includes common definitions for PSA storage
*/

#ifndef __PSA_STORAGE_COMMON_H__
#define __PSA_STORAGE_COMMON_H__

#include <stddef.h>
#include <stdint.h>
#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Flags used when creating a data entry
 */
typedef uint32_t psa_storage_create_flags_t;

#define PSA_STORAGE_FLAG_NONE                 0         /**< No flags to pass */
#define PSA_STORAGE_FLAG_WRITE_ONCE           (1 << 0)  /**< The data associated with the uid will not be able to be modified or deleted. Intended to be used to set bits in `psa_storage_create_flags_t`*/
#define PSA_STORAGE_FLAG_NO_CONFIDENTIALITY   (1 << 1)  /**< The data associated with the uid is public and therefore does not require confidentiality. It therefore only needs to be integrity protected */
#define PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION (1 << 2)  /**< The data associated with the uid does not require replay protection. This may permit faster storage - but it permits an attecker with physical access to revert to an earlier version of the data.   */

/** \brief A type for UIDs used for identifying data
 */
typedef uint64_t psa_storage_uid_t;

/**
 * \brief A container for metadata associated with a specific uid
 */
struct psa_storage_info_t {
    size_t capacity;              /**< The allocated capacity of the storage associated with a UID **/
    size_t size;                  /**< The size of the data associated with a uid **/
    psa_storage_create_flags_t flags;    /**< The flags set when the uid was created **/
};

/** \brief Flag indicating that \ref psa_storage_create and \ref psa_storage_set_extended are supported */
#define PSA_STORAGE_SUPPORT_SET_EXTENDED (1 << 0)

/** \brief PSA storage specific error codes */
#define PSA_ERROR_DATA_CORRUPT          ((psa_status_t)-152)

#ifdef   __cplusplus
}
#endif

#endif // __PSA_STORAGE_COMMON_H__
