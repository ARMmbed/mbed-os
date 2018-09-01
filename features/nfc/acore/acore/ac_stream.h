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
 * \file ac_stream.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef ACORE_STREAM_H_
#define ACORE_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

struct __ac_istream;
struct __ac_ostream;

typedef struct __ac_istream ac_istream_t;
typedef struct __ac_ostream ac_ostream_t;

#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"

#include "acore/ac_buffer.h"

typedef void (*ac_istream_fn)(ac_buffer_t *pDataIn, bool *pClose, size_t maxLength, void *pUserParam);
typedef void (*ac_ostream_fn)(ac_buffer_t *pDataOut, bool closed, void *pUserParam);

//Input stream -- pulled by consumer
struct __ac_istream {
    ac_istream_fn fn;
    void *pUserParam;
};

//Output stream -- pushed by supplier
struct __ac_ostream {
    ac_ostream_fn fn;
    void *pUserParam;
};

//Called by supplier
void ac_istream_init(ac_istream_t *pac_istream, ac_istream_fn fn, void *pUserParam);
//Called by consumer
void ac_istream_pull(ac_istream_t *pac_istream, ac_buffer_t *pDataIn, bool *pClose, size_t maxLength);

//Called by consumer
void ac_ostream_init(ac_ostream_t *pac_ostream, ac_ostream_fn fn, void *pUserParam);
//Called by supplier
void ac_ostream_push(ac_ostream_t *pac_ostream, ac_buffer_t *pDataOut, bool closed);

#ifdef __cplusplus
}
#endif

#endif /* ACORE_STREAM_H_ */
