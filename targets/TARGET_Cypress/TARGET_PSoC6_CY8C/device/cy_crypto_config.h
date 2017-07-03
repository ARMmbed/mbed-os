/***************************************************************************//**
* \file cy_crypto_config.h
* \version 1.0
*
* \brief
*  This file provides user parameters for the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CRYPTO_CONFIG_H)
#define CY_CRYPTO_CONFIG_H


/* Defines to configure interrupts used in Crypto driver */

/* Number of Crypto Notify interrupt inputs to CM0+ */
#define CY_CRYPTO_CM0_NOTIFY_INTR_NR  (26u)
/* Priority of Crypto Notify interrupt inputs to CM0+ */
#define CY_CRYPTO_NOTIFY_INTR_PR      (2u)

/* Number of Crypto release interrupt inputs to CM0+ */
#define CY_CRYPTO_CM0_RELEASE_INTR_NR (27u)
/* Priority of Crypto release interrupt inputs to CM0+ */
#define CY_CRYPTO_RELEASE_INTR_PR     (2u)

/* Number of Crypto Error interrupt inputs to CM0+ */
#define CY_CRYPTO_CM0_ERROR_INTR_NR   (28u)
/* Priority of Crypto Error interrupt inputs to CM0+ */
#define CY_CRYPTO_CM0_ERROR_INTR_PR   (2u)

/* Default Crypto driver configuration */
#define CY_CRYPTO_DEFAULT_CONFIG \
    { \
        CY_CRYPTO_CM0_NOTIFY_INTR_NR,  \
        CY_CRYPTO_NOTIFY_INTR_PR,      \
        CY_CRYPTO_CM0_RELEASE_INTR_NR, \
        CY_CRYPTO_RELEASE_INTR_PR,     \
        CY_CRYPTO_CM0_ERROR_INTR_NR,   \
        CY_CRYPTO_CM0_ERROR_INTR_PR,   \
        NULL, \
        NULL, \
        NULL \
    }

/* Defines to Enable/Disable Crypto functionality */

/* AES CMAC support (0 = no support, 1 = support),
 * to run CMAC, AES ECB mode should be enabled */
#define CY_CRYPTO_USER_CMAC           (1u)
/* AES ECB cipher support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_AES_ECB        (1u)
/* AES CBC cipher support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_AES_CBC        (1u)
/* AES CFB cipher support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_AES_CFB        (1u)
/* AES CTR cipher support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_AES_CTR        (1u)
/* PKCS1-v1_5 verification support (0 = no support, 1 = support),
 * to run PKCS1-v1_ at least one of SHA modes should be enabled */
#define CY_CRYPTO_USER_PKCS1_5        (1u)
/* HMAC support (0 = no support, 1 = support),
 * to run HMAC at least one of SHA modes should be enabled */
#define CY_CRYPTO_USER_HMAC           (1u)
/* SHA1 hash support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_SHA1           (1u)
/* SHA 224 and 256 hash support,  (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_SHA256         (1u)
/* SHA 384, 512, 512_224 and 512_256 hash support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_SHA512         (1u)
/* (Triple) DES cipher support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_DES            (1u)
/* Pseudo random number generation support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_PR             (1u)
/* Cyclic Redundancy Check support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_CRC            (1u)
/* Vector unit support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_VU             (1u)
/* True random number generation support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_TR             (1u)
/* String support (0 = no support, 1 = support) */
#define CY_CRYPTO_USER_STR            (1u)

#endif /* #if !defined(CY_CRYPTO_CONFIG_H) */


/* [] END OF FILE */
