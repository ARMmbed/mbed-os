/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file psa_crypto_extra.h
 *
 * \brief PSA cryptography module: vendor extensions
 */

#ifndef __PSA_CRYPTO_EXTRA_H__
#define __PSA_CRYPTO_EXTRA_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \note This file is meant to be included by psa_crypto.h only
 *
 */

/**
 * \brief Library deinitialization.
 *
 * This function clears all data associated with the PSA layer,
 * including the whole key store.
 *
 * This is an Mbed TLS extension.
 */
void mbedtls_psa_crypto_free(void);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_CRYPTO_EXTRA_H__ */
