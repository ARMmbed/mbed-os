/***************************************************************************//**
* \file cy_crypto_core_instructions.h
* \version 1.0
*
* \brief
*  This file provides internal (not public) constant and parameters
*  for the Crypto server.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CRYPTO_CORE_INSTRUCTIONS_H)
#define CY_CRYPTO_CORE_INSTRUCTIONS_H

#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if (CPUSS_CRYPTO_PRESENT == 1)


/***************************************
*        Crypto IP opcodes
***************************************/

#define CY_CRYPTO_REGFILE_R0             (0x00uL)
#define CY_CRYPTO_REGFILE_R1             (0x01uL)
#define CY_CRYPTO_REGFILE_R2             (0x02uL)
#define CY_CRYPTO_REGFILE_R3             (0x03uL)
#define CY_CRYPTO_REGFILE_R4             (0x04uL)
#define CY_CRYPTO_REGFILE_R5             (0x05uL)
#define CY_CRYPTO_REGFILE_R6             (0x06uL)
#define CY_CRYPTO_REGFILE_R7             (0x07uL)
#define CY_CRYPTO_REGFILE_R8             (0x08uL)
#define CY_CRYPTO_REGFILE_R9             (0x09uL)
#define CY_CRYPTO_REGFILE_R10            (0x0AuL)
#define CY_CRYPTO_REGFILE_R11            (0x0BuL)
#define CY_CRYPTO_REGFILE_R12            (0x0CuL)
#define CY_CRYPTO_REGFILE_R13            (0x0DuL)
#define CY_CRYPTO_REGFILE_R14            (0x0EuL)
#define CY_CRYPTO_REGFILE_R15            (0x0FuL)


#define CY_CRYPTO_SET_REG1_OPC           (0x40u)
#define CY_CRYPTO_SET_REG2_OPC           (0x41u)
#define CY_CRYPTO_SET_REG3_OPC           (0x42u)
#define CY_CRYPTO_SET_REG4_OPC           (0x43u)

#define CY_CRYPTO_RSRC0_SHIFT            (0u)
#define CY_CRYPTO_RSRC4_SHIFT            (4u)
#define CY_CRYPTO_RSRC8_SHIFT            (8u)
#define CY_CRYPTO_RSRC12_SHIFT           (12u)
#define CY_CRYPTO_RSRC20_SHIFT           (20u)
#define CY_CRYPTO_OPCODE_POS             (24u)
#define CY_CRYPTO_RSRC26_SHIFT           (26u)
#define CY_CRYPTO_RSRC30_SHIFT           (30u)

#define CY_CRYPTO_AES_BLOCK_OPC          (0x44u)
#define CY_CRYPTO_AES_BLOCK_INV_OPC      (0x45u)
#define CY_CRYPTO_AES_KEY_OPC            (0x46u)
#define CY_CRYPTO_AES_KEY_INV_OPC        (0x47u)
#define CY_CRYPTO_AES_XOR_OPC            (0x48u)

#define CY_CRYPTO_SHA_OPC                (0x4cu)

#define CY_CRYPTO_STR_MEMCPY_OPC         (0x50u)
#define CY_CRYPTO_STR_MEMSET_OPC         (0x51u)
#define CY_CRYPTO_STR_MEMCMP_OPC         (0x52u)
#define CY_CRYPTO_STR_MEMXOR_OPC         (0x53u)

#define CY_CRYPTO_CRC_OPC                (0x58u)

#define CY_CRYPTO_PRNG_OPC               (0x5cu)

#define CY_CRYPTO_TRNG_OPC               (0x60u)

#define CY_CRYPTO_DES_BLOCK_OPC          (0x70u)
#define CY_CRYPTO_DES_BLOCK_INV_OPC      (0x71u)

#define CY_CRYPTO_TDES_BLOCK_OPC         (0x72u)
#define CY_CRYPTO_TDES_BLOCK_INV_OPC     (0x73u)

#define CY_CRYPTO_SYNC_OPC               (0x7fu)

#define CY_CRYPTO_FIFODEPTH              (0x08u)

/* Vector Unit instructions */
#define CY_CRYPTO_VU_ALLOC_MEM_OPC           (0x12u)
#define CY_CRYPTO_VU_FREE_MEM_OPC            (0x13u)

/* Instructions with register operand only, category I */
#define CY_CRYPTO_VU_SET_REG_OPC             (0x80u)
#define CY_CRYPTO_VU_MOV_REG_OPC             (0x02u)
#define CY_CRYPTO_VU_LD_REG_OPC              (0x00u)
#define CY_CRYPTO_VU_ST_REG_OPC              (0x01u)

/* Instructions with register operand only, category III */
#define CY_CRYPTO_VU_SWAP_REG_OPC            (0x03u)

/* Instructions with register operand only, category IV */
#define CY_CRYPTO_VU_MOV_REG_TO_STATUS_OPC   (0x04u)

/* Instructions with register operand only, category V */
#define CY_CRYPTO_VU_MOV_STATUS_TO_REG_OPC   (0x05u)

/* Instructions with register operand only, category VI */
#define CY_CRYPTO_VU_PUSH_REG_OPC            (0x10u)
#define CY_CRYPTO_VU_POP_REG_OPC             (0x11u)

/* Instructions with register operand only, category VII */
#define CY_CRYPTO_VU_ADD_REG_OPC             (0x06u)
#define CY_CRYPTO_VU_SUB_REG_OPC             (0x07u)
#define CY_CRYPTO_VU_OR_REG_OPC              (0x08u)
#define CY_CRYPTO_VU_AND_REG_OPC             (0x09u)
#define CY_CRYPTO_VU_XOR_REG_OPC             (0x0Au)
#define CY_CRYPTO_VU_NOR_REG_OPC             (0x0Bu)
#define CY_CRYPTO_VU_NAND_REG_OPC            (0x0Cu)
#define CY_CRYPTO_VU_MIN_REG_OPC             (0x0Du)
#define CY_CRYPTO_VU_MAX_REG_OPC             (0x0Eu)

/* Instructions with mixed operands, category I */
#define CY_CRYPTO_VU_LSL_OPC                 (0x20u)
#define CY_CRYPTO_VU_LSR_OPC                 (0x24u)

/* Instructions with mixed operands, category II */
#define CY_CRYPTO_VU_LSL1_OPC                (0x21u)
#define CY_CRYPTO_VU_LSL1_WITH_CARRY_OPC     (0x22u)
#define CY_CRYPTO_VU_LSR1_OPC                (0x25u)
#define CY_CRYPTO_VU_LSR1_WITH_CARRY_OPC     (0x26u)

/* Instructions with mixed operands, category III */
#define CY_CRYPTO_VU_SET_BIT_OPC             (0x2Cu)
#define CY_CRYPTO_VU_CLR_BIT_OPC             (0x2Du)
#define CY_CRYPTO_VU_INV_BIT_OPC             (0x2Eu)

/* Instructions with mixed operands, category IV */
#define CY_CRYPTO_VU_GET_BIT_OPC             (0x2Fu)

/* Instructions with mixed operands, category V */
#define CY_CRYPTO_VU_CLSAME_OPC              (0x28u)
#define CY_CRYPTO_VU_CTSAME_OPC              (0x29u)

/* Instructions with memory buffer operands, category I */
#define CY_CRYPTO_VU_SET_TO_ZERO_OPC         (0x34u)
#define CY_CRYPTO_VU_SET_TO_ONE_OPC          (0x35u)

/* Instructions with memory buffer operands, category II */
#define CY_CRYPTO_VU_MOV_OPC                 (0x30u)
#define CY_CRYPTO_VU_XSQUARE_OPC             (0x31u)

/* Instructions with memory buffer operands, category III */
#define CY_CRYPTO_VU_CMP_SUB_OPC             (0x3Du)
#define CY_CRYPTO_VU_CMP_DEFREE_OPC          (0x3Eu)

/* Instructions with memory buffer operands, category IV */
#define CY_CRYPTO_VU_TST_OPC                 (0x3fu)
#define CY_CRYPTO_VU_XMUL_OPC                (0x32u)
#define CY_CRYPTO_VU_UMUL_OPC                (0x33u)
#define CY_CRYPTO_VU_ADD_OPC                 (0x36u)
#define CY_CRYPTO_VU_SUB_OPC                 (0x37u)
#define CY_CRYPTO_VU_OR_OPC                  (0x38u)
#define CY_CRYPTO_VU_AND_OPC                 (0x39u)
#define CY_CRYPTO_VU_XOR_OPC                 (0x3Au)
#define CY_CRYPTO_VU_NOR_OPC                 (0x3Bu)
#define CY_CRYPTO_VU_NAND_OPC                (0x3Cu)


#endif /* #if (CPUSS_CRYPTO_PRESENT == 1) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_INSTRUCTIONS_H) */


/* [] END OF FILE */
