/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MBEDTLS_CCM_ALT_H
#define MBEDTLS_CCM_ALT_H


#if defined(MBEDTLS_CCM_ALT)

#include "aes_alt.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief    The CCM context-type definition. The CCM context is passed
 *           to the APIs called.
 */
typedef struct mbedtls_ccm_context
{
    mbedtls_cipher_context_t cipher_ctx;    /*!< The cipher context used. */
}
mbedtls_ccm_context;

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_CCM_ALT */

#endif /* MBEDTLS_CCM_ALT_H */

