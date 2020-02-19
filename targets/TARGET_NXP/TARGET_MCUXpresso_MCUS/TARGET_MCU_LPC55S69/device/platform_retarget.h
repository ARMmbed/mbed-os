/*
 * Copyright (c) 2017-2018 Arm Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * \file platform_retarget.h
 * \brief This file defines all the peripheral base addresses for Niobe platform.
 */

#ifndef __ARM_LTD_NIOBE_RETARGET_H__
#define __ARM_LTD_NIOBE_RETARGET_H__

#include "platform_regs.h"           /* Platform registers */
#include "LPC55S69_cm33_core0.h"


/* SRAM MPC ranges and limits */
/* Internal memory */
#define MPC_ISRAM0_RANGE_BASE_NS   0x20000000
#define MPC_ISRAM0_RANGE_LIMIT_NS  0x2000FFFF
#define MPC_ISRAM0_RANGE_BASE_S    0x30000000
#define MPC_ISRAM0_RANGE_LIMIT_S   0x3000FFFF

#define MPC_ISRAM1_RANGE_BASE_NS   0x20010000
#define MPC_ISRAM1_RANGE_LIMIT_NS  0x2001FFFF
#define MPC_ISRAM1_RANGE_BASE_S    0x30010000
#define MPC_ISRAM1_RANGE_LIMIT_S   0x3001FFFF

#define MPC_ISRAM2_RANGE_BASE_NS   0x20020000
#define MPC_ISRAM2_RANGE_LIMIT_NS  0x2002FFFF
#define MPC_ISRAM2_RANGE_BASE_S    0x30020000
#define MPC_ISRAM2_RANGE_LIMIT_S   0x3002FFFF

#define MPC_ISRAM3_RANGE_BASE_NS   0x20030000
#define MPC_ISRAM3_RANGE_LIMIT_NS  0x2003FFFF
#define MPC_ISRAM3_RANGE_BASE_S    0x30030000
#define MPC_ISRAM3_RANGE_LIMIT_S   0x3003FFFF

#define MPC_ISRAM4_RANGE_BASE_NS   0x20040000
#define MPC_ISRAM4_RANGE_LIMIT_NS  0x20043FFF 
#define MPC_ISRAM4_RANGE_BASE_S    0x30040000
#define MPC_ISRAM4_RANGE_LIMIT_S   0x30043FFF

/* Code SRAM memory */
#define MPC_CODE_SRAM_RANGE_BASE_NS  (0x04000000)
#define MPC_CODE_SRAM_RANGE_LIMIT_NS (0x04007FFF)
#define MPC_CODE_SRAM_RANGE_BASE_S   (0x14000000)
#define MPC_CODE_SRAM_RANGE_LIMIT_S  (0x14007FFF)

/* Internal Flash memory */
#define FLASH0_BASE_S                 (0x10000000)
#define FLASH0_BASE_NS                (0x00000000)
#define FLASH0_SIZE                   (0x0009FFFF)  /* 640 kB */
#define FLASH0_SECTOR_SIZE            (0x00008000)  /* 32 kB */
#define FLASH0_PAGE_SIZE              (0x00000200)  /* 512 B */
#define FLASH0_PROGRAM_UNIT           (0x4)         /* Minimum write size */

#endif  /* __ARM_LTD_NIOBE_RETARGET_H__ */
