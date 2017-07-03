/***************************************************************************//**
* \file cy_crypto_core_rsa.h
* \version 1.0
*
* \brief
*  This file provides provides constant and parameters
*  for the API of the RSA in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CRYPTO_CORE_RSA_H)
#define CY_CRYPTO_CORE_RSA_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))


/* Crypto IP condition codes (vector unit) */
#define CY_CRYPTO_VU_COND_ALWAYS    (0x00u)
#define CY_CRYPTO_VU_COND_EQ        (0x01u)
#define CY_CRYPTO_VU_COND_NE        (0x02u)
#define CY_CRYPTO_VU_COND_CS        (0x03u)
#define CY_CRYPTO_VU_COND_CC        (0x04u)
#define CY_CRYPTO_VU_COND_HI        (0x05u)
#define CY_CRYPTO_VU_COND_LS        (0x06u)
#define CY_CRYPTO_VU_COND_EVEN      (0x07u)
#define CY_CRYPTO_VU_COND_ODD       (0x08u)


/* Crypto IP status (vector unit) */
#define CY_CRYPTO_VU_STATUS_ONE     (3u)
#define CY_CRYPTO_VU_STATUS_ZERO    (2u)
#define CY_CRYPTO_VU_STATUS_EVEN    (1u)
#define CY_CRYPTO_VU_STATUS_CARRY   (0u)


cy_en_crypto_status_t Cy_Crypto_Core_Rsa_Proc(cy_stc_crypto_context_rsa_t *cryptoContextPtr);
cy_en_crypto_status_t Cy_Crypto_Core_Rsa_Coef(cy_stc_crypto_context_rsa_t *cryptoContextPtr);

#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#if (CY_CRYPTO_USER_PKCS1_5 == 1)
cy_en_crypto_status_t Cy_Crypto_Core_RsaVerify(cy_stc_crypto_context_rsa_ver_t *cryptoContextPtr);
#endif /* #if (CY_CRYPTO_USER_PKCS1_5 == 1) */


#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_RSA_H) */


/* [] END OF FILE */
