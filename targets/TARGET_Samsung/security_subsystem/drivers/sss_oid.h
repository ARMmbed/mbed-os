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
#ifndef SSS_OID_H
#define SSS_OID_H


/*************** Include Files ********************************************/

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*
 *      OID LIST
 */
#define OIDMAX_AES                  (12)
#define OID_AES_ECB                 (0x00000008)
#define OID_AES_ECB_128             (0x00100008)
#define OID_AES_ECB_192             (0x00180008)
#define OID_AES_ECB_256             (0x00200008)
#define OID_AES_CBC                 (0x00000108)
#define OID_AES_CBC_128             (0x00100108)
#define OID_AES_CBC_192             (0x00180108)
#define OID_AES_CBC_256             (0x00200108)
#define OID_AES_CTR                 (0x00000208)
#define OID_AES_CTR_128             (0x00100208)
#define OID_AES_CTR_192             (0x00180208)
#define OID_AES_CTR_256             (0x00200208)
#define OID_AES_XTS                 (0x00000308)
#define OID_AES_XTS_128             (0x00100308)
#define OID_AES_XTS_256             (0x00200308)
#define OID_AES_CCM                 (0x00001008)
#define OID_AES_CCM_128             (0x00101008)
#define OID_AES_CCM_192             (0x00181008)
#define OID_AES_CCM_256             (0x00201008)
#define OID_AES_GCM                 (0x00001108)
#define OID_AES_GCM_128             (0x00101108)
#define OID_AES_GCM_192             (0x00181108)
#define OID_AES_GCM_256             (0x00201108)
#define OID_AES_CMAC                (0x00001208)
#define OID_AES_CMAC_128            (0x00101208)
#define OID_AES_CMAC_192            (0x00181208)
#define OID_AES_CMAC_256            (0x00201208)

#define OID_AES_ENCRYPT             (0x00000000)
#define OID_AES_DECRYPT             (0x01000000)

#define OID_ENCRYPTION              (0x00000000)
#define OID_DECRYPTION              (0x01000000)

#define OIDMAX_SM4                  (1)
#define OID_SM4_ECB                 (0x00000009)
#define OID_SM4_ECB_128             (0x00100009)
#define OID_SM4_ENCRYPT             (OID_AES_ENCRYPT)
#define OID_SM4_DECRYPT             (OID_AES_DECRYPT)

#define OIDMAX_HMAC                 (4)
#define OID_HMAC_SHA1_160           (0x00011100)
#define OID_HMAC_SHA2_224           (0x00012200)    //NA
#define OID_HMAC_SHA2_256           (0x00012300)
#define OID_HMAC_SHA2_384           (0x00012400)
#define OID_HMAC_SHA2_512           (0x00012500)

#define OIDMAX_HASH                 (8)
#define OID_SHA1_160                (0x00001100)
#define OID_SHA2_224                (0x00002200)    //NA
#define OID_SHA2_256                (0x00002300)
#define OID_SHA2_384                (0x00002400)
#define OID_SHA2_512                (0x00002500)
#define OID_SHA3_224                (0x00003200)    //NA
#define OID_SHA3_256                (0x00003300)    //NA
#define OID_SHA3_384                (0x00003400)    //NA
#define OID_SHA3_512                (0x00003500)    //NA

#define OIDMAX_HMACSM3              (1)
#define OID_HMAC_SM3_256            (0x01012300)

#define OIDMAX_SM3                  (1)
#define OID_SM3_256                 (0x01002300)

#define OIDMAX_DH                   (7)
#define OID_DH_R5114_1024_160       (0x00001151)
#define OID_DH_R5114_2048_224       (0x00003252)
#define OID_DH_R5114_2048_256       (0x00003352)
#define OID_DH_R2409_1024           (0x00001131)
#define OID_DH_R3526_2048           (0x00003332)
#define OID_DH_R5054_3072           (0x00004033)
#define OID_DH_R5054_4096           (0x00005034)

#define OIDMAX_ECC                  (10)
#define OID_ECC_P192                (0x00000011)
#define OID_ECC_P224                (0x00000012)
#define OID_ECC_P256                (0x00000013)
#define OID_ECC_P384                (0x00000014)
#define OID_ECC_P521                (0x00000015)
#define OID_ECC_BP192               (0x00000051)
#define OID_ECC_BP224               (0x00000052)
#define OID_ECC_BP256               (0x00000053)
#define OID_ECC_BP384               (0x00000054)
#define OID_ECC_BP512               (0x00000055)
#define OID_ECC_25519               (0x00000093)

#define OIDMAX_RSA                  (2)
#define OID_RSA_1024                (0x000000B1)
#define OID_RSA_2048                (0x000000B2)
#define OID_RSA_3072                (0x000000B3)
#define OID_RSA_4096                (0x000000B4)

#define OIDMAX_ECDSA                (40)
#define OID_ECDSA_BP192_SHA1_160    (OID_ECC_BP192|OID_SHA1_160)
#define OID_ECDSA_BP192_SHA2_256    (OID_ECC_BP192|OID_SHA2_256)
#define OID_ECDSA_BP192_SHA2_384    (OID_ECC_BP192|OID_SHA2_384)
#define OID_ECDSA_BP192_SHA2_512    (OID_ECC_BP192|OID_SHA2_512)
#define OID_ECDSA_BP224_SHA1_160    (OID_ECC_BP224|OID_SHA1_160)
#define OID_ECDSA_BP224_SHA2_256    (OID_ECC_BP224|OID_SHA2_256)
#define OID_ECDSA_BP224_SHA2_384    (OID_ECC_BP224|OID_SHA2_384)
#define OID_ECDSA_BP224_SHA2_512    (OID_ECC_BP224|OID_SHA2_512)
#define OID_ECDSA_BP256_SHA1_160    (OID_ECC_BP256|OID_SHA1_160)
#define OID_ECDSA_BP256_SHA2_256    (OID_ECC_BP256|OID_SHA2_256)
#define OID_ECDSA_BP256_SHA2_384    (OID_ECC_BP256|OID_SHA2_384)
#define OID_ECDSA_BP256_SHA2_512    (OID_ECC_BP256|OID_SHA2_512)
#define OID_ECDSA_BP384_SHA1_160    (OID_ECC_BP384|OID_SHA1_160)
#define OID_ECDSA_BP384_SHA2_256    (OID_ECC_BP384|OID_SHA2_256)
#define OID_ECDSA_BP384_SHA2_384    (OID_ECC_BP384|OID_SHA2_384)
#define OID_ECDSA_BP384_SHA2_512    (OID_ECC_BP384|OID_SHA2_512)
#define OID_ECDSA_BP512_SHA1_160    (OID_ECC_BP512|OID_SHA1_160)
#define OID_ECDSA_BP512_SHA2_256    (OID_ECC_BP512|OID_SHA2_256)
#define OID_ECDSA_BP512_SHA2_384    (OID_ECC_BP512|OID_SHA2_384)
#define OID_ECDSA_BP512_SHA2_512    (OID_ECC_BP512|OID_SHA2_512)

#define OID_ECDSA_P192_SHA1_160     (OID_ECC_P192|OID_SHA1_160)
#define OID_ECDSA_P192_SHA2_256     (OID_ECC_P192|OID_SHA2_256)
#define OID_ECDSA_P192_SHA2_384     (OID_ECC_P192|OID_SHA2_384)
#define OID_ECDSA_P192_SHA2_512     (OID_ECC_P192|OID_SHA2_512)
#define OID_ECDSA_P224_SHA1_160     (OID_ECC_P224|OID_SHA1_160)
#define OID_ECDSA_P224_SHA2_256     (OID_ECC_P224|OID_SHA2_256)
#define OID_ECDSA_P224_SHA2_384     (OID_ECC_P224|OID_SHA2_384)
#define OID_ECDSA_P224_SHA2_512     (OID_ECC_P224|OID_SHA2_512)
#define OID_ECDSA_P256_SHA1_160     (OID_ECC_P256|OID_SHA1_160)
#define OID_ECDSA_P256_SHA2_256     (OID_ECC_P256|OID_SHA2_256)
#define OID_ECDSA_P256_SHA2_384     (OID_ECC_P256|OID_SHA2_384)
#define OID_ECDSA_P256_SHA2_512     (OID_ECC_P256|OID_SHA2_512)
#define OID_ECDSA_P384_SHA1_160     (OID_ECC_P384|OID_SHA1_160)
#define OID_ECDSA_P384_SHA2_256     (OID_ECC_P384|OID_SHA2_256)
#define OID_ECDSA_P384_SHA2_384     (OID_ECC_P384|OID_SHA2_384)
#define OID_ECDSA_P384_SHA2_512     (OID_ECC_P384|OID_SHA2_512)
#define OID_ECDSA_P521_SHA1_160     (OID_ECC_P521|OID_SHA1_160)
#define OID_ECDSA_P521_SHA2_256     (OID_ECC_P521|OID_SHA2_256)
#define OID_ECDSA_P521_SHA2_384     (OID_ECC_P521|OID_SHA2_384)
#define OID_ECDSA_P521_SHA2_512     (OID_ECC_P521|OID_SHA2_512)

#define OIDMAX_RSASSA               (8)
#define OID_RSASSA_PADDING_PKCS     (0x01)
#define OID_RSASSA_PADDING_PSS      (0x00)
#define OID_RSASSA_1024_SHA1_160    (OID_RSA_1024|OID_SHA1_160)
#define OID_RSASSA_1024_SHA2_256    (OID_RSA_1024|OID_SHA2_256)
#define OID_RSASSA_1024_SHA2_384    (OID_RSA_1024|OID_SHA2_384)
#define OID_RSASSA_1024_SHA2_512    (OID_RSA_1024|OID_SHA2_512)
#define OID_RSASSA_2048_SHA1_160    (OID_RSA_2048|OID_SHA1_160)
#define OID_RSASSA_2048_SHA2_256    (OID_RSA_2048|OID_SHA2_256)
#define OID_RSASSA_2048_SHA2_384    (OID_RSA_2048|OID_SHA2_384)
#define OID_RSASSA_2048_SHA2_512    (OID_RSA_2048|OID_SHA2_512)

#define OIDMAX_RSAES                (8)
#define OID_RSAES_PADDING_PKCS      (0x01)
#define OID_RSAES_PADDING_OAEP      (0x00)
#define OID_RSAES_1024_SHA1_160     (OID_RSA_1024|OID_SHA1_160)
#define OID_RSAES_1024_SHA2_256     (OID_RSA_1024|OID_SHA2_256)
#define OID_RSAES_1024_SHA2_384     (OID_RSA_1024|OID_SHA2_384)
#define OID_RSAES_1024_SHA2_512     (OID_RSA_1024|OID_SHA2_512)
#define OID_RSAES_2048_SHA1_160     (OID_RSA_2048|OID_SHA1_160)
#define OID_RSAES_2048_SHA2_256     (OID_RSA_2048|OID_SHA2_256)
#define OID_RSAES_2048_SHA2_384     (OID_RSA_2048|OID_SHA2_384)
#define OID_RSAES_2048_SHA2_512     (OID_RSA_2048|OID_SHA2_512)


/*************** Macros *****************************************/
//ECC
#define GET_ECC_CURVE(OID)          ((OID) & 0x07)
#define GET_ECC_ALG(OID)            ((OID >> 4) & 0x0F)
#define Is_ECC_BP(OID)              (OID & 0x40)

//RSA
#define GET_RSA_Keyblen(OID)        ((OID & 0xF) << 10) //1024 or 2048
#define GET_RSA_KeyBytelen(OID)     ((OID & 0xF) << 7)  //128, 256
#define GET_RSA_Type(OID)           ((OID >> 4) & 0xF)  //B(ENC) or C(SIGN)

//Hash
#define GET_HASH_SIZE(OID)          ((OID >> 8) & 0x07)
#define GET_HASH_ALG(OID)           ((OID >> 12) & 0x0F)

//HMAC
#define Is_HMAC_ALG(OID)            ((OID >> 16) & 0x0F)

//AES
#define GET_AES_MODE(OID)           (OID & 0xffff)
#define GET_AES_SIZE(OID)           ((OID >> 16) & 0xFF)
#define GET_ENC_DIRECTION(OID)      ((OID >> 24) & 0x0F)

//DH
#define IS_NOT_DHPARAM_R5114(OID)   (((OID >> 4) & 0xf) ^ 0x5)
#define GET_DHPRIME_SIZE(OID)       ((OID >> 12) & 0xf)
#define GET_DHORDER_SIZE(OID)       ((OID >> 8) & 0xf)



#define OIDMAX_MILENAGE             (1)
#define OID_MILENAGE                (0x00080000)
#define OID_F1                      (0x01000000)
#define OID_F1S                     (0x02000000)
#define OID_F2                      (0x03000000)
#define OID_F3                      (0x04000000)
#define OID_F4                      (0x05000000)
#define OID_F5                      (0x06000000)
#define OID_F5S                     (0x07000000)
#define OID_MILENAGE_F1             (OID_MILENAGE | OID_F1)
#define OID_MILENAGE_F1S            (OID_MILENAGE | OID_F1S)
#define OID_MILENAGE_F2             (OID_MILENAGE | OID_F2)
#define OID_MILENAGE_F3             (OID_MILENAGE | OID_F3)
#define OID_MILENAGE_F4             (OID_MILENAGE | OID_F4)
#define OID_MILENAGE_F5             (OID_MILENAGE | OID_F5)
#define OID_MILENAGE_F5S            (OID_MILENAGE | OID_F5S)



#endif  /* SSS_OID_H */
