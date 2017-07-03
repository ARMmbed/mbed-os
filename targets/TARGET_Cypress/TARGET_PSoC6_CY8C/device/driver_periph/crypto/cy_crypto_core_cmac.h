/***************************************************************************//**
* \file cy_crypto_core_cmac.h
* \version 1.0
*
* \brief
*  This file provides constants and function prototypes
*  for the API for the CMAC method in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CRYPTO_CORE_CMAC_H)
#define CY_CRYPTO_CORE_CMAC_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_CMAC == 1))

/** \cond INTERNAL */

/* The bit string used to generate sub-keys */
#define CY_CRYPTO_CMAC_RB  (0x87u)

/* The structure to store the AES-CMAC context */
typedef struct
{
    uint32_t *blockPtr;
    uint32_t *kPtr;
    uint32_t *tempPtr;
} cy_stc_crypto_cmac_state_t;


/* The function prototypes */
uint32_t Cy_Crypto_Core_Cmac_LeftShift(uint8_t *srcDstPtr);

void Cy_Crypto_Core_Cmac_Init(cy_stc_crypto_cmac_state_t *cmacStatePtr,
                                uint32_t *tempPtr,
                                uint32_t *blockPtr,
                                uint32_t *kPtr);

void Cy_Crypto_Core_Cmac_Start(cy_stc_crypto_aes_state_t  *aesStatePtr,
                                 cy_stc_crypto_cmac_state_t *cmacStatePtr);

void Cy_Crypto_Core_Cmac_Update(cy_stc_crypto_aes_state_t  *aesStatePtr,
                                  cy_stc_crypto_cmac_state_t *cmacStatePtr,
                                  uint32_t *messagePtr,
                                  uint32_t messageSize);

void Cy_Crypto_Core_Cmac_Finish(cy_stc_crypto_aes_state_t  *aesStatePtr,
                                  cy_stc_crypto_cmac_state_t *cmacStatePtr,
                                  uint32_t* cmacPtr);

cy_en_crypto_status_t Cy_Crypto_Core_Cmac(cy_stc_crypto_context_aes_t *cryptoAesContext);

/** \endcond */


#endif /* ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_CMAC == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_CMAC_H) */

/* [] END OF FILE */
