/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file psa_crypto_platform.h
 *
 * \brief PSA cryptography module: platform definitions
 */

#ifndef __PSA_CRYPTO_PLATFORM_H__
#define __PSA_CRYPTO_PLATFORM_H__

/**
 * \note This file is meant to be included by psa_crypto.h only
 *
 */

/* PSA requires several types which C99 provides in stdint.h. */
#include <stdint.h>

/* The following header is needed for platform specific constants */
#include <limits.h>

/* Integral type representing a key slot number. */
typedef uint16_t psa_key_slot_t;

#endif /* __PSA_CRYPTO_PLATFORM_H__ */
