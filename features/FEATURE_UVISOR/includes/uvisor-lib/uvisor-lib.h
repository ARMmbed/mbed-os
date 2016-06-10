/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_LIB_UVISOR_LIB_H__
#define __UVISOR_LIB_UVISOR_LIB_H__

/* This file translates mbed-specific pre-processor symbols into
 * uVisor-specific ones. Then the main uvisor-lib.h file is included. */

/* mbed uses UVISOR_SUPPORTED to determine whether the full uVisor binaries
 * should be included or not. This symbol maps to the uVisor-internal symbol
 * UVISOR_PRESENT. */
/* By default uVisor is not supported. */
#if !defined(FEATURE_UVISOR) || !defined(TARGET_UVISOR_SUPPORTED) || defined(TARGET_UVISOR_UNSUPPORTED)
#define UVISOR_PRESENT 0
#else
#define UVISOR_PRESENT 1
#endif

/* Detect the target using the mbed-specific symbols and determine the MPU
 * architecture accordingly. */
#if defined(TARGET_KINETIS)
#define ARCH_MPU_KINETIS
#else
#define ARCH_MPU_ARMv7M
#endif

/* The uVisor API main header file will use the above definitions. */
#include "uvisor/api/inc/uvisor-lib.h"
#include "uvisor-lib/rtx/process_malloc.h"
#include "uvisor-lib/rtx/rtx_box_index.h"
#include "uvisor-lib/rtx/secure_allocator.h"

#endif /* __UVISOR_LIB_UVISOR_LIB_H__ */
