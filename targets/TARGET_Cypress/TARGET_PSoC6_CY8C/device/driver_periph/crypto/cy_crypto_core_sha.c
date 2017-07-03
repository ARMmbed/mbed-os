/***************************************************************************//**
* \file cy_crypto_core_sha.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for the SHA method
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
#include "crypto/cy_crypto_core_sha.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_str.h"
#include "syslib/cy_syslib.h"
#include <string.h>


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))


/* Initialization vectors for different modes of the SHA algorithm */
#if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1))
static const uint32_t Sha1InitHash[] =
{
    0x67452301uL, 0xEFCDAB89uL, 0x98BADCFEuL, 0x10325476uL,
    0xC3D2E1F0uL
};
#endif /* #if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1)) */

#if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1))
static const uint32_t Sha224InitHash[] =
{
    0xC1059ED8uL, 0x367CD507uL, 0x3070DD17uL, 0xF70E5939uL,
    0xFFC00B31uL, 0x68581511uL, 0x64F98FA7uL, 0xBEFA4FA4uL
};

static const uint32_t Sha256InitHash[] =
{
    0x6A09E667uL, 0xBB67AE85uL, 0x3C6EF372uL, 0xA54FF53AuL,
    0x510E527FuL, 0x9B05688CuL, 0x1F83D9ABuL, 0x5BE0CD19uL
};
#endif /* #if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1)) */

#if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1))
static const uint32_t Sha512_224InitHash[] =
{
    0x8C3D37C8uL, 0x19544DA2uL, 0x73E19966uL, 0x89DCD4D6uL,
    0x1DFAB7AEuL, 0x32FF9C82uL, 0x679DD514uL, 0x582F9FCFuL,
    0x0F6D2B69uL, 0x7BD44DA8uL, 0x77E36F73uL, 0x04C48942uL,
    0x3F9D85A8uL, 0x6A1D36C8uL, 0x1112E6ADuL, 0x91D692A1uL
};

static const uint32_t Sha512_256InitHash[] =
{
    0x22312194uL, 0xFC2BF72CuL, 0x9F555FA3uL, 0xC84C64C2uL,
    0x2393B86BuL, 0x6F53B151uL, 0x96387719uL, 0x5940EABDuL,
    0x96283EE2uL, 0xA88EFFE3uL, 0xBE5E1E25uL, 0x53863992uL,
    0x2B0199FCuL, 0x2C85B8AAuL, 0x0EB72DDCuL, 0x81C52CA2uL
};

static const uint32_t Sha384InitHash[] =
{
    0xCBBB9D5DuL, 0xC1059ED8uL, 0x629A292AuL, 0x367CD507uL,
    0x9159015AuL, 0x3070DD17uL, 0x152FECD8uL, 0xF70E5939uL,
    0x67332667uL, 0xFFC00B31uL, 0x8EB44A87uL, 0x68581511uL,
    0xDB0C2E0DuL, 0x64F98FA7uL, 0x47B5481DuL, 0xBEFA4FA4uL
};

