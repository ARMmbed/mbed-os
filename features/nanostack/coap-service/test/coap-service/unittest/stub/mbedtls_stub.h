/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#ifndef MBEDTLS_STUB_H
#define MBEDTLS_STUB_H

#define MBEDTLS_SSL_EXPORT_KEYS

#include <inttypes.h>
#include <stdbool.h>
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/sha256.h"
#include "mbedtls/entropy.h"
#include "mbedtls/pk.h"



#define HANDSHAKE_FINISHED_VALUE 8888
#define HANDSHAKE_FINISHED_VALUE_RETURN_ZERO 8889

typedef struct {
    int crt_expected_int;
    bool useCounter;
    int counter;
    int retArray[20];
    int expected_int;
    uint32_t uint32_value;
    bool invalidate_timer;
    void *cookie_obj;
    unsigned char cookie_value[8];
    size_t cookie_len;
} mbedtls_stub_def;

extern mbedtls_stub_def mbedtls_stub;

#endif // MBEDTLS_STUB_H
