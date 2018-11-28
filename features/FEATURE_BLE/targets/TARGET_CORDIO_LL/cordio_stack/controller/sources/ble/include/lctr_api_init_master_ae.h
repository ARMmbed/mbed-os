/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller extended initiating master interface file.
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

#ifndef LCTR_API_INIT_MASTER_AE_H
#define LCTR_API_INIT_MASTER_AE_H

#include "lctr_api.h"
#include "wsf_assert.h"
#include "lctr_api_init_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_INIT_MST_AE
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master extended initiate task messages for \a LCTR_DISP_EXT_INIT dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_EXT_INIT_MSG_RESET                   = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Initiate events */
  LCTR_EXT_INIT_MSG_INITIATE,               /*!< Initiate API event. */
  LCTR_EXT_INIT_MSG_INITIATE_CANCEL,        /*!< Initiate cancel API event. */
  LCTR_EXT_INIT_MSG_TERMINATE,              /*!< Initiate scan BOD terminated event. */
  LCTR_EXT_INIT_MSG_TOTAL                   /*!< Total number of initiate events. */
};

/*! \brief  LCTR_EXT_INIT_MSG_INITIATE_CANCEL and LCTR_INIT_MSG_INITIATE_CANCEL shall be aligned. */
WSF_CT_ASSERT(((int)LCTR_EXT_INIT_MSG_INITIATE_CANCEL == (int)LCTR_INIT_MSG_INITIATE_CANCEL));

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstExtInitInit(void);
void LctrMstExtInitDefaults(void);

/* Control */
void LctrMstExtInitParam(uint8_t initPhy, const LlExtInitScanParam_t *pScanParam, const LlConnSpec_t *pConnSpec);
void LctrMstExtInitSetScanPhy(uint8_t scanPhy);
void LctrMstExtInitClearScanPhy(uint8_t scanPhy);

/*! \} */    /* LL_LCTR_API_INIT_MST_AE */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_INIT_MASTER_AE_H */
