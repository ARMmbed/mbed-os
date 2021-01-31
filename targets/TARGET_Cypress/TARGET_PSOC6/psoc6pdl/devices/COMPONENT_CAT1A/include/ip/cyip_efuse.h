/***************************************************************************//**
* \file cyip_efuse.h
*
* \brief
* EFUSE IP definitions
*
* \note
* Generator version: 1.6.0.225
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#ifndef _CYIP_EFUSE_H_
#define _CYIP_EFUSE_H_

#include "cyip_headers.h"

/*******************************************************************************
*                                    EFUSE
*******************************************************************************/

#define EFUSE_SECTION_SIZE                      0x00000080UL

/**
  * \brief EFUSE MXS40 registers (EFUSE)
  */
typedef struct {
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control */
   __IM uint32_t RESERVED[3];
  __IOM uint32_t CMD;                           /*!< 0x00000010 Command */
   __IM uint32_t RESERVED1[3];
  __IOM uint32_t SEQ_DEFAULT;                   /*!< 0x00000020 Sequencer Default value */
   __IM uint32_t RESERVED2[7];
  __IOM uint32_t SEQ_READ_CTL_0;                /*!< 0x00000040 Sequencer read control 0 */
  __IOM uint32_t SEQ_READ_CTL_1;                /*!< 0x00000044 Sequencer read control 1 */
  __IOM uint32_t SEQ_READ_CTL_2;                /*!< 0x00000048 Sequencer read control 2 */
  __IOM uint32_t SEQ_READ_CTL_3;                /*!< 0x0000004C Sequencer read control 3 */
  __IOM uint32_t SEQ_READ_CTL_4;                /*!< 0x00000050 Sequencer read control 4 */
  __IOM uint32_t SEQ_READ_CTL_5;                /*!< 0x00000054 Sequencer read control 5 */
   __IM uint32_t RESERVED3[2];
  __IOM uint32_t SEQ_PROGRAM_CTL_0;             /*!< 0x00000060 Sequencer program control 0 */
  __IOM uint32_t SEQ_PROGRAM_CTL_1;             /*!< 0x00000064 Sequencer program control 1 */
  __IOM uint32_t SEQ_PROGRAM_CTL_2;             /*!< 0x00000068 Sequencer program control 2 */
  __IOM uint32_t SEQ_PROGRAM_CTL_3;             /*!< 0x0000006C Sequencer program control 3 */
  __IOM uint32_t SEQ_PROGRAM_CTL_4;             /*!< 0x00000070 Sequencer program control 4 */
  __IOM uint32_t SEQ_PROGRAM_CTL_5;             /*!< 0x00000074 Sequencer program control 5 */
} EFUSE_V1_Type;                                /*!< Size = 120 (0x78) */


/* EFUSE.CTL */
#define EFUSE_CTL_ENABLED_Pos                   31UL
#define EFUSE_CTL_ENABLED_Msk                   0x80000000UL
/* EFUSE.CMD */
#define EFUSE_CMD_BIT_DATA_Pos                  0UL
#define EFUSE_CMD_BIT_DATA_Msk                  0x1UL
#define EFUSE_CMD_BIT_ADDR_Pos                  4UL
#define EFUSE_CMD_BIT_ADDR_Msk                  0x70UL
#define EFUSE_CMD_BYTE_ADDR_Pos                 8UL
#define EFUSE_CMD_BYTE_ADDR_Msk                 0x1F00UL
#define EFUSE_CMD_MACRO_ADDR_Pos                16UL
#define EFUSE_CMD_MACRO_ADDR_Msk                0xF0000UL
#define EFUSE_CMD_START_Pos                     31UL
#define EFUSE_CMD_START_Msk                     0x80000000UL
/* EFUSE.SEQ_DEFAULT */
#define EFUSE_SEQ_DEFAULT_STROBE_A_Pos          16UL
#define EFUSE_SEQ_DEFAULT_STROBE_A_Msk          0x10000UL
#define EFUSE_SEQ_DEFAULT_STROBE_B_Pos          17UL
#define EFUSE_SEQ_DEFAULT_STROBE_B_Msk          0x20000UL
#define EFUSE_SEQ_DEFAULT_STROBE_C_Pos          18UL
#define EFUSE_SEQ_DEFAULT_STROBE_C_Msk          0x40000UL
#define EFUSE_SEQ_DEFAULT_STROBE_D_Pos          19UL
#define EFUSE_SEQ_DEFAULT_STROBE_D_Msk          0x80000UL
#define EFUSE_SEQ_DEFAULT_STROBE_E_Pos          20UL
#define EFUSE_SEQ_DEFAULT_STROBE_E_Msk          0x100000UL
#define EFUSE_SEQ_DEFAULT_STROBE_F_Pos          21UL
#define EFUSE_SEQ_DEFAULT_STROBE_F_Msk          0x200000UL
#define EFUSE_SEQ_DEFAULT_STROBE_G_Pos          22UL
#define EFUSE_SEQ_DEFAULT_STROBE_G_Msk          0x400000UL
/* EFUSE.SEQ_READ_CTL_0 */
#define EFUSE_SEQ_READ_CTL_0_CYCLES_Pos         0UL
#define EFUSE_SEQ_READ_CTL_0_CYCLES_Msk         0x3FFUL
#define EFUSE_SEQ_READ_CTL_0_STROBE_A_Pos       16UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_A_Msk       0x10000UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_B_Pos       17UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_B_Msk       0x20000UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_C_Pos       18UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_C_Msk       0x40000UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_D_Pos       19UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_D_Msk       0x80000UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_E_Pos       20UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_E_Msk       0x100000UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_F_Pos       21UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_F_Msk       0x200000UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_G_Pos       22UL
#define EFUSE_SEQ_READ_CTL_0_STROBE_G_Msk       0x400000UL
#define EFUSE_SEQ_READ_CTL_0_DONE_Pos           31UL
#define EFUSE_SEQ_READ_CTL_0_DONE_Msk           0x80000000UL
/* EFUSE.SEQ_READ_CTL_1 */
#define EFUSE_SEQ_READ_CTL_1_CYCLES_Pos         0UL
#define EFUSE_SEQ_READ_CTL_1_CYCLES_Msk         0x3FFUL
#define EFUSE_SEQ_READ_CTL_1_STROBE_A_Pos       16UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_A_Msk       0x10000UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_B_Pos       17UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_B_Msk       0x20000UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_C_Pos       18UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_C_Msk       0x40000UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_D_Pos       19UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_D_Msk       0x80000UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_E_Pos       20UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_E_Msk       0x100000UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_F_Pos       21UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_F_Msk       0x200000UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_G_Pos       22UL
#define EFUSE_SEQ_READ_CTL_1_STROBE_G_Msk       0x400000UL
#define EFUSE_SEQ_READ_CTL_1_DONE_Pos           31UL
#define EFUSE_SEQ_READ_CTL_1_DONE_Msk           0x80000000UL
/* EFUSE.SEQ_READ_CTL_2 */
#define EFUSE_SEQ_READ_CTL_2_CYCLES_Pos         0UL
#define EFUSE_SEQ_READ_CTL_2_CYCLES_Msk         0x3FFUL
#define EFUSE_SEQ_READ_CTL_2_STROBE_A_Pos       16UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_A_Msk       0x10000UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_B_Pos       17UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_B_Msk       0x20000UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_C_Pos       18UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_C_Msk       0x40000UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_D_Pos       19UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_D_Msk       0x80000UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_E_Pos       20UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_E_Msk       0x100000UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_F_Pos       21UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_F_Msk       0x200000UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_G_Pos       22UL
#define EFUSE_SEQ_READ_CTL_2_STROBE_G_Msk       0x400000UL
#define EFUSE_SEQ_READ_CTL_2_DONE_Pos           31UL
#define EFUSE_SEQ_READ_CTL_2_DONE_Msk           0x80000000UL
/* EFUSE.SEQ_READ_CTL_3 */
#define EFUSE_SEQ_READ_CTL_3_CYCLES_Pos         0UL
#define EFUSE_SEQ_READ_CTL_3_CYCLES_Msk         0x3FFUL
#define EFUSE_SEQ_READ_CTL_3_STROBE_A_Pos       16UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_A_Msk       0x10000UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_B_Pos       17UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_B_Msk       0x20000UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_C_Pos       18UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_C_Msk       0x40000UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_D_Pos       19UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_D_Msk       0x80000UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_E_Pos       20UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_E_Msk       0x100000UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_F_Pos       21UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_F_Msk       0x200000UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_G_Pos       22UL
#define EFUSE_SEQ_READ_CTL_3_STROBE_G_Msk       0x400000UL
#define EFUSE_SEQ_READ_CTL_3_DONE_Pos           31UL
#define EFUSE_SEQ_READ_CTL_3_DONE_Msk           0x80000000UL
/* EFUSE.SEQ_READ_CTL_4 */
#define EFUSE_SEQ_READ_CTL_4_CYCLES_Pos         0UL
#define EFUSE_SEQ_READ_CTL_4_CYCLES_Msk         0x3FFUL
#define EFUSE_SEQ_READ_CTL_4_STROBE_A_Pos       16UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_A_Msk       0x10000UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_B_Pos       17UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_B_Msk       0x20000UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_C_Pos       18UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_C_Msk       0x40000UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_D_Pos       19UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_D_Msk       0x80000UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_E_Pos       20UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_E_Msk       0x100000UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_F_Pos       21UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_F_Msk       0x200000UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_G_Pos       22UL
#define EFUSE_SEQ_READ_CTL_4_STROBE_G_Msk       0x400000UL
#define EFUSE_SEQ_READ_CTL_4_DONE_Pos           31UL
#define EFUSE_SEQ_READ_CTL_4_DONE_Msk           0x80000000UL
/* EFUSE.SEQ_READ_CTL_5 */
#define EFUSE_SEQ_READ_CTL_5_CYCLES_Pos         0UL
#define EFUSE_SEQ_READ_CTL_5_CYCLES_Msk         0x3FFUL
#define EFUSE_SEQ_READ_CTL_5_STROBE_A_Pos       16UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_A_Msk       0x10000UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_B_Pos       17UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_B_Msk       0x20000UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_C_Pos       18UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_C_Msk       0x40000UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_D_Pos       19UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_D_Msk       0x80000UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_E_Pos       20UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_E_Msk       0x100000UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_F_Pos       21UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_F_Msk       0x200000UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_G_Pos       22UL
#define EFUSE_SEQ_READ_CTL_5_STROBE_G_Msk       0x400000UL
#define EFUSE_SEQ_READ_CTL_5_DONE_Pos           31UL
#define EFUSE_SEQ_READ_CTL_5_DONE_Msk           0x80000000UL
/* EFUSE.SEQ_PROGRAM_CTL_0 */
#define EFUSE_SEQ_PROGRAM_CTL_0_CYCLES_Pos      0UL
#define EFUSE_SEQ_PROGRAM_CTL_0_CYCLES_Msk      0x3FFUL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_A_Pos    16UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_A_Msk    0x10000UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_B_Pos    17UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_B_Msk    0x20000UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_C_Pos    18UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_C_Msk    0x40000UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_D_Pos    19UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_D_Msk    0x80000UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_E_Pos    20UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_E_Msk    0x100000UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_F_Pos    21UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_F_Msk    0x200000UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_G_Pos    22UL
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_G_Msk    0x400000UL
#define EFUSE_SEQ_PROGRAM_CTL_0_DONE_Pos        31UL
#define EFUSE_SEQ_PROGRAM_CTL_0_DONE_Msk        0x80000000UL
/* EFUSE.SEQ_PROGRAM_CTL_1 */
#define EFUSE_SEQ_PROGRAM_CTL_1_CYCLES_Pos      0UL
#define EFUSE_SEQ_PROGRAM_CTL_1_CYCLES_Msk      0x3FFUL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_A_Pos    16UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_A_Msk    0x10000UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_B_Pos    17UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_B_Msk    0x20000UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_C_Pos    18UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_C_Msk    0x40000UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_D_Pos    19UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_D_Msk    0x80000UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_E_Pos    20UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_E_Msk    0x100000UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_F_Pos    21UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_F_Msk    0x200000UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_G_Pos    22UL
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_G_Msk    0x400000UL
#define EFUSE_SEQ_PROGRAM_CTL_1_DONE_Pos        31UL
#define EFUSE_SEQ_PROGRAM_CTL_1_DONE_Msk        0x80000000UL
/* EFUSE.SEQ_PROGRAM_CTL_2 */
#define EFUSE_SEQ_PROGRAM_CTL_2_CYCLES_Pos      0UL
#define EFUSE_SEQ_PROGRAM_CTL_2_CYCLES_Msk      0x3FFUL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_A_Pos    16UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_A_Msk    0x10000UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_B_Pos    17UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_B_Msk    0x20000UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_C_Pos    18UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_C_Msk    0x40000UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_D_Pos    19UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_D_Msk    0x80000UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_E_Pos    20UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_E_Msk    0x100000UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_F_Pos    21UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_F_Msk    0x200000UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_G_Pos    22UL
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_G_Msk    0x400000UL
#define EFUSE_SEQ_PROGRAM_CTL_2_DONE_Pos        31UL
#define EFUSE_SEQ_PROGRAM_CTL_2_DONE_Msk        0x80000000UL
/* EFUSE.SEQ_PROGRAM_CTL_3 */
#define EFUSE_SEQ_PROGRAM_CTL_3_CYCLES_Pos      0UL
#define EFUSE_SEQ_PROGRAM_CTL_3_CYCLES_Msk      0x3FFUL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_A_Pos    16UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_A_Msk    0x10000UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_B_Pos    17UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_B_Msk    0x20000UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_C_Pos    18UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_C_Msk    0x40000UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_D_Pos    19UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_D_Msk    0x80000UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_E_Pos    20UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_E_Msk    0x100000UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_F_Pos    21UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_F_Msk    0x200000UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_G_Pos    22UL
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_G_Msk    0x400000UL
#define EFUSE_SEQ_PROGRAM_CTL_3_DONE_Pos        31UL
#define EFUSE_SEQ_PROGRAM_CTL_3_DONE_Msk        0x80000000UL
/* EFUSE.SEQ_PROGRAM_CTL_4 */
#define EFUSE_SEQ_PROGRAM_CTL_4_CYCLES_Pos      0UL
#define EFUSE_SEQ_PROGRAM_CTL_4_CYCLES_Msk      0x3FFUL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_A_Pos    16UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_A_Msk    0x10000UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_B_Pos    17UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_B_Msk    0x20000UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_C_Pos    18UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_C_Msk    0x40000UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_D_Pos    19UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_D_Msk    0x80000UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_E_Pos    20UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_E_Msk    0x100000UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_F_Pos    21UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_F_Msk    0x200000UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_G_Pos    22UL
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_G_Msk    0x400000UL
#define EFUSE_SEQ_PROGRAM_CTL_4_DONE_Pos        31UL
#define EFUSE_SEQ_PROGRAM_CTL_4_DONE_Msk        0x80000000UL
/* EFUSE.SEQ_PROGRAM_CTL_5 */
#define EFUSE_SEQ_PROGRAM_CTL_5_CYCLES_Pos      0UL
#define EFUSE_SEQ_PROGRAM_CTL_5_CYCLES_Msk      0x3FFUL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_A_Pos    16UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_A_Msk    0x10000UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_B_Pos    17UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_B_Msk    0x20000UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_C_Pos    18UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_C_Msk    0x40000UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_D_Pos    19UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_D_Msk    0x80000UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_E_Pos    20UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_E_Msk    0x100000UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_F_Pos    21UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_F_Msk    0x200000UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_G_Pos    22UL
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_G_Msk    0x400000UL
#define EFUSE_SEQ_PROGRAM_CTL_5_DONE_Pos        31UL
#define EFUSE_SEQ_PROGRAM_CTL_5_DONE_Msk        0x80000000UL


#endif /* _CYIP_EFUSE_H_ */


/* [] END OF FILE */
