/*
 *  Definitions for authenticated encryption algorithms common to
 *  all crypto devices (AES, CRYPTO, etc.)
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

#ifndef __SILICON_LABS_AESDRV_AUTHENCR_H
#define __SILICON_LABS_AESDRV_AUTHENCR_H

#define CCM_MAX_DATA_LENGTH         (0x10000-1)

Ecode_t AESDRV_CCM_Generalized(AESDRV_Context_t*       pAesdrvContext,
                               const uint8_t*          pDataInput,
                                     uint8_t*          pOutputData,
                               const uint32_t          dataLength,
                               const uint8_t*          pHdr,
                               const uint32_t          hdrLength,
                               const uint8_t*          pKey,
                               const uint32_t          keyLength,
                               const uint8_t*          pNonce,
                               const uint32_t          nonceLength,
                               uint8_t*                pAuthTag,
                               const uint8_t           authTagLength,
                               const bool              encrypt,
                               const bool              encryptedPayload);

#endif /* __SILICON_LABS_AESDRV_AUTHENCR_H */
