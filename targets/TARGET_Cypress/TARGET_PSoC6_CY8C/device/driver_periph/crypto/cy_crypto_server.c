/***************************************************************************//**
* \file cy_crypto_server.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for Crypto Server
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_device_headers.h"
#include "ipc/cy_ipc_drv.h"
#include "sysint/cy_sysint.h"
#include "syslib/cy_syslib.h"
#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_server.h"
#include "crypto/cy_crypto_core_aes.h"
#include "crypto/cy_crypto_core_sha.h"
#include "crypto/cy_crypto_core_hmac.h"
#include "crypto/cy_crypto_core_cmac.h"
#include "crypto/cy_crypto_core_prng.h"
#include "crypto/cy_crypto_core_trng.h"
#include "crypto/cy_crypto_core_rsa.h"
#include "crypto/cy_crypto_core_str.h"
#include "crypto/cy_crypto_core_crc.h"
#include "crypto/cy_crypto_core_des.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_instructions.h"
#include <stdbool.h>


#if (CY_CPU_CORTEX_M0P)

#if (CPUSS_CRYPTO_PRESENT == 1)


/* The number of the interrupt to catch Crypto HW errors */
IRQn_CM0P_Type cy_crypto_ErrorIrqNum;

/*
 * The global variables to store a pointers on the customer callback functions.
 * This variable is global because it is called from an interrupt.
 */
cy_israddress cy_cryptoGetDataHandlerPtr = NULL;
cy_israddress cy_cryptoErrorHandlerPtr = NULL;

/*
 * The global variable to store a pointer to crypto processing context data.
 */
cy_stc_crypto_context_t *processData = NULL;

/* Functions Prototypes */
void Cy_Crypto_Server_PopulateHwErrInfo(cy_stc_crypto_context_t *cryptoContextPtr);
void Cy_Crypto_Server_Run(cy_stc_crypto_context_t* data);
void Cy_Crypto_Server_Invoke(void);
void Cy_Crypto_Server_ErrorHandler(void);

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Start
****************************************************************************//**
*
* Starts the Crypto server on the M0 core, sets up an interrupt
* for the IPC Crypto channel on the M0 core, sets up an interrupt
* to catch Crypto HW errors. Should be invoked only on CM0.
*
* This function available for CM0+ core only.
*
* \param configStruct
* The Crypto configuration structure.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Server_Start(cy_stc_crypto_config_t const *configStruct)
{
    /* Initialize the interrupt controller for M0 and IPC Interrupt */
    cy_stc_sysint_t intrCfg;

    intrCfg.intrSrc = (IRQn_Type)(cpuss_interrupts_ipc0_IRQn + CY_CRYPTO_NOTIFY_INTR);

    intrCfg.intrCm0p = (IRQn_CM0P_Type)(configStruct->cm0NotifyIntrNum);

    intrCfg.intrPriority = configStruct->notifyIntrPrior;

    if (configStruct->userGetDataHandler != NULL)
    {
        Cy_SysInt_Init(&intrCfg, configStruct->userGetDataHandler);
        cy_cryptoGetDataHandlerPtr = configStruct->userGetDataHandler;
    }
    else
    {
        Cy_SysInt_Init(&intrCfg, &Cy_Crypto_Server_GetDataHandler);
    }

    NVIC_EnableIRQ((IRQn_Type)intrCfg.intrCm0p);

    /* Do not bring up an IPC release interrupt here, only set up a notify interrupt */
    Cy_IPC_DRV_SetIntrMask(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_NOTIFY_INTR), CY_IPC_NO_NOTIFIFICATION, (1uL << CY_IPC_CHAN_CRYPTO));

    /* Initialize and enable an interrupt to handle possible Crypto HW errors */
    intrCfg.intrSrc = (IRQn_Type)(cpuss_interrupt_crypto_IRQn);

    /* Initialize an interrupt to check a Crypto HW error*/
    cy_crypto_ErrorIrqNum = (IRQn_CM0P_Type)(configStruct->cm0CryptoErrorIntrNum);

    intrCfg.intrCm0p = cy_crypto_ErrorIrqNum;

    intrCfg.intrPriority = configStruct->cryptoErrorIntrPrior;

    if (configStruct->userErrorHandler != NULL)
    {
        Cy_SysInt_Init(&intrCfg, configStruct->userErrorHandler);
        cy_cryptoErrorHandlerPtr = configStruct->userErrorHandler;
    }
    else
    {
        Cy_SysInt_Init(&intrCfg, &Cy_Crypto_Server_ErrorHandler);
    }

    NVIC_ClearPendingIRQ((IRQn_Type)intrCfg.intrCm0p);
    NVIC_EnableIRQ((IRQn_Type)intrCfg.intrCm0p);

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Stop
****************************************************************************//**
*
* Stops the Crypto server by disabling the IPC notify interrupt
* and Crypto error interrupt. Should be invoked only on CM0.
*
* This function available for CM0+ core only.
*
* \param configStruct
* The Crypto configuration structure.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Server_Stop(cy_stc_crypto_config_t const *configStruct)
{
    uint32_t interruptMask;

    /* Disable the Notify interrupt from IPC */
    NVIC_DisableIRQ((IRQn_Type)configStruct->cm0NotifyIntrNum);
    NVIC_ClearPendingIRQ((IRQn_Type)configStruct->cm0NotifyIntrNum);

    /* Disable the Error interrupt from Crypto */
    NVIC_DisableIRQ((IRQn_Type)configStruct->cm0CryptoErrorIntrNum);
    NVIC_ClearPendingIRQ((IRQn_Type)configStruct->cm0CryptoErrorIntrNum);

    /* Disable CRYPTO IPC interrupt */
    interruptMask = Cy_IPC_DRV_GetIntrMask(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_NOTIFY_INTR));

    Cy_IPC_DRV_SetIntrMask(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_NOTIFY_INTR), CY_IPC_NO_NOTIFIFICATION, interruptMask & ~(1uL << CY_IPC_CHAN_CRYPTO));

    cy_cryptoGetDataHandlerPtr = NULL;
    cy_cryptoErrorHandlerPtr = NULL;

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Server_ErrorHandler
****************************************************************************//**
*
* The routine to handle an interrupt caused by the Crypto hardware error.
*
* This function available for CM0+ core only.
*
*******************************************************************************/
void Cy_Crypto_Server_ErrorHandler(void)
{
    NVIC_ClearPendingIRQ((IRQn_Type)cy_crypto_ErrorIrqNum);
    cy_crypto_IsHwErrorOccured = true;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Process
****************************************************************************//**
*
* Parses input data received from the Crypto Client
* and runs the appropriate Crypto function.
*
* This function available for CM0+ core only.
*
*******************************************************************************/
void Cy_Crypto_Server_Process(void)
{
    cy_stc_crypto_context_t* data = processData;

    if (data != NULL)
    {
        /* Default error */
        data->resp = CY_CRYPTO_HW_NOT_ENABLED;

        if (CY_CRYPTO_INSTR_ENABLE == data->instr)
        {
            data->resp = Cy_Crypto_Core_Enable();
        }
        else
        {
            /* Check if Crypto HW is enabled */
            if(1uL == (uint32_t)_FLD2VAL(CRYPTO_CTL_ENABLED, CRYPTO->CTL))
            {
                data->resp = CY_CRYPTO_SUCCESS;

                switch(data->instr)
                {
                case CY_CRYPTO_INSTR_DISABLE:
                     data->resp = Cy_Crypto_Core_Disable();
                    break;

#if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1))
                case CY_CRYPTO_INSTR_PRNG_INIT:
                     data->resp = Cy_Crypto_Core_Prng_Init((cy_stc_crypto_context_prng_t*)data->xdata);
                    break;

                case CY_CRYPTO_INSTR_PRNG:
                     data->resp = Cy_Crypto_Core_Prng((cy_stc_crypto_context_prng_t*)data->xdata);
                    break;
#endif /* #if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1)) */

#if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1))
                case CY_CRYPTO_INSTR_TRNG:
                     data->resp = Cy_Crypto_Core_Trng((cy_stc_crypto_context_trng_t*)data->xdata);
                    break;
#endif /* #if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1)) */

#if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))
                case CY_CRYPTO_INSTR_AES_INIT:
                     data->resp = Cy_Crypto_Core_Aes_Init((cy_stc_crypto_context_aes_t *)data->xdata);
                    break;

#if (CY_CRYPTO_USER_AES_ECB == 1)
                case CY_CRYPTO_INSTR_AES_ECB:
                     data->resp = Cy_Crypto_Core_Aes_Ecb((cy_stc_crypto_context_aes_t *)data->xdata);
                    break;
#endif /* #if (CRYPTO_USER_AES_ECB == 1) */

#if (CY_CRYPTO_USER_AES_CBC == 1)
                case CY_CRYPTO_INSTR_AES_CBC:
                     data->resp = Cy_Crypto_Core_Aes_Cbc((cy_stc_crypto_context_aes_t *)data->xdata);
                    break;
#endif /* #if (CRYPTO_USER_AES_CBC == 1) */

#if (CY_CRYPTO_USER_AES_CFB == 1)
                case CY_CRYPTO_INSTR_AES_CFB:
                     data->resp = Cy_Crypto_Core_Aes_Cfb((cy_stc_crypto_context_aes_t *)data->xdata);
                    break;
#endif /* #if (CRYPTO_USER_AES_CFB == 1) */

#if (CY_CRYPTO_USER_AES_CTR == 1)
                case CY_CRYPTO_INSTR_AES_CTR:
                     data->resp = Cy_Crypto_Core_Aes_Ctr((cy_stc_crypto_context_aes_t *)data->xdata);
                    break;
#endif /* #if (CRYPTO_USER_AES_CTR == 1) */

#if (CY_CRYPTO_USER_CMAC == 1)
                case CY_CRYPTO_INSTR_CMAC:
                     data->resp = Cy_Crypto_Core_Cmac((cy_stc_crypto_context_aes_t *)data->xdata);
                    break;
#endif /* #if (CRYPTO_USER_CMAC == 1) */

#endif /* #if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))
                case CY_CRYPTO_INSTR_SHA:
                     data->resp = Cy_Crypto_Core_Sha((cy_stc_crypto_context_sha_t *)data->xdata);
                    break;
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1))
                case CY_CRYPTO_INSTR_HMAC:
                     data->resp = Cy_Crypto_Core_Hmac((cy_stc_crypto_context_sha_t *)data->xdata);
                    break;
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1)) */

#if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1))
                case CY_CRYPTO_INSTR_MEM_CPY:
                    Cy_Crypto_Core_Str_MemCpy(((cy_stc_crypto_context_str_t* )data->xdata)->dstPtr,
                                             ((cy_stc_crypto_context_str_t* )data->xdata)->srcPtr0,
                                             (uint16_t)((cy_stc_crypto_context_str_t* )data->xdata)->dataSize );
                    break;

                case CY_CRYPTO_INSTR_MEM_SET:
                    Cy_Crypto_Core_Str_MemSet(((cy_stc_crypto_context_str_t*)data->xdata)->dstPtr,
                                             ((cy_stc_crypto_context_str_t*)data->xdata)->data,
                                             (uint16_t)(((cy_stc_crypto_context_str_t*)data->xdata)->dataSize) );
                    break;

                case CY_CRYPTO_INSTR_MEM_CMP:
                    *(uint32_t* )(((cy_stc_crypto_context_str_t* )data->xdata)->dstPtr) =
                            Cy_Crypto_Core_Str_MemCmp(((cy_stc_crypto_context_str_t* )data->xdata)->srcPtr0,
                                                     ((cy_stc_crypto_context_str_t* )data->xdata)->srcPtr1,
                                                     (uint16_t)((cy_stc_crypto_context_str_t* )data->xdata)->dataSize );
                    break;

                case CY_CRYPTO_INSTR_MEM_XOR:
                    Cy_Crypto_Core_Str_MemXor(((cy_stc_crypto_context_str_t* )data->xdata)->srcPtr0,
                                             ((cy_stc_crypto_context_str_t* )data->xdata)->srcPtr1,
                                             ((cy_stc_crypto_context_str_t* )data->xdata)->dstPtr,
                                             (uint16_t)((cy_stc_crypto_context_str_t* )data->xdata)->dataSize );
                    break;
#endif /* #if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1)) */

#if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1))
                case CY_CRYPTO_INSTR_CRC_INIT:
                    data->resp = Cy_Crypto_Core_Crc_Init((cy_stc_crypto_context_crc_t*)data->xdata);
                    break;

                case CY_CRYPTO_INSTR_CRC:
                    data->resp = Cy_Crypto_Core_Crc((cy_stc_crypto_context_crc_t*)data->xdata);
                    break;
#endif /* #if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1)) */

#if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1))
                case CY_CRYPTO_INSTR_DES:
                    data->resp = Cy_Crypto_Core_Des((cy_stc_crypto_context_des_t *)data->xdata);
                    break;

                case CY_CRYPTO_INSTR_3DES:
                    data->resp = Cy_Crypto_Core_Tdes((cy_stc_crypto_context_des_t *)data->xdata);
                    break;
#endif /* #if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1)) */

#if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))
                case CY_CRYPTO_INSTR_RSA_PROC:
                    data->resp = Cy_Crypto_Core_Rsa_Proc((cy_stc_crypto_context_rsa_t *)data->xdata);
                    break;

                case CY_CRYPTO_INSTR_RSA_COEF:
                    data->resp = Cy_Crypto_Core_Rsa_Coef((cy_stc_crypto_context_rsa_t *)data->xdata);
                    break;
#endif /* #if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#if (CY_CRYPTO_USER_PKCS1_5 == 1)
                case CY_CRYPTO_INSTR_RSA_VER:
                    data->resp = Cy_Crypto_Core_RsaVerify((cy_stc_crypto_context_rsa_ver_t *)data->xdata);
                    break;
#endif /* #if (CY_CRYPTO_USER_PKCS1_5 == 1) */

                default:
                    data->resp = CY_CRYPTO_NOT_SUPPORTED;
                    break;
                }

                if (CY_CRYPTO_SUCCESS == data->resp)
                {
                    data->resp = Cy_Crypto_Core_CheckHwForErrors(data);
                }
            }
        }
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Server_GetDataHandler
****************************************************************************//**
*
* The IPC Crypto channel notify interrupt-routine,
* receives information from the Crypto client, runs the process
* (if user not setup own handler),
* releases the Crypto IPC channel after the Crypto server completes.
*
* This function available for CM0+ core only.
*
*******************************************************************************/
void Cy_Crypto_Server_GetDataHandler()
{
    uint32_t interruptMasked;

    interruptMasked = Cy_IPC_DRV_GetIntrStatusMasked(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_NOTIFY_INTR));

    /*
     * Check that there is really the IPC Crypto Notify interrupt,
     * because the same line can be used for the IPC Crypto Release interrupt.
     */
    if((1uL << (CY_IPC_CHAN_CRYPTO + IPC_INTR_STRUCT_INTR_MASKED_NOTIFY_Pos)) == (interruptMasked & IPC_INTR_STRUCT_INTR_MASKED_NOTIFY_Msk))
    {
        Cy_IPC_DRV_ClearIntr(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_NOTIFY_INTR), CY_IPC_NO_NOTIFIFICATION, interruptMasked >> IPC_INTR_STRUCT_INTR_MASKED_NOTIFY_Pos);

        if(CY_IPC_DRV_SUCCESS == Cy_IPC_DRV_ReadMsgPtr(Cy_IPC_DRV_GetIpcBaseAddress(CY_IPC_CHAN_CRYPTO), (void**)&processData))
        {
            if (cy_cryptoGetDataHandlerPtr == NULL)
            {
                Cy_Crypto_Server_Process();
            }

            /* Release the Crypto IPC channel with the Release interrupt */
            (void)Cy_IPC_DRV_Release(Cy_IPC_DRV_GetIpcBaseAddress(CY_IPC_CHAN_CRYPTO), (1uL << CY_CRYPTO_RELEASE_INTR));
        }
    }
}

#endif /* #if (CPUSS_CRYPTO_PRESENT == 1) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
