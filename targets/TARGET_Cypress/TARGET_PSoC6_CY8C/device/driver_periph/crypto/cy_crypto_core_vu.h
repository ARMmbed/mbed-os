/***************************************************************************//**
* \file cy_crypto_core_vu.h
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

#if !defined(CY_CRYPTO_CORE_VU_H)
#define CY_CRYPTO_CORE_VU_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))


/* Function Prototypes */
void Cy_Crypto_Core_Vu_WaitForComplete(void);
uint16_t Cy_Crypto_Core_Vu_StatusRead(void);

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Vu_RegRead
****************************************************************************//**
*
* Returns size of the data pointed in given register,
* it is lower 14Bit of the 32Bit word
*
* \param srcReg
*
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_Crypto_Core_Vu_RegRead(uint8_t srcReg)
{
    return ((uint32_t)_FLD2VAL(CRYPTO_RF_DATA_DATA32, CRYPTO->RF_DATA[srcReg]));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Vu_RegSizeRead
****************************************************************************//**
*
* Returns size of the data pointed in given register,
* it is lower 14Bit of the 32Bit word
*
* \param srcReg
*
*
*******************************************************************************/
__STATIC_INLINE uint16_t Cy_Crypto_Core_Vu_RegSizeRead(uint8_t srcReg)
{
    return ((uint16_t)(_FLD2VAL(CRYPTO_RF_DATA_DATA32, CRYPTO->RF_DATA[srcReg]) & 0x00000fff));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Vu_RegBitSizeRead
****************************************************************************//**
*
* Returns size of the data pointed in given register, in Bits.
* It is lower 14Bit of the 32Bit word
*
* \param srcReg
*
*
*******************************************************************************/
__STATIC_INLINE uint16_t Cy_Crypto_Core_Vu_RegBitSizeRead(uint8_t srcReg)
{
    return ((uint16_t)(_FLD2VAL(CRYPTO_RF_DATA_DATA32, CRYPTO->RF_DATA[srcReg]) & 0x00000fff) + 1);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Vu_RegByteSizeRead
****************************************************************************//**
*
* Returns size of the data pointed in given register, in Bytes.
* It is lower 14Bit of the 32Bit word
*
* \param srcReg
*
*
*******************************************************************************/
__STATIC_INLINE uint16_t Cy_Crypto_Core_Vu_RegByteSizeRead(uint8_t srcReg)
{
    return ((uint16_t)(_FLD2VAL(CRYPTO_RF_DATA_DATA32, CRYPTO->RF_DATA[srcReg]) & 0x00000fff) >> 3);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Vu_RegWordRead
****************************************************************************//**
*
* Returns size of the data pointed in given register, in words (uint32_t).
* It is lower 14Bit of the 32Bit word
*
* \param srcReg
*
*
*******************************************************************************/
__STATIC_INLINE uint16_t Cy_Crypto_Core_Vu_RegWordSizeRead(uint8_t srcReg)
{
    return ((uint16_t)(_FLD2VAL(CRYPTO_RF_DATA_DATA32, CRYPTO->RF_DATA[srcReg]) & 0x00000fff) >> 5);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Vu_RegDataRead
****************************************************************************//**
*
* Returns the data pointer, in 14-bit format of the data pointed in given register,
* it is upper 16Bit of the 32Bir word. Pointer is in words (uint32_t).
*
* \param srcReg
*
*
*******************************************************************************/
__STATIC_INLINE uint16_t Cy_Crypto_Core_Vu_RegDataPtrRead(uint8_t srcReg)
{
    return ((uint16_t)(_FLD2VAL(CRYPTO_RF_DATA_DATA32, CRYPTO->RF_DATA[srcReg]) >> 16) & 0x00003fff);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#endif /* #if (CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_VU_H) */

/* [] END OF FILE */
