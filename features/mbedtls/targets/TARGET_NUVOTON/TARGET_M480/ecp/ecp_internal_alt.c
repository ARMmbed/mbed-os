/*
 * Copyright (c) 2016-2018, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* Some internal functions are used for Nuvoton internal self-test.
 * Remove the static modifier for self-test compile. */
#if defined(NU_CRYPTO_SELF_TEST) && NU_CRYPTO_SELF_TEST
#define NU_STATIC
#else
#define NU_STATIC static
#endif

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/threading.h"

#include <string.h>

#if !defined(MBEDTLS_ECP_ALT)

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#include <stdio.h>
#define mbedtls_printf     printf
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#if defined(MBEDTLS_ECP_INTERNAL_ALT)

/* FIXME: We shouldn't define ECP_SHORTWEIERSTRASS here. It is expected ECP_SHORTWEIERSTRASS 
 *        would be defined in mbedtls/ecp.h from ecp.c for our inclusion */
#define ECP_SHORTWEIERSTRASS

#include "mbedtls/platform.h"
#include "mbedtls/ecp_internal.h"
#include "mbed_toolchain.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_toolchain.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"

/* Max key size supported */
#define NU_ECC_MAXKEYBITS           571
/* Max ECC big-number words */
#define NU_ECC_BIGNUM_MAXWORD       18
/* words in limb  */
#define wiL                         (sizeof (mbedtls_mpi_uint) / sizeof (uint32_t))
/* Min MPI limbs for ECC big-number */
#define NU_ECC_BIGNUM_MINLIMB       (NU_ECC_BIGNUM_MAXWORD / wiL)

/*
 * Convert between words and number of limbs
 * Divide first in order to avoid potential overflows
 */
#define WORDS_TO_LIMBS(i) ( (i) / wiL + ( (i) % wiL != 0 ) )


#define ECCOP_POINT_MUL     (0x0UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_MODULE        (0x1UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_ADD     (0x2UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_DOUBLE  (0x3UL << CRPT_ECC_CTL_ECCOP_Pos)

#define MODOP_DIV           (0x0UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_MUL           (0x1UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_ADD           (0x2UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_SUB           (0x3UL << CRPT_ECC_CTL_MODOP_Pos)

/**
 * \brief           Check if MPI has been normalized
 *
 * \param N         Input MPI which is to check
 * \param P         Prime modulus
 *
 * \return          0 if not normalized,
 *                  1 if normalized
 */
#define INTERNAL_MPI_IS_NORM(N, P)                          \
    ((mbedtls_mpi_cmp_int(&N, 0) >= 0) && (mbedtls_mpi_cmp_mpi(&N, &P) < 0))
  
        
/**
 * \brief           Normalize MPI if it is not normalized yet
 *
 * \param R         Holds pointer to normalized MPI (N1 or N2)
 * \param N1        Input MPI which is to normalize
 * \param N2        Output MPI which holds normalized N1 if N1 is not normalized yet
 * \param P         Prime modulus
 */
#define INTERNAL_MPI_NORM(R, N1, N2, P)                     \
    do {                                                            \
        if (INTERNAL_MPI_IS_NORM(N1, P)) {                  \
            *R = &N1;                                               \
        } else {                                                    \
            MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&N2, &N1, &P));     \
            *R = &N2;                                               \
        }                                                           \
    } while(0)

/**
 * \brief           Configure ECCOP operation, start it, and wait for its completion
 *
 * \param grp       ECP group
 * \param R         Destination point
 * \param m         Integer by which to multiply P
 * \param P         Point to multiply by m
 * \param n         Integer by which to multiply Q
 * \param Q         Point to be multiplied by n
 * \param eccop     ECCOP code. Could be ECCOP_POINT_MUL/ADD/DOUBLE
 *                  Dependent on passed-in eccop, only partial parameters among m/P/n/Q are needed and checked.
 *                  ECCOP_POINT_MUL     R = m*P
 *                  ECCOP_POINT_ADD     R = P + Q
 *                  ECCOP_POINT_DOUBLE  R = 2*P
 *
 * \return          0 if successful
 *
 * \note            P/Q must be normalized (= affine). R would be normalized.
 *
 * \note            m/n could be negative.
 *
 * \note            ECC accelerator doesn't support R = 0, and we need to detect it additionally.
 *                  For R = P + Q or R = 2*P, we can detect all R = 0 cases.
 *                  For R = m*P, we can detect all R = 0 cases only if grp->N (order) is a prime.
 *
 * \note            According to ECCOP operation, n is unnecessary. But to be consistent with R = m*P + n*Q,
 *                  n is kept with unused modifier.
 *                  
 */
NU_STATIC int internal_run_eccop(const mbedtls_ecp_group *grp,
                                    mbedtls_ecp_point *R,
                                    const mbedtls_mpi *m,
                                    const mbedtls_ecp_point *P,
                                    MBED_UNUSED const mbedtls_mpi *n,
                                    const mbedtls_ecp_point *Q,
                                    uint32_t eccop);

/**
 * \brief           Configure MODOP operation and wait for its completion
 *
 * \param r         Destination MPI
 * \param o1        Input MPI for first operand of MODOP
 * \param o2        Input MPI for second operand of MODOP
 * \param p         Prime modulus
 * \param pbits     Bit number of p
 * \param modop     ECCOP code. Could be MODOP_ADD/SUB/MUL/DIV
 *                  MODOP_ADD       r = o1 + o2 mod p
 *                  MODOP_SUB       r = o1 - o2 mod p
 *                  MODOP_MUL       r = o1 * o2 mod p
 *                  MODOP_DIV       r = o1 / o2 mod p
 *
 * \return          0 if successful
 *
 * \note            o1/o2 must be normalized (within [0, p - 1]). r would be normalized.
 */
NU_STATIC int internal_run_modop(mbedtls_mpi *r,
                                    const mbedtls_mpi *o1,
                                    const mbedtls_mpi *o2,
                                    const mbedtls_mpi *p,
                                    uint32_t pbits,
                                    uint32_t modop);

/**
 * \brief               Import X from ECC registers, little endian
 *
 * \param X             Destination MPI
 * \param eccreg        Start of input ECC register
 * \param eccreg_num    Number of input ECC register
 *
 * \return              0 if successful
 *
 * \note                Destination MPI is always non-negative.
 */
NU_STATIC int internal_mpi_read_eccreg( mbedtls_mpi *X, const volatile uint32_t *eccreg, size_t eccreg_num );

/**
 * \brief               Export X into ECC registers, little endian
 *
 * \param X             Source MPI
 * \param eccreg        Start of ECC output registers
 * \param eccreg_num    Number of ECC output registers
 *
 * \return              0 if successful
 *
 * \note                Source MPI cannot be negative.
 * \note                Fills the remaining MSB ECC registers with zeros if X doesn't cover all.
 */
NU_STATIC int internal_mpi_write_eccreg( const mbedtls_mpi *X, volatile uint32_t *eccreg, size_t eccreg_num );
                            
unsigned char mbedtls_internal_ecp_grp_capable( const mbedtls_ecp_group *grp )
{
    /* Support only short Weierstrass type
     * 
     * ECP type is checked by referring to mbed-os/features/mbedtls/src/ecp.c > ecp_get_type
     */
    if (grp->G.X.p == NULL || grp->G.Y.p == NULL) {
        return 0;
    }

    return 1;
}

int mbedtls_internal_ecp_init( const mbedtls_ecp_group *grp )
{
    /* Behavior of mbedtls_internal_ecp_init()/mbedtls_internal_ecp_free()
     *
     * mbedtls_internal_ecp_init()/mbedtls_internal_ecp_free() are like pre-op/post-op calls
     * and they guarantee:
     *
     * 1. Paired
     * 2. No overlapping
     * 3. Upper public function cannot return when ECP alter. is still activated.
     */
    
    /* Acquire ownership of ECC accelerator */
    crypto_ecc_acquire();
    
    /* Initialize crypto module */
    crypto_init();
    
    /* Enable ECC interrupt */
    ECC_ENABLE_INT(CRPT);

    return 0;
}

void mbedtls_internal_ecp_free( const mbedtls_ecp_group *grp )
{
    /* Disable ECC interrupt */
    ECC_DISABLE_INT(CRPT);

    /* Uninit crypto module */
    crypto_uninit();

    /* Release ownership of ECC accelerator */
    crypto_ecc_release();
}

#if defined(ECP_SHORTWEIERSTRASS)

#if defined(MBEDTLS_ECP_ADD_MIXED_ALT)
/**
 * \brief           Addition: R = P + Q, mixed affine-Jacobian coordinates.
 *
 *                  The coordinates of Q must be normalized (= affine),
 *                  but those of P don't need to. R is not normalized.
 *
 *                  We accept Q->Z being unset (saving memory in tables) as
 *                  meaning 1.
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
        mbedtls_ecp_point *R, const mbedtls_ecp_point *P,
        const mbedtls_ecp_point *Q )
{
    int ret;
    mbedtls_ecp_point P_, Q_;
    
    mbedtls_ecp_point_init(&P_);
    mbedtls_ecp_point_init(&Q_);
    
    /* P_ = normalized P */
    MBEDTLS_MPI_CHK(mbedtls_ecp_copy(&P_, P));
    MBEDTLS_MPI_CHK(mbedtls_internal_ecp_normalize_jac(grp, &P_));
    
    /* Q_ = normalized Q */
    MBEDTLS_MPI_CHK(mbedtls_ecp_copy(&Q_, Q));
    /* NOTE: We accept Q->Z being unset (saving memory in tables) as meaning 1.
     *
     * Q->Z.p == NULL                                           ==> Q->Z = 1
     * Q->Z.p != NULL && mbedtls_mpi_cmp_int(&Q->Z, 0) == 0     ==> Q->Z = 0
     * Q->Z.p != NULL && mbedtls_mpi_cmp_int(&Q->Z, 0) != 0     ==> Q->Z = 1
     */
    if (Q->Z.p != NULL && mbedtls_mpi_cmp_int(&Q->Z, 0) == 0) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&Q_.Z, 0));
    } else {
        MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&Q_.Z, 1));
    }
    
    /* Run ECC point addition: R = P + Q */
    MBEDTLS_MPI_CHK(internal_run_eccop(grp, R, NULL, &P_, NULL, &Q_, ECCOP_POINT_ADD));
    
cleanup:

    mbedtls_ecp_point_free(&Q_);
    mbedtls_ecp_point_free(&P_);

    return ret;
}

#endif

#if defined(MBEDTLS_ECP_DOUBLE_JAC_ALT)
/**
 * \brief           Point doubling R = 2 P, Jacobian coordinates.
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
int mbedtls_internal_ecp_double_jac( const mbedtls_ecp_group *grp,
        mbedtls_ecp_point *R, const mbedtls_ecp_point *P )
{
    int ret;
    mbedtls_ecp_point P_;

    mbedtls_ecp_point_init(&P_);
    
    /* P_ = normalized P */
    MBEDTLS_MPI_CHK(mbedtls_ecp_copy(&P_, P));
    MBEDTLS_MPI_CHK(mbedtls_internal_ecp_normalize_jac(grp, &P_));
    
    /* Run ECC point doubling: R = 2*P */
    MBEDTLS_MPI_CHK(internal_run_eccop(grp, R, NULL, &P_, NULL, NULL, ECCOP_POINT_DOUBLE));
    
cleanup:

    mbedtls_ecp_point_free(&P_);

    return ret;
}
#endif
             
#if defined(MBEDTLS_ECP_NORMALIZE_JAC_ALT)
/**
 * \brief           Normalize jacobian coordinates so that Z == 0 || Z == 1.
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param pt        pointer to the point to be normalised. This is an
 *                  input/output parameter.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_normalize_jac( const mbedtls_ecp_group *grp,
        mbedtls_ecp_point *pt )
{
    if (grp == NULL || pt == NULL) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    
    /* Is a zero point
     *
     * Z = 0
     */
    if (mbedtls_mpi_cmp_int(&pt->Z, 0) == 0) {
        return 0;
    }
    
    /* Is a non-zero point which has been normalized
     *
     * Z = 1
     * 0 <= X < P
     * 0 <= y < P 
     */
    if (mbedtls_mpi_cmp_int(&pt->Z, 1) == 0 &&
        mbedtls_mpi_cmp_int(&pt->X, 0) >= 0 &&
        mbedtls_mpi_cmp_mpi(&pt->X, &grp->P) < 0 &&
        mbedtls_mpi_cmp_int(&pt->Y, 0) >= 0 &&
        mbedtls_mpi_cmp_mpi(&pt->Y, &grp->P) < 0) {
        return 0;
    }

    int ret;
    mbedtls_mpi N, Zi, ZZi;
    const mbedtls_mpi *Np;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&Zi);
    mbedtls_mpi_init(&ZZi);

    /* Use INTERNAL_MPI_NORM(Np, N1, N_, P) to get normalized MPI
     *
     * N_: Holds normalized MPI if the passed-in MPI N1 is not
     * Np: Pointer to normalized MPI, which could be N1 or N_
     */

    /* Zi = 1 / Z */
    mbedtls_mpi_lset(&Zi, 1);
    INTERNAL_MPI_NORM(&Np, pt->Z, N, grp->P);
    MBEDTLS_MPI_CHK(internal_run_modop(&Zi, &Zi, Np, &grp->P, grp->pbits, MODOP_DIV));
    
    /* ZZi = 1 / Z^2 = Zi * Zi */
    MBEDTLS_MPI_CHK(internal_run_modop(&ZZi, &Zi, &Zi, &grp->P, grp->pbits, MODOP_MUL));
    
    /* X = X / Z^2 = X * ZZi */
    INTERNAL_MPI_NORM(&Np, pt->X, N, grp->P);
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->X, Np, &ZZi, &grp->P, grp->pbits, MODOP_MUL));
    
    /* Y = Y / Z^3 = Y * ZZi * Zi */
    INTERNAL_MPI_NORM(&Np, pt->Y, N, grp->P);
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->Y, Np, &ZZi, &grp->P, grp->pbits, MODOP_MUL));
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->Y, &pt->Y, &Zi, &grp->P, grp->pbits, MODOP_MUL));
    
    /* Z = 1 */
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->Z, 1));

cleanup:

    mbedtls_mpi_free(&ZZi);
    mbedtls_mpi_free(&Zi);
    mbedtls_mpi_free(&N);

    return ret;
}
#endif

#if defined(MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT)
/**
 * \brief           Normalize jacobian coordinates of an array of (pointers to)
 *                  points.
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param T         Array of pointers to the points to normalise.
 *
 * \param t_len     Number of elements in the array.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_normalize_jac_many(const mbedtls_ecp_group *grp,
                                            mbedtls_ecp_point *T[], size_t t_len)
{
    if (T == NULL || t_len == 0) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    
    int ret;
    
    mbedtls_ecp_point **ecp_point = T;
    mbedtls_ecp_point **ecp_point_end = T + t_len;
    
    for (; ecp_point != ecp_point_end; ecp_point ++) {
        MBEDTLS_MPI_CHK(mbedtls_internal_ecp_normalize_jac(grp, *ecp_point));
    }

cleanup:

    return ret;
}
#endif

NU_STATIC int internal_run_eccop(const mbedtls_ecp_group *grp,
                                    mbedtls_ecp_point *R,
                                    const mbedtls_mpi *m,
                                    const mbedtls_ecp_point *P,
                                    MBED_UNUSED const mbedtls_mpi *n,
                                    const mbedtls_ecp_point *Q,
                                    uint32_t eccop)
{
    /* Check necessary arguments for all ECC operations */
    if (grp == NULL || R == NULL) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    
    /* Check grp->P is positive */
    if (mbedtls_mpi_cmp_int(&grp->P, 0) <= 0) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    
    /* Check supported maximum key bits */
    if (grp->pbits > NU_ECC_MAXKEYBITS) {
        return MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
    }

    int ret;
    bool ecc_done;

    mbedtls_mpi N_;
    const mbedtls_mpi *Np;
    
    mbedtls_mpi_init(&N_);
    
    /* Use INTERNAL_MPI_NORM(Np, N1, N_, P) to get normalized MPI
     *
     * N_: Holds normalized MPI if the passed-in MPI N1 is not
     * Np: Pointer to normalized MPI, which could be N1 or N_
     */
    
    /* Check necessary arguments and handle special cases for specified ECC operation 
     *
     * ECCOP_POINT_MUL      R = m*P
     * ECCOP_POINT_ADD      R = P + Q
     * ECCOP_POINT_DOUBLE   R = 2*P
     *
     * ECC accelerator doesn't support R = 0, and we need to detect it.
     */
    if (eccop == ECCOP_POINT_MUL) {
        /* R = m*P */
        if (m == NULL || P == NULL) {
            ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
            goto cleanup;
        }
        
         /* R = 0*P = 0 or R = P = 0 */
        if (mbedtls_mpi_cmp_int(m, 0) == 0 || mbedtls_mpi_cmp_int(&P->Z, 0) == 0) {
            ret = mbedtls_ecp_set_zero(R);
            goto cleanup;
        }
        
        /* R = 1*P */
        if (mbedtls_mpi_cmp_int(m, 1) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, P));
            MBEDTLS_MPI_CHK(mbedtls_internal_ecp_normalize_jac(grp, R));
            goto cleanup;
        }
        
        /* R = m*P = (multiple of order)*G = 0 */
        /* NOTE: If grp->N (order) is a prime, we could detect R = 0 for all m*P cases
         *       by just checking if m is a multiple of grp->N. Otherwise, sigh. */
        /* TODO: Find an approach to detecting R = 0 for all m*P cases */
        INTERNAL_MPI_NORM(&Np, *m, N_, grp->N);
        if (mbedtls_mpi_cmp_int(Np, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_set_zero(R));
            goto cleanup;
        }
        
    } else if (eccop == ECCOP_POINT_ADD) {
        /* R = P + Q */
        if (P == NULL || Q == NULL) {
            ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
            goto cleanup;
        }
        
        /* R = 0 + Q = Q */
        if (mbedtls_mpi_cmp_int(&P->Z, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, Q));
            MBEDTLS_MPI_CHK(mbedtls_internal_ecp_normalize_jac(grp, R));
            goto cleanup;
        }
        
        /* R = P + 0 = P */
        if (mbedtls_mpi_cmp_int(&Q->Z, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, P));
            MBEDTLS_MPI_CHK(mbedtls_internal_ecp_normalize_jac(grp, R));
            goto cleanup;
        }
        
        /* R = P + Q = P + (-P) = 0 */
        MBEDTLS_MPI_CHK(internal_run_modop(&N_, &P->Y, &Q->Y, &grp->P, grp->pbits, MODOP_ADD));
        if (mbedtls_mpi_cmp_int(&N_, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_set_zero(R));
            goto cleanup;
        }
    } else if (eccop == ECCOP_POINT_DOUBLE) {
        /* R = 2*P */
        if (P == NULL) {
            ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
            goto cleanup;
        }
        
        /* R = 2*0 = 0 */
        if (mbedtls_mpi_cmp_int(&P->Z, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_set_zero(R));
            goto cleanup;
        }
        
        /* R = 2*P = P + P = P + (-P) = 0 */
        MBEDTLS_MPI_CHK(internal_run_modop(&N_, &P->Y, &P->Y, &grp->P, grp->pbits, MODOP_ADD));
        if (mbedtls_mpi_cmp_int(&N_, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_set_zero(R));
            goto cleanup;
        }
    } else {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    /* Configure ECC curve coefficients A/B */
    /* Special case for A = -3 */
    if (grp->A.p == NULL) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&N_, -3));
        INTERNAL_MPI_NORM(&Np, N_, N_, grp->P);
    } else {
        INTERNAL_MPI_NORM(&Np, grp->A, N_, grp->P);
    }
    MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_A, NU_ECC_BIGNUM_MAXWORD));
    INTERNAL_MPI_NORM(&Np, grp->B, N_, grp->P);
    MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_B, NU_ECC_BIGNUM_MAXWORD));
    
    /* Configure ECC prime modulus */ 
    MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(&grp->P, (uint32_t *) CRPT->ECC_N, NU_ECC_BIGNUM_MAXWORD));
    
    /* Configure ECC scalar for point multiplication 
     *
     * Normalize m to within [1, order - 1] which ECCOP_POINT_MUL supports
     * Special cases R = 0 should have been detected out above.
     */
    if (eccop == ECCOP_POINT_MUL) {
        INTERNAL_MPI_NORM(&Np, *m, N_, grp->N);
        MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_K, NU_ECC_BIGNUM_MAXWORD));
    }
    
    /* Configure ECC point (X1, Y1) */
    INTERNAL_MPI_NORM(&Np, P->X, N_, grp->P);
    MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
    INTERNAL_MPI_NORM(&Np, P->Y, N_, grp->P);
    MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));

    /* Configure ECC points (X2, Y2) */
    if (eccop == ECCOP_POINT_ADD) {
        INTERNAL_MPI_NORM(&Np, Q->X, N_, grp->P);
        MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X2, NU_ECC_BIGNUM_MAXWORD));
        INTERNAL_MPI_NORM(&Np, Q->Y, N_, grp->P);
        MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y2, NU_ECC_BIGNUM_MAXWORD));
    }

    crypto_ecc_prestart();
    CRPT->ECC_CTL = (grp->pbits << CRPT_ECC_CTL_CURVEM_Pos) | eccop | CRPT_ECC_CTL_FSEL_Msk | CRPT_ECC_CTL_START_Msk;
    ecc_done = crypto_ecc_wait();

    MBEDTLS_MPI_CHK(ecc_done ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);

    /* (X1, Y1) hold the normalized result. */
    MBEDTLS_MPI_CHK(internal_mpi_read_eccreg(&R->X, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
    MBEDTLS_MPI_CHK(internal_mpi_read_eccreg(&R->Y, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&R->Z, 1));
        
cleanup:

    mbedtls_mpi_free(&N_);

    return ret;
}

NU_STATIC int internal_run_modop(mbedtls_mpi *r,
                                    const mbedtls_mpi *o1,
                                    const mbedtls_mpi *o2,
                                    const mbedtls_mpi *p,
                                    uint32_t pbits,
                                    uint32_t modop)
{
    if (r == NULL || 
        o1 == NULL ||
        o2 == NULL ||
        p == NULL) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    
    /* Check o1/o2 are not negative */
    if (mbedtls_mpi_cmp_int(o1, 0) < 0 ||
        mbedtls_mpi_cmp_int(o2, 0) < 0) {
        return MBEDTLS_ERR_MPI_NEGATIVE_VALUE;
    }
    
    /* Check p is positive */
    if (mbedtls_mpi_cmp_int(p, 0) <= 0) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    
    /* Check supported maximum key bits */
    if (pbits > NU_ECC_MAXKEYBITS) {
        return MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
    }
    
    /* Check MODOP operations are legal */
    if (modop != MODOP_DIV &&
        modop != MODOP_MUL &&
        modop != MODOP_ADD &&
        modop != MODOP_SUB) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    
    /* Check divisor is not zero in MODOP_DIV operation */
    if (modop == MODOP_DIV && mbedtls_mpi_cmp_int(o2, 0) == 0) {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }

    int ret;
    bool ecc_done;

    mbedtls_mpi N_;
    const mbedtls_mpi *Np;
    
    mbedtls_mpi_init(&N_);

    /* Use INTERNAL_MPI_NORM(Np, N1, N_, P) to get normalized MPI
     *
     * N_: Holds normalized MPI if the passed-in MPI N1 is not
     * Np: Pointer to normalized MPI, which could be N1 or N_
     */

    if (modop == MODOP_MUL ||
        modop == MODOP_ADD ||
        modop == MODOP_SUB) {
        INTERNAL_MPI_NORM(&Np, *o1, N_, *p);
        MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
        INTERNAL_MPI_NORM(&Np, *o2, N_, *p);
        MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));
    } else if (modop == MODOP_DIV) {
        INTERNAL_MPI_NORM(&Np, *o2, N_, *p);
        MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
        INTERNAL_MPI_NORM(&Np, *o1, N_, *p);
        MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));
    } else {
        MBEDTLS_MPI_CHK(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }

    MBEDTLS_MPI_CHK(internal_mpi_write_eccreg(p, (uint32_t *) CRPT->ECC_N, NU_ECC_BIGNUM_MAXWORD));

    crypto_ecc_prestart();
    CRPT->ECC_CTL = (pbits << CRPT_ECC_CTL_CURVEM_Pos) | (ECCOP_MODULE | modop) | CRPT_ECC_CTL_FSEL_Msk | CRPT_ECC_CTL_START_Msk;
    ecc_done = crypto_ecc_wait();

    MBEDTLS_MPI_CHK(ecc_done ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);

    /* X1 holds the result. */
    MBEDTLS_MPI_CHK(internal_mpi_read_eccreg(r, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));

cleanup:
    
    mbedtls_mpi_free(&N_);

    return ret;
}

#endif  // ECP_SHORTWEIERSTRASS

NU_STATIC int internal_mpi_read_eccreg(mbedtls_mpi *x, const volatile uint32_t *eccreg, size_t eccreg_num)
{
    if (x == NULL) {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }
    
    int ret;
    size_t i, n;

    for (n = eccreg_num; n > 0; n --) {
        if (eccreg[n - 1] != 0) {
            break;
        }
    }
    
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(x, 0));
    MBEDTLS_MPI_CHK(mbedtls_mpi_grow(x, WORDS_TO_LIMBS(n)));

    for (i = 0; i < n; i ++) {
        x->p[i / wiL] |= ((mbedtls_mpi_uint) eccreg[i]) << ((i % wiL) << 5);
    }

cleanup:

    return ret;
}

NU_STATIC int internal_mpi_write_eccreg( const mbedtls_mpi *x, volatile uint32_t *eccreg, size_t eccreg_num )
{
    if (x == NULL) {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }
    
    if (mbedtls_mpi_cmp_int(x, 0) < 0) {
        return MBEDTLS_ERR_MPI_NEGATIVE_VALUE;
    }
    
    size_t i, n;

    /* How many words needed? */
    n = (mbedtls_mpi_size(x) + sizeof (uint32_t) - 1) / sizeof (uint32_t);

    if (eccreg_num < n) {
        return MBEDTLS_ERR_MPI_BUFFER_TOO_SMALL;
    }

    /* Fill non-zero part */
    for (i = 0; i < n; i ++) {
        eccreg[i] = (uint32_t) (x->p[i / wiL] >> ((i % wiL) << 5));
    }
    
    /* Zeroize remaining part
     *
     * crypto_zeroize32() has excluded optimization doubt, so we can safely set H/W registers to 0 via it.
     */
    crypto_zeroize32((uint32_t *) eccreg + n, eccreg_num - n);
    
    return 0;
}

#endif /* MBEDTLS_ECP_INTERNAL_ALT */
#endif /* ! MBEDTLS_ECP_ALT */
#endif /* MBEDTLS_ECP_C */
