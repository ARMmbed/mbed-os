/***************************************************************************//**
* \file cy_crypto_cora_hmac.h
* \version 1.0
*
* \brief
*  This file provides constants and function prototypes
*  for the API for the SHA method in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CRYPTO_CORE_HMAC_H)
#define CY_CRYPTO_CORE_HMAC_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1))


cy_en_crypto_status_t Cy_Crypto_Core_Hmac(cy_stc_crypto_context_sha_t *cryptoContext);


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_HMAC_H) */

/* [] END OF FILE */
