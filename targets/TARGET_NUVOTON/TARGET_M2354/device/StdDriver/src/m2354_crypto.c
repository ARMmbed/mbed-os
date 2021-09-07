/**************************************************************************//**
 * @file     crypto.c
 * @version  V3.00
 * @brief  Cryptographic Accelerator driver source file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "NuMicro.h"

#define ENABLE_DEBUG    0

#define ECC_SCA_PROTECT 1       // Enable Side-Channel Protecton

#if ENABLE_DEBUG
#define CRPT_DBGMSG   printf
#else
#define CRPT_DBGMSG(...)   do { } while (0)       /* disable debug */
#endif

#if defined(__ICCARM__)
# pragma diag_suppress=Pm073, Pm143        /* Misra C rule 14.7 */
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup CRYPTO_Driver CRYPTO Driver
  @{
*/


/** @addtogroup CRYPTO_EXPORTED_FUNCTIONS CRYPTO Exported Functions
  @{
*/

/* // @cond HIDDEN_SYMBOLS */


static char  hex_char_tbl[] = "0123456789abcdef";

static void dump_ecc_reg(char *str, uint32_t volatile regs[], int32_t count);
static char get_Nth_nibble_char(uint32_t val32, uint32_t idx);
static void Hex2Reg(char input[], uint32_t volatile reg[]);
static void Reg2Hex(int32_t count, uint32_t volatile reg[], char output[]);
static char ch2hex(char ch);
static void Hex2RegEx(char input[], uint32_t volatile reg[], int shift);
static int  get_nibble_value(char c);
int32_t ECC_Mutiply(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char x1[], char y1[], char *k, char x2[], char y2[]);
void ECC_Complete(CRPT_T *crpt);


/* // @endcond HIDDEN_SYMBOLS */

/**
  * @brief  Open PRNG function
  * @param[in]  crpt         The pointer of CRYPTO module
  * @param[in]  u32KeySize   it is PRNG key size, including:
  *         - \ref PRNG_KEY_SIZE_64
  *         - \ref PRNG_KEY_SIZE_128
  *         - \ref PRNG_KEY_SIZE_192
  *         - \ref PRNG_KEY_SIZE_256
  * @param[in]  u32SeedReload is PRNG seed reload or not, including:
  *         - \ref PRNG_SEED_CONT
  *         - \ref PRNG_SEED_RELOAD
  * @param[in]  u32Seed  The new seed. Only valid when u32SeedReload is PRNG_SEED_RELOAD.
  * @return None
  */
void PRNG_Open(CRPT_T *crpt, uint32_t u32KeySize, uint32_t u32SeedReload, uint32_t u32Seed)
{
    if(u32SeedReload)
    {
        crpt->PRNG_SEED = u32Seed;
    }

    crpt->PRNG_CTL = (u32KeySize << CRPT_PRNG_CTL_KEYSZ_Pos) |
                     (u32SeedReload << CRPT_PRNG_CTL_SEEDRLD_Pos);
}

/**
  * @brief  Start to generate one PRNG key.
  * @param[in]  crpt         The pointer of CRYPTO module
  * @return None
  */
void PRNG_Start(CRPT_T *crpt)
{
    crpt->PRNG_CTL |= CRPT_PRNG_CTL_START_Msk;

    /* Waiting for PRNG Busy */
    while(crpt->PRNG_CTL & CRPT_PRNG_CTL_BUSY_Msk) {}

}

/**
  * @brief  Read the PRNG key.
  * @param[in]   crpt         The pointer of CRYPTO module
  * @param[out]  u32RandKey  The key buffer to store newly generated PRNG key.
  * @return None
  */
void PRNG_Read(CRPT_T *crpt, uint32_t u32RandKey[])
{
    uint32_t  i, wcnt;

    wcnt = (((crpt->PRNG_CTL & CRPT_PRNG_CTL_KEYSZ_Msk) >> CRPT_PRNG_CTL_KEYSZ_Pos) + 1U) * 2U;

    for(i = 0U; i < wcnt; i++)
    {
        u32RandKey[i] = crpt->PRNG_KEY[i];
    }

    crpt->PRNG_CTL &= ~CRPT_PRNG_CTL_SEEDRLD_Msk;
}


/**
  * @brief  Open AES encrypt/decrypt function.
  * @param[in]  crpt         The pointer of CRYPTO module
  * @param[in]  u32Channel   AES channel. Must be 0~3.
  * @param[in]  u32EncDec    1: AES encode;  0: AES decode
  * @param[in]  u32OpMode    AES operation mode, including:
  *         - \ref AES_MODE_ECB
  *         - \ref AES_MODE_CBC
  *         - \ref AES_MODE_CFB
  *         - \ref AES_MODE_OFB
  *         - \ref AES_MODE_CTR
  *         - \ref AES_MODE_CBC_CS1
  *         - \ref AES_MODE_CBC_CS2
  *         - \ref AES_MODE_CBC_CS3
  * @param[in]  u32KeySize is AES key size, including:
  *         - \ref AES_KEY_SIZE_128
  *         - \ref AES_KEY_SIZE_192
  *         - \ref AES_KEY_SIZE_256
  * @param[in]  u32SwapType is AES input/output data swap control, including:
  *         - \ref AES_NO_SWAP
  *         - \ref AES_OUT_SWAP
  *         - \ref AES_IN_SWAP
  *         - \ref AES_IN_OUT_SWAP
  * @return None
  */
void AES_Open(CRPT_T *crpt, uint32_t u32Channel, uint32_t u32EncDec,
              uint32_t u32OpMode, uint32_t u32KeySize, uint32_t u32SwapType)
{
    (void)u32Channel;

    crpt->AES_CTL = (u32EncDec << CRPT_AES_CTL_ENCRPT_Pos) |
                    (u32OpMode << CRPT_AES_CTL_OPMODE_Pos) |
                    (u32KeySize << CRPT_AES_CTL_KEYSZ_Pos) |
                    (u32SwapType << CRPT_AES_CTL_OUTSWAP_Pos);

}

/**
  * @brief  Start AES encrypt/decrypt
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  u32Channel  AES channel. Must be 0~3.
  * @param[in]  u32DMAMode  AES DMA control, including:
  *         - \ref CRYPTO_DMA_ONE_SHOT   One shot AES encrypt/decrypt.
  *         - \ref CRYPTO_DMA_CONTINUE   Continuous AES encrypt/decrypt.
  *         - \ref CRYPTO_DMA_LAST       Last AES encrypt/decrypt of a series of AES_Start.
  * @return None
  */
void AES_Start(CRPT_T *crpt, int32_t u32Channel, uint32_t u32DMAMode)
{
    (void)u32Channel;

    crpt->AES_CTL |= CRPT_AES_CTL_START_Msk | (u32DMAMode << CRPT_AES_CTL_DMALAST_Pos);
}

/**
  * @brief  Set AES keys
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  u32Channel  AES channel. Must be 0~3.
  * @param[in]  au32Keys    An word array contains AES keys.
  * @param[in]  u32KeySize is AES key size, including:
  *         - \ref AES_KEY_SIZE_128
  *         - \ref AES_KEY_SIZE_192
  *         - \ref AES_KEY_SIZE_256
  * @return None
  */
void AES_SetKey(CRPT_T *crpt, uint32_t u32Channel, uint32_t au32Keys[], uint32_t u32KeySize)
{
    uint32_t  i, wcnt, key_reg_addr;

    (void) u32Channel;

    key_reg_addr = (uint32_t)&crpt->AES_KEY[0];
    wcnt = 4UL + u32KeySize * 2UL;

    for(i = 0U; i < wcnt; i++)
    {
        outpw(key_reg_addr, au32Keys[i]);
        key_reg_addr += 4UL;
    }
}



/**
  * @brief  Set AES keys index of Key Store
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  mem         Memory type of Key Store key. it could be:
  *                              - \ref KS_SRAM
  *                              - \ref KS_FLASH
  *                              - \ref KS_OTP
  * @param[in]  i32KeyIdx   Index of the key in Key Store.
  * @details    AES could use the key in Key Store. This function is used to set the key index of Key Store.
  */
void AES_SetKey_KS(CRPT_T *crpt, KS_MEM_Type mem, int32_t i32KeyIdx)
{
    /* Use key in key store */
    crpt->AES_KSCTL = CRPT_AES_KSCTL_RSRC_Msk /* use KS */  |
                      (uint32_t)((int)mem << CRPT_AES_KSCTL_RSSRC_Pos) /* KS Memory type */ |
                      (uint32_t)i32KeyIdx /* key num */ ;

}


/**
  * @brief  Set AES initial vectors
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  u32Channel  AES channel. Must be 0~3.
  * @param[in]  au32IV      A four entry word array contains AES initial vectors.
  * @return None
  */
void AES_SetInitVect(CRPT_T *crpt, uint32_t u32Channel, uint32_t au32IV[])
{
    uint32_t  i, key_reg_addr;

    (void) u32Channel;

    key_reg_addr = (uint32_t)&crpt->AES_IV[0];

    for(i = 0U; i < 4U; i++)
    {
        outpw(key_reg_addr, au32IV[i]);
        key_reg_addr += 4UL;
    }
}

/**
  * @brief  Set AES DMA transfer configuration.
  * @param[in]  crpt         The pointer of CRYPTO module
  * @param[in]  u32Channel   AES channel. Must be 0~3.
  * @param[in]  u32SrcAddr   AES DMA source address
  * @param[in]  u32DstAddr   AES DMA destination address
  * @param[in]  u32TransCnt  AES DMA transfer byte count
  * @return None
  */
void AES_SetDMATransfer(CRPT_T *crpt, uint32_t u32Channel, uint32_t u32SrcAddr,
                        uint32_t u32DstAddr, uint32_t u32TransCnt)
{
    (void) u32Channel;

    crpt->AES_SADDR = u32SrcAddr;
    crpt->AES_DADDR = u32DstAddr;
    crpt->AES_CNT   = u32TransCnt;

}

/**
  * @brief  Open SHA encrypt function.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  u32OpMode   SHA operation mode, including:
  *         - \ref SHA_MODE_SHA1
  *         - \ref SHA_MODE_SHA224
  *         - \ref SHA_MODE_SHA256
  * @param[in]  u32SwapType is SHA input/output data swap control, including:
  *         - \ref SHA_NO_SWAP
  *         - \ref SHA_OUT_SWAP
  *         - \ref SHA_IN_SWAP
  *         - \ref SHA_IN_OUT_SWAP
  * @param[in]  hmac_key_len   HMAC key byte count
  * @return None
  */
void SHA_Open(CRPT_T *crpt, uint32_t u32OpMode, uint32_t u32SwapType, uint32_t hmac_key_len)
{
    crpt->HMAC_CTL = (u32OpMode << CRPT_HMAC_CTL_OPMODE_Pos) |
                     (u32SwapType << CRPT_HMAC_CTL_OUTSWAP_Pos);

    if(hmac_key_len != 0UL)
    {
        crpt->HMAC_KEYCNT = hmac_key_len;
    }
}

/**
  * @brief  Start SHA encrypt
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  u32DMAMode  TDES DMA control, including:
  *         - \ref CRYPTO_DMA_ONE_SHOT   One shop SHA encrypt.
  *         - \ref CRYPTO_DMA_CONTINUE   Continuous SHA encrypt.
  *         - \ref CRYPTO_DMA_LAST       Last SHA encrypt of a series of SHA_Start.
  * @return None
  */
void SHA_Start(CRPT_T *crpt, uint32_t u32DMAMode)
{
    crpt->HMAC_CTL &= ~(0x7UL << CRPT_HMAC_CTL_DMALAST_Pos);
    crpt->HMAC_CTL |= CRPT_HMAC_CTL_START_Msk | (u32DMAMode << CRPT_HMAC_CTL_DMALAST_Pos);
}

/**
  * @brief  Set SHA DMA transfer
  * @param[in]  crpt         The pointer of CRYPTO module
  * @param[in]  u32SrcAddr   SHA DMA source address
  * @param[in]  u32TransCnt  SHA DMA transfer byte count
  * @return None
  */
void SHA_SetDMATransfer(CRPT_T *crpt, uint32_t u32SrcAddr, uint32_t u32TransCnt)
{
    crpt->HMAC_SADDR = u32SrcAddr;
    crpt->HMAC_DMACNT = u32TransCnt;
}

/**
  * @brief  Read the SHA digest.
  * @param[in]   crpt       The pointer of CRYPTO module
  * @param[out]  u32Digest  The SHA encrypt output digest.
  * @return None
  */
void SHA_Read(CRPT_T *crpt, uint32_t u32Digest[])
{
    uint32_t  i, wcnt, reg_addr;

    i = (crpt->HMAC_CTL & CRPT_HMAC_CTL_OPMODE_Msk) >> CRPT_HMAC_CTL_OPMODE_Pos;

    if(i == SHA_MODE_SHA1)
    {
        wcnt = 5UL;
    }
    else if(i == SHA_MODE_SHA224)
    {
        wcnt = 7UL;
    }
    else if(i == SHA_MODE_SHA256)
    {
        wcnt = 8UL;
    }
    else if(i == SHA_MODE_SHA384)
    {
        wcnt = 12UL;
    }
    else
    {
        /* SHA_MODE_SHA512 */
        wcnt = 16UL;
    }

    reg_addr = (uint32_t) & (crpt->HMAC_DGST[0]);
    for(i = 0UL; i < wcnt; i++)
    {
        u32Digest[i] = inpw(reg_addr);
        reg_addr += 4UL;
    }
}


/*-----------------------------------------------------------------------------------------------*/
/*                                                                                               */
/*    ECC                                                                                        */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

#define ECCOP_POINT_MUL     (0x0UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_MODULE        (0x1UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_ADD     (0x2UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_DOUBLE  (0x0UL << CRPT_ECC_CTL_ECCOP_Pos)

#define MODOP_DIV           (0x0UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_MUL           (0x1UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_ADD           (0x2UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_SUB           (0x3UL << CRPT_ECC_CTL_MODOP_Pos)

#define OP_ECDSAS           (0x1UL << CRPT_ECC_CTL_ECDSAS_Pos)
#define OP_ECDSAR           (0x1UL << CRPT_ECC_CTL_ECDSAR_Pos)

enum
{
    CURVE_GF_P,
    CURVE_GF_2M,
};

/*-----------------------------------------------------*/
/*  Define elliptic curve (EC):                        */
/*-----------------------------------------------------*/
static const ECC_CURVE _Curve[] =
{
    {
        /* NIST: Curve P-192 : y^2=x^3-ax+b (mod p) */
        CURVE_P_192,
        48,     /* Echar */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC",   /* "000000000000000000000000000000000000000000000003" */
        "64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1",
        "188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012",
        "07192b95ffc8da78631011ed6b24cdd573f977a11e794811",
        58,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF",   /* "6277101735386680763835789423207666416083908700390324961279" */
        58,     /* Eol */
        "FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831",   /* "6277101735386680763835789423176059013767194773182842284081" */
        192,    /* key_len */
        7,
        2,
        1,
        CURVE_GF_P
    },
    {
        /* NIST: Curve P-224 : y^2=x^3-ax+b (mod p) */
        CURVE_P_224,
        56,     /* Echar */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE",  /* "00000000000000000000000000000000000000000000000000000003" */
        "b4050a850c04b3abf54132565044b0b7d7bfd8ba270b39432355ffb4",
        "b70e0cbd6bb4bf7f321390b94a03c1d356c21122343280d6115c1d21",
        "bd376388b5f723fb4c22dfe6cd4375a05a07476444d5819985007e34",
        70,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "0026959946667150639794667015087019630673557916260026308143510066298881" */
        70,     /* Eol */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D",  /* "0026959946667150639794667015087019625940457807714424391721682722368061" */
        224,    /* key_len */
        9,
        8,
        3,
        CURVE_GF_P
    },
    {
        /* NIST: Curve P-256 : y^2=x^3-ax+b (mod p) */
        CURVE_P_256,
        64,     /* Echar */
        "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC",  /* "0000000000000000000000000000000000000000000000000000000000000003" */
        "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b",
        "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296",
        "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5",
        78,     /* Epl */
        "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF",  /* "115792089210356248762697446949407573530086143415290314195533631308867097853951" */
        78,     /* Eol */
        "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551",  /* "115792089210356248762697446949407573529996955224135760342422259061068512044369" */
        256,    /* key_len */
        10,
        5,
        2,
        CURVE_GF_P
    },
    {
        /* NIST: Curve P-384 : y^2=x^3-ax+b (mod p) */
        CURVE_P_384,
        96,     /* Echar */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC",  /* "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003" */
        "b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aef",
        "aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7",
        "3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5f",
        116,    /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF",  /* "39402006196394479212279040100143613805079739270465446667948293404245721771496870329047266088258938001861606973112319" */
        116,    /* Eol */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973",  /* "39402006196394479212279040100143613805079739270465446667946905279627659399113263569398956308152294913554433653942643" */
        384,    /* key_len */
        12,
        3,
        2,
        CURVE_GF_P
    },
    {
        /* NIST: Curve P-521 : y^2=x^3-ax+b (mod p)*/
        CURVE_P_521,
        131,    /* Echar */
        "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC",  /* "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003" */
        "051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00",
        "0c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66",
        "11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650",
        157,    /* Epl */
        "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",  /* "6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151" */
        157,    /* Eol */
        "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA51868783BF2F966B7FCC0148F709A5D03BB5C9B8899C47AEBB6FB71E91386409",  /* "6864797660130609714981900799081393217269435300143305409394463459185543183397655394245057746333217197532963996371363321113864768612440380340372808892707005449" */
        521,    /* key_len */
        32,
        32,
        32,
        CURVE_GF_P
    },
    {
        /* NIST: Curve B-163 : y^2+xy=x^3+ax^2+b */
        CURVE_B_163,
        41,     /* Echar */
        "00000000000000000000000000000000000000001",
        "20a601907b8c953ca1481eb10512f78744a3205fd",
        "3f0eba16286a2d57ea0991168d4994637e8343e36",
        "0d51fbc6c71a0094fa2cdd545b11c5c0c797324f1",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        49,     /* Eol */
        "40000000000000000000292FE77E70C12A4234C33",   /* "5846006549323611672814742442876390689256843201587" */
        163,    /* key_len */
        7,
        6,
        3,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve B-233 : y^2+xy=x^3+ax^2+b */
        CURVE_B_233,
        59,     /* Echar 59 */
        "00000000000000000000000000000000000000000000000000000000001",
        "066647ede6c332c7f8c0923bb58213b333b20e9ce4281fe115f7d8f90ad",
        "0fac9dfcbac8313bb2139f1bb755fef65bc391f8b36f8f8eb7371fd558b",
        "1006a08a41903350678e58528bebf8a0beff867a7ca36716f7e01f81052",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        70,     /* Eol */
        "1000000000000000000000000000013E974E72F8A6922031D2603CFE0D7",  /* "6901746346790563787434755862277025555839812737345013555379383634485463" */
        233,    /* key_len */
        74,
        74,
        74,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve B-283 : y^2+xy=x^3+ax^2+b */
        CURVE_B_283,
        71,     /* Echar */
        "00000000000000000000000000000000000000000000000000000000000000000000001",
        "27b680ac8b8596da5a4af8a19a0303fca97fd7645309fa2a581485af6263e313b79a2f5",
        "5f939258db7dd90e1934f8c70b0dfec2eed25b8557eac9c80e2e198f8cdbecd86b12053",
        "3676854fe24141cb98fe6d4b20d02b4516ff702350eddb0826779c813f0df45be8112f4",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        85,     /* Eol */
        "3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEF90399660FC938A90165B042A7CEFADB307",  /* "7770675568902916283677847627294075626569625924376904889109196526770044277787378692871" */
        283,    /* key_len */
        12,
        7,
        5,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve B-409 : y^2+xy=x^3+ax^2+b */
        CURVE_B_409,
        103,    /* Echar */
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
        "021a5c2c8ee9feb5c4b9a753b7b476b7fd6422ef1f3dd674761fa99d6ac27c8a9a197b272822f6cd57a55aa4f50ae317b13545f",
        "15d4860d088ddb3496b0c6064756260441cde4af1771d4db01ffe5b34e59703dc255a868a1180515603aeab60794e54bb7996a7",
        "061b1cfab6be5f32bbfa78324ed106a7636b9c5a7bd198d0158aa4f5488d08f38514f1fdf4b4f40d2181b3681c364ba0273c706",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        123,    /* Eol */
        "10000000000000000000000000000000000000000000000000001E2AAD6A612F33307BE5FA47C3C9E052F838164CD37D9A21173",  /* "661055968790248598951915308032771039828404682964281219284648798304157774827374805208143723762179110965979867288366567526771" */
        409,    /* key_len */
        87,
        87,
        87,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve B-571 : y^2+xy=x^3+ax^2+b */
        CURVE_B_571,
        143,    /* Echar */
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
        "2f40e7e2221f295de297117b7f3d62f5c6a97ffcb8ceff1cd6ba8ce4a9a18ad84ffabbd8efa59332be7ad6756a66e294afd185a78ff12aa520e4de739baca0c7ffeff7f2955727a",
        "303001d34b856296c16c0d40d3cd7750a93d1d2955fa80aa5f40fc8db7b2abdbde53950f4c0d293cdd711a35b67fb1499ae60038614f1394abfa3b4c850d927e1e7769c8eec2d19",
        "37bf27342da639b6dccfffeb73d69d78c6c27a6009cbbca1980f8533921e8a684423e43bab08a576291af8f461bb2a8b3531d2f0485c19b16e2f1516e23dd3c1a4827af1b8ac15b",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        172,    /* Eol */
        "3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE661CE18FF55987308059B186823851EC7DD9CA1161DE93D5174D66E8382E9BB2FE84E47",  /* "3864537523017258344695351890931987344298927329706434998657235251451519142289560424536143999389415773083133881121926944486246872462816813070234528288303332411393191105285703" */
        571,    /* key_len */
        10,
        5,
        2,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve K-163 : y^2+xy=x^3+ax^2+b */
        CURVE_K_163,
        41,     /* Echar */
        "00000000000000000000000000000000000000001",
        "00000000000000000000000000000000000000001",
        "2fe13c0537bbc11acaa07d793de4e6d5e5c94eee8",
        "289070fb05d38ff58321f2e800536d538ccdaa3d9",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        49,     /* Eol */
        "4000000000000000000020108A2E0CC0D99F8A5EF",  /* "5846006549323611672814741753598448348329118574063" */
        163,    /* key_len */
        7,
        6,
        3,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve K-233 : y^2+xy=x^3+ax^2+b */
        CURVE_K_233,
        59,     /* Echar 59 */
        "00000000000000000000000000000000000000000000000000000000000",
        "00000000000000000000000000000000000000000000000000000000001",
        "17232ba853a7e731af129f22ff4149563a419c26bf50a4c9d6eefad6126",
        "1db537dece819b7f70f555a67c427a8cd9bf18aeb9b56e0c11056fae6a3",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",    /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        70,     /* Eol */
        "8000000000000000000000000000069D5BB915BCD46EFB1AD5F173ABDF",  /* "3450873173395281893717377931138512760570940988862252126328087024741343" */
        233,    /* key_len */
        74,
        74,
        74,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve K-283 : y^2+xy=x^3+ax^2+b */
        CURVE_K_283,
        71,     /* Echar */
        "00000000000000000000000000000000000000000000000000000000000000000000000",
        "00000000000000000000000000000000000000000000000000000000000000000000001",
        "503213f78ca44883f1a3b8162f188e553cd265f23c1567a16876913b0c2ac2458492836",
        "1ccda380f1c9e318d90f95d07e5426fe87e45c0e8184698e45962364e34116177dd2259",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        85,     /* Eol */
        "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9AE2ED07577265DFF7F94451E061E163C61",  /* "3885337784451458141838923813647037813284811733793061324295874997529815829704422603873" */
        283,    /* key_len */
        12,
        7,
        5,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve K-409 : y^2+xy=x^3+ax^2+b */
        CURVE_K_409,
        103,    /* Echar */
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
        "060f05f658f49c1ad3ab1890f7184210efd0987e307c84c27accfb8f9f67cc2c460189eb5aaaa62ee222eb1b35540cfe9023746",
        "1e369050b7c4e42acba1dacbf04299c3460782f918ea427e6325165e9ea10e3da5f6c42e9c55215aa9ca27a5863ec48d8e0286b",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        123,    /* Eol */
        "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE5F83B2D4EA20400EC4557D5ED3E3E7CA5B4B5C83B8E01E5FCF",  /* "330527984395124299475957654016385519914202341482140609642324395022880711289249191050673258457777458014096366590617731358671" */
        409,    /* key_len */
        87,
        87,
        87,
        CURVE_GF_2M
    },
    {
        /* NIST: Curve K-571 : y^2+xy=x^3+ax^2+b */
        CURVE_K_571,
        143,    /* Echar */
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
        "26eb7a859923fbc82189631f8103fe4ac9ca2970012d5d46024804801841ca44370958493b205e647da304db4ceb08cbbd1ba39494776fb988b47174dca88c7e2945283a01c8972",
        "349dc807f4fbf374f4aeade3bca95314dd58cec9f307a54ffc61efc006d8a2c9d4979c0ac44aea74fbebbb9f772aedcb620b01a7ba7af1b320430c8591984f601cd4c143ef1c7a3",
        68,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  /* "26959946667150639794667015087019630673557916260026308143510066298881" */
        172,    /* Eol */
        "20000000000000000000000000000000000000000000000000000000000000000000000131850E1F19A63E4B391A8DB917F4138B630D84BE5D639381E91DEB45CFE778F637C1001",  /* "1932268761508629172347675945465993672149463664853217499328617625725759571144780212268133978522706711834706712800825351461273674974066617311929682421617092503555733685276673" */
        571,    /* key_len */
        10,
        5,
        2,
        CURVE_GF_2M
    },
    {
        /* Koblitz: Curve secp192k1 : y2 = x3+ax+b over Fp */
        CURVE_KO_192,
        48,     /* Echar */
        "00000000000000000000000000000000000000000",
        "00000000000000000000000000000000000000003",
        "DB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D",
        "9B2F2F6D9C5628A7844163D015BE86344082AA88D95E2F9D",
        58,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFEE37",  /* p */
        58,     /* Eol */
        "FFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D",  /* n */
        192,    /* key_len */
        7,
        2,
        1,
        CURVE_GF_P
    },
    {
        /* Koblitz: Curve secp224k1 : y2 = x3+ax+b over Fp */
        CURVE_KO_224,
        56,     /* Echar */
        "00000000000000000000000000000000000000000000000000000000",
        "00000000000000000000000000000000000000000000000000000005",
        "A1455B334DF099DF30FC28A169A467E9E47075A90F7E650EB6B7A45C",
        "7E089FED7FBA344282CAFBD6F7E319F7C0B0BD59E2CA4BDB556D61A5",
        70,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFE56D",  /* p */
        70,     /* Eol */
        "0000000000000000000000000001DCE8D2EC6184CAF0A971769FB1F7",  /* n */
        224,    /* key_len */
        7,
        2,
        1,
        CURVE_GF_P
    },
    {
        /* Koblitz: Curve secp256k1 : y2 = x3+ax+b over Fp */
        CURVE_KO_256,
        64,     /* Echar */
        "0000000000000000000000000000000000000000000000000000000000000000",
        "0000000000000000000000000000000000000000000000000000000000000007",
        "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798",
        "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8",
        78,     /* Epl */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F",  /* p */
        78,     /* Eol */
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141",  /* n */
        256,    /* key_len */
        7,
        2,
        1,
        CURVE_GF_P
    },
    {
        /* Brainpool: Curve brainpoolP256r1 */
        CURVE_BP_256,
        64,     /* Echar */
        "7D5A0975FC2C3057EEF67530417AFFE7FB8055C126DC5C6CE94A4B44F330B5D9",  /* A */
        "26DC5C6CE94A4B44F330B5D9BBD77CBF958416295CF7E1CE6BCCDC18FF8C07B6",  /* B */
        "8BD2AEB9CB7E57CB2C4B482FFC81B7AFB9DE27E1E3BD23C23A4453BD9ACE3262",  /* x */
        "547EF835C3DAC4FD97F8461A14611DC9C27745132DED8E545C1D54C72F046997",  /* y */
        78,     /* Epl */
        "A9FB57DBA1EEA9BC3E660A909D838D726E3BF623D52620282013481D1F6E5377",  /* p */
        78,     /* Eol */
        "A9FB57DBA1EEA9BC3E660A909D838D718C397AA3B561A6F7901E0E82974856A7",  /* q */
        256,    /* key_len */
        7,
        2,
        1,
        CURVE_GF_P
    },
    {
        /* Brainpool: Curve brainpoolP384r1 */
        CURVE_BP_384,
        96,     /* Echar */
        "7BC382C63D8C150C3C72080ACE05AFA0C2BEA28E4FB22787139165EFBA91F90F8AA5814A503AD4EB04A8C7DD22CE2826",  /* A */
        "04A8C7DD22CE28268B39B55416F0447C2FB77DE107DCD2A62E880EA53EEB62D57CB4390295DBC9943AB78696FA504C11",  /* B */
        "1D1C64F068CF45FFA2A63A81B7C13F6B8847A3E77EF14FE3DB7FCAFE0CBD10E8E826E03436D646AAEF87B2E247D4AF1E",  /* x */
        "8ABE1D7520F9C2A45CB1EB8E95CFD55262B70B29FEEC5864E19C054FF99129280E4646217791811142820341263C5315",  /* y */
        116,     /* Epl */
        "8CB91E82A3386D280F5D6F7E50E641DF152F7109ED5456B412B1DA197FB71123ACD3A729901D1A71874700133107EC53",  /* p */
        116,     /* Eol */
        "8CB91E82A3386D280F5D6F7E50E641DF152F7109ED5456B31F166E6CAC0425A7CF3AB6AF6B7FC3103B883202E9046565",  /* q */
        384,    /* key_len */
        7,
        2,
        1,
        CURVE_GF_P
    },
    {
        /* Brainpool: Curve brainpoolP512r1 */
        CURVE_BP_512,
        128,     /* Echar */
        "7830A3318B603B89E2327145AC234CC594CBDD8D3DF91610A83441CAEA9863BC2DED5D5AA8253AA10A2EF1C98B9AC8B57F1117A72BF2C7B9E7C1AC4D77FC94CA",  /* A */
        "3DF91610A83441CAEA9863BC2DED5D5AA8253AA10A2EF1C98B9AC8B57F1117A72BF2C7B9E7C1AC4D77FC94CADC083E67984050B75EBAE5DD2809BD638016F723",  /* B */
        "81AEE4BDD82ED9645A21322E9C4C6A9385ED9F70B5D916C1B43B62EEF4D0098EFF3B1F78E2D0D48D50D1687B93B97D5F7C6D5047406A5E688B352209BCB9F822",  /* x */
        "7DDE385D566332ECC0EABFA9CF7822FDF209F70024A57B1AA000C55B881F8111B2DCDE494A5F485E5BCA4BD88A2763AED1CA2B2FA8F0540678CD1E0F3AD80892",  /* y */
        156,     /* Epl */
        "AADD9DB8DBE9C48B3FD4E6AE33C9FC07CB308DB3B3C9D20ED6639CCA703308717D4D9B009BC66842AECDA12AE6A380E62881FF2F2D82C68528AA6056583A48F3",  /* p */
        156,     /* Eol */
        "AADD9DB8DBE9C48B3FD4E6AE33C9FC07CB308DB3B3C9D20ED6639CCA70330870553E5C414CA92619418661197FAC10471DB1D381085DDADDB58796829CA90069",  /* q */
        512,    /* key_len */
        7,
        2,
        1,
        CURVE_GF_P
    },
    {
        CURVE_25519,
        64,		// Echar
        "0000000000000000000000000000000000000000000000000000000000076D06",  // "0000000000000000000000000000000000000000000000000000000000000003",
        "0000000000000000000000000000000000000000000000000000000000000001",
        "0000000000000000000000000000000000000000000000000000000000000009",
        "20ae19a1b8a086b4e01edd2c7748d14c923d4d7e6d7c61b229e9c5a27eced3d9",
        78,		// Epl
        "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed",  // "115792089210356248762697446949407573530086143415290314195533631308867097853951",
        78,		// Eol
        "1000000000000000000000000000000014def9dea2f79cd65812631a5cf5d3ed",  // "115792089210356248762697446949407573529996955224135760342422259061068512044369",
        255,	// key_len
        10,
        5,
        2,
        CURVE_GF_P
    },
    {
        /* NIST: Curve P-256 : y^2=x^3-ax+b (mod p) */
        CURVE_SM2_256,
        64,     /* Echar */
        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC",  /* a */
        "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93",  /* b */
        "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7",  /* x */
        "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0",  /* y */
        78,     /* Epl */
        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF",  /* p */
        78,     /* Eol */
        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123",  /* n */
        256,    /* key_len */
        10,
        5,
        2,
        CURVE_GF_P
    },

};


static ECC_CURVE  *pCurve;
static ECC_CURVE  Curve_Copy;

static ECC_CURVE * get_curve(E_ECC_CURVE ecc_curve);
static int32_t ecc_init_curve(CRPT_T *crpt, E_ECC_CURVE ecc_curve);
static void run_ecc_codec(CRPT_T *crpt, uint32_t mode);

static char  temp_hex_str[160];

static volatile uint32_t g_ECC_done, g_ECCERR_done;

void ECC_DriverISR(CRPT_T *crpt)
{
    if(crpt->INTSTS & CRPT_INTSTS_ECCIF_Msk)
    {
        g_ECC_done = 1UL;
        crpt->INTSTS = CRPT_INTSTS_ECCIF_Msk;
        /* printf("ECC done IRQ.\n"); */
    }

    if(crpt->INTSTS & CRPT_INTSTS_ECCEIF_Msk)
    {
        g_ECCERR_done = 1UL;
        crpt->INTSTS = CRPT_INTSTS_ECCEIF_Msk;
        /* printf("ECCERRIF is set!!\n"); */
    }
}


#if ENABLE_DEBUG
static void dump_ecc_reg(char *str, uint32_t volatile regs[], int32_t count)
{
    int32_t  i;

    printf("%s => ", str);
    for(i = 0; i < count; i++)
    {
        printf("0x%08x ", regs[i]);
    }
    printf("\n");
}
#else
static void dump_ecc_reg(char *str, uint32_t volatile regs[], int32_t count)
{
    (void)str;
    (void)regs;
    (void)count;
}
#endif
static char  ch2hex(char ch)
{
    if(ch <= '9')
    {
        return ch - '0';
    }
    else if((ch <= 'z') && (ch >= 'a'))
    {
        return ch - 'a' + 10U;
    }
    else
    {
        return ch - 'A' + 10U;
    }
}

static void Hex2Reg(char input[], uint32_t volatile reg[])
{
    char      hex;
    int       si, ri;
    uint32_t  i, val32;

    si = (int)strlen(input) - 1;
    ri = 0;

    while(si >= 0)
    {
        val32 = 0UL;
        for(i = 0UL; (i < 8UL) && (si >= 0); i++)
        {
            hex = ch2hex(input[si]);
            val32 |= (uint32_t)hex << (i * 4UL);
            si--;
        }
        reg[ri++] = val32;
    }
}

static void Hex2RegEx(char input[], uint32_t volatile reg[], int shift)
{
    uint32_t  hex, carry;
    int       si, ri;
    uint32_t  i, val32;

    si = (int)strlen(input) - 1;
    ri = 0;
    carry = 0U;
    while(si >= 0)
    {
        val32 = 0UL;
        for(i = 0UL; (i < 8UL) && (si >= 0); i++)
        {
            hex = (uint32_t)ch2hex(input[si]);
            hex <<= shift;

            val32 |= (uint32_t)((hex & 0xFU) | carry) << (i * 4UL);
            carry = (hex >> 4) & 0xFU;
            si--;
        }
        reg[ri++] = val32;
    }
    if(carry != 0U)
    {
        reg[ri] = carry;
    }
}

/**
  * @brief  Extract specified nibble from an unsigned word in character format.
  *         For example:
  *                Suppose val32 is 0x786543210, get_Nth_nibble_char(val32, 3) will return a '3'.
  * @param[in]  val32   The input unsigned word
  * @param[in]  idx     The Nth nibble to be extracted.
  * @return  The nibble in character format.
  */
static char get_Nth_nibble_char(uint32_t val32, uint32_t idx)
{
    return hex_char_tbl[(val32 >> (idx * 4U)) & 0xfU ];
}


static void Reg2Hex(int32_t count, uint32_t volatile reg[], char output[])
{
    int32_t    idx, ri;
    uint32_t   i;

    output[count] = 0U;
    idx = count - 1;

    for(ri = 0; idx >= 0; ri++)
    {
        for(i = 0UL; (i < 8UL) && (idx >= 0); i++)
        {
            output[idx] = get_Nth_nibble_char(reg[ri], i);
            idx--;
        }
    }
}

/**
  * @brief  Translate registers value into hex string
  * @param[in]  count   The string length of ouptut hex string.
  * @param[in]  reg     Register array.
  * @param[in]  output  String buffer for output hex string.
  */
void CRPT_Reg2Hex(int32_t count,volatile uint32_t reg[], char output[])
{
    Reg2Hex(count, reg, output);
}

/**
  * @brief  Translate hex string to registers value
  * @param[in]  input   hex string.
  * @param[in]  reg     Register array.
  */
void CRPT_Hex2Reg(char input[], uint32_t volatile reg[])
{
    Hex2Reg(input, reg);
}


static int32_t ecc_init_curve(CRPT_T *crpt, E_ECC_CURVE ecc_curve)
{
    int32_t  i, ret = 0;

    pCurve = get_curve(ecc_curve);
    if(pCurve == NULL)
    {
        CRPT_DBGMSG("Cannot find curve %d!!\n", ecc_curve);
        ret = -1;
    }

    if(ret == 0)
    {
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_A[i] = 0UL;
            crpt->ECC_B[i] = 0UL;
            crpt->ECC_X1[i] = 0UL;
            crpt->ECC_Y1[i] = 0UL;
            crpt->ECC_N[i] = 0UL;
        }

        Hex2Reg(pCurve->Ea, crpt->ECC_A);
        Hex2Reg(pCurve->Eb, crpt->ECC_B);
        Hex2Reg(pCurve->Px, crpt->ECC_X1);
        Hex2Reg(pCurve->Py, crpt->ECC_Y1);

        CRPT_DBGMSG("Key length = %d\n", pCurve->key_len);
        dump_ecc_reg("CRPT_ECC_CURVE_A", crpt->ECC_A, 10);
        dump_ecc_reg("CRPT_ECC_CURVE_B", crpt->ECC_B, 10);
        dump_ecc_reg("CRPT_ECC_POINT_X1", crpt->ECC_X1, 10);
        dump_ecc_reg("CRPT_ECC_POINT_Y1", crpt->ECC_Y1, 10);

        if(pCurve->GF == (int)CURVE_GF_2M)
        {
            crpt->ECC_N[0] = 0x1UL;
            crpt->ECC_N[(pCurve->key_len) / 32] |= (1UL << ((pCurve->key_len) % 32));
            crpt->ECC_N[(pCurve->irreducible_k1) / 32] |= (1UL << ((pCurve->irreducible_k1) % 32));
            crpt->ECC_N[(pCurve->irreducible_k2) / 32] |= (1UL << ((pCurve->irreducible_k2) % 32));
            crpt->ECC_N[(pCurve->irreducible_k3) / 32] |= (1UL << ((pCurve->irreducible_k3) % 32));
        }
        else
        {
            Hex2Reg(pCurve->Pp, crpt->ECC_N);
        }
    }
    dump_ecc_reg("CRPT_ECC_CURVE_N", crpt->ECC_N, 10);
    return ret;
}


