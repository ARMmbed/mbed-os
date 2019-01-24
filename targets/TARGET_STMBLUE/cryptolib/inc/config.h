/**
  ******************************************************************************
  * @file    config.h
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

/* \mainpage
 *
 * \section intro Introduction
 *
 * This is a software library for the STM32 microcontroller family which provides cryptographic
 * functionalities. \n
 * Cryptographic functions available (selectable in \ref ConfOptions):
 * - Advanced Encryption Algorithm (\ref AES)<a href="http:en.wikipedia.org/wiki/Advanced_Encryption_Standard">[1]</a> with keys of 128, 192 and 256 bit. Supported Modes: 
 *   - \ref AESECB (Electronic Codebook Mode) <a href="http:en.wikipedia.org/wiki/Block_cipher_modes_of_operation#Electronic_codebook_.28ECB.29"> [2]</a>
 *   - \ref AESCBC (Cipher-Block Chaining) with support for Ciphertext Stealing <a href="http:en.wikipedia.org/wiki/Block_cipher_modes_of_operation#Cipher-block_chaining_.28CBC.29">[3]</a>
 *   - \ref AESCTR (CounTer Mode) <a href="http:en.wikipedia.org/wiki/Block_cipher_modes_of_operation#Counter_.28CTR.29"> [4]</a>
 *   - \ref AESCFB (Cipher Feedback) <a href="http:en.wikipedia.org/wiki/Modes_of_operation#Cipher_feedback_.28CFB.29"> [18]</a>
 *   - \ref AESOFB (Output Feedback) <a href="http:en.wikipedia.org/wiki/Modes_of_operation#Cipher_feedback_.28OFB.29"> [19]</a>
 *   - \ref AESXTS XTS as specified in IEEE Std 1619-2007
 *   - \ref AESCCM (Counter with CBC-MAC) <a href="http:en.wikipedia.org/wiki/CCM_mode"> [5]</a>
 *   - \ref AESGCM (Galois Counter Mode) <a href="http:en.wikipedia.org/wiki/Galois/Counter_Mode"> [6]</a>
 *   - \ref AESCMAC  <a href="http:en.wikipedia.org/wiki/CMAC"> [7]</a>
 *   - \ref AESKEYWRAP <a href="http:csrc.nist.gov/groups/ST/toolkit/documents/kms/key-wrap.pdf"> [8] </a>
 * - \ref DES <a href="http:en.wikipedia.org/wiki/Data_Encryption_Standard"> [9] </a>, \ref TDES<a href="http:en.wikipedia.org/wiki/Triple_DES"> [10]</a>. Supported Modes: 
 *   - \ref DESECB, \ref TDESECB (Electronic Codebook Mode)
 *   - \ref DESCBC. \ref TDESCBC (Cipher-Block Chaining)
 * - \ref ARC4 <a href="http:en.wikipedia.org/wiki/RC4"> [15] </a>
 * - \ref HASH <a href="http:en.wikipedia.org/wiki/Cryptographic_hash_function"> [11]</a> with HMAC <a href="http:en.wikipedia.org/wiki/HMAC">[12]</a> support:
 *   - \ref MD5 <a href="http:en.wikipedia.org/wiki/MD5">[12]</a>
 *   - \ref SHA1 <a href="http:en.wikipedia.org/wiki/SHA-1">[13]</a>
 *   - \ref SHA224 <a href="http:en.wikipedia.org/wiki/SHA-2">[14]</a>
 *   - \ref SHA256 <a href="http:en.wikipedia.org/wiki/SHA-2">[14]</a>
 *   - \ref SHA384 <a href="http:en.wikipedia.org/wiki/SHA-2">[14]</a>
 *   - \ref SHA512 <a href="http:en.wikipedia.org/wiki/SHA-2">[14]</a>
 * - \ref RNG Random Engine based on <a href="http:csrc.nist.gov/publications/nistpubs/800-90/SP800-90revised_March2007.pdf"> DRBG-AES-128 </a>
 * - \ref RSA <a href="http:en.wikipedia.org/wiki/RSA_%28algorithm%29"> [16] </a> encryption/decryption and signature/verification functions with <a href="http:www.emc.com/emc-plus/rsa-labs/standards-initiatives/pkcs-rsa-cryptography-standard.htm">PKCS#1v1.5</a>
 * - \ref ECC <a href="http:en.wikipedia.org/wiki/Elliptic_curve_cryptography">[17] (Elliptic Curve Cryptography)</a> with the following functionalities:
 *   - Key Generation
 *   - Scalar Multiplication (the base for ECDH)
 *   - <a href="http:en.wikipedia.org/wiki/ECDSA"> ECDSA </a>
 *
 *  Remember to set the "config.h" file according to your needs before
 *  compiling.
 *
 * \section the_doc This Documentation
 *
 * We encourage the user of this library to look at our \ref Tutorials page to be introduced in the library usage. \n
 * For functions documentation we suggest to look at the \ref UserAPI documentation. \n
 * All users of previous versions of the library should check the page \ref changelog \n
 * Developers should also look at the \ref InternalAPI functions. \n
 *
 * \section license Documentation License
 *  This documentation is subject to the same license and copyright as the
 *  source code from which it's generated.
 * \page Tutorials
 * Here you can find tutorial with examples for the following functions:
 *  - For Symmetric Key function see \subpage Tutorial_AES,\subpage Tutorial_DES_TDES, \subpage Tutorial_ARC4
 *  - For Hash and HMAC function see \subpage Tutorial_Hash 
 *  - For RSA functions see \subpage Tutorial_RSA
 *  - For ECC and ECDSA function see \subpage Tutorial_ECC
 *  - For Random Engine functions see \subpage Tutorial_RNG
 *
 * \page changelog Changes from Version 2.0
 * Some API and preprocessor directives have been modified and some functions has changed behaviour since version 2.0. \n Here's a list of the most significant changes: 
 * \section cryptodeinit Crypto_DeInit function
 *  A new function named \ref Crypto_DeInit has been added. It should be called to initialize the library, before performing any operation with it.
 * \section dynall Dynamic Allocation
 *  Basically since version 2.1 the Crypto Library doesn't rely on malloc for memory allocation. \n
 *  Instead it make use of a pre-allocated buffer that the user must allocate and assign to a specific structure (\ref membuf_stt). \n
 *  Then this structure will be passed to all the functions that requires a lot of internal memory for computations or to initialize dynamic-size variables.
 *  The majority of RSA, ECC and MATH functions has changed API to receive as parameter a \ref membuf_stt. \n
 * \section rngchange Changes to Random Engine
 *  The define of C_SW_DRBG_AES128 has been changed to \ref C_DRBG_AES128. \n The reason is that this function implements the NIST DRBG Based generator even if 
 *  the hardware has a TRNG.
 *  With version 2.1 of the library the HW RNG is enabled on every STM32F2 and STM32F4, wit the expection of the STM32F40X. \n \n 
 *  If \ref CRL_RANDOM_REQUIRE_RESEED is defined at build time, then after a \ref RNGinit call that initialize a DRBG, the random engine will accept up to 2^32-1 calls to either 
 *  \ref  RNGgenWords or \ref RNGgenBytes, after that those will return \ref RNG_ERR_RESEED_NEEDED. \n 
 *  In this case it is required to feed the RNG with new entropy through the \ref RNGreseed function.
 * \section eccchange Changes to ECC output
 *  The functions:
 *  - \ref ECCgetPointCoordinate 
 *  - \ref ECCgetPrivKeyValue
 *  - \ref ECDSAgetSignature
 *
 * have been changed in order to provide fixed size input, independent of the actual value. \n The size of the ouput is related to the size of the P Elliptic 
 * Curve parameter for \ref ECCgetPointCoordinate, while it's related to the N Elliptic Curve parameter fpr the other two functions. \n
 * The output size will always be a multiple of 4, even if P or N aren't. Therefore it is possible that the returned value will be zero padded.
 * \section defines Changes to preprocessor directives
 *  Some defines has changed since version 2.0 :
 *  - INCLUDE_ECC_SIGN and INCLUDE_ECC_VERIFY doesn't exist anymore and have been merged into \ref INCLUDE_ECC
 *  - The more generic USE_CRYPTO_HW has been removed. It is replaced by defines which clearly states which hardware is it possible to use on the selected platform. More specifically:
 *    - For AES, there are 3 different but NOT mutually exclusive directories: \ref USE_HW_AES_128 \ref USE_HW_AES_192 \ref USE_HW_AES_256
 *    - For DES and TDES, there is \ref USE_HW_DES
 *    - For MD5: \ref USE_HW_MD5
 *    - For SHA-1: \ref USE_HW_SHA1
 *    - For RNG: \ref USE_HW_RNG, which replaces INCLUDE_HW_RNG
 *  - For AES, some new possible values are defined, accordingly to the presence absense of some HW capabilities: \ref USE_HW_AES_XXX \ref USE_SW_AES \ref USE_HW_AES_ONLY_128
 *  - INCLUDE_MATH_OPS has been replaced by \ref INCLUDE_MATH
 *  - INCLUDE_SW_RNG has been removed. The reason is that now C_DRBG_AES128 uses HW AES on platform that supports it.
 *  - In case of absense of an HW crypto peripheral, the following respective values are automatically defined: \ref USE_SW_AES \ref USE_SW_DES \ref USE_SW_MD5 \ref USE_SW_SHA1
 *  - A new directive \ref CL_ON_STM32 is always defined when a proper STM32 device define is enabled (i.e. STM32F0XX, STM32F10X_LD, STM32F10X_LD_VL, STM32F10X_MD, STM32F10X_MD_VL, STM32F10X_HD,
 *     STM32F10X_HD_VL, STM32F10X_XL, STM32F10X_CL, STM32F2XX, STM32F30X, STM32F37X, STM32F4XX, STM32F40XX, STM32F427X, STM32L1XX_MD, STM32L1XX_MDP, STM32L1XX_HD
 *  - \ref CRL_RANDOM_REQUIRE_RESEED has been added. If defined then the library functions \ref RNGgenWords and \ref RNGgenBytes might return \ref RNG_ERR_RESEED_NEEDED if more than 2^32 
 *    requests for random generation have been made. In this case it is required to feed the RNG with new entropy through the \ref RNGreseed function.
 */


/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef STACKLIB__CRL_CONFIG_H__
#define STACKLIB__CRL_CONFIG_H__

/** @defgroup ConfOptions Library Configuration Options
  * @{
  */

  
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*************************************************************************************************
***-*-*-*-*--**- CONFIGURATION OPTIONS -**--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-***
*************************************************************************************************/

/** 
  * @defgroup CortexConf Compiler setting
  * @{
  */
#define CORTEX_M0 /*!< Library is compiled for Cortex M0 */
/*#define CORTEX_M3*/ /*!< Library is compiled for Cortex M3 */
/*#define CORTEX_M4*/ /*!< Library is compiled for Cortex M4 */
/**
  * @}
  */ 


/** 
  * @defgroup SoCConf Cryptographic hardware accellerators
  * @{
  */
/*#define STM32F2XX*/
/*#define STM32F21X*/ /*!< Use this if you are using STM32 Series F21X which has the cryptographic hardware accellerators */
/*#define STM32F4XX*/
/*#define STM32F41X*/ /*!< Use this if you are using STM32 Series F41X which has the cryptographic hardware accellerators */

/**
  * @}
  */


/** 
  * @defgroup EndiannessConf Endianness
  * @{
  */
/*! Used to specify the memory representation of the platform.
  *   - CRL_ENDIANNESS=1 for LITTLE ENDIAN
  *   - CRL_ENDIANNESS=2 for BIG ENDIAN \n
  *   This setting is \b VERY \b IMPORTANT */
