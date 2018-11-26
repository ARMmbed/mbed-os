/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Baseband driver internal interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef BB_BLE_DRV_INT_H
#define BB_BLE_DRV_INT_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Inline encryption */
void BbBleDrvInlineEncryptTxEnable(bool_t enable);
void BbBleDrvInlineEncryptSetPacketCount(uint64_t count);
void BbBleDrvInlineEncryptDecryptSuppressMic(bool_t enable);
void BbBleDrvInlineEncryptDecryptSetKey(uint8_t * key);
void BbBleDrvInlineEncryptDecryptSetIv(uint8_t * iv);
void BbBleDrvInlineEncryptDecryptSetDirection(uint8_t dir);

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_DRV_INT_H */
