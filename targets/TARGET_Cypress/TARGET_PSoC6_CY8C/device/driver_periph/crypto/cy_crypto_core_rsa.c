/***************************************************************************//**
* \file cy_crypto_core_rsa.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API to calculate
*  a signature by the RSA method in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_core_rsa.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_vu_hw.h"
#include "crypto/cy_crypto_core_vu.h"
#include "crypto/cy_crypto_core_sha.h"
#include "syslib/cy_syslib.h"
#include <string.h>


#if (CY_CPU_CORTEX_M0P)

#define CY_CRYPTO_SHA1_PADDING_SIZE        (15u)
#define CY_CRYPTO_SHA256_512_PADDING_SIZE  (19u)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))

/* Functions prototypes */
void Cy_Crypto_Core_Rsa_MontCoeff(uint32_t modDerReg, uint32_t modReg, uint32_t size);
void Cy_Crypto_Core_Rsa_BarrettGetU(uint32_t barrettUReg, uint32_t modReg, uint32_t size);
void Cy_Crypto_Core_Rsa_BarretRed(uint32_t resultReg, uint32_t aReg, uint32_t barrettUReg, uint32_t modReg, uint32_t size);
void Cy_Crypto_Core_Rsa_MontTransform(uint32_t z, uint32_t a, uint32_t barrett_u, uint32_t mod, uint32_t size);
void Cy_Crypto_Core_Rsa_MontMul(uint32_t z, uint32_t a, uint32_t b, uint32_t mont_mod_der, uint32_t mod, uint32_t size);

void Cy_Crypto_Core_Rsa_expModByMont(uint32_t y,
                                     uint32_t x,
                                     uint32_t e,
                                     uint32_t n,
                                     uint32_t barretCoef,
                                     uint32_t inverseModulo,
                                     uint32_t rBar,
                                     uint32_t size);

#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */


#if ((CPUSS_CRYPTO_PRESENT == 1) && (CY_CRYPTO_USER_PKCS1_5 == 1))

/* Encodings for hash functions */

#if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1))
static const uint8_t Sha1EncStr[CY_CRYPTO_SHA1_PADDING_SIZE] =
{
    0x14u, 0x04u, 0x00u, 0x05u,
    0x1Au, 0x02u, 0x03u, 0x0Eu, 0x2Bu, 0x05u, 0x06u, 0x09u,
    0x30u, 0x21u, 0x30u
};
#endif /* #if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1)) */

#if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1))
static const uint8_t Sha224EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
{
    0x1Cu, 0x04u, 0x00u, 0x05u, 0x04u, 0x02u, 0x04u, 0x03u,
    0x65u, 0x01u, 0x48u, 0x86u, 0x60u, 0x09u, 0x06u, 0x0du,
    0x30u, 0x2Du, 0x30u
};

static const uint8_t Sha256EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
{
    0x20u, 0x04u, 0x00u, 0x05u, 0x01u, 0x02u, 0x04u, 0x03u,
    0x65u, 0x01u, 0x48u, 0x86u, 0x60u, 0x09u, 0x06u, 0x0du,
    0x30u, 0x31u, 0x30u
};
#endif /* #if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1)) */

#if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1))
static const uint8_t Sha384EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
{
    0x30u, 0x04u, 0x00u, 0x05u, 0x02u, 0x02u, 0x04u, 0x03u,
    0x65u, 0x01u, 0x48u, 0x86u, 0x60u, 0x09u, 0x06u, 0x0du,
    0x30u, 0x41u, 0x30u
};

static const uint8_t Sha512EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
{
    0x40u, 0x04u, 0x00u, 0x05u, 0x03u, 0x02u, 0x04u, 0x03u,
    0x65u, 0x01u, 0x48u, 0x86u, 0x60u, 0x09u, 0x06u, 0x0du,
    0x30u, 0x51u, 0x30u
};

static const uint8_t Sha512_224EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
{
    0x1Cu, 0x04u, 0x00u, 0x05u, 0x05u, 0x02u, 0x04u, 0x03u,
    0x65u, 0x01u, 0x48u, 0x86u, 0x60u, 0x09u, 0x06u, 0x0du,
    0x30u, 0x2Du, 0x30u
};

static const uint8_t Sha512_256EncStr[CY_CRYPTO_SHA256_512_PADDING_SIZE] =
{
    0x20u, 0x04u, 0x00u, 0x05u, 0x06u, 0x02u, 0x04u, 0x03u,
    0x65u, 0x01u, 0x48u, 0x86u, 0x60u, 0x09u, 0x06u, 0x0du,
    0x30u, 0x31u, 0x30u
};
#endif /* #if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1)) */

/*******************************************************************************
* Function Name: Cy_Crypto_Core_RsaVerify
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
* \param cryptoContextPtr
* Pointer to the RSA context
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_RsaVerify(cy_stc_crypto_context_rsa_ver_t *cryptoContextPtr)
{
    cy_en_crypto_status_t result = CY_CRYPTO_SUCCESS;

    cy_en_crypto_sha_mode_t _digestType       = cryptoContextPtr->digestType;
    uint8_t *_digestPtr                       = (uint8_t*)cryptoContextPtr->hashPtr;
    uint8_t *_decryptedSignaturePtr           = (uint8_t*)cryptoContextPtr->decryptedSignaturePtr;
    uint32_t _decryptedSignatureLength        = cryptoContextPtr->decryptedSignatureLength;
    cy_en_crypto_rsa_ver_result_t *_verResult = cryptoContextPtr->verResult;

    uint8_t *psBeginPtr;
    uint8_t *encodingArrPtr = NULL;
    uint32_t encodingArrSize = 0;
    uint32_t digestSize = 0;
    uint32_t i;
    int32_t psLength;
    int32_t cmpRes = 0;


    switch (_digestType)
    {

#if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1))
    case CY_CRYPTO_MODE_SHA1:
        encodingArrPtr  = (uint8_t*)Sha1EncStr;
        encodingArrSize = sizeof(Sha1EncStr);
        digestSize      = CY_CRYPTO_SHA1_DIGEST_SIZE;
        break;
#endif /* #if ((CPUSS_CRYPTO_SHA1 == 1) && (CY_CRYPTO_USER_SHA1 == 1)) */

#if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1))
    case CY_CRYPTO_MODE_SHA224:
        encodingArrPtr  = (uint8_t*)Sha224EncStr;
        encodingArrSize = sizeof(Sha224EncStr);
        digestSize      = CY_CRYPTO_SHA224_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA256:
        encodingArrPtr  = (uint8_t*)Sha256EncStr;
        encodingArrSize = sizeof(Sha256EncStr);
        digestSize      = CY_CRYPTO_SHA256_DIGEST_SIZE;
        break;
#endif /* #if ((CPUSS_CRYPTO_SHA256 == 1) && (CY_CRYPTO_USER_SHA256 == 1)) */

#if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1))
    case CY_CRYPTO_MODE_SHA384:
        encodingArrPtr  = (uint8_t*)Sha384EncStr;
        encodingArrSize = sizeof(Sha384EncStr);
        digestSize      = CY_CRYPTO_SHA384_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512:
        encodingArrPtr  = (uint8_t*)Sha512EncStr;
        encodingArrSize = sizeof(Sha512EncStr);
        digestSize      = CY_CRYPTO_SHA512_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512_224:
        encodingArrPtr  = (uint8_t*)Sha512_224EncStr;
        encodingArrSize = sizeof(Sha512_224EncStr);
        digestSize      = CY_CRYPTO_SHA512_224_DIGEST_SIZE;
        break;

    case CY_CRYPTO_MODE_SHA512_256:
        encodingArrPtr  = (uint8_t*)Sha512_256EncStr;
        encodingArrSize = sizeof(Sha512_256EncStr);
        digestSize      = CY_CRYPTO_SHA512_256_DIGEST_SIZE;
        break;
#endif /* #if ((CPUSS_CRYPTO_SHA512 == 1) && (CY_CRYPTO_USER_SHA512 == 1)) */

    default:
        break;
    }

    /* Fail by default */
    *_verResult = CY_CRYPTO_RSA_VERIFY_FAIL;

    /* Check size of decrypted message */
    if (_decryptedSignatureLength < (encodingArrSize + digestSize + 11))
    {
        cmpRes = 1;  /* further checking is not needed */
    }

    psLength = _decryptedSignatureLength - digestSize - encodingArrSize - 3;

    /* Address of start of T string (0xFFs) in padding */
    psBeginPtr = _decryptedSignaturePtr + _decryptedSignatureLength - 3;

    /* Check whether the begin of message is 0x00, 0x01 and after PS string (before T string) is 0x00 byte.*/
    if ( (0 != cmpRes) ||
         (0x00 != *(psBeginPtr + 2)) ||
         (0x01 != *(psBeginPtr + 1)) ||
         (0x00 != *(_decryptedSignaturePtr + _decryptedSignatureLength - psLength - 3)) )
    {
        cmpRes = 1;  /* Further checking is not needed */
    }

    /* Check FFs */
    if (0 == cmpRes )
    {
        for (i = psLength; i > 0; i--)
        {
            if (0xFF != *(psBeginPtr - i + 1))
            {
                cmpRes = 1;  /* Further checking is not needed */
                break;
            }
        }
    }

    /* Check the padding (T string) */
    if (0 == cmpRes)
    {
        cmpRes = memcmp(encodingArrPtr,
                        (_decryptedSignaturePtr + digestSize),
                        encodingArrSize);
    }

    /* Check the digest */
    if (0 == cmpRes)
    {
        _decryptedSignaturePtr = _decryptedSignaturePtr + digestSize - 1;
        for (i = digestSize; i > 0; i--)
        {
             if( *_decryptedSignaturePtr != *_digestPtr )
             {
                 cmpRes = 1;  /* Further checking is not needed */
                 break;
             }
             _decryptedSignaturePtr--;
             _digestPtr++;
        }
    }

    if (0 == cmpRes )
    {
        *_verResult = CY_CRYPTO_RSA_VERIFY_SUCCESS;
    }

    return (result);
}

#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CY_CRYPTO_USER_PKCS1_5 == 1)) */


#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))

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
* Leaf function.
*
* \param modDerReg
* Register index for Montgomery coefficient value.
*
* \param modReg
* Register index for modulo value.
*
* \param size
* Size of modulo, in Bits.
*
*******************************************************************************/
void Cy_Crypto_Core_Rsa_MontCoeff(uint32_t modDerReg, uint32_t modReg, uint32_t size)
{
    uint32_t my_mod  = 9;
    uint32_t tmp    = 10;
    uint32_t a      = 11;
    uint32_t b      = 12;
    uint32_t u      = 13;
    uint32_t v      = 14;

    uint16_t status;
    uint16_t a_zero;
    uint16_t u_even;

    CY_CRYPTO_VU_PUSH_REG    ();

    CY_CRYPTO_VU_LD_REG      (v, modDerReg);
    CY_CRYPTO_VU_LD_REG      (my_mod, modReg);

    CY_CRYPTO_VU_ALLOC_MEM   (a, size);
    CY_CRYPTO_VU_ALLOC_MEM   (b, size);
    CY_CRYPTO_VU_ALLOC_MEM   (u, size);

    CY_CRYPTO_VU_SET_TO_ONE  (u);
    CY_CRYPTO_VU_SET_TO_ZERO (v);

    CY_CRYPTO_VU_SET_TO_ZERO (a);
    CY_CRYPTO_VU_SET_REG     (tmp, (size - 1), 1);

    CY_CRYPTO_VU_SET_BIT     (a, tmp);
    CY_CRYPTO_VU_MOV         (b, my_mod);

    while(1)
    {
        Cy_Crypto_Core_WaitForReady();

        CY_CRYPTO_VU_TST(a);

        status = Cy_Crypto_Core_Vu_StatusRead();
        a_zero = status & (1 << CY_CRYPTO_VU_STATUS_ZERO);

        if(a_zero != 0)
        {
            break;
        }

        CY_CRYPTO_VU_LSR1(a, a);

        CY_CRYPTO_VU_TST(u);
        status = Cy_Crypto_Core_Vu_StatusRead();
        u_even = status & (1 << CY_CRYPTO_VU_STATUS_EVEN);

        if(u_even != 0)
        {
            CY_CRYPTO_VU_LSR1(u, u);
            CY_CRYPTO_VU_LSR1(v, v);
        }
        else
        {
            CY_CRYPTO_VU_ADD(u, u, b);
            CY_CRYPTO_VU_LSR1_WITH_CARRY(u, u);
            CY_CRYPTO_VU_LSR1(v, v);
            CY_CRYPTO_VU_SET_BIT(v, tmp);
        }
    }

    CY_CRYPTO_VU_FREE_MEM((1 << a) | (1 << b) | (1 << u));

    CY_CRYPTO_VU_POP_REG();
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
* barrett_u = (1 << (2 * size)) / mod      NO!!! leading '1' Barrett bit.
*
* Leaf function.
*
* \param barrettUReg
* Register index for Barrett reduction value.
*
* \param modReg
* Register index for modulo value.
*
* \param
* Size modulo size in Bits.
*
*******************************************************************************/
void Cy_Crypto_Core_Rsa_BarrettGetU(uint32_t barrettUReg, uint32_t modReg, uint32_t size)
{
    uint32_t dividend = 0;
    uint32_t t        = 1;
    uint32_t temp     = 2;
    uint32_t sh       = 3;
    int32_t i;

    CY_CRYPTO_VU_ALLOC_MEM(dividend, size+1);
    CY_CRYPTO_VU_ALLOC_MEM(t,        size+1);
    CY_CRYPTO_VU_ALLOC_MEM(temp,     size+1);

    /* (1 << size) – there is probably a more efficient way to initialize this */
    CY_CRYPTO_VU_SET_REG     (sh, size, 1);
    CY_CRYPTO_VU_SET_TO_ZERO (dividend);
    CY_CRYPTO_VU_SET_BIT     (dividend, sh);

    CY_CRYPTO_VU_MOV (temp, dividend);

    CY_CRYPTO_VU_SET_TO_ZERO (barrettUReg);

    for (i = size; i >= 0; i--)
    {
        /* C = (a >= b) */
        CY_CRYPTO_VU_SUB           (t, dividend, modReg);
        CY_CRYPTO_VU_COND_SWAP_REG (CY_CRYPTO_VU_COND_CS, dividend, t);
        CY_CRYPTO_VU_COND_XOR      (CY_CRYPTO_VU_COND_CS, barrettUReg, barrettUReg, temp);
        CY_CRYPTO_VU_LSL1          (dividend, dividend);
        CY_CRYPTO_VU_LSR1          (temp, temp);
    }

    CY_CRYPTO_VU_FREE_MEM((1 << dividend) | (1 << temp) | (1 << t));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_BarretRed
****************************************************************************//**
*
* Barrett reduction in GF(p).
*
* z = a_double mod modulo
*
* t[b-1:0] = z_double >> size
* t        = t * barrett_u
* t        = t + ((z_double >> size) << size)  "for leading '1' Barrett bit"
* t        = t >> size
* t        = t * mod
* u        = z_double - t
* u        = IF (u >= mod) u = u - mod
*
* Leaf function.
*
* \param resultReg
* Register index for Barrett reduced value.
*
* \param aReg
* Register index for non reduced value.
*
* \param barrettUReg
* Register index for Barrett reduction value.
*
* \param modReg
* Register index for modulo value.
*
* \param size
* Bit size.
*
*******************************************************************************/
void Cy_Crypto_Core_Rsa_BarretRed(uint32_t resultReg, uint32_t aReg, uint32_t barrettUReg, uint32_t modReg, uint32_t size)
{
    uint32_t sh       = 0;
    uint32_t t_double = 1;
    uint32_t z_double = 2;

    CY_CRYPTO_VU_ALLOC_MEM      (t_double, 2 * size);
    CY_CRYPTO_VU_ALLOC_MEM      (z_double, 2 * size);

    CY_CRYPTO_VU_SET_REG        (sh, size, 1);


    CY_CRYPTO_VU_LSR            (t_double, aReg, sh);

    CY_CRYPTO_VU_UMUL           (z_double, t_double, barrettUReg);

    CY_CRYPTO_VU_LSL            (t_double, t_double, sh);

    CY_CRYPTO_VU_ADD            (z_double, z_double, t_double);

    CY_CRYPTO_VU_LSR            (z_double, z_double, sh);

    CY_CRYPTO_VU_UMUL           (t_double, z_double, modReg);

    CY_CRYPTO_VU_SUB            (z_double, aReg, t_double);

    /* C = (a >= b) */
    CY_CRYPTO_VU_CMP_SUB        (z_double, modReg);
    CY_CRYPTO_VU_COND_SUB       (CY_CRYPTO_VU_COND_CS, z_double, z_double, modReg);

    CY_CRYPTO_VU_CMP_SUB        (z_double, modReg);
    CY_CRYPTO_VU_COND_SUB       (CY_CRYPTO_VU_COND_CS, z_double, z_double, modReg);

    CY_CRYPTO_VU_MOV            (resultReg, z_double);

    CY_CRYPTO_VU_FREE_MEM       ((1 << z_double) | (1 << t_double));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_MontTransform
****************************************************************************//**
*
* \brief Conversion to Montgomery representation.
*
* z = (a << size) % mod
*
* \param z
* Register index for Montgomery representation value.
*
* \param a
* Register index for regular representation value.
*
* \param barrett_u
* Register index for Barrett reduction value.
*
* \param mod
* Register index for modulo value.
*
* \param size
* Bit size.
*
*******************************************************************************/
void Cy_Crypto_Core_Rsa_MontTransform(uint32_t z, uint32_t a, uint32_t barrett_u, uint32_t mod, uint32_t size)
{
    uint32_t sh       = 0;
    uint32_t t1_plus2 = 1;
    uint32_t t2_plus2 = 0;
    uint32_t t_double = 2;
    uint32_t a_double = 3;

    CY_CRYPTO_VU_ALLOC_MEM      (t_double, 2 * size);
    CY_CRYPTO_VU_ALLOC_MEM      (a_double, 2 * size);

    CY_CRYPTO_VU_SET_REG        (sh, size, 1);

    CY_CRYPTO_VU_UMUL           (t_double, a, barrett_u);
    CY_CRYPTO_VU_LSR            (a_double, t_double, sh);

    CY_CRYPTO_VU_UMUL           (t_double, a_double, mod);

    CY_CRYPTO_VU_LSL            (a_double, a, sh);

    CY_CRYPTO_VU_ALLOC_MEM      (t2_plus2, size + 2);

    CY_CRYPTO_VU_SUB            (t2_plus2, a_double, t_double);

    CY_CRYPTO_VU_FREE_MEM       ((1 << a_double) | (1 << t_double));
    CY_CRYPTO_VU_ALLOC_MEM      (t1_plus2, size + 2);

    /* Check CARRY = (a >= b) */
    CY_CRYPTO_VU_SUB            (t1_plus2, t2_plus2, mod);
    CY_CRYPTO_VU_COND_SWAP_REG  (CY_CRYPTO_VU_COND_CC, t1_plus2, t2_plus2);

    /* Check CARRY = (a >= b) */
    CY_CRYPTO_VU_SUB            (t2_plus2, t1_plus2, mod);
    CY_CRYPTO_VU_COND_MOV       (CY_CRYPTO_VU_COND_CC, z, t1_plus2);
    CY_CRYPTO_VU_COND_MOV       (CY_CRYPTO_VU_COND_CS, z, t2_plus2);

    CY_CRYPTO_VU_FREE_MEM       ((1 << t2_plus2) | (1 << t1_plus2));
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
* u = t * mont_mod_der    "only lower 32 bits are needed"
* u = u * mod
* u = u + t
* u = u >> size
* u = IF (u >= mod) u = u - mod
*
* Leaf function.
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
* \param mont_mod_der
* Register index for Montgomery coefficient value.
*
* \param mod
* Register index for modulo value.
*
* \param size
*  Bit size.
*
*******************************************************************************/
void Cy_Crypto_Core_Rsa_MontMul(uint32_t z, uint32_t a, uint32_t b, uint32_t mont_mod_der, uint32_t mod, uint32_t size)
{
    uint32_t sh       = 0;
    uint32_t t        = 1;
    uint32_t u_double = 2;
    uint32_t t_double = 3;

    uint32_t status = 4;

    CY_CRYPTO_VU_ALLOC_MEM  (t,        size + 1);
    CY_CRYPTO_VU_ALLOC_MEM  (u_double, 2 * size);
    CY_CRYPTO_VU_ALLOC_MEM  (t_double, 2 * size);

    CY_CRYPTO_VU_SET_REG    (sh, size, 1);

    CY_CRYPTO_VU_UMUL       (t_double, a, b);

    /* Only lower 32 bits are needed */
    CY_CRYPTO_VU_UMUL       (t, t_double, mont_mod_der);

    /* Clear the MSB bit (cut to size length) */
    CY_CRYPTO_VU_LSL1(t, t);
    CY_CRYPTO_VU_LSR1(t, t);

    CY_CRYPTO_VU_UMUL       (u_double, t, mod);

    CY_CRYPTO_VU_ADD        (u_double, u_double, t_double);

    CY_CRYPTO_VU_MOV_STATUS_TO_REG (status);

    CY_CRYPTO_VU_LSR          (u_double, u_double, sh);

    CY_CRYPTO_VU_SET_TO_ZERO  (t);
    CY_CRYPTO_VU_SET_REG      (sh, 0, 1);
    CY_CRYPTO_VU_SET_BIT      (t, sh);
    CY_CRYPTO_VU_SET_REG      (sh, size, 1);
    CY_CRYPTO_VU_LSL          (t, t, sh);

    CY_CRYPTO_VU_MOV_REG_TO_STATUS (status);
    Cy_Crypto_Core_WaitForReady();

    CY_CRYPTO_VU_COND_XOR (CY_CRYPTO_VU_COND_CS, u_double, u_double, t);

    /* C = (a >= b) */
    CY_CRYPTO_VU_SUB (t_double, u_double, mod);

    CY_CRYPTO_VU_COND_SWAP_REG (CY_CRYPTO_VU_COND_CS, u_double, t_double);

    CY_CRYPTO_VU_MOV (z, u_double);

    CY_CRYPTO_VU_FREE_MEM ((1 << t_double) | (1 << u_double) | (1 << t));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_expModByMont
****************************************************************************//**
*
* Perform y = x^e mod n using Montgomery reduction technique to speed-up
* calculation. Suitable for cases with short e.
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
* \param size
* modulo size, in Bits
*
*******************************************************************************/
void Cy_Crypto_Core_Rsa_expModByMont(uint32_t y,
                                       uint32_t x,
                                       uint32_t e,
                                       uint32_t n,
                                       uint32_t barretCoef,
                                       uint32_t inverseModulo,
                                       uint32_t rBar,
                                       uint32_t size)
{
    int32_t i;
    uint16_t j;
    uint16_t status;
    uint16_t carry;
    uint16_t clsame;

    uint32_t my_y      = 5;
    uint32_t my_x      = 6;
    uint32_t my_e      = 7;
    uint32_t my_n      = 8;
    uint32_t R0_1      = 9;
    uint32_t n_prime   = 10;
    uint32_t temp      = 11;
    uint32_t barrett_u = 12;
    uint32_t x_bar     = 13;
    uint32_t REG       = 14;


    CY_CRYPTO_VU_PUSH_REG();

    CY_CRYPTO_VU_LD_REG(my_x, x);
    CY_CRYPTO_VU_LD_REG(my_y, y);
    CY_CRYPTO_VU_LD_REG(my_e, e);
    CY_CRYPTO_VU_LD_REG(my_n, n);
    CY_CRYPTO_VU_LD_REG(n_prime,   inverseModulo);
    CY_CRYPTO_VU_LD_REG(barrett_u, barretCoef);

    CY_CRYPTO_VU_MOV(my_y, rBar);

    CY_CRYPTO_VU_ALLOC_MEM(R0_1,      size);
    CY_CRYPTO_VU_ALLOC_MEM(x_bar,     size);
    CY_CRYPTO_VU_ALLOC_MEM(temp,      size);

    Cy_Crypto_Core_Rsa_MontTransform(x_bar, my_x, barrett_u, my_n, size);
    Cy_Crypto_Core_WaitForReady();

    CY_CRYPTO_VU_MOV(temp, my_e);
    CY_CRYPTO_VU_SET_TO_ZERO(R0_1);
    CY_CRYPTO_VU_CLSAME(REG, temp, R0_1);

    /* This is needed, otherwise clsame is wrong */
    Cy_Crypto_Core_WaitForReady();

    clsame = Cy_Crypto_Core_Vu_RegDataPtrRead(REG);
    j = Cy_Crypto_Core_Vu_RegSizeRead(temp) + 1 - clsame;
    CY_CRYPTO_VU_SET_REG(REG, clsame, 1);
    CY_CRYPTO_VU_LSL(temp, temp, REG);

    for (i = j; i > 0; i--)
    {

        /* j is number of bits in exponent e */
        Cy_Crypto_Core_Rsa_MontMul(my_y, my_y, my_y, n_prime, my_n, size);
        Cy_Crypto_Core_WaitForReady();

        CY_CRYPTO_VU_LSL1(temp, temp);
        status = Cy_Crypto_Core_Vu_StatusRead();
        carry = status & (1 << CY_CRYPTO_VU_STATUS_CARRY);

        if (carry != 0)
        {
            Cy_Crypto_Core_Rsa_MontMul(my_y, my_y, x_bar, n_prime, my_n, size);
        }
    }

    CY_CRYPTO_VU_SET_TO_ONE(R0_1);
    Cy_Crypto_Core_Rsa_MontMul(my_y, my_y, R0_1, n_prime , my_n, size);

    CY_CRYPTO_VU_FREE_MEM((1 << R0_1) | (1 << x_bar) | (1 << temp));
    CY_CRYPTO_VU_POP_REG();
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Rsa_Proc
****************************************************************************//**
*
* RSA process algorithm based on the Montgomery algorithm
* using Barrett reduction
*
* https://en.wikipedia.org/wiki/RSA_%28cryptosystem%29
*
* \param cryptoContextPtr
* Pointer to the RSA context
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Rsa_Proc(cy_stc_crypto_context_rsa_t *cryptoContextPtr)
{
    cy_en_crypto_status_t result = CY_CRYPTO_SUCCESS;

    uint8_t *_expPtr             = cryptoContextPtr->keyPtr->pubExpPtr;
    uint32_t _expBitLength       = cryptoContextPtr->keyPtr->pubExpLength;
    uint8_t *_nPtr               = cryptoContextPtr->keyPtr->moduloPtr;
    uint32_t _nBitLength         = cryptoContextPtr->keyPtr->moduloLength;
    uint32_t *_messagePtr        = (uint32_t*)cryptoContextPtr->messagePtr;
    uint32_t _messageByteLength  = cryptoContextPtr->messageLength;
    uint32_t *_resultPtr         = (uint32_t*)cryptoContextPtr->resultPtr;
    uint8_t *_barretCoefPtr      = cryptoContextPtr->keyPtr->barretCoefPtr;
    uint8_t *_inverseModuloPtr   = cryptoContextPtr->keyPtr->inverseModuloPtr;
    uint8_t *_rBarPtr            = cryptoContextPtr->keyPtr->rBarPtr;

    uint32_t _nByteLength      = 0;
    uint32_t _barretByteLength = 0;
    uint32_t _expByteLength    = 0;

    uint32_t yReg              = 5;
    uint32_t xReg              = 6;
    uint32_t eReg              = 7;
    uint32_t modReg            = 8;
    uint32_t inverseModuloReg  = 9;
    uint32_t barrettReg        = 10;
    uint32_t rBarReg           = 11;


    /* Clear all Crypto SRAM before operations */
    memset((void*)CRYPTO->MEM_BUFF, 0x00, CPUSS_CRYPTO_BUFF_SIZE * 4);

    CY_CRYPTO_VU_ALLOC_MEM(yReg,             _nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(xReg,             _nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(eReg,             _nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(modReg,           _nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(barrettReg,       _nBitLength + 1);
    CY_CRYPTO_VU_ALLOC_MEM(inverseModuloReg, _nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(rBarReg,          _nBitLength);

    _nByteLength = _nBitLength / 8;
    if ((_nBitLength % 8) != 0)
    {
        _nByteLength += 1;
    }

    _barretByteLength = (_nBitLength + 1) / 8;
    if (((_nBitLength + 1) % 8) != 0)
    {
        _barretByteLength += 1;
    }

    _expByteLength = _expBitLength / 8;
    if ((_expBitLength % 8) != 0)
    {
        _expByteLength += 1;
    }

    memcpy((void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(modReg)), _nPtr, _nByteLength);
    memcpy((void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(eReg)), _expPtr, _expByteLength);
    memcpy((void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(xReg)), _messagePtr, _messageByteLength);

    /* Check coefficients */
    if (_barretCoefPtr == NULL)
    {
        Cy_Crypto_Core_Rsa_BarrettGetU(barrettReg, modReg, _nBitLength);
        Cy_Crypto_Core_WaitForReady();
    }
    else
    {
        memcpy((void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(barrettReg)),
               _barretCoefPtr,
               _barretByteLength);
    }

    if (_rBarPtr == NULL)
    {
        /* inverseModuloReg used here as temp variable */
        CY_CRYPTO_VU_SET_TO_ONE(inverseModuloReg);
        Cy_Crypto_Core_Rsa_MontTransform(rBarReg, inverseModuloReg, barrettReg, modReg, _nBitLength);
        Cy_Crypto_Core_WaitForReady();
    }
    else
    {
        memcpy((void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(rBarReg)),
               _rBarPtr,
               _nByteLength);
    }

    if (_inverseModuloPtr == NULL)
    {
        Cy_Crypto_Core_Rsa_MontCoeff(inverseModuloReg, modReg, _nBitLength);
        Cy_Crypto_Core_WaitForReady();
    }
    else
    {
        memcpy((void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(inverseModuloReg)),
               _inverseModuloPtr,
               _nByteLength);
    }

    Cy_Crypto_Core_Rsa_expModByMont(yReg,
                                    xReg,
                                    eReg,
                                    modReg,
                                    barrettReg,
                                    inverseModuloReg,
                                    rBarReg,
                                    _nBitLength);

    Cy_Crypto_Core_Vu_WaitForComplete();

    /* Copy the result to output buffer */
    memcpy((void*)_resultPtr, (void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(yReg)), _nByteLength);

    CY_CRYPTO_VU_FREE_MEM((1 << yReg) | (1 << xReg) | (1 << eReg) | (1 << modReg) |
                          (1 << inverseModuloReg) | (1 << barrettReg) | (1 << rBarReg));

    return (result);
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
* \param cryptoContextPtr
* Pointer to the RSA context
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Rsa_Coef(cy_stc_crypto_context_rsa_t *cryptoContextPtr)
{
    cy_en_crypto_status_t result = CY_CRYPTO_SUCCESS;

    uint8_t *_nPtr               = cryptoContextPtr->keyPtr->moduloPtr;
    uint32_t _nBitLength         = cryptoContextPtr->keyPtr->moduloLength;
    uint8_t *_barretCoefPtr      = cryptoContextPtr->keyPtr->barretCoefPtr;
    uint8_t *_inverseModuloPtr   = cryptoContextPtr->keyPtr->inverseModuloPtr;
    uint8_t *_rBarPtr            = cryptoContextPtr->keyPtr->rBarPtr;
    uint32_t _nByteLength;
    uint32_t _barretByteLength;

    uint32_t modReg              = 11;
    uint32_t inverseModuloReg    = 12;
    uint32_t barrettReg          = 13;
    uint32_t rBarReg             = 14;


    /* Clear all Crypto SRAM before operations */
    memset((void*)CRYPTO->MEM_BUFF, 0x00, CPUSS_CRYPTO_BUFF_SIZE * 4);

    CY_CRYPTO_VU_ALLOC_MEM(modReg,           _nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(barrettReg,       _nBitLength + 1);
    CY_CRYPTO_VU_ALLOC_MEM(inverseModuloReg, _nBitLength);
    CY_CRYPTO_VU_ALLOC_MEM(rBarReg,          _nBitLength);

    _nByteLength = _nBitLength / 8;
    if ((_nBitLength % 8) != 0)
    {
        _nByteLength += 1;
    }

    _barretByteLength = (_nBitLength + 1) / 8;
    if (((_nBitLength + 1) % 8) != 0)
    {
        _barretByteLength += 1;
    }

    /* Copy modulo to Crypto SRAM */
    memcpy((void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(modReg)),
           _nPtr,
           _nByteLength);

    Cy_Crypto_Core_Rsa_BarrettGetU(barrettReg, modReg, _nBitLength);
    Cy_Crypto_Core_WaitForReady();

    /* Copy calculated Barrett coefficient */
    memcpy((void*)_barretCoefPtr,
           (void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(barrettReg)),
           _barretByteLength);

    /* inverseModuloReg used here as temp variable */
    CY_CRYPTO_VU_SET_TO_ONE(inverseModuloReg);
    Cy_Crypto_Core_Rsa_MontTransform(rBarReg, inverseModuloReg, barrettReg, modReg, _nBitLength);
    Cy_Crypto_Core_WaitForReady();

    /* Copy calculated r-bar = (1 << size) mod modulo */
    memcpy((void*)_rBarPtr,
           (void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(rBarReg)),
           _nByteLength);

    Cy_Crypto_Core_Rsa_MontCoeff(inverseModuloReg, modReg, _nBitLength);
    Cy_Crypto_Core_WaitForReady();

    /* Copy calculated inverse modulo */
    memcpy((void*)_inverseModuloPtr,
           (void*)((uint32_t)CRYPTO->MEM_BUFF + 4 * Cy_Crypto_Core_Vu_RegDataPtrRead(inverseModuloReg)),
           _nByteLength);

    CY_CRYPTO_VU_FREE_MEM((1 << modReg) | (1 << inverseModuloReg) | (1 << barrettReg) | (1 << rBarReg));

    return (result);
}

#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */
