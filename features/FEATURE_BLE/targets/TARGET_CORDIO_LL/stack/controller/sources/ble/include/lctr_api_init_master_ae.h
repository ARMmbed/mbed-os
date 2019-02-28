/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller extended initiating master interface file.
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
