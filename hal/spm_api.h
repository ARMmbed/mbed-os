
/** \addtogroup hal */
/** @{*/
/* Copyright (c) 2017-2018 ARM Limited
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

#ifndef __SPM_API_H__
#define __SPM_API_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup SPM-HAL SPM HAL API
 *  The HAL functions for PSA SPM
 * @{
 */


/* ------------------------------------ HAL-SPE API ------------------------- */


#if defined(COMPONENT_SPE)
/**
 * Start running the NSPE.
 *
 * Secure Processing Environment (SPE) expected to boot first. Once all
 * the initializations are done, Nonsecure Processing Environment (NSPE)
 * should be booted.
 *
 * @note The function must be implemented by target specific code.
 */
void spm_hal_start_nspe(void);


/**
 * Configure memory protection mechanism.
 *
 * Apply memory protection schemes to ensure secure memory can only be accessed
 * from secure-state.
 *
 * @note The function must be implemented by target specific code.
 *
 */
void spm_hal_memory_protection_init(void);

#endif // defined(COMPONENT_SPE)

#ifdef __cplusplus
}
#endif

#endif  // __SPM_API_H__

/** @}*/
