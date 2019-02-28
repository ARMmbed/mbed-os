/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager connection interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
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

#ifndef LMGR_API_CONN_H
#define LMGR_API_CONN_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_CONN
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Slave role device parameter definition. */
typedef struct
{
  uint8_t       availTxBuf;             /*!< Available number of transmit buffers. */
  uint8_t       availRxBuf;             /*!< Available number of receive buffers. */

  uint16_t      maxTxLen;               /*!< Default maximum number of Data PDU bytes. */
  uint16_t      maxTxTime;              /*!< Default maximum microseconds for a Data PDU. */

  wsfQueue_t    rxDataQ;                /*!< Receive Data PDU (LE-U or LE-C) queue. */

  uint32_t      dataPendMsk;            /*!< Bitmask of connection handles with new pending data. */

  uint8_t       allPhys;                /*!< Default all PHYs. */
  uint8_t       txPhys;                 /*!< Default transmitter PHYs. */
  uint8_t       rxPhys;                 /*!< Default receiver PHYs. */

} lmgrConnCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrConnCtrlBlk_t lmgrConnCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrConnInit(void);

/*! \} */    /* LL_LMGR_API_CONN */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_CONN_H */
