/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller PHY features interface file.
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

#ifndef LCTR_API_PHY_H
#define LCTR_API_PHY_H

#include "lctr_api.h"
#include "lmgr_api_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrSlvPhyConnInit(void);
void LctrMstPhyConnInit(void);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_PHY_H */
