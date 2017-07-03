/***************************************************************************//**
* \file cy_crypto_core_str.c
* \version 1.0
*
* \brief
*  This file provides the source code for the string management API
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_crypto_common.h"
#include "cy_crypto_core_str.h"
#include "cy_crypto_core_instructions.h"
#include "cy_crypto_core_util.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1))


/*******************************************************************************
* Function Name: Cy_Crypto_Core_Str_MemCpy
****************************************************************************//**
*
* Function MemCpy uses Crypto HW.
* Memory structures should not overlap!
* There is no alignment restriction.
*
* This function available for CM0+ core only.
*
* \param src
* The pointer to the source of MemCpy.
*
* \param dst
* The pointer to the destination of MemCpy.
*
* \param size
* The size in bytes of the copy operation.
*
*******************************************************************************/
void Cy_Crypto_Core_Str_MemCpy(void* dstPtr, void const *srcPtr, uint16_t size)
{
    /* Prepare data in the register file for next instruction */
    Cy_Crypto_SetReg3Instr((uint32_t)srcPtr,
                           (uint32_t)dstPtr,
                           (uint32_t)size);

    /* Issue the STR_MEMCPY instruction */
    Cy_Crypto_Run3ParamInstr(CY_CRYPTO_STR_MEMCPY_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC4_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT);

    /* Wait until the STR instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_STR_BUSY, CRYPTO->STATUS))
    {
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Str_MemSet
****************************************************************************//**
*
* Function MemSet uses Crypto HW.
* There is no alignment restriction.
*
* This function available for CM0+ core only.
*
* \param dst
* The pointer to the destination of MemSet.

* \param data
* The value to be set.

* \param size
* The size in bytes of the set operation.
*
*******************************************************************************/
void Cy_Crypto_Core_Str_MemSet(void* dstPtr, uint8_t data, uint16_t size)
{
    Cy_Crypto_SetReg3Instr((uint32_t)dstPtr,
                           (uint32_t)size,
                           (uint32_t)data);

    /* Issue the STR_MEMSET instruction */
    Cy_Crypto_Run3ParamInstr(CY_CRYPTO_STR_MEMSET_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT,
                             CY_CRYPTO_RSRC12_SHIFT);

    /* Wait until the STR instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_STR_BUSY, CRYPTO->STATUS))
    {
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Str_MemCmp
****************************************************************************//**
*
* Function MemCmp uses Crypto HW.
* There is no alignment restriction.
*
* This function available for CM0+ core only.
*
* \param src0
* The pointer to the first source of MemCmp.

* \param src1
* The pointer to the second source of MemCmp.

* \param size
* the size in bytes of the compare operation.
*
* \return
* 0 - if Source 1 = Source 2, 1 - if not.
*
*******************************************************************************/
uint32_t Cy_Crypto_Core_Str_MemCmp(void const *src0Ptr, void const *src1Ptr, uint16_t size)
{
    Cy_Crypto_SetReg3Instr((uint32_t)src0Ptr,
                           (uint32_t)src1Ptr,
                           (uint32_t)size);

    /* Issue the STR_MEMCMP instruction */
    Cy_Crypto_Run3ParamInstr(CY_CRYPTO_STR_MEMCMP_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC4_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT);

    /* Wait until the STR instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_STR_BUSY, CRYPTO->STATUS))
    {
    }

    return((uint32_t)(CRYPTO->STR_RESULT));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Str_MemXor
****************************************************************************//**
*
* Function MemXor uses Crypto HW.
* Memory structures should not overlap!
* There is no alignment restriction.
*
* This function available for CM0+ core only.
*
* \param src0
* The pointer to the first source of MemXor.

* \param src1
* The pointer to the second source of MemXor.

* \param dest
* The pointer to the destination of MemXor.
*
* \param size
* The size in bytes of the compare operation.
*
*******************************************************************************/
void Cy_Crypto_Core_Str_MemXor(void const *src0Ptr, void const *src1Ptr, void* dstPtr, uint16_t size)
{
    Cy_Crypto_SetReg4Instr((uint32_t)src0Ptr,
                           (uint32_t)src1Ptr,
                           (uint32_t)size,
                           (uint32_t)dstPtr);

    /* Issue the STR_MEMXOR instruction */
    Cy_Crypto_Run4ParamInstr(CY_CRYPTO_STR_MEMXOR_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC4_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT,
                             CY_CRYPTO_RSRC12_SHIFT);

    /* Wait until the STR instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_STR_BUSY, CRYPTO->STATUS))
    {
    }
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
