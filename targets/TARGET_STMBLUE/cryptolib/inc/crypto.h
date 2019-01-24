/**
  ******************************************************************************
  * @file    crypto.h
  * @author  MCD Application Team
  * @version V2.1
  * @date    22-June-2012
  * @brief   This file contains all the functions prototypes for the config firmware 
  *          library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http:www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#ifndef STACKLIB_CRYPTO_H
#define STACKLIB_CRYPTO_H
#include "config.h"

/* List of automatically enabled defines and checks */

/* INCLUDE_DRBG_AES128 requires AES128 with Encryption capabilities */
#ifdef INCLUDE_DRBG_AES128
#define INCLUDE_AES128
#define INCLUDE_ENCRYPTION
#endif

/* XTS REQUIRES ENCRYPTION */
#ifdef INCLUDE_XTS
#define INCLUDE_ENCRYPTION
#endif

#if defined(INCLUDE_AES128) || defined(INCLUDE_AES192) ||  defined(INCLUDE_AES256)
#define INCLUDE_AES /*!< Automatically defined if AES has been selected */
#endif

#if defined(INCLUDE_DES) || defined(INCLUDE_TDES)
#define INCLUDE_DES_CORE /*!< TripleDES and DES has the same core function, this flag enables it */
#endif

#if defined(INCLUDE_RSA) || defined (INCLUDE_ECC)
#define INCLUDE_MATH /*!< The Multiprecision Math engine is included */
#endif

#if defined(INCLUDE_SHA1) || defined(INCLUDE_SHA224) || defined(INCLUDE_SHA256) || defined(INCLUDE_MD5)
#define INCLUDE_HASH /*!< Hash functions are included  */
#endif


/*************************************************************************************************
***-*-*-*-*--**- SOME CHECKS, DON'T MODIFY -**--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**
*************************************************************************************************/
#if (defined(INCLUDE_ENCRYPTION) || defined(INCLUDE_DECRYPTION)) && !(defined(INCLUDE_AES) || defined(INCLUDE_DES_CORE) || defined(INCLUDE_ARC4))
#error "Either encryption or decryption has been specified but no AES or DES are present"
#elif (!defined(INCLUDE_ENCRYPTION) && !defined(INCLUDE_DECRYPTION)) && (defined(INCLUDE_AES) || defined(INCLUDE_DES_CORE))
#error "AES or DES are present but without encryption or decryption functionalities!"
#endif

#ifdef CRL_CPU_SUPPORT_MISALIGNED
  #ifndef CRL_ENDIANNESS
    #error "With CRL_CPU_SUPPORT_MISALIGNED=1 a definition for CRL_ENDIANNESS is required!"
  #endif
#endif

#if defined(INCLUDE_GCM) && !defined(INCLUDE_AES)
#error "GCM is currently supported only by AES, but AES has not be included, please include AES or remove GCM"
#endif

#if defined(INCLUDE_CMAC) && !defined(INCLUDE_AES)
#error "CMAC is currently supported only by AES, but AES has not be included, please include AES or remove GCM"
#endif

#if defined(INCLUDE_KEY_WRAP) && !defined(INCLUDE_AES)
#error "KWRAP is currently supported only by AES, but AES has not be included, please include AES or remove KWRAP"
#endif

#if defined(INCLUDE_CTR) && !defined(INCLUDE_AES)
#error "CTR is currently supported only by AES, but AES has not be included, please include AES or remove CTR"
#endif

#if defined(INCLUDE_CCM) && !defined(INCLUDE_AES)
#error "CCM is currently supported only by AES, but AES has not be included, please include AES or remove CCM"
#endif

#if defined(INCLUDE_CFB) && !defined(INCLUDE_AES)
#error "CFB is currently supported only by AES, but AES has not be included, please include AES or remove CFB"
#endif

#if defined(INCLUDE_OFB) && !defined(INCLUDE_AES)
#error "OFB is currently supported only by AES, but AES has not be included, please include AES or remove OFB"
#endif

#if defined(INCLUDE_XTS) && !defined(INCLUDE_AES)
#error "XTS is currently supported only by AES, but AES has not be included, please include AES or remove XTS"
#endif

#if defined(INCLUDE_DRBG_AES128) && !defined(INCLUDE_AES128)
#error "DRBG_AES128 requires INCLUDE_AES_128"
#endif

#if defined(INCLUDE_ECC_SIGN) & !(defined(STM32F21X) || defined(INCLUDE_DRBG_AES128))
#error "To include ECC_SIGN functionalities you need a random generator. If you use an STM32F21X define STM32F21X, otherwise define INCLUDE_DRBG_AES128\n"
#endif

#if defined(INCLUDE_RSA) && !defined(RSA_WINDOW_SIZE)
#define RSA_WINDOW_SIZE 1
#endif


/* Checks */
/* If there is HW AES support include hw implementations */
#if defined(USE_HW_AES_128) || defined(USE_HW_AES_192) || defined(USE_HW_AES_256)
#define USE_HW_AES   /*!< There is support for HW AES  */
#endif 

/* If hw supports all keys sizes then prepare to exclude software */
#if defined(USE_HW_AES_128) && defined(USE_HW_AES_192) && defined(USE_HW_AES_256)
#define USE_HW_AES_XXX   /*!< There is support for HW AES with all three key sizes  */
#else
/* Otherwise software should be included */
#define USE_SW_AES   /*!< AES is implemented in SW  */
#endif

#ifndef USE_HW_DES  
#define USE_SW_DES   /*!< DES/TDES is implemented in SW  */
#endif

#ifndef USE_HW_MD5
#define USE_SW_MD5  /*!< MD5 is implemented in SW  */
#endif

#ifndef USE_HW_SHA1
#define USE_SW_SHA1  /*!< SHA-1 is implemented in SW  */
#endif

/* If we NOW have both sw and hw then it means we are using an hw with support to 128 keys only */
#if defined(USE_SW_AES) && defined(USE_HW_AES)
#define USE_HW_AES_ONLY_128  /*!< There is HW AES support only for keys of 128 bit  */
#endif

/* If we NOW have both sw and hw then it means we are using an hw with support to 128 keys only */
#if defined(INCLUDE_DRBG_AES128) || defined(USE_HW_RNG)
#define INCLUDE_RNG  /*!< A Random Engine is present in the library  */
#endif

/*************************************************************************************************
***-*-*-*-*--**- CRYPTO LIBRARY INCLUSIONS -**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-***
*************************************************************************************************/
#include <stdlib.h>
#include "Common/types.h"
#include "Common/macros.h"
#include "Common/err_codes.h"
#include "Common/sk.h"

#ifdef INCLUDE_AES
#include "AES/aes.h"
#endif

#ifdef INCLUDE_DES
#include "DES/des.h"
#endif

#ifdef INCLUDE_TDES
#include "TDES/tdes.h"
#endif

#ifdef INCLUDE_ARC4
#include "ARC4/arc4.h"
#endif

#ifdef INCLUDE_HASH
#include "HASH/hash.h"
#endif

#ifdef INCLUDE_MATH
#include "MATH/math.h"
#endif

#ifdef INCLUDE_RSA
#include "RSA/rsa.h"
#endif

#ifdef INCLUDE_RNG
#include "RNG/rng.h"
#endif

#ifdef INCLUDE_ECC
#include "ECC/ecc.h"
#endif

void Crypto_DeInit(void);

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
#endif /* STACKLIB_CRYPTO_H */
