/***************************************************************************//**
* \file cy_crypto_core_cmac_v2.c
* \version 2.40
*
* \brief
*  This file provides the source code to the API for the CMAC method
*  in the Crypto block driver.
*
*  Implementation is done in accordance with information from this weblink:
*  nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38b.pdf
*
********************************************************************************
* Copyright 2016-2020 Cypress Semiconductor Corporation
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

#include "cy_device.h"

#if defined (CY_IP_MXCRYPTO)

#include "cy_crypto_core_cmac_v2.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if (CPUSS_CRYPTO_AES == 1)

#include "cy_crypto_core_aes_v2.h"
#include "cy_crypto_core_hw_v2.h"
#include "cy_syslib.h"

/* The bit string used to generate sub-keys */
#define CY_CRYPTO_CMAC_RB  (0x87u)

/* The structure to define used memory buffers */
typedef struct
{
    cy_stc_crypto_v2_cmac_state_t cmacState;
    uint8_t k[CY_CRYPTO_AES_BLOCK_SIZE];
} cy_stc_crypto_v2_cmac_buffers_t;

static void Cy_Crypto_Core_V2_Cmac_CalcSubKey(uint8_t *srcDstPtr);

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Cmac_CalcSubKey
****************************************************************************//**
*
* Calculates the sub-key for the CMAC algorithm
* according to the NIST publication 800-38B, page 7.
*
* \param srcDstPtr
* The pointer to the source data for sub-key calculation, see 800-38B.
*
*******************************************************************************/
static void Cy_Crypto_Core_V2_Cmac_CalcSubKey(uint8_t *srcDstPtr)
{
    int32_t i;
    uint32_t c;
    uint32_t msb = 0UL;

    for (i = (int32_t)((int32_t)CY_CRYPTO_AES_BLOCK_SIZE - 1); i >= 0; i--)
    {
        c = (uint32_t)srcDstPtr[i];
        c = (c << 1U) | msb;
        srcDstPtr[i] = (uint8_t) c;
        msb = (c >> 8U) & 1UL;
    }

    if (0UL != msb)
    {
        /* Just one byte is valuable, the rest are zeros */
        srcDstPtr[(uint8_t)(CY_CRYPTO_AES_BLOCK_SIZE - 1U)] ^= CY_CRYPTO_CMAC_RB;
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Cmac_Init
****************************************************************************//**
*
* The function for initialization of CMAC operation.
*
* \param cmacState
* The pointer to the structure which stores the CMAC context.
*
* \param k
* The pointer to the sub-key.

*******************************************************************************/
void Cy_Crypto_Core_V2_Cmac_Init(cy_stc_crypto_v2_cmac_state_t* cmacState, uint8_t* k)
{
    cmacState->k = k;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Cmac_Start
****************************************************************************//**
*
* Starts CMAC calculation.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param cmacState
* The pointer to the structure which stores the CMAC context.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_Cmac_Start(CRYPTO_Type *base,
                               cy_stc_crypto_v2_cmac_state_t *cmacState)
{
    cmacState->block_idx = 0U;

    /* Calculate the K1 sub-key */
    Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_BLOCK0,
                                     CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_RunAes(base);
    Cy_Crypto_Core_V2_FFStart (base, CY_CRYPTO_V2_RB_FF_STORE, cmacState->k, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_BlockMov(base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_Sync(base);

    Cy_Crypto_Core_V2_Cmac_CalcSubKey(cmacState->k);

    Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_V2_RB_BLOCK1,
                                     CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Cmac_Update
****************************************************************************//**
*
* Calculates CMAC on a message.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param cmacState
* The pointer to the structure which stores the CMAC context.
*
* \param message
* The pointer to the message whose CMAC is being computed.
*
* \param messageSize
* The size of the message whose CMAC is being computed.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_Cmac_Update(CRYPTO_Type *base,
                                cy_stc_crypto_v2_cmac_state_t *cmacState,
                                uint8_t const *message,
                                uint32_t  messageSize)
{
    uint32_t myBlockIdx = cmacState->block_idx + messageSize;

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, message, messageSize);

    while (myBlockIdx > CY_CRYPTO_AES_BLOCK_SIZE)
    {
        Cy_Crypto_Core_V2_BlockXor(base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0,
                                         CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);
        Cy_Crypto_Core_V2_RunAes(base);

        myBlockIdx -= CY_CRYPTO_AES_BLOCK_SIZE;
    }

    cmacState->block_idx = myBlockIdx;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Cmac_Finish
****************************************************************************//**
*
* Completes CMAC calculation.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param cmacState
* The pointer to the structure which stores the CMAC context.
*
* \param cmac
* The pointer to the computed CMAC value.
*
*******************************************************************************/
void Cy_Crypto_Core_V2_Cmac_Finish(CRYPTO_Type *base,
                                cy_stc_crypto_v2_cmac_state_t *cmacState,
                                uint8_t* cmac)
{
    uint8_t  const p_padding[16] =
    {
        0x80u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
    };

    uint8_t* myK = cmacState->k;
    uint32_t myBlockIdx = cmacState->block_idx;

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, p_padding, CY_CRYPTO_AES_BLOCK_SIZE - myBlockIdx);
    Cy_Crypto_Core_V2_BlockXor  (base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0,
                                       CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);

    if (myBlockIdx < CY_CRYPTO_AES_BLOCK_SIZE)
    {
        Cy_Crypto_Core_V2_Cmac_CalcSubKey(myK);        /* calculate "k2" */
    }

    Cy_Crypto_Core_V2_FFContinue(base, CY_CRYPTO_V2_RB_FF_LOAD0, myK, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_BlockXor  (base, CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_V2_RB_FF_LOAD0,
                                       CY_CRYPTO_V2_RB_BLOCK0, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_RunAes(base);

    Cy_Crypto_Core_V2_FFStart   (base, CY_CRYPTO_V2_RB_FF_STORE, cmac, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_BlockMov  (base, CY_CRYPTO_V2_RB_FF_STORE, CY_CRYPTO_V2_RB_BLOCK1, CY_CRYPTO_AES_BLOCK_SIZE);
    Cy_Crypto_Core_V2_Sync(base);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_V2_Cmac
****************************************************************************//**
*
* Performs CMAC(Cipher-based Message Authentication Code) operation
* on a message to produce message authentication code using AES.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param message
* The pointer to a source plain text. Must be 4-byte aligned.
*
* \param messageSize
* The size of a source plain text.
*
* \param key
* The pointer to the encryption key. Must be 4-byte aligned.
*
* \param keyLength
* \ref cy_en_crypto_aes_key_length_t
*
* \param cmac
* The pointer to the calculated CMAC.
*
* \param aesState
* The pointer to the AES state structure allocated by the user. The user
* must not modify anything in this structure.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_V2_Cmac(CRYPTO_Type *base,
                                          uint8_t const *message,
                                          uint32_t messageSize,
                                          uint8_t  const *key,
                                          cy_en_crypto_aes_key_length_t keyLength,
                                          uint8_t *cmac,
                                          cy_stc_crypto_aes_state_t *aesState)
{
    /* Allocate space for the structure which stores the CMAC context */
    cy_stc_crypto_aes_buffers_t      aesBuffersData  = {{ 0 }, { 0 }, { 0 }, { 0 }, { 0 }};
    cy_stc_crypto_v2_cmac_buffers_t  cmacBuffersData = {{0UL, NULL}, { 0 }};
    cy_stc_crypto_v2_cmac_buffers_t *cmacBuffers  = &cmacBuffersData;
    cy_stc_crypto_v2_cmac_state_t   *cmacStateLoc = &cmacBuffers->cmacState;

    CY_MISRA_DEVIATE_LINE('MISRA C-2012 Rule 18.6','No valid data expected after funtion return');
    (void)Cy_Crypto_Core_V2_Aes_Init(base, key, keyLength, aesState, &aesBuffersData);
    Cy_Crypto_Core_V2_Aes_LoadEncKey(base, aesState);

    Cy_Crypto_Core_V2_Cmac_Init  (cmacStateLoc, cmacBuffers->k);
    Cy_Crypto_Core_V2_Cmac_Start (base, cmacStateLoc);
    Cy_Crypto_Core_V2_Cmac_Update(base, cmacStateLoc, message, messageSize);
    Cy_Crypto_Core_V2_Cmac_Finish(base, cmacStateLoc, cmac);

    return (CY_CRYPTO_SUCCESS);
}

#endif /* #if (CPUSS_CRYPTO_AES == 1) */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */
