/***************************************************************************//**
* \file cy_crypto_core_vu.c
* \version 2.20
*
* \brief
*  This file provides the source code to the API for the Vector Unit helpers
*  in the Crypto driver.
*
********************************************************************************
* Copyright 2016-2018 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#include "cy_crypto_core_vu.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_VU == 1)

#include "cy_crypto_core_hw_v1.h"
#include "cy_crypto_core_hw_v2.h"
#include "cy_crypto_core_mem.h"
#include "cy_syslib.h"

#if !defined(CY_CRYPTO_SERVICE_LIBRARY_LEVEL)
    #define CY_CRYPTO_SERVICE_LIBRARY_LEVEL CY_CRYPTO_FULL_LIBRARY
#endif

void Cy_Crypto_Core_Vu_SetMemValue(CRYPTO_Type *base, uint32_t dstReg, uint8_t const *src, uint32_t size)
{
    uint32_t reg0_data = 0uL;
    uint32_t reg1_data = 0uL;

    Cy_Crypto_Core_Vu_WaitForComplete(base);

    if (CY_CRYPTO_HW_V1)
    {
        CY_CRYPTO_VU_SAVE_REG(base, CY_CRYPTO_VU_HW_REG0, &reg0_data);
        CY_CRYPTO_VU_SAVE_REG(base, CY_CRYPTO_VU_HW_REG1, &reg1_data);
    }

    /* Copy value to Crypto SRAM */
    uint16_t byteSize = (uint16_t)((size / 8u) & 0xFFFFu);
    if ((size % 8u) != 0u)
    {
        ++byteSize;
    }

    CY_ASSERT_L1(size <= Cy_Crypto_Core_Vu_RegBitSizeRead(base, dstReg));

    CY_ASSERT_L1( (((uint32_t)Cy_Crypto_Core_Vu_RegMemPointer(base, dstReg) + byteSize) - 1u) < ((uint32_t)REG_CRYPTO_MEM_BUFF(base) + CY_CRYPTO_MEM_BUFF_SIZE));
    Cy_Crypto_Core_MemCpy(base, (void*)Cy_Crypto_Core_Vu_RegMemPointer(base, dstReg), (const void*)src, byteSize);

    if (CY_CRYPTO_HW_V1)
    {
        CY_CRYPTO_VU_RESTORE_REG(base, CY_CRYPTO_VU_HW_REG0, reg0_data);
        CY_CRYPTO_VU_RESTORE_REG(base, CY_CRYPTO_VU_HW_REG1, reg1_data);
    }
}

void Cy_Crypto_Core_Vu_GetMemValue(CRYPTO_Type *base, uint8_t *dst, uint32_t srcReg, uint32_t size)
{
    uint32_t reg0_data = 0uL;
    uint32_t reg1_data = 0uL;

    Cy_Crypto_Core_Vu_WaitForComplete(base);

    if (CY_CRYPTO_HW_V1)
    {
        CY_CRYPTO_VU_SAVE_REG(base, CY_CRYPTO_VU_HW_REG0, &reg0_data);
        CY_CRYPTO_VU_SAVE_REG(base, CY_CRYPTO_VU_HW_REG1, &reg1_data);
    }

    /* Copy value from Crypto SRAM */
    uint16_t byteSize = (uint16_t)((size / 8u) & 0xFFFFu);
    if ((size % 8u) != 0u)
    {
        ++byteSize;
    }

    CY_ASSERT_L1(size <= Cy_Crypto_Core_Vu_RegBitSizeRead(base, srcReg));
    CY_ASSERT_L1((((uint32_t)Cy_Crypto_Core_Vu_RegMemPointer(base, srcReg) + byteSize) - 1u) < ((uint32_t)REG_CRYPTO_MEM_BUFF(base) + CY_CRYPTO_MEM_BUFF_SIZE) );

    Cy_Crypto_Core_MemCpy(base, (void*)dst, (void*)Cy_Crypto_Core_Vu_RegMemPointer(base, srcReg), byteSize);

    if (CY_CRYPTO_HW_V1)
    {
        CY_CRYPTO_VU_RESTORE_REG(base, CY_CRYPTO_VU_HW_REG0, reg0_data);
        CY_CRYPTO_VU_RESTORE_REG(base, CY_CRYPTO_VU_HW_REG1, reg1_data);
    }
}

cy_en_crypto_status_t Cy_Crypto_Core_Cleanup(CRYPTO_Type *base)
{
    /* Set the stack pointer to the Crypto buff size, in words */
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG15, CY_CRYPTO_MEM_BUFF_SIZE_U32, 1u);

    /* Clear whole register file */
    Cy_Crypto_Core_ClearVuRegisters(base);

    /* PRNG */
    REG_CRYPTO_PR_LFSR_CTL0(base) = 0u;
    REG_CRYPTO_PR_LFSR_CTL1(base) = 0u;
    REG_CRYPTO_PR_LFSR_CTL2(base) = 0u;
    REG_CRYPTO_PR_RESULT(base)    = 0u;

    /* TRNG */
    REG_CRYPTO_TR_CTL0(base)      = 0u;
    REG_CRYPTO_TR_CTL1(base)      = 0u;
    REG_CRYPTO_TR_RESULT(base)    = 0u;

    /* CRC */
    REG_CRYPTO_CRC_POL_CTL(base)  = 0u;
    REG_CRYPTO_CRC_REM_CTL(base)  = 0u;

    /* AES */
    REG_CRYPTO_AES_CTL(base)      = 0u;

    if (CY_CRYPTO_HW_V1)
    {
        REG_CRYPTO_CRC_LFSR_CTL(base) = 0u;
        REG_CRYPTO_SHA_CTL(base)  = 0u;
    }
    else
    {
        REG_CRYPTO_TR_CTL2(base)  = 0u;
        REG_CRYPTO_RESULT(base)   = 0u;

        Cy_Crypto_Core_V2_FFStop(base, CY_CRYPTO_V2_RB_FF_LOAD0);
        Cy_Crypto_Core_V2_FFStop(base, CY_CRYPTO_V2_RB_FF_LOAD1);
        Cy_Crypto_Core_V2_FFStop(base, CY_CRYPTO_V2_RB_FF_STORE);
        Cy_Crypto_Core_V2_RBClear(base);
    }

    Cy_Crypto_Core_MemSet(base, (void *)REG_CRYPTO_MEM_BUFF(base), 0u, (uint16_t)CY_CRYPTO_MEM_BUFF_SIZE);

    return (CY_CRYPTO_SUCCESS);
}

bool Cy_Crypto_Core_Vu_IsRegZero(CRYPTO_Type *base, uint32_t srcReg)
{
    bool tmpResult;
    uint32_t status;

    CY_CRYPTO_VU_TST(base, srcReg);
    status = Cy_Crypto_Core_Vu_StatusRead(base);

    if (0u != (status & CY_CRYPTO_VU_STATUS_ZERO_BIT))
    {
        tmpResult = true;
    }
    else
    {
        tmpResult = false;
    }

    return tmpResult;
}

bool Cy_Crypto_Core_Vu_IsRegEqual(CRYPTO_Type *base, uint32_t srcReg0, uint32_t srcReg1)
{
    bool tmpResult;
    uint32_t status;

    CY_CRYPTO_VU_CMP_SUB (base, srcReg1, srcReg0);                /* C = (a >= b) */
    status = Cy_Crypto_Core_Vu_StatusRead(base);

    if (0u != (status &  CY_CRYPTO_VU_STATUS_ZERO_BIT))
    {
        tmpResult = true;
    }
    else
    {
        tmpResult = false;
    }

    return tmpResult;
}

bool Cy_Crypto_Core_Vu_IsRegLess(CRYPTO_Type *base, uint32_t srcReg0, uint32_t srcReg1)
{
    bool tmpResult;
    uint32_t status;

    CY_CRYPTO_VU_CMP_SUB (base, srcReg1, srcReg0);                /* C = (a >= b) */
    status = Cy_Crypto_Core_Vu_StatusRead(base);

    if (0u != (status &  CY_CRYPTO_VU_STATUS_CARRY_BIT))
    {
        tmpResult = true;
    }
    else
    {
        tmpResult = false;
    }

    return tmpResult;
}


#endif /* #if (CPUSS_CRYPTO_VU == 1) */

#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */

