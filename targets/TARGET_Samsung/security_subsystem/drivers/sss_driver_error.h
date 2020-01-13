/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef SSS_DRIVER_ERROR_H
#define SSS_DRIVER_ERROR_H

/*************** Include Files ********************************************/
#include "sss_common.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

#define FROM_FW                                 (0x0F)
#define FROM_DRV                                (0x0D)

//! Common Error Code
#define ERROR_AES                               (0x00001000)
#define ERROR_DH                                (0x00009000)
#define ERROR_ECC                               (0x00006000)
#define ERROR_ECDSA                             (0x00007000)
#define ERROR_ECDH                              (0x00008000)
#define ERROR_PKA                               (0x0000E000)
#define ERROR_RSA                               (0x00005000)
#define ERROR_RNG                               (0x00004000)
#define ERROR_RNG_TRNG                          (0x00004100)
#define ERROR_SHA                               (0x00002000)
#define ERROR_HMAC                              (0x00003000)
#define ERROR_SM3                               (0x0000C200)
#define ERROR_SM3_HMAC                          (0x0000C300)
#define ERROR_SM4                               (0x0000C100)
#define ERROR_KM                                (0x0000A000)
#define ERROR_SWDT1                             (0x0000D100)
#define ERROR_SWDT2                             (0x0000D200)
#define ERROR_FEEDER                            (0x0000F000)
#define INVALID_LEN                             (0x00010000)
#define INVALID_SEL                             (0x00FE0000)
#define INVALID_VAL                             (0x00FF0000)
#define INVALID_STS                             (0x00FD0000)
#define INVALID_SZ                              (0x00FC0000)
#define INVALID_FORM                            (0x00FB0000)
#define INVALID_FUNCID00                        (0x00F00000)
#define INVALID_FUNCID01                        (0x00F10000)
#define INVALID_FUNCID02                        (0x00F20000)
#define INVALID_FUNCID03                        (0x00F30000)
#define INVALID_ORDER                           (0x00FA0000)
#define INVALID_STATUS                          (0x00190000)
#define ERR_CNT                                 (0x01000000)
#define ERR_IV                                  (0x02000000)
#define ERR_TAG                                 (0x03000000)
#define ERR_KEY                                 (0x04000000)
#define ERR_BLOCK                               (0x05000000)
#define ERR_MSG                                 (0x06000000)
#define ERR_MODE                                (0x07000000)
#define ERR_OID_ALG                             (0x08000000)
#define ERR_OID_SIZE                            (0x09000000)
#define ERR_SIGNATURE                           (0x0A000000)
#define ERR_PUBLICKEY                           (0x0B000000)
#define ERR_PRIVKEY                             (0x1B000000)
#define ERR_ROLLBACK_CNT                        (0x0C000000)
#define ERR_SIGNER_VER                          (0x0D000000)
#define ERR_FW                                  (0x0E000000)
#define ERR_OUTPUT                              (0x0F000000)
#define ERR_BUSY                                (0x10000000)
#define ERR_CRNGT                               (0x11000000)
#define ERR_SLOT                                (0x12000000)
#define ERR_KEYMODE                             (0x13000000)
#define ERR_KAT                                 (0x14000000)
#define ERR_HT                                  (0x15000000)
#define ERR_RANDOM                              (0x16000000)
#define ERR_SALT                                (0x17000000)
#define ERR_PUF                                 (0x18000000)
#define ERR_ERR_RATE                            (0x19000000)
#define ERR_MB_CMD                              (0x20000000)
#define ERR_AAD                                 (0x21000000)
#define ERR_LABEL                               (0x30000000)
#define ERR_HW                                  (0xFF000000)
#define ERR_DECRYPTION                          (0xDD000000)

//! Error From DRV
// System
#define ERROR_SYSTEM_MAILBOX_BUSY               (FROM_DRV | (ERR_MB_CMD | INVALID_STATUS | ERR_BUSY))
#define ERROR_INVALID_VAL_OID                   (FROM_DRV | (INVALID_VAL | ERR_OID_ALG))
// AES
#define ERROR_AES_INVALID_LEN_BLOCK             (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_BLOCK))
#define ERROR_AES_INVALID_LEN_RSP_BLOCK         (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_OUTPUT))
#define ERROR_AES_INVALID_LEN_MSG               (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_MSG))
#define ERROR_AES_INVALID_VAL_MODE              (FROM_DRV | (ERROR_AES | INVALID_VAL | ERR_MSG))
#define ERROR_AES_INVALID_LEN_KEY               (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_KEY))
#define ERROR_AES_INVALID_LEN_AAD               (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_AAD))
#define ERROR_AES_INVALID_LEN_IV                (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_IV))
#define ERROR_AES_INVALID_LEN_CNT               (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_CNT))
#define ERROR_AES_INVALID_LEN_TAG               (FROM_DRV | (ERROR_AES | INVALID_LEN | ERR_TAG))
#define ERROR_AES_INVALID_VAL_TAG               (FROM_DRV | (ERROR_AES | INVALID_VAL | ERR_TAG))
// SM4
#define ERROR_SM4_INVALID_LEN_BLOCK             (FROM_DRV | (ERROR_SM4 | INVALID_LEN | ERR_BLOCK))
#define ERROR_SM4_INVALID_LEN_RSP_BLOCK         (FROM_DRV | (ERROR_SM4 | INVALID_LEN | ERR_OUTPUT))
#define ERROR_SM4_INVALID_LEN_MSG               (FROM_DRV | (ERROR_SM4 | INVALID_LEN | ERR_MSG))
#define ERROR_SM4_INVALID_VAL_MODE              (FROM_DRV | (ERROR_SM4 | INVALID_VAL | ERR_MODE))
#define ERROR_SM4_INVALID_LEN_KEY               (FROM_DRV | (ERROR_SM4 | INVALID_LEN | ERR_KEY))
// Hash
#define ERROR_HASH_INVALID_LEN_BLOCK            (FROM_DRV | (ERROR_SHA | INVALID_LEN | ERR_BLOCK))
#define ERROR_HASH_INVALID_LEN_MSG              (FROM_DRV | (ERROR_SHA | INVALID_LEN | ERR_MSG))
#define ERROR_HASH_INVALID_VAL_MODE             (FROM_DRV | (ERROR_SHA | INVALID_LEN | ERR_MODE))
// HMAC
#define ERROR_HMAC_INVALID_LEN_BLOCK            (FROM_DRV | (ERROR_HMAC | INVALID_LEN | ERR_BLOCK))
#define ERROR_HMAC_INVALID_LEN_MSG              (FROM_DRV | (ERROR_HMAC | INVALID_LEN | ERR_MSG))
#define ERROR_HMAC_INVALID_VAL_MODE             (FROM_DRV | (ERROR_HMAC | INVALID_LEN | ERR_MODE))
#define ERROR_HMAC_INVALID_LEN_KEY              (FROM_DRV | (ERROR_HMAC | INVALID_LEN | ERR_KEY))
// SM3
#define ERROR_SM3_INVALID_LEN_BLOCK             (FROM_DRV | (ERROR_SM3 | INVALID_LEN | ERR_BLOCK))
#define ERROR_SM3_INVALID_LEN_MSG               (FROM_DRV | (ERROR_SM3 | INVALID_LEN | ERR_MSG))
#define ERROR_SM3_INVALID_VAL_MODE              (FROM_DRV | (ERROR_SM3 | INVALID_LEN | ERR_MODE))
// DH
#define ERROR_DH_INVALID_LEN_PUBKEY             (FROM_DRV | (ERROR_DH | INVALID_LEN | ERR_PUBLICKEY))
#define ERROR_DH_INVALID_LEN_PRIVKEY            (FROM_DRV | (ERROR_DH | INVALID_LEN | ERR_PRIVKEY))
#define ERROR_DH_INVALID_LEN_KEY                (FROM_DRV | (ERROR_DH | INVALID_LEN | ERR_OUTPUT))
// ECDSA
#define ERROR_ECDSA_INVALID_LEN_MSG             (FROM_DRV | (ERROR_ECDSA | INVALID_LEN | ERR_MSG))
#define ERROR_ECDSA_INVALID_LEN_SIGNATURE       (FROM_DRV | (ERROR_ECDSA | INVALID_LEN | ERR_SIGNATURE))
#define ERROR_ECDSA_INVALID_LEN_PUBKEY          (FROM_DRV | (ERROR_ECDSA | INVALID_LEN | ERR_PUBLICKEY))
#define ERROR_ECDSA_INVALID_LEN_PRIVKEY         (FROM_DRV | (ERROR_ECDSA | INVALID_LEN | ERR_PRIVKEY))
//ECDH
#define ERROR_ECDH_INVALID_LEN_PUBKEY           (FROM_DRV | (ERROR_ECDH | INVALID_LEN | ERR_PUBLICKEY))
#define ERROR_ECDH_INVALID_LEN_PRIVKEY          (FROM_DRV | (ERROR_ECDH | INVALID_LEN | ERR_PRIVKEY))
#define ERROR_ECDH_INVALID_LEN_KEY              (FROM_DRV | (ERROR_ECDH | INVALID_LEN | ERR_OUTPUT))
// RSASSA, RSAES
#define ERROR_RSA_INVALID_LEN_MSG               (FROM_DRV | (ERROR_RSA | INVALID_LEN | ERR_MSG))
#define ERROR_RSA_INVALID_LEN_SIGNATURE         (FROM_DRV | (ERROR_RSA | INVALID_LEN | ERR_SIGNATURE))
#define ERROR_RSA_INVALID_LEN_PUBKEY            (FROM_DRV | (ERROR_RSA | INVALID_LEN | ERR_PUBLICKEY))
#define ERROR_RSA_INVALID_LEN_PRIVKEY           (FROM_DRV | (ERROR_RSA | INVALID_LEN | ERR_PRIVKEY))
#define ERROR_RSA_INVALID_LEN_CIPHER            (FROM_DRV | (ERROR_RSA | INVALID_LEN | ERR_OUTPUT))
#define ERROR_RSA_INVALID_LEN_LABEL             (FROM_DRV | (ERROR_RSA | INVALID_LEN | ERR_LABEL))
// RNG
#define ERROR_RNG_INVALID_LEN_REQUEST           (FROM_DRV | (ERROR_RNG | INVALID_LEN | ERR_OUTPUT))

//! Error From FW
#define FW_ERROR_AES_INVALID_VAL_OID            (FROM_FW | (ERROR_AES | INVALID_VAL | ERR_OID_ALG))
#define FW_ERROR_AES_INVALID_LEN_MSG            (FROM_FW | (ERROR_AES | INVALID_LEN | ERR_MSG))
#define FW_ERROR_AES_INVALID_STS_KEYMODE        (FROM_FW | (ERROR_AES | INVALID_STS | ERR_KEYMODE))
#define FW_ERROR_AES_INVALID_LEN_KEY            (FROM_FW | (ERROR_AES | INVALID_LEN | ERR_KEY))
#define FW_ERROR_AES_INVALID_VAL_TAG            (FROM_FW | (ERROR_AES | INVALID_VAL | ERR_TAG))
#define FW_ERROR_AES_INVALID_LEN_TAG            (FROM_FW | (ERROR_AES | INVALID_LEN | ERR_TAG))
#define FW_ERROR_AES_INVALID_LEN_IV             (FROM_FW | (ERROR_AES | INVALID_LEN | ERR_IV))
#define FW_ERROR_AES_INVALID_LEN_AAD            (FROM_FW | (ERROR_AES | INVALID_LEN | ERR_AAD))
#define FW_ERROR_AES_INVALID_LEN_CNT            (FROM_FW | (ERROR_AES | INVALID_LEN | ERR_CNT))
#define FW_ERROR_DH_INVALID_VAL_OID             (FROM_FW | (ERROR_DH | INVALID_VAL | ERR_OID_ALG))
#define FW_ERROR_DH_INVALID_VAL_KEY             (FROM_FW | (ERROR_DH | INVALID_VAL | ERR_KEY))
#define FW_ERROR_DH_RNG_ERROR                   (FROM_FW | (ERROR_DH | INVALID_VAL | ERR_RANDOM))
#define FW_ERROR_ECC_INVALID_VAL_OID            (FROM_FW | (ERROR_ECC | INVALID_VAL | ERR_OID_ALG))
#define FW_ERROR_ECDSA_INVALID_VAL_KEY          (FROM_FW | (ERROR_ECDSA | INVALID_VAL | ERR_KEY))
#define FW_ERROR_ECDH_INVALID_VAL_KEY           (FROM_FW | (ERROR_ECDH | INVALID_VAL | ERR_KEY))
#define FW_ERROR_ECDSA_INVALID_VAL_SIGN         (FROM_FW | (ERROR_ECDSA | INVALID_VAL | ERR_SIGNATURE))
#define FW_ERROR_ECDSA_RNG_ERROR                (FROM_FW | (ERROR_ECDSA | INVALID_VAL | ERR_RANDOM))
#define FW_ERROR_ECDH_RNG_ERROR                 (FROM_FW | (ERROR_ECDH | INVALID_VAL | ERR_RANDOM))
#define FW_ERROR_PKA_SFR_SET                    (FROM_FW | (ERROR_PKA|INVALID_VAL|ERR_HW))
#define FW_ERROR_RNG_TRNG_INVALID_CLKDIV        (FROM_FW | (ERROR_RNG_TRNG | INVALID_VAL | ERR_HT))
#define FW_ERROR_RNG_TRNG_KAT_ERROR             (FROM_FW | (ERROR_RNG_TRNG | INVALID_STS | ERR_KAT))
#define FW_ERROR_RNG_TRNG_HT_ERROR              (FROM_FW | (ERROR_RNG_TRNG | INVALID_STS | ERR_HT))
#define FW_ERROR_RSA_INVALID_VAL_OID            (FROM_FW | (ERROR_RSA | INVALID_VAL | ERR_OID_ALG))
#define FW_ERROR_RSA_INVALID_LEN_MESSAGE        (FROM_FW | (ERROR_RSA | INVALID_LEN | ERR_MSG))
#define FW_ERROR_RSA_FAIL_DECRYPTION            (FROM_FW | (ERROR_RSA | INVALID_STS | ERR_DECRYPTION))
#define FW_ERROR_RSA_INVALID_LEN_SALT           (FROM_FW | (ERROR_RSA | INVALID_LEN | ERR_SALT))
#define FW_ERROR_RSA_INVALID_VAL_SIGNATURE      (FROM_FW | (ERROR_RSA | INVALID_VAL | ERR_SIGNATURE))
#define FW_ERROR_RSA_INVALID_LEN_SIGNATURE      (FROM_FW | (ERROR_RSA | INVALID_LEN | ERR_SIGNATURE))
#define FW_ERROR_SHA_INVALID_VAL_OID            (FROM_FW | (ERROR_SHA | INVALID_VAL | ERR_OID_ALG))
#define FW_ERROR_HASH_INVALID_LEN_MSG           (FROM_FW | (ERROR_SHA | INVALID_LEN | ERR_MSG))
#define FW_ERROR_HMAC_INVALID_LEN_KEY           (FROM_FW | (ERROR_HMAC | INVALID_LEN | ERR_KEY))
#define FW_ERROR_SM3_INVALID_VAL_OID            (FROM_FW | (ERROR_SM3 | INVALID_VAL | ERR_OID_ALG))
#define FW_ERROR_SM3_INVALID_LEN_MSG            (FROM_FW | (ERROR_SM3 | INVALID_LEN | ERR_MSG))
#define FW_ERROR_SM3_HMAC_INVALID_LEN_KEY       (FROM_FW | (ERROR_SM3_HMAC | INVALID_LEN | ERR_KEY))
#define FW_ERROR_SM4_INVALID_VAL_OID            (FROM_FW | (ERROR_SM4 | INVALID_LEN | ERR_OID_ALG))
#define FW_ERROR_SM4_INVALID_LEN_KEY            (FROM_FW | (ERROR_SM4 | INVALID_LEN | ERR_KEY))
#define FW_ERROR_SM4_INVALID_SEL_KEY            (FROM_FW | (ERROR_SM4 | INVALID_SEL | ERR_KEY))
#define FW_ERROR_SM4_INVALID_LEN_MSG            (FROM_FW | (ERROR_SM4 | INVALID_LEN | ERR_MSG))
#define FW_ERROR_AES_IP_BUSY                    (FROM_FW | (ERROR_AES | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_SM4_IP_BUSY                    (FROM_FW | (ERROR_SM4 | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_SM3_IP_BUSY                    (FROM_FW | (ERROR_SM3 | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_HASH_IP_BUSY                   (FROM_FW | (ERROR_SHA | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_TRNG_IP_BUSY                   (FROM_FW | (ERROR_RNG | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_PKA_IP_BUSY                    (FROM_FW | (ERROR_PKA | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_KM_IP_BUSY                     (FROM_FW | (ERROR_KM  | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_SWDT1_IP_BUSY                  (FROM_FW | (ERROR_SWDT1 | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_SWDT2_IP_BUSY                  (FROM_FW | (ERROR_SWDT2 | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_FEEDER_IP_BUSY                 (FROM_FW | (ERROR_FEEDER | INVALID_STATUS | ERR_BUSY))
#define FW_ERROR_INVALID_FUNCID00               (FROM_FW | (ERR_MB_CMD | INVALID_FUNCID00))
#define FW_ERROR_INVALID_FUNCID01               (FROM_FW | (ERR_MB_CMD | INVALID_FUNCID01))
#define FW_ERROR_INVALID_FUNCID02               (FROM_FW | (ERR_MB_CMD | INVALID_FUNCID02))
#define FW_ERROR_INVALID_FUNCID03               (FROM_FW | (ERR_MB_CMD | INVALID_FUNCID03))
#define FW_ERROR_EXECUTION_ORDER                (FROM_FW | (ERR_MB_CMD | INVALID_ORDER))

// Secure Storage
#define ERROR_SSTORAGE_INVALID_SLOT_INDEX                   (0x00F1A1D1)
#define ERROR_SSTORAGE_INVALID_DATA_LEN                     (0x00F2A1D1)
#define ERROR_SSTORAGE_INVALID_TYPE                         (0x00F3A1D1)
#define ERROR_SSTORAGE_DATA_INVALID_DATA_LEN                (0x0002A1D1)
#define ERROR_SSTORAGE_CERT_INVALID_DATA_LEN                (0x0003A2D1)
#define ERROR_SSTORAGE_KEY_INVALID_DATA_LEN                 (0x0004A3D1)
#define ERROR_SSTORAGE_KEY_INVALID_KEY_LEN                  (0x0005A3D1)
#define ERROR_SSTORAGE_KEY_INVALID_KEY_TYPE                 (0x0006A3D1)
#define ERROR_SSTORAGE_FACTORYKEY_PBKEY_INVALID_DATA_LEN    (0x0007A2D1)
#define ERROR_SSTORAGE_WRITE                                (0x0008A2D1)
#define ERROR_SSTORAGE_READ                                 (0x0008A2D1)
#define ERROR_SSTORAGE_SFS_FOPEN                            (0x0009A2D1)
#define ERROR_SSTORAGE_SFS_FSEEK                            (0x000AA2D1)
#define ERROR_SSTORAGE_SFS_FREAD                            (0x000BA2D1)
#define ERROR_SSTORAGE_SFS_FWRITE                           (0x000CA2D1)

#if 0

// SM4
#define ERROR_SM4_INVALID_BLOCK_LEN                         (0x000134D1)
#define ERROR_SM4_INVALID_RSP_BLOCK_LEN                     (0x000234D1)
#define ERROR_SM4_INVALID_MSG_LEN                           (0x000334D1)
#define ERROR_SM4_INVALID_MODE                              (0x000434D1)
#define ERROR_SM4_INVALID_KEY_LEN                           (0x000534D1)
#define ERROR_SM4_INVALID_INDEX                             (0x000834D1)

// Hash
#define ERROR_HASH_INVALID_MODE                             (0x000131D1)
#define ERROR_HASH_INVALID_BLOCK_LEN                        (0x000231D1)
#define ERROR_HASH_INVALID_MSG_LEN                          (0x000331D1)

// HMAC
#define ERROR_HMAC_INVALID_MODE                             (0x000132D1)
#define ERROR_HMAC_INVALID_KEY_LEN                          (0x000232D1)
#define ERROR_HMAC_INVALID_BLOCK_LEN                        (0x000332D1)
#define ERROR_HMAC_INVALID_INDEX                            (0x000432D1)
#define ERROR_HMAC_INVALID_RSP_BLOCK_LEN                    (0x000532D1)

// SM3
#define ERROR_SM3_INVALID_MODE                              (0x000133D1)
#define ERROR_SM3_INVALID_BLOCK_LEN                         (0x000233D1)
#define ERROR_SM3_INVALID_MSG_LEN                           (0x000333D1)

// DH
#define ERROR_DH_INVALID_PRIME_LEN                          (0x000125D1)
#define ERROR_DH_INVALID_PUBKEY_LEN                         (0x000225D1)
#define ERROR_DH_INVALID_PRIVATEKEY_LEN                     (0x000525D1)
#define ERROR_DH_INVALID_GENERATOR_LEN                      (0x000325D1)
#define ERROR_DH_INVALID_PRIME                              (0x000425D1)
#define ERROR_DH_INVALID_PUBKEY                             (0x000525D1)
#define ERROR_DH_INVALID_GENERATOR                          (0x000625D1)

// ECDSA
#define ERROR_ECDSA_INVALID_MSG_LEN                         (0x000111D1)
#define ERROR_ECDSA_INVALID_SIGNATURE_LEN                   (0x000311D1)
#define ERROR_ECDH_INVALID_PUBKEY_LEN                       (0x000411D1)
#define ERROR_ECDH_INVALID_PUBKEY                           (0x000511D1)
#define ERROR_ECDH_INVALID_PRIVKEY_LEN                      (0x000611D1)
#define ERROR_ECDH_INVALID_PRIVKEY                          (0x000711D1)
// RSA
#define ERROR_RSA_INVALID_CIPHER_LEN                        (0x000151D1)
#define ERROR_RSA_INVALID_MSG_LEN                           (0x000251D1)
#define ERROR_RSA_INVALID_SIGN_LEN                          (0x000351D1)
#define ERROR_RSA_INVALID_PAD_SELECTION                     (0x000451D1)
#define ERROR_RSA_INVALID_PUKEY                             (0x000551D1)
#define ERROR_RSA_INVALID_PRIVKEY                           (0x000651D1)

// RNG
#define ERROR_RNG_INVALID_RANDOM_REQUEST                    (0x000161D1)

// Common
#define ERROR_SSKeyID_InputID_MISSMATCH                     (0x000171D1)
#define ERROR_INVALID_OID                                   (0x000271D1)

// System Function
#define ERROR_SYSTEM_INVALID_DATA_LEN                       (0x000201D1)
#define ERROR_SYSTEM_MAILBOX_BUSY                           (0x000100D1)

// Error from FW

#define FW_ERROR_ISP_INVALID_FUNCID00                       (0x801000F1)
#define FW_ERROR_ISP_INVALID_FUNCID01                       (0x801100F1)
#define FW_ERROR_ISP_INVALID_FUNCID02                       (0x801200F1)
#define FW_ERROR_ISP_INVALID_FUNCID03                       (0x801300F1)
#define FW_ERROR_ISP_INVALID_DATASIZE                       (0x801400F1)
#define FW_ERROR_ISP_FW_BODYSIZE                            (0x801500F1)
#define FW_ERROR_ISP_FW_ROLLBACK_CNT                        (0x801600F1)
#define FW_ERROR_ISP_FW_INVALID_PUBLICKEY                   (0x801700F1)
#define FW_ERROR_ISP_RESTORE_INTEGRITY_FAIL                 (0x801800F1)
#define FW_ERROR_ISP_IP_BUSY                                (0x801900F1)
#define FW_ERROR_ISP_SRAM_CMD_NOT_SUPPORTED                 (0x801A00F1)

#define FW_ERROR_INVALID_FUNCTION                           (0x000100f1)
#define FW_ERROR_FW_VERIFY                                  (0x001400f1)
#define FW_ERROR_RESTORE_FAIL                               (0x001800f1)
#define FW_ERROR_IP_BUSY                                    (0x001900f1)
#define FW_ERROR_INVALID_OID                                (0x003000f1)
#define FW_ERROR_INVALID_INPUT                              (0x003400f1)
#define FW_ERROR_INPUT_SETTING                              (0x004000f1)
#define FW_ERROR_PRNG                                       (0x005000f1)
#define FW_FAIL_INVALID_SIGNATURE                           (0x006000f1)
#define FW_FAIL_INFINITY_POINT                              (0x006100f1)
#define FW_FAIL_NOT_ON_ECC_CURVE                            (0x006200f1)

// SRAM error
#define FW_ERROR_INVALID_EXEC_ORDER                         (0x80AF00F1)
#define FW_ERROR_OVER_VALID_RSA_MSGLEN                      (0x803500F1)
#define FW_ERROR_INVALID_RSA_MODLEN                         (0x803600F1)
#define FW_ERROR_Input_Public_is_not_odd                    (0x803700F1)
#define FW_ERROR_OVER_VALID_RSA_Saltlen                     (0x803800F1)
#define FW_ERROR_NO_PUKEY                                   (0x803900F1)
#define FW_FAIL_OVER_MR_TRIALS                              (0x805600F1)
#define FW_FAIL_OVER_DH_RETRIALS                            (0x805600F1)
#define FW_FAIL_OVER_ECC_RETRIALS                           (0x805600F1)
#define FW_FAIL_NO_OUTPUT_KEY                               (0x805700F1)
#define FW_FAIL_OVER_GEN_RETRIALS                           (0x805800F1)
#define FW_ERROR_INVALID_RSASIGNATURE_0xBC                  (0x806400F1)
#define FW_ERROR_INVALID_RSASIGNATURE_lsb                   (0x806500F1)
#define FW_ERROR_INVALID_RSANONZERO_PS                      (0x806600F1)
#define FW_ERROR_INVALID_RSADB_SPLITTER                     (0x806700F1)
#define FW_ERROR_INVALID_SIGNATURE_BLEN                     (0x806800F1)
#define FW_ERROR_INVALID_CIPHER                             (0x806900F1)
#define FW_ERROR_INVALID_SEQUENCE                           (0x80ff00F1)
#define FW_ERROR_DER2INT_PARSE                              (0x80fe00F1)
#define FW_ERROR_AES_INVALID_KEYSEL                         (0x80D000F1)
#define FW_ERROR_AES_INVALID_KEY_LEN                        (0x80D100F1)
#define FW_ERROR_AES_INVALID_DIR_MODE                       (0x80D200F1)
#define FW_ERROR_AES_INVLIAD_SWAP                           (0x80D300F1)
#define FW_ERROR_AES_INVALID_MODE                           (0x80D400F1)
#define FW_ERROR_AES_INVALID_TAG                            (0x80D500F1)
#define FW_ERROR_AES_INVALID_BLOCK_LEN                      (0x80D600F1)
#define FW_ERROR_AES_KM_BUSY                                (0x80D700F1)
#define FW_ERROR_AES_KM_INIT                                (0x80D800F1)
#define FW_ERROR_SSTORAGE_DATA_INVALID_SLOT_INDEX           (0x80E100F1)
#define FW_ERROR_SSTORAGE_CERT_INVALID_SLOT_INDEX           (0x80E200F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_KEY_LEN               (0x80E300F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_MODE                  (0x80E400F1)
#define FW_ERROR_SSTORAGE_INVALID_DATA_TYPE                 (0x80E500F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_KEYTYPE               (0x80E600F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_SLOT_INDEX            (0x80E700F1)
#define FW_ERROR_SSTORAGE_FACTORYKEY_INVALID_HMAC           (0x80E800F1)
#define FW_ERROR_SSTORAGE_FACTORYKEY_INVALID_ENCODING       (0x80E900F1)
#define FW_ERROR_SSTORAGE_FACTORYKEY_INVALID_KEYTYPE        (0x80EA00F1)
#define FW_ERROR_RNG_INVALID_LEN                            (0x80f100F1)
#define FW_ERROR_KEYGEN_INVALID_KEYLEN                      (0x80f200F1)
#define FW_ERROR_PKA_IP_BUSY                                (0x80B100F1)
#define FW_ERROR_HASH_IP_BUSY                               (0x80B200F1)
#define FW_ERROR_PRNG_IP_BUSY                               (0x80B300F1)
#endif

#endif  /* SSS_DRIVER_ERROR_H */
