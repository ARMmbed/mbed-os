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

#ifndef LCTR_API_BIS_SLAVE_H
#define LCTR_API_BIS_SLAVE_H

#include "lctr_api.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave BIS broadcasting task messages for \a LCTR_DISP_BIG_BCST dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_SLV_BIG_MSG_RESET                = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Host/API events */
  LCTR_SLV_BIG_MSG_CREATE_BIG,          /*!< BIG slave creates BIG API event. */
  LCTR_SLV_BIG_MSG_CH_MAP_UPD,          /*!< BIG channel map update. */
  LCTR_SLV_BIG_MSG_TERMINATE_BIG,       /*!< BIG slave terminates BIG API event. */
  /* Internal events */
  LCTR_SLV_BIG_MSG_TERMINATED,          /*!< BIG slave terminated internal event. */

  LCTR_SLV_BIG_MSG_TOTAL                /*!< Total number of BIG slave broadcasting events. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      BIG slave message data. */
typedef union
{
  lctrMsgHdr_t          hdr;        /*!< Message header. */
  lctrMsgHdr_t          term;       /*!< Terminate BIG message data. */
} LctrSlvBigMsg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
uint16_t LctrInitBisMem(uint8_t *pFreeMem, uint32_t freeMemSize);
void LctrSlvBisInit(void);

/* Control */
uint8_t LctrSlvBisCreateBig(LlCreateBig_t *pCreateBit);
uint8_t LctrSlvBisCreateBigTest(LlCreateBigTest_t *pCreateBigTest);
uint8_t LctrSlvBisTerminateBig(uint8_t bigHandle, uint8_t reason);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_BIS_SLAVE_H */
