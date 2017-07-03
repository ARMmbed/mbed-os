/***************************************************************************//**
* \file cy_crypto_core_aes.c
* \version 1.0
*
* \brief
*  This file provides the source code fro the API for the AES method
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
#include "crypto/cy_crypto_core_aes.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_str.h"
#include "syslib/cy_syslib.h"
#include <string.h>

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_InvKey
****************************************************************************//**
*
* Calculates an inverse block cipher key from the block cipher key.
*
* This function available for CM0+ core only.
*
* \param cryptoAesContext
* The pointer to the stc_crypto_context_t structure which stores all internal variables
* the Crypto driver requires.
*
*******************************************************************************/
void Cy_Crypto_Core_Aes_InvKey(cy_stc_crypto_aes_state_t const *aesStatePtr)
{
    /* Issue the AES_KEY instruction to prepare the key for decrypt operation */
    Cy_Crypto_SetReg2Instr((uint32_t)aesStatePtr->keyPtr, (uint32_t)aesStatePtr->invKey);

    Cy_Crypto_Run2ParamInstr(CY_CRYPTO_AES_KEY_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT);

    /* Wait until the AES instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_AES_BUSY, CRYPTO->STATUS))
    {
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Init
****************************************************************************//**
*
* Sets Aes mode and prepare inversed key.
*
* This function available for CM0+ core only.
*
* \param cryptoAesContext
* The pointer to the stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Init(cy_stc_crypto_context_aes_t const *cryptoAesContext)
{
    /* Set the key mode: 128, 192 or 256 Bit */
    CRYPTO->AES_CTL = (uint32_t)(_VAL2FLD(CRYPTO_AES_CTL_KEY_SIZE,
                      (uint32_t)(cryptoAesContext->aesState.keyLength)));

    Cy_Crypto_Core_Aes_InvKey(&(cryptoAesContext->aesState));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_ProcessBlock
****************************************************************************//**
*
* Performs the AES block cipher.
*
* This function available for CM0+ core only.
*
* \param aesStatePtr
* The pointer to the aesStatePtr structure which stores the AES context.
*
* \param dirMode
* One of CRYPTO_ENCRYPT or CRYPTO_DECRYPT.
*
* \param dstBlockPtr
* The pointer to the cipher text.
*
* \param srcBlockPtr
* The pointer to the plain text. Must be 4-Byte aligned!
*
*******************************************************************************/
void Cy_Crypto_Core_Aes_ProcessBlock(cy_stc_crypto_aes_state_t const *aesStatePtr,
                                       cy_en_crypto_dir_mode_t dirMode,
                                       uint32_t *dstBlockPtr,
                                       uint32_t const *srcBlockPtr)
{
    Cy_Crypto_SetReg3Instr((CY_CRYPTO_DECRYPT == dirMode) ? (uint32_t)aesStatePtr->invKey : (uint32_t)aesStatePtr->keyPtr,
                           (uint32_t)srcBlockPtr,
                           (uint32_t)dstBlockPtr);

    Cy_Crypto_Run3ParamInstr((CY_CRYPTO_DECRYPT == dirMode) ? CY_CRYPTO_AES_BLOCK_INV_OPC : CY_CRYPTO_AES_BLOCK_OPC,
                              CY_CRYPTO_RSRC0_SHIFT,
                              CY_CRYPTO_RSRC4_SHIFT,
                              CY_CRYPTO_RSRC12_SHIFT);

    /* Wait until the AES instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_AES_BUSY, CRYPTO->STATUS))
    {
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Xor
****************************************************************************//**
*
* Perform the XOR of two 16-Byte memory structures.
* All addresses must be 4-Byte aligned!
*
* This function available for CM0+ core only.
*
* \param src0BlockPtr
* The pointer to the first memory structure. Must be 4-Byte aligned!
*
* \param src1BlockPtr
* The pointer to the second memory structure. Must be 4-Byte aligned!
*
* \param dstBlockPtr
* The pointer to the memory structure with the XOR results.
*
*******************************************************************************/
void Cy_Crypto_Core_Aes_Xor(uint32_t const *src0BlockPtr,
                              uint32_t const *src1BlockPtr,
                              uint32_t *dstBlockPtr)
{
    Cy_Crypto_SetReg3Instr((uint32_t)src0BlockPtr,
                           (uint32_t)src1BlockPtr,
                           (uint32_t)dstBlockPtr);

    /* Issue the AES_XOR instruction */
    Cy_Crypto_Run3ParamInstr(CY_CRYPTO_AES_XOR_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC4_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT);

    /* Wait until the AES instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_AES_BUSY, CRYPTO->STATUS))
    {
    }
}

#if (CY_CRYPTO_USER_AES_ECB == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Ecb
****************************************************************************//**
*
* Performs AES operation on one Block.
*
* This function available for CM0+ core only.
*
* \param cryptoAesContext
* The pointer to the stc_crypto_context_t structure which stores
* Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Ecb(cy_stc_crypto_context_aes_t *cryptoAesContext)
{
    cy_stc_crypto_aes_state_t aesStateTmp = cryptoAesContext->aesState;
    cy_en_crypto_dir_mode_t dirModeTmp = (cy_en_crypto_dir_mode_t)(cryptoAesContext->dirMode);

    Cy_Crypto_Core_Aes_ProcessBlock(&aesStateTmp,
                                      dirModeTmp,
                                      cryptoAesContext->dstPtr,
                                      cryptoAesContext->srcPtr);

    return (CY_CRYPTO_SUCCESS);
}
#endif /* #if (CY_CRYPTO_USER_AES_ECB == 1) */

#if (CY_CRYPTO_USER_AES_CBC == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Cbc
****************************************************************************//**
*
* Performs AES operation on a plain text with Cipher Block Chaining (CBC).
*
* This function available for CM0+ core only.
*
* \param cryptoAesContext
* The pointer to stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Cbc(cy_stc_crypto_context_aes_t *cryptoAesContext)
{
    uint32_t size = (uint32_t)cryptoAesContext->srcSize;
    uint32_t *dstPtrTmp = (uint32_t*)cryptoAesContext->dstPtr;
    uint32_t *srcPtrTmp = (uint32_t*)cryptoAesContext->srcPtr;
    cy_en_crypto_dir_mode_t dirModeTmp = cryptoAesContext->dirMode;
    uint32_t *tempBuffPtr = (uint32_t*)CRYPTO->MEM_BUFF;
    cy_en_crypto_status_t result;

    /* Check whether the data size is multiple of 16 */
    if (0uL != (uint32_t)(size & (CY_CRYPTO_AES_BLOCK_SIZE - 1u)))
    {
        result = CY_CRYPTO_SIZE_NOT_X16;
    }
    else
    {
        /* Copy the Initialization Vector to the local buffer because it changes during calculation */
        memcpy(tempBuffPtr, cryptoAesContext->ivPtr, CY_CRYPTO_AES_BLOCK_SIZE);

        if (CY_CRYPTO_DECRYPT == dirModeTmp)
        {
            while (size > 0uL)
            {
                Cy_Crypto_Core_Aes_ProcessBlock(&(cryptoAesContext->aesState),
                                                  dirModeTmp,
                                                  dstPtrTmp,
                                                  srcPtrTmp);

                Cy_Crypto_Core_Aes_Xor(tempBuffPtr, dstPtrTmp, dstPtrTmp);

                memcpy(tempBuffPtr, srcPtrTmp, CY_CRYPTO_AES_BLOCK_SIZE);

                srcPtrTmp += CY_CRYPTO_AES_BLOCK_SIZE_U32;
                dstPtrTmp += CY_CRYPTO_AES_BLOCK_SIZE_U32;
                size -= CY_CRYPTO_AES_BLOCK_SIZE;
            }
        }
        else
        {
            while (size > 0uL)
            {
                Cy_Crypto_Core_Aes_Xor(srcPtrTmp, tempBuffPtr, tempBuffPtr);

                Cy_Crypto_Core_Aes_ProcessBlock(&(cryptoAesContext->aesState),
                                                  dirModeTmp,
                                                  dstPtrTmp,
                                                  tempBuffPtr);

                memcpy(tempBuffPtr, dstPtrTmp, CY_CRYPTO_AES_BLOCK_SIZE);

                srcPtrTmp += CY_CRYPTO_AES_BLOCK_SIZE_U32;
                dstPtrTmp += CY_CRYPTO_AES_BLOCK_SIZE_U32;
                size -= CY_CRYPTO_AES_BLOCK_SIZE;
            }
        }

        result = CY_CRYPTO_SUCCESS;
    }

    return (result);
}
#endif /* #if (CY_CRYPTO_USER_AES_CBC == 1) */

#if (CY_CRYPTO_USER_AES_CFB == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Cfb
********************************************************************************
*
* Performs AES operation on a plain text with the Cipher Feedback Block method (CFB).
*
* This function available for CM0+ core only.
*
* \param cryptoAesContext
* The pointer to the stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Cfb(cy_stc_crypto_context_aes_t *cryptoAesContext)
{
    uint32_t size = (uint32_t)cryptoAesContext->srcSize;
    uint32_t *tempIvPtr = (uint32_t*)CRYPTO->MEM_BUFF;
    uint32_t *dstPtrTmp = (uint32_t*)cryptoAesContext->dstPtr;
    uint32_t *srcPtrTmp = (uint32_t*)cryptoAesContext->srcPtr;
    uint32_t *tempBuffPtr = (uint32_t*)dstPtrTmp;
    cy_en_crypto_status_t result;

    /* Check whether the data size is multiple of 16 */
    if (0uL != (size & (CY_CRYPTO_AES_BLOCK_SIZE - 1u)))
    {
        result = CY_CRYPTO_SIZE_NOT_X16;
    }
    else
    {
        /* Copy the Initialization Vector to local buffer because it is changing during calculation */
        memcpy(tempIvPtr, cryptoAesContext->ivPtr, CY_CRYPTO_AES_BLOCK_SIZE);

        if (CY_CRYPTO_DECRYPT == cryptoAesContext->dirMode)
        {
            tempBuffPtr = srcPtrTmp;
        }

        while (size > 0uL)
        {
            /* In this mode, (CFB) is always an encryption! */
            Cy_Crypto_Core_Aes_ProcessBlock(&(cryptoAesContext->aesState),
                                              CY_CRYPTO_ENCRYPT,
                                              dstPtrTmp,
                                              tempIvPtr);

            Cy_Crypto_Core_Aes_Xor(srcPtrTmp, dstPtrTmp, dstPtrTmp);

            memcpy(tempIvPtr, tempBuffPtr, CY_CRYPTO_AES_BLOCK_SIZE);

            srcPtrTmp   += CY_CRYPTO_AES_BLOCK_SIZE_U32;
            dstPtrTmp   += CY_CRYPTO_AES_BLOCK_SIZE_U32;
            tempBuffPtr += CY_CRYPTO_AES_BLOCK_SIZE_U32;
            size -= CY_CRYPTO_AES_BLOCK_SIZE;
        }

        result = CY_CRYPTO_SUCCESS;
    }

    return (result);
}
#endif /* #if (CY_CRYPTO_USER_AES_CFB == 1) */

#if (CY_CRYPTO_USER_AES_CTR == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Core_Aes_Ctr
********************************************************************************
*
* Performs AES operation on a plain text using the counter method (CTR).
*
* This function available for CM0+ core only.
*
* \param cryptoAesContext
* The pointer to the stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Ctr(cy_stc_crypto_context_aes_t *cryptoAesContext)
{
    uint32_t *dstPtrTmp = (uint32_t*)cryptoAesContext->dstPtr;
    uint32_t *srcPtrTmp = (uint32_t*)cryptoAesContext->srcPtr;
    uint32_t size = (uint32_t)cryptoAesContext->srcSize;
    uint32_t *streamBlockTmp = (uint32_t*)cryptoAesContext->streamBlock;
    uint32_t *nonceCounter = (uint32_t*)cryptoAesContext->ivPtr;
    uint32_t cnt;
    uint32_t i;
    uint64_t counter;

    cnt = (uint32_t)(size / CY_CRYPTO_AES_BLOCK_SIZE);

    for (i = 0uL; i < cnt; i++)
    {
        /* In this mode, (CTR) is always an encryption! */
        Cy_Crypto_Core_Aes_ProcessBlock(&(cryptoAesContext->aesState),
                                          CY_CRYPTO_ENCRYPT,
                                          streamBlockTmp,
                                          nonceCounter);

        /* Increment the nonce counter, at least 64Bits (from 128) is the counter part */
        counter = *(uint64_t*)(nonceCounter + CY_CRYPTO_AES_CTR_CNT_POS);
        counter = Cy_Crypto_InvertEndian8(counter);
        counter++;
        counter = Cy_Crypto_InvertEndian8(counter);
        *(uint64_t*)(nonceCounter + CY_CRYPTO_AES_CTR_CNT_POS) = counter;

        Cy_Crypto_Core_Aes_Xor(srcPtrTmp,
                                 streamBlockTmp,
                                 dstPtrTmp);

        srcPtrTmp += CY_CRYPTO_AES_BLOCK_SIZE_U32;
        dstPtrTmp += CY_CRYPTO_AES_BLOCK_SIZE_U32;
    }

    /* Save the reminder of the last non-complete block */
    *cryptoAesContext->srcOffset = (uint32_t)(size % CY_CRYPTO_AES_BLOCK_SIZE);

    return (CY_CRYPTO_SUCCESS);
}
#endif /* #if (CY_CRYPTO_USER_AES_CTR == 1) */

#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