static const uint32_t Sha512InitHash[] =
{
    0x6A09E667uL, 0xF3BCC908uL, 0xBB67AE85uL, 0x84CAA73BuL,
    0x3C6EF372uL, 0xFE94F82BuL, 0xA54FF53AuL, 0x5F1D36F1uL,
    0x510E527FuL, 0xADE682D1uL, 0x9B05688CuL, 0x2B3E6C1FuL,
    0x1F83D9ABuL, 0xFB41BD6BuL, 0x5BE0CD19uL, 0x137E2179uL
};
#endif /* #if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1)) */

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Sha_ProcessBlock
****************************************************************************//**
*
* Performs the SHA calculation on one block.
* All addresses must be 4-Byte aligned!
*
* This function available for CM0+ core only.
*
* \param hashStatePtr
* The pointer to a Hash State.
*
* \param blockPtr
* The pointer to the block whose Hash is being computed.
*
*******************************************************************************/
void Cy_Crypto_Core_Sha_ProcessBlock(cy_stc_crypto_sha_state_t *hashStatePtr, uint32_t *blockPtr)
{
    Cy_Crypto_SetReg4Instr((uint32_t)blockPtr,
                           (uint32_t)hashStatePtr->hashPtr,  /* Initial hash */
                           (uint32_t)hashStatePtr->roundMemPtr,
                           (uint32_t)hashStatePtr->hashPtr);  /* Digest */

    /* Issue the SHA instruction */
    Cy_Crypto_Run4ParamInstr(CY_CRYPTO_SHA_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC4_SHIFT,
                             CY_CRYPTO_RSRC8_SHIFT,
                             CY_CRYPTO_RSRC12_SHIFT);

    /* Wait until the SHA instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_SHA_BUSY, CRYPTO->STATUS))
    {
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Sha_Init
****************************************************************************//**
*
* The function to initialize SHA operation.
*
* This function available for CM0+ core only.
*
* \param hashStatePtr
* The pointer to a Hash State.
*
* \param mode
* One of these: CY_CRYPTO_SHA256, CY_CRYPTO_SHA1, CY_CRYPTO_SHA256_224, CY_CRYPTO_SHA512,
* CY_CRYPTO_SHA384, CY_CRYPTO_SHA512_224, CY_CRYPTO_SHA512_256
*
*******************************************************************************/
void Cy_Crypto_Core_Sha_Init(cy_stc_crypto_sha_state_t *hashStatePtr,
                             uint8_t *blockPtr,
                             uint32_t *hashPtr,
                             uint32_t *roundMemPtr,
                             cy_en_crypto_sha_mode_t mode)
{
    cy_en_crypto_sha_hw_mode_t shaHwMode;

#if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1))
    shaHwMode = CY_CRYPTO_SHA_CTL_MODE_SHA512;
#endif /* #if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1)) */

    switch (mode)
    {
#if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1))

    case CY_CRYPTO_MODE_SHA1:
        shaHwMode = CY_CRYPTO_SHA_CTL_MODE_SHA1;
        hashStatePtr->initialHashPtr = Sha1InitHash;
        hashStatePtr->blockSize      = CY_CRYPTO_SHA1_BLOCK_SIZE;
        hashStatePtr->hashSize       = CY_CRYPTO_SHA1_HASH_SIZE;
        hashStatePtr->digestSize     = CY_CRYPTO_SHA1_DIGEST_SIZE;
        hashStatePtr->roundMemSize   = CY_CRYPTO_SHA1_ROUND_MEM_SIZE;
        break;
#endif /* #if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1)) */

#if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1))

    case CY_CRYPTO_MODE_SHA224:
        shaHwMode = CY_CRYPTO_SHA_CTL_MODE_SHA256;
        hashStatePtr->initialHashPtr = Sha224InitHash;
        hashStatePtr->blockSize      = CY_CRYPTO_SHA256_BLOCK_SIZE;
        hashStatePtr->hashSize       = CY_CRYPTO_SHA256_HASH_SIZE;
        hashStatePtr->digestSize     = CY_CRYPTO_SHA224_DIGEST_SIZE;
        hashStatePtr->roundMemSize   = CY_CRYPTO_SHA256_ROUND_MEM_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA256:
        shaHwMode = CY_CRYPTO_SHA_CTL_MODE_SHA256;
        hashStatePtr->initialHashPtr = Sha256InitHash;
        hashStatePtr->blockSize      = CY_CRYPTO_SHA256_BLOCK_SIZE;
        hashStatePtr->hashSize       = CY_CRYPTO_SHA256_HASH_SIZE;
        hashStatePtr->digestSize     = CY_CRYPTO_SHA256_DIGEST_SIZE;
        hashStatePtr->roundMemSize   = CY_CRYPTO_SHA256_ROUND_MEM_SIZE;
        break;
#endif /* #if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1)) */

#if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1))

    case CY_CRYPTO_MODE_SHA384:
        hashStatePtr->initialHashPtr = Sha384InitHash;
        hashStatePtr->blockSize      = CY_CRYPTO_SHA512_BLOCK_SIZE;
        hashStatePtr->hashSize       = CY_CRYPTO_SHA512_HASH_SIZE;
        hashStatePtr->digestSize     = CY_CRYPTO_SHA384_DIGEST_SIZE;
        hashStatePtr->roundMemSize   = CY_CRYPTO_SHA512_ROUND_MEM_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512:
        hashStatePtr->initialHashPtr = Sha512InitHash;
        hashStatePtr->blockSize      = CY_CRYPTO_SHA512_BLOCK_SIZE;
        hashStatePtr->hashSize       = CY_CRYPTO_SHA512_HASH_SIZE;
        hashStatePtr->digestSize     = CY_CRYPTO_SHA512_DIGEST_SIZE;
        hashStatePtr->roundMemSize   = CY_CRYPTO_SHA512_ROUND_MEM_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512_224:
        hashStatePtr->initialHashPtr = Sha512_224InitHash;
        hashStatePtr->blockSize      = CY_CRYPTO_SHA512_BLOCK_SIZE;
        hashStatePtr->hashSize       = CY_CRYPTO_SHA512_HASH_SIZE;
        hashStatePtr->digestSize     = CY_CRYPTO_SHA512_224_DIGEST_SIZE;
        hashStatePtr->roundMemSize   = CY_CRYPTO_SHA512_ROUND_MEM_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512_256:
        hashStatePtr->initialHashPtr = Sha512_256InitHash;
        hashStatePtr->blockSize      = CY_CRYPTO_SHA512_BLOCK_SIZE;
        hashStatePtr->hashSize       = CY_CRYPTO_SHA512_HASH_SIZE;
        hashStatePtr->digestSize     = CY_CRYPTO_SHA512_256_DIGEST_SIZE;
        hashStatePtr->roundMemSize   = CY_CRYPTO_SHA512_ROUND_MEM_SIZE;
        break;
#endif /* #if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1)) */

    default:
        break;
    }

    hashStatePtr->mode = mode;
    hashStatePtr->blockPtr = blockPtr;
    hashStatePtr->hashPtr = hashPtr;
    hashStatePtr->roundMemPtr = roundMemPtr;

    /* Set the SHA mode */
    CRYPTO->SHA_CTL = (uint32_t)(_VAL2FLD(CRYPTO_SHA_CTL_MODE, (uint32_t)shaHwMode));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Sha_Start
****************************************************************************//**
*
* Initializes the initial hash vector.
*
* This function available for CM0+ core only.
*
* \param hashStatePtr
* The pointer to the SHA context.
*
*******************************************************************************/
void Cy_Crypto_Core_Sha_Start(cy_stc_crypto_sha_state_t *hashStatePtr)
{
    memcpy(hashStatePtr->hashPtr, hashStatePtr->initialHashPtr, hashStatePtr->hashSize);
}

/*******************************************************************************
* Function Name: Crypto_Server_SHA_Update
****************************************************************************//**
*
* Performs the SHA calculation on one message.
*
* This function available for CM0+ core only.
*
* \param hashStatePtr
* The pointer to the SHA context.
*
* \param messagePtr
* The pointer to the message whose Hash is being computed.
*
* \param messageSize
* The size of the message whose Hash is being computed.
*
*******************************************************************************/
void Cy_Crypto_Core_Sha_Update(cy_stc_crypto_sha_state_t *hashStatePtr,
                               uint32_t *messagePtr,
                               uint32_t  messageSize)
{
    uint8_t *blockPtrTmp = hashStatePtr->blockPtr;
    uint32_t blockSizeTmp = hashStatePtr->blockSize;
    uint32_t blockIdx = messageSize & (blockSizeTmp - 1u);
    hashStatePtr->messageSize = messageSize;

    if (messageSize >= blockSizeTmp)
    {
        while (messageSize >= blockSizeTmp)
        {
            Cy_Crypto_Core_Sha_ProcessBlock(hashStatePtr, messagePtr);

            /* Use /4 because the pointer is to uint32_t and blockSize is in Bytes */
            messagePtr += (blockSizeTmp / 4u);
            messageSize -= blockSizeTmp;
        }
    }

    /* Copy the end of the message to the block */
    memcpy(blockPtrTmp, messagePtr, blockIdx);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Sha_Finish
****************************************************************************//**
*
* Completes SHA calculation.
*
* This function available for CM0+ core only.
*
* \param hashStatePtr
* The pointer to the SHA context.
*
* \param digestPtr
* The pointer to the calculated hash digest.
*
* \param finalMessageSize
* The final message size, can be different from the messageSize
* if you use command Cy_Crypto_Core_Sha_ProcessBlock() before.
*
*******************************************************************************/
void Cy_Crypto_Core_Sha_Finish(cy_stc_crypto_sha_state_t *hashStatePtr,
                               uint32_t *digestPtr,
                               uint32_t finalMessageSize)
{
    uint32_t *hashTmpPtr = hashStatePtr->hashPtr;
    uint8_t *blockTmpPtr = hashStatePtr->blockPtr;
    uint32_t blockSizeTmp = hashStatePtr->blockSize;
    uint32_t blockIdx = (uint32_t)(hashStatePtr->messageSize & (blockSizeTmp - 1u));
    uint32_t size;

    if (CY_CRYPTO_SHA512_BLOCK_SIZE == blockSizeTmp)
    {
        size = CY_CRYPTO_SHA512_PAD_SIZE; /* Pad size = 112 */
    }
    else
    {
        size = CY_CRYPTO_SHA256_PAD_SIZE; /* Pad size = 56 */
    }

    /* Append 1 bit to the end of the message */
    blockTmpPtr[blockIdx] = 0x80u;

    /* Clear the rest of the block */
    memset((void* )&blockTmpPtr[blockIdx + 1u], 0x00u, (blockSizeTmp - blockIdx - 1u));

    if (blockIdx >= size)
    {
        /* Here we need one additional last block to calculate SHA, prepare it: */
        Cy_Crypto_Core_Sha_ProcessBlock(hashStatePtr, (uint32_t*)blockTmpPtr);

        /* Clear the last block */
        memset(blockTmpPtr, 0x00u, blockSizeTmp);
    }

    /* Write at the end length of the message, in Bits */
    blockTmpPtr[blockSizeTmp - 4u] = (uint8_t)((finalMessageSize * 8uL) >> 24u);
    blockTmpPtr[blockSizeTmp - 3u] = (uint8_t)((finalMessageSize * 8uL) >> 16u);
    blockTmpPtr[blockSizeTmp - 2u] = (uint8_t)((finalMessageSize * 8uL) >> 8u);
    blockTmpPtr[blockSizeTmp - 1u] = (uint8_t)(finalMessageSize * 8uL);

    /* Process the last block */
    Cy_Crypto_Core_Sha_ProcessBlock(hashStatePtr, (uint32_t*)blockTmpPtr);

    /* Invert endians of the hash and copy it to digestPtr, re-use the size variable */
    size = (uint32_t)(hashStatePtr->digestSize / 4u);

    for(; size != 0u; size--)
    {
        *digestPtr = CY_SWAP_ENDIAN32(*hashTmpPtr);
        digestPtr++;
        hashTmpPtr++;
    }

}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Sha_Free
****************************************************************************//**
*
* Clears the used memory buffers.
*
* This function available for CM0+ core only.
*
* \param hashStatePtr
* The pointer to the SHA context.
*
*******************************************************************************/
void Cy_Crypto_Core_Sha_Free(cy_stc_crypto_sha_state_t *hashStatePtr)
{
    /* Clears the memory buffer. */
    memset(hashStatePtr->blockPtr, 0x00u, hashStatePtr->blockSize);
    memset(hashStatePtr->hashPtr, 0x00u, hashStatePtr->hashSize);
    memset(hashStatePtr->roundMemPtr, 0x00u, hashStatePtr->roundMemSize);
}

/*******************************************************************************
* Function Name: Crypto_Core_Sha
****************************************************************************//**
*
* Performs the SHA Hash function.
*
* This function available for CM0+ core only.
*
* \param cryptoShaContext
* the pointer to the stc_crypto_context_t structure which stores all internal variables
* for Crypto driver.
*
* \return
* A Crypto status \ref en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Sha(cy_stc_crypto_context_sha_t *cryptoShaContext)
{
    uint32_t *messageTmpPtr  = cryptoShaContext->messagePtr;
    uint32_t  messageSizeTmp = cryptoShaContext->messageSize;
    uint32_t *digestPtr = cryptoShaContext->dstPtr;
    cy_en_crypto_sha_mode_t modeTmp = cryptoShaContext->mode;

    /* Allocate CRYPTO_MAX_BLOCK_SIZE Bytes for blockPtr */
    uint8_t *blockPtr = (uint8_t*)CRYPTO->MEM_BUFF;

    /* Allocate CRYPTO_MAX_HASH_SIZE Bytes for hashPtr */
    uint32_t *hashPtr = (uint32_t*)(blockPtr + CY_CRYPTO_MAX_BLOCK_SIZE);

    /* Allocate CRYPTO_MAX_ROUND_MEM_SIZE Bytes for roundMemPtr */
    uint32_t *roundMemPtr = (uint32_t*)((uint8_t*)hashPtr + CY_CRYPTO_MAX_HASH_SIZE);

    /* Allocate space for the structure which stores the SHA context */
    cy_stc_crypto_sha_state_t *hashStatePtr =
              (cy_stc_crypto_sha_state_t*)((uint8_t*)roundMemPtr + CY_CRYPTO_MAX_ROUND_MEM_SIZE);

    /* Initialize the hashStatePtr structure with zeros */
    memset(hashStatePtr, 0x00u, sizeof(cy_stc_crypto_sha_state_t));

    Cy_Crypto_Core_Sha_Init   (hashStatePtr, blockPtr, hashPtr, roundMemPtr, modeTmp);
    Cy_Crypto_Core_Sha_Start  (hashStatePtr);
    Cy_Crypto_Core_Sha_Update (hashStatePtr, messageTmpPtr, messageSizeTmp);
    Cy_Crypto_Core_Sha_Finish (hashStatePtr, digestPtr, messageSizeTmp);
    Cy_Crypto_Core_Sha_Free   (hashStatePtr);

    return (CY_CRYPTO_SUCCESS);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
