/***************************************************************************//**
* \file cyip_fault_v2.h
*
* \brief
* FAULT IP definitions
*
* \note
* Generator version: 1.5.0.1286
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
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

#ifndef _CYIP_FAULT_V2_H_
#define _CYIP_FAULT_V2_H_

#include "cyip_headers.h"

/*******************************************************************************
*                                    FAULT
*******************************************************************************/

#define FAULT_STRUCT_V2_SECTION_SIZE            0x00000100UL
#define FAULT_V2_SECTION_SIZE                   0x00010000UL

/**
  * \brief Fault structure (FAULT_STRUCT)
  */
typedef struct {
  __IOM uint32_t CTL;                           /*!< 0x00000000 Fault control */
   __IM uint32_t RESERVED[2];
  __IOM uint32_t STATUS;                        /*!< 0x0000000C Fault status */
  __IOM uint32_t DATA[4];                       /*!< 0x00000010 Fault data */
   __IM uint32_t RESERVED1[8];
   __IM uint32_t PENDING0;                      /*!< 0x00000040 Fault pending 0 */
   __IM uint32_t PENDING1;                      /*!< 0x00000044 Fault pending 1 */
   __IM uint32_t PENDING2;                      /*!< 0x00000048 Fault pending 2 */
   __IM uint32_t RESERVED2;
  __IOM uint32_t MASK0;                         /*!< 0x00000050 Fault mask 0 */
  __IOM uint32_t MASK1;                         /*!< 0x00000054 Fault mask 1 */
  __IOM uint32_t MASK2;                         /*!< 0x00000058 Fault mask 2 */
   __IM uint32_t RESERVED3[25];
  __IOM uint32_t INTR;                          /*!< 0x000000C0 Interrupt */
  __IOM uint32_t INTR_SET;                      /*!< 0x000000C4 Interrupt set */
  __IOM uint32_t INTR_MASK;                     /*!< 0x000000C8 Interrupt mask */
   __IM uint32_t INTR_MASKED;                   /*!< 0x000000CC Interrupt masked */
   __IM uint32_t RESERVED4[12];
} FAULT_STRUCT_V2_Type;                         /*!< Size = 256 (0x100) */

/**
  * \brief Fault structures (FAULT)
  */
typedef struct {
        FAULT_STRUCT_V2_Type STRUCT[4];         /*!< 0x00000000 Fault structure */
} FAULT_V2_Type;                                /*!< Size = 1024 (0x400) */


/* FAULT_STRUCT.CTL */
#define FAULT_STRUCT_V2_CTL_TR_EN_Pos           0UL
#define FAULT_STRUCT_V2_CTL_TR_EN_Msk           0x1UL
#define FAULT_STRUCT_V2_CTL_OUT_EN_Pos          1UL
#define FAULT_STRUCT_V2_CTL_OUT_EN_Msk          0x2UL
#define FAULT_STRUCT_V2_CTL_RESET_REQ_EN_Pos    2UL
#define FAULT_STRUCT_V2_CTL_RESET_REQ_EN_Msk    0x4UL
/* FAULT_STRUCT.STATUS */
#define FAULT_STRUCT_V2_STATUS_IDX_Pos          0UL
#define FAULT_STRUCT_V2_STATUS_IDX_Msk          0x7FUL
#define FAULT_STRUCT_V2_STATUS_VALID_Pos        31UL
#define FAULT_STRUCT_V2_STATUS_VALID_Msk        0x80000000UL
/* FAULT_STRUCT.DATA */
#define FAULT_STRUCT_V2_DATA_DATA_Pos           0UL
#define FAULT_STRUCT_V2_DATA_DATA_Msk           0xFFFFFFFFUL
/* FAULT_STRUCT.PENDING0 */
#define FAULT_STRUCT_V2_PENDING0_SOURCE_Pos     0UL
#define FAULT_STRUCT_V2_PENDING0_SOURCE_Msk     0xFFFFFFFFUL
/* FAULT_STRUCT.PENDING1 */
#define FAULT_STRUCT_V2_PENDING1_SOURCE_Pos     0UL
#define FAULT_STRUCT_V2_PENDING1_SOURCE_Msk     0xFFFFFFFFUL
/* FAULT_STRUCT.PENDING2 */
#define FAULT_STRUCT_V2_PENDING2_SOURCE_Pos     0UL
#define FAULT_STRUCT_V2_PENDING2_SOURCE_Msk     0xFFFFFFFFUL
/* FAULT_STRUCT.MASK0 */
#define FAULT_STRUCT_V2_MASK0_SOURCE_Pos        0UL
#define FAULT_STRUCT_V2_MASK0_SOURCE_Msk        0xFFFFFFFFUL
/* FAULT_STRUCT.MASK1 */
#define FAULT_STRUCT_V2_MASK1_SOURCE_Pos        0UL
#define FAULT_STRUCT_V2_MASK1_SOURCE_Msk        0xFFFFFFFFUL
/* FAULT_STRUCT.MASK2 */
#define FAULT_STRUCT_V2_MASK2_SOURCE_Pos        0UL
#define FAULT_STRUCT_V2_MASK2_SOURCE_Msk        0xFFFFFFFFUL
/* FAULT_STRUCT.INTR */
#define FAULT_STRUCT_V2_INTR_FAULT_Pos          0UL
#define FAULT_STRUCT_V2_INTR_FAULT_Msk          0x1UL
/* FAULT_STRUCT.INTR_SET */
#define FAULT_STRUCT_V2_INTR_SET_FAULT_Pos      0UL
#define FAULT_STRUCT_V2_INTR_SET_FAULT_Msk      0x1UL
/* FAULT_STRUCT.INTR_MASK */
#define FAULT_STRUCT_V2_INTR_MASK_FAULT_Pos     0UL
#define FAULT_STRUCT_V2_INTR_MASK_FAULT_Msk     0x1UL
/* FAULT_STRUCT.INTR_MASKED */
#define FAULT_STRUCT_V2_INTR_MASKED_FAULT_Pos   0UL
#define FAULT_STRUCT_V2_INTR_MASKED_FAULT_Msk   0x1UL


#endif /* _CYIP_FAULT_V2_H_ */


/* [] END OF FILE */
