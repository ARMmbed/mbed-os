 /*
  *  crypto_platform.c
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

#include "platform_alt.h"
#include "nrf52840.h"
#include "sns_silib.h"
#if defined(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)

static CRYS_RND_WorkBuff_t  rndWorkBuff = { { 0 } } ;

int crypto_platform_setup( crypto_platform_ctx *ctx )
{
    NRF_CRYPTOCELL->ENABLE = 1;

    if( SaSi_LibInit( &ctx->rndState, &rndWorkBuff ) != 0 )
          return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

    return ( 0 );
}

void crypto_platform_terminate( crypto_platform_ctx *ctx )
{
    SaSi_LibFini( &ctx->rndState );
    NRF_CRYPTOCELL->ENABLE = 0;
}

#endif /* MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT */
