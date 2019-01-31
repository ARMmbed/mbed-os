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
#include "mbed_critical.h"

mbedtls_platform_context plat_ctx = { { 0 } };

int mbedtls_platform_setup( mbedtls_platform_context *unused_ctx )
{
    int ret = 0;

    core_util_atomic_incr_u32( ( volatile uint32_t * )&plat_ctx.reference_count, 1 );

    if( plat_ctx.reference_count == 1 )
    {
        /* call platform specific code to setup crypto driver */
        ret = crypto_platform_setup( &plat_ctx.platform_impl_ctx );
    }
    return ( ret );
}

void mbedtls_platform_teardown( mbedtls_platform_context *unused_ctx )
{
    core_util_atomic_decr_u32( ( volatile uint32_t * )&plat_ctx.reference_count, 1 );
    if( plat_ctx.reference_count < 1 )
    {
        /* call platform specific code to terminate crypto driver */
        crypto_platform_terminate( &plat_ctx.platform_impl_ctx );
        plat_ctx.reference_count = 0;
    }
}

#endif /* MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT*/
