/***************************************************************************//**
* \file cy_crypto_core_prng.h
* \version 1.0
*
* \brief
*  This file provides provides constant and parameters for the API of the PRNG
*  in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CRYPTO_CORE_PRNG_H)
#define CY_CRYPTO_CORE_PRNG_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1))


cy_en_crypto_status_t Cy_Crypto_Core_Prng_Init (cy_stc_crypto_context_prng_t *cryptoPrngContext);
cy_en_crypto_status_t Cy_Crypto_Core_Prng      (cy_stc_crypto_context_prng_t *cryptoPrngContext);


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_PRNG_H) */


/* [] END OF FILE */
