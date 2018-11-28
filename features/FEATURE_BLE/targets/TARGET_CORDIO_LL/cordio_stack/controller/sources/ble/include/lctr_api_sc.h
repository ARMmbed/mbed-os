/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller secure connections interface file.
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

#ifndef LCTR_API_SC_H
#define LCTR_API_SC_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_SC
 *  \{
 */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrScInit(void);

/* Execution. */
uint8_t LctrGenerateP256KeyPair(void);
uint8_t LctrGenerateDhKey(const uint8_t *pPubKey, const uint8_t *pPrivKey);

/*! \} */    /* LL_LCTR_API_SC */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_SC_H */
