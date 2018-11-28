/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager secure connections interface file.
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

#ifndef LMGR_API_SC_H
#define LMGR_API_SC_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_SC
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Secure connections parameter definition. */
typedef struct
{
  uint8_t   privKey[LL_ECC_KEY_LEN];   /*!< P-256 private key. */
  bool_t    eccOpActive;                /*!< An ECC generation operation is active. */
  bool_t    privKeySet;                 /*!< P-256 private key set; do not generate new one. */
} lmgrScCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrScCtrlBlk_t lmgrScCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrScInit(void);

/*! \} */    /* LL_LMGR_API_SC */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_SC_H */
