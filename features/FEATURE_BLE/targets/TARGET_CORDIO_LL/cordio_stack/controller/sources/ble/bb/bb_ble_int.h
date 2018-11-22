/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal BLE baseband interface file.
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

#ifndef BB_BLE_INT_H
#define BB_BLE_INT_H

#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "bb_ble_drv.h"
#include "bb_drv.h"
#include "ll_defs.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum length of an advertising packet. */
#define BB_ADVB_MAX_LEN         WSF_MAX(BB_FIXED_ADVB_PKT_LEN, LL_ADVB_MAX_LEN)

/*! \brief      Mark the start of an ISR. */
#define BB_ISR_START()          bbIsrStartTime = BbDrvGetCurrentTime()

/*! \brief      Mark the ISR duration, recording the high watermark. */
#define BB_ISR_MARK(x)          x = WSF_MAX(x, BB_TICKS_TO_US(BbDrvGetCurrentTime() - bbIsrStartTime))

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Execution operation function. */
typedef void (*bbBleExecOpFn_t)(BbOpDesc_t *pBod, BbBleData_t *pBle);

/*! \brief      BB control block. */
typedef struct
{
  struct
  {
    bbBleExecOpFn_t execOpCback;        /*!< Execute operation handler. */
    bbBleExecOpFn_t cancelOpCback;      /*!< Cancel operation handler. */
  } opCbacks[BB_BLE_OP_NUM];            /*!< Operation handlers. */

  uint8_t evtState;                     /*!< Action state of the currently operating BOD. */
  uint8_t advChIdx;                     /*!< Current advertising channel index. */
  uint32_t lastScanStart;               /*!< Last scan start time. */

  BbBleDrvDataParam_t bbParam;          /*!< Baseband data parameters. */

  uint16_t rxDataLen;                   /*!< Receive data buffer length. */
  uint8_t *pRxDataBuf;                  /*!< Current Rx data buffer. */
} bbBleCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern bbBleCtrlBlk_t bbBleCb;

/* ISR start time. */
extern uint32_t bbIsrStartTime;

/*************************************************************************************************/
/*!
 *  \brief      Register operation handlers.
 *
 *  \param      opType          Operation type.
 *  \param      execOpCback     Execute operation callback.
 *  \param      cancelOpCback   Cancel operation callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void bbBleRegisterOp(uint8_t opType, bbBleExecOpFn_t execOpCback, bbBleExecOpFn_t cancelOpCback);

#ifdef __cplusplus
};
#endif

/*************************************************************************************************/
/*!
 *  \brief      Clear IFS in operation.
 *
 *  \return     None.
 *
 *  The BB may choose not to enable IFS after the next Tx or Rx.
 */
/*************************************************************************************************/
static inline void bbBleClrIfs(void)
{
  BbBleDrvOpParam_t opParams = { .ifsSetup = FALSE, .ifsUsec = 0 };
  BbBleDrvSetOpParams(&opParams);
}

/*************************************************************************************************/
/*!
 *  \brief      Set IFS in operation.
 *
 *  \return     None.
 *
 *  The BB must enable IFS after the next Tx or Rx.
 */
/*************************************************************************************************/
static inline void bbBleSetIfs(void)
{
  BbBleDrvOpParam_t opParams = { .ifsSetup = TRUE, .ifsUsec = LL_BLE_TIFS_US };
  BbBleDrvSetOpParams(&opParams);
}

#endif /* BB_BLE_INT_H */
