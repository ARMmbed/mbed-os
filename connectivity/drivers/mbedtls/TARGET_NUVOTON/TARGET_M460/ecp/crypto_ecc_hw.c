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

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_ECP_ALT) || defined(MBEDTLS_ECP_INTERNAL_ALT)

#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "mbed_toolchain.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_toolchain.h"
#include "nu_bitutil.h"
#include "nu_timer.h"
#include "crypto-misc.h"
#include "crypto_ecc_hw.h"
#include "ecp_helper.h"

/* Enable ECC debug */
//#define NU_CRYPTO_ECC_ENABLE_DEBUG

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

/* Notes for Crypto ECC H/W port
 *
 * The following point operations are not supported and will cause engine to trap:
 * 1. P + P. Workaround by 2*P.
 * 2. m*P with SCAP enabled, esp m = 2 or close to (order - 1). Cannot work around by
 *    fallback to S/W, because following operations are easily to fail with data error.
 *    Disable SCAP temporarily.
 */

int crypto_ecc_capable(const mbedtls_ecp_group *grp)
{
    /* Curve types
     *
     * - Short Weierstrass
     * - Montgomery
     */
    mbedtls_ecp_curve_type curve_type = mbedtls_ecp_get_type(grp);
    if (curve_type == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS) {
        return 1;
    } else if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        if (grp->id == MBEDTLS_ECP_DP_CURVE25519 ||
            grp->id == MBEDTLS_ECP_DP_CURVE448) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int crypto_ecc_init(const mbedtls_ecp_group *grp)
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

    /* Release ownership of ECC accelerator */
    crypto_ecc_release();

    return 0;
}

void crypto_ecc_free(const mbedtls_ecp_group *grp)
{
    /* Acquire ownership of ECC accelerator */
    crypto_ecc_acquire();

    /* Uninit crypto module */
    crypto_uninit();

    /* Release ownership of ECC accelerator */
    crypto_ecc_release();
}

int crypto_ecc_run_eccop_add(const mbedtls_ecp_group *grp,
                             mbedtls_ecp_point *R,
                             const mbedtls_ecp_point *P,
                             const mbedtls_ecp_point *Q,
                             bool blinding)
{
    /* SCAP is applicable only for PM. */
    blinding = false;

    return crypto_ecc_run_eccop(grp, R, NULL, P, NULL, Q, ECCOP_POINT_ADD, blinding);
}

int crypto_ecc_run_eccop_double(const mbedtls_ecp_group *grp,
                                mbedtls_ecp_point *R,
                                const mbedtls_ecp_point *P,
                                bool blinding)
{
    /* SCAP is applicable only for PM. */
    blinding = false;

    return crypto_ecc_run_eccop(grp, R, NULL, P, NULL, NULL, ECCOP_POINT_DOUBLE, blinding);
}

int crypto_ecc_run_eccop_mul(const mbedtls_ecp_group *grp,
                             mbedtls_ecp_point *R,
                             const mbedtls_mpi *m,
                             const mbedtls_ecp_point *P,
                             bool blinding)
{
    /* NOTE: Engine can trap when SCAP is enabled. See above. */
    blinding = false;
    return crypto_ecc_run_eccop(grp, R, m, P, NULL, NULL, ECCOP_POINT_MUL, blinding);
}

int crypto_ecc_run_eccop(const mbedtls_ecp_group *grp,
                         mbedtls_ecp_point *R,
                         const mbedtls_mpi *m,
                         const mbedtls_ecp_point *P,
                         MBED_UNUSED const mbedtls_mpi *n,
                         const mbedtls_ecp_point *Q,
                         uint32_t eccop,
                         bool blinding)
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

    /* NOTE: Engine doesn't support P + Q when P and Q are the same. Workaround by 2*P */
    if (eccop == ECCOP_POINT_ADD) {
        if (P == NULL || Q == NULL) {
            return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        }
        if (mbedtls_ecp_point_cmp(P, Q) == 0) {
            return crypto_ecc_run_eccop(grp, R, NULL, P, NULL, NULL, ECCOP_POINT_DOUBLE, blinding);
        }
    }

    /* Acquire ownership of ECC accelerator */
    crypto_ecc_acquire();

    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    bool ecc_done;

    mbedtls_mpi N_;
    const mbedtls_mpi *Np;

    mbedtls_mpi_init(&N_);

    /* Use ECP_HELPER_MPI_NORM(Np, N1, N_, P) to get normalized MPI
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
            /* Unnecessary because passed-in P and Q are required to be normalized */
            //MBEDTLS_MPI_CHK(internal_ecp_normalize(grp, R));
            goto cleanup;
        }

        /* R = m*P = (multiple of order)*G = 0 */
        /* NOTE: If grp->N (order) is a prime, we could detect R = 0 for all m*P cases
         *       by just checking if m is a multiple of grp->N. Otherwise, sigh. */
        /* TODO: Find an approach to detecting R = 0 for all m*P cases */
        ECP_HELPER_MPI_NORM(&Np, *m, N_, grp->N);
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
            /* Unnecessary because passed-in P and Q are required to be normalized */
            //MBEDTLS_MPI_CHK(internal_ecp_normalize(grp, R));
            goto cleanup;
        }

        /* R = P + 0 = P */
        if (mbedtls_mpi_cmp_int(&Q->Z, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_copy(R, P));
            /* Unnecessary because passed-in P and Q are required to be normalized */
            //MBEDTLS_MPI_CHK(internal_ecp_normalize(grp, R));
            goto cleanup;
        }

        /* R = P + Q = P + (-P) = 0 */
        MBEDTLS_MPI_CHK(crypto_ecc_run_modop(&N_, &P->Y, &Q->Y, &grp->P, grp->pbits, MODOP_ADD));
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
        MBEDTLS_MPI_CHK(crypto_ecc_run_modop(&N_, &P->Y, &P->Y, &grp->P, grp->pbits, MODOP_ADD));
        if (mbedtls_mpi_cmp_int(&N_, 0) == 0) {
            MBEDTLS_MPI_CHK(mbedtls_ecp_set_zero(R));
            goto cleanup;
        }
    } else {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    /* ECC curve type */
    mbedtls_ecp_curve_type curve_type = mbedtls_ecp_get_type(grp);
    uint32_t curve_sel = 0;
    switch (curve_type) {
    case MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS:
        curve_sel = 0;                      // Short Weierstrass
        break;

    case MBEDTLS_ECP_TYPE_MONTGOMERY:
        curve_sel = CRPT_ECC_CTL_CSEL_Msk;  // Montgomery
        break;

    default:
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    /* Enable ECC interrupt */
    ECC_ENABLE_INT(CRPT);

    /* For safe, recover from previous failure if ever */
    MBEDTLS_MPI_CHK(crypto_ecc_abort(5*1000*1000));

    /* Configure ECC curve coefficient A */
    if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        /*
         * In S/W impl, A is used as (A + 2) / 4. Figure out its original value for engine.
         * https://github.com/ARMmbed/mbed-os/blob/2eb06e76208588afc6cb7580a8dd64c5429a10ce/connectivity/mbedtls/include/mbedtls/ecp.h#L219-L220
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_int(&N_, &grp->A, 4));
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_int(&N_, &N_, 2));
        Np = &N_;
    } else {
        /* Special case for A = -3 */
        if (grp->A.p == NULL) {
            MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&N_, -3));
            ECP_HELPER_MPI_NORM(&Np, N_, N_, grp->P);
        } else {
            ECP_HELPER_MPI_NORM(&Np, grp->A, N_, grp->P);
        }
    }
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_A, NU_ECC_BIGNUM_MAXWORD));

    /* Configure ECC curve coefficient B */
    if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        /*
         * In S/W impl, B is unused. Actually, B is 1 for Curve25519/Curve448.
         * https://github.com/ARMmbed/mbed-os/blob/2eb06e76208588afc6cb7580a8dd64c5429a10ce/connectivity/mbedtls/include/mbedtls/ecp.h#L221-L222
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&N_, 1));
        Np = &N_;
    } else {
        ECP_HELPER_MPI_NORM(&Np, grp->B, N_, grp->P);
    }
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_B, NU_ECC_BIGNUM_MAXWORD));

    /* Configure ECC prime modulus */ 
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(&grp->P, (uint32_t *) CRPT->ECC_N, NU_ECC_BIGNUM_MAXWORD));

    /* Configure ECC scalar for point multiplication 
     *
     * Normalize m to within [1, order - 1] which ECCOP_POINT_MUL supports
     * Special cases R = 0 should have been detected out above.
     */
    if (eccop == ECCOP_POINT_MUL) {
        ECP_HELPER_MPI_NORM(&Np, *m, N_, grp->N);
        MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_K, NU_ECC_BIGNUM_MAXWORD));
        /* To enable SCAP, must write order of G into X2 */
        if (blinding) {
            MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(&grp->N, (uint32_t *) CRPT->ECC_X2, NU_ECC_BIGNUM_MAXWORD));
        }
    }

    /* Configure ECC point (X1, Y1) */
    ECP_HELPER_MPI_NORM(&Np, P->X, N_, grp->P);
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
    if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        /* Deduce Y from X for Montgomery curve
         *
         * For Montgomery curve, y-coord is absent. Deduce it from x-coord.
         * https://github.com/ARMmbed/mbed-os/blob/2eb06e76208588afc6cb7580a8dd64c5429a10ce/connectivity/mbedtls/source/ecp_curves.c#L702-L706
         * https://github.com/ARMmbed/mbed-os/blob/2eb06e76208588afc6cb7580a8dd64c5429a10ce/connectivity/mbedtls/source/ecp_curves.c#L741-L745
         *
         * Per real test, y-coord is necessary for engine.
         */
        MBEDTLS_MPI_CHK(ecp_helper_deduce_y(grp, &N_, &P->X));
        Np = &N_;
    } else {
        ECP_HELPER_MPI_NORM(&Np, P->Y, N_, grp->P);
    }
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));

    /* Configure ECC points (X2, Y2) */
    if (eccop == ECCOP_POINT_ADD) {
        ECP_HELPER_MPI_NORM(&Np, Q->X, N_, grp->P);
        MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X2, NU_ECC_BIGNUM_MAXWORD));
        if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
            /* Deduce Y from X (see above) */
            MBEDTLS_MPI_CHK(ecp_helper_deduce_y(grp, &N_, &Q->X));
            Np = &N_;
        } else {
            ECP_HELPER_MPI_NORM(&Np, Q->Y, N_, grp->P);
        }
        MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y2, NU_ECC_BIGNUM_MAXWORD));
    }

    /* Configure for point operation */
    uint32_t ecc_ctl = 0 |
        CRPT_ECC_CTL_START_Msk |                    // Start
        CRPT_ECC_CTL_FSEL_Msk |                     // Prime field (GF(p))
        eccop |                                     // Point operation
        curve_sel |                                 // Curve selection
        (grp->pbits << CRPT_ECC_CTL_CURVEM_Pos) |   // Key length of elliptic curve
        (blinding ? CRPT_ECC_CTL_SCAP_Msk : 0) |    // SCAP
        0;

    crypto_ecc_prestart();
#if defined(NU_CRYPTO_ECC_ENABLE_DEBUG) && !defined(MBEDTLS_ECP_INTERNAL_ALT)
    mbedtls_printf("[CRPT][ECC] Crypto ECC ...\n");
#endif
    CRPT->ECC_CTL = ecc_ctl;
    ecc_done = crypto_ecc_wait2(1000*1000); // 1s timeout
#if defined(NU_CRYPTO_ECC_ENABLE_DEBUG) && !defined(MBEDTLS_ECP_INTERNAL_ALT)
    mbedtls_printf("[CRPT][ECC] Crypto ECC ... %s\n", ecc_done ? "Done" : "Error");
#endif

    /* For safe, recover from current failure */
    if (!ecc_done) {
        crypto_ecc_abort(5*1000*1000);
    }

    /* Disable ECC interrupt */
    ECC_DISABLE_INT(CRPT);

    MBEDTLS_MPI_CHK(ecc_done ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);

    /* (X1, Y1) hold the normalized result. */
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_read_eccreg(&R->X, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_read_eccreg(&R->Y, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));
    MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&R->Z, 1));

cleanup:

    mbedtls_mpi_free(&N_);

    /* Release ownership of ECC accelerator */
    crypto_ecc_release();

    return ret;
}

int crypto_ecc_run_modop(mbedtls_mpi *r,
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

    /* Acquire ownership of ECC accelerator */
    crypto_ecc_acquire();

    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    bool ecc_done;

    mbedtls_mpi N_;
    const mbedtls_mpi *Np;

    mbedtls_mpi_init(&N_);

    /* Enable ECC interrupt */
    ECC_ENABLE_INT(CRPT);

    /* For safe, recover from previous failure if ever */
    MBEDTLS_MPI_CHK(crypto_ecc_abort(5*1000*1000));

    /* Use ECP_HELPER_MPI_NORM(Np, N1, N_, P) to get normalized MPI
     *
     * N_: Holds normalized MPI if the passed-in MPI N1 is not
     * Np: Pointer to normalized MPI, which could be N1 or N_
     */

    if (modop == MODOP_MUL ||
        modop == MODOP_ADD ||
        modop == MODOP_SUB) {
        ECP_HELPER_MPI_NORM(&Np, *o1, N_, *p);
        MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
        ECP_HELPER_MPI_NORM(&Np, *o2, N_, *p);
        MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));
    } else if (modop == MODOP_DIV) {
        ECP_HELPER_MPI_NORM(&Np, *o2, N_, *p);
        MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));
        ECP_HELPER_MPI_NORM(&Np, *o1, N_, *p);
        MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(Np, (uint32_t *) CRPT->ECC_Y1, NU_ECC_BIGNUM_MAXWORD));
    } else {
        MBEDTLS_MPI_CHK(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }

    MBEDTLS_MPI_CHK(crypto_ecc_mpi_write_eccreg(p, (uint32_t *) CRPT->ECC_N, NU_ECC_BIGNUM_MAXWORD));

    /* Configure for modulus operation */
    uint32_t ecc_ctl = 0 |
        CRPT_ECC_CTL_START_Msk |                    // Start
        CRPT_ECC_CTL_FSEL_Msk |                     // Prime field (GF(p))
        ECCOP_MODULE |                              // No point operation
        modop |                                     // Modulus operation
        (pbits << CRPT_ECC_CTL_CURVEM_Pos) |        // Key length of elliptic curve
        0;

    crypto_ecc_prestart();
#if defined(NU_CRYPTO_ECC_ENABLE_DEBUG) && !defined(MBEDTLS_ECP_INTERNAL_ALT)
    mbedtls_printf("[CRPT][ECC] Crypto Modulus ...\n");
#endif
    CRPT->ECC_CTL = ecc_ctl;
    ecc_done = crypto_ecc_wait2(1000*1000); // 1s timeout
#if defined(NU_CRYPTO_ECC_ENABLE_DEBUG) && !defined(MBEDTLS_ECP_INTERNAL_ALT)
    mbedtls_printf("[CRPT][ECC] Crypto Modulus ... %s\n", ecc_done ? "Done" : "Fallback");
#endif

    /* For safe, recover from current failure */
    if (!ecc_done) {
        crypto_ecc_abort(5*1000*1000);
    }

    /* Disable ECC interrupt */
    ECC_DISABLE_INT(CRPT);

    MBEDTLS_MPI_CHK(ecc_done ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);

    /* X1 holds the result. */
    MBEDTLS_MPI_CHK(crypto_ecc_mpi_read_eccreg(r, (uint32_t *) CRPT->ECC_X1, NU_ECC_BIGNUM_MAXWORD));

cleanup:

    mbedtls_mpi_free(&N_);

    /* Release ownership of ECC accelerator */
    crypto_ecc_release();

    return ret;
}

int crypto_ecc_mpi_read_eccreg(mbedtls_mpi *x, const volatile uint32_t *eccreg, size_t eccreg_num)
{
    if (x == NULL) {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }
    
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
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

int crypto_ecc_mpi_write_eccreg( const mbedtls_mpi *x, volatile uint32_t *eccreg, size_t eccreg_num )
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

int crypto_ecc_abort(uint32_t timeout_us)
{
    /* Acquire ownership of ECC H/W */
    crypto_ecc_acquire();

    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    CRPT->ECC_CTL = CRPT_ECC_CTL_STOP_Msk;
    struct nu_countdown_ctx_s cd_ctx;
    nu_countdown_init(&cd_ctx, timeout_us);
    while (CRPT->ECC_STS & CRPT_ECC_STS_BUSY_Msk) {
        if (nu_countdown_expired(&cd_ctx)) {
            break;
        }
    }
    nu_countdown_free(&cd_ctx);
    if (CRPT->ECC_STS & CRPT_ECC_STS_BUSY_Msk) {
#if defined(NU_CRYPTO_ECC_ENABLE_DEBUG) && !defined(MBEDTLS_ECP_INTERNAL_ALT)
        mbedtls_printf("[CRPT][ECC] Crypto ECC ... Busy\n");
#endif
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto cleanup;
    }

    ret = 0;

cleanup:

    /* Release ownership of ECC accelerator */
    crypto_ecc_release();

    return ret;
}

#endif /* MBEDTLS_ECP_ALT || MBEDTLS_ECP_INTERNAL_ALT */
#endif /* MBEDTLS_ECP_C */
