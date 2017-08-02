/*
 *  mbed OS configuration header for mbed TLS HW acceleration
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
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
 */
#ifndef MBEDTLS_DEVICE_H
#define MBEDTLS_DEVICE_H

#include "em_device.h"

#if defined ( AES_PRESENT )
#define MBEDTLS_AES_ALT
#endif

#if defined ( CRYPTO_PRESENT )
#define MBEDTLS_AES_ALT

#define MBEDTLS_ECP_INTERNAL_ALT
#define MBEDTLS_ECP_ADD_MIXED_ALT
#define MBEDTLS_ECP_DOUBLE_JAC_ALT
#define MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT
#define MBEDTLS_ECP_NORMALIZE_JAC_ALT
#define MBEDTLS_ECP_RANDOMIZE_JAC_ALT

#define MBEDTLS_SHA1_ALT
#define MBEDTLS_SHA256_ALT
#endif

#endif /* MBEDTLS_DEVICE_H */
