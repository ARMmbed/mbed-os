/**
 * \file ac_debug.h
 * \copyright Copyright (c) ARM Ltd 2018
 * \author Donatien Garnier
 */
/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ACORE_DEBUG_H_
#define ACORE_DEBUG_H_

// Macro that can be defined to lock stdio in multithreaded environments
#if !defined(ACORE_STDIO_LOCK)
#define ACORE_STDIO_LOCK()
#endif

#if !defined(ACORE_STDIO_UNLOCK)
#define ACORE_STDIO_UNLOCK()
#endif

// Macro that can be defined to define an alternative printf impl for debugging
#if !defined(ACORE_STDIO_PRINT)
#include "stdio.h"
#define ACORE_STDIO_PRINT(...) printf(__VA_ARGS__)
#endif

#endif
