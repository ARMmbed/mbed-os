/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
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
