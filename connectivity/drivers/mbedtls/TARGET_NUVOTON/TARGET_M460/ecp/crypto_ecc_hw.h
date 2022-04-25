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

#ifndef CRYPTO_ECC_HW_H
#define CRYPTO_ECC_HW_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECP_ALT) || defined(MBEDTLS_ECP_INTERNAL_ALT)

#include "mbedtls/ecp.h"
#include <stdbool.h>

/* Crypto ECC H/W point operations */
#define ECCOP_POINT_MUL     (0x0UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_MODULE        (0x1UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_ADD     (0x2UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_DOUBLE  (0x3UL << CRPT_ECC_CTL_ECCOP_Pos)

/* Crypto ECC H/W modulus operations */
#define MODOP_DIV           (0x0UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_MUL           (0x1UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_ADD           (0x2UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_SUB           (0x3UL << CRPT_ECC_CTL_MODOP_Pos)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           This function checks whether a given group can be used
 *                  for Crypto ECC H/W.
 *
 * \param grp       ECP group
 *
 * \return          \c 1 if the group can be used, \c 0 otherwise
 */
int crypto_ecc_capable(const mbedtls_ecp_group *grp);

/**
 * \brief           Initialize/Free Crypto ECC H/W
 *
 * \return          \c 0 on success.
 * \return          A non-zero error code on failure.
 *
 * \note            crypto_ecp_init()/crypto_ecp_free() are like pre-op/post-op calls
 *                  and they guarantee:
 *
 *                  1. Paired
 *                  2. No overlapping
 *                  3. Upper public function cannot return when ECP alter. is still activated.
 */
int crypto_ecc_init(const mbedtls_ecp_group *grp);
void crypto_ecc_free(const mbedtls_ecp_group *grp);
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
 * \param blinding  Blinding (SCAP) or not.
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
 * \note            Blinding (SCAP) is applicable only for point multiplication. But for future extension,
 *                  blinding is kept with all point operations.
 *                  
 */
int crypto_ecc_run_eccop_add(const mbedtls_ecp_group *grp,
                             mbedtls_ecp_point *R,
                             const mbedtls_ecp_point *P,
                             const mbedtls_ecp_point *Q,
                             bool blinding);
int crypto_ecc_run_eccop_double(const mbedtls_ecp_group *grp,
                                mbedtls_ecp_point *R,
                                const mbedtls_ecp_point *P,
                                bool blinding);
int crypto_ecc_run_eccop_mul(const mbedtls_ecp_group *grp,
                             mbedtls_ecp_point *R,
                             const mbedtls_mpi *m,
                             const mbedtls_ecp_point *P,
                             bool blinding);
int crypto_ecc_run_eccop(const mbedtls_ecp_group *grp,
                         mbedtls_ecp_point *R,
                         const mbedtls_mpi *m,
                         const mbedtls_ecp_point *P,
                         const mbedtls_mpi *n,
                         const mbedtls_ecp_point *Q,
                         uint32_t eccop,
                         bool blinding);

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
int crypto_ecc_run_modop(mbedtls_mpi *r,
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
int crypto_ecc_mpi_read_eccreg( mbedtls_mpi *X, const volatile uint32_t *eccreg, size_t eccreg_num );

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
int crypto_ecc_mpi_write_eccreg( const mbedtls_mpi *X, volatile uint32_t *eccreg, size_t eccreg_num );

/**
 * \brief           Abort Crypto ECC H/W
 *
 * \param timeout_us    Timeout in microseconds.
 *
 * \return          \c 0 on success.
 * \return          A non-zero error code on failure.
 */
int crypto_ecc_abort(uint32_t timeout_us);

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_ECP_ALT || MBEDTLS_ECP_INTERNAL_ALT */
#endif /* CRYPTO_ECC_HW_H */
