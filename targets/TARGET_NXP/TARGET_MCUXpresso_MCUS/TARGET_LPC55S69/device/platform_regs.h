/*
 * Copyright (c) 2016-2018 Arm Limited
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
 *
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ARM_LTD_NIOBE_REGS_H__
#define __ARM_LTD_NIOBE_REGS_H__

#include <stdint.h>
#include "platform_retarget.h"

/* Secure System Control (SYSCTRL) Alias */
#define CMSDK_SYSCTRL_BASE_S    SYSCON_BASE + 0xFA4

/* sysctrl memory mapped register access structure */
struct sysctrl_t {
		union {
			volatile uint32_t secdbgstat;             /* (R/ ) Secure Debug Configuration
                                               *       Status Register*/
      volatile uint32_t secdbgset;              /* ( /W) Secure Debug Configuration
                                               *       Set Register */
      volatile uint32_t secdbgclr;              /* ( /W) Secure Debug Configuration
                                               *       Clear Register */
		};
};

#endif /* __ARM_LTD_NIOBE_REGS_H__ */
