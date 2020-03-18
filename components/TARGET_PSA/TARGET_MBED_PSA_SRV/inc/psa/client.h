/* Copyright (c) 2017-2018 ARM Limited
 *
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

#if defined(TARGET_TFM)
#include "interface/include/psa_client.h"
#else

#ifndef __MBED_OS_DEFAULT_PSA_CLIENT_API_H__
#define __MBED_OS_DEFAULT_PSA_CLIENT_API_H__

#include <stddef.h>
#include "psa/error.h"

#if !defined(UINT32_MAX)
#define UINT32_MAX ((uint32_t)-1)
#endif

#if !defined(INT32_MIN)
#define INT32_MIN   (-0x7fffffff - 1)
#endif

#define PSA_FRAMEWORK_VERSION   (0x0100) /**< Version of the PSA Framework API. */
#define PSA_VERSION_NONE        (0L)     /**< Identifier for an unimplemented Root of Trust (RoT) Service. */
#define PSA_CONNECTION_REFUSED  (INT32_MIN + 1)   /**< The return value from psa_connect() if the RoT Service or SPM was unable to establish a connection.*/
#define PSA_CONNECTION_BUSY     (INT32_MIN + 2)   /**< The return value from psa_connect() if the RoT Service rejects the connection for a transient reason.*/
#define PSA_DROP_CONNECTION     (INT32_MIN)       /**< The result code in a call to psa_reply() to indicate a nonrecoverable error in the client.*/
#define PSA_NULL_HANDLE         ((psa_handle_t)0)   /**< Denotes an invalid handle.*/

typedef int32_t psa_handle_t;

typedef struct psa_invec {
    const void *base;   /**< Starting address of the buffer.*/
    size_t len;         /**< Length in bytes of the buffer.*/
} psa_invec;


typedef struct psa_outvec {
    void *base;      /**< Starting address of the buffer.*/
    size_t len;      /**< Length in bytes of the buffer.*/
} psa_outvec;

#endif // __MBED_OS_DEFAULT_PSA_CLIENT_API_H__
#endif