#define CRL_ENDIANNESS  1  /*!< LITTLE ENDIAN is selected */
/**
  * @}
  */ 

/**  
  * @defgroup  MisalignedConf Misaligned words read/write operations.
  * @{
  */
/*! Used to specify if the processor supports misaligned integers read/write operations. \n
  * To be portable this flag is not defined but if the processor supports read/write operations of 4
  * bytes to address not multiple by 4 then setting this flag to 1 will improve the performance
  * of AES when used through high level functions (ex AES_CBC or crl_AES).
  *******************************************************************/
/*#define CRL_CPU_SUPPORT_MISALIGNED*/ /*!< improve the performance of AES when used through high level functions */
 /**
  * @}
  */ 

/** 
  * @defgroup EncDecConf Encryption and/or Decryption functionalities
  * @{
  */
#define INCLUDE_ENCRYPTION  /*!< This defines includes the Encryption functionalities, remove it if only decryption is needed */

/**
  * @}
  */ 

/** 
  * @defgroup SymKeyConf Symmetric Key Algorithms
  * @{
  */

#define INCLUDE_AES128  /*!< This defines if AES functions with key size of 128 bit are included in the library \n If it's \b NOT defined then aes128.c is not needed. */ 
/**
  * @}
  */ 


/**
  * @defgroup SymKeyModesConf Modes of Operations for Symmetric Key Algorithms
  * @{
  */

#define INCLUDE_CMAC     /*!< This defines if AES-CMAC is included in the library */
/**
  * @}
  */ 

/** 
  * @defgroup AsymKeyConf Asymmetric Key Algorithms
  * @{
  */
/**
  * @}
  */ 

/** 
  * @defgroup HashConf Hash and HMAC Algorithms
  * @{
  */
 /**
  * @}
  */ 

/**
  * @defgroup DRBGConf Deterministic Random Bit Generator Based on AES-128
  * @{
  */

/**
  * @}
  */ 




/**
  * @defgroup OptiConf Configuration for Optimization
  * @{
  */

/** 
  * @defgroup AESConf AES version
  * @{
  */
/*! Used to select the AES algorithm version to use
 * - CRL_AES_ALGORITHM = 1 : version with 522 bytes of look-up tables, slower than version 2.
 * - CRL_AES_ALGORITHM = 2 : version with 2048 bytes of look-up tables, faster than version 1.*/
#define CRL_AES_ALGORITHM 1  /*!< version with 522 bytes of look-up tables is selected */
/**
  * @}
  */ 

/**
  * @defgroup RSAConf RSA Window Size
  * @{
  */ 
/*! This parameter is used to speed up RSA operation with private key at expense of RAM memory.
  * It can't be set less than one.
  * Suggested values are 3 or 4. Entering a value of 7 or more will be probably worst than using 6.*/
#define RSA_WINDOW_SIZE 4 /*!< Window size selected is 4 */
/**
  * @}
  */ 

/** 
  * @defgroup GCMConf AES GCM GF(2^128) Precomputations Tables
   * @{
  */
/*! Used to specify the algorithm to be used for polynomial multiplication in AES-GCM.\n
  * The algorithm's choice also defines the size of the precomputed table made to speed
  * up the multiplication. \n There are two types of table, one is based on the value of
  * the key and so needs to be generated at runting (through AES_GCM_keyschedule), the other
  * is constant and is defined (if included here) in privkey.h.
  * There are 3 possible choices:
  *  - 0 = Without any tables. No space required. Slower version.
  *  - 1 = Key-dependent table for *Poly(y) 0000<y<1111 and constant table for *x^4 \n
  *       (256 key-dependent bytes - 32 constant bytes)
  *  - 2 = 4 key-dependent tables for *Poly(y^(2^(32*i))) and 4 key-dependent tables for *Poly((y*x^4)^(2^(32*i)))
  *        with 0000<y<1111 and 0<i<4 and constant tables for *x^8 and for *x^4\n
  *       (2048 key-dependent bytes - 544 constant bytes) */  
#define CRL_GFMUL 2 /*!< 4 key-dependent tables is selected */
/**
  * @}
  */ 

/**
  * @} End of optimization
  */ 

/**
  * @} End of options
  */ 

#endif  /*__CRL_CONFIG_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
