/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller initiating master interface file.
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

#ifndef LCTR_API_INIT_MASTER_H
#define LCTR_API_INIT_MASTER_H

#include "lctr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_INIT_MST
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master initiate task messages for \a LCTR_DISP_INIT dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_INIT_MSG_RESET                   = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Initiate events */
  LCTR_INIT_MSG_INITIATE,               /*!< Initiate API event. */
  LCTR_INIT_MSG_INITIATE_CANCEL,        /*!< Initiate cancel API event. */
  LCTR_INIT_MSG_TERMINATE,              /*!< Scan BOD terminated event. */
  LCTR_INIT_MSG_TOTAL                   /*!< Total number of initiate events. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstInitInit(void);
void LctrMstInitDefaults(void);

/*! \} */    /* LL_LCTR_API_INIT_MST */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_INIT_MASTER_H */
