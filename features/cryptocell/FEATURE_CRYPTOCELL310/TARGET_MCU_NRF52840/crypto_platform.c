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
/* once https://github.com/ARMmbed/mbedtls/issues/1200 will be supported,
 * rndState should be part of mbedtls_platform_context
 * Until then, we should keep it global and extern */

CRYS_RND_State_t     rndState = { { 0 } } ;
CRYS_RND_WorkBuff_t  rndWorkBuff = { { 0 } } ;

int crypto_platform_setup( crypto_platform_ctx *ctx )
{
    NRF_CRYPTOCELL->ENABLE = 1;

    if( SaSi_LibInit( &rndState, &rndWorkBuff ) != 0 )
          return ( -1 );

    return ( 0 );
}

void crypto_platform_terminate( crypto_platform_ctx *ctx )
{
    SaSi_LibFini( &rndState );
    NRF_CRYPTOCELL->ENABLE = 0;
}

#endif /* MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT */
