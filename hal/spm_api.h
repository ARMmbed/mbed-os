/* Copyright (c) 2018 ARM Limited
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


/** @addtogroup SPM
 * The SPM (Secure Partition Manager) is responsible for isolating software in
 * partitions, managing the execution of software within partitions, and
 * providing IPC between partitions.
 * @{
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup HAL-SPE
 *  The HAL functions for SPE.
 * @{
 */


/* ------------------------------------ HAL-SPE API ------------------------- */


#if defined(COMPONENT_SPE)
/**
 * Start running the NSPE.
 *
 * SPE (Secure Processing Environment) expected to boot first. Once all
 * the initializations are done, NSPE (Non-Secure Processing Environment)
 * should be booted.
 *
 * @note The function must be implemented by target specific code.
 */
void spm_hal_start_nspe(void);


/**
 * Configure memory protection mechanism.
 *
 * Apply memory protection schemes to ensure secure memory can only be accessed
 * from secure-state
 *
 * @note The function must be implemented by target specific code.
 *
 */
void spm_hal_memory_protection_init(void);

#endif // defined(COMPONENT_SPE)

/* ---------------------------------- HAL-Mailbox API ----------------------- */

#if defined(COMPONENT_SPM_MAILBOX)
/**
 * @brief Wakeup mailbox dispatcher thread
 *
 * This function is implemented by ARM and expected to be called by target
 * specific Inter-Processor-Communication logic on mailbox interrupt handler.
 *
 */
void spm_mailbox_irq_callback(void);

/**
 * @brief Notify the peer processor about a general event occurrence.
 *
 * Wakeup the peer processor waiting on the mailbox driver event.
 *
 * @note The functions below should be implemented by target specific code.
 */
void spm_hal_mailbox_notify(void);

#endif // defined(COMPONENT_SPM_MAILBOX)

/** @}*/ // end of HAL-SPE group

#ifdef __cplusplus
}
#endif

/** @}*/ // end of SPM group

#endif  // __SPM_API_H__
