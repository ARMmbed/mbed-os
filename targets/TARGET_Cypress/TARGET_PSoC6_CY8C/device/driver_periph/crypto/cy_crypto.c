/***************************************************************************//**
* \file cy_crypto.c
* \version 1.0
*
* \brief
*  Provides API implementation of the Cypress PDL Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto.h"
#include "crypto/cy_crypto_common.h"
#include "ipc/cy_ipc_drv.h"
#include "sysint/cy_sysint.h"
#include "syslib/cy_syslib.h"


#if (CPUSS_CRYPTO_PRESENT == 1)


#if defined(__cplusplus)
extern "C" {
#endif

/* The function prototypes */
void Cy_Crypto_Client_ReleaseIntrHndlr(void);
cy_en_crypto_status_t Cy_Crypto_Client_Send(cy_stc_crypto_context_t const *cryptoContext);


/*
 * The global variable to store a pointer on the customer callback function.
 * This variable is global because it is called from an interrupt.
 */
cy_crypto_callback_ptr_t cy_cryptoCompleteCallbackPtr = NULL;

/*******************************************************************************
* Function Name: Cy_Crypto_Client_ReleaseIntrHndlr
****************************************************************************//**
*
* The interrupt handler for the Crypto IPC Release interrupt; happens
* when Crypto hardware completes operation. This function clears the specific
* interrupt source and calls the customer interrupt handler.
*
*******************************************************************************/
void Cy_Crypto_Client_ReleaseIntrHndlr(void)
{
    uint32_t interruptMasked;

    interruptMasked = Cy_IPC_DRV_GetIntrStatusMasked(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_RELEASE_INTR));

    /* Check that there is really the IPC Crypto Release interrupt */
    if((1uL << CY_IPC_CHAN_CRYPTO) == (interruptMasked & IPC_INTR_STRUCT_INTR_MASKED_RELEASE_Msk))
    {
        Cy_IPC_DRV_ClearIntr(Cy_IPC_DRV_GetIntrBaseAddr(CY_IPC_INTR_CRYPTO_M4),
                             (interruptMasked & IPC_INTR_STRUCT_INTR_MASKED_RELEASE_Msk), CY_IPC_NO_NOTIFIFICATION);

        if (cy_cryptoCompleteCallbackPtr != NULL)
        {
            cy_cryptoCompleteCallbackPtr();
        }
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_IsServerStarted
****************************************************************************//**
*
* Checks if CryptoServer is started.
*
* \return
* True  - CryptoServer is started.
* False - CryptoServer is not started.
*
*******************************************************************************/
bool Cy_Crypto_IsServerStarted(void)
{
    /* Check if the Crypto server started (Crypto IPC Notify interrupt is started) */
    return ((1uL << (CY_IPC_CHAN_CRYPTO + IPC_INTR_STRUCT_INTR_MASK_NOTIFY_Pos)) ==
            (Cy_IPC_DRV_GetIntrMask(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_NOTIFY_INTR)) & IPC_INTR_STRUCT_INTR_MASK_NOTIFY_Msk));
}

/*******************************************************************************
* Function Name: Cy_Crypto_IsServerReady
****************************************************************************//**
*
* Checks if CryptoServer is ready for operations.
*
* \return
* True  - CryptoServer is ready.
* False - CryptoServer is not ready.
*
*******************************************************************************/
bool Cy_Crypto_IsServerReady(void)
{
    return (CY_IPC_DRV_LOCKED != Cy_IPC_DRV_GetLockStatus(Cy_IPC_DRV_GetIpcBaseAddress(CY_IPC_CHAN_CRYPTO)));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Sync
****************************************************************************//**
*
* The synchronization function that waits or just checks (depending on the parameter)
* for the Crypto operation to complete.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param isBlocking
* Set whether Crypto_Sync is blocking or not:
* True - is blocking.
* False - is not blocking.
*
* \return
* True  - Crypto HW is ready to new task.
* False - Crypto HW is busy.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Sync(cy_stc_crypto_context_t const *cryptoContext, bool isBlocking)
{
    cy_en_crypto_status_t status = CY_CRYPTO_NOT_INITIALIZED;

    if (CY_CRYPTO_INSTR_UNKNOWN != cryptoContext->instr)
    {
      status = CY_CRYPTO_SERVER_NOT_STARTED;

      /* Check if the Crypto server started (Crypto IPC Notify interrupt is started) */
      if (Cy_Crypto_IsServerStarted())
      {

        if (!isBlocking)
        {
            status = CY_CRYPTO_SERVER_BUSY;

            if (Cy_Crypto_IsServerReady())
            {
                status = cryptoContext->resp;
            }
        }
        else
        {
            while (!Cy_Crypto_IsServerReady())
            {
            }

            status = cryptoContext->resp;
        }
      }
    }

    return (status);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Client_Send
****************************************************************************//**
*
* Sends a pointer to the Crypto Server.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Client_Send(cy_stc_crypto_context_t const *cryptoContext)
{
    cy_en_crypto_status_t status = CY_CRYPTO_SERVER_NOT_STARTED;

    if (Cy_Crypto_IsServerStarted())
    {
      status = CY_CRYPTO_SERVER_BUSY;

      if (Cy_Crypto_IsServerReady())
      {
        status = CY_CRYPTO_SUCCESS;

        if (CY_IPC_DRV_SUCCESS != Cy_IPC_DRV_SendMsgPtr(Cy_IPC_DRV_GetIpcBaseAddress(CY_IPC_CHAN_CRYPTO), (1uL << CY_IPC_INTR_CRYPTO_M0), cryptoContext))
        {
            status = CY_CRYPTO_COMM_FAIL;
        }
      }
    }

    return (status);
}

/*******************************************************************************
* Function Name: Cy_Crypto_GetErrorStatus
****************************************************************************//**
*
* The function returns a cause of a Crypto hardware error.
* This function is independent of the Crypto previous state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param hwErrorCause
* \ref cy_stc_crypto_hw_error_t.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_GetErrorStatus(const cy_stc_crypto_context_t *cryptoContext,
                                               cy_stc_crypto_hw_error_t *hwErrorCause)
{
    if(NULL != hwErrorCause)
    {
        hwErrorCause->errorStatus0 = cryptoContext->hwErrorStatus.errorStatus0;
        hwErrorCause->errorStatus1 = cryptoContext->hwErrorStatus.errorStatus1;
    }

    return (cryptoContext->resp);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Init
****************************************************************************//**
*
* Initializes the Crypto context buffer and
* interrupt for the Crypto callback. Must be called at first.
*
* To start working with Crypto methods after Crypto_Init(),
* call Crypto_Enable() to turn-on the Crypto Hardware.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param configStruct
* The pointer to the Crypto configuration structure.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Init(cy_stc_crypto_context_t *cryptoContext,
                                     cy_stc_crypto_config_t const *configStruct)
{
    cy_stc_sysint_t intrCfg;
    cy_en_crypto_status_t status = CY_CRYPTO_SERVER_NOT_STARTED;

    /* Release the Crypto IPC channel with the Release interrupt */
    (void)Cy_IPC_DRV_Release(Cy_IPC_DRV_GetIpcBaseAddress(CY_IPC_CHAN_CRYPTO), CY_IPC_NO_NOTIFIFICATION);

    cryptoContext->instr = CY_CRYPTO_INSTR_ENABLE;

    /* Check if the Crypto server started (Crypto IPC Notify interrupt is started) */
    if (Cy_Crypto_IsServerStarted())
    {
        /* Set up a Release interrupt if the customer wants */
        if(NULL != configStruct->userCompleteCallback)
        {
            intrCfg.intrSrc = (IRQn_Type)(cpuss_interrupts_ipc0_IRQn + CY_CRYPTO_RELEASE_INTR);
            intrCfg.intrCm0p = (IRQn_CM0P_Type)(configStruct->cm0ReleaseIntrNum);
            intrCfg.intrPriority = configStruct->releaseIntrPrior;
            Cy_SysInt_Init(&intrCfg, &Cy_Crypto_Client_ReleaseIntrHndlr);

            cy_cryptoCompleteCallbackPtr = configStruct->userCompleteCallback;

            /* Set up the IPC Release interrupt here */
            Cy_IPC_DRV_SetIntrMask(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_RELEASE_INTR), (1uL << CY_IPC_CHAN_CRYPTO), CY_IPC_NO_NOTIFIFICATION);

#if (CY_CPU_CORTEX_M0P)
            NVIC_EnableIRQ((IRQn_Type)intrCfg.intrCm0p);
#else
            NVIC_EnableIRQ((IRQn_Type)intrCfg.intrSrc);
#endif
        }

        status = CY_CRYPTO_SUCCESS;
    }

    return(status);
}

/*******************************************************************************
* Function Name: Cy_Crypto_DeInit
****************************************************************************//**
*
* The function to de-initialize the Crypto driver.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param configStruct
* The pointer to the Crypto configuration structure.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_DeInit(cy_stc_crypto_context_t *cryptoContext,
                                       cy_stc_crypto_config_t const *configStruct)
{
    uint32_t interruptMasked;
    cy_en_crypto_status_t err = CY_CRYPTO_SUCCESS;

    cryptoContext->instr = CY_CRYPTO_INSTR_UNKNOWN;

    /* If the release interrupt was enable, disable it here */
    if(NULL != configStruct->userCompleteCallback)
    {
        /* Disable the Release interrupt from IPC */
#if (CY_CPU_CORTEX_M0P)
        NVIC_DisableIRQ((IRQn_Type)configStruct->cm0ReleaseIntrNum);
#else
        NVIC_DisableIRQ((IRQn_Type)(cpuss_interrupts_ipc0_IRQn + CY_IPC_INTR_CRYPTO_M4));
#endif
        /* Reset up the IPC Release interrupt here */
        interruptMasked = Cy_IPC_DRV_GetIntrStatusMasked(Cy_IPC_DRV_GetIntrBaseAddr(CY_CRYPTO_RELEASE_INTR));

        /* Check that there is really the IPC Crypto Release interrupt */
        if((1uL << CY_IPC_CHAN_CRYPTO) == (interruptMasked & IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk))
        {
            Cy_IPC_DRV_ClearIntr(Cy_IPC_DRV_GetIntrBaseAddr(CY_IPC_INTR_CRYPTO_M4), (interruptMasked & IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk), CY_IPC_NO_NOTIFIFICATION);
        }
    }

    return (err);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Enable
****************************************************************************//**
*
* Enable the Crypo hardware.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Enable(cy_stc_crypto_context_t *cryptoContext)
{
    cy_en_crypto_status_t err;

    cryptoContext->instr = CY_CRYPTO_INSTR_ENABLE;
    cryptoContext->xdata = NULL;

    err = Cy_Crypto_Client_Send(cryptoContext);

    /* Wait until initialization completes */
    if (CY_CRYPTO_SUCCESS == err)
    {
      err = Cy_Crypto_Sync(cryptoContext, CY_CRYPTO_SYNC_BLOCKING);
    }

    return (err);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Disable
****************************************************************************//**
*
* Disables the Crypto hardware.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Disable(cy_stc_crypto_context_t *cryptoContext)
{
    cy_en_crypto_status_t err;

    cryptoContext->instr = CY_CRYPTO_INSTR_DISABLE;
    cryptoContext->xdata = NULL;

    err = Cy_Crypto_Client_Send(cryptoContext);

    /* Wait until initialization completes */
    if (CY_CRYPTO_SUCCESS == err)
    {
      err = Cy_Crypto_Sync(cryptoContext, CY_CRYPTO_SYNC_BLOCKING);
    }

   return (err);
}

#if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Prng_Init
****************************************************************************//**
*
* This function initializes parameters of the PRND.
* Resets the pseudo random sequence.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoPrngContext
* The pointer to the \ref cy_stc_crypto_context_prng_t structure which stores
* the Crypto function context.
*
* \param lfsr32InitState
* A non-zero seed value for the first LFSR.

* \param lfsr31InitState
* A non-zero seed value for the second LFSR.

* \param lfsr29InitState
* A non-zero seed value for the third LFSR
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Prng_Init(cy_stc_crypto_context_t *cryptoContext,
                                          cy_stc_crypto_context_prng_t *cryptoPrngContext,
                                          const uint32_t lfsr32InitState,
                                          const uint32_t lfsr31InitState,
                                          const uint32_t lfsr29InitState)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_PRNG_INIT;
    cryptoContext->xdata = (void *)cryptoPrngContext;

    cryptoPrngContext->lfsr32InitState = lfsr32InitState;
    cryptoPrngContext->lfsr31InitState = lfsr31InitState;
    cryptoPrngContext->lfsr29InitState = lfsr29InitState;

    return (Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Prng_Generate
****************************************************************************//**
*
* Generates the Pseudo Random Number.
* This function is dependent on Cy_Crypto_Prng_Init which should be called before.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoPrngContext
* The pointer to the \ref cy_stc_crypto_context_prng_t structure which stores
* the Crypto function context.
*
* \param max
* The maximum value of a random number.
*
* \param rndNumPtr
* The pointer to a generated pseudo random number.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Prng_Generate(cy_stc_crypto_context_t *cryptoContext,
                                              cy_stc_crypto_context_prng_t *cryptoPrngContext,
                                               uint32_t const max,
                                               uint32_t *rndNumPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_PRNG;
    cryptoContext->xdata = (void *)cryptoPrngContext;

    cryptoPrngContext->max = max;
    cryptoPrngContext->prngNumPtr = rndNumPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1)) */

#if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Aes_Init
****************************************************************************//**
*
* Initialize the AES operation by setting key and key length.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoAesContext
* The pointer to the \ref cy_stc_crypto_context_aes_t structure which stores all internal variables
* the Crypto driver requires.
*
* \param keyPtr
* The pointer to the encryption/decryption key.
*
* \param keyLength
* One of these: CRYPTO_AES_128, CRYPTO_AES_192 or CRYPTO_AES_256.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Aes_Init(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_aes_t *cryptoAesContext,
                                         uint32_t *keyPtr,
                                         cy_en_crypto_aes_key_length_t keyLength)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_AES_INIT;
    cryptoContext->xdata = cryptoAesContext;

    cryptoAesContext->aesState.keyPtr = keyPtr;
    cryptoAesContext->aesState.keyLength = keyLength;

    return (Cy_Crypto_Client_Send(cryptoContext));
}

