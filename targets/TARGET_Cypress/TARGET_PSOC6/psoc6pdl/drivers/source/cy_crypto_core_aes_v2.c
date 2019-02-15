/***************************************************************************//**
* \file cy_crypto_core_aes_v2.c
* \version 2.20
*
* \brief
*  This file provides the source code fro the API for the AES method
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


#include "cy_crypto_core_aes_v2.h"
#include "cy_crypto_core_hw_v2.h"
#include "cy_crypto_core_mem_v2.h"
#include "cy_syslib.h"
#include <string.h>

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_AES == 1)

#define CY_CRYPTO_IS_KEYLENGTH_VALID(keyLength) ((CY_CRYPTO_KEY_AES_128 == (keyLength)) || \
                                                 (CY_CRYPTO_KEY_AES_192 == (keyLength)) || \
                                                 (CY_CRYPTO_KEY_AES_256 == (keyLength)))

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Aes_LoadEncKey
****************************************************************************//**
*
* Calculates an inverse block cipher key from the block cipher key.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_Aes_LoadEncKey(CRYPTO_Type *base,
                                      cy_stc_crypto_aes_state_t const *aesState)
{
    /* Set the key mode: 128, 192 or 256 Bit */
    uint32_t keySize = CY_CRYPTO_AES_128_KEY_SIZE + ((uint32_t)aesState->keyLength * 8u);

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, aesState->key, keySize);
    Cy_Crypto_Core_V2_BlockMov  (base, CY_CRYPTO_V2_RB_KEY0, CY_CRYPTO_V2_RB_FF_LOAD0, CY_CRYPTO_AES_128_KEY_SIZE);

    keySize -= CY_CRYPTO_AES_128_KEY_SIZE;

    if (keySize != 0u)
    {
        Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_KEY1, CY_CRYPTO_V2_RB_FF_LOAD0, keySize);
    }

    Cy_Crypto_Core_WaitForReady(base);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Aes_LoadDecKey
****************************************************************************//**
*
* Calculates an inverse block cipher key from the block cipher key.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_Aes_LoadDecKey(CRYPTO_Type *base,
                                      cy_stc_crypto_aes_state_t const *aesState)
{
    /* Issue the AES_KEY instruction to prepare the key for decrypt operation */
    uint32_t keySize = CY_CRYPTO_AES_128_KEY_SIZE + ((uint32_t)aesState->keyLength * 8u);

    Cy_Crypto_Core_V2_Aes_LoadEncKey(base, aesState);

    Cy_Crypto_Core_V2_RunAes(base);             /* dummy encryption */

    Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_KEY0, CY_CRYPTO_V2_RB_KEY2, CY_CRYPTO_AES_128_KEY_SIZE);

    keySize -= CY_CRYPTO_AES_128_KEY_SIZE;

    if (keySize != 0u)
    {
        Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_KEY1, CY_CRYPTO_V2_RB_KEY3, keySize);
    }

    Cy_Crypto_Core_WaitForReady(base);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Aes_Init
****************************************************************************//**
*
* Sets AES mode and prepares inverse key.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param key
* The pointer to the encryption/decryption key.
*
* \param keyLength
* \ref cy_en_crypto_aes_key_length_t
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Aes_Init(CRYPTO_Type *base,
                                                 uint8_t const *key,
                                                 cy_en_crypto_aes_key_length_t keyLength,
                                                 cy_stc_crypto_aes_state_t *aesState)
{
    CY_ASSERT_L1(NULL != key);
    CY_ASSERT_L1(NULL != aesState);
    CY_ASSERT_L3(CY_CRYPTO_IS_KEYLENGTH_VALID(keyLength));

    Cy_Crypto_Core_V2_MemSet(base, aesState, 0u, sizeof(cy_stc_crypto_aes_state_t));

    aesState->key = (uint8_t *)key;
    aesState->keyLength = keyLength;

    REG_CRYPTO_AES_CTL(base) = (uint32_t)(_VAL2FLD(CRYPTO_AES_CTL_KEY_SIZE, aesState->keyLength));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Aes_Ecb
****************************************************************************//**
*
* Performs an AES operation on one block.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t).
*
* \param dst
* The pointer to a destination cipher block.
*
* \param src
* The pointer to a source block.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Aes_Ecb(CRYPTO_Type *base,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint8_t *dst,
                                            uint8_t const *src,
                                            cy_stc_crypto_aes_state_t *aesState)
{
    (CY_CRYPTO_ENCRYPT == dirMode) ? \
        Cy_Crypto_Core_V2_Aes_LoadEncKey(base, aesState) : Cy_Crypto_Core_V2_Aes_LoadDecKey(base, aesState);

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, src, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_FFStart   (base, CY_CRYPTO_V2_RB_FF_STORE, dst, CY_CRYPTO_AES_BLOCK_SIZE);

    Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0, CY_CRYPTO_AES_BLOCK_SIZE);

    (CY_CRYPTO_ENCRYPT == dirMode) ? Cy_Crypto_Core_V2_RunAes(base) : Cy_Crypto_Core_V2_RunAesInv(base);

    Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);

    Cy_Crypto_Core_WaitForReady(base);

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Aes_Cbc
****************************************************************************//**
*
* Performs AES operation on a plain text with Cipher Block Chaining (CBC).
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t)
*
* \param srcSize
* The size of the source plain text.
*
* \param ivPtr
* The pointer to the initial vector.
*
* \param dst
* The pointer to a destination cipher text.
*
* \param src
* The pointer to a source plain text. Must be 4-Byte aligned.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Aes_Cbc(CRYPTO_Type *base,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint8_t const *ivPtr,
                                            uint8_t *dst,
                                            uint8_t const *src,
                                            cy_stc_crypto_aes_state_t *aesState)
{
    uint32_t size = srcSize;
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_SIZE_NOT_X16;

    /* Check whether the data size is multiple of CY_CRYPTO_AES_BLOCK_SIZE */
    if (0UL == (uint32_t)(srcSize & (uint32_t)(CY_CRYPTO_AES_BLOCK_SIZE - 1U)))
    {

        (CY_CRYPTO_ENCRYPT == dirMode) ? \
            Cy_Crypto_Core_V2_Aes_LoadEncKey(base, aesState) : Cy_Crypto_Core_V2_Aes_LoadDecKey(base, aesState);

        Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD1, ivPtr, CY_CRYPTO_AES_BLOCK_SIZE);

        Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, src, size);
        Cy_Crypto_Core_V2_FFStart   (base, CY_CRYPTO_V2_RB_FF_STORE, dst, size);

        if (CY_CRYPTO_ENCRYPT == dirMode)
        {
            Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_V2_RB_FF_LOAD1, CY_CRYPTO_AES_BLOCK_SIZE);

            while (size != 0U)
            {
                Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0,
                                                 CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);
                Cy_Crypto_Core_V2_RunAes(base);
                Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);

                size -= CY_CRYPTO_AES_BLOCK_SIZE;
            }
        }
        else
        {
            Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_BLOCK2, CY_CRYPTO_V2_RB_FF_LOAD1, CY_CRYPTO_AES_BLOCK_SIZE);

            while (size != 0U)
            {
                Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0, CY_CRYPTO_AES_BLOCK_SIZE);
                Cy_Crypto_Core_V2_RunAesInv(base);
                Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_BLOCK1,
                                                 CY_CRYPTO_V2_RB_BLOCK2, CY_CRYPTO_AES_BLOCK_SIZE);
                Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_BLOCK2, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_AES_BLOCK_SIZE);

                size -= CY_CRYPTO_AES_BLOCK_SIZE;
            }
        }

        tmpResult = CY_CRYPTO_SUCCESS;
    }

    Cy_Crypto_Core_WaitForReady(base);

    return (tmpResult);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Aes_Cfb
********************************************************************************
*
* Performs AES operation on a plain text with the Cipher Feedback Block method (CFB).
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t)
*
* \param srcSize
* The size of the source plain text.
*
* \param ivPtr
* The pointer to the initial vector.
*
* \param dst
* The pointer to a destination cipher text.
*
* \param src
* The pointer to a source plain text. Must be 4-Byte aligned.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Aes_Cfb(CRYPTO_Type *base,
                                             cy_en_crypto_dir_mode_t dirMode,
                                             uint32_t srcSize,
                                             uint8_t const *ivPtr,
                                             uint8_t *dst,
                                             uint8_t const *src,
                                             cy_stc_crypto_aes_state_t *aesState)
{
    uint32_t size = srcSize;

    cy_en_crypto_status_t tmpResult = CY_CRYPTO_SIZE_NOT_X16;

    /* Check whether the data size is multiple of CY_CRYPTO_AES_BLOCK_SIZE */
    if (0UL == (uint32_t)(size & (uint32_t)(CY_CRYPTO_AES_BLOCK_SIZE - 1U)))
    {
        Cy_Crypto_Core_V2_Aes_LoadEncKey(base, aesState);

        Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD1, ivPtr, CY_CRYPTO_AES_BLOCK_SIZE);
        Cy_Crypto_Core_V2_BlockMov  (base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD1, CY_CRYPTO_AES_BLOCK_SIZE);

        Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, src, size);
        Cy_Crypto_Core_V2_FFStart   (base, CY_CRYPTO_V2_RB_FF_STORE, dst, size);

        if (CY_CRYPTO_ENCRYPT == dirMode)
        {
            while (size != 0U)
            {
                Cy_Crypto_Core_V2_RunAes(base);
                Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0,
                                                 CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);
                Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_AES_BLOCK_SIZE);

                size -= CY_CRYPTO_AES_BLOCK_SIZE;
            }
        }
        else
        {
            while (size != 0U)
            {
                Cy_Crypto_Core_V2_RunAes(base);
                Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0, CY_CRYPTO_AES_BLOCK_SIZE);
                Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_BLOCK1,
                                                 CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_AES_BLOCK_SIZE);

                size -= CY_CRYPTO_AES_BLOCK_SIZE;
            }
        }

        tmpResult = CY_CRYPTO_SUCCESS;
    }

    Cy_Crypto_Core_WaitForReady(base);

    return (tmpResult);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Aes_Ctr
********************************************************************************
*
* Performs AES operation on a plain text using the counter method (CTR).
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param srcSize
* The size of a source plain text.
*
* \param srcOffset
* The size of an offset within the current block stream for resuming within the
* current cipher stream.
*
* \param ivPtr
* The 128-bit nonce and counter (16 bytes).
*
* \param streamBlock
* The saved stream-block for resuming. Is over-written by the function.
*
* \param dst
* The pointer to a destination cipher text.
*
* \param src
* The pointer to a source plain text. Must be 4-Byte aligned.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
#define CY_CRYPTO_AES_CTR_CNT_POS           (2U)
cy_en_crypto_status_t Cy_Crypto_Core_V2_Aes_Ctr(CRYPTO_Type *base,
                                            uint32_t srcSize,
                                            uint32_t *srcOffset,
                                            uint8_t *ivPtr,
                                            uint8_t *streamBlock,
                                            uint8_t *dst,
                                            uint8_t const *src,
                                            cy_stc_crypto_aes_state_t *aesState)
{
    uint32_t blockCounter[4];
    uint64_t counter;
    uint32_t cnt;
    uint32_t i;

    blockCounter[ 0] = (uint32_t) CY_CRYPTO_MERGE_BYTES(ivPtr[ 3], ivPtr[ 2], ivPtr[ 1], ivPtr[0]);
    blockCounter[ 1] = (uint32_t) CY_CRYPTO_MERGE_BYTES(ivPtr[ 7], ivPtr[ 6], ivPtr[ 5], ivPtr[4]);
    blockCounter[ 2] = (uint32_t) CY_CRYPTO_MERGE_BYTES(ivPtr[11], ivPtr[10], ivPtr[ 9], ivPtr[8]);
    blockCounter[ 3] = (uint32_t) CY_CRYPTO_MERGE_BYTES(ivPtr[15], ivPtr[14], ivPtr[13], ivPtr[12]);

    counter = CY_SWAP_ENDIAN64(*(uint64_t*)(blockCounter + CY_CRYPTO_AES_CTR_CNT_POS));

    Cy_Crypto_Core_V2_Aes_LoadEncKey(base, aesState);

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD1, (const uint8_t *) &blockCounter,  CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_BlockMov  (base, CY_CRYPTO_V2_RB_BLOCK0,   CY_CRYPTO_V2_RB_FF_LOAD1, CY_CRYPTO_AES_BLOCK_SIZE);

    /* CTR counter is placed into last 4 bytes of the Nonce block */
    Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 4U, (uint8_t)((counter >> 24U) & 0xffU));
    Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 3U, (uint8_t)((counter >> 16U) & 0xffU));
    Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 2U, (uint8_t)((counter >>  8U) & 0xffU));
    Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 1U, (uint8_t)((counter) & 0xffU));

    Cy_Crypto_Core_V2_RunAes(base);

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, src, srcSize);
    Cy_Crypto_Core_V2_FFStart   (base, CY_CRYPTO_V2_RB_FF_STORE, dst, srcSize);

    cnt = (uint32_t)(srcSize / CY_CRYPTO_AES_BLOCK_SIZE);

    for (i = 0uL; i < cnt; i++)
    {
        Cy_Crypto_Core_V2_RunAes(base);

        /* Increment the nonce counter */
        counter++;
        *(uint64_t*)(blockCounter + CY_CRYPTO_AES_CTR_CNT_POS) = CY_SWAP_ENDIAN64(counter);

        /* CTR counter is placed into last 4 bytes of the Nonce block */
        Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 4U, (uint8_t)((counter >> 24u) & 0xffU));
        Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 3U, (uint8_t)((counter >> 16u) & 0xffU));
        Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 2U, (uint8_t)((counter >>  8u) & 0xffU));
        Cy_Crypto_Core_V2_RBSetByte(base, CY_CRYPTO_AES_BLOCK_SIZE - 1U, (uint8_t)((counter) & 0xffU));

        Cy_Crypto_Core_V2_RBXor  (base, CY_CRYPTO_AES_BLOCK_SIZE, CY_CRYPTO_AES_BLOCK_SIZE);
        Cy_Crypto_Core_V2_RBStore(base, CY_CRYPTO_AES_BLOCK_SIZE, CY_CRYPTO_AES_BLOCK_SIZE);
    }

    Cy_Crypto_Core_V2_MemCpy(base, ivPtr, blockCounter, CY_CRYPTO_AES_BLOCK_SIZE);

    /* Save the reminder of the last non-complete block */
    *srcOffset = (uint32_t)(srcSize % CY_CRYPTO_AES_BLOCK_SIZE);

    return (CY_CRYPTO_SUCCESS);
}

#endif /* #if (CPUSS_CRYPTO_AES == 1) */

#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */
