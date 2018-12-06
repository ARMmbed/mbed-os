/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller advertising slave interface file.
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
