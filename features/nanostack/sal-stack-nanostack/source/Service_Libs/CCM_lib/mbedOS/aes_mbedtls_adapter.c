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
/*
 * Implementation of platform/arm_hal_aes.h using mbed TLS.
 *
 * Possible scenarios:
 *
 * 1) Platform with no hardware AES assist, mbed TLS not in use:
 *     Use this source file, and let it pull in the minimal mbed TLS code
 *     contained in aes_mbedtls.c to implement software AES.
 *
 * 2) Platform with hardware AES assist, mbed TLS not in use:
 *     Do not use this source file - implement arm_hal_aes.h yourself using
 *     your AES hardware.
 *
 * 3) Platform without hardware assist, already using (or wanting to use) mbed TLS:
 *     Use this source file, and define NS_USE_EXTERNAL_MBED_TLS so that
 *     it uses the external mbed TLS library. That library must be built with
 *     MBEDTLS_AES_C enabled, and it must be on the include path.
 *
 * 4) Platform with context-capable hardware assist, already using mbed TLS:
 *     Use this source file, and define NS_USE_EXTERNAL_MBED_TLS so that
 *     it uses the external mbed TLS library. That library must be built with
 *     MBEDTLS_AES_C enabled. Attach your hardware-accelerated AES to mbed TLS
 *     by defining MBEDTLS_AES_ALT; it will then be used both by users
 *     of arm_hal_aes.h, and other users of mbed TLS.
 *
 * 5) Platform with non-context-capable hardware assist, already using mbed TLS:
 *     If it's not possible, or too complex, to handle multiple contexts for the
 *     AES decode, then you will not be able to accelerate all mbed TLS users.
 *     Instead you can reserve the AES hardware for providing arm_hal_aes.h, so
 *     this becomes the same as case 2. Don't use this source file - implement
 *     arm_hal_aes.h yourself using your AES hardware. The external mbed TLS
 *     will use its software implementation.
 */

/* Get the API we are implementing from libService */
#include "platform/arm_hal_aes.h"

/* Either pull in the external mbed TLS header for its AES functions, or
 * pull in our own local cut-down copy of the mbed TLS code.
 */
#ifdef NS_USE_EXTERNAL_MBED_TLS
#include "mbedtls/aes.h"
#else
#include "aes_mbedtls.c"
#endif /* NS_USE_EXTERNAL_MBED_TLS */

static mbedtls_aes_context context;

void arm_aes_start(const uint8_t key[static 16])
{
    mbedtls_aes_init(&context);
    mbedtls_aes_setkey_enc(&context, key, 128);
}

void arm_aes_encrypt(const uint8_t src[static 16], uint8_t dst[static 16])
{
    mbedtls_aes_crypt_ecb(&context, MBEDTLS_AES_ENCRYPT, src, dst);
}

void arm_aes_finish(void)
{
    mbedtls_aes_free(&context);
}
