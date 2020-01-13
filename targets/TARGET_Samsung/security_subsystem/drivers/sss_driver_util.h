/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef SSS_DRIVER_UTIL_H
#define SSS_DRIVER_UTIL_H

/*************** Include Files ********************************************/
#include "sss_common.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/
#define SSS_HMAC    (0x001)
#define SSS_HASH    (0x002)
#define SSS_DH      (0x003)
#define SSS_ECDH    (0x004)
#define SSS_RSA     (0x005)
#define SSS_ECDSA   (0x006)

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

int _sss_check_oid(u32 inputoid, u32 algorithm);

int _sss_is_zero(const u08 *pu08Src, u32 u32ByteLen);
int _sss_is_valid(const u08 *pu08Src, u32 u32ByteLen);

int _sss_MB_to_BN(stBIG_NUM *pstBNSDst, u32 *pu32Src, u32 u32RespondByteLen);
int _sss_BN_to_MB(const stBIG_NUM *pstBNSrc, u32 *pu32Dst, u32 u32RequestByteLen);
int _sss_OS_to_MB(const stOCTET_STRING *pstOSSrc, u32 *pu32Dst, u32 u32RequestByteLen);
int _sss_MB_to_OS(stOCTET_STRING *pstOSSDst, u32 *pu32Src, u32 u32RespondByteLen);

int _sss_memcpy_u08(u08 *pu08Dst, u08 *pu08Src, u32 u32Size);
int _sss_memset_u08(u08 *pu08Dst, const u08 u08Src, u32 u32Size);
int _sss_memcpy_mailbox(u32 *pu32Dst, u32 *pu32Src, u32 u32Size_byte_len);

#endif  /* SSS_UTIL_H */
