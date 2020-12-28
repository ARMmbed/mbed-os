/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller BIS slave interface file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef LCTR_API_BIS_H
#define LCTR_API_BIS_H

#include "lctr_api.h"
#include "lctr_api_adv_acad.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_BIS
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      BIG task messages for \a LCTR_DISP_BIG_SYNC dispatcher. */
typedef enum
{
  /* Broadcast events */
  LCTR_MST_BIG_MSG_RESET            = LCTR_MSG_RESET,
  /* Host/API events */
  LCTR_MST_BIG_API_CREATE_SYNC,     /*!< BIG Create Sync API event. */
  LCTR_MST_BIG_API_TERMINATE_SYNC,  /*!< BIG Terminate Sync API event. */
  /* Receive remote events */
  LCTR_MST_BIG_ACAD_BIG_INFO,       /*!< BIG Info received. */
  LCTR_MST_BIG_PDU_TERM,            /*!< Remote terminate received. */
  /* Internal events */
  LCTR_MST_BIG_INT_SYNC_TIMEOUT,    /*!< BIG Sync timeout expired. */
  LCTR_MST_BIG_INT_MIC_FAILED,      /*!< BIS PDU received with MIC failure. */
  LCTR_MST_BIG_INT_TERMINATED_SYNC, /*!< BIG Sync termination complete. */

  LCTR_MST_BIG_MSG_TOTAL            /*!< Total number of BIG events. */
} LctrMstBigMsg_t;

/*! \brief      Minimum BIS Handle number. */
#define LL_MIN_BIS    0x01

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ACAD BIG Info message. */
typedef struct
{
  lctrMsgHdr_t      hdr;            /*!< Message header. */
  LctrAcadBigInfo_t data;           /*!< BIG Info data. */
} LctrBigInfoMsg_t;

/*! \brief      BIG messages. */
typedef union
{
  lctrMsgHdr_t      hdr;            /*!< Common message header. */
  LctrBigInfoMsg_t  bigInfo;        /*!< BIG Info message. */
} lctrMstBigMsg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstBisInit(void);

/* Control */
uint8_t LctrMstBigCreateSync(LlBigCreateSync_t *pParam);
void LctrMstBigTerminateSync(uint8_t bisHandle);

#ifdef __cplusplus
};
#endif

/*! \} */    /* LL_LCTR_API_BIS */

#endif /* LCTR_API_BIS_H */
