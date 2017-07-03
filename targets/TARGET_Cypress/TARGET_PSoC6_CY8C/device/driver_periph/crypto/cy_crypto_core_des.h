/***************************************************************************//**
* \file cy_crypto_core_des.h
* \version 1.0
*
* \brief
*  This file provides constant and parameters for the API for the DES method
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_DES_H)
#define CY_CRYPTO_CORE_DES_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1))

#define CY_CRYPTO_DES_WEAK_KEY_COUNT   (16)
#define CY_CRYPTO_DES_KEY_BYTE_LENGTH  (8)

typedef enum {
    CY_CRYPTO_DES_MODE_SINGLE = 0,
    CY_CRYPTO_DES_MODE_TRIPLE = 1
} cy_en_crypto_des_mode_t;

cy_en_crypto_status_t Cy_Crypto_Core_Des(cy_stc_crypto_context_des_t *cryptoDesContext);

cy_en_crypto_status_t Cy_Crypto_Core_Tdes(cy_stc_crypto_context_des_t *cryptoDesContext);


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1)) */
#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_DES_H) */


/* [] END OF FILE */

