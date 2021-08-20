/** @file
 * Copyright (c) 2018-2020, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _PAL_CLIENT_API_H_
#define _PAL_CLIENT_API_H_

#include "pal_common.h"

#define PSA_VERSION_NONE            (0)
#define PSA_CONNECTION_REFUSED      (INT32_MIN + 1)
#define PSA_CONNECTION_BUSY         (INT32_MIN + 2)
#define PSA_DROP_CONNECTION         (INT32_MIN)
#define PSA_NULL_HANDLE             ((psa_handle_t)0)

typedef int32_t psa_status_t;
typedef int32_t psa_handle_t;

typedef struct psa_invec {
    const void *base;
    size_t len;
} psa_invec;

typedef struct psa_outvec {
    void *base;
    size_t len;
} psa_outvec;

uint32_t psa_framework_version(void);
uint32_t psa_version(uint32_t sid);
psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version);
psa_status_t psa_call(psa_handle_t handle,
                      const psa_invec *in_vec,
                      size_t in_len,
                      psa_outvec *out_vec,
                      size_t out_len);
void psa_close(psa_handle_t handle);

uint32_t pal_ipc_framework_version(void);
uint32_t pal_ipc_version(uint32_t sid);
psa_handle_t pal_ipc_connect(uint32_t sid, uint32_t minor_version);
psa_status_t pal_ipc_call(psa_handle_t handle,
                          const psa_invec *in_vec,
                          size_t in_len,
                          psa_outvec *out_vec,
                          size_t out_len);
void pal_ipc_close(psa_handle_t handle);
#endif /* _PAL_CLIENT_API_H_ */
