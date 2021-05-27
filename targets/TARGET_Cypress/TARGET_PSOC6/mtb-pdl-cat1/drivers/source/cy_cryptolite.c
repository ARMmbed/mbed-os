/***************************************************************************//**
* \file cy_cryptolite.c
* \version 1.0.0
*
* \brief
*  Provides API implementation of the Cryptolite PDL driver.
*
********************************************************************************
* Copyright 2020 Cypress Semiconductor Corporation
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

#if defined (CY_IP_MXCRYPTOLITE)

#include "cy_cryptolite.h"

#if defined(__cplusplus)
extern "C" {
#endif

#include "cy_sysint.h"
#include "cy_syslib.h"

/*Initial Hash*/
static const uint32_t sha256InitHash[] =
{
    0x6A09E667uL, 0xBB67AE85uL, 0x3C6EF372uL, 0xA54FF53AuL,
    0x510E527FuL, 0x9B05688CuL, 0x1F83D9ABuL, 0x5BE0CD19uL
};

/*****************************************************************************
* Cy_Cryptolite_Sha256_Process (for internal use)
******************************************************************************
*
* The function starts the hash calculation, blocks untill finished.
*
*  base
* The pointer to the Cryptolite instance.
*
*  cfContext
* The pointer to the cy_stc_cryptolite_context_sha_t structure that stores all
* internal variables for Cryptolite driver.
*
* return
* cy_en_cryptolite_status_t
*
*******************************************************************************/
static cy_en_cryptolite_status_t Cy_Cryptolite_Sha256_Process(CRYPTO_Type *base,
                                        cy_stc_cryptolite_context_sha_t *cfContext)
{
    if(REG_CRYPTOLITE_STATUS(base) & CRYPTO_STATUS_BUSY_Msk)
    {
        return CY_CRYPTOLITE_HW_BUSY;
    }

    /*write to SHA DESCR REG starts process
      IP will block another write to SHA DESCR REG untill its busy
      We poll for busy state and check for error before posting next
      descriptor */

    /*start message schedule*/
    REG_CRYPTOLITE_SHA_DESCR(base) = (uint32_t)&(cfContext->message_schedule_struct);
    while(REG_CRYPTOLITE_STATUS(base) & CRYPTO_STATUS_BUSY_Msk);
    if(REG_CRYPTOLITE_SHA_INTR_ERROR(base) & CRYPTO_INTR_ERROR_BUS_ERROR_Msk)
    {
        REG_CRYPTOLITE_SHA_INTR_ERROR(base) = CRYPTO_INTR_ERROR_BUS_ERROR_Msk;
        return CY_CRYPTOLITE_BUS_ERROR;
    }

    /*start process*/
    REG_CRYPTOLITE_SHA_DESCR(base) = (uint32_t)&(cfContext->message_process_struct);
    while(REG_CRYPTOLITE_STATUS(base) & CRYPTO_STATUS_BUSY_Msk);
    if(REG_CRYPTOLITE_SHA_INTR_ERROR(base) & CRYPTO_INTR_ERROR_BUS_ERROR_Msk)
    {
        REG_CRYPTOLITE_SHA_INTR_ERROR(base) = CRYPTO_INTR_ERROR_BUS_ERROR_Msk;
        return CY_CRYPTOLITE_BUS_ERROR;
    }

    return CY_CRYPTOLITE_SUCCESS;
}

/*****************************************************************************
* Cy_Cryptolite_Sha256_Init
******************************************************************************
*
* The function to initialize the SHA256 operation.
*
*  base
* The pointer to the Cryptolite instance.
*
*  cfContext
* The pointer to the cy_stc_cryptolite_context_sha_t structure that stores all
* internal variables for Cryptolite driver.
*
* return
* cy_en_cryptolite_status_t
*
*******************************************************************************/
cy_en_cryptolite_status_t Cy_Cryptolite_Sha256_Init(CRYPTO_Type *base,
                                        cy_stc_cryptolite_context_sha_t *cfContext)
{
    /* Input parameters verification */
    if ((NULL == base) || (NULL == cfContext))
    {
        return CY_CRYPTOLITE_BAD_PARAMS;
    }

    cfContext->message = (uint8_t*)cfContext->msgblock;

    return (CY_CRYPTOLITE_SUCCESS);
}

/*******************************************************************************
* Cy_Cryptolite_Sha256_Start
******************************************************************************
*
* Initializes the initial Hash vector.
*
*  base
* The pointer to the CRYPTOLITE instance.
*
*  cfContext
* The pointer to the cy_stc_cryptolite_context_sha_t structure that stores all
* internal variables for Cryptolite driver.
*
* return
* cy_en_cryptolite_status_t
*
*******************************************************************************/
cy_en_cryptolite_status_t Cy_Cryptolite_Sha256_Start(CRYPTO_Type *base,
                                        cy_stc_cryptolite_context_sha_t *cfContext)
{
    uint32_t i;
    /* Input parameters verification */
    if ((NULL == base) || (NULL == cfContext))
    {
        return CY_CRYPTOLITE_BAD_PARAMS;
    }

    /*check if IP is busy*/
    if (REG_CRYPTOLITE_STATUS(base) & CRYPTO_STATUS_BUSY_Msk)
    {
        return CY_CRYPTOLITE_HW_BUSY;
    }

    cfContext->msgIdx = 0U;
    cfContext->messageSize = 0U;
    cfContext->message_schedule_struct.data0 = (uint32_t)CY_CRYPTOLITE_MSG_SCH_CTLWD;
    cfContext->message_schedule_struct.data1 = (uint32_t)cfContext->message;
    cfContext->message_schedule_struct.data2 = (uint32_t)cfContext->message_schedule;


    cfContext->message_process_struct.data0 = (uint32_t)CY_CRYPTOLITE_PROCESS_CTLWD;
    cfContext->message_process_struct.data1 = (uint32_t)cfContext->hash;
    cfContext->message_process_struct.data2 = (uint32_t)cfContext->message_schedule;

    /*copy initial hash*/
    for (i=0; i < CY_CRYPTOLITE_SHA256_HASH_SIZE/4; i++)
    {
        cfContext->hash[i] = sha256InitHash[i];
    }

    return CY_CRYPTOLITE_SUCCESS;
}

/*******************************************************************************
* Cy_Cryptolite_Sha256_Update
********************************************************************************
*
* Performs the SHA256 calculation on one message.
*
*  base
* The pointer to the CRYPTOLITE instance.
*
*  cfContext
* The pointer to the cy_stc_cryptolite_context_sha_t structure that stores all
* internal variables for Cryptolite driver.
*
*  message
* The pointer to the message whose Hash is being computed.
* 
*  messageSize
* The size of the message whose Hash is being computed.
*
* return
* cy_en_cryptolite_status_t
*
*******************************************************************************/
cy_en_cryptolite_status_t Cy_Cryptolite_Sha256_Update(CRYPTO_Type *base,
                                        cy_stc_cryptolite_context_sha_t *cfContext,
                                        uint8_t const *message,
                                        uint32_t  messageSize)
{
    cy_en_cryptolite_status_t err = CY_CRYPTOLITE_BAD_PARAMS;
    uint32_t idx = 0U;

    /* Input parameters verification */
    if ((NULL == base) || (NULL == cfContext) || (NULL == message)|| (0 == messageSize))
    {
        return err;
    }

    /*check if IP is busy*/
    if (REG_CRYPTOLITE_STATUS(base) & CRYPTO_STATUS_BUSY_Msk)
    {
        return CY_CRYPTOLITE_HW_BUSY;
    }

    while((cfContext->msgIdx + messageSize) >= CY_CRYPTOLITE_SHA256_BLOCK_SIZE)
    {
        uint32_t tocopy = CY_CRYPTOLITE_SHA256_BLOCK_SIZE - cfContext->msgIdx;
        /* Create a message block */
        for ( ; idx < tocopy; idx++ )
        {
            cfContext->message[cfContext->msgIdx] = message[idx];
        }
        /* calculate message schedule and process */
        err = Cy_Cryptolite_Sha256_Process(base, cfContext);
        if(CY_CRYPTOLITE_SUCCESS != err)
        {
            return err;
        }
        messageSize-= tocopy;
        cfContext->messageSize+= CY_CRYPTOLITE_SHA256_BLOCK_SIZE;
        cfContext->msgIdx = 0U;
    }
    /* Copy message fragment*/
    for ( ; idx < messageSize; idx++ )
    {
        cfContext->message[cfContext->msgIdx] = message[idx];
        cfContext->msgIdx++;
    }
    /*size of total bytes of message processed */
    cfContext->messageSize+= messageSize;

    return CY_CRYPTOLITE_SUCCESS;
}

/*******************************************************************************
* Cy_Cryptolite_Sha256_Finish
******************************************************************************
*
* Completes the SHA256 calculation.
*
*  base
* The pointer to the CRYPTOLITE instance.
*
*  cfContext
* the pointer to the cy_stc_cryptolite_context_sha_t structure that stores all
* internal variables for Cryptolite driver.
*
*  digest
* The pointer to the calculated Hash digest.
* 
* return
* cy_en_cryptolite_status_t
*
*******************************************************************************/
cy_en_cryptolite_status_t Cy_Cryptolite_Sha256_Finish(CRYPTO_Type *base,
                                    cy_stc_cryptolite_context_sha_t *cfContext,
                                    uint8_t *digest)
{
    cy_en_cryptolite_status_t err = CY_CRYPTOLITE_SUCCESS;
    uint8_t *hashptr;
    uint32_t idx;
    uint64_t totalMessageSizeInBits;

    /* Input parameters verification */
    if ((NULL == base) || (NULL == cfContext) || (NULL == digest))
    {
        return err;
    }

    /*check if IP is busy*/
    if (REG_CRYPTOLITE_STATUS(base) & CRYPTO_STATUS_BUSY_Msk)
    {
        return CY_CRYPTOLITE_HW_BUSY;
    }

    totalMessageSizeInBits = (uint64_t)cfContext->messageSize * 8U;
    /*Append one bit to end and clear rest of block*/
    cfContext->message[cfContext->msgIdx] = 0x80U;
    idx = cfContext->msgIdx + 1;

    for ( ; idx < CY_CRYPTOLITE_SHA256_BLOCK_SIZE; idx++ )
    {
        cfContext->message[idx] = 0U;
    }

    /*if message size is more than pad size process the block*/
    if (cfContext->msgIdx >= CY_CRYPTOLITE_SHA256_PAD_SIZE)
    {
        err = Cy_Cryptolite_Sha256_Process(base, cfContext);
        if(CY_CRYPTOLITE_SUCCESS != err)
        {
            return err;
        }
        /*clear the message block to finish*/
        for ( idx = 0; idx < CY_CRYPTOLITE_SHA256_PAD_SIZE; idx++ )
        {
            cfContext->message[idx] = 0U;
        }
    }

    /*append total message size in bits from 57 to 64 bytes */
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 1] = (uint8_t)totalMessageSizeInBits;
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 2] = (uint8_t)(totalMessageSizeInBits >> 8);
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 3] = (uint8_t)(totalMessageSizeInBits >> 16);
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 4] = (uint8_t)(totalMessageSizeInBits >> 24);
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 5] = (uint8_t)(totalMessageSizeInBits >> 32);
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 6] = (uint8_t)(totalMessageSizeInBits >> 40);
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 7] = (uint8_t)(totalMessageSizeInBits >> 48);
    cfContext->message[CY_CRYPTOLITE_SHA256_BLOCK_SIZE - 8] = (uint8_t)(totalMessageSizeInBits >> 56);

    /*Process the last block*/
    err = Cy_Cryptolite_Sha256_Process(base, cfContext);
    if(CY_CRYPTOLITE_SUCCESS != err)
    {
        return err;
    }
    
    /* This implementation uses little endian ordering and SHA uses big endian,
       reverse all the bytes in 32bit word when copying the final output hash.*/
    idx = (uint32_t)(CY_CRYPTOLITE_SHA256_HASH_SIZE / 4U);
    hashptr = (uint8_t*)cfContext->hash;

    for( ; idx != 0U; idx--)
    {
        *(digest)   = *(hashptr+3);
        *(digest+1) = *(hashptr+2);
        *(digest+2) = *(hashptr+1);
        *(digest+3) = *(hashptr);

        digest  += 4U;
        hashptr += 4U;
    }
    
    return CY_CRYPTOLITE_SUCCESS;
}

/*******************************************************************************
* Cy_Cryptolite_Sha256_Free
******************************************************************************
*
* Clears the used memory and context data.
*
*  base
* The pointer to the CRYPTOLITE instance.
*
*  cfContext
* the pointer to the cy_stc_cryptolite_context_sha_t structure that stores all
* internal variables for Cryptolite driver.
*
* return
* cy_en_cryptolite_status_t
*
*******************************************************************************/
cy_en_cryptolite_status_t Cy_Cryptolite_Sha256_Free(CRYPTO_Type *base,
                                    cy_stc_cryptolite_context_sha_t *cfContext)
{
    uint32_t idx;
    (void)base;

    /* Input parameters verification */
    if (NULL == cfContext)
    {
        return CY_CRYPTOLITE_BAD_PARAMS;
    }

    /* Clear the context memory */
    for ( idx = 0; idx < CY_CRYPTOLITE_SHA256_BLOCK_SIZE; idx++ )
    {
        cfContext->message[idx] = 0U;
        cfContext->message_schedule[idx] = 0U;
    }
    for ( idx = 0; idx < CY_CRYPTOLITE_SHA256_HASH_SIZE/4 ; idx++ )
    {
        cfContext->hash[idx] = 0U;
    }

    return CY_CRYPTOLITE_SUCCESS;
}

/*******************************************************************************
* Cy_Cryptolite_Sha256_Run
******************************************************************************
*
* This function performs the SHA256 Hash function.
* Provide the required parameters and the pointer
* to the context structure when making this function call.
* It is independent of the previous Crypto state because it already contains
* preparation, calculation, and finalization steps.
*
*  base
* The pointer to the CRYPTOLITE instance.
*
*  message
* The pointer to a message whose hash value is being computed.
* 
*  messageSize
* The size of a message in bytes.
*
*  digest
* The pointer to the hash digest.
* 
*  cfContext
* the pointer to the cy_stc_cryptolite_context_sha_t structure that stores all
* internal variables for Cryptolite driver.
*
* return
* cy_en_cryptolite_status_t
*
*******************************************************************************/
cy_en_cryptolite_status_t Cy_Cryptolite_Sha256_Run(CRYPTO_Type *base,
                                        uint8_t const *message,
                                        uint32_t  messageSize,
                                        uint8_t *digest,
                                        cy_stc_cryptolite_context_sha_t *cfContext)
{
    cy_en_cryptolite_status_t err = CY_CRYPTOLITE_BAD_PARAMS;
    /* Input parameters verification */
    if ((NULL == base) || (NULL == cfContext) || (NULL == message)|| (NULL == digest) || (0 == messageSize))
    {
        return err;
    }

    err = Cy_Cryptolite_Sha256_Init (base, cfContext);

    if (CY_CRYPTOLITE_SUCCESS == err)
    {
        err = Cy_Cryptolite_Sha256_Start (base, cfContext);
    }
    if (CY_CRYPTOLITE_SUCCESS == err)
    {
        err = Cy_Cryptolite_Sha256_Update (base, cfContext, message, messageSize);
    }
    if (CY_CRYPTOLITE_SUCCESS == err)
    {
        err = Cy_Cryptolite_Sha256_Finish (base, cfContext, digest);
    }
    if (CY_CRYPTOLITE_SUCCESS == err)
    {
        err = Cy_Cryptolite_Sha256_Free (base, cfContext);
    }

    return (err);
}

#if defined(__cplusplus)
}
#endif


#endif /* CY_IP_MXCRYPTOLITE */


/* [] END OF FILE */
