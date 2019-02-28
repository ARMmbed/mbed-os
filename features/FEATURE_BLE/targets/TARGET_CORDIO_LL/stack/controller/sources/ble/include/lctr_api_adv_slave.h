/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller advertising slave interface file.
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

#ifndef LCTR_API_ADV_SLAVE_H
#define LCTR_API_ADV_SLAVE_H

#include "lctr_api.h"
#include "lmgr_api_adv_slave.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_ADV_SLV
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave advertising task messages for \a LCTR_DISP_ADV dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_ADV_MSG_RESET                    = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Advertising events */
  LCTR_ADV_MSG_START,                   /*!< Advertising start API event. */
  LCTR_ADV_MSG_STOP,                    /*!< Advertising stop API event. */
  LCTR_ADV_MSG_INT_START,               /*!< Advertising start internal event. */
  LCTR_ADV_MSG_TERMINATE,               /*!< Advertising BOD terminated event. */
  LCTR_ADV_MSG_PARAM_UPD,               /*!< Advertising parameter update event. */
  LCTR_ADV_MSG_TOTAL                    /*!< Total number of advertising events. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising parameter message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  lmgrAdvParam_t    param;              /*!< Advertising parameters. */
} lctrAdvParamMsg_t;

/*! \brief      Link layer controller advertising message data. */
typedef union
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  lctrAdvParamMsg_t advParamUpd;        /*!< Advertising parameter update. */
} LctrAdvMsg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrSlvAdvInit(void);
void LctrSlvAdvDefaults(void);

/*! \} */    /* LL_LCTR_API_ADV_SLV */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_SLAVE_H */
