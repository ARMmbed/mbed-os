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

#ifndef __LIFECYCLE_H__
#define __LIFECYCLE_H__

/** @file
@brief This file describes the PSA RoT Lifecycle API
*/

#include <stddef.h>
#include <stdint.h>
#include "mbed_toolchain.h"
#include "psa/error.h"

#ifdef   __cplusplus
extern "C"
{
#endif

#define PSA_LIFECYCLE_STATE_MASK    (0xff00u)  /**< A mask value that extracts the main lifecycle state */
#define PSA_LIFECYCLE_SUBSTATE_MASK (0x00ffu)  /**< A mask value that extracts the IMPLEMENTATION DEFINED lifecycle sub-state */

#define PSA_LIFECYCLE_UNKNOWN                     (0x0000u)  /**< State is unknown */
#define PSA_LIFECYCLE_ASSEMBLY_AND_TEST           (0x1000u)  /**< Assembly and Test state */
#define PSA_LIFECYCLE_PSA_ROT_PROVISIONING        (0x2000u)  /**< PSA RoT Provisioning state */
#define PSA_LIFECYCLE_SECURED                     (0x3000u)  /**< Secured state */
#define PSA_LIFECYCLE_NON_PSA_ROT_DEBUG           (0x4000u)  /**< Non PSA RoT debug state */
#define PSA_LIFECYCLE_RECOVERABLE_PSA_ROT_DEBUG   (0x5000u)  /**< Recoverable PSA RoT Debug state */
#define PSA_LIFECYCLE_DECOMMISSIONED              (0x6000u)  /**< Decommissioned state */

/** \brief Get PSA RoT lifecycle state
 *
 * \retval The main state and sub-state are encoded as follows:@n
        @a version[15:8] – main lifecycle state
        @a version[7:0] – IMPLEMENTATION DEFINED sub-state
 */
uint32_t psa_security_lifecycle_state(void);

/** \brief Request state change
 *
 * State change requested and the reset the system.
 * \note System reset will not be performed when switching from PSA_LIFECYCLE_ASSEMBLY_AND_TEST
 * to PSA_LIFECYCLE_ASSEMBLY_AND_TEST.
 *
 * \note state change to follwing states will delete PSA internal storage:
 *   - PSA_LIFECYCLE_ASSEMBLY_AND_TEST
 *   - PSA_LIFECYCLE_PSA_ROT_PROVISIONING
 *   - PSA_LIFECYCLE_DECOMMISSIONED
 */
psa_status_t mbed_psa_reboot_and_request_new_security_state(uint32_t new_state);


/** \brief Resets the system
 *
 * PSA targets do not allow NSPE to access system power domain.
 * This API requests system reset to be carried out by SPE once all critical secure tasks are finished.
 */
MBED_NORETURN void mbed_psa_system_reset();

#ifdef   __cplusplus
}
#endif

#endif // __LIFECYCLE_H__
