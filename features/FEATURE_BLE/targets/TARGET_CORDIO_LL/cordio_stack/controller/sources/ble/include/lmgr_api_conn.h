/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager connection interface file.
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
