 /*
  *  platform_alt.c
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

#include "mbedtls/platform.h"
#if defined(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)
#include "sns_silib.h"

/* once https://github.com/ARMmbed/mbedtls/issues/1200 will be supported,
 * rndState should be part of mbedtls_platform_context
 * Until then, we should keep it global and extern */

CRYS_RND_State_t     rndState = { { 0 } } ;
CRYS_RND_WorkBuff_t  rndWorkBuff = { { 0 } } ;


int mbedtls_platform_setup( mbedtls_platform_context *ctx )
{
    int ret = 0;
    if( ctx == NULL )
        return ( -1 );

    /* call platform specific code to setup CC driver*/
    if( ( ret = cc_platform_setup( &ctx->platform_impl_ctx ) ) != 0 )
        return ( ret );

    if( SaSi_LibInit( &rndState, &rndWorkBuff ) != 0 )
        return ( -1 );
    return ( 0 );
}

void mbedtls_platform_teardown( mbedtls_platform_context *ctx )
{
    if( ctx == NULL )
        return;

    SaSi_LibFini( &rndState );
    cc_platform_terminate( &ctx->platform_impl_ctx );
}

#endif /* MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT*/
