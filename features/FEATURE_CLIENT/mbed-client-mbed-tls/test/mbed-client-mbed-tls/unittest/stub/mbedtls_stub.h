/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBEDTLS_STUB_H
#define MBEDTLS_STUB_H

#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/entropy.h"
#include "mbedtls/pk.h"

namespace mbedtls_stub
{
    extern int crt_expected_int;
    extern bool useCounter;
    extern int counter;
    extern int retArray[20];
    extern int expected_int;
    extern uint32_t expected_uint32_value;
    extern bool invalidate_timer;    
    void clear();
}

#endif // MBEDTLS_STUB_H
