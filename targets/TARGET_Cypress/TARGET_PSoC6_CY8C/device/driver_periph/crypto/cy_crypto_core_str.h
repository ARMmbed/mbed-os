/***************************************************************************//**
* \file cy_crypto_core_str.h
* \version 1.0
*
* \brief
*  This file provides the headers for the string management API
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_STR_H)
#define CY_CRYPTO_CORE_STR_H

#include "cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_STR == 1) & (CY_CRYPTO_USER_STR == 1))


void Cy_Crypto_Core_Str_MemCpy(void* dstPtr, void const *srcPtr, uint16_t size);
void Cy_Crypto_Core_Str_MemSet(void* dstPtr, uint8_t data, uint16_t size);
uint32_t Cy_Crypto_Core_Str_MemCmp(void const *src0Ptr, void const *src1Ptr, uint16_t size);
void Cy_Crypto_Core_Str_MemXor(void const *src0Ptr, void const *src1Ptr, void* dstPtr, uint16_t size);


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_STR == 1) & (CY_CRYPTO_USER_STR == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_STR_H) */



/* [] END OF FILE */
