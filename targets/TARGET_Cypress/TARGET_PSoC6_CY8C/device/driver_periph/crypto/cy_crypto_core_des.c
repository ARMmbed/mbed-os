/***************************************************************************//**
* \file cy_crypto_core_des.c
* \version 1.0
*
* \brief
*  This file provides the source code fro the API for the DES method
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_core_des.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_str.h"
#include "syslib/cy_syslib.h"
#include <string.h>


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1))


uint8_t const cy_desCommands[2][2] = {
    { CY_CRYPTO_DES_BLOCK_OPC,  CY_CRYPTO_DES_BLOCK_INV_OPC  },   /*  DES mode */
    { CY_CRYPTO_TDES_BLOCK_OPC, CY_CRYPTO_TDES_BLOCK_INV_OPC }    /* TDES mode */
};

/* Table with DES weak keys */
CY_ALIGN(4) uint8_t const cy_desWeakKeys[CY_CRYPTO_DES_WEAK_KEY_COUNT][CY_CRYPTO_DES_KEY_BYTE_LENGTH] =
{
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
    { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE },
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x0E },
    { 0xE0, 0xE0, 0xE0, 0xE0, 0xF1, 0xF1, 0xF1, 0xF1 },

    { 0x01, 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E },
    { 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E, 0x01 },
    { 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1 },
    { 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1, 0x01 },
    { 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE },
    { 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01 },
    { 0x1F, 0xE0, 0x1F, 0xE0, 0x0E, 0xF1, 0x0E, 0xF1 },
    { 0xE0, 0x1F, 0xE0, 0x1F, 0xF1, 0x0E, 0xF1, 0x0E },
    { 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E, 0xFE },
    { 0xFE, 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E },
    { 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1, 0xFE },
    { 0xFE, 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1 }
};


/*******************************************************************************
* Function Name: Cy_Crypto_Core_Des_ProcessBlock
****************************************************************************//**
*
* Performs the DES or TDES block cipher.
* All addresses must be 4Byte aligned,
* srcBlock could overlap dstBlock.
*
* This function available for CM0+ core only.
*
* \param dirMode
* One of CRYPTO_ENCRYPT or CRYPTO_DECRYPT.
*
* \param keyPtr
* The pointer to the encryption/decryption key.
*
* \param dstBlockPtr
* The pointer to the cipher text.
*
* \param srcBlockPtr
* The pointer to the plain text. Must be 4-Byte aligned!
*
*******************************************************************************/
void Cy_Crypto_Core_Des_ProcessBlock(cy_en_crypto_des_mode_t desMode,
                                       cy_en_crypto_dir_mode_t dirMode,
                                       uint32_t const *keyPtr,
                                       uint32_t *dstBlockPtr,
                                       uint32_t const *srcBlockPtr)
{
    Cy_Crypto_SetReg3Instr((uint32_t)keyPtr,
                           (uint32_t)srcBlockPtr,
                           (uint32_t)dstBlockPtr);

    /* Issue the DES_BLOCK instruction */
    Cy_Crypto_Run3ParamInstr(cy_desCommands[(uint8_t)desMode][(uint8_t)dirMode],
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC4_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT);

    /* Wait until the AES instruction is complete */
    while (0uL != _FLD2VAL(CRYPTO_STATUS_DES_BUSY, CRYPTO->STATUS))
    {
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Des
****************************************************************************//**
*
* Performs DES operation on a Single Block. All addresses must be 4-Byte aligned.
* Ciphertext (dstBlockPtr) may overlap with plaintext (srcBlockPtr)
* This function is independent from the previous Crypto state.
*
* This function available for CM0+ core only.
*
* \param cryptoDesContext
* The pointer to the cy_stc_crypto_context_des_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Des(cy_stc_crypto_context_des_t *cryptoDesContext)
{
    uint32_t i;
    cy_en_crypto_status_t status = CY_CRYPTO_SUCCESS;

    /* Check weak keys */
    for (i = 0; i < CY_CRYPTO_DES_WEAK_KEY_COUNT; i++)
    {
        if (memcmp(cryptoDesContext->keyPtr, (uint8_t*)cy_desWeakKeys[i], CY_CRYPTO_DES_KEY_BYTE_LENGTH) == 0)
        {
            status = CY_CRYPTO_DES_WEAK_KEY;
            break;
        }
    }

    Cy_Crypto_Core_Des_ProcessBlock(CY_CRYPTO_DES_MODE_SINGLE,
                                      cryptoDesContext->dirMode,
                                      cryptoDesContext->keyPtr,
                                      cryptoDesContext->dstPtr,
                                      cryptoDesContext->srcPtr);

    return (status);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Tdes
****************************************************************************//**
*
* Performs TDES operation on a Single Block. All addresses must be 4-Byte aligned.
* Ciphertext (dstBlockPtr) may overlap with plaintext (srcBlockPtr)
* This function is independent from the previous Crypto state.
*
* This function available for CM0+ core only.
*
* \param cryptoDesContext
* The pointer to the cy_stc_crypto_context_des_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Tdes(cy_stc_crypto_context_des_t *cryptoDesContext)
{
    uint32_t i;
    cy_en_crypto_status_t status = CY_CRYPTO_SUCCESS;
    uint32_t *_keyPtr = cryptoDesContext->keyPtr;

    /* Check weak keys */
    for (i = 0; i < CY_CRYPTO_DES_WEAK_KEY_COUNT; i++)
    {
        if (memcmp(_keyPtr, (uint8_t*)cy_desWeakKeys[i], CY_CRYPTO_DES_KEY_BYTE_LENGTH) == 0)
        {
            status = CY_CRYPTO_DES_WEAK_KEY;
            break;
        }

        if (memcmp(&(_keyPtr[CY_CRYPTO_DES_KEY_BYTE_LENGTH]),
                   (uint8_t*)cy_desWeakKeys[i], CY_CRYPTO_DES_KEY_BYTE_LENGTH) == 0)
        {
            status = CY_CRYPTO_DES_WEAK_KEY;
            break;
        }

        if (memcmp(&(_keyPtr[2 * CY_CRYPTO_DES_KEY_BYTE_LENGTH]),
                   (uint8_t*)cy_desWeakKeys[i], CY_CRYPTO_DES_KEY_BYTE_LENGTH) == 0)
        {
            status = CY_CRYPTO_DES_WEAK_KEY;
            break;
        }
    }

    Cy_Crypto_Core_Des_ProcessBlock(CY_CRYPTO_DES_MODE_TRIPLE,
                                      cryptoDesContext->dirMode,
                                      _keyPtr,
                                      cryptoDesContext->dstPtr,
                                      cryptoDesContext->srcPtr);

    return (status);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
