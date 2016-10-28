/**
 * \file cmac.h
 *
 * \brief CMAC cipher mode based on 128 bit AES.
 *
 *  Copyright (C) 2016, Silicon Labs, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of the mbedTLS plugins package from Silicon Labs
 */
#ifndef MBEDTLS_CMAC_H
#define MBEDTLS_CMAC_H

/*
 *  This file implements CMAC (Cipher-based Message Authentication Code)
 *  cipher mode encryption/decryption based on 128 bit AES.
 *  For a general description please see
 *  https://en.wikipedia.org/wiki/CMAC
 *  or for detailed specification see
 *  http://csrc.nist.gov/publications/nistpubs/800-38B/SP_800-38B.pdf
 *  http://dl.acm.org/citation.cfm?id=2206249
 */

#if defined( MBEDTLS_CMAC_ALT )
/* SiliconLabs CRYPTO hardware acceleration implementation */

#include "cmac_alt.h"

#else  /* #if defined( MBEDTLS_CMAC_ALT ) */

// Regular implementation does not exist yet.

#endif /* #if defined( MBEDTLS_CMAC_ALT ) */

#endif /* MBEDTLS_CMAC_H */
