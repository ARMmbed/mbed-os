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
#include "cc_platform.h"
#include "crys_rnd.h"

/**
 * \brief   The platform context structure.
 *
 * \note    This structure may be used to assist platform-specific
 *          setup or teardown operations.
 */
typedef struct {
    cc_platform_ctx platform_impl_ctx; /** A context holding all the partner's platform specific context */
    /*
     * Add CRYS_RND_State_t rndState; when https://github.com/ARMmbed/mbedtls/issues/1200 is supported
     * */
}
mbedtls_platform_context;


/**
 * \brief   This function performs any partner platform initialization operations,
 *          needed top enable CryptoCell.
 *
 * \param   ctx     The platform specific context.
 *
 * \return          \c 0 on success.
 *
 * \note    This function is intended to allow platform-specific initialization for CryptoCell,
 *          and is called before initializing the CC library(SaSi_LibInit). Its
 *          implementation is platform-specific, and its implementation MUST be provided.
 *
 */
int cc_platform_setup( cc_platform_ctx *ctx );

/**
 * \brief   This function performs any partner platform teardown operations, to disable CryptoCell.
 *
 * \param   ctx     The platform specific context.
 *
 * \note    This function is called after terminating CC library(SaSi_LibFini)
 *          and intended to free any resource used for CryptoCell by the platform.
 *          Its implementation is platform-specific,and its implementation MUST be provided.
 *
 */
void cc_platform_terminate( cc_platform_ctx *ctx );

#endif /* __PLATFORM_ALT__ */

