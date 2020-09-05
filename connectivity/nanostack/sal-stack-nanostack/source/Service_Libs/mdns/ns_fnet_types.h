/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef _NS_FNET_TYPES_H_
#define _NS_FNET_TYPES_H_

#include "ns_types.h"

typedef uint8_t fnet_uint8_t;       /*  8 bits */
typedef uint16_t fnet_uint16_t;     /* 16 bits */
typedef uint32_t fnet_uint32_t;     /* 32 bits */

typedef int8_t fnet_int8_t;     /*  8 bits */
typedef int16_t fnet_int16_t;   /* 16 bits */
typedef int32_t fnet_int32_t;   /* 32 bits */

typedef volatile fnet_uint8_t fnet_vuint8_t;     /*  8 bits */
typedef volatile fnet_uint16_t fnet_vuint16_t;   /* 16 bits */
typedef volatile fnet_uint32_t fnet_vuint32_t;   /* 32 bits */

#endif /* _NS_FNET_TYPES_H_ */
