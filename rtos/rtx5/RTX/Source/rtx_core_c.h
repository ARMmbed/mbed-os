/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       Cortex Core definitions
 *
 * -----------------------------------------------------------------------------
 */

#ifndef RTX_CORE_C_H_
#define RTX_CORE_C_H_

#include "RTE_Components.h"
#include CMSIS_device_header

#ifndef __ARM_ARCH_6M__
#define __ARM_ARCH_6M__         0U
#endif
#ifndef __ARM_ARCH_7A__
#define __ARM_ARCH_7A__         0U
#endif
#ifndef __ARM_ARCH_7M__
#define __ARM_ARCH_7M__         0U
#endif
#ifndef __ARM_ARCH_7EM__
#define __ARM_ARCH_7EM__        0U
#endif
#ifndef __ARM_ARCH_8M_BASE__
#define __ARM_ARCH_8M_BASE__    0U
#endif
#ifndef __ARM_ARCH_8M_MAIN__
#define __ARM_ARCH_8M_MAIN__    0U
#endif

#if   ((__ARM_ARCH_6M__      + \
        __ARM_ARCH_7A__      + \
        __ARM_ARCH_7M__      + \
        __ARM_ARCH_7EM__     + \
        __ARM_ARCH_8M_BASE__ + \
        __ARM_ARCH_8M_MAIN__) != 1U)
#error "Unknown ARM Architecture!"
#endif

#if (__ARM_ARCH_7A__ != 0U)
#include "rtx_core_ca.h"
#else
#include "rtx_core_cm.h"
#endif

#endif  // RTX_CORE_C_H_
