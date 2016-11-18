/*
 *  Elliptic curves over GF(p): CRYPTO hw acceleration functions
 *
 *  Copyright (C) 2015-2016 Silicon Labs, http://www.silabs.com
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*
 * This file includes alternative plugin implementations of various
 * functions in ecp.c using the CRYPTO hardware accelerator incorporated
 * in MCU devices from Silicon Laboratories.
 */
/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 * GECC = Guide to Elliptic Curve Cryptography - Hankerson, Menezes, Vanstone
 * FIPS 186-3 http://csrc.nist.gov/publications/fips/fips186-3/fips_186-3.pdf
 * RFC 4492 for the related TLS structures and constants
 *
 * [Curve25519] http://cr.yp.to/ecdh/curve25519-20060209.pdf
 *
 * [2] CORON, Jean-S'ebastien. Resistance against differential power analysis
 *     for elliptic curve cryptosystems. In : Cryptographic Hardware and
 *     Embedded Systems. Springer Berlin Heidelberg, 1999. p. 292-302.
 *     <http://link.springer.com/chapter/10.1007/3-540-48059-5_25>
 *
 * [3] HEDABOU, Mustapha, PINEL, Pierre, et B'EN'ETEAU, Lucien. A comb method to
 *     render ECC resistant against Side Channel Attacks. IACR Cryptology
 *     ePrint Archive, 2004, vol. 2004, p. 342.
 *     <http://eprint.iacr.org/2004/342.pdf>
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined( MBEDTLS_ECP_C )

#if defined( MBEDTLS_ECP_DEVICE_ALT )

#if !defined( MBEDTLS_SLCL_PLUGINS )

#include "em_device.h"

#if defined( CRYPTO_COUNT ) && ( CRYPTO_COUNT > 0 )

#include "em_crypto.h"

#include "mbedtls/ecp.h"

#include <stdbool.h>
#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#include <stdio.h>
#define mbedtls_printf     printf
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#define CRYPTO_ENABLE CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_CRYPTO; \
  CRYPTO->IFC = _CRYPTO_IFC_MASK; \
  CRYPTO->CMD = CRYPTO_CMD_SEQSTOP; \
  CRYPTO->CTRL = CRYPTO_CTRL_DMA0RSEL_DDATA0; \
  CRYPTO->SEQCTRL = 0; \
  CRYPTO->SEQCTRLB = 0

#define CRYPTO_DISABLE \
  CRYPTO->IEN = 0; \
  CMU->HFBUSCLKEN0 &= ~CMU_HFBUSCLKEN0_CRYPTO;

/** ECC big integer type. */
#define ECC_BIGINT_SIZE_IN_BITS        (256)
#define ECC_BIGINT_SIZE_IN_BYTES       (ECC_BIGINT_SIZE_IN_BITS/8)
#define ECC_BIGINT_SIZE_IN_32BIT_WORDS (ECC_BIGINT_SIZE_IN_BYTES/sizeof(uint32_t))
#define EC_BIGINT_COPY(X, Y) memcpy(X, Y, sizeof(ecc_bigint_t));
typedef uint32_t ecc_bigint_t[ECC_BIGINT_SIZE_IN_32BIT_WORDS];

#if defined(MBEDTLS_MPI_MODULAR_DIVISION_ALT)
#define MPI_TO_BIGINT(bigint, mpi) mpitobigint(bigint, mpi);

/***************************************************************************//**
 * @brief
 *   Convert an mpi number representation to a 32bit word array used by crypto.
 ******************************************************************************/
__STATIC_INLINE void mpitobigint( ecc_bigint_t bigint, const mbedtls_mpi* mpi )
{
    uint32_t* bi = bigint;
    
    if ( mpi->n < 8 )
    {
      memcpy(bigint, mpi->p, mpi->n * sizeof(uint32_t));
      memset(&bi[mpi->n], 0, sizeof(ecc_bigint_t) - mpi->n * sizeof(uint32_t));
    }
    else
    {
      memcpy(bigint, mpi->p, 8 * sizeof(uint32_t));
    }
}

/***************************************************************************//**
 * @brief
 *   Returns true if the value of the DDATA0 register is equal to zero.
 ******************************************************************************/
__STATIC_INLINE bool crypto_ddata0_is_zero(uint32_t* status_reg)
{
  CRYPTO_EXECUTE_3(CRYPTO,
                   CRYPTO_CMD_INSTR_CCLR,
                   CRYPTO_CMD_INSTR_DEC,  /* Decrement by one which will set
                                             carry bit if DDATA0 is zero. */
                   CRYPTO_CMD_INSTR_INC   /* Increment in order to restore
                                             original value. */
                   );

  *status_reg = CRYPTO->DSTATUS;

  return (*status_reg & CRYPTO_DSTATUS_CARRY) == CRYPTO_DSTATUS_CARRY;
}
#endif

/***************************************************************************//**
 * @brief
 *   Check if CRYPTO supports acceleration of given ecc curve.
 ******************************************************************************/
bool mbedtls_ecp_device_grp_capable( const mbedtls_ecp_group *grp )
{
    switch( grp->id )
    {
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP192R1:
#if defined( MBEDTLS_MPI_MUL_MPI_ALT ) || defined( MBEDTLS_MPI_MUL_INT_ALT )
            /* Need to reset operand width and result width if CRYPTO has been
               used for MPI acceleration since initialization. */
            CRYPTO_MulOperandWidthSet( CRYPTO, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( CRYPTO, cryptoResult256Bits );
#endif
            return( true );
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP224R1:
#if defined( MBEDTLS_MPI_MUL_MPI_ALT ) || defined( MBEDTLS_MPI_MUL_INT_ALT )
            /* Need to reset operand width and result width if CRYPTO has been
               used for MPI acceleration since initialization. */
            CRYPTO_MulOperandWidthSet( CRYPTO, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( CRYPTO, cryptoResult256Bits );
#endif
            return( true );
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP256R1:
#if defined( MBEDTLS_MPI_MUL_MPI_ALT ) || defined( MBEDTLS_MPI_MUL_INT_ALT )
            /* Need to reset operand width and result width if CRYPTO has been
               used for MPI acceleration since initialization. */
            CRYPTO_MulOperandWidthSet( CRYPTO, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( CRYPTO, cryptoResult260Bits );
#endif
            return( true );
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */

        default:
            return( false );
    }
}

/***************************************************************************//**
 * @brief
 *   Enable CRYPTO by setting up control registers for given ecc curve.
 ******************************************************************************/
int mbedtls_ecp_device_init( const mbedtls_ecp_group *grp )
{
    /* CRYPTO specific setup */
    CRYPTO_ENABLE;
    
    CRYPTO->SEQCTRL = CRYPTO_SEQCTRL_BLOCKSIZE_32BYTES | 32;

    switch( grp->id )
    {
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP192R1:
            CRYPTO_ModulusSet( CRYPTO, cryptoModulusEccP192 );
            CRYPTO_MulOperandWidthSet( CRYPTO, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( CRYPTO, cryptoResult256Bits );
            return 0;
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP224R1:
            CRYPTO_ModulusSet( CRYPTO, cryptoModulusEccP224 );
            CRYPTO_MulOperandWidthSet( CRYPTO, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( CRYPTO, cryptoResult256Bits );
            return 0;
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP256R1:
            CRYPTO_ModulusSet( CRYPTO, cryptoModulusEccP256 );
            CRYPTO_MulOperandWidthSet( CRYPTO, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( CRYPTO, cryptoResult260Bits );
            return 0;
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */

        default:
            return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );
    }
}

/***************************************************************************//**
 * @brief
 *   Disable CRYPTO by turning off clock (unless needed by mpi apis separately.)
 ******************************************************************************/
int mbedtls_ecp_device_deinit( const mbedtls_ecp_group *grp )
{
    (void) grp;
#if !defined( MBEDTLS_MPI_MUL_MPI_ALT ) && \
    !defined( MBEDTLS_MPI_MUL_INT_ALT )
    CRYPTO_DISABLE;
#endif
    return 0;
}

/***************************************************************************//**
 * @brief
 *   Write 256 bits of data to a DDATAX register in the CRYPTO module.
 *
 * @details
 *   Write 256 bits of data into a DDATAX (Double Data) register in the crypto
 *   module.
 *
 *  @param[in]  ddataReg   Data register identifier
 *  @param[in]  val        Value of the data to write to the DDATA register.
 ******************************************************************************/
__STATIC_INLINE void ecp_crypto_ddata_write(CRYPTO_DDataReg_TypeDef  ddataReg,
                                            const mbedtls_mpi*       mpi)
{
  uint32_t volatile* regPtr = (volatile uint32_t *) ddataReg;
  uint32_t* pVal = mpi->p;
  register uint32_t v0;
  register uint32_t v1;
  register uint32_t v2;
  register uint32_t v3;
  int      i;
  
  if (mpi->n <4)
  {
    /* Non optimal write of data. */
    for (i=0; i<(int)mpi->n; i++)
      *regPtr = *pVal++;
    for (; i<8; i++)
      *regPtr = 0;
  }
  else
  {
    if (mpi->n < 8)
    {
      /* Optimal write of first 4 words. */
      v0 = *pVal++;
      v1 = *pVal++;
      v2 = *pVal++;
      v3 = *pVal++;
      *regPtr = v0;
      *regPtr = v1;
      *regPtr = v2;
      *regPtr = v3;

      /* Non optimal write of remaining words */
      for (i=4; i<(int)mpi->n; i++)
        *regPtr = *pVal++;
      for (; i<8; i++)
        *regPtr = 0;
    }
    else
    {
      /* Optimal write of all data. */
      v0 = *pVal++;
      v1 = *pVal++;
      v2 = *pVal++;
      v3 = *pVal++;
      *regPtr = v0;
      *regPtr = v1;
      *regPtr = v2;
      *regPtr = v3;
      
      v0 = *pVal++;
      v1 = *pVal++;
      v2 = *pVal++;
      v3 = *pVal++;
      *regPtr = v0;
      *regPtr = v1;
      *regPtr = v2;
      *regPtr = v3;
    }
  }
}

/***************************************************************************//**
 * @brief
 *   Read 256 bits of data from a DDATAX register in the CRYPTO module.
 *
 * @details
 *   Read 256 bits of data from a DDATAX (Double Data) register in the crypto
 *   module.
 *
 *  @param[in]  ddataReg   Data register identifier
 *  @param[out] val        Location where to store the value in memory.
 ******************************************************************************/

__STATIC_INLINE int ecp_crypto_ddata_read(CRYPTO_DDataReg_TypeDef  ddataReg,
                                          mbedtls_mpi*             mpi)
{
  CRYPTO_DData_TypeDef  ddata;
  uint32_t              val32;
  int                   i;
  int                   used;
  int                   ret = 0;

  if (mpi->n == 8)
  {
    CRYPTO_DDataRead(ddataReg, mpi->p);
  }
  else
  {
    if (mpi->n > 8)
    {
      CRYPTO_DDataRead(ddataReg, mpi->p);
      memset(&mpi->p[8], 0, sizeof(uint32_t)*(mpi->n-8));
    }
    else
    {
      uint32_t volatile* regPtr = (volatile uint32_t*) ddataReg;
      used = 0;
      for (i=0; i<8; i++)
      {
        ddata[i] = val32 = *regPtr;
        if (val32)
          used = i+1;
      }
      if (used > (int)mpi->n)
      {
        MBEDTLS_MPI_CHK( mbedtls_mpi_grow(mpi, used) );
        memcpy(mpi->p, ddata, used*sizeof(uint32_t));
        mpi->s = 1;
      }
      else
      {
        memcpy(mpi->p, ddata, mpi->n*sizeof(uint32_t));
      }
    }
  }
 cleanup:
  return( ret );
}

#if defined(MBEDTLS_ECP_DOUBLE_JAC_ALT)
/*
 * Alternative implementation of ecp_double_jac using CRYPTO hardware
 * acceleration.
 *
 * Point doubling R = 2 P, Jacobian coordinates
 *
 * Based on Section 3.2.4 in "Introduction to Identity-Based Encryption"
 * by Martin Luther
 *
 * Cost: 1D := 4M + 4S          (A == -3)
 *             
 */
int ecp_device_double_jac( const mbedtls_ecp_group *grp,
                                 mbedtls_ecp_point *R,
                           const mbedtls_ecp_point *P )
{
  ecc_bigint_t _2YY;
  int          ret;
  (void) grp;
  
  /*       
  STEP 1:
           
  Goals:
  ZZ   = Z^2
  R->Z = 2 * Y * Z
  YY   = Y^2
  4YY  = 4 * Y^2
  
  Write Operations:
  
  R2 = Y
  R3 = Z
  
  Instructions to be executed:
  
  1.  R0 = DMA = Z
  2.  R1 = R0 = Z
  3.  R2 = R0 = Z
  4.  Select R1, R2
  5.  R0 = R1 * R2 = Z^2 = ZZ 
  6.  R3 = R0 = ZZ

  7.  R0 = DMA = Y
  8.  R2 = R0 = Y
  9.  R0 = R1 * R2 = Y * Z
  10. Select R0, R0
  11. R0 = R0 + R0 = 2 * Y * Z = R->Z

  12. DMA = R0 = R->Z

  13. R1 = R2 = Y
  14. Select R1, R2
  15. R0 = R1 * R2 = Y^2 = YY
  16.  Select R0, R0
  17.  R0 = R0 + R0 = 2YY
  
  Read Operations:
  
  R->Z = R0 = 2 * Y * Z
  2YY  = R0

  Output State:
  R0 = 2YY
  R1 = FREE
  R2 = FREE
  R3 = ZZ
  R4 = FREE

  STEP 1:
  */
  CRYPTO_EXECUTE_17(CRYPTO,
                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3,

                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                    CRYPTO_CMD_INSTR_MADD,
                    CRYPTO_CMD_INSTR_DATATODMA0,

                    CRYPTO_CMD_INSTR_DDATA2TODDATA1,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                    CRYPTO_CMD_INSTR_MADD
                    );
  ecp_crypto_ddata_write(&CRYPTO->DDATA0, &P->Z);
  ecp_crypto_ddata_write(&CRYPTO->DDATA0, &P->Y);
  MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&CRYPTO->DDATA0, &R->Z) );
  ecp_crypto_ddata_write(&CRYPTO->DDATA4, &P->X);
  CRYPTO_DDataRead(&CRYPTO->DDATA0, _2YY);

  /*
  STEP 2:
    
  Goals:
  A   = 4YY * X
  C   = 3(X - ZZ)(X + ZZ)

  Write Operations:
  
  R4 = X

  Input State:
  R0 = 2YY
  R1 = FREE
  R2 = FREE
  R3 = ZZ
  R4 = X

  Instructions to be executed:
  
  1.  R0 = R0 + R0 = 4YY
  2.  R1 = R0 = 4YY
  3.  Select R1, R4
  4.  R0 = R1 * R4 = 4YY * X = A
  5.  R2 = R0 = A
  6.  Select R4, R3
  7.  R0 = R4 + R3 = X + ZZ
  8.  R1 = R0 = X + ZZ
  9.  R0 = R4 - R3 = X - ZZ
  0.  R2 = R0 = X - ZZ
  11. Select R1, R2
  12. R0 = R1 * R2 = (X + ZZ)(X - ZZ)
  13. R1 = R0 = (X + ZZ)(X - ZZ)
  14. Select R0, R1
  15. R0 = R0 + R1 = 2(X + ZZ)(X - ZZ)
  16. R0 = R0 + R1 = 3(X + ZZ)(X - ZZ) = C
  17. R1 = R0 = C 
  
  Output State:
  R0 = FREE
  R1 = C
  R2 = A
  R3 = FREE
  R4 = FREE

  STEP 2:
  */

  CRYPTO_EXECUTE_17(CRYPTO,
                    CRYPTO_CMD_INSTR_MADD,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    CRYPTO_CMD_INSTR_SELDDATA4DDATA3,
                    CRYPTO_CMD_INSTR_MADD,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA1,
                    CRYPTO_CMD_INSTR_MADD,
                    CRYPTO_CMD_INSTR_MADD,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1
                    );
  /*       
  STEP 3:
           
  Goals:
  R->X = C^2 - 2A
  D = C(A - R->X)
  
  Input State:
  R0 = FREE
  R1 = C
  R2 = A
  R3 = FREE
  R4 = FREE
  
  Instructions to be executed:
  
  1.  R4 = R1 = C
  2.  Select R1, R4
  3.  R0 = R1 * R4 = C^2
  4.  Select R0, R2
  5.  R0 = R0 - R2 = C^2 - 2A = R->X
  6.  R4 = R0 = R->X
  7.  Select R3, R4
  8.  R0 = R3 - R4 = A - R->X
  9.  R2 = R0 = A - R->X
  10  Select R1, R2
  11. R0 = R1 * R2 = C(A - R->X) = D
  
  Read Operations:     

  R->X = R4 = C^2 - 2A
  
  Output State:
  R0   = D
  R1   = FREE
  R2   = FREE
  R3   = FREE
  R4   = FREE

  STEP 3:
  */
  CRYPTO_EXECUTE_15(CRYPTO,
                    CRYPTO_CMD_INSTR_SELDDATA2DDATA2,
                    CRYPTO_CMD_INSTR_MADD,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                      
                    CRYPTO_CMD_INSTR_DDATA1TODDATA3,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                    CRYPTO_CMD_INSTR_MMUL,
                    
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA4,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_DATATODMA0,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                    
                    CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                    CRYPTO_CMD_INSTR_MMUL
                    );
  MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&CRYPTO->DDATA0, &R->X) );

  /*
  STEP 4:
           
  Goals:
  B    = 8 * Y^4
  R->Y = D - B
  
  Write Operations:
  
  R1 = YY

  Input State:
  R0   = D
  R1   = YY
  R2   = FREE
  R3   = FREE
  R4   = FREE
  
  Instructions to be executed:

  1. R3 = R0 = D
  2. R0 = DMA0
  3. R1 = R0 = Y^2
  4. R2 = R0 = Y^2
  5. Select R1, R2
  6. R0 = R1 * R2 = Y^4
  7. Select R0, R0
  8. R0 = R0 + R0 = 2 * Y^4
  9. R0 = R0 + R0 = 4 * Y^4
 10. R0 = R0 + R0 = 8 * Y^4
 11. R2 = R0
 12. Select R3, R2
 13. R0 = R3 - R2 = D - B = R->Y
  
  Read Operations:     
  
  R->Y = R0 = D - B
  
  STEP 4:
  */
  CRYPTO_EXECUTE_11(CRYPTO,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                    
                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                    CRYPTO_CMD_INSTR_MMUL,

                    CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                    CRYPTO_CMD_INSTR_MADD,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    
                    CRYPTO_CMD_INSTR_SELDDATA3DDATA2,
                    CRYPTO_CMD_INSTR_MSUB
                   );
  CRYPTO_DDataWrite(&CRYPTO->DDATA0, _2YY);
  MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&CRYPTO->DDATA0, &R->Y) );

 cleanup:
  return( ret );
}
#endif /* #if defined(MBEDTLS_ECP_DOUBLE_JAC_ALT) */

#if defined(MBEDTLS_ECP_DEVICE_ADD_MIXED_ALT)
/*
 * Alternative implementation of ecp_add_mixed using CRYPTO hardware
 * acceleration.
 *
 * Addition: R = P + Q, mixed affine-Jacobian coordinates (GECC 3.22)
 *
 * The coordinates of Q must be normalized (= affine),
 * but those of P don't need to. R is not normalized.
 *
 * We accept Q->Z being unset (saving memory in tables) as meaning 1.
 *
 * Cost: 1A := 8M + 3S
 */
int ecp_device_add_mixed( const mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                          const mbedtls_ecp_point *P, const mbedtls_ecp_point *Q )
{
  int    ret;
  (void) grp;
  
  /*
  STEP 1:

  Goals:
  A = Qx*Pz^2
  B = Qy*Pz^3
  
  Write Operations:
  
  R0 = Pz
  R0 = Qx
  R0 = Qy
  
  Instructions to be executed:
  
  1. R0 = DMA = Pz
  2. R1 = R0 = Pz
  3. R2 = R0 = Pz
  4. Select R1, R2
  5. R0 = R1 * R2 = Pz^2
  6. R1 = R0 = Pz^2

  7.  R0 = DMA = Qx
  8.  R3 = R0 = Qx
  9.  Select R1, R3
  10. R0 = R1 * R3 = Qx * Pz^2
  11. R3 = R0 = Qx * Pz^2

  12. Select R1, R2
  13. R0 = R1 * R2 = Pz^3
  14. R1 = R0 = Pz^3

  15. R0 = DMA = Qy
  16. R4 = R0 = Qx
  17. Select R1, R4
  18. R0 = R1 * R4 = Qy * Pz^3
  19. Select R0, R1  (for MSUB in step 2)
  
  Output State:
  R0   = B
  R1   = FREE
  R2   = FREE
  R3   = A
  R4   = Pz

  STEP 1:
  */
  CRYPTO_EXECUTE_19(CRYPTO,
                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    
                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                    
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,

                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA1
                    );
  ecp_crypto_ddata_write(&CRYPTO->DDATA0, &P->Z);
  ecp_crypto_ddata_write(&CRYPTO->DDATA0, &Q->X);
  ecp_crypto_ddata_write(&CRYPTO->DDATA0, &Q->Y);

  /*
  STEP 2:

  Goals:
  C  = A - Px
  D  = B - Py
  R->Z = Pz * C
  
  Write Operations:
  
  R1 = Py
  R0 = Px (via DMA)

  Input State:
  R0   = B
  R1   = Py
  R2   = FREE
  R3   = A
  R4   = Pz
  
  Instructions to be executed:
  
  1. R0 = R0 - R2 = B - Py = D
  2. R2 = R0 = D
  3. R1 = R3 = A
  4. R0 = DMA = Px
  5. R3 = R0 = Px
  6. Select R1, R3
  7. R0 = R1 - R3 = A - Px = C
  8. R1 = R0 = C
  9. Select R1, R4
  10. R0 = R1 * R4 = Pz * C = R->Z
  11. R4 = R1 = C
  
  Read Operations:     
  
  R->Z = R0 = Pz * C

  Output State:
  R0   = FREE
  R1   = C
  R2   = D
  R3   = Px
  R4   = FREE
  
  STEP 2:
  */
  
  ecp_crypto_ddata_write(&CRYPTO->DDATA1, &P->Y);
  CRYPTO_EXECUTE_11(CRYPTO,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2, /* R2 = D */

                    CRYPTO_CMD_INSTR_DDATA3TODDATA1,
                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,  /* R1 = C */
                   
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA1TODDATA4
                    );
  ecp_crypto_ddata_write(&CRYPTO->DDATA0, &P->X);
  MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&CRYPTO->DDATA0, &R->Z) );

  /*
  STEP 3:
    
  Goals:
  X1C2  = Px * C^2
  C3    = C^3
  D2    = D^2
  
  Input State:
  R0   = FREE
  R1   = C
  R2   = D
  R3   = Px
  R4   = FREE

  Instructions to be executed:
  
  1. R0 = R1 * R4 = C^2
  2. R1 = R0 = C^2
  3. R0 = R1 * R4 = C^3
  4. R4 = R0 = C^3
  5. Select R1, R3
  6. R0 = R1 * R3 = Px * C^2
  7. R3 = R0 = Px * C^2
  8. R1 = R2 = D
  9. Select R1, R1
  10. R0 = R1 * R1 = D^2
  11. Select R0, R4   (for MSUB operation in next sequence)
  
  Output state:
  
  R0 = D2
  R1 = FREE
  R2 = D
  R3 = X1C2 = Px * C^2
  R4 = C3   = C^3
  
  STEP 3:
  */
  CRYPTO_EXECUTE_11(CRYPTO,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                    CRYPTO_CMD_INSTR_DDATA2TODDATA1,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA4
                    );
  /*       
  STEP 3:
           
  Goals:
  R->X   = D2 - (C3 + 2 * X1C2) = D2 - C3 - X1C2- X1C2
  Y1C3 = Py * C3
  R->Y = D * (X1C2 - R->X) - Y1C3

  Write Operations:
  R1 = Py

  Input State:  
  R0 = D2
  R1 = FREE
  R2 = D
  R3 = X1C2
  R4 = C3
  
  Instructions to be executed:
  
  1.  R0 = R0 - R4 = D2 - C3
  2.  Select R0, R3
  3.  R0 = R0 - R3 = D2 - C3 - X1C2
  4.  R0 = R0 - R3 = D2 - C3 - X1C2 - X1C2 = R->X
  5.  DMA = R0 = R->X
  6.  R1 = R0 = R->X

  7.  Select R3, R1
  8.  R0 = R3 - R1 = X1C2 - R->X
  9.  R1 = R0 = X1C2 - R->X
  10. Select R1, R2
  11. R0 = R1 * R2 = D *(X1C2 - R->X)
  12. R2 = R0

  13. R0 = DMA = Py
  14. R1 = R0 = Py
  15. Select R1, R4
  16. R0 = R1 * R4 = Py * C3 = Y1C3
  17. R4 = R0 = Y1C3

  18. Select R2, R4
  19. R0 = R2 - R4 
  
  Read Operations:     
  
  R->X = R2 = D2 - (C3 + 2 * X1C2)
  R->Y = R0 = D * (X1C2 - R->X) - Y1C3
  
  STEP 4:
  */

  CRYPTO_EXECUTE_19(CRYPTO,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_SELDDATA0DDATA3,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_DATATODMA0,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    
                    CRYPTO_CMD_INSTR_SELDDATA3DDATA1,
                    CRYPTO_CMD_INSTR_MSUB,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                    
                    CRYPTO_CMD_INSTR_DMA0TODATA,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                    CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                    CRYPTO_CMD_INSTR_MMUL,
                    CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                    
                    CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                    CRYPTO_CMD_INSTR_MSUB
                    );
  MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&CRYPTO->DDATA0, &R->X) );
  ecp_crypto_ddata_write(&CRYPTO->DDATA0, &P->Y);
  MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&CRYPTO->DDATA0, &R->Y) );

 cleanup:
  return( ret );
}
#endif /* #if defined(MBEDTLS_ECP_DEVICE_ADD_MIXED_ALT) */

#if defined(MBEDTLS_MPI_MODULAR_DIVISION_ALT)
/***************************************************************************//**
 * @brief
 *   Modular division using CRYPTO hardware acceleration.
 *
 * @details
 *   This function computes R = X/Y mod(N) using CRYPTO hardware acceleration.
 *   The implementation is not a direct replacement plugin, i.e. alternative
 *   implementation, of an existing mbedtls function. This function is used
 *   internally in other CRYPTO plugin functions indirectly replacing
 *   mbedtls_mpi_inv_mod.
 *
 *  @param[in]  X        Dividend of modular division operation
 *  @param[in]  Y        Divisor of  modular division operation
 *  @param[in]  N        Modulus
 *  @param[out] R        The destination of the result
 *
 * @return N/A
 ******************************************************************************/
static void mbedtls_mpi_div_mod(ecc_bigint_t  X,
                                ecc_bigint_t  Y,
                                ecc_bigint_t  N,
                                ecc_bigint_t  R)
{
    uint32_t            D[9];
    uint32_t            status_reg;
    uint8_t             rdata;
    uint8_t             lsb_C;
    uint8_t             lsb_D;
    uint8_t             lsb_U;
    int                 t;
    int                 k;

    /************** Initialize and organize data in crypto module **************/

    /*
    ** Register usage:
    **
    ** DDATA0 - holds temporary results and loads 260 bit variables in/out 
    ** DDATA1 - variable referred to as 'C' in the following algorithm
    ** DDATA2 - variable referred to as 'U' in the following algorithm
    ** DDATA3 - variable referred to as 'D' in the following algorithm
    ** DDATA4 - variable referred to as 'W' in the following algorithm
    */
    
    EC_BIGINT_COPY(D, N);             /* D will hold the modulus (n) initially */
    D[8]=0;                           /* Set MSWord of D to 0. */
    
    CRYPTO_DDataWrite(&CRYPTO->DDATA1, Y);  /* Set C to Y (divisor) initially */
    CRYPTO_DDataWrite(&CRYPTO->DDATA2, X);  /* Set U to X (dividend)initially */
    
    CRYPTO_DDataWrite(&CRYPTO->DDATA3, N);  /* Set D to modulus p initially   */
    
    CRYPTO_EXECUTE_3(CRYPTO,
                     CRYPTO_CMD_INSTR_CLR,            /* DDATA0 = 0 */
                     CRYPTO_CMD_INSTR_DDATA0TODDATA4, /* Set W to zero initially*/
                     CRYPTO_CMD_INSTR_DDATA1TODDATA0);/* DDATA0 = C initially */
    
    t     = 0;
    k     = 1;
    
    /******************* Run main loop while 'C' is non-zero ********************/
    
    /* while (C != 1024'd0)  */
    while ( !crypto_ddata0_is_zero(&status_reg) )
    {
        lsb_C = (status_reg & _CRYPTO_DSTATUS_DDATA0LSBS_MASK) >> _CRYPTO_DSTATUS_DDATA0LSBS_SHIFT;
        if ((lsb_C & 0x1) == 0)
        {
            CRYPTO_EXECUTE_3(CRYPTO,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA1,
                             CRYPTO_CMD_INSTR_SHRA,
                             CRYPTO_CMD_INSTR_DDATA0TODDATA1                                    
                             );
            t = t-1;  
        }
        else
        {
            if (t<0)
            {
                CRYPTO_EXECUTE_6(CRYPTO,
                                 CRYPTO_CMD_INSTR_DDATA2TODDATA0,
                                 CRYPTO_CMD_INSTR_DDATA4TODDATA2,
                                 CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                                 CRYPTO_CMD_INSTR_DDATA1TODDATA0,
                                 CRYPTO_CMD_INSTR_DDATA3TODDATA1,
                                 CRYPTO_CMD_INSTR_DDATA0TODDATA3);
                CRYPTO_DDATA0_260_BITS_READ(CRYPTO, D);
                t = -t;
            } 
            
            k = 1;
            
            CRYPTO_EXECUTE_2(CRYPTO,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                             CRYPTO_CMD_INSTR_ADD);
            
            rdata = CRYPTO_DData0_4LSBitsRead(CRYPTO);
            
            if((rdata & 0x3) != 0x0)
              k = -1;
            else
              t = t-1;
            
            /*  R1 = C >> 1  */
            CRYPTO->CMD = CRYPTO_CMD_INSTR_DDATA1TODDATA0; /* to get the lsb of C */
            
            lsb_C = CRYPTO_DData0_4LSBitsRead(CRYPTO);
            CRYPTO_EXECUTE_4(CRYPTO,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA1,
                             CRYPTO_CMD_INSTR_SHRA,
                             CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                             CRYPTO_CMD_INSTR_DDATA3TODDATA0); /* to get the lsb of D(R3) */
            
            /*  R3 = D >> 1  */
            lsb_D = CRYPTO_DData0_4LSBitsRead(CRYPTO);
            
            CRYPTO_EXECUTE_2(CRYPTO,
                             CRYPTO_CMD_INSTR_SELDDATA3DDATA3,
                             CRYPTO_CMD_INSTR_SHRA);
            
            if(k == 1)
            {
                if (((lsb_C & 0x1)==0x1) && ((lsb_D & 0x1)==0x1))
                {
                    CRYPTO_EXECUTE_7(CRYPTO,
                                     /*  C = R1+R3+1  */
                                     CRYPTO_CMD_INSTR_SELDDATA0DDATA1,
                                     CRYPTO_CMD_INSTR_CSET,
                                     CRYPTO_CMD_INSTR_ADDC,
                                     CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                                     /*  U = mod(R2+R4,n)  */
                                     CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                                     CRYPTO_CMD_INSTR_MADD,
                                     CRYPTO_CMD_INSTR_DDATA0TODDATA2
                                     );
                }
                else
                {
                    CRYPTO_EXECUTE_6(CRYPTO,
                                     /*  C = R1+R3  */
                                     CRYPTO_CMD_INSTR_SELDDATA0DDATA1,
                                     CRYPTO_CMD_INSTR_ADD,
                                     CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                                     /*  U = mod(R2+R4,n)  */
                                     CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                                     CRYPTO_CMD_INSTR_MADD,
                                     CRYPTO_CMD_INSTR_DDATA0TODDATA2
                                     );
                }
            }
            else
            {
                if (k == -1)
                {
                    if (((lsb_C & 0x1)==0x0) && ((lsb_D & 0x1)==0x1))     
                    {
                        CRYPTO_EXECUTE_8(CRYPTO,
                                         /*  C = R1-R3-1  */
                                         CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                                         CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                                         CRYPTO_CMD_INSTR_CSET,
                                         CRYPTO_CMD_INSTR_SUBC,
                                         CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                                         /*  U = mod(R2-R4,p)  */
                                         CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                                         CRYPTO_CMD_INSTR_MSUB,
                                         CRYPTO_CMD_INSTR_DDATA0TODDATA2
                                         );
                    }
                    else
                    {
                        CRYPTO_EXECUTE_7(CRYPTO,
                                         /*  C = R1+R3  */
                                         CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                                         CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                                         CRYPTO_CMD_INSTR_SUB,
                                         CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                                         /*  U = mod(R2-R4,p)  */
                                         CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                                         CRYPTO_CMD_INSTR_MSUB,
                                         CRYPTO_CMD_INSTR_DDATA0TODDATA2
                                         );
                    }
                    
                    CRYPTO_DDATA0_260_BITS_WRITE(CRYPTO, D);
                    CRYPTO->CMD = CRYPTO_CMD_INSTR_DDATA0TODDATA3;
                    
                } /* if (k == -1) */
            }        
        } /* else: !if((C[31:0] & 0x1) == 0x0) */
           
        CRYPTO->CMD = CRYPTO_CMD_INSTR_DDATA2TODDATA0;
    
        lsb_U = CRYPTO_DData0_4LSBitsRead(CRYPTO);

        /* if ((U[31:0] & 0x1) == 0x1) */
        if((lsb_U & 0x1) == 0x1)
        {
            CRYPTO_EXECUTE_10(CRYPTO,
                              CRYPTO_CMD_INSTR_SELDDATA2DDATA2,
                              CRYPTO_CMD_INSTR_SHRA,
                              CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                              CRYPTO_CMD_INSTR_DMA0TODATA,  /* DDATA0 = N */
                              CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                              CRYPTO_CMD_INSTR_SHR,
                              CRYPTO_CMD_INSTR_SELDDATA0DDATA2,
                              CRYPTO_CMD_INSTR_CSET,
                              CRYPTO_CMD_INSTR_ADDC,
                              CRYPTO_CMD_INSTR_DDATA0TODDATA2);
            
            CRYPTO_DDataWrite(&CRYPTO->DDATA0, N);
        }
        else
        {
            CRYPTO_EXECUTE_3(CRYPTO,
                             CRYPTO_CMD_INSTR_SELDDATA2DDATA2,
                             CRYPTO_CMD_INSTR_SHRA,
                             CRYPTO_CMD_INSTR_DDATA0TODDATA2);
        } 
        
        /* DDATA0 = C */
        CRYPTO->CMD = CRYPTO_CMD_INSTR_DDATA1TODDATA0;
        
    } /* End of main loop:  while (C != 0)  */
    
    /* if (D == 1): */
    /* Decrement D by 1 and test if zero. */
    CRYPTO_EXECUTE_2(CRYPTO,
                     CRYPTO_CMD_INSTR_DDATA3TODDATA0,
                     CRYPTO_CMD_INSTR_DEC);
    
    if (crypto_ddata0_is_zero(&status_reg))
    {
        CRYPTO_DDataRead(&CRYPTO->DDATA4, R);
    }
    else
    {
        CRYPTO_DDataWrite(&CRYPTO->DDATA0, N);
        CRYPTO_EXECUTE_2(CRYPTO,
                         CRYPTO_CMD_INSTR_SELDDATA0DDATA4,
                         CRYPTO_CMD_INSTR_SUB
                         );
        CRYPTO_DDataRead(&CRYPTO->DDATA0, R);
    }
    
    return;
} /* mbedtls_mpi_div_mod  */
#endif /* #if defined(MBEDTLS_MPI_MODULAR_DIVISION_ALT) */

#if defined(MBEDTLS_ECP_NORMALIZE_JAC_ALT)
/*
 * Alternative implementation of ecp_normalize_jac using CRYPTO hardware
 * acceleration.
 *
 * Normalize jacobian coordinates so that Z == 0 || Z == 1  (GECC 3.2.1)
 */
int ecp_device_normalize_jac( const mbedtls_ecp_group *grp, mbedtls_ecp_point *P )
{
    int ret = 0;

#if defined(MBEDTLS_MPI_MODULAR_DIVISION_ALT)

    ecc_bigint_t    one;
    ecc_bigint_t    Z;
    ecc_bigint_t    modulus;
    ecc_bigint_t    Z_inv;

    memset(one, 0, sizeof(one));
    one[0]=1;

    MPI_TO_BIGINT( Z, &P->Z );
    MPI_TO_BIGINT( modulus, &grp->P );

    mbedtls_mpi_div_mod(one, Z, modulus, Z_inv);

    CRYPTO_DDataWrite(&CRYPTO->DDATA1, Z_inv);
    
#else
    
    mbedtls_mpi Z_inv;
    mbedtls_mpi_init( &Z_inv );

    /*
     * Z_inv = 1 / Z  mod p
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &Z_inv,      &P->Z,     &grp->P ) );

    ecp_crypto_ddata_write(&CRYPTO->DDATA1, &Z_inv);
#endif
    
    /*
             
    Goals:
    R->X = P->X * Z_inv ^2
    R->Y = P->Y * Z_inv ^3
    
    Write Operations:
    
    R1 = Z_inv
    R3 = P->X
    R4 = P->Y
    
    Instructions to be executed:
    
    1.  R2 = R1 = Z_inv
    2.  Select R1, R2
    3.  R0 = R1 * R2 = Z_inv^2
    4.  R1 = R0 = Z_inv^2
    5.  Select R1, R3
    6.  R0 = R1 * R3 = P->X * Z_inv^2 = R->X
    7.  R3 = R0
    8.  Select R1, R2
    9.  R0 = R1 * R2 = Z_inv^3
    10. R1 = R0 = Z_inv^3
    11. Select R1, R4
    12. R0 = R1 * R4 = P->Y * Z_inv^3 = R->Y
    
    Read Operations:     
    
    R->Y = R0 = P->Y * P->Z_inv^3
    R->X = R3 = P->X * P->Z_inv^2
            
    */
    
    ecp_crypto_ddata_write(&CRYPTO->DDATA3, &P->X);
    ecp_crypto_ddata_write(&CRYPTO->DDATA4, &P->Y);
    
    CRYPTO_EXECUTE_12(CRYPTO,
                      CRYPTO_CMD_INSTR_DDATA1TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL
                      );

    ecp_crypto_ddata_read(&CRYPTO->DDATA0, &P->Y);
    ecp_crypto_ddata_read(&CRYPTO->DDATA3, &P->X);

    /*
     * Z = 1
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &P->Z, 1 ) );

 cleanup:

#if !defined(MBEDTLS_MPI_MODULAR_DIVISION_ALT)
    mbedtls_mpi_free( &Z_inv );
#endif /* #if !defined(MBEDTLS_MPI_MODULAR_DIVISION_ALT) */

    return( ret );
}
#endif /* #if defined(MBEDTLS_ECP_NORMALIZE_JAC_ALT) */

#if defined(MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT)
/*
 * Alternative implementation of ecp_normalize_jac_many using
 * CRYPTO hardware acceleration.
 *
 * Normalize jacobian coordinates of an array of (pointers to) points,
 * using Montgomery's trick to perform only one inversion mod P.
 * (See for example Cohen's "A Course in Computational Algebraic Number
 * Theory", Algorithm 10.3.4.)
 */
int ecp_device_normalize_jac_many( const mbedtls_ecp_group *grp,
                                   mbedtls_ecp_point *T[], size_t t_len )
{
    int ret;
    size_t i;
    ecc_bigint_t*   cc;
    ecc_bigint_t    uu;
    ecc_bigint_t    one;
    ecc_bigint_t    modulus;

    if( t_len < 2 )
        return( ecp_device_normalize_jac( grp, *T ) );

    if( ( cc = mbedtls_calloc( t_len, sizeof( ecc_bigint_t ) ) ) == NULL )
        return( MBEDTLS_ERR_ECP_ALLOC_FAILED );

    /*
     * c[i] = Z_0 * ... * Z_i
     */
    MPI_TO_BIGINT( cc[0], &T[0]->Z );
    for( i = 1; i < t_len; i++ )
    {
        ecp_crypto_ddata_write( &CRYPTO->DDATA1, &T[i]->Z );
        CRYPTO_DDataWrite( &CRYPTO->DDATA2, cc[i-1] );
        CRYPTO_EXECUTE_2(CRYPTO,
                         CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                         CRYPTO_CMD_INSTR_MMUL);
        CRYPTO_DDataRead( &CRYPTO->DDATA0, cc[i] );
    }

    memset(one, 0, sizeof(one));
    one[0]=1;
    MPI_TO_BIGINT( modulus, &grp->P );

    /*
     * u = 1 / (Z_0 * ... * Z_n) mod P
     */
    mbedtls_mpi_div_mod(one, cc[t_len-1], modulus, uu);

    for( i = t_len - 1; ; i-- )
    {
        /*
         * Zi = 1 / Z_i mod p
         * u = 1 / (Z_0 * ... * Z_i) mod P
         */
        if( i == 0 )
        {
            /* Z_inv (DDATA2) = uu */
            CRYPTO_DDataWrite(&CRYPTO->DDATA2, uu);
        }
        else
        {
            /* Z_inv (DDATA1) = uu x cc[i-1] modulo p */
            /* uu = uu x T[i]->Z modulo p */
            CRYPTO_DDataWrite(&CRYPTO->DDATA1, uu);
            CRYPTO_DDataWrite(&CRYPTO->DDATA2, cc[i-1]);
            ecp_crypto_ddata_write( &CRYPTO->DDATA3, &T[i]->Z );
            CRYPTO_EXECUTE_5(CRYPTO,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                             CRYPTO_CMD_INSTR_MMUL,
                             CRYPTO_CMD_INSTR_DDATA0TODDATA2, /* Z_inv (DDATA2) */
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                             CRYPTO_CMD_INSTR_MMUL);
            CRYPTO_DDataRead(&CRYPTO->DDATA0, uu);
        }

        /*
         * proceed as in normalize()
         */
        ecp_crypto_ddata_write(&CRYPTO->DDATA3, &T[i]->X);
        ecp_crypto_ddata_write(&CRYPTO->DDATA4, &T[i]->Y);

        /* Z_inv  already in DDATA2 */
        CRYPTO_EXECUTE_12(CRYPTO,
                          CRYPTO_CMD_INSTR_DDATA2TODDATA1,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                          CRYPTO_CMD_INSTR_MMUL,
                          CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                          CRYPTO_CMD_INSTR_MMUL,
                          CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                          CRYPTO_CMD_INSTR_MMUL,
                          CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                          CRYPTO_CMD_INSTR_MMUL
                          );

        ecp_crypto_ddata_read(&CRYPTO->DDATA0, &T[i]->Y);
        ecp_crypto_ddata_read(&CRYPTO->DDATA3, &T[i]->X);

        /*
         * Post-precessing: reclaim some memory by shrinking coordinates
         * - not storing Z (always 1)
         * - shrinking other coordinates, but still keeping the same number of
         *   limbs as P, as otherwise it will too likely be regrown too fast.
         */
        MBEDTLS_MPI_CHK( mbedtls_mpi_shrink( &T[i]->X, grp->P.n ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_shrink( &T[i]->Y, grp->P.n ) );
        mbedtls_mpi_free( &T[i]->Z );

        if( i == 0 )
            break;
    }

cleanup:

    mbedtls_free( cc );

    return( ret );
}
#endif /* #if defined( MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT ) */

#endif /* #if defined( CRYPTO_COUNT ) && ( CRYPTO_COUNT > 0 ) */

#endif /* #if !defined( MBEDTLS_SLCL_PLUGINS ) */

#endif /* #if defined( MBEDTLS_ECP_DEVICE_ALT ) */

#endif /* #if defined( MBEDTLS_ECP_C ) */
