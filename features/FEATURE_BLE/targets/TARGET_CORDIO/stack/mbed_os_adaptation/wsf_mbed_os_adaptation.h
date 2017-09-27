/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WSF_MBED_OS_ADAPTATION_H_
#define WSF_MBED_OS_ADAPTATION_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Wrap core_util_critical_section_enter
 */
void wsf_mbed_os_critical_section_enter(void);

/**
 * Wrap core_util_critical_section_exit
 */
void wsf_mbed_os_critical_section_exit(void);

/**
 * Signal an event insertion in the Cordio stack to ble API.
 */
void wsf_mbed_ble_signal_event(void);

#ifdef __cplusplus
};
#endif

#endif /* WSF_MBED_OS_ADAPTATION_H_ */
