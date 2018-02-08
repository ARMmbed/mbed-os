/*
 *  Elliptic curves over GF(p): CRYPTO hw acceleration functions
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
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

#include "em_device.h"

#if defined( CRYPTO_PRESENT )

#if defined( MBEDTLS_ECP_C )
#if defined( MBEDTLS_ECP_INTERNAL_ALT )

#include "mbedtls/ecp.h"
#include "mbedtls/ecp_internal.h"
#include "mbedtls/platform.h"
#include "em_crypto.h"
#include "em_core.h"
#include "crypto_management.h"

#include <stdbool.h>
#include <string.h>

/** ECC big integer type. */
#define ECC_BIGINT_SIZE_IN_BITS        (256)
#define ECC_BIGINT_SIZE_IN_BYTES       (ECC_BIGINT_SIZE_IN_BITS/8)
#define ECC_BIGINT_SIZE_IN_32BIT_WORDS (ECC_BIGINT_SIZE_IN_BYTES/sizeof(uint32_t))
#define EC_BIGINT_COPY(X, Y) memcpy((X), (Y), sizeof(ecc_bigint_t));
typedef uint32_t ecc_bigint_t[ECC_BIGINT_SIZE_IN_32BIT_WORDS];

#define SLCL_ECP_CHK(f) do { if( ( ret = (f) ) != 0 ) goto cleanup; } while( 0 )

#if defined(MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT) || defined(MBEDTLS_ECP_NORMALIZE_JAC_ALT)
#define MPI_TO_BIGINT(bigint, mpi) mpitobigint((bigint), (mpi));

/***************************************************************************//**
 * @brief
 *   Convert an mpi number representation to a 32bit word array used by crypto.
 ******************************************************************************/
__STATIC_INLINE void mpitobigint( ecc_bigint_t bigint, const mbedtls_mpi* mpi )
{
    uint32_t* bi = bigint;

    if ( mpi->n < ECC_BIGINT_SIZE_IN_32BIT_WORDS )
    {
      memcpy(bigint, mpi->p, mpi->n * sizeof(uint32_t));
      memset(&bi[mpi->n],
             0,
             ECC_BIGINT_SIZE_IN_BYTES - ( mpi->n * sizeof(uint32_t) ) );
    }
    else
    {
      /* mpi has more room than bigint, so only store up to sizeof(bigint) */
      memcpy(bigint, mpi->p, ECC_BIGINT_SIZE_IN_BYTES);
    }
}

/***************************************************************************//**
 * @brief
 *   Returns true if the value of the DDATA0 register is equal to zero.
 ******************************************************************************/
