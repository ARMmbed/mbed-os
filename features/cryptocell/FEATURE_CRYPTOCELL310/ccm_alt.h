/*
 *  ccm_alt.h
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

#ifndef __CCM_ALT__
#define __CCM_ALT__

#if defined(MBEDTLS_CCM_ALT)
#include "crys_aesccm.h"

typedef struct {
    CRYS_AESCCM_Key_t     cipher_key;    /*!< cipher key used */
    CRYS_AESCCM_KeySize_t key_size;
}
mbedtls_ccm_context;

#endif /* MBEDTLS_CCM_ALT */
#endif /* __CCM_ALT__ */
