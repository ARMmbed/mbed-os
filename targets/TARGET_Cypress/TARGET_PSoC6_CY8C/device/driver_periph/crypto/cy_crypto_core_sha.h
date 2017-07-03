/***************************************************************************//**
* \file cy_crypto_core_sha.h
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


#if !defined(CY_CRYPTO_CORE_SHA_H)
#define CY_CRYPTO_CORE_SHA_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))

/** \cond INTERNAL */

/* Defines for the SHA algorithm */
#define CY_CRYPTO_SHA1_BLOCK_SIZE           (64u)
#define CY_CRYPTO_SHA256_BLOCK_SIZE         (64u)
#define CY_CRYPTO_SHA512_BLOCK_SIZE         (128u)
#define CY_CRYPTO_MAX_BLOCK_SIZE            (128u)

#define CY_CRYPTO_SHA256_PAD_SIZE           (56uL)
#define CY_CRYPTO_SHA512_PAD_SIZE           (112uL)

#define CY_CRYPTO_SHA1_HASH_SIZE            (20u)
#define CY_CRYPTO_SHA256_HASH_SIZE          (64u)
#define CY_CRYPTO_SHA512_HASH_SIZE          (128u)
#define CY_CRYPTO_MAX_HASH_SIZE             (128u)

#define CY_CRYPTO_SHA1_ROUND_MEM_SIZE       (320uL)
#define CY_CRYPTO_SHA256_ROUND_MEM_SIZE     (256uL)
#define CY_CRYPTO_SHA512_ROUND_MEM_SIZE     (640uL)
#define CY_CRYPTO_MAX_ROUND_MEM_SIZE        (640uL)

/* The structure for storing the SHA context */
typedef struct
{
    cy_en_crypto_sha_mode_t mode;
    uint8_t *blockPtr;
    uint32_t blockSize;
    uint32_t *hashPtr;
    uint32_t hashSize;
    uint32_t *roundMemPtr;
    uint32_t roundMemSize;
    uint32_t messageSize;
    uint32_t digestSize;
    const uint32_t *initialHashPtr;
} cy_stc_crypto_sha_state_t;


typedef enum
{

#if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1))
    CY_CRYPTO_SHA_CTL_MODE_SHA1    = 0u,
#endif /* #if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1)) */

#if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1))
    CY_CRYPTO_SHA_CTL_MODE_SHA256  = 1u,
#endif /* #if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1)) */

#if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1))
    CY_CRYPTO_SHA_CTL_MODE_SHA512  = 2u,
#endif /* #if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1)) */

} cy_en_crypto_sha_hw_mode_t;


void Cy_Crypto_Core_Sha_Init(cy_stc_crypto_sha_state_t *hashStatePtr,
                               uint8_t *blockPtr,
                               uint32_t *hashPtr,
                               uint32_t *roundMemPtr,
                               cy_en_crypto_sha_mode_t mode);

void Cy_Crypto_Core_Sha_ProcessBlock(cy_stc_crypto_sha_state_t *hashStatePtr,
                                    uint32_t *blockPtr);

void Cy_Crypto_Core_Sha_Start(cy_stc_crypto_sha_state_t *hashStatePtr);

void Cy_Crypto_Core_Sha_Update(cy_stc_crypto_sha_state_t *hashStatePtr,
                               uint32_t *messagePtr,
                               uint32_t messageSize);

void Cy_Crypto_Core_Sha_Finish(cy_stc_crypto_sha_state_t *hashStatePtr,
                               uint32_t *digestPtr,
                               uint32_t finalMessageSize);

void Cy_Crypto_Core_Sha_Free(cy_stc_crypto_sha_state_t *hashStatePtr);

cy_en_crypto_status_t Cy_Crypto_Core_Sha(cy_stc_crypto_context_sha_t *cryptoShaContext);

/** \endcond */


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_SHA_H) */

/* [] END OF FILE */
