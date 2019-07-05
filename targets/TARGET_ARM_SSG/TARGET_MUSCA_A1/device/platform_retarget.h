/*
 * Copyright (c) 2017-2019 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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
 */

/**
 * \file platform_retarget.h
 * \brief This file defines all the peripheral base addresses for Musca platform.
 */

#ifndef __ARM_LTD_MUSCA_RETARGET_H__
#define __ARM_LTD_MUSCA_RETARGET_H__

#include "platform_regs.h"           /* Platform registers */
#include "system_cmsdk_musca.h"


/* SRAM MPC ranges and limits */
/* Internal memory */
#define MPC_ISRAM0_RANGE_BASE_NS   0x20000000
#define MPC_ISRAM0_RANGE_LIMIT_NS  0x20007FFF
#define MPC_ISRAM0_RANGE_BASE_S    0x30000000
#define MPC_ISRAM0_RANGE_LIMIT_S   0x30007FFF

#define MPC_ISRAM1_RANGE_BASE_NS   0x20008000
#define MPC_ISRAM1_RANGE_LIMIT_NS  0x2000FFFF
#define MPC_ISRAM1_RANGE_BASE_S    0x30008000
#define MPC_ISRAM1_RANGE_LIMIT_S   0x3000FFFF

#define MPC_ISRAM2_RANGE_BASE_NS   0x20010000
#define MPC_ISRAM2_RANGE_LIMIT_NS  0x20017FFF
#define MPC_ISRAM2_RANGE_BASE_S    0x30010000
#define MPC_ISRAM2_RANGE_LIMIT_S   0x30017FFF

#define MPC_ISRAM3_RANGE_BASE_NS   0x20018000
#define MPC_ISRAM3_RANGE_LIMIT_NS  0x2001FFFF
#define MPC_ISRAM3_RANGE_BASE_S    0x30018000
#define MPC_ISRAM3_RANGE_LIMIT_S   0x3001FFFF

/* Code SRAM memory */
#define MPC_CODE_SRAM_RANGE_BASE_NS  (0x00000000)
#define MPC_CODE_SRAM_RANGE_LIMIT_NS (0x00200000)
#define MPC_CODE_SRAM_RANGE_BASE_S   (0x10000000)
#define MPC_CODE_SRAM_RANGE_LIMIT_S  (0x10200000)

/* QSPI Flash memory */
#define MPC_QSPI_RANGE_BASE_NS        (0x00200000)
#define MPC_QSPI_RANGE_LIMIT_NS       (0x00240000)
#define MPC_QSPI_RANGE_BASE_S         (0x10200000)
#define MPC_QSPI_RANGE_LIMIT_S        (0x10240000)

#endif  /* __ARM_LTD_MUSCA_RETARGET_H__ */
