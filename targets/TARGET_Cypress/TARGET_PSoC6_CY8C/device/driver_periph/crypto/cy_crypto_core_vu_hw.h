/***************************************************************************//**
* \file cy_crypto_core_vu_hw.h
* \version 1.0
*
* \brief
*  This file provides constants and function prototypes
*  for the Vector Unit functions in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_VU_HW_H)
#define CY_CRYPTO_CORE_VU_HW_H

#include "cy_device_headers.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "syslib/cy_syslib.h"


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1))


/* VU Instructions */
#define CY_CRYPTO_VU_ALLOC_MEM(rdst, size) \
\
    (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_ALLOC_MEM_OPC << 24) | \
                            ((uint32_t)rdst << 12) | \
                             ((uint32_t)size - 1)))

#define CY_CRYPTO_VU_CALLOC_MEM(rdst, size) \
\
    {CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_ALLOC_MEM_OPC << 24) | \
                            ((uint32_t)rdst << 12) | \
                             ((uint32_t)size - 1)); \
memset( (CRYPTO->MEM_BUFF + (((CRYPTO->RF_DATA[rdst] >> 16) & 0x00003fff) >> 2)), 0x00, 4 * (size + 31) / 32);  }

#define CY_CRYPTO_VU_FREE_MEM(reg_mask) \
\
    (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_FREE_MEM_OPC << 24) | \
                            (uint32_t)reg_mask))

#define CY_CRYPTO_VU_SET_REG(rdst, data, size) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SET_REG_OPC << 24) | \
                           ((uint32_t)rdst << 26) | \
                           ((uint32_t)data << 12) | \
                           ((uint32_t)size - 1)))

#define CY_CRYPTO_VU_MOV_REG(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_MOV_REG_TO_STATUS_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc << 0)))

#define CY_CRYPTO_VU_LD_REG(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_LD_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc << 0)))

#define CY_CRYPTO_VU_ST_REG(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_ST_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc << 0)))

#define CY_CRYPTO_VU_SWAP_REG(rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SWAP_REG_OPC << 24) | \
                           ((uint32_t)rsrc1 << 4) | \
                           ((uint32_t)rsrc0 << 0)))

#define CY_CRYPTO_VU_COND_SWAP_REG(cc, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SWAP_REG_OPC << 24) | \
                           ((uint32_t)cc << 20) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_MOV_REG_TO_STATUS(rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_MOV_REG_TO_STATUS_OPC << 24) | (uint32_t)rsrc))

#define CY_CRYPTO_VU_MOV_STATUS_TO_REG(rdst) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_MOV_STATUS_TO_REG_OPC << 24) | ((uint32_t)rdst << 12)))

#define CY_CRYPTO_VU_PUSH_REG()  (CRYPTO->INSTR_FF_WR = ((uint32_t)CY_CRYPTO_VU_PUSH_REG_OPC << 24))

#define CY_CRYPTO_VU_POP_REG()   (CRYPTO->INSTR_FF_WR = ((uint32_t)CY_CRYPTO_VU_POP_REG_OPC << 24))

#define CY_CRYPTO_VU_ADD_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_ADD_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_SUB_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SUB_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_OR_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_OR_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_AND_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_AND_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_XOR_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_XOR_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_NOR_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_NOR_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_NAND_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_NAND_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_MIN_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_MIN_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_MAX_REG(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_MAX_REG_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                           (uint32_t)rsrc0))

#define CY_CRYPTO_VU_LSL(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_LSL_OPC << 24) |\
                           ((uint32_t)rdst << 12) |\
                           ((uint32_t)rsrc1 << 4) |\
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_LSR(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_LSR_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_LSL1(rdst, rsrc1) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_LSL1_OPC << 24) |\
                           ((uint32_t)rdst << 12) |\
                           ((uint32_t)rsrc1 << 4)))

#define CY_CRYPTO_VU_LSL1_WITH_CARRY(rdst, rsrc1) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_LSL1_WITH_CARRY_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4)))

#define CY_CRYPTO_VU_LSR1(rdst, rsrc1) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_LSR1_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4)))

#define CY_CRYPTO_VU_LSR1_WITH_CARRY(rdst, rsrc1) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_LSR1_WITH_CARRY_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4)))

#define CY_CRYPTO_VU_SET_BIT(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SET_BIT_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           (uint32_t)rsrc))

#define CY_CRYPTO_VU_CLR_BIT(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_CLR_BIT_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           (uint32_t)rsrc))

#define CY_CRYPTO_VU_INV_BIT(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_INV_BIT_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                            (uint32_t)rsrc))

#define CY_CRYPTO_VU_GET_BIT(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_GET_BIT_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_CLSAME(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_CLSAME_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_CTSAME(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_CTSAME_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_SET_TO_ZERO(rdst) \
\
    (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SET_TO_ZERO_OPC << 24) | ((uint32_t)rdst << 12)))

#define CY_CRYPTO_VU_SET_TO_ONE(rdst) \
\
    (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SET_TO_ONE_OPC << 24) | ((uint32_t)rdst << 12)))

#define CY_CRYPTO_VU_MOV(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_MOV_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                            (uint32_t)rsrc))

#define CY_CRYPTO_VU_COND_MOV(cc, rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_MOV_OPC << 24) | \
                           ((uint32_t)cc << 20) | \
                           ((uint32_t)rdst << 12) | \
                            (uint32_t)rsrc))

#define CY_CRYPTO_VU_XSQUARE(rdst, rsrc) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_XSQUARE_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                            (uint32_t)rsrc))

#define CY_CRYPTO_VU_CMP_SUB(rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_CMP_SUB_OPC << 24) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_COND_CMP_SUB(cc, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_CMP_SUB_OPC << 24) | \
                           ((uint32_t)cc << 20) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_CMP_DEGREE(rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_CMP_DEGREE_OPC << 24) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_TST(rsrc0)   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_TST_OPC << 24) | (uint32_t)rsrc0))

#define CY_CRYPTO_VU_XMUL(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_XMUL_OPC << 24) |\
                           ((uint32_t)rdst << 12) |\
                           ((uint32_t)rsrc1 << 4) |\
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_UMUL(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_UMUL_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_ADD(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_ADD_OPC << 24) |\
                           ((uint32_t)rdst << 12) |\
                           ((uint32_t)rsrc1 << 4) |\
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_COND_ADD(cc, rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_ADD_OPC << 24) | \
                           ((uint32_t)cc << 20) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_SUB(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SUB_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_COND_SUB(cc, rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_SUB_OPC << 24) | \
                           ((uint32_t)cc << 20) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_OR(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_OR_OPC  << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_AND(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_AND_OPC  << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_XOR(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_XOR_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_COND_XOR(cc, rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_XOR_OPC << 24) | \
                           ((uint32_t)cc << 20) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_NOR(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_NOR_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))

#define CY_CRYPTO_VU_NAND(rdst, rsrc1, rsrc0) \
\
   (CRYPTO->INSTR_FF_WR = (((uint32_t)CY_CRYPTO_VU_NAND_OPC << 24) | \
                           ((uint32_t)rdst << 12) | \
                           ((uint32_t)rsrc1 << 4) | \
                            (uint32_t)rsrc0))


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_VU_HW_H) */
