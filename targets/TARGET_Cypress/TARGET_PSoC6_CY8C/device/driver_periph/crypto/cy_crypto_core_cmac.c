/***************************************************************************//**
* \file cy_crypto_core_cmac.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for the CMAC method
*  in the Crypto block driver.
*
*  Implementation is done in accordance with:
*  http://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38b.pdf
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto_core_cmac.h"
#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_core_aes.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_str.h"
#include "syslib/cy_syslib.h"
#include <string.h>


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_CMAC == 1))


/*******************************************************************************
* Function Name: Cy_Crypto_Core_Cmac_CalcSubKey
****************************************************************************//**
*
* Calculates the sub-key for the CMAC algorithm
* according to the NIST publication 800-38B, page 7.
*
* This function available for CM0+ core only.
*
* \param srcDstPtr
* The pointer to the source data for sub-key calculation, see 800-38B.
*
*******************************************************************************/
static void Cy_Crypto_Core_Cmac_CalcSubKey(uint8_t *srcDstPtr)
{
    int32_t i;
    uint32_t c;
    uint32_t msb = 0uL;

    for (i = 15; i >= 0; i--)
    {
        c = (uint32_t)srcDstPtr[i];
        c = (c << 1) | msb;
        srcDstPtr[i] = (uint8_t) c;
        msb = (c >> 8u) & 1uL;
    }

    if (0uL != msb)
    {
        /* Just one byte is valuable, the rest are zeros */
        srcDstPtr[(uint8_t)(CY_CRYPTO_AES_BLOCK_SIZE - 1u)] ^= CY_CRYPTO_CMAC_RB;
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Cmac_Init
****************************************************************************//**
*
* The function for initialization of CMAC operation.
*
* This function available for CM0+ core only.
*
* \param cmacStatePtr
* The pointer to the structure which stores the CMAC context.
*
* \param tempPtr
* The pointer to the temporary memory needed for CMAC calculation,
* the max needed - 128 Bytes.
*
* \param blockPtr
* The pointer to the temporary storage for block, the max needed - 128 Bytes.
*
* \param kPtr
* The pointer to the sub-key.

*******************************************************************************/
void Cy_Crypto_Core_Cmac_Init(cy_stc_crypto_cmac_state_t* cmacStatePtr,
                                uint32_t* tempPtr,
                                uint32_t* blockPtr,
                                uint32_t* kPtr)
{
    cmacStatePtr->blockPtr = blockPtr;
    cmacStatePtr->tempPtr  = tempPtr;
    cmacStatePtr->kPtr     = kPtr;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Cmac_Start
****************************************************************************//**
*
* Starts CMAC calculation.
*
* This function available for CM0+ core only.
*
* \param aesStatePtr
* The pointer to the structure which stores the AES context.
*
* \param cmacStatePtr
* the pointer to the structure which stores the CMAC context.
*
*******************************************************************************/
void Cy_Crypto_Core_Cmac_Start(cy_stc_crypto_aes_state_t  *aesStatePtr,
                                 cy_stc_crypto_cmac_state_t *cmacStatePtr)
{
    uint32_t *kPtrTmp = cmacStatePtr->kPtr;
    uint32_t *tempPtrTmp = cmacStatePtr->tempPtr;

    /* Calculate the K1 sub-key */
    memset((void*)tempPtrTmp, 0u, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_Aes_ProcessBlock(aesStatePtr, CY_CRYPTO_ENCRYPT, kPtrTmp, tempPtrTmp);

    Cy_Crypto_Core_Cmac_CalcSubKey((uint8_t*)kPtrTmp);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Cmac_Update
****************************************************************************//**
*
* Calculates CMAC on a message.
*
* This function available for CM0+ core only.
*
* \param aesStatePtr
* The pointer to the structure which stores the AES context.
*
* \param cmacStatePtr
* The pointer to the structure which stores the CMAC context.
*
* \param messagePtr
* The pointer to the message whose CMAC is being computed.
*
* \param messageSize
* The size of the message whose CMAC is being computed.
*
*******************************************************************************/
void Cy_Crypto_Core_Cmac_Update(cy_stc_crypto_aes_state_t  *aesStatePtr,
                                  cy_stc_crypto_cmac_state_t *cmacStatePtr,
                                  uint32_t *messagePtr,
                                  uint32_t  messageSize)
{
    uint32_t *blockPtrTmp = cmacStatePtr->blockPtr;
    uint32_t *tempPtrTmp  = cmacStatePtr->tempPtr;

    /* Clear the argument for XOR for the first block */
    memset((void* )tempPtrTmp, 0x00u, CY_CRYPTO_AES_BLOCK_SIZE);

    /* Process all blocks except last */
    while (messageSize > CY_CRYPTO_AES_BLOCK_SIZE)
    {
       Cy_Crypto_Core_Aes_Xor(messagePtr, tempPtrTmp, blockPtrTmp);
       Cy_Crypto_Core_Aes_ProcessBlock(aesStatePtr, CY_CRYPTO_ENCRYPT, tempPtrTmp, blockPtrTmp);

       /* Attention! 4 because the pointer is uint32_t */
       messagePtr  += 4u;
       messageSize -= 16u;
    }

    /* The calculation size of the last block */
    aesStatePtr->blockIdx = messageSize;

    /* Copy the last block to the blockPtr */
    memcpy((void*)blockPtrTmp, (void*)messagePtr, (uint16_t)aesStatePtr->blockIdx);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Cmac_Finish
****************************************************************************//**
*
* Completes CMAC calculation.
*
* This function available for CM0+ core only.
*
* \param aesStatePtr
* the pointer to the structure which stores the AES context.
*
* \param cmacStatePtr
* The pointer to the structure which stores the CMAC context.
*
* \param cmacPtr
* The pointer to the computed CMAC value.
*
*******************************************************************************/
void Cy_Crypto_Core_Cmac_Finish(cy_stc_crypto_aes_state_t  *aesStatePtr,
                                  cy_stc_crypto_cmac_state_t *cmacStatePtr,
                                  uint32_t* cmacPtr)
{
    uint32_t *blockPtrTmp = cmacStatePtr->blockPtr;
    uint32_t *tempPtrTmp  = cmacStatePtr->tempPtr;
    uint32_t *kPtrTmp     = cmacStatePtr->kPtr;
    uint32_t blockIdxTmp  = aesStatePtr->blockIdx;
    uint32_t copySize;

    if (blockIdxTmp < CY_CRYPTO_AES_BLOCK_SIZE)
    {
        /* Calculate the K2 sub-key */
        Cy_Crypto_Core_Cmac_CalcSubKey((uint8_t* )kPtrTmp);

        /* Appended '1' bit to the end of message, followed by '0' */
        *((uint8_t* )blockPtrTmp + blockIdxTmp) = 0x80u;

        /* Write zeros into the rest of the message */
        copySize = CY_CRYPTO_AES_BLOCK_SIZE - 1u - blockIdxTmp;
        memset(((uint8_t* )blockPtrTmp + blockIdxTmp + 1), 0x00u, (uint16_t)copySize);
    }

    Cy_Crypto_Core_Aes_Xor(blockPtrTmp, tempPtrTmp, blockPtrTmp);
    Cy_Crypto_Core_Aes_Xor(blockPtrTmp, kPtrTmp, blockPtrTmp);

    Cy_Crypto_Core_Aes_ProcessBlock(aesStatePtr, CY_CRYPTO_ENCRYPT, cmacPtr, blockPtrTmp);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Cmac
****************************************************************************//**
*
* Performs CMAC(Cipher-based Message Authentication Code) operation
* on a message to produce message authentication code using AES.
*
* This function available for CM0+ core only.
*
* \param cryptoAesContext
* The pointer to the stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Cmac(cy_stc_crypto_context_aes_t *cryptoAesContext)
{
    uint32_t *resultPtr = (uint32_t*)cryptoAesContext->dstPtr;
    uint32_t *messagePtr = (uint32_t*)cryptoAesContext->srcPtr;
    uint32_t  messageSize = (uint32_t)cryptoAesContext->srcSize;

    cy_stc_crypto_aes_state_t *aesStatePtr = &cryptoAesContext->aesState;

    uint32_t *blockPtr = (uint32_t*)CRYPTO->MEM_BUFF;
    uint32_t *tempPtr  = (uint32_t*)((uint8_t*)blockPtr + CY_CRYPTO_AES_BLOCK_SIZE);
    uint32_t *kPtr     = (uint32_t*)((uint8_t*)tempPtr + CY_CRYPTO_AES_BLOCK_SIZE);

    cy_stc_crypto_cmac_state_t *cmacStatePtr = (cy_stc_crypto_cmac_state_t* )((uint8_t*)kPtr + CY_CRYPTO_AES_BLOCK_SIZE);

    (void)Cy_Crypto_Core_Aes_Init(cryptoAesContext);

    Cy_Crypto_Core_Cmac_Init  (cmacStatePtr, tempPtr, blockPtr, kPtr);
    Cy_Crypto_Core_Cmac_Start (aesStatePtr, cmacStatePtr);
    Cy_Crypto_Core_Cmac_Update(aesStatePtr, cmacStatePtr, messagePtr, messageSize);
    Cy_Crypto_Core_Cmac_Finish(aesStatePtr, cmacStatePtr, resultPtr);

    return (CY_CRYPTO_SUCCESS);
}


#endif /* ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_CMAC == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
