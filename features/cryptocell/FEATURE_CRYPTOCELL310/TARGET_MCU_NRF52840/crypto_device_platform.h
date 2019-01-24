/*
  *  crypto_platform.h
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
#ifndef __CRYPTO_PLATFORM_H_
#define __CRYPTO_PLATFORM_H_
#if defined(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)
#include "crys_rnd.h"
/**
 * \brief   The CC platform context structure.
 *
 * \note    This structure may be used to assist platform-specific
 *          setup or teardown operations.
 */
typedef struct {
     CRYS_RND_State_t rndState;
}
crypto_platform_ctx;
#endif /* MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT */
#endif /* __CRYPTO_PLATFORM_H_ */
