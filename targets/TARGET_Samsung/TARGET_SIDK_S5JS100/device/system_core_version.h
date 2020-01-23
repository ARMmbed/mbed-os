/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#ifndef SYSTEM_CORE_VERSION_H
#define SYSTEM_CORE_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Platform Name */
#define SYSTEM_CORE_PLATFORM        "SAMSUNG SIDK S5JS100 "

/* OS Version */
#define SYSTEM_CORE_OS              5

/* System Core Version */
#define SYSTEM_CORE_VERSION_MAJOR   0
#define SYSTEM_CORE_VERSION_MINOR   1
#define SYSTEM_CORE_VERSION_PATCH   0
#define SYSTEM_CORE_DATE            __DATE__
#define SYSTEM_CORE_TIME            __TIME__

/* Get System Core Version */
const char *SystemCoreGetVersion(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_CORE_VERSION_H */
