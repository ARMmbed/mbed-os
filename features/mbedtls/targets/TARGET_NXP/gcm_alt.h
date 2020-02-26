/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MBEDTLS_GCM_ALT_H
#define MBEDTLS_GCM_ALT_H


#if defined(MBEDTLS_GCM_ALT)

#include "aes_alt.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          The GCM context structure.
 */
typedef struct mbedtls_gcm_context
{
    mbedtls_cipher_context_t cipher_ctx;  /*!< The cipher context used. */
    uint64_t HL[16];                      /*!< Precalculated HTable low. */
    uint64_t HH[16];                      /*!< Precalculated HTable high. */
    uint64_t len;                         /*!< The total length of the encrypted data. */
    uint64_t add_len;                     /*!< The total length of the additional data. */
    unsigned char base_ectr[16];          /*!< The first ECTR for tag. */
    unsigned char y[16];                  /*!< The Y working value. */
    unsigned char buf[16];                /*!< The buf working value. */
    int mode;                             /*!< The operation to perform:
                                               #MBEDTLS_GCM_ENCRYPT or
                                               #MBEDTLS_GCM_DECRYPT. */
}
mbedtls_gcm_context;

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_GCM_ALT */

#endif /* MBEDTLS_GCM_ALT_H */

