/***************************************************************************//**
* \file cy_crypto_core_rsa.c
* \version 2.40
*
* \brief
*  This file provides the source code to the API to calculate
*  a signature by the RSA method in the Crypto block driver.
*
********************************************************************************
* Copyright 2016-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

#include "cy_crypto_core_rsa.h"
#include "cy_crypto_core_vu.h"
#include "cy_crypto_core_mem.h"
#include "cy_syslib.h"
#include <string.h>

#define CY_CRYPTO_SHA1_PADDING_SIZE        (15u)
#define CY_CRYPTO_SHA256_512_PADDING_SIZE  (19u)

#if (CPUSS_CRYPTO_VU == 1)

/* Functions prototypes */
static void Cy_Crypto_Core_Rsa_MontCoeff(CRYPTO_Type *base, uint32_t modDerReg, uint32_t modReg, uint32_t size);
static void Cy_Crypto_Core_Rsa_BarrettGetU(CRYPTO_Type *base, uint32_t barrettUReg, uint32_t modReg, uint32_t size);
static void Cy_Crypto_Core_Rsa_MontTransform(CRYPTO_Type *base, uint32_t z, uint32_t a, uint32_t barrett, uint32_t mod, uint32_t size);
static void Cy_Crypto_Core_Rsa_MontMul(CRYPTO_Type *base, uint32_t z, uint32_t a, uint32_t b, uint32_t montModDer, uint32_t mod, uint32_t size);

static void Cy_Crypto_Core_Rsa_expModByMont(CRYPTO_Type *base,
                                     uint32_t y,
                                     uint32_t x,
                                     uint32_t e,
                                     uint32_t n,
                                     uint32_t barretCoef,
                                     uint32_t inverseModulo,
                                     uint32_t rBar,
                                     uint32_t size);

#endif /* #if (CPUSS_CRYPTO_VU == 1) */

/**
* \addtogroup group_crypto_lld_asymmetric_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_Verify
****************************************************************************//**
*
* RSA verification with checks for content, paddings and signature format.
* SHA digest of the message and decrypted message should be calculated before.
* Supports only PKCS1-v1_5 format, inside of this format supported padding
* using only SHA, cases with MD2 and MD5 are not supported.
* PKCS1-v1_5 described here, page 31:
* http://www.emc.com/collateral/white-papers/h11300-pkcs-1v2-2-rsa-cryptography-standard-wp.pdf
*
* Returns the verification result \ref cy_en_crypto_rsa_ver_result_t.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param verResult
* The pointer to the verification result \ref cy_en_crypto_rsa_ver_result_t.
*
* \param digestType
* SHA mode used for hash calculation \ref cy_en_crypto_sha_mode_t.
*
* \param digest
* The pointer to the hash of the message whose signature is to be verified.
*
* \param decryptedSignature
* The pointer to the decrypted signature to be verified.
*
* \param decryptedSignatureLength
* The length of the decrypted signature to be verified (in bytes)
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Rsa_Verify(CRYPTO_Type *base,
                            cy_en_crypto_rsa_ver_result_t *verResult,
                            cy_en_crypto_sha_mode_t digestType,
                            uint8_t const *digest,
                            uint8_t const *decryptedSignature,
                            uint32_t decryptedSignatureLength)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_SUCCESS;

    /* Encodings for hash functions */

    #if (CPUSS_CRYPTO_SHA1 == 1)
    static const uint8_t sha1EncStr[CY_CRYPTO_SHA1_PADDING_SIZE] =
    {
        0x30u, 0x21u, 0x30u, 0x09u, 0x06u, 0x05u, 0x2Bu, 0x0Eu,
        0x03u, 0x02u, 0x1Au, 0x05u, 0x00u, 0x04u, 0x14u
    };
    #endif /* #if (CPUSS_CRYPTO_SHA1 == 1) */

    #if (CPUSS_CRYPTO_SHA256 == 1)
    static const uint8_t sha224EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
    {
        0x30u, 0x2Du, 0x30u, 0x0Du, 0x06u, 0x09u, 0x60u, 0x86u,
        0x48u, 0x01u, 0x65u, 0x03u, 0x04u, 0x02u, 0x04u, 0x05u,
        0x00u, 0x04u, 0x1Cu
    };

    static const uint8_t sha256EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
    {
        0x30u, 0x31u, 0x30u, 0x0Du, 0x06u, 0x09u, 0x60u, 0x86u,
        0x48u, 0x01u, 0x65u, 0x03u, 0x04u, 0x02u, 0x01u, 0x05u,
        0x00u, 0x04u, 0x20u
    };
    #endif /* #if (CPUSS_CRYPTO_SHA256 == 1) */

    #if (CPUSS_CRYPTO_SHA512 == 1)
    static const uint8_t sha384EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
    {
        0x30u, 0x41u, 0x30u, 0x0Du, 0x06u, 0x09u, 0x60u, 0x86u,
        0x48u, 0x01u, 0x65u, 0x03u, 0x04u, 0x02u, 0x02u, 0x05u,
        0x00u, 0x04u, 0x30u
    };

    static const uint8_t sha512EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
    {
        0x30u, 0x51u, 0x30u, 0x0Du, 0x06u, 0x09u, 0x60u, 0x86u,
        0x48u, 0x01u, 0x65u, 0x03u, 0x04u, 0x02u, 0x03u, 0x05u,
        0x00u, 0x04u, 0x40u
    };

    static const uint8_t sha512_224EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
    {
        0x30u, 0x2Du, 0x30u, 0x0Du, 0x06u, 0x09u, 0x60u, 0x86u,
        0x48u, 0x01u, 0x65u, 0x03u, 0x04u, 0x02u, 0x05u, 0x05u,
        0x00u, 0x04u, 0x1Cu
    };

    static const uint8_t sha512_256EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
    {
        0x30u, 0x31u, 0x30u, 0x0Du, 0x06u, 0x09u, 0x60u, 0x86u,
        0x48u, 0x01u, 0x65u, 0x03u, 0x04u, 0x02u, 0x06u, 0x05u,
        0x00u, 0x04u, 0x20u
    };
    #endif /* #if (CPUSS_CRYPTO_SHA512 == 1) */

    uint8_t  const *encodingArr = NULL;
    uint32_t encodingArrSize = 0u;
    uint32_t locDigestSize = 0u;
    uint32_t i;
    uint32_t psLength;
    uint32_t cmpRes = 0u;


    switch (digestType)
    {

#if (CPUSS_CRYPTO_SHA1 == 1)
    case CY_CRYPTO_MODE_SHA1:
        encodingArr  = sha1EncStr;
        encodingArrSize = sizeof(sha1EncStr);
        locDigestSize      = CY_CRYPTO_SHA1_DIGEST_SIZE;
        break;
#endif /* #if (CPUSS_CRYPTO_SHA1 == 1) */

#if (CPUSS_CRYPTO_SHA256 == 1)
    case CY_CRYPTO_MODE_SHA224:
        encodingArr  = sha224EncStr;
        encodingArrSize = sizeof(sha224EncStr);
        locDigestSize      = CY_CRYPTO_SHA224_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA256:
        encodingArr  = sha256EncStr;
        encodingArrSize = sizeof(sha256EncStr);
        locDigestSize      = CY_CRYPTO_SHA256_DIGEST_SIZE;
        break;
#endif /* #if (CPUSS_CRYPTO_SHA256 == 1) */

#if (CPUSS_CRYPTO_SHA512 == 1)
    case CY_CRYPTO_MODE_SHA384:
        encodingArr  = sha384EncStr;
        encodingArrSize = sizeof(sha384EncStr);
        locDigestSize      = CY_CRYPTO_SHA384_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512:
        encodingArr  = sha512EncStr;
        encodingArrSize = sizeof(sha512EncStr);
        locDigestSize      = CY_CRYPTO_SHA512_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512_224:
        encodingArr  = sha512_224EncStr;
        encodingArrSize = sizeof(sha512_224EncStr);
        locDigestSize      = CY_CRYPTO_SHA512_224_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512_256:
        encodingArr  = sha512_256EncStr;
        encodingArrSize = sizeof(sha512_256EncStr);
        locDigestSize      = CY_CRYPTO_SHA512_256_DIGEST_SIZE;
        break;
#endif /* #if (CPUSS_CRYPTO_SHA512 == 1) */

    default:
    /* Unknown Digest Type */
        break;
    }

    /* Fail by default */
    *verResult = CY_CRYPTO_RSA_VERIFY_FAIL;

    /* Check size of decrypted message */
    if (decryptedSignatureLength < (encodingArrSize + locDigestSize + 11u))
    {
        cmpRes = 1u;  /* further checking is not needed */
    }

    psLength = decryptedSignatureLength - locDigestSize - encodingArrSize - 3u;

    /* Check whether the begin of message is 0x00, 0x01 and after PS string (before T string) is 0x00 byte.*/
    if ( (0u != cmpRes) ||
         (0x00u != *(decryptedSignature)) ||
         (0x01u != *(decryptedSignature + 1)) ||
         (0x00u != *(decryptedSignature + psLength + 2u)) )
    {
        cmpRes = 1u;  /* Further checking is not needed */
    }

    /* Check FFs */
    if (0u == cmpRes )
    {
        for (i = 0u; i < psLength; i++)
        {
            if (0xFFu != *(decryptedSignature + 2u + i))
            {
                cmpRes = 1u;  /* Further checking is not needed */
                break;
            }
        }
    }

    /* Check the padding (T string) */
    if (0u == cmpRes)
    {
        cmpRes = Cy_Crypto_Core_MemCmp(base, encodingArr,
                        (decryptedSignature + psLength + 3u),
                        (uint16_t)encodingArrSize);
    }

    /* Check the digest */
    if (0u == cmpRes)
    {
        cmpRes = Cy_Crypto_Core_MemCmp(base, digest,
                        (decryptedSignature + (decryptedSignatureLength - locDigestSize)),
                        (uint16_t)locDigestSize);
    }

    if (0u == cmpRes )
    {
        *verResult = CY_CRYPTO_RSA_VERIFY_SUCCESS;
    }

    return (tmpResult);
}

/** \} group_crypto_lld_asymmetric_functions */

#if (CPUSS_CRYPTO_VU == 1)

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_MontCoeff
****************************************************************************//**
*
* Calculation of Montgomery coefficients for Montgomery multiplication in GF(p).
*
* Reference: "Montgomery Multiplication", H. S. Warren, Jr., July 2012
* Reference: http://www.hackersdelight.org/MontgomeryMultiplication.pdf
*
* GCD method is used, reference:
* https://en.wikipedia.org/wiki/Binary_GCD_algorithm
*
* Inputs: modulus n, k = number of bits in n;
* Outputs: R^-1 and n' which allows equation: R*R^-1 - n*n' = 1,
* where R = 1 << size.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param modDerReg
* Register index for Montgomery coefficient value.
*
* \param modReg
* Register index for modulo value.
*
* \param size
* Modulo size in bits.
*
*******************************************************************************/
static void Cy_Crypto_Core_Rsa_MontCoeff(CRYPTO_Type *base, uint32_t modDerReg, uint32_t modReg, uint32_t size)
{
    uint32_t myMod  = 9u;
    uint32_t tmp    = 10u;
    uint32_t ra      = 11u;
    uint32_t rb      = 12u;
    uint32_t ru      = 13u;
    uint32_t rv      = 14u;

    uint32_t status;
    uint32_t aZero;
    uint32_t uEven;

    CY_CRYPTO_VU_PUSH_REG    (base);

    CY_CRYPTO_VU_LD_REG      (base, rv,     modDerReg);
    CY_CRYPTO_VU_LD_REG      (base, myMod, modReg);

    CY_CRYPTO_VU_ALLOC_MEM   (base, ra, size);
    CY_CRYPTO_VU_ALLOC_MEM   (base, rb, size);
    CY_CRYPTO_VU_ALLOC_MEM   (base, ru, size);

    CY_CRYPTO_VU_SET_TO_ONE  (base, ru);
    CY_CRYPTO_VU_SET_TO_ZERO (base, rv);

    CY_CRYPTO_VU_SET_TO_ZERO (base, ra);
    CY_CRYPTO_VU_SET_REG     (base, tmp, (size - 1u), 1u);

    CY_CRYPTO_VU_SET_BIT     (base, ra, tmp);
    CY_CRYPTO_VU_MOV         (base, rb, myMod);

    while (true)
    {
        Cy_Crypto_Core_Vu_WaitForComplete(base);

        CY_CRYPTO_VU_TST(base, ra);

        status = Cy_Crypto_Core_Vu_StatusRead(base);
        aZero = status &  CY_CRYPTO_VU_STATUS_ZERO_BIT;

        if (aZero != 0u)
        {
            break;
        }

        CY_CRYPTO_VU_LSR1(base, ra, ra);

        CY_CRYPTO_VU_TST(base, ru);
        status = Cy_Crypto_Core_Vu_StatusRead(base);
        uEven = status & CY_CRYPTO_VU_STATUS_EVEN_BIT;

        if (uEven != 0u)
        {
            CY_CRYPTO_VU_LSR1(base, ru, ru);
            CY_CRYPTO_VU_LSR1(base, rv, rv);
        }
        else
        {
            CY_CRYPTO_VU_ADD(base, ru, ru, rb);
            CY_CRYPTO_VU_LSR1_WITH_CARRY(base, ru, ru);
            CY_CRYPTO_VU_LSR1(base, rv, rv);
            CY_CRYPTO_VU_SET_BIT(base, rv, tmp);
        }
    }

    CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(ra) | CY_CRYPTO_VU_REG_BIT(rb) | CY_CRYPTO_VU_REG_BIT(ru));

    CY_CRYPTO_VU_POP_REG(base);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_BarrettGetU
****************************************************************************//**
*
* Calculation of Barrett coefficient for Barrett reduction in GF(p).
*
* The function reduces the size of the Barrett coefficient by removing leading '0' bits.
* This improves the performance of the Barrett reduction (faster multiplication).
* (1 << bit_size) > mod > (1 << (bit_size-1))
*
* barrett = (1 << (2 * size)) / mod      NO!!! leading '1' Barrett bit.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param barrettUReg
* Register index for Barrett reduction value.
*
* \param modReg
* Register index for modulo value.
*
* \param size
* The modulo size in bits.
*
*******************************************************************************/
static void Cy_Crypto_Core_Rsa_BarrettGetU(CRYPTO_Type *base, uint32_t barrettUReg, uint32_t modReg, uint32_t size)
{
    uint32_t dividend = 0u;
    uint32_t t        = 1u;
    uint32_t temp     = 2u;
    uint32_t sh       = 3u;
    int32_t i;

    CY_CRYPTO_VU_ALLOC_MEM (base, dividend, size + 1u);
    CY_CRYPTO_VU_ALLOC_MEM (base, t,        size + 1u);
    CY_CRYPTO_VU_ALLOC_MEM (base, temp,     size + 1u);

    /* (1 << size) - there is probably a more efficient way to initialize this */
    CY_CRYPTO_VU_SET_REG     (base, sh, size, 1u);
    CY_CRYPTO_VU_SET_TO_ZERO (base, dividend);
    CY_CRYPTO_VU_SET_BIT     (base, dividend, sh);

    CY_CRYPTO_VU_MOV (base, temp, dividend);

    CY_CRYPTO_VU_SET_TO_ZERO (base, barrettUReg);

    for (i = (int32_t)size; i >= 0; i--)
    {
        /* C = (a >= b) */
        CY_CRYPTO_VU_SUB           (base, t, dividend, modReg);
        CY_CRYPTO_VU_COND_SWAP_REG (base, CY_CRYPTO_VU_COND_CS, dividend, t);
        CY_CRYPTO_VU_COND_XOR      (base, CY_CRYPTO_VU_COND_CS, barrettUReg, barrettUReg, temp);
        CY_CRYPTO_VU_LSL1          (base, dividend, dividend);
        CY_CRYPTO_VU_LSR1          (base, temp, temp);
    }

    CY_CRYPTO_VU_FREE_MEM (base, CY_CRYPTO_VU_REG_BIT(dividend) | CY_CRYPTO_VU_REG_BIT(temp) | CY_CRYPTO_VU_REG_BIT(t));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_MontTransform
****************************************************************************//**
*
* \brief Conversion to Montgomery representation.
*
* z = (a << size) % mod
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param z
* Register index for Montgomery representation value.
*
* \param a
* Register index for regular representation value.
*
* \param barrett
* Register index for Barrett reduction value.
*
* \param mod
* Register index for modulo value.
*
* \param size
* Bit size.
*
*******************************************************************************/
static void Cy_Crypto_Core_Rsa_MontTransform(CRYPTO_Type *base, uint32_t z, uint32_t a, uint32_t barrett, uint32_t mod, uint32_t size)
{
    uint32_t sh      = 0u;
    uint32_t t1Plus2 = 1u;
    uint32_t t2Plus2 = 0u;
    uint32_t tDouble = 2u;
    uint32_t aDouble = 3u;

    CY_CRYPTO_VU_ALLOC_MEM      (base, tDouble, 2u * size);
    CY_CRYPTO_VU_ALLOC_MEM      (base, aDouble, 2u * size);

    CY_CRYPTO_VU_SET_REG        (base, sh, size, 1u);

    CY_CRYPTO_VU_UMUL           (base, tDouble, a, barrett);
    CY_CRYPTO_VU_LSR            (base, aDouble, tDouble, sh);

    CY_CRYPTO_VU_UMUL           (base, tDouble, aDouble, mod);

    CY_CRYPTO_VU_LSL            (base, aDouble, a, sh);

    CY_CRYPTO_VU_ALLOC_MEM      (base, t2Plus2, size + 2u);

    CY_CRYPTO_VU_SUB            (base, t2Plus2, aDouble, tDouble);

    CY_CRYPTO_VU_FREE_MEM       (base, CY_CRYPTO_VU_REG_BIT(aDouble) | CY_CRYPTO_VU_REG_BIT(tDouble));

    CY_CRYPTO_VU_ALLOC_MEM      (base, t1Plus2, size + 2u);

    /* Check CARRY = (a >= b) */
    CY_CRYPTO_VU_SUB            (base, t1Plus2, t2Plus2, mod);
    CY_CRYPTO_VU_COND_SWAP_REG  (base, CY_CRYPTO_VU_COND_CC, t1Plus2, t2Plus2);

    /* Check CARRY = (a >= b) */
    CY_CRYPTO_VU_SUB            (base, t2Plus2, t1Plus2, mod);
    CY_CRYPTO_VU_COND_MOV       (base, CY_CRYPTO_VU_COND_CC, z, t1Plus2);
    CY_CRYPTO_VU_COND_MOV       (base, CY_CRYPTO_VU_COND_CS, z, t2Plus2);

    CY_CRYPTO_VU_FREE_MEM       (base, CY_CRYPTO_VU_REG_BIT(t2Plus2) | CY_CRYPTO_VU_REG_BIT(t1Plus2));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_MontMul
****************************************************************************//**
*
* Montgomery multiplication in GF(p).
*
* z = a * b * r % mod
*
* t = mont_a * mont_b
* u = t * montModDer    "only lower 32 bits are needed"
* u = u * mod
* u = u + t
* u = u >> size
* u = IF (u >= mod) u = u - mod
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param z
* Register index for product value.
*
* \param a
* Register index for multiplicand value.
*
* \param b
* Register index for multiplier value.
*
* \param montModDer
* Register index for Montgomery coefficient value.
*
* \param mod
* Register index for modulo value.
*
* \param size
* Bit size.
*
*******************************************************************************/
static void Cy_Crypto_Core_Rsa_MontMul(CRYPTO_Type *base,
                                uint32_t z, uint32_t a, uint32_t b,
                                uint32_t montModDer, uint32_t mod, uint32_t size)
{
    uint32_t sh       = 0u;
    uint32_t t        = 1u;
    uint32_t uDouble = 2u;
    uint32_t tDouble = 3u;

    uint32_t status = 4u;

    CY_CRYPTO_VU_ALLOC_MEM  (base, t,       size + 1u);
    CY_CRYPTO_VU_ALLOC_MEM  (base, uDouble, 2u * size);
    CY_CRYPTO_VU_ALLOC_MEM  (base, tDouble, 2u * size);

    CY_CRYPTO_VU_SET_REG    (base, sh, size, 1u);

    CY_CRYPTO_VU_UMUL       (base, tDouble, a, b);

    /* Only the lower 32 bits are needed. */
    CY_CRYPTO_VU_UMUL       (base, t, tDouble, montModDer);

    /* Clear the MSB bit (cut to size length) */
    CY_CRYPTO_VU_LSL1(base, t, t);
    CY_CRYPTO_VU_LSR1(base, t, t);

    CY_CRYPTO_VU_UMUL       (base, uDouble, t, mod);

    CY_CRYPTO_VU_ADD        (base, uDouble, uDouble, tDouble);

    CY_CRYPTO_VU_MOV_STATUS_TO_REG (base, status);

    CY_CRYPTO_VU_LSR          (base, uDouble, uDouble, sh);

    CY_CRYPTO_VU_SET_TO_ZERO  (base, t);
    CY_CRYPTO_VU_SET_REG      (base, sh, 0u,    1u);
    CY_CRYPTO_VU_SET_BIT      (base, t,  sh);
    CY_CRYPTO_VU_SET_REG      (base, sh, size, 1u);
    CY_CRYPTO_VU_LSL          (base, t,  t,    sh);

    CY_CRYPTO_VU_MOV_REG_TO_STATUS (base, status);
    Cy_Crypto_Core_Vu_WaitForComplete(base);

    CY_CRYPTO_VU_XOR (base, tDouble, uDouble, t);
    CY_CRYPTO_VU_COND_SWAP_REG (base, CY_CRYPTO_VU_COND_CS, uDouble, tDouble);

    /* C = (a >= b) */
    CY_CRYPTO_VU_SUB (base, tDouble, uDouble, mod);
    CY_CRYPTO_VU_COND_SWAP_REG (base, CY_CRYPTO_VU_COND_CS, uDouble, tDouble);

    CY_CRYPTO_VU_MOV (base, z, uDouble);

    CY_CRYPTO_VU_FREE_MEM (base, CY_CRYPTO_VU_REG_BIT(tDouble) | CY_CRYPTO_VU_REG_BIT(uDouble) | CY_CRYPTO_VU_REG_BIT(t));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_expModByMont
****************************************************************************//**
*
* Perform y = x^e mod n using Montgomery reduction technique to speed-up
* calculation. Suitable for cases with short e.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param y
* Register index for calculated value.
*
* \param x
* Register index for multiplicand value.
*
* \param e
* Register index for exponent value.
*
* \param n
* Register index for modulo value.
*
* \param barretCoef
* Barrett coefficient.
*
* \param inverseModulo
* Binary inverse of the modulo.
*
* \param rBar
* Values of (2^moduloLength mod modulo).
*
* \param size
* The modulo size in bits.
*
*******************************************************************************/
static void Cy_Crypto_Core_Rsa_expModByMont(CRYPTO_Type *base,
                                     uint32_t y,
                                     uint32_t x,
                                     uint32_t e,
                                     uint32_t n,
                                     uint32_t barretCoef,
                                     uint32_t inverseModulo,
                                     uint32_t rBar,
                                     uint32_t size)
{
    int32_t  i;
    uint32_t j;
    uint32_t status;
    uint32_t carry;
    uint32_t clsame;

    uint32_t myY      = 5u;
    uint32_t myX      = 6u;
    uint32_t myE      = 7u;
    uint32_t myN      = 8u;
    uint32_t myReg0   = 9u;
    uint32_t nPrime   = 10u;
    uint32_t temp     = 11u;
    uint32_t barrett  = 12u;
    uint32_t xBar     = 13u;
    uint32_t REG      = 14u;

    CY_CRYPTO_VU_PUSH_REG(base);

    CY_CRYPTO_VU_LD_REG(base, myX, x);
    CY_CRYPTO_VU_LD_REG(base, myY, y);
    CY_CRYPTO_VU_LD_REG(base, myE, e);
    CY_CRYPTO_VU_LD_REG(base, myN, n);
    CY_CRYPTO_VU_LD_REG(base, nPrime,  inverseModulo);
    CY_CRYPTO_VU_LD_REG(base, barrett, barretCoef);

    CY_CRYPTO_VU_MOV(base, myY, rBar);

    CY_CRYPTO_VU_ALLOC_MEM(base, myReg0,    size);
    CY_CRYPTO_VU_ALLOC_MEM(base, xBar,      size);
    CY_CRYPTO_VU_ALLOC_MEM(base, temp,      size);

    Cy_Crypto_Core_Rsa_MontTransform(base, xBar, myX, barrett, myN, size);

    CY_CRYPTO_VU_MOV(base, temp, myE);
    CY_CRYPTO_VU_SET_TO_ZERO(base, myReg0);

    CY_CRYPTO_VU_CLSAME(base, REG, temp, myReg0);

    /* This is needed, otherwise clsame is wrong */
    Cy_Crypto_Core_Vu_WaitForComplete(base);

    clsame = Cy_Crypto_Core_Vu_RegDataPtrRead(base, REG);

    j = ((uint32_t)Cy_Crypto_Core_Vu_RegSizeRead(base, temp) + 1u) - clsame;

    CY_CRYPTO_VU_SET_REG(base, REG, clsame, 1u);
    CY_CRYPTO_VU_LSL(base, temp, temp, REG);

    for (i = (int32_t)j; i > 0; i--)
    {
        /* j is number of bits in exponent e */
        CY_CRYPTO_VU_LSL1(base, temp, temp);
        status = Cy_Crypto_Core_Vu_StatusRead(base);
        carry = status & CY_CRYPTO_VU_STATUS_CARRY_BIT;

        if (carry != 0u)
        {
            /* myY = myY * xBar */
            Cy_Crypto_Core_Rsa_MontMul(base, myY, myY, xBar, nPrime, myN, size);
            Cy_Crypto_Core_Vu_WaitForComplete(base);

            /* xBar = xBar ^ 2 */
            Cy_Crypto_Core_Rsa_MontMul(base, xBar, xBar, xBar, nPrime, myN, size);
            Cy_Crypto_Core_Vu_WaitForComplete(base);
        }
        else
        {
            /* xBar = myY * xBar */
            Cy_Crypto_Core_Rsa_MontMul(base, xBar, myY, xBar, nPrime, myN, size);
            Cy_Crypto_Core_Vu_WaitForComplete(base);

            /* myY = myY ^ 2 */
            Cy_Crypto_Core_Rsa_MontMul(base, myY, myY, myY, nPrime, myN, size);
            Cy_Crypto_Core_Vu_WaitForComplete(base);
        }
    }

    CY_CRYPTO_VU_SET_TO_ONE(base, myReg0);

    Cy_Crypto_Core_Rsa_MontMul(base, myY, myY, myReg0, nPrime , myN, size);

    CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(myReg0) | CY_CRYPTO_VU_REG_BIT(xBar) | CY_CRYPTO_VU_REG_BIT(temp));
    CY_CRYPTO_VU_POP_REG(base);

    Cy_Crypto_Core_Vu_WaitForComplete(base);
}

/**
* \addtogroup group_crypto_lld_asymmetric_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_Proc
****************************************************************************//**
*
* RSA process algorithm based on the Montgomery algorithm
* using Barrett reduction
*
* https://en.wikipedia.org/wiki/RSA_%28cryptosystem%29
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param key
* The pointer to the \ref cy_stc_crypto_rsa_pub_key_t structure that stores
* public key.
*
* \param message
* The pointer to the message to be processed.
*
* \param messageSize
* The length of the message to be processed.
*
* \param processedMessage
* The pointer to processed message.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Rsa_Proc(CRYPTO_Type *base,
                                              cy_stc_crypto_rsa_pub_key_t const *key,
                                              uint8_t const *message,
                                              uint32_t messageSize,
                                              uint8_t *processedMessage)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_SUCCESS;

    uint8_t *expPtr              = key->pubExpPtr;
    uint32_t expBitLength        = key->pubExpLength;
    uint8_t *nPtr                = key->moduloPtr;
    uint32_t nBitLength          = key->moduloLength;
    uint8_t *barretCoef          = key->barretCoefPtr;
    uint8_t *inverseModulo       = key->inverseModuloPtr;
    uint8_t *rBar                = key->rBarPtr;

    uint32_t yReg                = 5u;
    uint32_t xReg                = 6u;
    uint32_t eReg                = 7u;
    uint32_t modReg              = 8u;
    uint32_t inverseModuloReg    = 9u;
    uint32_t barrettReg          = 10u;
    uint32_t rBarReg             = 11u;
    uint16_t vu_mem_size         = 0U;
    void *vu_mem_address         = NULL;

    vu_mem_address = Cy_Crypto_Core_GetVuMemoryAddress(base);
    vu_mem_size = (uint16_t)Cy_Crypto_Core_GetVuMemorySize(base);

    /* Clear all Crypto Buffer before operations */
    Cy_Crypto_Core_MemSet(base, vu_mem_address, 0x00u, vu_mem_size);

    CY_CRYPTO_VU_ALLOC_MEM(base, yReg,             nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(base, xReg,             nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(base, eReg,             nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(base, modReg,           nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(base, barrettReg,       nBitLength + (uint32_t)1u);
    CY_CRYPTO_VU_ALLOC_MEM(base, rBarReg,          nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(base, inverseModuloReg, nBitLength);

    Cy_Crypto_Core_Vu_SetMemValue(base, modReg, nPtr,   nBitLength);
    Cy_Crypto_Core_Vu_SetMemValue(base, eReg,   expPtr, expBitLength);
    Cy_Crypto_Core_Vu_SetMemValue(base, xReg,   (uint8_t const *)message, messageSize * (uint32_t)8u);

    /* Check coefficients */
    if (barretCoef == NULL)
    {
        Cy_Crypto_Core_Rsa_BarrettGetU(base, barrettReg, modReg, nBitLength);
        Cy_Crypto_Core_Vu_WaitForComplete(base);
    }
    else
    {
        Cy_Crypto_Core_Vu_SetMemValue(base, barrettReg, barretCoef, nBitLength + (uint32_t)1u);
    }

    if (rBar == NULL)
    {
        /* inverseModuloReg used here as temp variable */
        CY_CRYPTO_VU_SET_TO_ONE(base, inverseModuloReg);
        Cy_Crypto_Core_Rsa_MontTransform(base, rBarReg, inverseModuloReg, barrettReg, modReg, nBitLength);
        Cy_Crypto_Core_Vu_WaitForComplete(base);
    }
    else
    {
        Cy_Crypto_Core_Vu_SetMemValue(base, rBarReg, rBar, nBitLength);
    }

    if (inverseModulo == NULL)
    {
        Cy_Crypto_Core_Rsa_MontCoeff(base, inverseModuloReg, modReg, nBitLength);
        Cy_Crypto_Core_Vu_WaitForComplete(base);
    }
    else
    {
        Cy_Crypto_Core_Vu_SetMemValue(base, inverseModuloReg, inverseModulo, nBitLength);
    }

    Cy_Crypto_Core_Rsa_expModByMont(base,
                                    yReg,
                                    xReg,
                                    eReg,
                                    modReg,
                                    barrettReg,
                                    inverseModuloReg,
                                    rBarReg,
                                    nBitLength);

    Cy_Crypto_Core_Vu_WaitForComplete(base);

    /* Copy the tmpResult to output buffer */
    Cy_Crypto_Core_Vu_GetMemValue(base, (uint8_t*)processedMessage, yReg, nBitLength);

    CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(yReg) | CY_CRYPTO_VU_REG_BIT(xReg) |
                                CY_CRYPTO_VU_REG_BIT(eReg) | CY_CRYPTO_VU_REG_BIT(modReg) |
                                CY_CRYPTO_VU_REG_BIT(inverseModuloReg) |
                                CY_CRYPTO_VU_REG_BIT(barrettReg) | CY_CRYPTO_VU_REG_BIT(rBarReg));

    Cy_Crypto_Core_Vu_WaitForComplete(base);

    return (tmpResult);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_Coef
****************************************************************************//**
*
* Calculation constant coefficients to to speed-up Montgomery algorithm.
* These coefficients are:
*                         coefficient for Barrett reduction,
*                         binary inverse of the modulo,
*                         result of (2^moduloLength mod modulo)
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param key
* The pointer to the \ref cy_stc_crypto_rsa_pub_key_t structure that stores a
* public key.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Rsa_Coef(CRYPTO_Type *base,
                                              cy_stc_crypto_rsa_pub_key_t const *key)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_SUCCESS;

    uint8_t *nPtr               = key->moduloPtr;
    uint32_t nBitLength         = key->moduloLength;
    uint8_t *barretCoef         = key->barretCoefPtr;
    uint8_t *inverseModulo      = key->inverseModuloPtr;
    uint8_t *rBar               = key->rBarPtr;

    uint32_t modReg              = 11u;
    uint32_t inverseModuloReg    = 12u;
    uint32_t barrettReg          = 13u;
    uint32_t rBarReg             = 14u;
    uint16_t vu_mem_size         = 0U;
    void *vu_mem_address         = NULL;

    vu_mem_address = Cy_Crypto_Core_GetVuMemoryAddress(base);
    vu_mem_size = (uint16_t)Cy_Crypto_Core_GetVuMemorySize(base);

    /* Clear all Crypto Buffer before operations */
    Cy_Crypto_Core_MemSet(base, vu_mem_address, 0x00u, vu_mem_size);

    CY_CRYPTO_VU_ALLOC_MEM(base, modReg,           nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(base, barrettReg,       nBitLength + 1u);
    CY_CRYPTO_VU_ALLOC_MEM(base, inverseModuloReg, nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(base, rBarReg,          nBitLength);

    /* Copy modulo to Crypto SRAM */
    Cy_Crypto_Core_Vu_SetMemValue(base, modReg, nPtr, nBitLength);

    Cy_Crypto_Core_Rsa_BarrettGetU(base, barrettReg, modReg, nBitLength);
    Cy_Crypto_Core_Vu_WaitForComplete(base);

    /* Copy calculated Barrett coefficient */
    Cy_Crypto_Core_Vu_GetMemValue(base, barretCoef, barrettReg, nBitLength + 1u);

    /* inverseModuloReg used here as temp variable */
    CY_CRYPTO_VU_SET_TO_ONE(base, inverseModuloReg);
    Cy_Crypto_Core_Rsa_MontTransform(base, rBarReg, inverseModuloReg, barrettReg, modReg, nBitLength);
    Cy_Crypto_Core_Vu_WaitForComplete(base);

    /* Copy calculated r-bar = (1 << size) mod modulo */
    Cy_Crypto_Core_Vu_GetMemValue(base, rBar, rBarReg, nBitLength);

    Cy_Crypto_Core_Rsa_MontCoeff(base, inverseModuloReg, modReg, nBitLength);

    /* Copy calculated inverse modulo */
    Cy_Crypto_Core_Vu_GetMemValue(base, inverseModulo, inverseModuloReg, nBitLength);

    CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(modReg) | CY_CRYPTO_VU_REG_BIT(inverseModuloReg) | CY_CRYPTO_VU_REG_BIT(barrettReg) | CY_CRYPTO_VU_REG_BIT(rBarReg));

    Cy_Crypto_Core_Vu_WaitForComplete(base);

    return (tmpResult);
}

/** \} group_crypto_lld_asymmetric_functions */

#endif /* #if (CPUSS_CRYPTO_VU == 1) */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */


/* [] END OF FILE */
