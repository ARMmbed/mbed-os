/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
 * Copyright (c) 2021 Embedded Planet, Inc.
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

#ifndef _NORDIC_SUBTARGET_INIT_
#define _NORDIC_SUBTARGET_INIT_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Since Mbed's `mbed_sdk_init` hook is used by the NRF52 family code, this
 * initialization hook is provided so subtargets may implement their own startup
 * initialization code, if necessary.
 *
 * By default, it is a blank function that is declared a "weak" symbol
 */
void subtarget_sdk_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _NORDIC_SUBTARGET_INIT_ */
