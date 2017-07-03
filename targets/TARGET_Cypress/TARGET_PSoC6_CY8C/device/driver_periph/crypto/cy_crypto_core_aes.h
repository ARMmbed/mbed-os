/***************************************************************************//**
* \file cy_crypto_core_aes.h
* \version 1.0
*
* \brief
*  This file provides constant and parameters for the API for the AES method
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_AES_H)
#define CY_CRYPTO_CORE_AES_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))


#define CY_CRYPTO_AES_CTR_CNT_POS          (0x02u)

void Cy_Crypto_Core_Aes_InvKey(cy_stc_crypto_aes_state_t const *aesStatePtr);

void Cy_Crypto_Core_Aes_ProcessBlock(cy_stc_crypto_aes_state_t const *aesStatePtr,
                                       cy_en_crypto_dir_mode_t dirMode,
                                       uint32_t *dstBlockPtr,
                                       uint32_t const *srcBlockPtr);

void Cy_Crypto_Core_Aes_Xor(uint32_t const *src0BlockPtr,
                              uint32_t const *src1BlockPtr,
                              uint32_t *dstBlockPtr);

cy_en_crypto_status_t Cy_Crypto_Core_Aes_Init(cy_stc_crypto_context_aes_t const *cryptoContextPtr);

#if (CY_CRYPTO_USER_AES_ECB == 1)
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Ecb(cy_stc_crypto_context_aes_t *cryptoContextPtr);
#endif /* #if (CY_CRYPTO_USER_AES_ECB == 1) */

#if (CY_CRYPTO_USER_AES_CBC == 1)
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Cbc(cy_stc_crypto_context_aes_t *cryptoContextPtr);
#endif /* #if (CY_CRYPTO_USER_AES_CBC == 1) */

#if (CY_CRYPTO_USER_AES_CFB == 1)
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Cfb(cy_stc_crypto_context_aes_t *cryptoContextPtr);
#endif /* #if (CY_CRYPTO_USER_AES_CFB == 1) */

#if (CY_CRYPTO_USER_AES_CTR == 1)
cy_en_crypto_status_t Cy_Crypto_Core_Aes_Ctr(cy_stc_crypto_context_aes_t *cryptoContextPtr);
#endif /* #if (CY_CRYPTO_USER_AES_CTE == 1) */

__STATIC_INLINE void Cy_Crypto_Core_Aes_WaitReady(void)
{
    /* Wait until the TRNG instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_AES_BUSY, CRYPTO->STATUS))
    {
    }
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_AES_H) */


/* [] END OF FILE */

