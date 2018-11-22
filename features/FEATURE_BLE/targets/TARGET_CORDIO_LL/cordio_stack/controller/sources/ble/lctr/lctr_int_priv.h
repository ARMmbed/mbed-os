/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller privacy interface file.
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

#ifndef LCTR_INT_PRIV_H
#define LCTR_INT_PRIV_H

#include "lctr_int.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Slave advertising state context. */
typedef struct
{
  wsfTimer_t        tmrResPrivAddrTimeout;    /*!< Resolvable private address timeout timer. */
} lctrPrivCtx_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrPrivCtx_t lctrPriv;


#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_PRIV_H */
