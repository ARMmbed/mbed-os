/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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

#include "ns_sha256.h"

void ns_sha256_init( ns_sha256_context *ctx )
{

}

void ns_sha256_free( ns_sha256_context *ctx )
{

}

void ns_sha256_clone( ns_sha256_context *dst,
                      const ns_sha256_context *src )
{

}

void ns_sha256_starts( ns_sha256_context *ctx )
{

}

void ns_sha256_update( ns_sha256_context *ctx, const void *input,
                       size_t ilen )
{

}

void ns_sha256_finish( ns_sha256_context *ctx, void *output )
{

}

void ns_sha256_finish_nbits( ns_sha256_context *ctx,
                             void *output, unsigned obits )
{

}


void ns_sha256( const void *input, size_t ilen,
                void *output )
{

}

void ns_sha256_nbits( const void *input, size_t ilen,
                      void *output, unsigned obits )
{

}
