/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager privacy interface file.
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

#ifndef LMGR_API_PRIV_H
#define LMGR_API_PRIV_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_PRIV
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Slave role device parameter definition. */
typedef struct
{
  uint32_t          resPrivAddrTimeout; /*!< Resolvable private address timeout. */
  uint8_t           numPendingAddrRes;  /*!< Number of pending address resolutions. */
} lmgrPrivCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrPrivCtrlBlk_t lmgrPrivCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrPrivInit(void);

/*! \} */    /* LL_LMGR_API_PRIV */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_PRIV_H */
