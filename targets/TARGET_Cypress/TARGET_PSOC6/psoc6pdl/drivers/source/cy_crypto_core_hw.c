/***************************************************************************//**
* \file cy_crypto_core_hw.c
* \version 2.20
*
* \brief
*  This file provides the source code to the API for the utils
*  in the Crypto driver.
*
********************************************************************************
* Copyright 2016-2019 Cypress Semiconductor Corporation
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


#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#include "cy_crypto_core_hw.h"
#include "cy_crypto_core_hw_vu.h"
#include "cy_syslib.h"
#include <stdbool.h>

#if !defined(CY_CRYPTO_SERVICE_LIBRARY_LEVEL)
    #define CY_CRYPTO_SERVICE_LIBRARY_LEVEL CY_CRYPTO_FULL_LIBRARY
#endif

/*******************************************************************************
*                   Global Variables
*******************************************************************************/

/* This is set in Cy_Crypto_Core_Enable() to the device information relevant
 * for the current target.
 */
const cy_stc_cryptoIP_t * cy_cryptoIP = NULL;

/* Platform and peripheral crypto block configuration */
const cy_stc_cryptoIP_t cy_cryptoIpBlockCfgPSoC6_01 =
{
    /* CRYPTO register offsets */
    /* cryptoStatusOffset        */ offsetof(CRYPTO_V1_Type, STATUS),
    /* cryptoIstrFfCtlOffset     */ offsetof(CRYPTO_V1_Type, INSTR_FF_CTL),
    /* cryptoInstrFfStatusOffset */ offsetof(CRYPTO_V1_Type, INSTR_FF_STATUS),
    /* cryptoInstrFfWrOffset     */ offsetof(CRYPTO_V1_Type, INSTR_FF_WR),
    /* cryptoVuRfDataOffset      */ offsetof(CRYPTO_V1_Type, RF_DATA),
    /* cryptoAesCtlOffset        */ offsetof(CRYPTO_V1_Type, AES_CTL),
    /* cryptoPrResultOffset      */ offsetof(CRYPTO_V1_Type, PR_RESULT),
    /* cryptoTrResultOffset      */ offsetof(CRYPTO_V1_Type, TR_RESULT),
    /* cryptoCrcCtlOffset        */ offsetof(CRYPTO_V1_Type, CRC_CTL),
    /* cryptoCrcDataCtlOffset    */ offsetof(CRYPTO_V1_Type, CRC_DATA_CTL),
    /* cryptoCrcPolCtlOffset     */ offsetof(CRYPTO_V1_Type, CRC_POL_CTL),
    /* cryptoCrcRemCtlOffset     */ offsetof(CRYPTO_V1_Type, CRC_REM_CTL),
    /* cryptoCrcRemResultOffset  */ offsetof(CRYPTO_V1_Type, CRC_REM_RESULT),
    /* cryptoVuCtl0Offset        */ offsetof(CRYPTO_V1_Type, VU_CTL0),
    /* cryptoVuCtl1Offset        */ offsetof(CRYPTO_V1_Type, VU_CTL1),
    /* cryptoVuStatusOffset      */ offsetof(CRYPTO_V1_Type, VU_STATUS),
    /* cryptoIntrOffset          */ offsetof(CRYPTO_V1_Type, INTR),
    /* cryptoIntrSetOffset       */ offsetof(CRYPTO_V1_Type, INTR_SET),
    /* cryptoIntrMaskOffset      */ offsetof(CRYPTO_V1_Type, INTR_MASK),
    /* cryptoIntrMaskedOffset    */ offsetof(CRYPTO_V1_Type, INTR_MASKED),
    /* cryptoMemBufOffset        */ offsetof(CRYPTO_V1_Type, MEM_BUFF),
};

const cy_stc_cryptoIP_t cy_cryptoIpBlockCfgPSoC6_02 =
{
    /* CRYPTO register offsets */
    /* cryptoStatusOffset        */ offsetof(CRYPTO_V2_Type, STATUS),
    /* cryptoIstrFfCtlOffset     */ offsetof(CRYPTO_V2_Type, INSTR_FF_CTL),
    /* cryptoInstrFfStatusOffset */ offsetof(CRYPTO_V2_Type, INSTR_FF_STATUS),
    /* cryptoInstrFfWrOffset     */ offsetof(CRYPTO_V2_Type, INSTR_FF_WR),
    /* cryptoVuRfDataOffset      */ offsetof(CRYPTO_V2_Type, VU_RF_DATA),
    /* cryptoAesCtlOffset        */ offsetof(CRYPTO_V2_Type, AES_CTL),
    /* cryptoPrResultOffset      */ offsetof(CRYPTO_V2_Type, PR_RESULT),
    /* cryptoTrResultOffset      */ offsetof(CRYPTO_V2_Type, TR_RESULT),
    /* cryptoCrcCtlOffset        */ offsetof(CRYPTO_V2_Type, CRC_CTL),
    /* cryptoCrcDataCtlOffset    */ offsetof(CRYPTO_V2_Type, CRC_DATA_CTL),
    /* cryptoCrcPolCtlOffset     */ offsetof(CRYPTO_V2_Type, CRC_POL_CTL),
    /* cryptoCrcRemCtlOffset     */ offsetof(CRYPTO_V2_Type, CRC_REM_CTL),
    /* cryptoCrcRemResultOffset  */ offsetof(CRYPTO_V2_Type, CRC_REM_RESULT),
    /* cryptoVuCtl0Offset        */ offsetof(CRYPTO_V2_Type, VU_CTL0),
    /* cryptoVuCtl1Offset        */ offsetof(CRYPTO_V2_Type, VU_CTL1),
    /* cryptoVuStatusOffset      */ offsetof(CRYPTO_V2_Type, VU_STATUS),
    /* cryptoIntrOffset          */ offsetof(CRYPTO_V2_Type, INTR),
    /* cryptoIntrSetOffset       */ offsetof(CRYPTO_V2_Type, INTR_SET),
    /* cryptoIntrMaskOffset      */ offsetof(CRYPTO_V2_Type, INTR_MASK),
    /* cryptoIntrMaskedOffset    */ offsetof(CRYPTO_V2_Type, INTR_MASKED),
    /* cryptoMemBufOffset        */ offsetof(CRYPTO_V2_Type, MEM_BUFF),
};

/* The defines of the power modes of the CRYPTO */
#define CY_CRYPTO_PWR_MODE_OFF               (0UL)
#define CY_CRYPTO_PWR_MODE_RETAINED          (2UL)
#define CY_CRYPTO_PWR_MODE_ENABLED           (3UL)


/**
* \addtogroup group_crypto_lld_hw_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_Crypto_Core_ClearVuRegisters
****************************************************************************//**
*
* The function to initialize the Crypto VU registers.
*
* \param base
* The pointer to the CRYPTO instance.
*
*******************************************************************************/
void Cy_Crypto_Core_ClearVuRegisters(CRYPTO_Type *base)
{
    /* Clear whole register file */
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG14, 0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG13, 0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG12, 0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG11, 0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG10, 0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG9,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG8,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG7,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG6,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG5,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG4,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG3,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG2,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG1,  0u, 1u);
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG0,  0u, 1u);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_HwInit
****************************************************************************//**
*
* The function to initialize the Crypto hardware.
*
*******************************************************************************/
void Cy_Crypto_Core_HwInit(void)
{
    cy_cryptoIP = (cy_device->cryptoVersion == 1u) ? &cy_cryptoIpBlockCfgPSoC6_01 : &cy_cryptoIpBlockCfgPSoC6_02;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Enable
****************************************************************************//**
*
* The function to enable the Crypto hardware.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \return
* Crypto status \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Enable(CRYPTO_Type *base)
{
    Cy_Crypto_Core_HwInit();

    if (CY_CRYPTO_HW_V1)
    {
        /* Enable Crypto HW */
        REG_CRYPTO_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_CTL_PWR_MODE, CY_CRYPTO_PWR_MODE_ENABLED) |
                               _VAL2FLD(CRYPTO_CTL_ENABLED,  1uL));
    }
    else
    {
        REG_CRYPTO_CTL(base) &= ~(_VAL2FLD(CRYPTO_V2_CTL_ENABLED,  1uL) | _VAL2FLD(CRYPTO_V2_CTL_ECC_EN, 1uL));

        REG_CRYPTO_INSTR_FF_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_INSTR_FF_CTL_BLOCK, 1u)
                                              | _VAL2FLD(CRYPTO_V2_INSTR_FF_CTL_CLEAR, 0u)
                                              | _VAL2FLD(CRYPTO_V2_INSTR_FF_CTL_EVENT_LEVEL, 0u));

        REG_CRYPTO_CTL(base) |= _VAL2FLD(CRYPTO_V2_CTL_ENABLED,  1uL);

        REG_CRYPTO_RAM_PWR_CTL(base) = (uint32_t)(CY_CRYPTO_PWR_MODE_ENABLED);

        /*
        Specifies the size of  the vector operand memory region.
        Legal values:
        "0b0000000":  32 KB memory region (VU_VTL1.ADDR[14:8] ignored).
        "0b1000000":  16 KB memory region (VU_VTL1.ADDR[13:8] ignored).
        "0b1100000":   8 KB memory region (VU_VTL1.ADDR[12:8] ignored).
        "0b1110000":   4 KB memory region (VU_VTL1.ADDR[11:8] ignored).
        "0b1111000":   2 KB memory region (VU_VTL1.ADDR[10:8] ignored).
        "0b1111100":   1 KB memory region (VU_VTL1.ADDR[9:8]  ignored).
        "0b1111110": 512  B memory region (VU_VTL1.ADDR[8]    ignored).
        "0b1111111": 256  B memory region.

        Used value: "0b1000000": 16 KB memory region
        */
        REG_CRYPTO_VU_CTL2(base) = _VAL2FLD(CRYPTO_V2_VU_CTL2_MASK, 0x40u);
    }

    /*
    Specifies if a conditional instruction is executed or not, when its condition
    code evaluates to false("0"):
      "0": The instruction is NOT executed. As a result, the instruction may be
           handled faster than when it is executed.
      "1": The instruction is executed, but the execution result (including
           status field information) is not reflected in the IP. The instruction
           is handled just as fast as when it is executed.
    */
    REG_CRYPTO_VU_CTL0(base) = (uint32_t)1u;

    REG_CRYPTO_VU_CTL1(base) = (uint32_t)REG_CRYPTO_MEM_BUFF(base);

    /* Set the stack pointer to the Crypto buff size, in words */
    CY_CRYPTO_VU_SET_REG(base, CY_CRYPTO_VU_HW_REG15, cy_device->cryptoMemSize, 1u);

    /* Clear whole register file */
    Cy_Crypto_Core_ClearVuRegisters(base);

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_GetLibInfo
****************************************************************************//**
*
* Get Crypto service information
*
* \param libInfo
* The pointer to a variable to store gathered crypto library information.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_GetLibInfo(cy_en_crypto_lib_info_t *libInfo)
{
    *libInfo = (cy_en_crypto_lib_info_t)CY_CRYPTO_SERVICE_LIBRARY_LEVEL;

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Disable
****************************************************************************//**
*
* Disables the operation of the CRYPTO block.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Disable(CRYPTO_Type *base)
{
    if (CY_CRYPTO_HW_V1)
    {
        /* Disable Crypto HW */
        REG_CRYPTO_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_CTL_PWR_MODE, CY_CRYPTO_PWR_MODE_OFF) |
                               _VAL2FLD(CRYPTO_CTL_ENABLED, 0uL));
    }
    else
    {
        REG_CRYPTO_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_V2_CTL_ENABLED,  0uL));
        REG_CRYPTO_RAM_PWR_CTL(base) = (uint32_t)(CY_CRYPTO_PWR_MODE_OFF);
    }

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_InvertEndianness
****************************************************************************//**
*
* This function reverts byte-array memory block, like:<br>
* inArr[0] <---> inArr[n]<br>
* inArr[1] <---> inArr[n-1]<br>
* inArr[2] <---> inArr[n-2]<br>
* ........................<br>
* inArr[n/2] <---> inArr[n/2-1]<br>
*
* Odd or even byteSize are acceptable.
*
* \param inArrPtr
* The pointer to the memory whose endianness is to be inverted.
*
* \param byteSize
* The length of the memory array whose endianness is to be inverted (in bytes)
*
*******************************************************************************/
void Cy_Crypto_Core_InvertEndianness(void *inArrPtr, uint32_t byteSize)
{
    int32_t limit;
    int32_t i;
    int32_t j = 0;
    uint8_t temp;
    uint8_t *tempPtr = (uint8_t*)inArrPtr;

    if (byteSize > 1u)
    {
        limit = (int32_t)byteSize / 2;
        if (0u == (byteSize % 2u))
        {
            --limit;
        }

        j = 0;
        i = (int32_t)byteSize - 1;
        while ( i > limit)
        {
            temp = tempPtr[j];
            tempPtr[j] = tempPtr[i];
            tempPtr[i] = temp;

            --i;
            ++j;
        }
    }
}

/** \} group_crypto_lld_hw_functions */

#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */

