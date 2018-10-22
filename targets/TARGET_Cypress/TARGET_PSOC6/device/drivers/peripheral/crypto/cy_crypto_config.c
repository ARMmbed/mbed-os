/***************************************************************************//**
* \file
* \version 2.0
*
*  Description:
*  This C file is not intended to be part of the Crypto driver.  It is the code
*  required to configure the crypto driver by user.
*
********************************************************************************
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_crypto_config.h"

#include "ipc/cy_ipc_drv.h"
#include "sysint/cy_sysint.h"


/** The Crypto configuration structure. */
const cy_stc_crypto_config_t cryptoConfig =
{
    /* .ipcChannel             */ CY_IPC_CHAN_CRYPTO,
    /* .acquireNotifierChannel */ CY_CRYPTO_IPC_INTR_NOTIFY_NUM,
    /* .releaseNotifierChannel */ CY_CRYPTO_IPC_INTR_RELEASE_NUM,

    /* .releaseNotifierConfig */ {
#if (CY_CPU_CORTEX_M0P)
        /* .intrSrc            */ CY_CRYPTO_CM0_RELEASE_INTR_NR,
        /* .cm0pSrc            */ (cy_en_intr_t)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_RELEASE_NUM),
#else
        /* .intrSrc            */ (IRQn_Type)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_RELEASE_NUM),
#endif
        /* .intrPriority       */ CY_CRYPTO_RELEASE_INTR_PR,
    },
    /* .userCompleteCallback   */ NULL

#if (CY_CRYPTO_CORE_ENABLE)
    ,
    /* .userGetDataHandler     */ NULL,
    /* .userErrorHandler       */ NULL,

    /* .acquireNotifierConfig */ {
#if (CY_CPU_CORTEX_M0P)
        /* .intrSrc            */ CY_CRYPTO_CM0_NOTIFY_INTR_NR,
        /* .cm0pSrc            */ (cy_en_intr_t)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_NOTIFY_NUM),
#else
        /* .intrSrc            */ (IRQn_Type)CY_IPC_INTR_NUM_TO_VECT((int32_t)CY_CRYPTO_IPC_INTR_NOTIFY_NUM),
#endif
        /* .intrPriority       */ CY_CRYPTO_NOTIFY_INTR_PR,
    },
    /* .cryptoErrorIntrConfig */ {
#if (CY_CPU_CORTEX_M0P)
        /* .intrSrc            */ CY_CRYPTO_CM0_ERROR_INTR_NR,
        /* .cm0pSrc            */ cpuss_interrupt_crypto_IRQn,
#else
        /* .intrSrc            */ cpuss_interrupt_crypto_IRQn,
#endif
        /* .intrPriority       */ CY_CRYPTO_ERROR_INTR_PR,
    }
#endif

};


/* [] END OF FILE */

