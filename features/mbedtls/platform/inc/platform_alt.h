/*
  *  platform_alt.h
  *
  *  Copyright (C) 2018, Arm Limited, All Rights Reserved
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
  */

#ifndef __PLATFORM_ALT__
#define __PLATFORM_ALT__
#include "platform_mbed.h"
#if defined(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)
#include "crypto_device_platform.h"
/**
 * \brief   The platform context structure.
 *
 * \note    This structure may be used to assist platform-specific
 *          setup or teardown operations.
 */
typedef struct {
    crypto_platform_ctx platform_impl_ctx; /* A context holding all the platform specific context for cryptography. Should be defined in crypto_platform.h */
    int reference_count;
}
mbedtls_platform_context;


/**
 * \brief   This function performs any platform initialization operations,
 *          needed for setting up cryptographic modules.
 *
 * \param   ctx     The platform specific context.
 *
 * \return          \c 0 on success.
 *
 * \note    This function is intended to allow platform-specific initialization for Mbed TLS,
 *          and is called before initializing the Mbed TLS functions. Its
 *          implementation is platform-specific, and its implementation MUST be provided.
 *
 */
int crypto_platform_setup( crypto_platform_ctx *unused_ctx );

/**
 * \brief   This function performs any  platform teardown operations, to disable cryptographic operations.
 *
 * \param   ctx     The platform specific context.
 *
 * \note    This function is intended to free any resource used Mbed TLS by the platform.
 *          Its implementation is platform-specific,and its implementation MUST be provided.
 *
 */
void crypto_platform_terminate( crypto_platform_ctx *unused_ctx );
#endif
#endif /* __PLATFORM_ALT__ */

