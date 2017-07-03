/***************************************************************************//**
* \file cy_crypto_core_hmac.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for the HMAC method
*  in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_core_hmac.h"
#include "crypto/cy_crypto_core_sha.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_str.h"
#include "syslib/cy_syslib.h"
#include <string.h>


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1))

#define CY_CRYPTO_HMAC_IPAD               (0x36u)
#define CY_CRYPTO_HMAC_0PAD               (0x5Cu)
#define CY_CRYPTO_HMAC_MAX_PAD_SIZE       (128u)
#define CY_CRYPTO_HMAC_MAX_M0_KEY_SIZE    (128u)

typedef struct
{
    uint32_t *ipadPtr;
    uint32_t *opadPtr;
    uint32_t *m0KeyPtr;
} cy_stc_crypto_hmac_state_t;


/*******************************************************************************
* Function Name: Cy_Crypto_Core_Hmac_Init
****************************************************************************//**
*
* Initializes HMAC (Hash-based Message Authentication Code) calculation.
* Allocate two buffers (ipadPtr and opadPtr) for HMAC calculation.
*
* This function available for CM0+ core only.
*
* \param hmacStatePtr
* the pointer to the hmacStatePtr structure which stores internal variables
* for HMAC calculation.
*
* \param ipadPtr
* The memory buffer for HMAC calculation, the max used size = CRYPTO_HMAC_MAX_PAD_SIZE.
*
* \param opadPtr
* The memory buffer for HMAC calculation, the max used size = CRYPTO_HMAC_MAX_PAD_SIZE.
*
* * \param m0KeyPtr
* The memory buffer for HMAC calculation, the max used size = CRYPTO_HMAC_MAX_M0_KEY_SIZE.
*
*******************************************************************************/
void Cy_Crypto_Core_Hmac_Init(cy_stc_crypto_hmac_state_t *hmacStatePtr,
                                uint32_t *ipadPtr,
                                uint32_t *opadPtr,
                                uint32_t *m0KeyPtr)
{
    hmacStatePtr->ipadPtr  = ipadPtr;
    hmacStatePtr->opadPtr  = opadPtr;
    hmacStatePtr->m0KeyPtr = m0KeyPtr;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Hmac_Calculate
****************************************************************************//**
*
* Starts HMAC (Hash-based Message Authentication Code) calculation.
*
* This function available for CM0+ core only.
*
* \param hmacStatePtr
* The pointer to the hmacStatePtr structure which stores internal variables
* for HMAC calculation.
*
* \param hashStatePtr
* The pointer to the hashStatePtr structure which stores internal variables
* of the SHA algorithm because it is used for HMAC calculation.
*
* \param keyPtr
* The pointer to the encryption key used in computing AES-CMAC.
*
* \param keyLength
* The size of the encryption key.
*
* \param messagePtr
* The pointer to the input message.
*
* \param messageSize
* The size of the input message.
*
* \param hmacPtr
* The pointer to the calculated HMAC.
*
*******************************************************************************/
void Cy_Crypto_Core_Hmac_Calculate(cy_stc_crypto_hmac_state_t *hmacStatePtr,
                                     cy_stc_crypto_sha_state_t *hashStatePtr,
                                     uint32_t *keyPtr,
                                     uint32_t  keyLength,
                                     uint32_t *messagePtr,
                                     uint32_t  messageSize,
                                     uint32_t *hmacPtr)
{
    uint32_t i = 0uL;
    uint8_t *ipadPtrTmp  = (uint8_t*)hmacStatePtr->ipadPtr;
    uint8_t *opadPtrTmp  = (uint8_t*)hmacStatePtr->opadPtr;
    uint8_t *m0KeyPtrTmp = (uint8_t*)hmacStatePtr->m0KeyPtr;


    /* Steps 1-3 according to FIPS 198-1 */
    if (keyLength > hashStatePtr->blockSize)
    {
        /* The key is larger than the block size. Do a hash on the key. */
        Cy_Crypto_Core_Sha_Start  (hashStatePtr);
        Cy_Crypto_Core_Sha_Update (hashStatePtr, keyPtr, keyLength);
        Cy_Crypto_Core_Sha_Finish (hashStatePtr, (uint32_t*)m0KeyPtrTmp, keyLength);

        /* Append zeros */
        memset((m0KeyPtrTmp + hashStatePtr->digestSize), 0x00u,
               (hashStatePtr->blockSize - hashStatePtr->digestSize));
    }
    else if (keyLength < hashStatePtr->blockSize)
    {
        /* If the key is shorter than the block, append zeros */
        memcpy(m0KeyPtrTmp, keyPtr, (uint16_t)keyLength);
        memset((m0KeyPtrTmp + keyLength), 0x00u, (hashStatePtr->blockSize - keyLength));
    }
    else
    {
        memcpy(m0KeyPtrTmp, keyPtr, keyLength);
    }

    /* Steps 4 and 7 according to FIPS 198-1 */
    while (i < hashStatePtr->blockSize)
    {
        ipadPtrTmp[i] = CY_CRYPTO_HMAC_IPAD ^ m0KeyPtrTmp[i];
        opadPtrTmp[i] = CY_CRYPTO_HMAC_0PAD ^ m0KeyPtrTmp[i];
        i++;
    }

    /* Step 6 according to FIPS 198-1 */
    Cy_Crypto_Core_Sha_Start        (hashStatePtr);
    Cy_Crypto_Core_Sha_ProcessBlock (hashStatePtr, (uint32_t*)ipadPtrTmp);

    /* Append a message */
    Cy_Crypto_Core_Sha_Update (hashStatePtr, messagePtr, messageSize);
    Cy_Crypto_Core_Sha_Finish (hashStatePtr, (uint32_t*)ipadPtrTmp, (messageSize + hashStatePtr->blockSize));

    /* Here is the ready part of HASH: Hash((Key^ipad)||text) */

    /* Steps 8, 9 according to FIPS 198-1 */
    Cy_Crypto_Core_Sha_Start        (hashStatePtr);
    Cy_Crypto_Core_Sha_ProcessBlock (hashStatePtr, (uint32_t*)opadPtrTmp);

    /* Append HASH from Step 6 */
    Cy_Crypto_Core_Sha_Update (hashStatePtr, (uint32_t*)ipadPtrTmp, hashStatePtr->digestSize);
    Cy_Crypto_Core_Sha_Finish (hashStatePtr, hmacPtr, (hashStatePtr->digestSize + hashStatePtr->blockSize));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Hmac_Free
****************************************************************************//**
*
* Clears the used memory buffers.
*
* This function available for CM0+ core only.
*
* \param hmacStatePtr
* The pointer to the HMAC context.
*
*******************************************************************************/
void Cy_Crypto_Core_Hmac_Free(cy_stc_crypto_hmac_state_t *hmacStatePtr)
{
    /* Clear the memory buffer. */
    memset(hmacStatePtr->ipadPtr, 0x00u, CY_CRYPTO_HMAC_MAX_PAD_SIZE);
    memset(hmacStatePtr->opadPtr, 0x00u, CY_CRYPTO_HMAC_MAX_PAD_SIZE);
    memset(hmacStatePtr->m0KeyPtr, 0x00u, CY_CRYPTO_HMAC_MAX_M0_KEY_SIZE);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Hmac
****************************************************************************//**
*
* Performs HMAC calculation.
*
* This function available for CM0+ core only.
*
* \param cryptoShaContext
* The pointer to the stc_crypto_context_t structure which stores all internal variables
* for Crypto driver.
*
* \return
* A Crypto status \ref en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Hmac(cy_stc_crypto_context_sha_t *cryptoShaContext)
{
    uint32_t *messageTmpPtr = cryptoShaContext->messagePtr;
    uint32_t  messageSizeTmp = cryptoShaContext->messageSize;
    uint32_t *hmacPtr = cryptoShaContext->dstPtr;
    cy_en_crypto_sha_mode_t modeTmp = cryptoShaContext->mode;
    uint32_t *keyTmpPtr = cryptoShaContext->keyPtr;
    uint32_t keyLengthTmp = cryptoShaContext->keyLength;

    /* Allocating internal variables into the CRYPTO SRAM Buffer */
    uint32_t *blockPtr    = (uint32_t*)CRYPTO->MEM_BUFF;
    uint32_t *hashPtr     = (uint32_t*)((uint8_t*)blockPtr + CY_CRYPTO_MAX_BLOCK_SIZE);
    uint32_t *roundMemPtr = (uint32_t*)((uint8_t*)hashPtr + CY_CRYPTO_MAX_HASH_SIZE);
    uint32_t *ipadPtr     = (uint32_t*)((uint8_t*)roundMemPtr + CY_CRYPTO_MAX_ROUND_MEM_SIZE);
    uint32_t *opadPtr     = (uint32_t*)((uint8_t*)ipadPtr + CY_CRYPTO_HMAC_MAX_PAD_SIZE);
    uint32_t *m0KeyPtr    = (uint32_t*)((uint8_t*)opadPtr + CY_CRYPTO_HMAC_MAX_PAD_SIZE);

    cy_stc_crypto_hmac_state_t *hmacStatePtr = (cy_stc_crypto_hmac_state_t* )((uint8_t*)m0KeyPtr + CY_CRYPTO_MAX_BLOCK_SIZE);
    cy_stc_crypto_sha_state_t  *hashStatePtr = (cy_stc_crypto_sha_state_t* )((uint8_t*)hmacStatePtr + sizeof(cy_stc_crypto_hmac_state_t));

    Cy_Crypto_Core_Sha_Init       (hashStatePtr, (uint8_t*)blockPtr, hashPtr, roundMemPtr, modeTmp);
    Cy_Crypto_Core_Hmac_Init      (hmacStatePtr, ipadPtr, opadPtr, m0KeyPtr);

    Cy_Crypto_Core_Hmac_Calculate (hmacStatePtr, hashStatePtr, keyTmpPtr, keyLengthTmp, messageTmpPtr, messageSizeTmp, hmacPtr);

    Cy_Crypto_Core_Hmac_Free      (hmacStatePtr);
    Cy_Crypto_Core_Sha_Free       (hashStatePtr);

    return (CY_CRYPTO_SUCCESS);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
