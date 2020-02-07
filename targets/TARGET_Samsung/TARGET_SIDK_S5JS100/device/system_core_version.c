/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "system_core_version.h"

#define REALLY_MAKE_STR(y) #y
#define MAKE_STR(x) REALLY_MAKE_STR(x)
#define SYSTEM_CORE_VERSION() (SYSTEM_CORE_PLATFORM ".SYSTEM.CORE." \
    MAKE_STR(SYSTEM_CORE_OS) \
    "." MAKE_STR(SYSTEM_CORE_VERSION_MAJOR) \
    "." MAKE_STR(SYSTEM_CORE_VERSION_MINOR) \
    "." MAKE_STR(SYSTEM_CORE_VERSION_PATCH) \
    " " SYSTEM_CORE_DATE \
    " " SYSTEM_CORE_TIME)

/* Private Data */
const char *system_core_version = SYSTEM_CORE_VERSION();

/* Get System Core Version */
const char *SystemCoreGetVersion()
{
    return system_core_version;
}