__STATIC_INLINE bool crypto_ddata0_is_zero(CRYPTO_TypeDef* crypto,
                                           uint32_t*       status_reg)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  CRYPTO_EXECUTE_3(crypto,
                   CRYPTO_CMD_INSTR_CCLR,
                   CRYPTO_CMD_INSTR_DEC,  /* Decrement by one which will set
                                             carry bit if DDATA0 is zero. */
                   CRYPTO_CMD_INSTR_INC   /* Increment in order to restore
                                             original value. */
                   );

  *status_reg = crypto->DSTATUS;
  CORE_EXIT_CRITICAL();

  return (*status_reg & CRYPTO_DSTATUS_CARRY) == CRYPTO_DSTATUS_CARRY;
}

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
static void crypto_mpi_div_mod(CRYPTO_TypeDef *crypto,
                               ecc_bigint_t   X,
                               ecc_bigint_t   Y,
                               ecc_bigint_t   N,
                               ecc_bigint_t   R)
{
    uint32_t            D[9];
    uint32_t            status_reg;
    uint8_t             rdata;
    uint8_t             lsb_C;
    uint8_t             lsb_D;
    uint8_t             lsb_U;
    int                 t;
    int                 k;
    CORE_DECLARE_IRQ_STATE;

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

    CORE_ENTER_CRITICAL();
    CRYPTO_DDataWrite(&crypto->DDATA1, Y);  /* Set C to Y (divisor) initially */
    CRYPTO_DDataWrite(&crypto->DDATA2, X);  /* Set U to X (dividend)initially */
    CRYPTO_DDataWrite(&crypto->DDATA3, N);  /* Set D to modulus p initially   */
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_3(crypto,
                     CRYPTO_CMD_INSTR_CLR,            /* DDATA0 = 0 */
                     CRYPTO_CMD_INSTR_DDATA0TODDATA4, /* Set W to zero initially*/
                     CRYPTO_CMD_INSTR_DDATA1TODDATA0);/* DDATA0 = C initially */

    t     = 0;
    k     = 1;

    /******************* Run main loop while 'C' is non-zero ********************/

    /* while (C != 1024'd0)  */
    while ( !crypto_ddata0_is_zero(crypto, &status_reg) )
    {

        lsb_C = (status_reg & _CRYPTO_DSTATUS_DDATA0LSBS_MASK) >> _CRYPTO_DSTATUS_DDATA0LSBS_SHIFT;
        if ((lsb_C & 0x1) == 0)
        {
            CRYPTO_EXECUTE_3(crypto,
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
                CRYPTO_EXECUTE_6(crypto,
                                 CRYPTO_CMD_INSTR_DDATA2TODDATA0,
                                 CRYPTO_CMD_INSTR_DDATA4TODDATA2,
                                 CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                                 CRYPTO_CMD_INSTR_DDATA1TODDATA0,
                                 CRYPTO_CMD_INSTR_DDATA3TODDATA1,
                                 CRYPTO_CMD_INSTR_DDATA0TODDATA3);
                CORE_ENTER_CRITICAL();
                CRYPTO_DDATA0_260_BITS_READ(crypto, D);
                CORE_EXIT_CRITICAL();
                t = -t;
            }

            k = 1;

            CRYPTO_EXECUTE_2(crypto,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                             CRYPTO_CMD_INSTR_ADD);

            rdata = CRYPTO_DData0_4LSBitsRead(crypto);

            if((rdata & 0x3) != 0x0)
              k = -1;
            else
              t = t-1;

            /*  R1 = C >> 1  */
            crypto->CMD = CRYPTO_CMD_INSTR_DDATA1TODDATA0; /* to get the lsb of C */

            lsb_C = CRYPTO_DData0_4LSBitsRead(crypto);
            CRYPTO_EXECUTE_4(crypto,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA1,
                             CRYPTO_CMD_INSTR_SHRA,
                             CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                             CRYPTO_CMD_INSTR_DDATA3TODDATA0); /* to get the lsb of D(R3) */

            /*  R3 = D >> 1  */
            lsb_D = CRYPTO_DData0_4LSBitsRead(crypto);

            CRYPTO_EXECUTE_2(crypto,
                             CRYPTO_CMD_INSTR_SELDDATA3DDATA3,
                             CRYPTO_CMD_INSTR_SHRA);

            if(k == 1)
            {
                if (((lsb_C & 0x1)==0x1) && ((lsb_D & 0x1)==0x1))
                {
                    CRYPTO_EXECUTE_7(crypto,
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
                    CRYPTO_EXECUTE_6(crypto,
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
                        CRYPTO_EXECUTE_8(crypto,
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
                        CRYPTO_EXECUTE_7(crypto,
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

                    CRYPTO_DDATA0_260_BITS_WRITE(crypto, D);
                    crypto->CMD = CRYPTO_CMD_INSTR_DDATA0TODDATA3;

                } /* if (k == -1) */
            }
        } /* else: !if((C[31:0] & 0x1) == 0x0) */

        crypto->CMD = CRYPTO_CMD_INSTR_DDATA2TODDATA0;

        lsb_U = CRYPTO_DData0_4LSBitsRead(crypto);

        /* if ((U[31:0] & 0x1) == 0x1) */
        if((lsb_U & 0x1) == 0x1)
        {
            CRYPTO_EXECUTE_3( crypto,
                              CRYPTO_CMD_INSTR_SELDDATA2DDATA2,
                              CRYPTO_CMD_INSTR_SHRA,
                              CRYPTO_CMD_INSTR_DDATA0TODDATA2);

            CORE_ENTER_CRITICAL();
            CRYPTO_DDataWrite(&crypto->DDATA0, N);
            CORE_EXIT_CRITICAL();

            CRYPTO_EXECUTE_6( crypto,
                              CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                              CRYPTO_CMD_INSTR_SHR,
                              CRYPTO_CMD_INSTR_SELDDATA0DDATA2,
                              CRYPTO_CMD_INSTR_CSET,
                              CRYPTO_CMD_INSTR_ADDC,
                              CRYPTO_CMD_INSTR_DDATA0TODDATA2);
        }
        else
        {
            CRYPTO_EXECUTE_3(crypto,
                             CRYPTO_CMD_INSTR_SELDDATA2DDATA2,
                             CRYPTO_CMD_INSTR_SHRA,
                             CRYPTO_CMD_INSTR_DDATA0TODDATA2);
        }

        /* DDATA0 = C */
        crypto->CMD = CRYPTO_CMD_INSTR_DDATA1TODDATA0;

    } /* End of main loop:  while (C != 0)  */

    /* if (D == 1): */
    /* Decrement D by 1 and test if zero. */
    CRYPTO_EXECUTE_2(crypto,
                     CRYPTO_CMD_INSTR_DDATA3TODDATA0,
                     CRYPTO_CMD_INSTR_DEC);

    if (crypto_ddata0_is_zero(crypto, &status_reg))
    {
        CORE_ENTER_CRITICAL();
        CRYPTO_DDataRead(&crypto->DDATA4, R);
        CORE_EXIT_CRITICAL();
    }
    else
    {
        CORE_ENTER_CRITICAL();
        CRYPTO_DDataWrite(&crypto->DDATA0, N);
        CORE_EXIT_CRITICAL();

        CRYPTO_EXECUTE_2(crypto,
                         CRYPTO_CMD_INSTR_SELDDATA0DDATA4,
                         CRYPTO_CMD_INSTR_SUB
                         );

        CORE_ENTER_CRITICAL();
        CRYPTO_DDataRead(&crypto->DDATA0, R);
        CORE_EXIT_CRITICAL();
    }
    return;
} /* crypto_mpi_div_mod  */
#endif /* MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT || MBEDTLS_ECP_NORMALIZE_JAC_ALT */

/***************************************************************************//**
 * @brief
 *   Enable CRYPTO by setting up control registers for given ecc curve.
 ******************************************************************************/
static int crypto_device_init( CRYPTO_TypeDef *device, const mbedtls_ecp_group *grp)
{
    int             ret = 0;

    /* Setup CRYPTO registers for ECC operation */
    device->CTRL     = 0;
    device->SEQCTRL  = CRYPTO_SEQCTRL_BLOCKSIZE_32BYTES | 32;
    device->SEQCTRLB = 0;

    switch( grp->id )
    {
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP192R1:
            CRYPTO_ModulusSet( device, cryptoModulusEccP192 );
            CRYPTO_MulOperandWidthSet( device, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( device, cryptoResult256Bits );
        break;
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP224R1:
            CRYPTO_ModulusSet( device, cryptoModulusEccP224 );
            CRYPTO_MulOperandWidthSet( device, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( device, cryptoResult256Bits );
        break;
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP256R1:
            CRYPTO_ModulusSet( device, cryptoModulusEccP256 );
            CRYPTO_MulOperandWidthSet( device, cryptoMulOperandModulusBits );
            CRYPTO_ResultWidthSet( device, cryptoResult260Bits );
        break;
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */

        default:
            ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
        break;
    }

    return( ret );
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
  CORE_DECLARE_IRQ_STATE;

  if (mpi->n == 8)
  {
    CORE_ENTER_CRITICAL();
    CRYPTO_DDataRead(ddataReg, mpi->p);
    CORE_EXIT_CRITICAL();
  }
  else
  {
    if (mpi->n > 8)
    {
      CORE_ENTER_CRITICAL();
      CRYPTO_DDataRead(ddataReg, mpi->p);
      CORE_EXIT_CRITICAL();
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
        SLCL_ECP_CHK( mbedtls_mpi_grow(mpi, used) );
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

/**
 * \brief           Indicate if the Elliptic Curve Point module extension can
 *                  handle the group.
 *
 * \param grp       The pointer to the elliptic curve group that will be the
 *                  basis of the cryptographic computations.
 *
 * \return          Non-zero if successful.
 */
unsigned char mbedtls_internal_ecp_grp_capable( const mbedtls_ecp_group *grp )
{
    switch( grp->id )
    {
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP192R1:
            return( true );
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP224R1:
            return( true );
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
        case MBEDTLS_ECP_DP_SECP256R1:
            return( true );
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */

        default:
            return( false );
    }
}

/**
 * \brief           Initialise the Elliptic Curve Point module extension.
 *
 *                  If mbedtls_internal_ecp_grp_capable returns true for a
 *                  group, this function has to be able to initialise the
 *                  module for it.
 *
 *                  This module can be a driver to a crypto hardware
 *                  accelerator, for which this could be an initialise function.
 *
 * \param grp       The pointer to the group the module needs to be
 *                  initialised for.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_init( const mbedtls_ecp_group *grp )
{
    /* Crypto operations are atomic, so no need to setup any context here */
    (void) grp;
    return 0;
}

/**
 * \brief           Frees and deallocates the Elliptic Curve Point module
 *                  extension.
 *
 * \param grp       The pointer to the group the module was initialised for.
 */
void mbedtls_internal_ecp_free( const mbedtls_ecp_group *grp )
{
    /* Crypto operations are atomic, so no need to free any context here */
    (void) grp;
}

#if defined(MBEDTLS_ECP_RANDOMIZE_JAC_ALT)
/**
 * \brief           Randomize jacobian coordinates:
 *                  (X, Y, Z) -> (l^2 X, l^3 Y, l Z) for random l.
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param pt        The point on the curve to be randomised, given with Jacobian
 *                  coordinates.
 *
 * \param f_rng     A function pointer to the random number generator.
 *
 * \param p_rng     A pointer to the random number generator state.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_randomize_jac( const mbedtls_ecp_group *grp,
                                        mbedtls_ecp_point *pt,
                                        int (*f_rng)(void *, unsigned char *, size_t),
                                        void *p_rng )
{
    int ret;
    ecc_bigint_t l;
    CORE_DECLARE_IRQ_STATE;
    CRYPTO_TypeDef *crypto;

    /* Strategy:
     *  1) Generate l such that 1 < l < p
     *  2) Z = l (R1) * Z (R4)
     *  3) ll (R1) = l (R4) * l
     *  4) X = ll (R1) * X (R2)
     *  5) lll (R1) = ll (R1) * l (R4)
     *  6) Y = lll (R1) * Y (R3)
     */

    /* Acquire entropy before grabbing crypto, since the entropy function might use crypto */
    /* Generate l such that 1 < l < p */
    ret = f_rng(p_rng, (unsigned char *)l, sizeof(l));
    if ( ret != 0 ) {
        return( ret );
    }

    crypto = crypto_management_acquire();
    crypto_device_init(crypto, grp);

    CORE_ENTER_CRITICAL();
    CRYPTO_DDataWrite(&crypto->DDATA1, l);
    ecp_crypto_ddata_write(&crypto->DDATA2, &pt->X);
    ecp_crypto_ddata_write(&crypto->DDATA3, &pt->Y);
    ecp_crypto_ddata_write(&crypto->DDATA4, &pt->Z);
    CORE_EXIT_CRITICAL();

    /* Z = l * Z */
    CRYPTO_EXECUTE_2 (  crypto,
                        CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                        CRYPTO_CMD_INSTR_MMUL );
    CRYPTO_InstructionSequenceWait(crypto);

    MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&crypto->DDATA0, &pt->Z) );

    /* X = l^2 * X */
    CRYPTO_EXECUTE_6 (  crypto,
                        CRYPTO_CMD_INSTR_DDATA1TODDATA4,
                        CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                        CRYPTO_CMD_INSTR_MMUL,
                        CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                        CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                        CRYPTO_CMD_INSTR_MMUL );
    CRYPTO_InstructionSequenceWait(crypto);

    MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&crypto->DDATA0, &pt->X) );

    /* Y = l^3 * Y */
    CRYPTO_EXECUTE_5 (  crypto,
                        CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                        CRYPTO_CMD_INSTR_MMUL,
                        CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                        CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                        CRYPTO_CMD_INSTR_MMUL );
    CRYPTO_InstructionSequenceWait(crypto);

    MBEDTLS_MPI_CHK( ecp_crypto_ddata_read(&crypto->DDATA0, &pt->Y) );

cleanup:
    crypto_management_release( crypto );
    return( ret );
}
#endif

#if defined(MBEDTLS_ECP_ADD_MIXED_ALT)
/**
 * \brief           Addition: R = P + Q, mixed affine-Jacobian coordinates.
 *
 *                  The coordinates of Q must be normalized (= affine),
 *                  but those of P don't need to. R is not normalized.
 *
 *                  This function is used only as a subrutine of
 *                  ecp_mul_comb().
 *
 *                  Special cases: (1) P or Q is zero, (2) R is zero,
 *                      (3) P == Q.
 *                  None of these cases can happen as intermediate step in
 *                  ecp_mul_comb():
 *                      - at each step, P, Q and R are multiples of the base
 *                      point, the factor being less than its order, so none of
 *                      them is zero;
 *                      - Q is an odd multiple of the base point, P an even
 *                      multiple, due to the choice of precomputed points in the
 *                      modified comb method.
 *                  So branches for these cases do not leak secret information.
 *
 *                  We accept Q->Z being unset (saving memory in tables) as
 *                  meaning 1.
 *
 *                  Cost in field operations if done by [5] 3.22:
 *                      1A := 8M + 3S
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param R         Pointer to a point structure to hold the result.
 *
 * \param P         Pointer to the first summand, given with Jacobian
 *                  coordinates
 *
 * \param Q         Pointer to the second summand, given with affine
 *                  coordinates.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_add_mixed( const mbedtls_ecp_group *grp,
                                    mbedtls_ecp_point *R,
                                    const mbedtls_ecp_point *P,
                                    const mbedtls_ecp_point *Q )
{
    int             ret;
    CORE_DECLARE_IRQ_STATE;
    CRYPTO_TypeDef *crypto = crypto_management_acquire();

    crypto_device_init(crypto, grp);

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
    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA0, &P->Z);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_5(crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA4,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1);
    CRYPTO_InstructionSequenceWait(crypto);

    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA0, &Q->X);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_4 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3);
    CRYPTO_InstructionSequenceWait(crypto);
    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1);
    CRYPTO_InstructionSequenceWait(crypto);

    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA0, &Q->Y);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL
                      );
    CRYPTO_InstructionSequenceWait(crypto);

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

      0. Select R0, R1
      1. R0 = R0 - R1 = B - Py = D
      2. R2 = R0 = D
      3. R1 = R3 = A
      4. R0 = DMA = Px
      5. R3 = R0 = Px
      6. Select R1, R3
      7. R0 = R1 - R3 = A - Px = C
      8. R1 = R0 = C
      9. Select R1, R4
      10. R0 = R1 * R4 = Pz * C = R->Z

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

    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA1, &P->Y);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA1,
                      CRYPTO_CMD_INSTR_MSUB,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA2); /* R2 = D */
    CRYPTO_InstructionSequenceWait(crypto);

    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA0, &P->X);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_7 (crypto,
                      CRYPTO_CMD_INSTR_DDATA3TODDATA1,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                      CRYPTO_CMD_INSTR_MSUB,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,  /* R1 = C */

                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL
                      );
    CRYPTO_InstructionSequenceWait(crypto);

    ret = ecp_crypto_ddata_read(&crypto->DDATA0, &R->Z);

    if (ret != 0) goto cleanup;

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

      1. R4 = R1 = C
      2. Select R1, R4
      3. R0 = R1 * R4 = C^2
      4. R1 = R0 = C^2
      5. R0 = R1 * R4 = C^3
      6. R4 = R0 = C^3
      7. Select R1, R3
      8. R0 = R1 * R3 = Px * C^2
      9. R3 = R0 = Px * C^2
      10. R1 = R2 = D
      11. Select R1, R1
      12. R0 = R1 * R1 = D^2
      13. Select R0, R4
      14. R0 = R0 - R4 = D2 - C3

      Output state:

      R0 = D2 - C3
      R1 = FREE
      R2 = D
      R3 = X1C2 = Px * C^2
      R4 = C3   = C^3

      STEP 3:
    */
    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_DDATA1TODDATA4,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL);
    CRYPTO_InstructionSequenceWait(crypto);

    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA4);
    CRYPTO_InstructionSequenceWait(crypto);

    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3);
    CRYPTO_InstructionSequenceWait(crypto);

    CRYPTO_EXECUTE_5 (crypto,
                      CRYPTO_CMD_INSTR_DDATA2TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA4,
                      CRYPTO_CMD_INSTR_MSUB
                      );
    CRYPTO_InstructionSequenceWait(crypto);

    /*
      STEP 3:

      Goals:
      R->X   = D2 - (C3 + 2 * X1C2) = D2 - C3 - X1C2- X1C2
      Y1C3 = Py * C3
      R->Y = D * (X1C2 - R->X) - Y1C3

      Write Operations:
      R1 = Py

      Input State:
      R0 = D2 - C3
      R1 = FREE
      R2 = D
      R3 = X1C2
      R4 = C3

      Instructions to be executed:

      1.  Select R0, R3
      2.  R0 = R0 - R3 = D2 - C3 - X1C2
      3.  R0 = R0 - R3 = D2 - C3 - X1C2 - X1C2 = R->X
      4.  DMA = R0 = R->X
      5.  R1 = R0 = R->X

      6.  Select R3, R1
      7.  R0 = R3 - R1 = X1C2 - R->X
      8.  R1 = R0 = X1C2 - R->X
      9.  Select R1, R2
      10. R0 = R1 * R2 = D *(X1C2 - R->X)
      11. R2 = R0

      12. R0 = DMA = Py
      13. R1 = R0 = Py
      14. Select R1, R4
      15. R0 = R1 * R4 = Py * C3 = Y1C3
      16. R4 = R0 = Y1C3

      17. Select R2, R4
      18. R0 = R2 - R4

      Read Operations:

      R->X = R2 = D2 - (C3 + 2 * X1C2)
      R->Y = R0 = D * (X1C2 - R->X) - Y1C3

      STEP 4:
    */

    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA3,
                      CRYPTO_CMD_INSTR_MSUB,
                      CRYPTO_CMD_INSTR_MSUB);
    CRYPTO_InstructionSequenceWait(crypto);

    ret = ecp_crypto_ddata_read(&crypto->DDATA0, &R->X);
    if ( ret != 0 ) goto cleanup;

    CRYPTO_EXECUTE_7 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,

                      CRYPTO_CMD_INSTR_SELDDATA3DDATA1,
                      CRYPTO_CMD_INSTR_MSUB,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA2);
    CRYPTO_InstructionSequenceWait(crypto);

    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA0, &P->Y);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_6 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA4,

                      CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                      CRYPTO_CMD_INSTR_MSUB
                      );
    CRYPTO_InstructionSequenceWait(crypto);

    ret = ecp_crypto_ddata_read(&crypto->DDATA0, &R->Y);
    if ( ret != 0 ) goto cleanup;

 cleanup:
    crypto_management_release( crypto );
    return ( ret );
}
#endif

/**
 * \brief           Point doubling R = 2 P, Jacobian coordinates.
 *
 *                  Cost:   1D := 3M + 4S    (A ==  0)
 *                          4M + 4S          (A == -3)
 *                          3M + 6S + 1a     otherwise
 *                  when the implementation is based on the "dbl-1998-cmo-2"
 *                  doubling formulas in [8] and standard optimizations are
 *                  applied when curve parameter A is one of { 0, -3 }.
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param R         Pointer to a point structure to hold the result.
 *
 * \param P         Pointer to the point that has to be doubled, given with
 *                  Jacobian coordinates.
 *
 * \return          0 if successful.
 */
#if defined(MBEDTLS_ECP_DOUBLE_JAC_ALT)
int mbedtls_internal_ecp_double_jac( const mbedtls_ecp_group *grp,
                                     mbedtls_ecp_point *R,
                                     const mbedtls_ecp_point *P )
{
    int             ret;
    CORE_DECLARE_IRQ_STATE;
    CRYPTO_TypeDef *crypto = crypto_management_acquire();

    crypto_device_init(crypto, grp);

    ecc_bigint_t _2YY;
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
    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA0, &P->Z);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_5 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3);
    CRYPTO_InstructionSequenceWait(crypto);

    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA0, &P->Y);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_4 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                      CRYPTO_CMD_INSTR_MADD);
    CRYPTO_InstructionSequenceWait(crypto);

    ret = ecp_crypto_ddata_read(&crypto->DDATA0, &R->Z);
    if ( ret != 0 ) goto cleanup;

    CRYPTO_EXECUTE_5 (crypto,
                      CRYPTO_CMD_INSTR_DDATA2TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                      CRYPTO_CMD_INSTR_MADD
                      );
    CRYPTO_InstructionSequenceWait(crypto);

    CORE_ENTER_CRITICAL();
    ecp_crypto_ddata_write(&crypto->DDATA4, &P->X);
    CRYPTO_DDataRead(&crypto->DDATA0, _2YY);
    CORE_EXIT_CRITICAL();

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
    CRYPTO_EXECUTE_11(crypto,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA0,
                      CRYPTO_CMD_INSTR_MADD,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA4DDATA3,
                      CRYPTO_CMD_INSTR_MADD,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_MSUB,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA4);
    CRYPTO_InstructionSequenceWait(crypto);
    CRYPTO_EXECUTE_7 (crypto,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA1,
                      CRYPTO_CMD_INSTR_MADD,
                      CRYPTO_CMD_INSTR_MADD,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1
                      );
    CRYPTO_InstructionSequenceWait(crypto);
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
      R0   = FREE
      R1   = FREE
      R2   = FREE
      R3   = D
      R4   = FREE

      STEP 3:
    */

    CRYPTO_EXECUTE_8 (crypto,
                      CRYPTO_CMD_INSTR_SELDDATA2DDATA2,
                      CRYPTO_CMD_INSTR_MADD,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA4,

                      CRYPTO_CMD_INSTR_DDATA1TODDATA3,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                      CRYPTO_CMD_INSTR_MMUL,

                      CRYPTO_CMD_INSTR_SELDDATA0DDATA4,
                      CRYPTO_CMD_INSTR_MSUB);
    CRYPTO_InstructionSequenceWait(crypto);

    ret = ecp_crypto_ddata_read(&crypto->DDATA0, &R->X);
    if ( ret != 0 ) goto cleanup;

    CRYPTO_EXECUTE_7 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA4,

                      CRYPTO_CMD_INSTR_SELDDATA2DDATA4,
                      CRYPTO_CMD_INSTR_MSUB,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3
                      );
    CRYPTO_InstructionSequenceWait(crypto);


    /*
      STEP 4:

      Goals:
      B    = 8 * Y^4
      R->Y = D - B

      Write Operations:

      R1 = YY

      Input State:
      R0   = FREE
      R1   = YY
      R2   = FREE
      R3   = D
      R4   = FREE

      Instructions to be executed:

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
    CORE_ENTER_CRITICAL();
    CRYPTO_DDataWrite(&crypto->DDATA0, _2YY);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_9 (crypto,
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
    CRYPTO_InstructionSequenceWait(crypto);

    ret = ecp_crypto_ddata_read(&crypto->DDATA0, &R->Y);
    if ( ret != 0 ) goto cleanup;

 cleanup:
    crypto_management_release( crypto );

    return ( ret );
}
#endif

/**
 * \brief           Normalize jacobian coordinates of an array of (pointers to)
 *                  points.
 *
 *                  Using Montgomery's trick to perform only one inversion mod P
 *                  the cost is:
 *                      1N(t) := 1I + (6t - 3)M + 1S
 *                  (See for example Algorithm 10.3.4. in [9])
 *
 *                  This function is used only as a subrutine of
 *                  ecp_mul_comb().
 *
 *                  Warning: fails (returning an error) if one of the points is
 *                  zero!
 *                  This should never happen, see choice of w in ecp_mul_comb().
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param T         Array of pointers to the points to normalise.
 *
 * \param t_len     Number of elements in the array.
 *
 * \return          0 if successful,
 *                      an error if one of the points is zero.
 */
