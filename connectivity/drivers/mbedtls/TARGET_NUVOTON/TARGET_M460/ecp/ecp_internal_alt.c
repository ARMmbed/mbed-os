/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
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

#include "mbedtls/common.h"

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
#include "mbedtls/error.h"

#include <string.h>

#if !defined(MBEDTLS_ECP_ALT)
#if defined(MBEDTLS_ECP_INTERNAL_ALT)

#include "mbedtls/platform.h"
#include "mbedtls/ecp_internal.h"
#include "mbed_toolchain.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_toolchain.h"
#include "nu_bitutil.h"
#include "crypto-misc.h"
#include "crypto_ecc_hw.h"
#include "ecp_helper.h"

/**
 * \brief           Normalize Jacobian coordinates or Montgomery x/z coordinates, dependent on curve type.
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param pt        pointer to the point to be normalized. This is an
 *                  input/output parameter.
 *
 * \return          0 if successful.
 */
NU_STATIC int internal_ecp_normalize(const mbedtls_ecp_group *grp,
                                     mbedtls_ecp_point *pt);

/**
 * \note            See \c crypto_ecc_run_eccop.                  
 */
NU_STATIC int internal_run_eccop(const mbedtls_ecp_group *grp,
                                 mbedtls_ecp_point *R,
                                 const mbedtls_mpi *m,
                                 const mbedtls_ecp_point *P,
                                 const mbedtls_mpi *n,
                                 const mbedtls_ecp_point *Q,
                                 uint32_t eccop);

/**
 * \note            See \c crypto_ecc_run_modop.                  
 */
NU_STATIC int internal_run_modop(mbedtls_mpi *r,
                                 const mbedtls_mpi *o1,
                                 const mbedtls_mpi *o2,
                                 const mbedtls_mpi *p,
                                 uint32_t pbits,
                                 uint32_t modop);

/**
 * \note            See \c crypto_ecc_mpi_read_eccreg.                  
 */
NU_STATIC int internal_mpi_read_eccreg( mbedtls_mpi *X, const volatile uint32_t *eccreg, size_t eccreg_num );

/**
 * \note            See \c crypto_ecc_mpi_write_eccreg.                  
 */
NU_STATIC int internal_mpi_write_eccreg( const mbedtls_mpi *X, volatile uint32_t *eccreg, size_t eccreg_num );

unsigned char mbedtls_internal_ecp_grp_capable( const mbedtls_ecp_group *grp )
{
    /* Curve types
     *
     * - Short Weierstrass
     * - Montgomery
     */
    mbedtls_ecp_curve_type curve_type = mbedtls_ecp_get_type(grp);
    if (curve_type == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS ||
        curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        return 1;
    } else {
        return 0;
    }
}

int mbedtls_internal_ecp_init( const mbedtls_ecp_group *grp )
{
    return crypto_ecc_init( grp );
}

void mbedtls_internal_ecp_free( const mbedtls_ecp_group *grp )
{
    crypto_ecc_free( grp );
}

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)

#if defined(MBEDTLS_ECP_RANDOMIZE_JAC_ALT)
/**
 * \brief           Randomize jacobian coordinates:
 *                  (X, Y, Z) -> (l^2 X, l^3 Y, l Z) for random l
 *                  This is sort of the reverse operation of ecp_normalize_jac().
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param pt        Pointer to a point structure to randomize.
 *
 * \param f_rng     The RNG function. This may be \c NULL if randomization
 *                  of intermediate results isn't desired (discouraged).
 *
 * \param p_rng     The RNG context to be passed to \p p_rng.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_randomize_jac( const mbedtls_ecp_group *grp, mbedtls_ecp_point *pt,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi l, ll;
    size_t p_size;
    int count = 0;
    
    p_size = ( grp->pbits + 7 ) / 8;
    mbedtls_mpi_init( &l ); mbedtls_mpi_init( &ll );

    /* Generate l such that 1 < l < p */
    do
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( &l, p_size, f_rng, p_rng ) );

        while( mbedtls_mpi_cmp_mpi( &l, &grp->P ) >= 0 )
            MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &l, 1 ) );

        if( count++ > 10 )
        {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }
    }
    while( mbedtls_mpi_cmp_int( &l, 1 ) <= 0 );

    /* Z = l * Z */
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->Z, &pt->Z, &l, &grp->P, grp->pbits, MODOP_MUL));

    /* X = l^2 * X */
    MBEDTLS_MPI_CHK(internal_run_modop(&ll, &l, &l, &grp->P, grp->pbits, MODOP_MUL));
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->X, &pt->X, &ll, &grp->P, grp->pbits, MODOP_MUL));

    /* Y = l^3 * Y */
    MBEDTLS_MPI_CHK(internal_run_modop(&ll, &ll, &l, &grp->P, grp->pbits, MODOP_MUL));
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->Y, &pt->Y, &ll, &grp->P, grp->pbits, MODOP_MUL));

cleanup:

    mbedtls_mpi_free( &l );
    mbedtls_mpi_free( &ll );

    return ret;
}
#endif

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
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
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
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
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

    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi N, Zi, ZZi;
    const mbedtls_mpi *Np;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&Zi);
    mbedtls_mpi_init(&ZZi);

    /* Use ECP_HELPER_MPI_NORM(Np, N1, N_, P) to get normalized MPI
     *
     * N_: Holds normalized MPI if the passed-in MPI N1 is not
     * Np: Pointer to normalized MPI, which could be N1 or N_
     */

    /* Zi = 1 / Z */
    mbedtls_mpi_lset(&Zi, 1);
    ECP_HELPER_MPI_NORM(&Np, pt->Z, N, grp->P);
    MBEDTLS_MPI_CHK(internal_run_modop(&Zi, &Zi, Np, &grp->P, grp->pbits, MODOP_DIV));

    /* ZZi = 1 / Z^2 = Zi * Zi */
    MBEDTLS_MPI_CHK(internal_run_modop(&ZZi, &Zi, &Zi, &grp->P, grp->pbits, MODOP_MUL));

    /* X = X / Z^2 = X * ZZi */
    ECP_HELPER_MPI_NORM(&Np, pt->X, N, grp->P);
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->X, Np, &ZZi, &grp->P, grp->pbits, MODOP_MUL));

    /* Y = Y / Z^3 = Y * ZZi * Zi */
    ECP_HELPER_MPI_NORM(&Np, pt->Y, N, grp->P);
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
    
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    
    mbedtls_ecp_point **ecp_point = T;
    mbedtls_ecp_point **ecp_point_end = T + t_len;
    
    for (; ecp_point != ecp_point_end; ecp_point ++) {
        MBEDTLS_MPI_CHK(mbedtls_internal_ecp_normalize_jac(grp, *ecp_point));
    }

cleanup:

    return ret;
}
#endif

#endif  /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)

#if defined(MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT)
/**
 * \brief           Double-and-add: R = 2P, S = P + Q, with d = X(P - Q),
 *                  for Montgomery curves in x/z coordinates.
 *
 *                  http://www.hyperelliptic.org/EFD/g1p/auto-code/montgom/xz/ladder/mladd-1987-m.op3
 *                  with
 *                  d =  X1
 *                  P = (X2, Z2)
 *                  Q = (X3, Z3)
 *                  R = (X4, Z4)
 *                  S = (X5, Z5)
 *                  and eliminating temporary variables tO, ..., t4.
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param R         Pointer to a point structure to hold the result of 2P.
 *
 * \param S         Pointer to a point structure to hold the result of P + Q.
 *
 * \param P         Pointer to a point structure for P + Q and 2P, given with Jacobian
 *                  coordinates
 *
 * \param Q         Pointer to a point structure for P + Q, given with affine
 *                  coordinates.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_double_add_mxz( const mbedtls_ecp_group *grp,
        mbedtls_ecp_point *R, mbedtls_ecp_point *S, const mbedtls_ecp_point *P,
        const mbedtls_ecp_point *Q, const mbedtls_mpi *d )
{
    /* Mbed TLS's implementation for Montgomery curve has optimized and
     * becomes incompatible with ECC H/W, for example, on mbedtls_mpi A/B. */
    #error "Incompatible with ECC HW. Undefine MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT in mbedtls_device.h"
}
#endif

#if defined(MBEDTLS_ECP_RANDOMIZE_MXZ_ALT)
/**
 * \brief           Randomize projective x/z coordinates:
 *                  (X, Z) -> (l X, l Z) for random l
 *                  This is sort of the reverse operation of ecp_normalize_mxz().
 *
 * \param grp       Pointer to the group representing the curve.
 *
 * \param P         Pointer to a point structure to randomize.
 *
 * \param f_rng     The RNG function. This may be \c NULL if randomization
 *                  of intermediate results isn't desired (discouraged).
 *
 * \param p_rng     The RNG context to be passed to \p p_rng.
 *
 * \return          0 if successful.
 */
int mbedtls_internal_ecp_randomize_mxz( const mbedtls_ecp_group *grp, mbedtls_ecp_point *P,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi l;
    size_t p_size;
    int count = 0;

    p_size = ( grp->pbits + 7 ) / 8;
    mbedtls_mpi_init( &l );

    /* Generate l such that 1 < l < p */
    do
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( &l, p_size, f_rng, p_rng ) );

        while( mbedtls_mpi_cmp_mpi( &l, &grp->P ) >= 0 )
            MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &l, 1 ) );

        if( count++ > 10 )
        {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }
    }
    while( mbedtls_mpi_cmp_int( &l, 1 ) <= 0 );

    /* X = l * X */
    MBEDTLS_MPI_CHK(internal_run_modop(&P->X, &P->X, &l, &grp->P, grp->pbits, MODOP_MUL));

    /* Z = l * Z */
    MBEDTLS_MPI_CHK(internal_run_modop(&P->Z, &P->Z, &l, &grp->P, grp->pbits, MODOP_MUL));

cleanup:

    mbedtls_mpi_free( &l );

    return( ret );
}
#endif

#if defined(MBEDTLS_ECP_NORMALIZE_MXZ_ALT)
/**
 * \brief           Normalize Montgomery x/z coordinates: X = X/Z, Z = 1.
 *
 * \param grp       pointer to the group representing the curve
 *
 * \param P         pointer to the point to be normalised. This is an
 *                  input/output parameter.
 *
 * \return          0 if successful
 */
int mbedtls_internal_ecp_normalize_mxz( const mbedtls_ecp_group *grp,
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
     * 
     * NOTE: Y not used
     */
    if (mbedtls_mpi_cmp_int(&pt->Z, 1) == 0 &&
        mbedtls_mpi_cmp_int(&pt->X, 0) >= 0 &&
        mbedtls_mpi_cmp_mpi(&pt->X, &grp->P) < 0) {
        return 0;
    }

    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi N, Zi;
    const mbedtls_mpi *Np;

    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&Zi);

    /* Use ECP_HELPER_MPI_NORM(Np, N1, N_, P) to get normalized MPI
     *
     * N_: Holds normalized MPI if the passed-in MPI N1 is not
     * Np: Pointer to normalized MPI, which could be N1 or N_
     */

    /* Zi = 1 / Z */
    mbedtls_mpi_lset(&Zi, 1);
    ECP_HELPER_MPI_NORM(&Np, pt->Z, N, grp->P);
    MBEDTLS_MPI_CHK(internal_run_modop(&Zi, &Zi, Np, &grp->P, grp->pbits, MODOP_DIV));

    /* X = X / Z = X * Zi */
    ECP_HELPER_MPI_NORM(&Np, pt->X, N, grp->P);
    MBEDTLS_MPI_CHK(internal_run_modop(&pt->X, Np, &Zi, &grp->P, grp->pbits, MODOP_MUL));

    /* Z = 1 */
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&pt->Z, 1));

cleanup:

    mbedtls_mpi_free(&Zi);
    mbedtls_mpi_free(&N);

    return ret;
}
#endif

#endif  /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

NU_STATIC int internal_ecp_normalize(const mbedtls_ecp_group *grp,
                                     mbedtls_ecp_point *pt)
{
    mbedtls_ecp_curve_type curve_type = mbedtls_ecp_get_type(grp);
    if (curve_type == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS) {
        return mbedtls_internal_ecp_normalize_jac(grp, pt);
    } else if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        return mbedtls_internal_ecp_normalize_mxz(grp, pt);
    } else {
        return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
}

NU_STATIC int internal_run_eccop(const mbedtls_ecp_group *grp,
                                    mbedtls_ecp_point *R,
                                    const mbedtls_mpi *m,
                                    const mbedtls_ecp_point *P,
                                    const mbedtls_mpi *n,
                                    const mbedtls_ecp_point *Q,
                                    uint32_t eccop)
{
    return crypto_ecc_run_eccop( grp, R, m, P, n, Q, eccop, false );
}

NU_STATIC int internal_run_modop(mbedtls_mpi *r,
                                    const mbedtls_mpi *o1,
                                    const mbedtls_mpi *o2,
                                    const mbedtls_mpi *p,
                                    uint32_t pbits,
                                    uint32_t modop)
{
    return crypto_ecc_run_modop( r, o1, o2, p, pbits, modop );
}

NU_STATIC int internal_mpi_read_eccreg(mbedtls_mpi *x, const volatile uint32_t *eccreg, size_t eccreg_num)
{
    return crypto_ecc_mpi_read_eccreg( x, eccreg, eccreg_num);
}

NU_STATIC int internal_mpi_write_eccreg( const mbedtls_mpi *x, volatile uint32_t *eccreg, size_t eccreg_num )
{
    return crypto_ecc_mpi_write_eccreg( x, eccreg, eccreg_num );
}

#endif /* MBEDTLS_ECP_INTERNAL_ALT */
#endif /* ! MBEDTLS_ECP_ALT */
#endif /* MBEDTLS_ECP_C */
