/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/**
 * \file psa/crypto_platform.h
 *
 * \brief PSA cryptography module: Mbed TLS platform definitions
 *
 * \note This file may not be included directly. Applications must
 * include psa/crypto.h.
 *
 * This file contains platform-dependent type definitions.
 *
 * In implementations with isolation between the application and the
 * cryptography module, implementers should take care to ensure that
 * the definitions that are exposed to applications match what the
 * module implements.
 */

#ifndef PSA_CRYPTO_PLATFORM_H
#define PSA_CRYPTO_PLATFORM_H

/* PSA requires several types which C99 provides in stdint.h. */
#include <stdint.h>

/* Integral type representing a key handle. */
typedef uint16_t psa_key_handle_t;

/* This implementation distinguishes *application key identifiers*, which
 * are the key identifiers specified by the application, from
 * *key file identifiers*, which are the key identifiers that the library
 * sees internally. The two types can be different if there is a remote
 * call layer between the application and the library which supports
 * multiple client applications that do not have access to each others'
 * keys. The point of having different types is that the key file
 * identifier may encode not only the key identifier specified by the
 * application, but also the the identity of the application.
 *
 * Note that this is an internal concept of the library and the remote
 * call layer. The application itself never sees anything other than
 * #psa_app_key_id_t with its standard definition.
 */

/* The application key identifier is always what the application sees as
 * #psa_key_id_t. */
typedef uint32_t psa_app_key_id_t;

#endif /* PSA_CRYPTO_PLATFORM_H */