#if (CY_CRYPTO_USER_AES_ECB == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Aes_Ecb_Run
****************************************************************************//**
*
* Performs AES operation on one Block.
* This function is dependent from Cy_Crypto_Aes_Init which should be called before.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoAesContext
* The pointer to the \ref cy_stc_crypto_context_aes_t structure which stores all internal variables
* the Crypto driver requires.
*
* \param dirMode
* Can be CRYPTO_ENCRYPT or CRYPTO_DECRYPT.
*
* \param srcBlockPtr
* The pointer to a source block.
*
* \param dstBlockPtr
* The pointer to a destination cipher block.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Aes_Ecb_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t *dstBlockPtr,
                                            uint32_t *srcBlockPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_AES_ECB;
    cryptoContext->xdata = cryptoAesContext;

    cryptoAesContext->dirMode = dirMode;
    cryptoAesContext->dstPtr = dstBlockPtr;
    cryptoAesContext->srcPtr = srcBlockPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if (CY_CRYPTO_USER_AES_ECB == 1) */

#if (CY_CRYPTO_USER_AES_CBC == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Aes_Cbc_Run
****************************************************************************//**
*
* Performs AES operation on a plain text with Cipher Block Chaining (CBC).
* This function is dependent from Cy_Crypto_Aes_Init which should be called before.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoAesContext
* The pointer to the \ref cy_stc_crypto_context_aes_t structure which stores all internal variables
* the Crypto driver requires.
*
* \param dirMode
* Can be CRYPTO_ENCRYPT or CRYPTO_DECRYPT.
*
* \param srcSize
* The size of the source plain text.
*
* \param ivPtr
* The pointer to the initial vector.
*
* \param dstPtr
* The pointer to a destination cipher text.
*
* \param srcPtr
* The pointer to a source plain text. Must be 4-Byte aligned!
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Aes_Cbc_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *ivPtr,
                                            uint32_t *dstPtr,
                                            uint32_t *srcPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_AES_CBC;
    cryptoContext->xdata = cryptoAesContext;

    cryptoAesContext->dirMode = dirMode;
    cryptoAesContext->srcSize = srcSize;
    cryptoAesContext->ivPtr = ivPtr;
    cryptoAesContext->dstPtr = dstPtr;
    cryptoAesContext->srcPtr = srcPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if (CY_CRYPTO_USER_AES_CBC == 1) */

#if (CY_CRYPTO_USER_AES_CFB == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Aes_Cfb_Run
****************************************************************************//**
*
* Performs AES operation on a plain text with Cipher Feedback mode (CFB).
* This function is dependent from Cy_Crypto_Aes_Init
* which should be called before.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoAesContext
* The pointer to the \ref cy_stc_crypto_context_aes_t structure which stores all internal variables
* the Crypto driver requires.
*
* \param dirMode
* Can be CRYPTO_ENCRYPT or CRYPTO_DECRYPT.
*
* \param srcSize
* The size of the source plain text.
*
* \param ivPtr
* The pointer to the initial vector.
*
* \param dstPtr
* The pointer to a destination cipher text.
*
* \param srcPtr
* The pointer to a source plain text. Must be 4-Byte aligned!
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Aes_Cfb_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *ivPtr,
                                            uint32_t *dstPtr,
                                            uint32_t *srcPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_AES_CFB;
    cryptoContext->xdata = cryptoAesContext;

    cryptoAesContext->dirMode = dirMode;
    cryptoAesContext->srcSize = srcSize;
    cryptoAesContext->ivPtr = ivPtr;
    cryptoAesContext->dstPtr = dstPtr;
    cryptoAesContext->srcPtr = srcPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if (CY_CRYPTO_USER_AES_CFB == 1) */

#if (CY_CRYPTO_USER_AES_CTR == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Aes_Ctr_Run
****************************************************************************//**
*
* Performs AES operation on a plain text with Cipher Block Counter mode (CTR).
* NOTE: preparation of the unique nonceCounter for each block is
* the user's responsibility. This function is dependent from
* Cy_Crypto_Aes_Init which should be called before.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoAesContext
* The pointer to the \ref cy_stc_crypto_context_aes_t structure which stores all internal variables
* the Crypto driver requires.
*
* \param dirMode
* Can be CRYPTO_ENCRYPT or CRYPTO_DECRYPT.
*
* \param srcSize
* The size of a source plain text.
*
* \param srcOffset
* The size of an offset within the current block stream for resuming within the current cipher stream.
*
* \param nonceCounter
* The 128-bit nonce and counter.
*
* \param streamBlock
* The saved stream-block for resuming, is over-written by the function.
*
* \param dstPtr
* The pointer to a destination cipher text.
*
* \param srcPtr
* The pointer to a source plain text. Must be 4-Byte aligned!
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Aes_Ctr_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *srcOffset,
                                            uint32_t nonceCounter[CY_CRYPTO_AES_BLOCK_SIZE / 8u],
                                            uint32_t streamBlock[CY_CRYPTO_AES_BLOCK_SIZE / 8u],
                                            uint32_t *dstPtr,
                                            uint32_t *srcPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_AES_CTR;
    cryptoContext->xdata = cryptoAesContext;

    cryptoAesContext->dirMode = dirMode;
    cryptoAesContext->srcSize = srcSize;
    cryptoAesContext->srcOffset = srcOffset;
    cryptoAesContext->ivPtr = nonceCounter;
    cryptoAesContext->streamBlock = streamBlock;
    cryptoAesContext->dstPtr = dstPtr;
    cryptoAesContext->srcPtr = srcPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if (CY_CRYPTO_USER_AES_CTR == 1) */

#if (CY_CRYPTO_USER_CMAC == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Aes_Cmac_Run
****************************************************************************//**
*
* Performs the cipher-block chaining-message authentication-code.
* This function is independent of the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoAesContext
* The pointer to the \ref cy_stc_crypto_context_aes_t structure which stores all internal variables
* the Crypto driver requires.
*
* \param srcPtr
* The pointer to a source plain text. Must be 4-Byte aligned!
*
* \param srcSize
* The size of a source plain text.
*
* \param keyPtr
* The pointer to the encryption key.
*
* \param keyLength
* One of these: \ref cy_en_crypto_aes_key_length_t
*
* \param cmacPtr
* The pointer to the calculated CMAC.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Aes_Cmac_Run(cy_stc_crypto_context_t *cryptoContext,
                                             cy_stc_crypto_context_aes_t *cryptoAesContext,
                                             uint32_t *srcPtr,
                                             uint32_t srcSize,
                                             uint32_t *keyPtr,
                                             cy_en_crypto_aes_key_length_t keyLength,
                                             uint32_t *cmacPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_CMAC;
    cryptoContext->xdata = cryptoAesContext;

    cryptoAesContext->srcSize = srcSize;
    cryptoAesContext->dstPtr = cmacPtr;
    cryptoAesContext->srcPtr = srcPtr;
    cryptoAesContext->aesState.keyPtr = keyPtr;
    cryptoAesContext->aesState.keyLength = keyLength;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if (CY_CRYPTO_USER_CMAC == 1) */

#endif /* #if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Sha_Run
****************************************************************************//**
*
* Performs the SHA Hash function.
* This function is independent of the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoShaContext
* the pointer to the \ref cy_stc_crypto_context_sha_t structure which stores all internal variables
* for Crypto driver.
*
* \param mode
* One of CRYPTO_SHA256, CRYPTO_SHA1, CRYPTO_SHA256_224, CRYPTO_SHA512,
*        CRYPTO_SHA384, CRYPTO_SHA512_224, CRYPTO_SHA512_256.
*
* \param messagePtr
* The pointer to a message whose hash value is being computed.
*
* \param messageSize
* The size of a message.
*
* \param digestPtr
* The pointer to the hash digest .
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Sha_Run(cy_stc_crypto_context_t *cryptoContext,
                                        cy_stc_crypto_context_sha_t *cryptoShaContext,
                                        uint32_t *messagePtr,
                                        uint32_t messageSize,
                                        uint32_t *digestPtr,
                                        cy_en_crypto_sha_mode_t mode)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_SHA;
    cryptoContext->xdata = cryptoShaContext;

    cryptoShaContext->messagePtr = messagePtr;
    cryptoShaContext->messageSize = messageSize;
    cryptoShaContext->dstPtr = digestPtr;
    cryptoShaContext->mode = mode;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Hmac_Run
****************************************************************************//**
*
* Performs HMAC calculation.
* This function is independent of the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoShaContext
* the pointer to the \ref cy_stc_crypto_context_sha_t structure which stores all internal variables
* for Crypto driver.
*
* \param hmacPtr
* The pointer to the calculated HMAC .
*
* \param messagePtr
* The pointer to a message whose hash value is being computed.
*
* \param messageSize
* The size of a message.
*
* \param keyPtr
* The pointer to the key.
*
* \param keyLength
* The length of the key.
*
* \param mode
* One of these: CRYPTO_SHA256, CRYPTO_SHA1, CRYPTO_SHA256_224, CRYPTO_SHA512,
*               CRYPTO_SHA384, CRYPTO_SHA512_224, CRYPTO_SHA512_256.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Hmac_Run(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_sha_t *cryptoShaContext,
                                         uint32_t *hmacPtr,
                                         uint32_t *messagePtr,
                                         uint32_t messageSize,
                                         uint32_t *keyPtr,
                                         uint32_t keyLength,
                                         cy_en_crypto_sha_mode_t mode)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_HMAC;
    cryptoContext->xdata = cryptoShaContext;

    cryptoShaContext->messagePtr = messagePtr;
    cryptoShaContext->messageSize = messageSize;
    cryptoShaContext->dstPtr = hmacPtr;
    cryptoShaContext->mode = mode;
    cryptoShaContext->keyPtr = keyPtr;
    cryptoShaContext->keyLength = keyLength;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1)) */

#if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Str_MemCpy
****************************************************************************//**
*
* Function MemCpy that uses Crypto HW.
* Memory structures should not overlap!
* There is no alignment restriction.
* This function is independent of the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoMemContext
* the pointer to the \ref cy_stc_crypto_context_str_t structure which stores all internal variables
* for Crypto driver.
*
* \param dstPtr
* The pointer to the destination of MemCpy.
*
* \param srcPtr
* The pointer to the source of MemCpy.
*
* \param size
* The size in bytes of the copy operation. Maximum size is 65535 Bytes.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Str_MemCpy(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void* dstPtr,
                                           void const *srcPtr,
                                           uint16_t size)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_MEM_CPY;
    cryptoContext->xdata = cryptoMemContext;

    cryptoMemContext->dstPtr = dstPtr;
    cryptoMemContext->srcPtr0 = srcPtr;
    cryptoMemContext->dataSize = size;

    return(Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Str_MemSet
****************************************************************************//**
*
* Function MemSet uses Crypto HW.
* There is no alignment restriction.
* This function is independent from the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoMemContext
* the pointer to the \ref cy_stc_crypto_context_str_t structure which stores all internal variables
* for Crypto driver.
*
* \param dstPtr
* The pointer to the destination of MemSet.
*
* \param data
* The value to be set.
*
* \param size
* The size in bytes of the set operation. Maximum size is 65535 Bytes.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Str_MemSet(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void* dstPtr,
                                           uint8_t data,
                                           uint16_t size)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_MEM_SET;
    cryptoContext->xdata = cryptoMemContext;

    cryptoMemContext->dstPtr = dstPtr;
    cryptoMemContext->data = data;
    cryptoMemContext->dataSize = size;

    return (Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Str_MemCmp
****************************************************************************//**
*
* Function MemCmp uses Crypto HW.
* There is no alignment restriction.
* This function is independent from the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoMemContext
* the pointer to the \ref cy_stc_crypto_context_str_t structure which stores all internal variables
* for Crypto driver.
*
* \param src0Ptr
* The pointer to the first source of MemCmp.
*
* \param src1Ptr
* The pointer to the second source of MemCmp.
*
* \param size
* The size in bytes of the compare operation. Maximum size is 65535 Bytes.
*
* \param resultPtr
* The pointer to the result: 0 - if Source 1 = Source 2, 1 - if not.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Str_MemCmp(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void const *src0Ptr,
                                           void const *src1Ptr,
                                           uint16_t size,
                                           uint32_t *resultPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_MEM_CMP;
    cryptoContext->xdata = cryptoMemContext;

    cryptoMemContext->srcPtr0 = src0Ptr;
    cryptoMemContext->srcPtr1 = src1Ptr;
    cryptoMemContext->dataSize = size;
    cryptoMemContext->dstPtr = (void* )resultPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Crypto_Str_MemXor
****************************************************************************//**
*
* Function MemXor uses Crypto HW.
* Memory structures should not overlap!
* There is no alignment restriction.
* This function is independent from the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoMemContext
* the pointer to the \ref cy_stc_crypto_context_str_t structure which stores all internal variables
* for Crypto driver.
*
* \param src0Ptr
* The pointer to the first source of MemXor.

* \param src1Ptr
* The pointer to the second source of MemXor.

* \param dstPtr
* The pointer to the destination of MemXor.
*
* \param size
* The size in bytes of the compare operation. Maximum size is 65535 Bytes.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Str_MemXor(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void const *src0Ptr,
                                           void const *src1Ptr,
                                           void* dstPtr,
                                           uint16_t size)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_MEM_XOR;
    cryptoContext->xdata = cryptoMemContext;

    cryptoMemContext->srcPtr0 = src0Ptr;
    cryptoMemContext->srcPtr1 = src1Ptr;
    cryptoMemContext->dstPtr = dstPtr;
    cryptoMemContext->dataSize = size;

    return(Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1)) */

#if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Crc_Init
****************************************************************************//**
*
* Performs CRC initialization.
* The peculiarity of CRC hardware block is that for some polynomials
* calculated CRC is MSB aligned, for another is LSB aligned.
* Here below is the table with knowing polynomials and their
* calculated CRCs from the string "123456789".
*
*  <table>
*  <caption id="crc_modes_table">CRC modes and parameters</caption>
*   <tr><th>Name</th><th>Width</th><th>Poly</th><th>Init</th><th>Data Rev</th><th>Data XOR</th><th>Rem Rev</th><th>Rem XOR</th><th>Expected CRC</th><th>Output of the CRC block</th>
*   </tr>
*   <tr>
*     <td>CRC-3 / ROHC</td><td>3</td><td>0x3</td><td>0x7</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x6</td><td>0x00000006</td>
*   </tr>
*   <tr>
*     <td>CRC-4 / ITU</td><td>4</td><td>0x3</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x7</td><td>0x00000007</td>
*   </tr>
*   <tr>
*     <td>CRC-5 / EPC</td><td>5</td><td>0x9</td><td>0x9</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x0</td><td>0x00000000</td>
*   </tr>
*   <tr>
*     <td>CRC-5 / ITU</td><td>5</td><td>0x15</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x7</td><td>0x00000007</td>
*   </tr>
*   <tr>
*     <td>CRC-5 / USB</td><td>5</td><td>0x5</td><td>0x1F</td><td>1</td><td>0</td><td>1</td><td>0x1F</td><td>0x19</td><td>0x00000019</td>
*   </tr>
*   <tr>
*     <td>CRC-6 / CDMA2000-A</td><td>6</td><td>0x27</td><td>0x3F</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0xD</td><td>0xD0000000</td>
*   </tr>
*   <tr>
*     <td>CRC-6 / CDMA2000-B</td><td>6</td><td>0x7</td><td>0x3F</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x3B</td><td>0x3B000000</td>
*   </tr>
*   <tr>
*     <td>CRC-6 / DARC</td><td>6</td><td>0x19</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x26</td><td>0x00000026</td>
*   </tr>
*   <tr>
*     <td>CRC-6 / ITU</td><td>6</td><td>0x3</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x6</td><td>0x00000006</td>
*   </tr>
*   <tr>
*     <td>CRC-7</td><td>7</td><td>0x9</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x75</td><td>0x75000000</td>
*   </tr>
*   <tr>
*     <td>CRC-7 / ROHC</td><td>7</td><td>0x4F</td><td>0x7F</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x53</td><td>0x00000053</td>
*   </tr>
*   <tr>
*     <td>CRC-8</td><td>8</td><td>0x7</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0xF4</td><td>0xF4000000</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / CDMA2000</td><td>8</td><td>0x9B</td><td>0xFF</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0xDA</td><td>0xDA000000</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / DARC</td><td>8</td><td>0x39</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x15</td><td>0x00000015</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / DVB-S2</td><td>8</td><td>0xD5</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0xBC</td><td>0xBC000000</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / EBU</td><td>8</td><td>0x1D</td><td>0xFF</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x97</td><td>0x00000097</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / I-CODE</td><td>8</td><td>0x1D</td><td>0xFD</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x7E</td><td>0x7E000000</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / ITU</td><td>8</td><td>0x7</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x55</td><td>0xA1</td><td>0xA1000000</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / MAXIM</td><td>8</td><td>0x31</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0xA1</td><td>0x000000A1</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / ROHC</td><td>8</td><td>0x7</td><td>0xFF</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0xD0</td><td>0x000000D0</td>
*   </tr>
*   <tr>
*     <td>CRC-8 / WCDMA</td><td>8</td><td>0x9B</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x25</td><td>0x00000025</td>
*   </tr>
*   <tr>
*     <td>CRC-10</td><td>10</td><td>0x233</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x199</td><td>0x19900000</td>
*   </tr>
*   <tr>
*     <td>CRC-10 / CDMA2000</td><td>10</td><td>0x3D9</td><td>0x3FF</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x233</td><td>0x23300000</td>
*   </tr>
*   <tr>
*     <td>CRC-11</td><td>11</td><td>0x385</td><td>0x1A</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x5A3</td><td>0x5A300000</td>
*   </tr>
*   <tr>
*     <td>CRC-12 / 3GPP</td><td>12</td><td>0x80F</td><td>0x0</td><td>0</td><td>0</td><td>1</td><td>0x0</td><td>0xDAF</td><td>0x00000DAF</td>
*   </tr>
*   <tr>
*     <td>CRC-12 / CDMA2000</td><td>12</td><td>0xF13</td><td>0xFFF</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0xD4D</td><td>0xD4D00000</td>
*   </tr>
*   <tr>
*     <td>CRC-12 / DECT</td><td>12</td><td>0x80F</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0xF5B</td><td>0xF5B00000</td>
*   </tr>
*   <tr>
*     <td>CRC-13 / BBC</td><td>13</td><td>0x1CF5</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x4FA</td><td>0x4FA00000</td>
*   </tr>
*   <tr>
*     <td>CRC-14 / DARC</td><td>14</td><td>0x805</td><td>0x0</td><td>1</td><td>0</td><td>1</td><td>0x0</td><td>0x82D</td><td>0x0000082D</td>
*   </tr>
*   <tr>
*     <td>CRC-15</td><td>15</td><td>0x4599</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x59E</td><td>0x59E00000</td>
*   </tr>
*   <tr>
*     <td>CRC-15 / MPT1327</td><td>15</td><td>0x6815</td><td>0x0</td><td>0</td><td>0</td><td>0</td><td>0x1</td><td>0x2566</td><td>0x25660000</td>
*   </tr>
*   <tr>
*     <td>CRC-24</td><td>24</td><td>0x0864CFB</td><td>0x00B704CE</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x21CF02</td><td>0x21CF0200</td>
*   </tr>
*   <tr>
*     <td>CRC-24 / FLEXRAY-A</td><td>24</td><td>0x05D6DCB</td><td>0x00FEDCBA</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x7979BD</td><td>0x7979BD00</td>
*   </tr>
*   <tr>
*     <td>CRC-24 / FLEXRAY-B</td><td>24</td><td>0x05D6DCB</td><td>0x00ABCDEF</td><td>0</td><td>0</td><td>0</td><td>0x0</td><td>0x1F23B8</td><td>0x1F23B800</td>
*   </tr>
*   <tr>
*     <td>CRC-31 / PHILIPS</td><td>31</td><td>0x4C11DB7</td><td>0x7FFFFFFF</td><td>0</td><td>0</td><td>0</td><td>0x7FFFFFFF</td><td>0xCE9E46C</td><td>0xCE9E46C0</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / ARC</td><td>16</td><td>0x8005</td><td>0x0000</td><td>1</td><td>0</td><td>1</td><td>0x0000</td><td>0xBB3D</td><td>0x0000BB3D</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / AUG-CCITT</td><td>16</td><td>0x1021</td><td>0x1D0F</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0xE5CC</td><td>0xE5CC0000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / BUYPASS</td><td>16</td><td>0x8005</td><td>0x0000</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0xFEE8</td><td>0xFEE80000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / CCITT-0</td><td>16</td><td>0x1021</td><td>0xFFFF</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0x29B1</td><td>0x29B10000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / CDMA2000</td><td>16</td><td>0xC867</td><td>0xFFFF</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0x4C06</td><td>0x4C060000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / DDS-110</td><td>16</td><td>0x8005</td><td>0x800D</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0x9ECF</td><td>0x9ECF0000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / DECT-R</td><td>16</td><td>0x0589</td><td>0x0000</td><td>0</td><td>0</td><td>0</td><td>0x0001</td><td>0x007E</td><td>0x007E0000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / DECT-X</td><td>16</td><td>0x0589</td><td>0x0000</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0x007F</td><td>0x007F0000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / DNP</td><td>16</td><td>0x3D65</td><td>0x0000</td><td>1</td><td>0</td><td>1</td><td>0xFFFF</td><td>0xEA82</td><td>0x0000EA82</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / EN-13757</td><td>16</td><td>0x3D65</td><td>0x0000</td><td>0</td><td>0</td><td>0</td><td>0xFFFF</td><td>0xC2B7</td><td>0xC2B70000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / GENIBUS</td><td>16</td><td>0x1021</td><td>0xFFFF</td><td>0</td><td>0</td><td>0</td><td>0xFFFF</td><td>0xD64E</td><td>0xD64E0000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / MAXIM</td><td>16</td><td>0x8005</td><td>0x0000</td><td>1</td><td>0</td><td>1</td><td>0xFFFF</td><td>0x44C2</td><td>0x000044C2</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / MCRF4XX</td><td>16</td><td>0x1021</td><td>0xFFFF</td><td>1</td><td>0</td><td>1</td><td>0x0000</td><td>0x6F91</td><td>0x00006F91</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / RIELLO</td><td>16</td><td>0x1021</td><td>0xB2AA</td><td>1</td><td>0</td><td>1</td><td>0x0000</td><td>0x63D0</td><td>0x000063D0</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / T10-DIF</td><td>16</td><td>0x8BB7</td><td>0x0000</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0xD0DB</td><td>0xD0DB0000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / TELEDISK</td><td>16</td><td>0xA097</td><td>0x0000</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0x0FB3</td><td>0x0FB30000</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / TMS37157</td><td>16</td><td>0x1021</td><td>0x89EC</td><td>1</td><td>0</td><td>1</td><td>0x0000</td><td>0x26B1</td><td>0x000026B1</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / USB</td><td>16</td><td>0x8005</td><td>0xFFFF</td><td>1</td><td>0</td><td>1</td><td>0xFFFF</td><td>0xB4C8</td><td>0x0000B4C8</td>
*   </tr>
*   <tr>
*     <td>CRC-A</td><td>16</td><td>0x1021</td><td>0xC6C6</td><td>1</td><td>0</td><td>1</td><td>0x0000</td><td>0xBF05</td><td>0x0000BF05</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / KERMIT</td><td>16</td><td>0x1021</td><td>0x0000</td><td>1</td><td>0</td><td>1</td><td>0x0000</td><td>0x2189</td><td>0x00002189</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / MODBUS</td><td>16</td><td>0x8005</td><td>0xFFFF</td><td>1</td><td>0</td><td>1</td><td>0x0000</td><td>0x4B37</td><td>0x00004B37</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / X-25</td><td>16</td><td>0x1021</td><td>0xFFFF</td><td>1</td><td>0</td><td>1</td><td>0xFFFF</td><td>0x906E</td><td>0x0000906E</td>
*   </tr>
*   <tr>
*     <td>CRC-16 / XMODEM</td><td>16</td><td>0x1021</td><td>0x0000</td><td>0</td><td>0</td><td>0</td><td>0x0000</td><td>0x31C3</td><td>0x31C30000</td>
*   </tr>
*   <tr>
*     <td>CRC-32</td><td>32</td><td>0x04C11DB7</td><td>0xFFFFFFFF</td><td>1</td><td>0</td><td>1</td><td>0xFFFFFFFF</td><td>0xCBF43926</td><td>0xCBF43926</td>
*   </tr>
*   <tr>
*     <td>CRC-32 / BZIP2</td><td>32</td><td>0x04C11DB7</td><td>0xFFFFFFFF</td><td>0</td><td>0</td><td>0</td><td>0xFFFFFFFF</td><td>0xFC891918</td><td>0xFC891918</td>
*   </tr>
*   <tr>
*     <td>CRC-32C</td><td>32</td><td>0x1EDC6F41</td><td>0xFFFFFFFF</td><td>1</td><td>0</td><td>1</td><td>0xFFFFFFFF</td><td>0xE3069283</td><td>0xE3069283</td>
*   </tr>
*   <tr>
*     <td>CRC-32D</td><td>32</td><td>0xA833982B</td><td>0xFFFFFFFF</td><td>1</td><td>0</td><td>1</td><td>0xFFFFFFFF</td><td>0x87315576</td><td>0x87315576</td>
*   </tr>
*   <tr>
*     <td>CRC-32 / MPEG-2</td><td>32</td><td>0x04C11DB7</td><td>0xFFFFFFFF</td><td>0</td><td>0</td><td>0</td><td>0x00000000</td><td>0x0376E6E7</td><td>0x0376E6E7</td>
*   </tr>
*   <tr>
*     <td>CRC-32 / POSIX</td><td>32</td><td>0x04C11DB7</td><td>0x00000000</td><td>0</td><td>0</td><td>0</td><td>0xFFFFFFFF</td><td>0x765E7680</td><td>0x765E7680</td>
*   </tr>
*   <tr>
*     <td>CRC-32Q</td><td>32</td><td>0x814141AB</td><td>0x00000000</td><td>0</td><td>0</td><td>0</td><td>0x00000000</td><td>0x3010BF7F</td><td>0x3010BF7F</td>
*   </tr>
*   <tr>
*     <td>CRC-32 / JAMCRC</td><td>32</td><td>0x04C11DB7</td><td>0xFFFFFFFF</td><td>1</td><td>0</td><td>1</td><td>0x00000000</td><td>0x340BC6D9</td><td>0x340BC6D9</td>
*   </tr>
*   <tr>
*     <td>CRC-32 / XFER</td><td>32</td><td>0x000000AF</td><td>0x00000000</td><td>0</td><td>0</td><td>0</td><td>0x00000000</td><td>0xBD0BE338</td><td>0xBD0BE338</td>
*   </tr>
* </table>
*
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoCrcContext
* The pointer to the \ref cy_stc_crypto_context_crc_t structure which stores
* the Crypto driver context.
*
* \param polynomial
* The polynomial (specified using 32 bits) used in the computing CRC.
*
* \param dataReverse
* The order in which data bytes are processed. 0 - MSB first, 1- LSB first.
*
* \param dataXor
* The byte mask for xoring data
*
* \param remReverse
* A reminder reverse: 0 means the remainder is not reversed. 1 means - reversed.
*
* \param remXor
* Specifies a mask with which the LFSR32 register is XORed to produce a remainder.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Crc_Init(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_crc_t *cryptoCrcContext,
                                         uint32_t polynomial,
                                         uint8_t  dataReverse,
                                         uint8_t  dataXor,
                                         uint8_t  remReverse,
                                         uint32_t remXor)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_CRC_INIT;
    cryptoContext->xdata = cryptoCrcContext;

    cryptoCrcContext->dataReverse = dataReverse;
    cryptoCrcContext->remReverse = remReverse;
    cryptoCrcContext->dataXor = dataXor;
    cryptoCrcContext->polynomial = polynomial;
    cryptoCrcContext->remXor = remXor;

    return (Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Crc_Run
****************************************************************************//**
*
* Performs CRC calculation on a message.
* This function is dependent from Cy_Crypto_Crc_Init which should be called before.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoCrcContext
* The pointer to the \ref cy_stc_crypto_context_crc_t structure which stores
* the Crypto driver context.
*
* \param dataPtr
* The pointer to the message whose CRC is being computed.
*
* \param dataSize
* The size of a message in bytes.
*
* \param crcPtr
* The pointer to a computed CRC value.
*
* \param lfsrInitState
* The initial state of the LFSR.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Crc_Run(cy_stc_crypto_context_t *cryptoContext,
                                        cy_stc_crypto_context_crc_t *cryptoCrcContext,
                                        void* dataPtr,
                                        uint16_t  dataSize,
                                        uint32_t *crcPtr,
                                        uint32_t  lfsrInitState)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_CRC;
    cryptoContext->xdata = cryptoCrcContext;

    cryptoCrcContext->lfsrInitState = lfsrInitState;
    cryptoCrcContext->srcDataPtr = dataPtr;
    cryptoCrcContext->dataSize = dataSize;
    cryptoCrcContext->crcPtr = crcPtr;

    return(Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1)) */

#if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Trng_Generate
****************************************************************************//**
*
* Generates a True Random Number.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoTrngContext
* The pointer to the \ref cy_stc_crypto_context_trng_t structure which stores
* the Crypto driver context.
*
* \param GAROPol;
* The polynomial for the programmable Galois ring oscillator.
*
* \param FIROPol;
* The polynomial for the programmable Fibonacci ring oscillator.
*
* \param max
* The maximum length of a random number, in the range [0, 32] Bits.
*
* \param rndNumPtr
* the pointer to a generated true random number.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Trng_Generate(cy_stc_crypto_context_t *cryptoContext,
                                              cy_stc_crypto_context_trng_t *cryptoTrngContext,
                                              uint32_t  GAROPol,
                                              uint32_t  FIROPol,
                                              uint32_t  max,
                                              uint32_t *rndNumPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_TRNG;
    cryptoContext->xdata = cryptoTrngContext;

    cryptoTrngContext->GAROPol = GAROPol;
    cryptoTrngContext->GAROPol = FIROPol;
    cryptoTrngContext->max = max;
    cryptoTrngContext->trngNumPtr = rndNumPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1)) */

#if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Des_Run
****************************************************************************//**
*
* Performs DES operation on a Single Block. All addresses must be 4-Byte aligned.
* Ciphertext (dstBlockPtr) may overlap with plaintext (srcBlockPtr)
* This function is independent from the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoDesContext
* The pointer to the cy_stc_crypto_context_des_t structure which stores
* the Crypto driver context.
*
* \param dirMode
* Can be CRYPTO_ENCRYPT or CRYPTO_DECRYPT
*
* \param keyPtr
* The pointer to the encryption/decryption key.
*
* \param srcBlockPtr
* The pointer to a source block. Must be 4-Byte aligned!
*
* \param dstBlockPtr
* The pointer to a destination cipher block.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Des_Run(cy_stc_crypto_context_t *cryptoContext,
                                        cy_stc_crypto_context_des_t *cryptoDesContext,
                                        cy_en_crypto_dir_mode_t dirMode,
                                        uint32_t *keyPtr,
                                        uint32_t *dstBlockPtr,
                                        uint32_t *srcBlockPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_DES;
    cryptoContext->xdata = cryptoDesContext;

    cryptoDesContext->dirMode = dirMode;
    cryptoDesContext->keyPtr = keyPtr;
    cryptoDesContext->dstPtr = dstBlockPtr;
    cryptoDesContext->srcPtr = srcBlockPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Tdes_Run
****************************************************************************//**
*
* Performs TDES operation on a Single Block. All addresses must be 4-Byte aligned.
* Ciphertext (dstBlockPtr) may overlap with plaintext (srcBlockPtr)
* This function is independent from the previous Crypto state.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoDesContext
* The pointer to the cy_stc_crypto_context_des_t structure which stores
* the Crypto driver context.
*
* \param dirMode
* Can be CRYPTO_ENCRYPT or CRYPTO_DECRYPT.
*
* \param keyPtr
* The pointer to the encryption/decryption key.
*
* \param srcBlockPtr
* The pointer to a source block. Must be 4-Byte aligned!
*
* \param dstBlockPtr
* The pointer to a destination cipher block.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Tdes_Run(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_des_t *cryptoDesContext,
                                         cy_en_crypto_dir_mode_t dirMode,
                                         uint32_t *keyPtr,
                                         uint32_t *dstBlockPtr,
                                         uint32_t *srcBlockPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_3DES;
    cryptoContext->xdata = cryptoDesContext;

    cryptoDesContext->dirMode = dirMode;
    cryptoDesContext->keyPtr = keyPtr;
    cryptoDesContext->dstPtr = dstBlockPtr;
    cryptoDesContext->srcPtr = srcBlockPtr;

    return (Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1)) */

#if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))
/*******************************************************************************
* Function Name: Cy_Crypto_Rsa_Proc
****************************************************************************//**
*
* Calculates (m^e mod modulo) where m is Message (Signature), e - public exponent
* using a public key in the next representation, it contains:
* modulo,
* public exponent,
* coefficient for Barrett reduction,
* binary inverse of the modulo,
* result of (2^moduloLength mod modulo)
*
* Not all fields in key must be given. Modulo and public exponent is mandatory;
* Barrett coefficient, inverse modulo and r-bar are optional.
* If they don't exists according pointers should be NULL. These coefficients
* could be calculated by \ref Cy_Crypto_Rsa_CalcCoefs.
* Their presence accelerates performance by 5 times.
*
* Returns the processed value and a success value.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoRsaContext
* The pointer to the \ref cy_stc_crypto_context_rsa_t structure which stores
* the RSA context.
*
* \param pubKeyPtr
* The pointer to the \ref cy_stc_crypto_rsa_pub_key_t structure which stores
* public key.
*
* \param messagePtr
* The pointer to the message to be processed.
*
* \param messageLength
* The length of the message to be processed.
*
* \param processedMessagePtr
* The pointer to processed message.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Rsa_Proc(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_rsa_t *cryptoRsaContext,
                                         cy_stc_crypto_rsa_pub_key_t const *pubKeyPtr,
                                         uint32_t const *messageePtr,
                                         uint32_t messageLength,
                                         uint32_t const *processedMessagePtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_RSA_PROC;
    cryptoContext->xdata = cryptoRsaContext;

    cryptoRsaContext->keyPtr = (cy_stc_crypto_rsa_pub_key_t*)pubKeyPtr;
    cryptoRsaContext->messagePtr = messageePtr;
    cryptoRsaContext->messageLength = messageLength;
    cryptoRsaContext->resultPtr = processedMessagePtr;

    return(Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Rsa_CalcCoefs
****************************************************************************//**
*
* Calculates constant coefficients (which is dependent only on modulo
* and independent on message). With this pre-calculated coefficients calculations
* speed-up by 5 times.
*
* These coefficients are:
*                         coefficient for Barrett reduction,
*                         binary inverse of the modulo,
*                         result of (2^moduloLength mod modulo)
*
* Calculated coefficients will be placed by addresses provided in
* pubKeyPtr structure for according coefficients.
* Function overwrites previous values.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoRsaContext
* The pointer to the \ref cy_stc_crypto_context_rsa_t structure which stores
* the RSA context.
*
* \param pubKeyPtr
* The pointer to the \ref cy_stc_crypto_rsa_pub_key_t structure which stores
* public key.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Rsa_CalcCoefs(cy_stc_crypto_context_t *cryptoContext,
                                              cy_stc_crypto_context_rsa_t *cryptoRsaContext,
                                              cy_stc_crypto_rsa_pub_key_t const *pubKeyPtr)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_RSA_COEF;
    cryptoContext->xdata = cryptoRsaContext;

    cryptoRsaContext->keyPtr = (cy_stc_crypto_rsa_pub_key_t*)pubKeyPtr;

    return(Cy_Crypto_Client_Send(cryptoContext));
}
#endif /* #if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#if (CY_CRYPTO_USER_PKCS1_5 == 1)
/*******************************************************************************
* Function Name: Cy_Crypto_Rsa_Verify
****************************************************************************//**
*
* RSA verification with checks for content, paddings and signature format.
* SHA digest of the message and decrypted message should be calculated before.
* Supports only PKCS1-v1_5 format, inside of this format supported padding
* using only SHA, cases with MD2 and MD5 are not supported.
*
* PKCS1-v1_5 described here, page 31:
* http://www.emc.com/collateral/white-papers/h11300-pkcs-1v2-2-rsa-cryptography-standard-wp.pdf
*
* Returns the verification result \ref cy_en_crypto_rsa_ver_result_t.
*
* \param cryptoContext
* The pointer to the \ref cy_stc_crypto_context_t structure which stores
* the Crypto driver common context.
*
* \param cryptoRsaVerContext
* The pointer to the \ref cy_stc_crypto_context_rsa_ver_t structure which stores
* the RSA context.
*
* \param verResult
* The pointer to the verification result \ref cy_en_crypto_rsa_ver_result_t.
*
* \param digestType
* SHA mode used for hash calculation \ref cy_en_crypto_sha_mode_t.
*
* \param digestPtr
* The pointer to the hash of message whose signature to be verified.
*
* \param decryptedSignaturePtr
* The pointer to decrypted signature to be verified.
*
* \param decryptedSignatureLength
* The length of the decrypted signature to be verified, in Bytes
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Rsa_Verify(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_rsa_ver_t *cryptoRsaVerContext,
                                           cy_en_crypto_rsa_ver_result_t *verResult,
                                           cy_en_crypto_sha_mode_t digestType,
                                           uint32_t const *digestPtr,
                                           uint32_t const *decryptedSignaturePtr,
                                           uint32_t decryptedSignatureLength)
{
    cryptoContext->instr = CY_CRYPTO_INSTR_RSA_VER;
    cryptoContext->xdata = cryptoRsaVerContext;

    cryptoRsaVerContext->verResult = verResult;
    cryptoRsaVerContext->digestType = digestType;
    cryptoRsaVerContext->hashPtr = digestPtr;
    cryptoRsaVerContext->decryptedSignaturePtr = decryptedSignaturePtr;
    cryptoRsaVerContext->decryptedSignatureLength = decryptedSignatureLength;

    return(Cy_Crypto_Client_Send(cryptoContext));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Rsa_InvertEndianness
****************************************************************************//**
*
* Inverts endianness in the memory range, like:
* inArr[0] <---> inArr[n]
* inArr[1] <---> inArr[n-1]
* inArr[2] <---> inArr[n-2]
* ........................
* inArr[n/2] <---> inArr[n/2-1]
*
* Odd or even byteSize are acceptable.
*
* \param inArrPtr
* The pointer to the memory whose endianness to be inverted.
*
* \param byteSize
* The length of the memory range whose endianness to be inverted, in Bytes
*
*******************************************************************************/
void Cy_Crypto_Rsa_InvertEndianness(void *inArrPtr, uint32_t byteSize)
{
    int32_t limit;
    int32_t i;
    int32_t j = 0;
    uint8_t temp;
    uint8_t *tempPtr = (uint8_t*)inArrPtr;

    if (byteSize > 1u)
    {
        limit = byteSize / 2u;
        if (0u == (byteSize % 2u))
        {
            limit -= 1u;
        }

        for (i = byteSize - 1u, j = 0u; i > limit; i--, j++)
        {
            temp = tempPtr[j];
            tempPtr[j] = tempPtr[i];
            tempPtr[i] = temp;
        }
    }
}

#endif /* #if (CY_CRYPTO_USER_PKCS1_5 == 1) */


#if defined(__cplusplus)
}
#endif


#endif /* #if (CPUSS_CRYPTO_PRESENT == 1) */

/* [] END OF FILE */