#if defined(MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT)
int mbedtls_internal_ecp_normalize_jac_many( const mbedtls_ecp_group *grp,
                                             mbedtls_ecp_point *T[],
                                             size_t t_len )
{
    int ret = 0;
    size_t i;
    ecc_bigint_t*   cc;
    ecc_bigint_t    uu;
    ecc_bigint_t    one;
    ecc_bigint_t    modulus;
    CORE_DECLARE_IRQ_STATE;

    if( t_len < 2 )
        return( mbedtls_internal_ecp_normalize_jac( grp, *T ) );

    if( ( cc = mbedtls_calloc( t_len, sizeof( ecc_bigint_t ) ) ) == NULL )
        return( MBEDTLS_ERR_ECP_ALLOC_FAILED );

    /*
     * c[i] = Z_0 * ... * Z_i
     */
    MPI_TO_BIGINT( cc[0], &T[0]->Z );

    CRYPTO_TypeDef *crypto = crypto_management_acquire();
    crypto_device_init(crypto, grp);

    for( i = 1; i < t_len; i++ )
    {
        CORE_ENTER_CRITICAL();
        ecp_crypto_ddata_write( &crypto->DDATA1, &T[i]->Z );
        CRYPTO_DDataWrite( &crypto->DDATA2, cc[i-1] );
        CORE_EXIT_CRITICAL();

        CRYPTO_EXECUTE_2(crypto,
                         CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                         CRYPTO_CMD_INSTR_MMUL);
        CRYPTO_InstructionSequenceWait(crypto);

        CORE_ENTER_CRITICAL();
        CRYPTO_DDataRead( &crypto->DDATA0, cc[i] );
        CORE_EXIT_CRITICAL();
    }

    memset(one, 0, sizeof(one));
    one[0]=1;
    MPI_TO_BIGINT( modulus, &grp->P );

    /*
     * u = 1 / (Z_0 * ... * Z_n) mod P
     */
    crypto_mpi_div_mod(crypto, one, cc[t_len-1], modulus, uu);

    for( i = t_len - 1; ; i-- )
    {
        /*
         * Zi = 1 / Z_i mod p
         * u = 1 / (Z_0 * ... * Z_i) mod P
         */
        if( i == 0 )
        {
            /* Z_inv (DDATA2) = uu */
            CORE_ENTER_CRITICAL();
            CRYPTO_DDataWrite(&crypto->DDATA2, uu);
            CORE_EXIT_CRITICAL();
        }
        else
        {
            /* Z_inv (DDATA1) = uu x cc[i-1] modulo p */
            /* uu = uu x T[i]->Z modulo p */
            CORE_ENTER_CRITICAL();
            CRYPTO_DDataWrite(&crypto->DDATA1, uu);
            CRYPTO_DDataWrite(&crypto->DDATA2, cc[i-1]);
            ecp_crypto_ddata_write( &crypto->DDATA3, &T[i]->Z );
            CORE_EXIT_CRITICAL();

            CRYPTO_EXECUTE_3(crypto,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                             CRYPTO_CMD_INSTR_MMUL,
                             CRYPTO_CMD_INSTR_DDATA0TODDATA2); /* Z_inv (DDATA2) */
            CRYPTO_InstructionSequenceWait(crypto);
            CRYPTO_EXECUTE_2(crypto,
                             CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                             CRYPTO_CMD_INSTR_MMUL);
            CRYPTO_InstructionSequenceWait(crypto);

            CORE_ENTER_CRITICAL();
            CRYPTO_DDataRead(&crypto->DDATA0, uu);
            CORE_EXIT_CRITICAL();
        }

        /*
         * proceed as in normalize()
         */
        CORE_ENTER_CRITICAL();
        ecp_crypto_ddata_write(&crypto->DDATA3, &T[i]->X);
        ecp_crypto_ddata_write(&crypto->DDATA4, &T[i]->Y);
        CORE_EXIT_CRITICAL();

        /* Z_inv  already in DDATA2 */
        CRYPTO_EXECUTE_3 (crypto,
                          CRYPTO_CMD_INSTR_DDATA2TODDATA1,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                          CRYPTO_CMD_INSTR_MMUL);
        CRYPTO_InstructionSequenceWait(crypto);
        CRYPTO_EXECUTE_3 (crypto,
                          CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                          CRYPTO_CMD_INSTR_MMUL);
        CRYPTO_InstructionSequenceWait(crypto);
        CRYPTO_EXECUTE_3 (crypto,
                          CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                          CRYPTO_CMD_INSTR_MMUL);
        CRYPTO_InstructionSequenceWait(crypto);
        CRYPTO_EXECUTE_3 (crypto,
                          CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                          CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                          CRYPTO_CMD_INSTR_MMUL);
        CRYPTO_InstructionSequenceWait(crypto);

        ecp_crypto_ddata_read(&crypto->DDATA0, &T[i]->Y);
        ecp_crypto_ddata_read(&crypto->DDATA3, &T[i]->X);

        /*
         * Post-precessing: reclaim some memory by shrinking coordinates
         * - not storing Z (always 1)
         * - shrinking other coordinates, but still keeping the same number of
         *   limbs as P, as otherwise it will too likely be regrown too fast.
         */
        SLCL_ECP_CHK( mbedtls_mpi_shrink( &T[i]->X, grp->P.n ) );
        SLCL_ECP_CHK( mbedtls_mpi_shrink( &T[i]->Y, grp->P.n ) );
        mbedtls_mpi_free( &T[i]->Z );

        if( i == 0 )
            break;
    }

 cleanup:
    crypto_management_release( crypto );
    mbedtls_free( cc );

    return( ret );
}
#endif

/**
 * \brief           Normalize jacobian coordinates so that Z == 0 || Z == 1.
 *
 *                  Cost in field operations if done by [5] 3.2.1:
 *                      1N := 1I + 3M + 1S
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param pt        pointer to the point to be normalised. This is an
 *                  input/output parameter.
 *
 * \return          0 if successful.
 */
#if defined(MBEDTLS_ECP_NORMALIZE_JAC_ALT)
int mbedtls_internal_ecp_normalize_jac( const mbedtls_ecp_group *grp,
                                        mbedtls_ecp_point *pt )
{
    int             ret      = 0;
    CORE_DECLARE_IRQ_STATE;
    CRYPTO_TypeDef *crypto = crypto_management_acquire();

    crypto_device_init(crypto, grp);

    ecc_bigint_t    one;
    ecc_bigint_t    Z;
    ecc_bigint_t    modulus;
    ecc_bigint_t    Z_inv;

    memset(one, 0, sizeof(one));
    one[0]=1;

    MPI_TO_BIGINT( Z, &pt->Z );
    MPI_TO_BIGINT( modulus, &grp->P );

    crypto_mpi_div_mod(crypto, one, Z, modulus, Z_inv);

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
    CORE_ENTER_CRITICAL();
    CRYPTO_DDataWrite(&crypto->DDATA1, Z_inv);
    ecp_crypto_ddata_write(&crypto->DDATA3, &pt->X);
    ecp_crypto_ddata_write(&crypto->DDATA4, &pt->Y);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_DDATA1TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL);
    CRYPTO_InstructionSequenceWait(crypto);
    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA3,
                      CRYPTO_CMD_INSTR_MMUL);
    CRYPTO_InstructionSequenceWait(crypto);
    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA3,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA2,
                      CRYPTO_CMD_INSTR_MMUL);
    CRYPTO_InstructionSequenceWait(crypto);
    CRYPTO_EXECUTE_3 (crypto,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA1DDATA4,
                      CRYPTO_CMD_INSTR_MMUL);
    CRYPTO_InstructionSequenceWait(crypto);

    ecp_crypto_ddata_read(&crypto->DDATA0, &pt->Y);
    ecp_crypto_ddata_read(&crypto->DDATA3, &pt->X);

    crypto_management_release( crypto );

    /*
     * Z = 1
     */
    SLCL_ECP_CHK( mbedtls_mpi_lset( &pt->Z, 1 ) );

 cleanup:
    return( ret );
}
#endif

#endif /* #if defined( MBEDTLS_ECP_INTERNAL_ALT ) */

#endif /* #if defined( MBEDTLS_ECP_C ) */

#endif /* #if defined( CRYPTO_PRESENT ) */