static int  get_nibble_value(char c)
{
    char ch;

    if((c >= '0') && (c <= '9'))
    {
        ch = '0';
        return ((int)c - (int)ch);
    }

    if((c >= 'a') && (c <= 'f'))
    {
        ch = 'a';
        return ((int)c - (int)ch + 10);
    }

    if((c >= 'A') && (c <= 'F'))
    {
        ch = 'A';
        return ((int)c - (int)ch + 10);
    }
    return 0;
}


/**
  * @brief  Check if the private key is located in valid range of curve.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  private_k   The input private key.
  * @return  1    Is valid.
  * @return  0    Is not valid.
  * @return  -1   Invalid curve.
  */
int ECC_IsPrivateKeyValid(CRPT_T *crpt, E_ECC_CURVE ecc_curve,  char private_k[])
{
    uint32_t  i;

    (void)crpt;
    pCurve = get_curve(ecc_curve);
    if(pCurve == NULL)
    {
        return -1;
    }

    if(strlen(private_k) < strlen(pCurve->Eorder))
    {
        return 1;
    }

    if(strlen(private_k) > strlen(pCurve->Eorder))
    {
        return 0;
    }

    for(i = 0U; i < strlen(private_k); i++)
    {
        if(get_nibble_value(private_k[i]) < get_nibble_value(pCurve->Eorder[i]))
        {
            return 1;
        }

        if(get_nibble_value(private_k[i]) > get_nibble_value(pCurve->Eorder[i]))
        {
            return 0;
        }
    }
    return 0;
}


/**
  * @brief  Given a private key and curve to generate the public key pair.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  private_k   The input private key.
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[out] public_k1   The output publick key 1.
  * @param[out] public_k2   The output publick key 2.
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  */
int32_t  ECC_GeneratePublicKey(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *private_k, char public_k1[], char public_k2[])
{
    int32_t  ret = 0, i;
    uint32_t u32Tmp;

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if(ret == 0)
    {
        CRPT->ECC_KSCTL = 0;

        for(i = 0; i < 18; i++)
        {
            crpt->ECC_K[i] = 0UL;
        }

        Hex2Reg(private_k, crpt->ECC_K);

        /* set FSEL (Field selection) */
        if(pCurve->GF == (int)CURVE_GF_2M)
        {
            crpt->ECC_CTL = 0UL;
        }
        else           /*  CURVE_GF_P */
        {
            crpt->ECC_CTL = CRPT_ECC_CTL_FSEL_Msk;
        }

        g_ECC_done = g_ECCERR_done = 0UL;
        crpt->ECC_CTL |= ((uint32_t)pCurve->key_len << CRPT_ECC_CTL_CURVEM_Pos) |
                         ECCOP_POINT_MUL | CRPT_ECC_CTL_START_Msk;

        do
        {
            u32Tmp = g_ECC_done;
            u32Tmp |= g_ECCERR_done;
        }
        while(u32Tmp == 0UL);

        Reg2Hex(pCurve->Echar, crpt->ECC_X1, public_k1);
        Reg2Hex(pCurve->Echar, crpt->ECC_Y1, public_k2);
    }

    return ret;
}




/**
  * @brief  Given a private key and curve to generate the public key pair.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  mem         Memory type of Key Store. It could be KS_SRAM, KS_FLASH or KS_OTP.
  * @param[in]  i32KeyIdx   Index of the key in Key Store.
  * @param[out] public_k1   The output publick key 1.
  * @param[out] public_k2   The output publick key 2.
  * @param[in]  u32ExtraOp  Extra options for ECC_KSCTL register.

  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  */
int32_t  ECC_GeneratePublicKey_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, KS_MEM_Type mem, int32_t i32KeyIdx, char public_k1[], char public_k2[], uint32_t u32ExtraOp)
{
    int32_t  ret = 0;
    uint32_t u32Tmp;

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if(ret == 0)
    {

        // key from key store
        crpt->ECC_KSCTL = (uint32_t)(mem << 6)/* KS Memory Type */ |
                          (CRPT_ECC_KSCTL_RSRCK_Msk)/* Key from KS */ |
                          u32ExtraOp |
                          (uint32_t)i32KeyIdx;

        /* set FSEL (Field selection) */
        if(pCurve->GF == (int)CURVE_GF_2M)
        {
            crpt->ECC_CTL = 0UL;
        }
        else           /*  CURVE_GF_P */
        {
            crpt->ECC_CTL = CRPT_ECC_CTL_FSEL_Msk;
        }

        g_ECC_done = g_ECCERR_done = 0UL;
        crpt->ECC_CTL |= ((uint32_t)pCurve->key_len << CRPT_ECC_CTL_CURVEM_Pos) |
                         ECCOP_POINT_MUL | CRPT_ECC_CTL_START_Msk;

        do
        {
            u32Tmp = g_ECC_done;
            u32Tmp |= g_ECCERR_done;
        }
        while(u32Tmp == 0UL);

        Reg2Hex(pCurve->Echar, crpt->ECC_X1, public_k1);
        Reg2Hex(pCurve->Echar, crpt->ECC_Y1, public_k2);
    }

    return ret;
}

/**
  * @brief  Given a private key and curve to generate the public key pair.
  * @param[in]  crpt        Reference to Crypto module.
  * @param[out] x1          The x-coordinate of input point.
  * @param[out] y1          The y-coordinate of input point.
  * @param[in]  k           The private key
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[out] x2          The x-coordinate of output point.
  * @param[out] y2          The y-coordinate of output point.
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  */
int32_t  ECC_Mutiply(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char x1[], char y1[], char *k, char x2[], char y2[])
{
    int32_t  i, ret = 0;

    if (ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if (ret == 0)
    {
        for (i = 0; i < 9; i++)
        {
            crpt->ECC_X1[i] = 0UL;
            crpt->ECC_Y1[i] = 0UL;
            crpt->ECC_K[i] = 0UL;
        }

        Hex2Reg(x1, crpt->ECC_X1);
        Hex2Reg(y1, crpt->ECC_Y1);
        Hex2Reg(k, crpt->ECC_K);

        /* set FSEL (Field selection) */
        if (pCurve->GF == (int)CURVE_GF_2M)
        {
            crpt->ECC_CTL = 0UL;
        }
        else
        {
            /*  CURVE_GF_P */
            crpt->ECC_CTL = CRPT_ECC_CTL_FSEL_Msk;
        }

        g_ECC_done = g_ECCERR_done = 0UL;

        if  (ecc_curve == CURVE_25519)
        {
            printf("!! Is curve-25519 !!\n");
            crpt->ECC_CTL |= CRPT_ECC_CTL_SCAP_Msk;
            crpt->ECC_CTL |= CRPT_ECC_CTL_CSEL_Msk;

            /* If SCAP enabled, the curve order must be written to ECC_X2 */
            if(crpt->ECC_CTL & CRPT_ECC_CTL_SCAP_Msk)
            {
                Hex2Reg(pCurve->Eorder, crpt->ECC_X2);
            }
        }

        crpt->ECC_CTL |= ((uint32_t)pCurve->key_len << CRPT_ECC_CTL_CURVEM_Pos) |
                         ECCOP_POINT_MUL | CRPT_ECC_CTL_START_Msk;

        while ((g_ECC_done == 0UL) && (g_ECCERR_done == 0UL))
        {
        }

        Reg2Hex(pCurve->Echar, crpt->ECC_X1, x2);
        Reg2Hex(pCurve->Echar, crpt->ECC_Y1, y2);

    }

    return ret;
}


/**
  * @brief  Given a curve parameter, the other party's public key, and one's own private key to generate the secret Z.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  private_k   One's own private key.
  * @param[in]  public_k1   The other party's publick key 1.
  * @param[in]  public_k2   The other party's publick key 2.
  * @param[out] secret_z    The ECC CDH secret Z.
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  */
int32_t  ECC_GenerateSecretZ(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *private_k, char public_k1[], char public_k2[], char secret_z[])
{
    int32_t  i, ret = 0;
    uint32_t u32Tmp;

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if(ret == 0)
    {
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_K[i] = 0UL;
            crpt->ECC_X1[i] = 0UL;
            crpt->ECC_Y1[i] = 0UL;
        }

        if((ecc_curve == CURVE_B_163) || (ecc_curve == CURVE_B_233) || (ecc_curve == CURVE_B_283) ||
                (ecc_curve == CURVE_B_409) || (ecc_curve == CURVE_B_571) || (ecc_curve == CURVE_K_163))
        {
            Hex2RegEx(private_k, crpt->ECC_K, 1);
        }
        else if((ecc_curve == CURVE_K_233) || (ecc_curve == CURVE_K_283) ||
                (ecc_curve == CURVE_K_409) || (ecc_curve == CURVE_K_571))
        {
            Hex2RegEx(private_k, crpt->ECC_K, 2);
        }
        else
        {
            Hex2Reg(private_k, crpt->ECC_K);
        }

        Hex2Reg(public_k1, crpt->ECC_X1);
        Hex2Reg(public_k2, crpt->ECC_Y1);

        /* set FSEL (Field selection) */
        if(pCurve->GF == (int)CURVE_GF_2M)
        {
            crpt->ECC_CTL = 0UL;
        }
        else           /*  CURVE_GF_P */
        {
            crpt->ECC_CTL = CRPT_ECC_CTL_FSEL_Msk;
        }
        g_ECC_done = g_ECCERR_done = 0UL;
        crpt->ECC_CTL |= ((uint32_t)pCurve->key_len << CRPT_ECC_CTL_CURVEM_Pos) |
                         ECCOP_POINT_MUL | CRPT_ECC_CTL_START_Msk;

        do
        {
            u32Tmp = g_ECC_done;
            u32Tmp |= g_ECCERR_done;
        }
        while(u32Tmp == 0UL);

        Reg2Hex(pCurve->Echar, crpt->ECC_X1, secret_z);
    }

    return ret;
}


/**
  * @brief  Given a curve parameter, the other party's public key, and one's own private key to generate the secret Z.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  private_k   One's own private key.
  * @param[in]  public_k1   The other party's publick key 1.
  * @param[in]  public_k2   The other party's publick key 2.
  * @param[out] secret_z    The ECC CDH secret Z.
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  */
int32_t ECC_GenerateSecretZ_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, KS_MEM_Type mem, int32_t i32KeyIdx, char public_k1[], char public_k2[])
{
    int32_t  i;
    uint32_t u32Tmp;

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        return -1;
    }

    for(i = 0; i < 18; i++)
    {
        crpt->ECC_K[i] = 0UL;
        crpt->ECC_X1[i] = 0UL;
        crpt->ECC_Y1[i] = 0UL;
    }

    crpt->ECC_KSCTL = CRPT_ECC_KSCTL_ECDH_Msk | CRPT_ECC_KSCTL_RSRCK_Msk |
                      (uint32_t)(mem << CRPT_ECC_KSCTL_RSSRCK_Pos)/* KS Memory Type */ |
                      (uint32_t)i32KeyIdx;

    Hex2Reg(public_k1, crpt->ECC_X1);
    Hex2Reg(public_k2, crpt->ECC_Y1);

    /* set FSEL (Field selection) */
    if(pCurve->GF == (int)CURVE_GF_2M)
    {
        crpt->ECC_CTL = 0UL;
    }
    else /*  CURVE_GF_P */
    {
        crpt->ECC_CTL = CRPT_ECC_CTL_FSEL_Msk;
    }

    g_ECC_done = g_ECCERR_done = 0UL;

    crpt->ECC_CTL |= ((uint32_t)pCurve->key_len << CRPT_ECC_CTL_CURVEM_Pos) |
                     ECCOP_POINT_MUL | CRPT_ECC_CTL_START_Msk;

    do
    {
        u32Tmp = g_ECC_done;
        u32Tmp |= g_ECCERR_done;
    }
    while(u32Tmp == 0UL);

    if(g_ECCERR_done)
        return -1;

    return (crpt->ECC_KSSTS & 0x1f);

}


static void run_ecc_codec(CRPT_T *crpt, uint32_t mode)
{
    uint32_t u32Tmp;
    uint32_t eccop;

    eccop = mode & CRPT_ECC_CTL_ECCOP_Msk;
    if(eccop == ECCOP_MODULE)
    {
        crpt->ECC_CTL = CRPT_ECC_CTL_FSEL_Msk;
    }
    else
    {
        if(pCurve->GF == (int)CURVE_GF_2M)
        {
            /* point */
            crpt->ECC_CTL = 0UL;
        }
        else
        {
            /* CURVE_GF_P */
            crpt->ECC_CTL = CRPT_ECC_CTL_FSEL_Msk;
        }

#ifdef ECC_SCA_PROTECT
        if(eccop == ECCOP_POINT_MUL)
        {
            /* Enable side-channel protection in some operation */
            crpt->ECC_CTL |= CRPT_ECC_CTL_SCAP_Msk;
            /* If SCAP enabled, the curve order must be written to ECC_X2 */
            Hex2Reg(pCurve->Eorder, crpt->ECC_X2);
        }
#endif

    }

    g_ECC_done = g_ECCERR_done = 0UL;

    crpt->ECC_CTL |= ((uint32_t)pCurve->key_len << CRPT_ECC_CTL_CURVEM_Pos) | mode | CRPT_ECC_CTL_START_Msk;

    do
    {
        u32Tmp = g_ECC_done;
        u32Tmp |= g_ECCERR_done;
    }
    while(u32Tmp == 0UL);

    while(crpt->ECC_STS & CRPT_ECC_STS_BUSY_Msk) { }
}

/**
  * @brief  ECDSA digital signature generation.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  message     The hash value of source context.
  * @param[in]  d           The private key.
  * @param[in]  k           The selected random integer.
  * @param[out] R           R of the (R,S) pair digital signature
  * @param[out] S           S of the (R,S) pair digital signature
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  */
int32_t  ECC_GenerateSignature(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message,
                               char *d, char *k, char *R, char *S)
{
    uint32_t volatile temp_result1[18], temp_result2[18];
    int32_t  i, ret = 0;

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if(ret == 0)
    {
        CRPT->ECC_KSCTL = 0;

        /*
         *   1. Calculate e = HASH(m), where HASH is a cryptographic hashing algorithm, (i.e. SHA-1)
         *      (1) Use SHA to calculate e
         */

        /*   2. Select a random integer k form [1, n-1]
         *      (1) Notice that n is order, not prime modulus or irreducible polynomial function
         */

        /*
         *   3. Compute r = x1 (mod n), where (x1, y1) = k * G. If r = 0, go to step 2
         *      (1) Write the curve parameter A, B, and curve length M to corresponding registers
         *      (2) Write the prime modulus or irreducible polynomial function to N registers according
         *      (3) Write the point G(x, y) to X1, Y1 registers
         *      (4) Write the random integer k to K register
         *      (5) Set ECCOP(CRPT_ECC_CTL[10:9]) to 00
         *      (6) Set FSEL(CRPT_ECC_CTL[8]) according to used curve of prime field or binary field
         *      (7) Set START(CRPT_ECC_CTL[0]) to 1
         *      (8) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (9) Write the curve order and curve length to N ,M registers according
         *      (10) Write 0x0 to Y1 registers
         *      (11) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (12) Set MOPOP(CRPT_ECC_CTL[12:11]) to 10
         *      (13) Set START(CRPT_ECC_CTL[0]) to 1         *
         *      (14) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (15) Read X1 registers to get r
         */

        /* 3-(4) Write the random integer k to K register */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_K[i] = 0UL;
        }
        Hex2Reg(k, crpt->ECC_K);

        run_ecc_codec(crpt, ECCOP_POINT_MUL);

        /*  3-(9) Write the curve order to N registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /* 3-(10) Write 0x0 to Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = 0UL;
        }

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_ADD);

        /* 3-(15) Read X1 registers to get r */
        for(i = 0; i < 18; i++)
        {
            temp_result1[i] = crpt->ECC_X1[i];
        }

        Reg2Hex(pCurve->Echar, temp_result1, R);

        /*
         *   4. Compute s = k^-1 * (e + d * r)(mod n). If s = 0, go to step 2
         *      (1) Write the curve order to N registers according
         *      (2) Write 0x1 to Y1 registers
         *      (3) Write the random integer k to X1 registers according
         *      (4) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (5) Set MOPOP(CRPT_ECC_CTL[12:11]) to 00
         *      (6) Set START(CRPT_ECC_CTL[0]) to 1
         *      (7) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (8) Read X1 registers to get k^-1
         *      (9) Write the curve order and curve length to N ,M registers
         *      (10) Write r, d to X1, Y1 registers
         *      (11) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (12) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (13) Set START(CRPT_ECC_CTL[0]) to 1
         *      (14) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (15) Write the curve order to N registers
         *      (16) Write e to Y1 registers
         *      (17) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (18) Set MOPOP(CRPT_ECC_CTL[12:11]) to 10
         *      (19) Set START(CRPT_ECC_CTL[0]) to 1
         *      (20) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (21) Write the curve order and curve length to N ,M registers
         *      (22) Write k^-1 to Y1 registers
         *      (23) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (24) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (25) Set START(CRPT_ECC_CTL[0]) to 1
         *      (26) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (27) Read X1 registers to get s
         */

        /* S/W: GFp_add_mod_order(pCurve->key_len+2, 0, x1, a, R); */

        /*  4-(1) Write the curve order to N registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*  4-(2) Write 0x1 to Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = 0UL;
        }
        crpt->ECC_Y1[0] = 0x1UL;

        /*  4-(3) Write the random integer k to X1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = 0UL;
        }
        Hex2Reg(k, crpt->ECC_X1);

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_DIV);

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, crpt->ECC_X1, temp_hex_str);
        CRPT_DBGMSG("(7) output = %s\n", temp_hex_str);
#endif

        /*  4-(8) Read X1 registers to get k^-1 */

        for(i = 0; i < 18; i++)
        {
            temp_result2[i] = crpt->ECC_X1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_result2, temp_hex_str);
        CRPT_DBGMSG("k^-1 = %s\n", temp_hex_str);
#endif

        /*  4-(9) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*  4-(10) Write r, d to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = temp_result1[i];
        }

        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = 0UL;
        }
        Hex2Reg(d, crpt->ECC_Y1);

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_MUL);

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, crpt->ECC_X1, temp_hex_str);
        CRPT_DBGMSG("(14) output = %s\n", temp_hex_str);
#endif

        /*  4-(15) Write the curve order to N registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*  4-(16) Write e to Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = 0UL;
        }

        Hex2Reg(message, crpt->ECC_Y1);

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_ADD);

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, crpt->ECC_X1, temp_hex_str);
        CRPT_DBGMSG("(20) output = %s\n", temp_hex_str);
#endif

        /*  4-(21) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*  4-(22) Write k^-1 to Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = temp_result2[i];
        }

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_MUL);

        /*  4-(27) Read X1 registers to get s */
        for(i = 0; i < 18; i++)
        {
            temp_result2[i] = crpt->ECC_X1[i];
        }

        Reg2Hex(pCurve->Echar, temp_result2, S);

    }  /* ret == 0 */

    return ret;
}



/**
  * @brief  ECDSA digital signature generation.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  message     The hash value of source context.
  * @param[in]  d           The private key.
  * @param[in]  k           The selected random integer.
  * @param[out] R           R of the (R,S) pair digital signature
  * @param[out] S           S of the (R,S) pair digital signature
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  */
int32_t  ECC_GenerateSignature_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message,KS_MEM_Type mem_d, int32_t i32KeyIdx_d,KS_MEM_Type mem_k, int32_t i32KeyIdx_k, char *R, char *S)
{
    uint32_t volatile temp_result1[18], temp_result2[18];
    int32_t  i, ret = 0;

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if(ret == 0)
    {
        CRPT->ECC_KSCTL = 0;
        CRPT->ECC_KSXY  = 0;

        /*
         *   1. Calculate e = HASH(m), where HASH is a cryptographic hashing algorithm, (i.e. SHA-1)
         *      (1) Use SHA to calculate e
         */

        /*   2. Select a random integer k form [1, n-1]
         *      (1) Notice that n is order, not prime modulus or irreducible polynomial function
         */

        /*
         *   3. Compute r = x1 (mod n), where (x1, y1) = k * G. If r = 0, go to step 2
         *      (1) Write the curve parameter A, B, and curve length M to corresponding registers
         *      (2) Write the prime modulus or irreducible polynomial function to N registers according
         *      (3) Write the point G(x, y) to X1, Y1 registers
         *      (4) Write the random integer k to K register
         *      (5) Set ECCOP(CRPT_ECC_CTL[10:9]) to 00
         *      (6) Set FSEL(CRPT_ECC_CTL[8]) according to used curve of prime field or binary field
         *      (7) Set START(CRPT_ECC_CTL[0]) to 1
         *      (8) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (9) Write the curve order and curve length to N ,M registers according
         *      (10) Write 0x0 to Y1 registers
         *      (11) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (12) Set MOPOP(CRPT_ECC_CTL[12:11]) to 10
         *      (13) Set START(CRPT_ECC_CTL[0]) to 1         *
         *      (14) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (15) Read X1 registers to get r
         */

        /* 3-(4) Use k in Key Store */
        crpt->ECC_KSCTL = (uint32_t)(mem_k << CRPT_ECC_KSCTL_RSSRCK_Pos)/* KS Memory Type */ |
                          CRPT_ECC_KSCTL_RSRCK_Msk/* Key from KS */ |
                          (uint32_t)i32KeyIdx_k;

        run_ecc_codec(crpt, ECCOP_POINT_MUL | OP_ECDSAR);

        /*  3-(9) Write the curve order to N registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /* 3-(10) Write 0x0 to Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = 0UL;
        }

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_ADD);

        /* 3-(15) Read X1 registers to get r */
        for(i = 0; i < 18; i++)
        {
            temp_result1[i] = crpt->ECC_X1[i];
        }

        Reg2Hex(pCurve->Echar, temp_result1, R);


        /*
         *   4. Compute s = k ^-1 * (e + d * r)(mod n). If s = 0, go to step 2
         *      (1) Write the curve order to N registers according
         *      (2) Write 0x1 to Y1 registers
         *      (3) Write the random integer k to X1 registers according
         *      (4) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (5) Set MOPOP(CRPT_ECC_CTL[12:11]) to 00
         *      (6) Set START(CRPT_ECC_CTL[0]) to 1
         *      (7) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (8) Read X1 registers to get k^-1
         *      (9) Write the curve order and curve length to N ,M registers
         *      (10) Write r, d to X1, Y1 registers
         *      (11) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (12) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (13) Set START(CRPT_ECC_CTL[0]) to 1
         *      (14) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (15) Write the curve order to N registers
         *      (16) Write e to Y1 registers
         *      (17) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (18) Set MOPOP(CRPT_ECC_CTL[12:11]) to 10
         *      (19) Set START(CRPT_ECC_CTL[0]) to 1
         *      (20) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (21) Write the curve order and curve length to N ,M registers
         *      (22) Write k^-1 to Y1 registers
         *      (23) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (24) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (25) Set START(CRPT_ECC_CTL[0]) to 1
         *      (26) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (27) Read X1 registers to get s
         */

        /* S/W: GFp_add_mod_order(pCurve->key_len+2, 0, x1, a, R); */

        /*  4-(1) Write the curve order to N registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /* 4-(2)(3)(4)(5) Use d, k in Key Store */
        crpt->ECC_CTL = 0;
        crpt->ECC_KSXY = CRPT_ECC_KSXY_RSRCXY_Msk |
                         (uint32_t)(mem_k << CRPT_ECC_KSXY_RSSRCX_Pos) | ((uint32_t)i32KeyIdx_k << CRPT_ECC_KSXY_NUMX_Pos) | // Key Store index of k
                         (uint32_t)(mem_d << CRPT_ECC_KSXY_RSSRCY_Pos) | ((uint32_t)i32KeyIdx_d << CRPT_ECC_KSXY_NUMY_Pos);  // Key Store index of d

        // 4-5
        for(i=0; i<18; i++)
        {
            crpt->ECC_X2[i] = temp_result1[i];
            crpt->ECC_Y2[i] = 0;
        }
        Hex2Reg(message, crpt->ECC_Y2);

        run_ecc_codec(crpt, ECCOP_MODULE | OP_ECDSAS);

        /*  4-11 Read X1 registers to get s */
        for(i = 0; i < 18; i++)
        {
            temp_result2[i] = crpt->ECC_X1[i];
        }
        Reg2Hex(pCurve->Echar, temp_result2, S);

        /* Clear KS Control */
        CRPT->ECC_KSCTL = 0;
        CRPT->ECC_KSXY = 0;

    }  /* ret == 0 */

    return ret;
}


/**
  * @brief  ECDSA dogotal signature verification.
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  message     The hash value of source context.
  * @param[in]  public_k1   The public key 1.
  * @param[in]  public_k2   The public key 2.
  * @param[in]  R           R of the (R,S) pair digital signature
  * @param[in]  S           S of the (R,S) pair digital signature
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  * @return  -2   Verification failed.
  */
int32_t  ECC_VerifySignature(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message,
                             char *public_k1, char *public_k2, char *R, char *S)
{
    uint32_t  temp_result1[18], temp_result2[18];
    uint32_t  temp_x[18], temp_y[18];
    int32_t   i, ret = 0;

    /*
     *   1. Verify that r and s are integers in the interval [1, n-1]. If not, the signature is invalid
     *   2. Compute e = HASH (m), where HASH is the hashing algorithm in signature generation
     *      (1) Use SHA to calculate e
     */

    /*
     *   3. Compute w = s^-1 (mod n)
     *      (1) Write the curve order to N registers
     *      (2) Write 0x1 to Y1 registers
     *      (3) Write s to X1 registers
     *      (4) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
     *      (5) Set MOPOP(CRPT_ECC_CTL[12:11]) to 00
     *      (6) Set FSEL(CRPT_ECC_CTL[8]) according to used curve of prime field or binary field
     *      (7) Set START(CRPT_ECC_CTL[0]) to 1
     *      (8) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
     *      (9) Read X1 registers to get w
     */

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if(ret == 0)
    {

        /*  3-(1) Write the curve order to N registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*  3-(2) Write 0x1 to Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = 0UL;
        }
        crpt->ECC_Y1[0] = 0x1UL;

        /*  3-(3) Write s to X1 registers */
        for(i = 0; i < 18; i++)
        {
            CRPT->ECC_X1[i] = 0UL;
        }
        Hex2Reg(S, crpt->ECC_X1);

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_DIV);

        /*  3-(9) Read X1 registers to get w */
        for(i = 0; i < 18; i++)
        {
            temp_result2[i] = crpt->ECC_X1[i];
        }

#if ENABLE_DEBUG
        CRPT_DBGMSG("e = %s\n", message);
        Reg2Hex(pCurve->Echar, temp_result2, temp_hex_str);
        CRPT_DBGMSG("w = %s\n", temp_hex_str);
        CRPT_DBGMSG("o = %s (order)\n", pCurve->Eorder);
#endif

        /*
         *   4. Compute u1 = e * w (mod n) and u2 = r * w (mod n)
         *      (1) Write the curve order and curve length to N ,M registers
         *      (2) Write e, w to X1, Y1 registers
         *      (3) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (4) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (5) Set START(CRPT_ECC_CTL[0]) to 1
         *      (6) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (7) Read X1 registers to get u1
         *      (8) Write the curve order and curve length to N ,M registers
         *      (9) Write r, w to X1, Y1 registers
         *      (10) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (11) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (12) Set START(CRPT_ECC_CTL[0]) to 1
         *      (13) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (14) Read X1 registers to get u2
         */

        /*  4-(1) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /* 4-(2) Write e, w to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = 0UL;
        }
        Hex2Reg(message, crpt->ECC_X1);

        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = temp_result2[i];
        }

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_MUL);

        /*  4-(7) Read X1 registers to get u1 */
        for(i = 0; i < 18; i++)
        {
            temp_result1[i] = crpt->ECC_X1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_result1, temp_hex_str);
        CRPT_DBGMSG("u1 = %s\n", temp_hex_str);
#endif

        /*  4-(8) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /* 4-(9) Write r, w to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = 0UL;
        }
        Hex2Reg(R, crpt->ECC_X1);

        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = temp_result2[i];
        }

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_MUL);

        /*  4-(14) Read X1 registers to get u2 */
        for(i = 0; i < 18; i++)
        {
            temp_result2[i] = crpt->ECC_X1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_result2, temp_hex_str);
        CRPT_DBGMSG("u2 = %s\n", temp_hex_str);
#endif

        /*
         *   5. Compute X * (x1', y1') = u1 * G + u2 * Q
         *      (1) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *      (2) Write the point G(x, y) to X1, Y1 registers
         *      (3) Write u1 to K registers
         *      (4) Set ECCOP(CRPT_ECC_CTL[10:9]) to 00
         *      (5) Set START(CRPT_ECC_CTL[0]) to 1
         *      (6) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (7) Read X1, Y1 registers to get u1*G
         *      (8) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *      (9) Write the public key Q(x,y) to X1, Y1 registers
         *      (10) Write u2 to K registers
         *      (11) Set ECCOP(CRPT_ECC_CTL[10:9]) to 00
         *      (12) Set START(CRPT_ECC_CTL[0]) to 1
         *      (13) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (14) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *      (15) Write the result data u1*G to X2, Y2 registers
         *      (16) Set ECCOP(CRPT_ECC_CTL[10:9]) to 10
         *      (17) Set START(CRPT_ECC_CTL[0]) to 1
         *      (18) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (19) Read X1, Y1 registers to get X *(x1', y1')
         *      (20) Write the curve order and curve length to N ,M registers
         *      (21) Write x1 * to X1 registers
         *      (22) Write 0x0 to Y1 registers
         *      (23) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (24) Set MOPOP(CRPT_ECC_CTL[12:11]) to 10
         *      (25) Set START(CRPT_ECC_CTL[0]) to 1
         *      (26) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (27) Read X1 registers to get x1 * (mod n)
         *
         *   6. The signature is valid if x1 * = r, otherwise it is invalid
         */

        /*
         *  (1) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *  (2) Write the point G(x, y) to X1, Y1 registers
         */
        ecc_init_curve(crpt, ecc_curve);

        /* (3) Write u1 to K registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_K[i] = temp_result1[i];
        }

        run_ecc_codec(crpt, ECCOP_POINT_MUL);

        /* (7) Read X1, Y1 registers to get u1*G */
        for(i = 0; i < 18; i++)
        {
            temp_x[i] = crpt->ECC_X1[i];
            temp_y[i] = crpt->ECC_Y1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_x, temp_hex_str);
        CRPT_DBGMSG("5-(7) u1*G, x = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, temp_y, temp_hex_str);
        CRPT_DBGMSG("5-(7) u1*G, y = %s\n", temp_hex_str);
#endif

        /* (8) Write the curve parameter A, B, N, and curve length M to corresponding registers */
        ecc_init_curve(crpt, ecc_curve);

        /* (9) Write the public key Q(x,y) to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = 0UL;
            crpt->ECC_Y1[i] = 0UL;
        }

        Hex2Reg(public_k1, crpt->ECC_X1);
        Hex2Reg(public_k2, crpt->ECC_Y1);

        /* (10) Write u2 to K registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_K[i] = temp_result2[i];
        }

        run_ecc_codec(crpt, ECCOP_POINT_MUL);

        for(i = 0; i < 18; i++)
        {
            temp_result1[i] = crpt->ECC_X1[i];
            temp_result2[i] = crpt->ECC_Y1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_result1, temp_hex_str);
        CRPT_DBGMSG("5-(13) u2*Q, x = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, temp_result2, temp_hex_str);
        CRPT_DBGMSG("5-(13) u2*Q, y = %s\n", temp_hex_str);
#endif

        /* (14) Write the curve parameter A, B, N, and curve length M to corresponding registers */
        ecc_init_curve(crpt, ecc_curve);

        /* Write the result data u2*Q to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = temp_result1[i];
            crpt->ECC_Y1[i] = temp_result2[i];
        }

        /* (15) Write the result data u1*G to X2, Y2 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X2[i] = temp_x[i];
            crpt->ECC_Y2[i] = temp_y[i];
        }

        run_ecc_codec(crpt, ECCOP_POINT_ADD);

        /* (19) Read X1, Y1 registers to get X * (x1', y1') */
        for(i = 0; i < 18; i++)
        {
            temp_x[i] = crpt->ECC_X1[i];
            temp_y[i] = crpt->ECC_Y1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_x, temp_hex_str);
        CRPT_DBGMSG("5-(19) x' = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, temp_y, temp_hex_str);
        CRPT_DBGMSG("5-(19) y' = %s\n", temp_hex_str);
#endif

        /*  (20) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*
         *  (21) Write x1 * to X1 registers
         *  (22) Write 0x0 to Y1 registers
         */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = temp_x[i];
            crpt->ECC_Y1[i] = 0UL;
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, crpt->ECC_X1, temp_hex_str);
        CRPT_DBGMSG("5-(21) x' = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, crpt->ECC_Y1, temp_hex_str);
        CRPT_DBGMSG("5-(22) y' = %s\n", temp_hex_str);
#endif

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_ADD);

        /*  (27) Read X1 registers to get x1 * (mod n) */
        Reg2Hex(pCurve->Echar, crpt->ECC_X1, temp_hex_str);
        CRPT_DBGMSG("5-(27) x1' (mod n) = %s\n", temp_hex_str);

        /* 6. The signature is valid if x1 * = r, otherwise it is invalid */

        /* Compare with test pattern to check if r is correct or not */
        if(strcasecmp(temp_hex_str, R) != 0)
        {
            CRPT_DBGMSG("x1' (mod n) != R Test filed!!\n");
            CRPT_DBGMSG("Signature R [%s] is not matched with expected R [%s]!\n", temp_hex_str, R);
            ret = -2;
        }
    }  /* ret == 0 */

    return ret;
}



/**
  * @brief  ECDSA signature verification with Key Store
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  ecc_curve   The pre-defined ECC curve.
  * @param[in]  message     The hash value of source context.
  * @param[in]  public_k1   The public key 1.
  * @param[in]  public_k2   The public key 2.
  * @param[in]  R           R of the (R,S) pair digital signature
  * @param[in]  S           S of the (R,S) pair digital signature
  * @return  0    Success.
  * @return  -1   "ecc_curve" value is invalid.
  * @return  -2   Verification failed.
  */
int32_t  ECC_VerifySignature_KS(CRPT_T *crpt, E_ECC_CURVE ecc_curve, char *message,KS_MEM_Type mem_pk1, int32_t i32KeyIdx_pk1,KS_MEM_Type mem_pk2, int32_t i32KeyIdx_pk2, char *R, char *S)
{
    uint32_t  temp_result1[18], temp_result2[18];
    uint32_t  temp_x[18], temp_y[18];
    int32_t   i, ret = 0;

    /*
     *   1. Verify that r and s are integers in the interval [1, n-1]. If not, the signature is invalid
     *   2. Compute e = HASH (m), where HASH is the hashing algorithm in signature generation
     *      (1) Use SHA to calculate e
     */

    /*
     *   3. Compute w = s^-1 (mod n)
     *      (1) Write the curve order to N registers
     *      (2) Write 0x1 to Y1 registers
     *      (3) Write s to X1 registers
     *      (4) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
     *      (5) Set MOPOP(CRPT_ECC_CTL[12:11]) to 00
     *      (6) Set FSEL(CRPT_ECC_CTL[8]) according to used curve of prime field or binary field
     *      (7) Set START(CRPT_ECC_CTL[0]) to 1
     *      (8) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
     *      (9) Read X1 registers to get w
     */

    if(ecc_init_curve(crpt, ecc_curve) != 0)
    {
        ret = -1;
    }

    if(ret == 0)
    {
        crpt->ECC_KSCTL = 0;
        crpt->ECC_KSXY  = 0;

        /*  3-(1) Write the curve order to N registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*  3-(2) Write 0x1 to Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = 0UL;
        }
        crpt->ECC_Y1[0] = 0x1UL;

        /*  3-(3) Write s to X1 registers */
        for(i = 0; i < 18; i++)
        {
            CRPT->ECC_X1[i] = 0UL;
        }
        Hex2Reg(S, crpt->ECC_X1);

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_DIV);

        /*  3-(9) Read X1 registers to get w */
        for(i = 0; i < 18; i++)
        {
            temp_result2[i] = crpt->ECC_X1[i];
        }

#if ENABLE_DEBUG
        CRPT_DBGMSG("e = %s\n", message);
        Reg2Hex(pCurve->Echar, temp_result2, temp_hex_str);
        CRPT_DBGMSG("w = %s\n", temp_hex_str);
        CRPT_DBGMSG("o = %s (order)\n", pCurve->Eorder);
#endif

        /*
         *   4. Compute u1 = e  * w (mod n) and u2 = r  * w (mod n)
         *      (1) Write the curve order and curve length to N ,M registers
         *      (2) Write e, w to X1, Y1 registers
         *      (3) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (4) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (5) Set START(CRPT_ECC_CTL[0]) to 1
         *      (6) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (7) Read X1 registers to get u1
         *      (8) Write the curve order and curve length to N ,M registers
         *      (9) Write r, w to X1, Y1 registers
         *      (10) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (11) Set MOPOP(CRPT_ECC_CTL[12:11]) to 01
         *      (12) Set START(CRPT_ECC_CTL[0]) to 1
         *      (13) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (14) Read X1 registers to get u2
         */

        /*  4-(1) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /* 4-(2) Write e, w to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = 0UL;
        }
        Hex2Reg(message, crpt->ECC_X1);

        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = temp_result2[i];
        }

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_MUL);

        /*  4-(7) Read X1 registers to get u1 */
        for(i = 0; i < 18; i++)
        {
            temp_result1[i] = crpt->ECC_X1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_result1, temp_hex_str);
        CRPT_DBGMSG("u1 = %s\n", temp_hex_str);
#endif

        /*  4-(8) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /* 4-(9) Write r, w to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = 0UL;
        }
        Hex2Reg(R, crpt->ECC_X1);

        for(i = 0; i < 18; i++)
        {
            crpt->ECC_Y1[i] = temp_result2[i];
        }

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_MUL);

        /*  4-(14) Read X1 registers to get u2 */
        for(i = 0; i < 18; i++)
        {
            temp_result2[i] = crpt->ECC_X1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_result2, temp_hex_str);
        CRPT_DBGMSG("u2 = %s\n", temp_hex_str);
#endif

        /*
         *   5. Compute X * (x1', y1') = u1 * G + u2 * Q
         *      (1) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *      (2) Write the point G(x, y) to X1, Y1 registers
         *      (3) Write u1 to K registers
         *      (4) Set ECCOP(CRPT_ECC_CTL[10:9]) to 00
         *      (5) Set START(CRPT_ECC_CTL[0]) to 1
         *      (6) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (7) Read X1, Y1 registers to get u1*G
         *      (8) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *      (9) Write the public key Q(x,y) to X1, Y1 registers
         *      (10) Write u2 to K registers
         *      (11) Set ECCOP(CRPT_ECC_CTL[10:9]) to 00
         *      (12) Set START(CRPT_ECC_CTL[0]) to 1
         *      (13) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (14) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *      (15) Write the result data u1*G to X2, Y2 registers
         *      (16) Set ECCOP(CRPT_ECC_CTL[10:9]) to 10
         *      (17) Set START(CRPT_ECC_CTL[0]) to 1
         *      (18) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (19) Read X1, Y1 registers to get X * (x1', y1')
         *      (20) Write the curve order and curve length to N ,M registers
         *      (21) Write x1 * to X1 registers
         *      (22) Write 0x0 to Y1 registers
         *      (23) Set ECCOP(CRPT_ECC_CTL[10:9]) to 01
         *      (24) Set MOPOP(CRPT_ECC_CTL[12:11]) to 10
         *      (25) Set START(CRPT_ECC_CTL[0]) to 1
         *      (26) Wait for BUSY(CRPT_ECC_STS[0]) be cleared
         *      (27) Read X1 registers to get x1 * (mod n)
         *
         *   6. The signature is valid if x1 * = r, otherwise it is invalid
         */

        /*
         *  (1) Write the curve parameter A, B, N, and curve length M to corresponding registers
         *  (2) Write the point G(x, y) to X1, Y1 registers
         */
        ecc_init_curve(crpt, ecc_curve);

        /* (3) Write u1 to K registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_K[i] = temp_result1[i];
        }

        run_ecc_codec(crpt, ECCOP_POINT_MUL);

        /* (7) Read X1, Y1 registers to get u1*G */
        for(i = 0; i < 18; i++)
        {
            temp_x[i] = crpt->ECC_X1[i];
            temp_y[i] = crpt->ECC_Y1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_x, temp_hex_str);
        CRPT_DBGMSG("5-(7) u1*G, x = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, temp_y, temp_hex_str);
        CRPT_DBGMSG("5-(7) u1*G, y = %s\n", temp_hex_str);
#endif

        /* (8) Write the curve parameter A, B, N, and curve length M to corresponding registers */
        ecc_init_curve(crpt, ecc_curve);

        /* (9) Write the public key Q(x,y) to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = 0UL;
            crpt->ECC_Y1[i] = 0UL;
        }


#if 0
        Hex2Reg(public_k1, crpt->ECC_X1);
        Hex2Reg(public_k2, crpt->ECC_Y1);
#else

        /* 5-(2) Get the public key from key store */
        crpt->ECC_KSCTL = 0ul;
        crpt->ECC_KSXY = CRPT_ECC_KSXY_RSRCXY_Msk |
                         (uint32_t)(mem_pk1 << CRPT_ECC_KSXY_RSSRCX_Pos) | ((uint32_t)i32KeyIdx_pk1 << CRPT_ECC_KSXY_NUMX_Pos) | // Key Store index of pk1
                         (uint32_t)(mem_pk2 << CRPT_ECC_KSXY_RSSRCY_Pos) | ((uint32_t)i32KeyIdx_pk2 << CRPT_ECC_KSXY_NUMY_Pos);  // Key Store index of pk2

#endif

        /* (10) Write u2 to K registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_K[i] = temp_result2[i];
        }

        run_ecc_codec(crpt, ECCOP_POINT_MUL);

        for(i = 0; i < 18; i++)
        {
            temp_result1[i] = crpt->ECC_X1[i];
            temp_result2[i] = crpt->ECC_Y1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_result1, temp_hex_str);
        CRPT_DBGMSG("5-(13) u2*Q, x = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, temp_result2, temp_hex_str);
        CRPT_DBGMSG("5-(13) u2*Q, y = %s\n", temp_hex_str);
#endif

        /* (14) Write the curve parameter A, B, N, and curve length M to corresponding registers */
        ecc_init_curve(crpt, ecc_curve);

        /* Write the result data u2*Q to X1, Y1 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = temp_result1[i];
            crpt->ECC_Y1[i] = temp_result2[i];
        }

        /* (15) Write the result data u1*G to X2, Y2 registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X2[i] = temp_x[i];
            crpt->ECC_Y2[i] = temp_y[i];
        }

        run_ecc_codec(crpt, ECCOP_POINT_ADD);

        /* (19) Read X1, Y1 registers to get X * (x1', y1') */
        for(i = 0; i < 18; i++)
        {
            temp_x[i] = crpt->ECC_X1[i];
            temp_y[i] = crpt->ECC_Y1[i];
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, temp_x, temp_hex_str);
        CRPT_DBGMSG("5-(19) x' = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, temp_y, temp_hex_str);
        CRPT_DBGMSG("5-(19) y' = %s\n", temp_hex_str);
#endif

        /*  (20) Write the curve order and curve length to N ,M registers */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_N[i] = 0UL;
        }
        Hex2Reg(pCurve->Eorder, crpt->ECC_N);

        /*
         *  (21) Write x1 * to X1 registers
         *  (22) Write 0x0 to Y1 registers
         */
        for(i = 0; i < 18; i++)
        {
            crpt->ECC_X1[i] = temp_x[i];
            crpt->ECC_Y1[i] = 0UL;
        }

#if ENABLE_DEBUG
        Reg2Hex(pCurve->Echar, crpt->ECC_X1, temp_hex_str);
        CRPT_DBGMSG("5-(21) x' = %s\n", temp_hex_str);
        Reg2Hex(pCurve->Echar, crpt->ECC_Y1, temp_hex_str);
        CRPT_DBGMSG("5-(22) y' = %s\n", temp_hex_str);
#endif

        run_ecc_codec(crpt, ECCOP_MODULE | MODOP_ADD);

        /*  (27) Read X1 registers to get x1 * (mod n) */
        Reg2Hex(pCurve->Echar, crpt->ECC_X1, temp_hex_str);
        CRPT_DBGMSG("5-(27) x1' (mod n) = %s\n", temp_hex_str);

        /* 6. The signature is valid if x1 * = r, otherwise it is invalid */

        /* Compare with test pattern to check if r is correct or not */
        if(strcasecmp(temp_hex_str, R) != 0)
        {
            CRPT_DBGMSG("x1' (mod n) != R Test filed!!\n");
            CRPT_DBGMSG("Signature R [%s] is not matched with expected R [%s]!\n", temp_hex_str, R);
            ret = -2;
        }
    }  /* ret == 0 */

    return ret;
}


static ECC_CURVE * get_curve(E_ECC_CURVE ecc_curve)
{
    uint32_t   i;
    ECC_CURVE  *ret = NULL;

    for(i = 0UL; i < sizeof(_Curve) / sizeof(ECC_CURVE); i++)
    {
        if(ecc_curve == _Curve[i].curve_id)
        {
            memcpy((char *)&Curve_Copy, &_Curve[i], sizeof(ECC_CURVE));
            ret = &Curve_Copy;   /* (ECC_CURVE *)&_Curve[i]; */
        }
        if(ret != NULL)
        {
            break;
        }
    }
    return ret;
}


/**
  * @brief  ECC interrupt service routine. User application must invoke this function in
  *         his CRYPTO_IRQHandler() to let Crypto driver know ECC processing was done.
  * @param[in]  crpt        Reference to Crypto module.
  * @return   none
  */
void ECC_Complete(CRPT_T *crpt)
{
    if (crpt->INTSTS & CRPT_INTSTS_ECCIF_Msk)
    {
        g_ECC_done = 1UL;
        crpt->INTSTS = CRPT_INTSTS_ECCIF_Msk;
        /* printf("ECC done IRQ.\n"); */
    }

    if (crpt->INTSTS & CRPT_INTSTS_ECCEIF_Msk)
    {
        g_ECCERR_done = 1UL;
        crpt->INTSTS = CRPT_INTSTS_ECCEIF_Msk;
        printf("ECCEIF flag is set!!\n");
    }
}


int32_t ECC_GetCurve(CRPT_T *crpt, E_ECC_CURVE ecc_curve, ECC_CURVE *curve)
{
    int32_t err;

    /* Update pCurve pointer */
    err = ecc_init_curve(crpt, ecc_curve);
    if(err == 0)
    {
        /* get curve */
        memcpy(curve, pCurve, sizeof(ECC_CURVE));
    }

    return err;
}


/*-----------------------------------------------------------------------------------------------*/
/*                                                                                               */
/*    RSA                                                                                        */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

/** @cond HIDDEN_SYMBOLS */

static void *s_pRSABuf;
static uint32_t s_u32RsaOpMode;

typedef enum
{
    BUF_NORMAL,
    BUF_CRT,
    BUF_CRTBYPASS,
    BUF_SCAP,
    BUF_CRT_SCAP,
    BUF_CRTBYPASS_SCAP,
    BUF_KS
} E_RSA_BUF_SEL;

static int32_t CheckRsaBufferSize(uint32_t u32OpMode, uint32_t u32BufSize, uint32_t u32UseKS);

/** @endcond HIDDEN_SYMBOLS */

/* Check the allocated buffer size for RSA operation. */
static int32_t CheckRsaBufferSize(uint32_t u32OpMode, uint32_t u32BufSize, uint32_t u32UseKS)
{
    /* RSA buffer size for MODE_NORMAL, MODE_CRT, MODE_CRTBYPASS, MODE_SCAP, MODE_CRT_SCAP, MODE_CRTBYPASS_SCAP */
    uint32_t s_au32RsaBufSizeTbl[] = {sizeof(RSA_BUF_NORMAL_T), sizeof(RSA_BUF_CRT_T), sizeof(RSA_BUF_CRT_T), \
                                      sizeof(RSA_BUF_SCAP_T), sizeof(RSA_BUF_CRT_SCAP_T), sizeof(RSA_BUF_CRT_SCAP_T), \
                                      sizeof(RSA_BUF_KS_T)};

    if (u32UseKS)
    {
        if (u32BufSize != s_au32RsaBufSizeTbl[BUF_KS])
            return (-1);
    }
    else
    {
        switch(u32OpMode)
        {
            case RSA_MODE_NORMAL:
                if (u32BufSize != s_au32RsaBufSizeTbl[BUF_NORMAL])
                    return (-1);
                break;
            case RSA_MODE_CRT:
                if (u32BufSize != s_au32RsaBufSizeTbl[BUF_CRT])
                    return (-1);
                break;
            case RSA_MODE_CRTBYPASS:
                if (u32BufSize != s_au32RsaBufSizeTbl[BUF_CRTBYPASS])
                    return (-1);
                break;
            case RSA_MODE_SCAP:
                if (u32BufSize != s_au32RsaBufSizeTbl[BUF_SCAP])
                    return (-1);
                break;
            case RSA_MODE_CRT_SCAP:
                if (u32BufSize != s_au32RsaBufSizeTbl[BUF_CRT_SCAP])
                    return (-1);
                break;
            case RSA_MODE_CRTBYPASS_SCAP:
                if (u32BufSize != s_au32RsaBufSizeTbl[BUF_CRTBYPASS_SCAP])
                    return (-1);
                break;
            default:
                return (-1);
        }
    }

    return 0;
}

/**
  * @brief  Open RSA encrypt/decrypt function.
  * @param[in]  crpt         The pointer of CRYPTO module
  * @param[in]  u32OpMode    RSA operation mode, including:
  *         - \ref RSA_MODE_NORMAL
  *         - \ref RSA_MODE_CRT
  *         - \ref RSA_MODE_CRTBYPASS
  *         - \ref RSA_MODE_SCAP
  *         - \ref RSA_MODE_CRT_SCAP
  *         - \ref RSA_MODE_CRTBYPASS_SCAP
  * @param[in]  u32KeySize is RSA key size, including:
  *         - \ref RSA_KEY_SIZE_1024
  *         - \ref RSA_KEY_SIZE_2048
  *         - \ref RSA_KEY_SIZE_3072
  *         - \ref RSA_KEY_SIZE_4096
  * @param[in]  psRSA_Buf    The pointer of RSA buffer struct. User should declare correct RSA buffer for specific operation mode first.
  *         - \ref RSA_BUF_NORMAL_T      The struct for normal mode
  *         - \ref RSA_BUF_CRT_T         The struct for CRT ( + CRT bypass) mode
  *         - \ref RSA_BUF_SCAP_T        The struct for SCAP mode
  *         - \ref RSA_BUF_CRT_SCAP_T    The struct for CRT ( + CRT bypass) +SCAP mode
  *         - \ref RSA_BUF_KS_T          The struct for using key store
  * @param[in]  u32BufSize is RSA buffer size.
  * @param[in]  u32UseKS is use key store function.
  *         - \ref 0    No use key store function
  *         - \ref 1    Use key store function
  * @return  0    Success.
  * @return  -1   The value of pointer of RSA buffer struct is null.
  */
int32_t RSA_Open(CRPT_T *crpt, uint32_t u32OpMode, uint32_t u32KeySize, \
                 void *psRSA_Buf, uint32_t u32BufSize, uint32_t u32UseKS)
{
    if(psRSA_Buf == 0)
    {
        return (-1);
    }
    if (CheckRsaBufferSize(u32OpMode, u32BufSize, u32UseKS) != 0)
    {
        return (-1);
    }

    s_u32RsaOpMode = u32OpMode;
    s_pRSABuf = psRSA_Buf;
    crpt->RSA_CTL = (u32OpMode) | (u32KeySize << CRPT_RSA_CTL_KEYLENG_Pos);

    return 0;
}

/**
  * @brief  Set the RSA key
  * @param[in]  crpt        The pointer of CRYPTO module
  * @param[in]  Key         The private or public key.
  * @return  0    Success.
  * @return  -1   The value of pointer of RSA buffer struct is null.
  */
int32_t RSA_SetKey(CRPT_T *crpt, char *Key)
{
    if(s_pRSABuf == 0)
    {
        return (-1);
    }
    Hex2Reg(Key, ((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaE);
    crpt->RSA_SADDR[2] = (uint32_t)&((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaE; /* the public key or private key */

    return 0;
}

/**
  * @brief  Set RSA DMA transfer configuration.
  * @param[in]  crpt         The pointer of CRYPTO module
  * @param[in]  Src   RSA DMA source data
  * @param[in]  n     The modulus for both the public and private keys
  * @param[in]  P     The factor of modulus operation(P) for CRT/SCAP mode
  * @param[in]  Q     The factor of modulus operation(Q) for CRT/SCAP mode
  * @return  0    Success.
  * @return  -1   The value of pointer of RSA buffer struct is null.
  */
int32_t RSA_SetDMATransfer(CRPT_T *crpt, char *Src, char *n, char *P, char *Q)
{
    if(s_pRSABuf == 0)
    {
        return (-1);
    }
    Hex2Reg(Src, ((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaM);
    Hex2Reg(n, ((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaN);

    /* Assign the data to DMA */
    crpt->RSA_SADDR[0] = (uint32_t)&((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaM; /* plaintext / encrypt data */
    crpt->RSA_SADDR[1] = (uint32_t)&((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaN; /* the base of modulus operation */
    crpt->RSA_DADDR    = (uint32_t)&((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaOutput; /* encrypt data / decrypt data */

    if ((s_u32RsaOpMode & CRPT_RSA_CTL_CRT_Msk) && (s_u32RsaOpMode & CRPT_RSA_CTL_SCAP_Msk))
    {
        /* For RSA CRT/SCAP mode, two primes of private key */
        Hex2Reg(P, ((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaP);
        Hex2Reg(Q, ((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaQ);

        crpt->RSA_SADDR[3] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaP; /* prime P */
        crpt->RSA_SADDR[4] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaQ; /* prime Q */

        crpt->RSA_MADDR[0] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaTmpCp; /* for storing the intermediate temporary value(Cp) */
        crpt->RSA_MADDR[1] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaTmpCq; /* for storing the intermediate temporary value(Cq) */
        crpt->RSA_MADDR[2] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaTmpDp; /* for storing the intermediate temporary value(Dp) */
        crpt->RSA_MADDR[3] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaTmpDq; /* for storing the intermediate temporary value(Dq) */
        crpt->RSA_MADDR[4] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaTmpRp; /* for storing the intermediate temporary value(Rp) */
        crpt->RSA_MADDR[5] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaTmpRq; /* for storing the intermediate temporary value(Rq) */

        /* For SCAP mode to store the intermediate temporary value(blind key) */
        crpt->RSA_MADDR[6] = (uint32_t)&((RSA_BUF_CRT_SCAP_T *)s_pRSABuf)->au32RsaTmpBlindKey;
    }
    else if (s_u32RsaOpMode & CRPT_RSA_CTL_CRT_Msk)
    {
        /* For RSA CRT/SCAP mode, two primes of private key */
        Hex2Reg(P, ((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaP);
        Hex2Reg(Q, ((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaQ);

        crpt->RSA_SADDR[3] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaP; /* prime P */
        crpt->RSA_SADDR[4] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaQ; /* prime Q */

        crpt->RSA_MADDR[0] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaTmpCp; /* for storing the intermediate temporary value(Cp) */
        crpt->RSA_MADDR[1] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaTmpCq; /* for storing the intermediate temporary value(Cq) */
        crpt->RSA_MADDR[2] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaTmpDp; /* for storing the intermediate temporary value(Dp) */
        crpt->RSA_MADDR[3] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaTmpDq; /* for storing the intermediate temporary value(Dq) */
        crpt->RSA_MADDR[4] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaTmpRp; /* for storing the intermediate temporary value(Rp) */
        crpt->RSA_MADDR[5] = (uint32_t)&((RSA_BUF_CRT_T *)s_pRSABuf)->au32RsaTmpRq; /* for storing the intermediate temporary value(Rq) */
    }
    else if (s_u32RsaOpMode & CRPT_RSA_CTL_SCAP_Msk)
    {
        /* For RSA CRT/SCAP mode, two primes of private key */
        Hex2Reg(P, ((RSA_BUF_SCAP_T *)s_pRSABuf)->au32RsaP);
        Hex2Reg(Q, ((RSA_BUF_SCAP_T *)s_pRSABuf)->au32RsaQ);

        crpt->RSA_SADDR[3] = (uint32_t)&((RSA_BUF_SCAP_T *)s_pRSABuf)->au32RsaP; /* prime P */
        crpt->RSA_SADDR[4] = (uint32_t)&((RSA_BUF_SCAP_T *)s_pRSABuf)->au32RsaQ; /* prime Q */

        /* For SCAP mode to store the intermediate temporary value(blind key) */
        crpt->RSA_MADDR[6] = (uint32_t)&((RSA_BUF_SCAP_T *)s_pRSABuf)->au32RsaTmpBlindKey;
    }

    return 0;
}

/**
  * @brief  Start RSA encrypt/decrypt
  * @param[in]  crpt        The pointer of CRYPTO module
  * @return None
  */
void RSA_Start(CRPT_T *crpt)
{
    crpt->RSA_CTL |= CRPT_RSA_CTL_START_Msk;
}

/**
  * @brief  Read the RSA output.
  * @param[in]   crpt       The pointer of CRYPTO module
  * @param[out]  Output     The RSA operation output data.
  * @return  0    Success.
  * @return  -1   The value of pointer of RSA buffer struct is null.
  */
int32_t RSA_Read(CRPT_T *crpt, char *Output)
{
    if(s_pRSABuf == 0)
    {
        return (-1);
    }
    uint32_t au32CntTbl[4] = {256, 512, 768, 1024}; /* count is key length divided by 4 */
    uint32_t u32CntIdx = 0;

    u32CntIdx = (crpt->RSA_CTL & CRPT_RSA_CTL_KEYLENG_Msk) >> CRPT_RSA_CTL_KEYLENG_Pos;
    Reg2Hex((int32_t)au32CntTbl[u32CntIdx], ((RSA_BUF_NORMAL_T *)s_pRSABuf)->au32RsaOutput, Output);

    return 0;
}

/**
  * @brief  Set the RSA key is read from key store
  * @param[in]  crpt           The pointer of CRYPTO module
  * @param[in]  u32KeyNum      The number of private or public key in key store.
  * @param[in]  u32KSMemType   The key is read from selected memory type of key store. It could be:
                            \ref KS_SRAM
                            \ref KS_FLASH
                            \ref KS_OTP
  * @param[in]  u32BlindKeyNum  The number of blind key in SRAM of key store for SCAP mode. This key is un-readable.
  * @return  0    Success.
  * @return  -1   The value of pointer of RSA buffer struct is null.
  */
int32_t RSA_SetKey_KS(CRPT_T *crpt, uint32_t u32KeyNum, uint32_t u32KSMemType, uint32_t u32BlindKeyNum)
{
    if (s_u32RsaOpMode & CRPT_RSA_CTL_SCAP_Msk)
    {
        crpt->RSA_KSCTL = (u32BlindKeyNum << 8) | (u32KSMemType << CRPT_RSA_KSCTL_RSSRC_Pos) | CRPT_RSA_KSCTL_RSRC_Msk | u32KeyNum;
    }
    else
    {
        crpt->RSA_KSCTL = (u32KSMemType << CRPT_RSA_KSCTL_RSSRC_Pos) | CRPT_RSA_KSCTL_RSRC_Msk | u32KeyNum;
    }
    return 0;
}

/**
  * @brief  Set RSA DMA transfer configuration while using key store.
  * @param[in]  crpt         The pointer of CRYPTO module
  * @param[in]  u32OpMode    RSA operation mode, including:
  *         - \ref RSA_MODE_NORMAL
  *         - \ref RSA_MODE_CRT
  *         - \ref RSA_MODE_CRTBYPASS
  *         - \ref RSA_MODE_SCAP
  *         - \ref RSA_MODE_CRT_SCAP
  *         - \ref RSA_MODE_CRTBYPASS_SCAP
  * @param[in]  Src   RSA DMA source data
  * @param[in]  n     The modulus for both the public and private keys
  * @param[in]  u32PNum         The number of the factor of modulus operation(P) in SRAM of key store for CRT/SCAP mode
  * @param[in]  u32QNum         The number of the factor of modulus operation(Q) in SRAM of key store for CRT/SCAP mode
  * @param[in]  u32CpNum        The number of Cp in SRAM of key store for CRT mode
  * @param[in]  u32CqNum        The number of Cq in SRAM of key store for CRT mode
  * @param[in]  u32DpNum        The number of Dp in SRAM of key store for CRT mode
  * @param[in]  u32DqNum        The number of Dq in SRAM of key store for CRT mode
  * @param[in]  u32RpNum        The number of Rp in SRAM of key store for CRT mode
  * @param[in]  u32RqNum        The number of Rq in SRAM of key store for CRT mode
  * @return  0    Success.
  * @return  -1   The value of pointer of RSA buffer struct is null.
  * @note P, Q, Dp, Dq are equal to half key length. Cp, Cq, Rp, Rq, Blind key are equal to key length.
  */
int32_t RSA_SetDMATransfer_KS(CRPT_T *crpt, char *Src, char *n, uint32_t u32PNum,
                                      uint32_t u32QNum, uint32_t u32CpNum, uint32_t u32CqNum, uint32_t u32DpNum,
                                      uint32_t u32DqNum, uint32_t u32RpNum, uint32_t u32RqNum)
{
    if(s_pRSABuf == 0)
    {
        return (-1);
    }
    Hex2Reg(Src, ((RSA_BUF_KS_T *)s_pRSABuf)->au32RsaM);
    Hex2Reg(n, ((RSA_BUF_KS_T *)s_pRSABuf)->au32RsaN);

    /* Assign the data to DMA */
    crpt->RSA_SADDR[0] = (uint32_t)&((RSA_BUF_KS_T *)s_pRSABuf)->au32RsaM; /* plaintext / encrypt data */
    crpt->RSA_SADDR[1] = (uint32_t)&((RSA_BUF_KS_T *)s_pRSABuf)->au32RsaN; /* the base of modulus operation */
    crpt->RSA_DADDR    = (uint32_t)&((RSA_BUF_KS_T *)s_pRSABuf)->au32RsaOutput; /* encrypt data / decrypt data */

    if ((s_u32RsaOpMode & CRPT_RSA_CTL_CRT_Msk) || (s_u32RsaOpMode & CRPT_RSA_CTL_SCAP_Msk))
    {
        /* For RSA CRT/SCAP mode, two primes of private key */
        crpt->RSA_KSSTS[0] = (crpt->RSA_KSSTS[0] & (~(CRPT_RSA_KSSTS0_NUM0_Msk | CRPT_RSA_KSSTS0_NUM1_Msk))) | \
                             (u32PNum << CRPT_RSA_KSSTS0_NUM0_Pos) | (u32QNum << CRPT_RSA_KSSTS0_NUM1_Pos);

    }
    if (s_u32RsaOpMode & CRPT_RSA_CTL_CRT_Msk)
    {
        /* For RSA CRT mode, Cp, Cq, Dp, Dq, Rp, Rq */
        crpt->RSA_KSSTS[0] = (crpt->RSA_KSSTS[0] & (~(CRPT_RSA_KSSTS0_NUM2_Msk | CRPT_RSA_KSSTS0_NUM3_Msk))) | \
                             (u32CpNum << CRPT_RSA_KSSTS0_NUM2_Pos) | (u32CqNum << CRPT_RSA_KSSTS0_NUM3_Pos);
        crpt->RSA_KSSTS[1] = (u32DpNum << CRPT_RSA_KSSTS1_NUM4_Pos) | (u32DqNum << CRPT_RSA_KSSTS1_NUM5_Pos) | \
                             (u32RpNum << CRPT_RSA_KSSTS1_NUM6_Pos) | (u32RqNum << CRPT_RSA_KSSTS1_NUM7_Pos);
    }

    return 0;
}


/**@}*/ /* end of group CRYPTO_EXPORTED_FUNCTIONS */

/**@}*/ /* end of group CRYPTO_Driver */

/**@}*/ /* end of group Standard_Driver */
