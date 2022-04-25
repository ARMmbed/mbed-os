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

#include "common.h"

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/threading.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include <string.h>

#if defined(MBEDTLS_ECP_ALT)

int ecp_helper_deduce_y(const mbedtls_ecp_group *grp,
                        mbedtls_mpi *y,
                        const mbedtls_mpi *x)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    mbedtls_mpi A;
    mbedtls_mpi B;
    mbedtls_mpi xx;     // x^2
    mbedtls_mpi yy;     // y^2
    mbedtls_mpi T1;     // Scratch
    mbedtls_mpi T2;     // Scratch
    mbedtls_mpi T3;     // Scratch
    mbedtls_mpi T4;     // Scratch

    mbedtls_mpi_init(&A);
    mbedtls_mpi_init(&B);
    mbedtls_mpi_init(&xx);
    mbedtls_mpi_init(&yy);
    mbedtls_mpi_init(&T1);
    mbedtls_mpi_init(&T2);
    mbedtls_mpi_init(&T3);
    mbedtls_mpi_init(&T4);

    /* ECC curve type */
    mbedtls_ecp_curve_type curve_type = mbedtls_ecp_get_type(grp);

    /* Resolve A */
    if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        /*
         * In S/W impl, A is used as (A + 2) / 4. Figure out its original value for engine.
         * https://github.com/ARMmbed/mbed-os/blob/2eb06e76208588afc6cb7580a8dd64c5429a10ce/connectivity/mbedtls/include/mbedtls/ecp.h#L219-L220
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_int(&A, &grp->A, 4));
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_int(&A, &A, 2));
    } else {
        MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&A, &grp->A));
    }

    /* Resolve B */
    if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        /*
         * In S/W impl, B is unused. Actually, B is 1 for Curve25519/Curve448.
         * https://github.com/ARMmbed/mbed-os/blob/2eb06e76208588afc6cb7580a8dd64c5429a10ce/connectivity/mbedtls/include/mbedtls/ecp.h#L221-L222
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&B, 1));
    } else {
        MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&B, &grp->B));
    }

    /* y^2 = C(x) (mod P) */
    if (curve_type == MBEDTLS_ECP_TYPE_MONTGOMERY) {
        /* Montgomery curve: B y^2 = x^3 + A x^2 + x (mod P)
         * For Curve25519/Curve448, B = 1 */
        MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&yy, x));              // yy = x (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&yy, &yy, &grp->P));

        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&xx, x, x));        // xx = x^2 (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&xx, &xx, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&T1, &A, &xx));     // T1 = A x^2 (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&T1, &T1, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&yy, &T1, &yy));    // yy = A x^2 + x (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&yy, &yy, &grp->P));

        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&T1, &xx, x));      // T1 = x^3 (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&T1, &T1, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&yy, &T1, &yy));    // yy = x^3 + A x^2 + x (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&yy, &yy, &grp->P));
    } else {
        ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
        goto cleanup;
    }

    mbedtls_mpi_uint rmn_4 = 0, rmn_8 = 0;
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_int(&rmn_4, &grp->P, 4));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_int(&rmn_8, &grp->P, 8));

    /* y = sqrt(y^2) (mod P) */
    if (rmn_8 == 5) {
        /*
         * Modulus congruent to 5 modulo 8 (Curve25519), apply the formula below:
         * https://www.rieselprime.de/ziki/Modular_square_root#Modulus_congruent_to_5_modulo_8
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_int(&T1, &grp->P, 5));              // T1 = (P - 5) / 8 (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_div_int(&T1, NULL, &T1, 8));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&T1, &T1, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_int(&T2, &yy, 2));                  // T2 = 2 y^2 (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&T2, &T2, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&T3, &T2, &T1, &grp->P, NULL)); // T3 = T2^T1 (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&T4, &T2, &T3));                // T4 = T2 T3^2 (mod P)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&T4, &T4, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&T4, &T4, &T3));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&T4, &T4, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_int(y, &T4, 1));                    // y = yy T3 (T4 - 1)
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(y, y, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(y, &T3, y));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(y, y, &grp->P));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(y, &yy, y));
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(y, y, &grp->P));
    } else if (rmn_4 == 3) {
        /* 
         * Modulus congruent to 3 modulo 4 (Curve448), apply the formula below:
         * https://www.rieselprime.de/ziki/Modular_square_root#Modulus_congruent_to_3_modulo_4
         */
        MBEDTLS_MPI_CHK(mbedtls_mpi_add_int(&T1, &grp->P, 1));              // T1 = (P + 1) / 4
        MBEDTLS_MPI_CHK(mbedtls_mpi_div_int(&T1, NULL, &T1, 4));
        MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(y, &yy, &T1, &grp->P, NULL));   // y = yy^T1 (mod P)
    } else {
        ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
        goto cleanup;
    }

    /* y = min(y, P - y) */
    MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(&T1, &grp->P, y));
    if (mbedtls_mpi_cmp_mpi(y, &T1) > 0) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_copy(y, &T1));
    }

cleanup:

    mbedtls_mpi_free(&T4);
    mbedtls_mpi_free(&T3);
    mbedtls_mpi_free(&T2);
    mbedtls_mpi_free(&T1);
    mbedtls_mpi_free(&yy);
    mbedtls_mpi_free(&xx);
    mbedtls_mpi_free(&B);
    mbedtls_mpi_free(&A);

    return ret;
}

#endif /* MBEDTLS_ECP_ALT */

#endif /* MBEDTLS_ECP_C */
