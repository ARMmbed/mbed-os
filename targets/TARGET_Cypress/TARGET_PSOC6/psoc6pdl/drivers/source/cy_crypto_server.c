/***************************************************************************//**
* \file cy_crypto_server.c
* \version 2.40
*
* \brief
*  This file provides the source code to the API for Crypto Server
*  in the Crypto driver.
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


#include "cy_crypto_server.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

#include "cy_crypto_core_aes.h"
#include "cy_crypto_core_sha.h"
#include "cy_crypto_core_hmac.h"
#include "cy_crypto_core_cmac.h"
#include "cy_crypto_core_prng.h"
#include "cy_crypto_core_trng.h"
#include "cy_crypto_core_rsa.h"
#include "cy_crypto_core_mem.h"
#include "cy_crypto_core_crc.h"
#include "cy_crypto_core_des.h"
#include "cy_crypto_core_hw.h"
#include "cy_crypto_core_ecc.h"
#include "cy_ipc_drv.h"
#include "cy_sysint.h"
#include "cy_syslib.h"
#include <stdbool.h>
#include <string.h>

/* The pointer to the CRYPTO instance. */
#define CY_CRYPTO_BASE           ((CRYPTO_Type *)cy_device->cryptoBase)

typedef struct
{
    cy_crypto_prng_init_func_t   prngInitFunc;
    cy_crypto_prng_func_t        prngFunc;
    cy_crypto_trng_func_t        trngFunc;
    cy_crypto_aes_init_func_t    aesInitFunc;
    cy_crypto_aes_ecb_func_t     aesEcbFunc;
    cy_crypto_aes_cbc_func_t     aesCbcFunc;
    cy_crypto_aes_cfb_func_t     aesCfbFunc;
    cy_crypto_aes_ctr_func_t     aesCtrFunc;
    cy_crypto_cmac_func_t        cmacFunc;
    cy_crypto_sha_func_t         shaFunc;
    cy_crypto_hmac_func_t        hmacFunc;
    cy_crypto_memcpy_func_t      memCpyFunc;
    cy_crypto_memset_func_t      memSetFunc;
    cy_crypto_memcmp_func_t      memCmpFunc;
    cy_crypto_memxor_func_t      memXorFunc;
    cy_crypto_crc_init_func_t    crcInitFunc;
    cy_crypto_crc_func_t         crcFunc;
    cy_crypto_des_func_t         desFunc;
    cy_crypto_des_func_t         tdesFunc;
    cy_crypto_rsa_proc_func_t    rsaProcFunc;
    cy_crypto_rsa_coef_func_t    rsaCoefFunc;
    cy_crypto_rsa_ver_func_t     rsaVerifyFunc;
} cy_stc_crypto_pfn_t;

static cy_stc_crypto_pfn_t const *cy_CryptoFunctionTable = NULL;

/*
 * The global variable to store a pointer to crypto processing context data.
 */
static cy_stc_crypto_context_t *processData = NULL;

static cy_stc_crypto_server_context_t *cy_crypto_serverContext;

/* Functions Prototypes */
static cy_en_crypto_status_t Cy_Crypto_Core_CheckHwForErrors(cy_stc_crypto_context_t *cryptoContext);

static cy_en_crypto_status_t Cy_Crypto_Server_Start_Common(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context);

cy_en_crypto_status_t Cy_Crypto_Server_Start_Base(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context)
{
    static const cy_stc_crypto_pfn_t cryptoV1BaseFuncs =
    {
        &Cy_Crypto_Core_V1_Prng_Init,
        &Cy_Crypto_Core_V1_Prng,
        &Cy_Crypto_Core_V1_Trng,
        NULL, /* Cy_Crypto_Core_V1_Aes_Init, */
        NULL, /* Cy_Crypto_Core_V1_Aes_Ecb,  */
        NULL, /* Cy_Crypto_Core_V1_Aes_Cbc,  */
        NULL, /* Cy_Crypto_Core_V1_Aes_Cfb,  */
        NULL, /* Cy_Crypto_Core_V1_Aes_Ctr,  */
        NULL, /* Cy_Crypto_Core_V1_Cmac,     */
        &Cy_Crypto_Core_V1_Sha,
        NULL, /* Cy_Crypto_Core_V1_Hmac,     */
        &Cy_Crypto_Core_V1_MemCpy,
        &Cy_Crypto_Core_V1_MemSet,
        &Cy_Crypto_Core_V1_MemCmp,
        &Cy_Crypto_Core_V1_MemXor,
        &Cy_Crypto_Core_V1_Crc_Init,
        &Cy_Crypto_Core_V1_Crc,
        &Cy_Crypto_Core_V1_Des,
        &Cy_Crypto_Core_V1_Tdes,
        NULL, /* Cy_Crypto_Core_Rsa_Proc,  */
        NULL, /* Cy_Crypto_Core_Rsa_Coef,  */
        NULL, /* Cy_Crypto_Core_RsaVerify, */
    };

    static const cy_stc_crypto_pfn_t cryptoV2BaseFuncs =
    {
        &Cy_Crypto_Core_V2_Prng_Init,
        &Cy_Crypto_Core_V2_Prng,
        &Cy_Crypto_Core_V2_Trng,
        NULL, /* Cy_Crypto_Core_V2_Aes_Init, */
        NULL, /* Cy_Crypto_Core_V2_Aes_Ecb,  */
        NULL, /* Cy_Crypto_Core_V2_Aes_Cbc,  */
        NULL, /* Cy_Crypto_Core_V2_Aes_Cfb,  */
        NULL, /* Cy_Crypto_Core_V2_Aes_Ctr,  */
        NULL, /* Cy_Crypto_Core_V2_Cmac,     */
        &Cy_Crypto_Core_V2_Sha,
        NULL, /* Cy_Crypto_Core_V2_Hmac,     */
        &Cy_Crypto_Core_V2_MemCpy,
        &Cy_Crypto_Core_V2_MemSet,
        &Cy_Crypto_Core_V2_MemCmp,
        &Cy_Crypto_Core_V2_MemXor,
        &Cy_Crypto_Core_V2_Crc_Init,
        &Cy_Crypto_Core_V2_Crc,
        &Cy_Crypto_Core_V2_Des,
        &Cy_Crypto_Core_V2_Tdes,
        NULL, /* Cy_Crypto_Core_Rsa_Proc,  */
        NULL, /* Cy_Crypto_Core_Rsa_Coef,  */
        NULL, /* Cy_Crypto_Core_RsaVerify, */
    };

    if (CY_CRYPTO_V1)
    {
        cy_CryptoFunctionTable = &cryptoV1BaseFuncs;
    }
    else
    {
        cy_CryptoFunctionTable = &cryptoV2BaseFuncs;
    }
    return Cy_Crypto_Server_Start_Common(config, context);
}

cy_en_crypto_status_t Cy_Crypto_Server_Start_Extra(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context)
{
    static const cy_stc_crypto_pfn_t cryptoV1ExtraFuncs =
    {
        &Cy_Crypto_Core_V1_Prng_Init,
        &Cy_Crypto_Core_V1_Prng,
        &Cy_Crypto_Core_V1_Trng,
        &Cy_Crypto_Core_V1_Aes_Init,
        &Cy_Crypto_Core_V1_Aes_Ecb,
        &Cy_Crypto_Core_V1_Aes_Cbc,
        &Cy_Crypto_Core_V1_Aes_Cfb,
        &Cy_Crypto_Core_V1_Aes_Ctr,
        &Cy_Crypto_Core_V1_Cmac,
        &Cy_Crypto_Core_V1_Sha,
        &Cy_Crypto_Core_V1_Hmac,
        &Cy_Crypto_Core_V1_MemCpy,
        &Cy_Crypto_Core_V1_MemSet,
        &Cy_Crypto_Core_V1_MemCmp,
        &Cy_Crypto_Core_V1_MemXor,
        &Cy_Crypto_Core_V1_Crc_Init,
        &Cy_Crypto_Core_V1_Crc,
        &Cy_Crypto_Core_V1_Des,
        &Cy_Crypto_Core_V1_Tdes,
        NULL, /* Cy_Crypto_Core_Rsa_Proc,  */
        NULL, /* Cy_Crypto_Core_Rsa_Coef,  */
        NULL, /* Cy_Crypto_Core_RsaVerify, */
    };

    static const cy_stc_crypto_pfn_t cryptoV2ExtraFuncs =
    {
        &Cy_Crypto_Core_V2_Prng_Init,
        &Cy_Crypto_Core_V2_Prng,
        &Cy_Crypto_Core_V2_Trng,
        &Cy_Crypto_Core_V2_Aes_Init,
        &Cy_Crypto_Core_V2_Aes_Ecb,
        &Cy_Crypto_Core_V2_Aes_Cbc,
        &Cy_Crypto_Core_V2_Aes_Cfb,
        &Cy_Crypto_Core_V2_Aes_Ctr,
        &Cy_Crypto_Core_V2_Cmac,
        &Cy_Crypto_Core_V2_Sha,
        &Cy_Crypto_Core_V2_Hmac,
        &Cy_Crypto_Core_V2_MemCpy,
        &Cy_Crypto_Core_V2_MemSet,
        &Cy_Crypto_Core_V2_MemCmp,
        &Cy_Crypto_Core_V2_MemXor,
        &Cy_Crypto_Core_V2_Crc_Init,
        &Cy_Crypto_Core_V2_Crc,
        &Cy_Crypto_Core_V2_Des,
        &Cy_Crypto_Core_V2_Tdes,
        NULL, /* Cy_Crypto_Core_Rsa_Proc,  */
        NULL, /* Cy_Crypto_Core_Rsa_Coef,  */
        NULL, /* Cy_Crypto_Core_RsaVerify, */
    };

    if (CY_CRYPTO_V1)
    {
        cy_CryptoFunctionTable = &cryptoV1ExtraFuncs;
    }
    else
    {
        cy_CryptoFunctionTable = &cryptoV2ExtraFuncs;
    }
    return Cy_Crypto_Server_Start_Common(config, context);
}

cy_en_crypto_status_t Cy_Crypto_Server_Start_Full(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context)
{
    static const cy_stc_crypto_pfn_t cryptoV1FullFuncs =
    {
        &Cy_Crypto_Core_V1_Prng_Init,
        &Cy_Crypto_Core_V1_Prng,
        &Cy_Crypto_Core_V1_Trng,
        &Cy_Crypto_Core_V1_Aes_Init,
        &Cy_Crypto_Core_V1_Aes_Ecb,
        &Cy_Crypto_Core_V1_Aes_Cbc,
        &Cy_Crypto_Core_V1_Aes_Cfb,
        &Cy_Crypto_Core_V1_Aes_Ctr,
        &Cy_Crypto_Core_V1_Cmac,
        &Cy_Crypto_Core_V1_Sha,
        &Cy_Crypto_Core_V1_Hmac,
        &Cy_Crypto_Core_V1_MemCpy,
        &Cy_Crypto_Core_V1_MemSet,
        &Cy_Crypto_Core_V1_MemCmp,
        &Cy_Crypto_Core_V1_MemXor,
        &Cy_Crypto_Core_V1_Crc_Init,
        &Cy_Crypto_Core_V1_Crc,
        &Cy_Crypto_Core_V1_Des,
        &Cy_Crypto_Core_V1_Tdes,
        &Cy_Crypto_Core_Rsa_Proc,
        &Cy_Crypto_Core_Rsa_Coef,
        &Cy_Crypto_Core_Rsa_Verify,
    };

    static const cy_stc_crypto_pfn_t cryptoV2FullFuncs =
    {
        &Cy_Crypto_Core_V2_Prng_Init,
        &Cy_Crypto_Core_V2_Prng,
        &Cy_Crypto_Core_V2_Trng,
        &Cy_Crypto_Core_V2_Aes_Init,
        &Cy_Crypto_Core_V2_Aes_Ecb,
        &Cy_Crypto_Core_V2_Aes_Cbc,
        &Cy_Crypto_Core_V2_Aes_Cfb,
        &Cy_Crypto_Core_V2_Aes_Ctr,
        &Cy_Crypto_Core_V2_Cmac,
        &Cy_Crypto_Core_V2_Sha,
        &Cy_Crypto_Core_V2_Hmac,
        &Cy_Crypto_Core_V2_MemCpy,
        &Cy_Crypto_Core_V2_MemSet,
        &Cy_Crypto_Core_V2_MemCmp,
        &Cy_Crypto_Core_V2_MemXor,
        &Cy_Crypto_Core_V2_Crc_Init,
        &Cy_Crypto_Core_V2_Crc,
        &Cy_Crypto_Core_V2_Des,
        &Cy_Crypto_Core_V2_Tdes,
        &Cy_Crypto_Core_Rsa_Proc,
        &Cy_Crypto_Core_Rsa_Coef,
        &Cy_Crypto_Core_Rsa_Verify,
    };

    if (CY_CRYPTO_V1)
    {
        cy_CryptoFunctionTable = &cryptoV1FullFuncs;
    }
    else
    {
        cy_CryptoFunctionTable = &cryptoV2FullFuncs;
    }
    return Cy_Crypto_Server_Start_Common(config, context);
}

static cy_en_crypto_status_t Cy_Crypto_Server_Start_Common(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context)
{
    cy_israddress isrHandler;

    CY_ASSERT(NULL != config);
    CY_ASSERT(NULL != context);
    CY_ASSERT(NULL != cy_CryptoFunctionTable);

    context->ipcChannel = config->ipcChannel;
    context->acquireNotifierChannel = config->acquireNotifierChannel;
    context->getDataHandlerPtr  = config->userGetDataHandler;
    context->errorHandlerPtr    = config->userErrorHandler;
    context->isHwErrorOccured = false;
    context->acquireNotifierConfig.intrSrc = config->acquireNotifierConfig.intrSrc;
    context->cryptoErrorIntrConfig.intrSrc = config->cryptoErrorIntrConfig.intrSrc;

    Cy_Crypto_Core_HwInit();

    /* Initialize the interrupt controller for CM0+ and IPC Interrupt */
    if (config->userGetDataHandler != NULL)
    {
        isrHandler = config->userGetDataHandler;
    }
    else
    {
        isrHandler = &Cy_Crypto_Server_GetDataHandler;
    }

    (void)Cy_SysInt_Init(&config->acquireNotifierConfig, isrHandler);

    NVIC_EnableIRQ(config->acquireNotifierConfig.intrSrc);

    /* Do not bring up an IPC release interrupt here, only set up a notify interrupt */
    Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(config->acquireNotifierChannel),
                                                      CY_IPC_NO_NOTIFICATION, (1uL << config->ipcChannel));

    /* Initialize and enable an interrupt to handle possible Crypto HW errors */
    if (config->userErrorHandler != NULL)
    {
        isrHandler = config->userErrorHandler;
    }
    else
    {
        isrHandler = &Cy_Crypto_Server_ErrorHandler;
    }

    (void)Cy_SysInt_Init(&config->cryptoErrorIntrConfig, isrHandler);

    NVIC_ClearPendingIRQ(config->cryptoErrorIntrConfig.intrSrc);
    NVIC_EnableIRQ(config->cryptoErrorIntrConfig.intrSrc);

    Cy_Crypto_Core_SetInterruptMask(CY_CRYPTO_BASE, CY_CRYPTO_INTR_MASK_ERROR_MASK);

    cy_crypto_serverContext = context;

    return (CY_CRYPTO_SUCCESS);
}

cy_en_crypto_status_t Cy_Crypto_Server_Stop(void)
{
    if (NULL != cy_crypto_serverContext)
    {
        uint32_t interruptMask;

        /* Disable the Notify interrupt from IPC */
        NVIC_DisableIRQ(cy_crypto_serverContext->acquireNotifierConfig.intrSrc);
        NVIC_ClearPendingIRQ(cy_crypto_serverContext->acquireNotifierConfig.intrSrc);

        /* Disable the Error interrupt from Crypto */
        NVIC_DisableIRQ(cy_crypto_serverContext->cryptoErrorIntrConfig.intrSrc);
        NVIC_ClearPendingIRQ(cy_crypto_serverContext->cryptoErrorIntrConfig.intrSrc);
        Cy_Crypto_Core_SetInterruptMask(CY_CRYPTO_BASE, 0u);

        /* Disable CRYPTO IPC interrupt */
        interruptMask = Cy_IPC_Drv_ExtractAcquireMask(Cy_IPC_Drv_GetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(cy_crypto_serverContext->acquireNotifierChannel)));

        Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(cy_crypto_serverContext->acquireNotifierChannel),
                                                          CY_IPC_NO_NOTIFICATION,
                                                          interruptMask & ~(1uL << cy_crypto_serverContext->ipcChannel));

        cy_crypto_serverContext->getDataHandlerPtr = NULL;
        cy_crypto_serverContext->errorHandlerPtr = NULL;
        cy_crypto_serverContext = NULL;
        cy_CryptoFunctionTable  = NULL;
    }

    return (CY_CRYPTO_SUCCESS);
}

void Cy_Crypto_Server_ErrorHandler(void)
{
    uint32_t interrupts;
    interrupts = Cy_Crypto_Core_GetInterruptStatus(CY_CRYPTO_BASE) & CY_CRYPTO_INTR_ERROR_MASK;

    if (interrupts != 0u)
    {
        Cy_Crypto_Core_ClearInterrupt(CY_CRYPTO_BASE, interrupts);

        if (false == cy_crypto_serverContext->isHwErrorOccured)
        {
            /* HW error is not captured before */
            cy_crypto_serverContext->hwErrorStatus.errorStatus0 = REG_CRYPTO_ERROR_STATUS0(CY_CRYPTO_BASE);
            cy_crypto_serverContext->hwErrorStatus.errorStatus1 = REG_CRYPTO_ERROR_STATUS1(CY_CRYPTO_BASE);

            cy_crypto_serverContext->isHwErrorOccured = true;
        }
        else
        {
            /* HW error is already captured by Cy_Crypto_Core_CheckHwForErrors function */
            cy_crypto_serverContext->isHwErrorOccured = false;
        }
    }
}

void Cy_Crypto_Server_Process(void)
{
    cy_stc_crypto_context_t* myData = processData;

    if (myData != NULL)
    {
        /* Default error */
        myData->resp = CY_CRYPTO_HW_NOT_ENABLED;

        if (CY_CRYPTO_INSTR_ENABLE == myData->instr)
        {
            myData->resp = Cy_Crypto_Core_Enable(CY_CRYPTO_BASE);
        }
        else
        {
            /* Check if Crypto HW is enabled */
            if (Cy_Crypto_Core_IsEnabled(CY_CRYPTO_BASE))
            {
                myData->resp = CY_CRYPTO_NOT_SUPPORTED;

                if (NULL != cy_CryptoFunctionTable)
                {
                    switch(myData->instr)
                    {
                    case CY_CRYPTO_INSTR_DISABLE:
                         myData->resp = Cy_Crypto_Core_Disable(CY_CRYPTO_BASE);
                        break;

                    case CY_CRYPTO_INSTR_SRV_INFO:
                         myData->resp = Cy_Crypto_Core_GetLibInfo((cy_en_crypto_lib_info_t*)myData->xdata);
                        break;

                    /* MEM_BUFF memory management */
                    case CY_CRYPTO_INSTR_MEMBUF_SET:
                        {
                            cy_stc_crypto_context_str_t *cfContext = myData->xdata;
                            myData->resp = Cy_Crypto_Core_SetVuMemoryAddress(CY_CRYPTO_BASE, cfContext->src0, cfContext->dataSize);
                        }
                        break;

                    case CY_CRYPTO_INSTR_MEMBUF_ADDR:
                        {
                            cy_stc_crypto_context_str_t *cfContext = myData->xdata;
                            *(uint32_t *)(cfContext->dst) = (uint32_t)Cy_Crypto_Core_GetVuMemoryAddress(CY_CRYPTO_BASE);
                            myData->resp = CY_CRYPTO_SUCCESS;
                        }
                        break;

                    case CY_CRYPTO_INSTR_MEMBUF_SIZE:
                        {
                            cy_stc_crypto_context_str_t *cfContext = myData->xdata;
                            *(uint32_t *)(cfContext->dst) = Cy_Crypto_Core_GetVuMemorySize(CY_CRYPTO_BASE);
                            myData->resp = CY_CRYPTO_SUCCESS;
                        }
                        break;

                    case CY_CRYPTO_INSTR_PRNG_INIT:
                        if (NULL != cy_CryptoFunctionTable->prngInitFunc)
                        {
                            cy_stc_crypto_context_prng_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->prngInitFunc)(CY_CRYPTO_BASE,
                                cfContext->lfsr32InitState, cfContext->lfsr31InitState, cfContext->lfsr29InitState);
                        }
                        break;

                    case CY_CRYPTO_INSTR_PRNG:
                        if (NULL != cy_CryptoFunctionTable->prngFunc)
                        {
                            cy_stc_crypto_context_prng_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->prngFunc)(CY_CRYPTO_BASE,
                                cfContext->max, cfContext->prngNum);
                        }
                        break;

                    case CY_CRYPTO_INSTR_TRNG:
                        if (NULL != cy_CryptoFunctionTable->trngFunc)
                        {
                            cy_stc_crypto_context_trng_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->trngFunc)(CY_CRYPTO_BASE,
                                cfContext->GAROPol, cfContext->FIROPol, cfContext->max, cfContext->trngNum);
                        }
                        break;

                    case CY_CRYPTO_INSTR_AES_INIT:
                        if (NULL != cy_CryptoFunctionTable->aesInitFunc)
                        {
                            cy_stc_crypto_context_aes_t *cfContext = (cy_stc_crypto_context_aes_t *)myData->xdata;
                            myData->resp =
                                (cy_CryptoFunctionTable->aesInitFunc)(CY_CRYPTO_BASE,
                                    (uint8_t*)cfContext->key, cfContext->keyLength, &cfContext->aesState, (cy_stc_crypto_aes_buffers_t *)(Cy_Crypto_Core_GetVuMemoryAddress(CY_CRYPTO_BASE)));
                        }
                        break;

                    case CY_CRYPTO_INSTR_AES_ECB:
                        if (NULL != cy_CryptoFunctionTable->aesEcbFunc)
                        {
                            cy_stc_crypto_context_aes_t *cfContext = (cy_stc_crypto_context_aes_t *)myData->xdata;
                            myData->resp =
                                (cy_CryptoFunctionTable->aesEcbFunc)(CY_CRYPTO_BASE,
                                cfContext->dirMode,
                                (uint8_t*)cfContext->dst,
                                (uint8_t*)cfContext->src,
                                &cfContext->aesState);
                        }
                        break;

                    case CY_CRYPTO_INSTR_AES_CBC:
                        if (NULL != cy_CryptoFunctionTable->aesCbcFunc)
                        {
                            cy_stc_crypto_context_aes_t *cfContext = (cy_stc_crypto_context_aes_t *)myData->xdata;
                            myData->resp =
                                (cy_CryptoFunctionTable->aesCbcFunc)(CY_CRYPTO_BASE,
                                    cfContext->dirMode,
                                    cfContext->srcSize,
                                    (uint8_t*)cfContext->ivPtr,
                                    (uint8_t*)cfContext->dst,
                                    (uint8_t*)cfContext->src,
                                    &cfContext->aesState);
                        }
                        break;

                    case CY_CRYPTO_INSTR_AES_CFB:
                        if (NULL != cy_CryptoFunctionTable->aesCfbFunc)
                        {
                            cy_stc_crypto_context_aes_t *cfContext = (cy_stc_crypto_context_aes_t *)myData->xdata;
                            myData->resp =
                                (cy_CryptoFunctionTable->aesCfbFunc)(CY_CRYPTO_BASE,
                                    cfContext->dirMode,
                                    cfContext->srcSize,
                                    (uint8_t*)cfContext->ivPtr,
                                    (uint8_t*)cfContext->dst,
                                    (uint8_t*)cfContext->src,
                                    &cfContext->aesState);
                        }
                        break;

                    case CY_CRYPTO_INSTR_AES_CTR:
                        if (NULL != cy_CryptoFunctionTable->aesCtrFunc)
                        {
                            cy_stc_crypto_context_aes_t *cfContext = (cy_stc_crypto_context_aes_t *)myData->xdata;
                            myData->resp =
                                (cy_CryptoFunctionTable->aesCtrFunc)(CY_CRYPTO_BASE,
                                    cfContext->srcSize,
                                    cfContext->srcOffset,
                                    (uint8_t*)cfContext->ivPtr,
                                    (uint8_t*)cfContext->streamBlock,
                                    (uint8_t*)cfContext->dst,
                                    (uint8_t*)cfContext->src,
                                    &cfContext->aesState);
                        }
                        break;

                    case CY_CRYPTO_INSTR_CMAC:
                        if (NULL != cy_CryptoFunctionTable->cmacFunc)
                        {
                            cy_stc_crypto_context_aes_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->cmacFunc)(CY_CRYPTO_BASE,
                                (uint8_t*)cfContext->src, cfContext->srcSize, (uint8_t*)cfContext->key, cfContext->keyLength,
                                (uint8_t*)cfContext->dst, &cfContext->aesState);
                        }
                        break;

                    case CY_CRYPTO_INSTR_SHA:
                        if (NULL != cy_CryptoFunctionTable->shaFunc)
                        {
                            cy_stc_crypto_context_sha_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->shaFunc)(CY_CRYPTO_BASE,
                                (uint8_t *)cfContext->message, cfContext->messageSize,
                                (uint8_t *)cfContext->dst, cfContext->mode);
                        }
                        break;

                    case CY_CRYPTO_INSTR_HMAC:
                        if (NULL != cy_CryptoFunctionTable->hmacFunc)
                        {
                            cy_stc_crypto_context_sha_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->hmacFunc)(CY_CRYPTO_BASE,
                                (uint8_t *)cfContext->dst, (uint8_t *)cfContext->message, cfContext->messageSize,
                                (uint8_t *)cfContext->key, cfContext->keyLength, cfContext->mode);
                        }
                        break;

                    case CY_CRYPTO_INSTR_MEM_CPY:
                        if (NULL != cy_CryptoFunctionTable->memCpyFunc)
                        {
                            cy_stc_crypto_context_str_t *cfContext = myData->xdata;
                            (cy_CryptoFunctionTable->memCpyFunc)(CY_CRYPTO_BASE,
                                cfContext->dst, cfContext->src0, (uint16_t)cfContext->dataSize);
                            myData->resp = CY_CRYPTO_SUCCESS;
                        }
                        break;

                    case CY_CRYPTO_INSTR_MEM_SET:
                        if (NULL != cy_CryptoFunctionTable->memSetFunc)
                        {
                            cy_stc_crypto_context_str_t *cfContext = myData->xdata;
                            (cy_CryptoFunctionTable->memSetFunc)(CY_CRYPTO_BASE,
                                cfContext->dst, (uint8_t)cfContext->data, (uint16_t)cfContext->dataSize);
                            myData->resp = CY_CRYPTO_SUCCESS;
                        }
                        break;

                    case CY_CRYPTO_INSTR_MEM_CMP:
                        if (NULL != cy_CryptoFunctionTable->memCmpFunc)
                        {
                            cy_stc_crypto_context_str_t *cfContext = myData->xdata;
                            *(uint32_t *)(cfContext->dst) =
                                (cy_CryptoFunctionTable->memCmpFunc)(CY_CRYPTO_BASE,
                                    cfContext->src0, cfContext->src1, (uint16_t)cfContext->dataSize);
                            myData->resp = CY_CRYPTO_SUCCESS;
                        }
                        break;

                    case CY_CRYPTO_INSTR_MEM_XOR:
                        if (NULL != cy_CryptoFunctionTable->memXorFunc)
                        {
                            cy_stc_crypto_context_str_t *cfContext = myData->xdata;
                            (cy_CryptoFunctionTable->memXorFunc)(CY_CRYPTO_BASE,
                                cfContext->dst, cfContext->src0, cfContext->src1, (uint16_t)cfContext->dataSize);
                            myData->resp = CY_CRYPTO_SUCCESS;
                        }
                        break;

                    case CY_CRYPTO_INSTR_CRC_INIT:
                        if (NULL != cy_CryptoFunctionTable->crcInitFunc)
                        {
                            cy_stc_crypto_context_crc_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->crcInitFunc)(CY_CRYPTO_BASE,
                                cfContext->polynomial, cfContext->dataReverse, cfContext->dataXor, cfContext->remReverse, cfContext->remXor);
                        }
                        break;

                    case CY_CRYPTO_INSTR_CRC:
                        if (NULL != cy_CryptoFunctionTable->crcFunc)
                        {
                            cy_stc_crypto_context_crc_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->crcFunc)(CY_CRYPTO_BASE,
                                cfContext->crc, cfContext->data, cfContext->dataSize, cfContext->lfsrInitState);
                        }
                        break;

                    case CY_CRYPTO_INSTR_DES:
                        if (NULL != cy_CryptoFunctionTable->desFunc)
                        {
                            cy_stc_crypto_context_des_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->desFunc)(CY_CRYPTO_BASE,
                                cfContext->dirMode, (uint8_t const *)cfContext->key, (uint8_t *)cfContext->dst, (uint8_t const *)cfContext->src);
                        }
                        break;

                    case CY_CRYPTO_INSTR_3DES:
                        if (NULL != cy_CryptoFunctionTable->tdesFunc)
                        {
                            cy_stc_crypto_context_des_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->tdesFunc)(CY_CRYPTO_BASE,
                                cfContext->dirMode, (uint8_t const *)cfContext->key, (uint8_t *)cfContext->dst, (uint8_t  const *)cfContext->src);
                        }
                        break;

                    case CY_CRYPTO_INSTR_RSA_PROC:
                        if (NULL != cy_CryptoFunctionTable->rsaProcFunc)
                        {
                            cy_stc_crypto_context_rsa_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->rsaProcFunc)(CY_CRYPTO_BASE,
                                cfContext->key, (uint8_t const *)cfContext->message, cfContext->messageSize, (uint8_t *)cfContext->result);
                        }
                        break;

                    case CY_CRYPTO_INSTR_RSA_COEF:
                        if (NULL != cy_CryptoFunctionTable->rsaCoefFunc)
                        {
                            cy_stc_crypto_context_rsa_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->rsaCoefFunc)(CY_CRYPTO_BASE, cfContext->key);
                        }
                        break;

                    case CY_CRYPTO_INSTR_RSA_VER:
                        if (NULL != cy_CryptoFunctionTable->rsaVerifyFunc)
                        {
                            cy_stc_crypto_context_rsa_ver_t *cfContext = myData->xdata;
                            myData->resp = (cy_CryptoFunctionTable->rsaVerifyFunc)(CY_CRYPTO_BASE,
                                cfContext->verResult, cfContext->digestType, (uint8_t const *)cfContext->hash,
                                (uint8_t const *)cfContext->decryptedSignature, cfContext->decryptedSignatureLength);
                        }
                        break;

                    case CY_CRYPTO_INSTR_ECDSA_SIGN:
                        {
                            cy_stc_crypto_context_ecc_t *cfContext = myData->xdata;
                            myData->resp = Cy_Crypto_Core_ECC_SignHash(CY_CRYPTO_BASE,
                                cfContext->src0, cfContext->datalen, cfContext->dst0,
                                cfContext->key, cfContext->src1);
                        }
                        break;

                    case CY_CRYPTO_INSTR_ECDSA_VER:
                        {
                            cy_stc_crypto_context_ecc_t *cfContext = myData->xdata;
                            myData->resp = Cy_Crypto_Core_ECC_VerifyHash(CY_CRYPTO_BASE,
                                cfContext->src1, cfContext->src0, cfContext->datalen,
                                cfContext->dst0, cfContext->key);
                        }
                        break;

                    default:
                        myData->resp = CY_CRYPTO_NOT_SUPPORTED;
                        break;
                    }
                }

                if (CY_CRYPTO_SUCCESS == myData->resp)
                {
                    myData->resp = Cy_Crypto_Core_CheckHwForErrors(myData);
                }
            }
        }

        processData = NULL;

        /* Release the Crypto IPC channel with the Release interrupt */
        (void)Cy_IPC_Drv_LockRelease(Cy_IPC_Drv_GetIpcBaseAddress(myData->ipcChannel),
                (NULL != myData->userCompleteCallback) ? (1uL << myData->releaseNotifierChannel) : CY_IPC_NO_NOTIFICATION);
    }
}

void Cy_Crypto_Server_GetDataHandler(void)
{
    uint32_t interruptMasked;

    /*
     * Check that there is really the IPC Crypto Notify interrupt,
     * because the same line can be used for the IPC Crypto Release interrupt.
     */

    interruptMasked = Cy_IPC_Drv_ExtractAcquireMask(Cy_IPC_Drv_GetInterruptStatusMasked(
                                         Cy_IPC_Drv_GetIntrBaseAddr(cy_crypto_serverContext->acquireNotifierChannel)));

    if ((1uL << (cy_crypto_serverContext->ipcChannel)) == interruptMasked )
    {
        Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(cy_crypto_serverContext->acquireNotifierChannel), CY_IPC_NO_NOTIFICATION, interruptMasked);

        if(CY_IPC_DRV_SUCCESS == Cy_IPC_Drv_ReadMsgPtr(Cy_IPC_Drv_GetIpcBaseAddress(cy_crypto_serverContext->ipcChannel), (void**)&processData))
        {
            if (cy_crypto_serverContext->getDataHandlerPtr == NULL)
            {
                Cy_Crypto_Server_Process();
            }
        }
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_CheckHwForErrors
****************************************************************************//**
*
* The function checks if a Crypto HW error occurred. If yes, copies the error
* information from the Crypto registers to the communication structure.
*
* This function available for CM0+ core only.
*
* This function is internal and should not to be called directly by user software
*
* \param cryptoContext
* The pointer to cy_stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
static cy_en_crypto_status_t Cy_Crypto_Core_CheckHwForErrors(cy_stc_crypto_context_t *cryptoContext)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_SUCCESS;
    uint32_t myErrorStatus0;
    uint32_t myErrorStatus1;

    CY_ASSERT(NULL != cryptoContext);

    if (false == cy_crypto_serverContext->isHwErrorOccured)
    {
        /* HW error is not captured before */
        myErrorStatus0 = REG_CRYPTO_ERROR_STATUS0(CY_CRYPTO_BASE);
        myErrorStatus1 = REG_CRYPTO_ERROR_STATUS1(CY_CRYPTO_BASE);

        if (_FLD2VAL(CRYPTO_ERROR_STATUS1_VALID, myErrorStatus1) == 1u)
        {
            tmpResult = CY_CRYPTO_HW_ERROR;

            cy_crypto_serverContext->isHwErrorOccured = true;
        }
    }
    else
    {
        /* HW error is already captured by error interrupt handler */
        myErrorStatus0 = cy_crypto_serverContext->hwErrorStatus.errorStatus0;
        myErrorStatus1 = cy_crypto_serverContext->hwErrorStatus.errorStatus1;

        if (_FLD2VAL(CRYPTO_ERROR_STATUS1_VALID, myErrorStatus1) == 1u)
        {
            tmpResult = CY_CRYPTO_HW_ERROR;
        }

        cy_crypto_serverContext->isHwErrorOccured = false;
    }

    cryptoContext->hwErrorStatus.errorStatus0 = myErrorStatus0;
    cryptoContext->hwErrorStatus.errorStatus1 = myErrorStatus1;

    return (tmpResult);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */
