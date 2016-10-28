/*
 *  Common interface for AES based algorithms for the CRYPTO hw module.
 *
 *  Copyright (C) 2016 Silicon Labs, http://www.silabs.com
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
 */
#ifndef __SILICON_LABS_AESDRV_COMMON_CRYPTO_H
#define __SILICON_LABS_AESDRV_COMMON_CRYPTO_H

#include "aesdrv_internal.h"

void AESDRV_HwIoSetup(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          pData,
                      uint32_t          authDataLength,
                      uint32_t          textLength);

#endif /* __SILICON_LABS_AESDRV_COMMON_CRYPTO_H */
