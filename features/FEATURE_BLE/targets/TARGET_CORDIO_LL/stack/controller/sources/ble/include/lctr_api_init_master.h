/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller initiating master interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
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
