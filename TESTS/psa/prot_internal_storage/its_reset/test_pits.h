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

#ifndef __TEST_INTERNAL_TRUSTED_STORAGE_H__
#define __TEST_INTERNAL_TRUSTED_STORAGE_H__

/** @file
@brief This file describes the PSA Internal Trusted Storage API
*/

#include <stddef.h>
#include <stdint.h>
#include "psa_prot_internal_storage.h"

#ifdef   __cplusplus
extern "C"
{
#endif

/**
 * \brief Remove the provided key and its associated data from the storage
 *
 * \param[in] uid   The uid value
 *
 * \return  A status indicating the success/failure of the operation
 *
 * \retval      PSA_ITS_SUCCESS                  The operation completed successfully
 * \retval      PSA_ITS_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 */
psa_its_status_t test_psa_its_reset(void);

#ifdef   __cplusplus
}
#endif

#endif // __TEST_INTERNAL_TRUSTED_STORAGE_H__
