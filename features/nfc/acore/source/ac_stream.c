/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file stream.c
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#include "acore/ac_stream.h"
#include "acore/ac_macros.h"

//Called by supplier
void ac_istream_init(ac_istream_t *pac_istream, ac_istream_fn fn, void *pUserParam)
{
    pac_istream->fn = fn;
    pac_istream->pUserParam = pUserParam;
}

//Called by consumer
void ac_istream_pull(ac_istream_t *pac_istream, ac_buffer_t *pDataIn, bool *pClose, size_t maxLength)
{
    pac_istream->fn(pDataIn, pClose, maxLength, pac_istream->pUserParam);
}

//Called by consumer
void ac_ostream_init(ac_ostream_t *pac_ostream, ac_ostream_fn fn, void *pUserParam)
{
    pac_ostream->fn = fn;
    pac_ostream->pUserParam = pUserParam;
}

//Called by supplier
void ac_ostream_push(ac_ostream_t *pac_ostream, ac_buffer_t *pDataOut, bool closed)
{
    pac_ostream->fn(pDataOut, closed, pac_ostream->pUserParam);
}
