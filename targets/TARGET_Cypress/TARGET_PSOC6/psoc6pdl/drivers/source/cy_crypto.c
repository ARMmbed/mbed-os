/***************************************************************************//**
* \file cy_crypto.c
* \version 2.40
*
* \brief
*  Provides API implementation of the Cypress PDL Crypto driver.
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


#include "cy_crypto.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

#include "cy_ipc_drv.h"
#include "cy_sysint.h"
#include "cy_syslib.h"

/*
 * The global variable to store a pointer to the customer's defined context.
 * This variable is global because it is accessed from an interrupt.
 */
static cy_stc_crypto_context_t *clientContext = NULL;

static void Cy_Crypto_Client_ReleaseIntrHndlr(void);
static bool Cy_Crypto_IsServerStarted(cy_stc_crypto_context_t const *context);
static bool Cy_Crypto_IsServerReady(cy_stc_crypto_context_t const *context);
static cy_en_crypto_status_t Cy_Crypto_Client_Send(void);

/*******************************************************************************
* Function Name: Cy_Crypto_Client_ReleaseIntrHndlr
****************************************************************************//**
*
* The interrupt handler for the Crypto IPC Release interrupt; happens
* when Crypto hardware completes operation. This function clears the specific
* interrupt source and calls the customer interrupt handler.
*
* This function is internal and should not to be called directly by user software
*
*******************************************************************************/
static void Cy_Crypto_Client_ReleaseIntrHndlr(void)
{
    uint32_t interruptMasked;

    interruptMasked = Cy_IPC_Drv_ExtractReleaseMask(Cy_IPC_Drv_GetInterruptStatusMasked(Cy_IPC_Drv_GetIntrBaseAddr(clientContext->releaseNotifierChannel)));

    /* Check that there is really the IPC Crypto Release interrupt */
    if((1uL << clientContext->ipcChannel) == interruptMasked)
    {
        Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(clientContext->releaseNotifierChannel),
                                                        interruptMasked, CY_IPC_NO_NOTIFICATION);

        if (clientContext->userCompleteCallback != NULL)
        {
            (clientContext->userCompleteCallback)();
        }
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_IsServerStarted
****************************************************************************//**
*
* Checks whether the CryptoServer is started.
*
* This function is internal and should not to be called directly by user software
*
* \param context
* The pointer to the \ref cy_stc_crypto_context_t structure that stores
* the Crypto server context.
*
* \return
* True  - CryptoServer is started.
* False - CryptoServer is not started.
*
*******************************************************************************/
static bool Cy_Crypto_IsServerStarted(cy_stc_crypto_context_t const *context)
{
    /* Check whether the CryptoServer is started (Crypto IPC Notify interrupt is started) */
    return ((1uL << (context->ipcChannel)) ==
        Cy_IPC_Drv_ExtractAcquireMask(Cy_IPC_Drv_GetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(context->acquireNotifierChannel))));
}

/*******************************************************************************
* Function Name: Cy_Crypto_IsServerReady
****************************************************************************//**
*
* Checks whether the CryptoServer is ready for operations.
*
* This function is internal and should not to be called directly by user software.
*
* \param context
* The pointer to the \ref cy_stc_crypto_context_t structure that stores
* the Crypto server context.
*
* \return
* True  - CryptoServer is ready.
* False - CryptoServer is not ready.
*
*******************************************************************************/
static bool Cy_Crypto_IsServerReady(cy_stc_crypto_context_t const *context)
{
    return (!Cy_IPC_Drv_IsLockAcquired(Cy_IPC_Drv_GetIpcBaseAddress(context->ipcChannel)));
}

cy_en_crypto_status_t Cy_Crypto_Sync(bool isBlocking)
{
    cy_en_crypto_status_t status = CY_CRYPTO_NOT_INITIALIZED;

    if (NULL != clientContext)
    {
        if (CY_CRYPTO_INSTR_UNKNOWN != clientContext->instr)
        {
            status = CY_CRYPTO_SERVER_NOT_STARTED;

            /* Check if the Crypto server started (Crypto IPC Notify interrupt is started) */
            if (Cy_Crypto_IsServerStarted(clientContext))
            {

                if (!isBlocking)
                {
                    status = CY_CRYPTO_SERVER_BUSY;

                    if (Cy_Crypto_IsServerReady(clientContext))
                    {
                        status = clientContext->resp;
                    }
                }
                else
                {
                    while (!Cy_Crypto_IsServerReady(clientContext))
                    {
                    }

                    status = clientContext->resp;
                }
            }
        }
    }

    return (status);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Client_Send
****************************************************************************//**
*
* This function sends a pointer to the Crypto Server.
*
* This function is internal and should not to be called directly by user software.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
static cy_en_crypto_status_t Cy_Crypto_Client_Send(void)
{
    cy_en_crypto_status_t status = CY_CRYPTO_SERVER_NOT_STARTED;

    if (Cy_Crypto_IsServerStarted(clientContext))
    {
        status = CY_CRYPTO_SERVER_BUSY;

        if (Cy_Crypto_IsServerReady(clientContext))
        {
            status = CY_CRYPTO_SUCCESS;

            if (CY_IPC_DRV_SUCCESS != Cy_IPC_Drv_SendMsgPtr(Cy_IPC_Drv_GetIpcBaseAddress(clientContext->ipcChannel), (1uL << clientContext->acquireNotifierChannel), clientContext))
            {
                status = CY_CRYPTO_COMM_FAIL;
            }
        }
    }

    return (status);
}

cy_en_crypto_status_t Cy_Crypto_GetErrorStatus(cy_stc_crypto_hw_error_t *hwErrorCause)
{
    if(NULL != hwErrorCause)
    {
        hwErrorCause->errorStatus0 = clientContext->hwErrorStatus.errorStatus0;
        hwErrorCause->errorStatus1 = clientContext->hwErrorStatus.errorStatus1;
    }

    return (clientContext->resp);
}

cy_en_crypto_status_t Cy_Crypto_Init(cy_stc_crypto_config_t const *config,
                                     cy_stc_crypto_context_t *context)
{
    cy_en_crypto_status_t status = CY_CRYPTO_SERVER_NOT_STARTED;

    CY_ASSERT(NULL != config);
    CY_ASSERT(NULL != context);

    context->ipcChannel = config->ipcChannel;
    context->acquireNotifierChannel = config->acquireNotifierChannel;
    context->releaseNotifierChannel = config->releaseNotifierChannel;
    context->userCompleteCallback   = config->userCompleteCallback;
    context->releaseNotifierConfig.intrSrc = config->releaseNotifierConfig.intrSrc;

    /* Release the Crypto IPC channel with the Release interrupt */
    (void)Cy_IPC_Drv_LockRelease(Cy_IPC_Drv_GetIpcBaseAddress(context->ipcChannel), CY_IPC_NO_NOTIFICATION);

    context->instr = CY_CRYPTO_INSTR_ENABLE;

    /* Check whether the Crypto server started (Crypto IPC Notify interrupt is started) */
    if (Cy_Crypto_IsServerStarted(context))
    {
        /* Sets up a Release interrupt if the customer wants */
        if (NULL != context->userCompleteCallback)
        {
            (void)Cy_SysInt_Init(&config->releaseNotifierConfig, &Cy_Crypto_Client_ReleaseIntrHndlr);

            /* Sets up the IPC Release interrupt here */
            Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(context->releaseNotifierChannel),
                                                            (1uL << context->ipcChannel), CY_IPC_NO_NOTIFICATION);

            NVIC_EnableIRQ(context->releaseNotifierConfig.intrSrc);
        }

        clientContext = context;

        status = CY_CRYPTO_SUCCESS;
    }

    return (status);
}

cy_en_crypto_status_t Cy_Crypto_DeInit(void)
{
    uint32_t interruptMasked;
    cy_en_crypto_status_t err = CY_CRYPTO_SUCCESS;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_UNKNOWN;

        /* If the release interrupt was enabled, disable it here */
        if (NULL != clientContext->userCompleteCallback)
        {
            /* Disable the Release interrupt from IPC */
            NVIC_DisableIRQ(clientContext->releaseNotifierConfig.intrSrc);

            /* Re-set up the IPC Release interrupt here */
            interruptMasked = Cy_IPC_Drv_ExtractReleaseMask(Cy_IPC_Drv_GetInterruptStatusMasked(Cy_IPC_Drv_GetIntrBaseAddr(clientContext->releaseNotifierChannel)));

            /* Check that there really is the IPC Crypto Release interrupt */
            if ((1uL << clientContext->ipcChannel) == interruptMasked)
            {
                Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(clientContext->releaseNotifierChannel), interruptMasked, CY_IPC_NO_NOTIFICATION);
            }
        }

        clientContext = NULL;
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Enable(void)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_ENABLE;
        clientContext->xdata = NULL;

        err = Cy_Crypto_Client_Send();

        /* Wait until initialization completes */
        if (CY_CRYPTO_SUCCESS == err)
        {
            err = Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);
        }
    }
    return (err);
}

/*******************************************************************************
* Function Name: Cy_Crypto_GetLibraryInfo
****************************************************************************//**
*
* This function gets information about Crypto library.
*
* \param cryptoInfo
* The pointer to a variable to store gathered crypto library information.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_GetLibraryInfo(cy_en_crypto_lib_info_t *cryptoInfo)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_SRV_INFO;
        clientContext->xdata = (void *)cryptoInfo;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Disable(void)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_DISABLE;
        clientContext->xdata = NULL;

        err = Cy_Crypto_Client_Send();

        /* Wait until initialization completes */
        if (CY_CRYPTO_SUCCESS == err)
        {
            err = Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);
        }
    }
   return (err);
}

#if (CPUSS_CRYPTO_PR == 1)
cy_en_crypto_status_t Cy_Crypto_Prng_Init(uint32_t lfsr32InitState,
                                          uint32_t lfsr31InitState,
                                          uint32_t lfsr29InitState,
                                          cy_stc_crypto_context_prng_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_PRNG_INIT;
        clientContext->xdata = (void *)cfContext;

        cfContext->lfsr32InitState = lfsr32InitState;
        cfContext->lfsr31InitState = lfsr31InitState;
        cfContext->lfsr29InitState = lfsr29InitState;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Prng_Generate(uint32_t max,
                                              uint32_t *randomNum,
                                              cy_stc_crypto_context_prng_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_PRNG;
        clientContext->xdata = (void *)cfContext;

        cfContext->max = max;
        cfContext->prngNum = randomNum;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_PR == 1) */

#if (CPUSS_CRYPTO_AES == 1)
cy_en_crypto_status_t Cy_Crypto_Aes_Init(uint32_t *key,
                                         cy_en_crypto_aes_key_length_t keyLength,
                                         cy_stc_crypto_context_aes_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_AES_INIT;
        clientContext->xdata = cfContext;

        cfContext->key = key;
        cfContext->keyLength = keyLength;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Aes_Ecb_Run(cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t *dstBlock,
                                            uint32_t *srcBlock,
                                            cy_stc_crypto_context_aes_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_AES_ECB;
        clientContext->xdata = cfContext;

        cfContext->dirMode = dirMode;
        cfContext->dst = dstBlock;
        cfContext->src = srcBlock;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Aes_Cbc_Run(cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *ivPtr,
                                            uint32_t *dst,
                                            uint32_t *src,
                                            cy_stc_crypto_context_aes_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_AES_CBC;
        clientContext->xdata = cfContext;

        cfContext->dirMode = dirMode;
        cfContext->srcSize = srcSize;
        cfContext->ivPtr = ivPtr;
        cfContext->dst = dst;
        cfContext->src = src;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Aes_Cfb_Run(cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *ivPtr,
                                            uint32_t *dst,
                                            uint32_t *src,
                                            cy_stc_crypto_context_aes_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_AES_CFB;
        clientContext->xdata = cfContext;

        cfContext->dirMode = dirMode;
        cfContext->srcSize = srcSize;
        cfContext->ivPtr = ivPtr;
        cfContext->dst = dst;
        cfContext->src = src;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Aes_Ctr_Run(cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *srcOffset,
                                            uint32_t nonceCounter[CY_CRYPTO_AES_BLOCK_SIZE / 8u],
                                            uint32_t streamBlock[CY_CRYPTO_AES_BLOCK_SIZE / 8u],
                                            uint32_t *dst,
                                            uint32_t *src,
                                            cy_stc_crypto_context_aes_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_AES_CTR;
        clientContext->xdata = cfContext;

        cfContext->dirMode = dirMode;
        cfContext->srcSize = srcSize;
        cfContext->srcOffset = srcOffset;
        cfContext->ivPtr = nonceCounter;
        cfContext->streamBlock = streamBlock;
        cfContext->dst = dst;
        cfContext->src = src;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Aes_Cmac_Run(uint32_t *src,
                                             uint32_t srcSize,
                                             uint32_t *key,
                                             cy_en_crypto_aes_key_length_t keyLength,
                                             uint32_t *cmacPtr,
                                             cy_stc_crypto_context_aes_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_CMAC;
        clientContext->xdata = cfContext;

        cfContext->srcSize = srcSize;
        cfContext->dst = cmacPtr;
        cfContext->src = src;
        cfContext->key = key;
        cfContext->keyLength = keyLength;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_AES == 1) */

#if (CPUSS_CRYPTO_SHA == 1)
cy_en_crypto_status_t Cy_Crypto_Sha_Run(uint32_t *message,
                                        uint32_t messageSize,
                                        uint32_t *digest,
                                        cy_en_crypto_sha_mode_t mode,
                                        cy_stc_crypto_context_sha_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_SHA;
        clientContext->xdata = cfContext;

        cfContext->message = message;
        cfContext->messageSize = messageSize;
        cfContext->dst = digest;
        cfContext->mode = mode;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_SHA == 1) */

#if (CPUSS_CRYPTO_SHA == 1)
cy_en_crypto_status_t Cy_Crypto_Hmac_Run(uint32_t *hmac,
                                         uint32_t *message,
                                         uint32_t messageSize,
                                         uint32_t *key,
                                         uint32_t keyLength,
                                         cy_en_crypto_sha_mode_t mode,
                                         cy_stc_crypto_context_sha_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_HMAC;
        clientContext->xdata = cfContext;

        cfContext->message = message;
        cfContext->messageSize = messageSize;
        cfContext->dst = hmac;
        cfContext->mode = mode;
        cfContext->key = key;
        cfContext->keyLength = keyLength;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_SHA == 1) */

#if (CPUSS_CRYPTO_STR == 1)
cy_en_crypto_status_t Cy_Crypto_Str_MemCpy(void* dst,
                                           void const *src,
                                           uint16_t size,
                                           cy_stc_crypto_context_str_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_MEM_CPY;
        clientContext->xdata = cfContext;

        cfContext->dst = dst;
        cfContext->src0 = src;
        cfContext->dataSize = size;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Str_MemSet(void* dst,
                                           uint8_t data,
                                           uint16_t size,
                                           cy_stc_crypto_context_str_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_MEM_SET;
        clientContext->xdata = cfContext;

        cfContext->dst = dst;
        cfContext->data = data;
        cfContext->dataSize = size;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Str_MemCmp(void const *src0,
                                           void const *src1,
                                           uint16_t size,
                                           uint32_t *resultPtr,
                                           cy_stc_crypto_context_str_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_MEM_CMP;
        clientContext->xdata = cfContext;

        cfContext->src0 = src0;
        cfContext->src1 = src1;
        cfContext->dataSize = size;
        cfContext->dst = (void* )resultPtr;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Str_MemXor(void const *src0,
                                           void const *src1,
                                           void* dst,
                                           uint16_t size,
                                           cy_stc_crypto_context_str_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_MEM_XOR;
        clientContext->xdata = cfContext;

        cfContext->src0 = src0;
        cfContext->src1 = src1;
        cfContext->dst = dst;
        cfContext->dataSize = size;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_STR == 1) */

#if (CPUSS_CRYPTO_CRC == 1)
cy_en_crypto_status_t Cy_Crypto_Crc_Init(uint32_t polynomial,
                                         uint8_t  dataReverse,
                                         uint8_t  dataXor,
                                         uint8_t  remReverse,
                                         uint32_t remXor,
                                         cy_stc_crypto_context_crc_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_CRC_INIT;
        clientContext->xdata = cfContext;

        cfContext->dataReverse = dataReverse;
        cfContext->remReverse = remReverse;
        cfContext->dataXor = dataXor;
        cfContext->polynomial = polynomial;
        cfContext->remXor = remXor;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Crc_Run(void *data,
                                        uint16_t dataSize,
                                        uint32_t *crc,
                                        uint32_t lfsrInitState,
                                        cy_stc_crypto_context_crc_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_CRC;
        clientContext->xdata = cfContext;

        cfContext->lfsrInitState = lfsrInitState;
        cfContext->data  = data;
        cfContext->dataSize = dataSize;
        cfContext->crc = crc;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_CRC == 1) */

#if (CPUSS_CRYPTO_TR == 1)
cy_en_crypto_status_t Cy_Crypto_Trng_Generate(uint32_t  GAROPol,
                                              uint32_t  FIROPol,
                                              uint32_t  max,
                                              uint32_t *randomNum,
                                              cy_stc_crypto_context_trng_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_TRNG;
        clientContext->xdata = cfContext;

        cfContext->GAROPol = GAROPol;
        cfContext->FIROPol = FIROPol;
        cfContext->max = max;
        cfContext->trngNum = randomNum;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_TR == 1) */

#if (CPUSS_CRYPTO_DES == 1)
cy_en_crypto_status_t Cy_Crypto_Des_Run(cy_en_crypto_dir_mode_t dirMode,
                                        uint32_t *key,
                                        uint32_t *dstBlock,
                                        uint32_t *srcBlock,
                                        cy_stc_crypto_context_des_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_DES;
        clientContext->xdata = cfContext;

        cfContext->dirMode = dirMode;
        cfContext->key = key;
        cfContext->dst = dstBlock;
        cfContext->src = srcBlock;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Tdes_Run(cy_en_crypto_dir_mode_t dirMode,
                                         uint32_t *key,
                                         uint32_t *dstBlock,
                                         uint32_t *srcBlock,
                                         cy_stc_crypto_context_des_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_3DES;
        clientContext->xdata = cfContext;

        cfContext->dirMode = dirMode;
        cfContext->key = key;
        cfContext->dst = dstBlock;
        cfContext->src = srcBlock;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_DES == 1) */

#if (CPUSS_CRYPTO_VU == 1)
cy_en_crypto_status_t Cy_Crypto_Rsa_Proc(cy_stc_crypto_rsa_pub_key_t const *pubKey,
                                         uint32_t const *message,
                                         uint32_t messageSize,
                                         uint32_t *processedMessage,
                                         cy_stc_crypto_context_rsa_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_RSA_PROC;
        clientContext->xdata = cfContext;

        cfContext->key = pubKey;
        cfContext->message = message;
        cfContext->messageSize = messageSize;
        cfContext->result = processedMessage;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_Rsa_CalcCoefs(cy_stc_crypto_rsa_pub_key_t const *pubKey,
                                              cy_stc_crypto_context_rsa_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_RSA_COEF;
        clientContext->xdata = cfContext;

        cfContext->key = pubKey;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

#if (CPUSS_CRYPTO_SHA == 1)
cy_en_crypto_status_t Cy_Crypto_Rsa_Verify(cy_en_crypto_rsa_ver_result_t *verResult,
                                           cy_en_crypto_sha_mode_t digestType,
                                           uint32_t const *digest,
                                           uint32_t const *decryptedSignature,
                                           uint32_t decryptedSignatureLength,
                                           cy_stc_crypto_context_rsa_ver_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_RSA_VER;
        clientContext->xdata = cfContext;

        cfContext->verResult = verResult;
        cfContext->digestType = digestType;
        cfContext->hash = digest;
        cfContext->decryptedSignature = decryptedSignature;
        cfContext->decryptedSignatureLength = decryptedSignatureLength;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}
#endif /* #if (CPUSS_CRYPTO_SHA == 1) */

cy_en_crypto_status_t Cy_Crypto_ECDSA_SignHash(const uint8_t *hash,
                                        uint32_t hashlen,
                                        uint8_t *sig,
                                        const cy_stc_crypto_ecc_key *key,
                                        const uint8_t *messageKey,
                                        cy_stc_crypto_context_ecc_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_ECDSA_SIGN;
        clientContext->xdata = cfContext;

        cfContext->datalen = hashlen;
        cfContext->src0 = hash;
        cfContext->dst0 = sig;
        cfContext->key  = key;
        cfContext->src1 = messageKey;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_ECDSA_VerifyHash(const uint8_t *sig,
                                        const uint8_t *hash,
                                        uint32_t hashlen,
                                        uint8_t *stat,
                                        const cy_stc_crypto_ecc_key *key,
                                        cy_stc_crypto_context_ecc_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_ECDSA_VER;
        clientContext->xdata = cfContext;

        cfContext->datalen = hashlen;
        cfContext->src0 = hash;
        cfContext->src1 = sig;
        cfContext->dst0 = stat;
        cfContext->key  = key;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

#endif /* #if (CPUSS_CRYPTO_VU == 1) */

cy_en_crypto_status_t Cy_Crypto_SetMemBufAddress(uint32_t const *newMembufAddress,
                                           uint32_t newMembufSize,
                                           cy_stc_crypto_context_str_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_MEMBUF_SET;
        clientContext->xdata = cfContext;

        cfContext->src0 = newMembufAddress;
        cfContext->dataSize = newMembufSize;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_GetMemBufAddress(uint32_t **membufAddress,
                                           cy_stc_crypto_context_str_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_MEMBUF_ADDR;
        clientContext->xdata = cfContext;

        cfContext->dst = (void *)membufAddress;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

cy_en_crypto_status_t Cy_Crypto_GetMemBufSize(uint32_t *membufSize,
                                           cy_stc_crypto_context_str_t *cfContext)
{
    cy_en_crypto_status_t err = CY_CRYPTO_NOT_INITIALIZED;

    if (clientContext != NULL)
    {
        clientContext->instr = CY_CRYPTO_INSTR_MEMBUF_SIZE;
        clientContext->xdata = cfContext;

        cfContext->dst = (void *)membufSize;

        err = Cy_Crypto_Client_Send();
    }
    return (err);
}

void Cy_Crypto_InvertEndianness(void *inArrPtr, uint32_t byteSize)
{
    int32_t limit;
    int32_t i;
    int32_t j = 0;
    uint8_t temp;
    uint8_t *tempPtr = (uint8_t*)inArrPtr;

    if (byteSize > 1u)
    {
        limit = (int32_t)byteSize / 2;
        if (0u == (byteSize % 2u))
        {
            --limit;
        }

        j = 0;
        i = (int32_t)byteSize - 1;
        while ( i > limit)
        {
            temp = tempPtr[j];
            tempPtr[j] = tempPtr[i];
            tempPtr[i] = temp;

            --i;
            ++j;
        }
    }
}


#if defined(__cplusplus)
}
#endif


#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */
