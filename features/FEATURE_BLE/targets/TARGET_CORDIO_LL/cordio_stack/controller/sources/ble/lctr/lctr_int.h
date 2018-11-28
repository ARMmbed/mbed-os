/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller interface file.
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

#ifndef LCTR_INT_H
#define LCTR_INT_H

#include "lctr_api.h"
#include "ll_defs.h"
#include "lmgr_api.h"
#include "ll_math.h"

#if (LL_ENABLE_TESTER)
#include "ll_tester_api.h"
#include "lctr_int_tester.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Minimum delay between connect indication and data channel in CONNECT_IND units. */
#define LCTR_DATA_CHAN_DLY              1       /* 1.25 ms */

/*! \brief      Minimum delay between auxiliary connect request and data channel using uncoded PHY in CONNECT_REQ units. */
#define LCTR_DATA_CHAN_DLY_AUX_UNCODED  2       /* 2.5 ms */

/*! \brief      Minimum delay between auxiliary connect request and data channel using coded PHY in CONNECT_REQ units. */
#define LCTR_DATA_CHAN_DLY_AUX_CODED    3       /* 3.75 ms */

/*! \brief      Convert connect indication ticks to scheduler ticks. */
#define LCTR_CONN_IND_TICKS(x)          ((x) * (1250 / LL_BLE_US_PER_TICK))

/*! \brief      Convert connect indication ticks to milliseconds (no divide, rounds up). */
#define LCTR_CONN_IND_MS(x)             ((x) + ((x) >> 2) + (((x) & 3) ? 1 : 0))

/*! \brief      Convert connect indication ticks to microseconds. */
#define LCTR_CONN_IND_US(x)             ((x) * 1250)

/*! \brief      Convert connect indication timeout ticks to milliseconds. */
#define LCTR_CONN_IND_TO_MS(x)          ((x) * 10)

/*! \brief      Convert microseconds to connection indication ticks. */
#define LCTR_US_TO_CONN_IND(x)          LL_MATH_DIV_1250(x)

/*! \brief      Convert BLE protocol ticks to microseconds. */
#define LCTR_BLE_TO_US(x)               ((x) * LL_BLE_US_PER_TICK)

/*! \brief      Convert periodic interval milliseconds to microseconds. */
#define LCTR_PER_INTER_TO_US(x)         ((x) * 1250)

/*! \brief      Convert periodic interval microseconds to milliseconds. */
#define LCTR_PER_INTER_TO_MS(x)         LL_MATH_DIV_1250(x)

/*! \brief      Convert periodic sync timeout unit to milliseconds. */
#define LCTR_PER_SYNC_TIMEOUT_TO_MS(x)  ((x) * 10)

/*! \brief      Fast termination supervision multiplier. */
#define LCTR_FAST_TERM_CNT              6

/*! \brief      Duration of a advertising packet in microseconds. */
#define LCTR_ADV_PKT_1M_US(len)         ((LL_PREAMBLE_LEN_1M + LL_AA_LEN + LL_ADV_HDR_LEN + len + LL_CRC_LEN) << 3)

/*! \brief      Duration of a connection indication packet in microseconds. */
#define LCTR_CONN_IND_PKT_1M_US         LCTR_ADV_PKT_1M_US(LL_CONN_IND_PDU_LEN)

/*! \brief      Extra area in ADVB buffer. */
#define LCTR_ADVB_BUF_EXTRA_SIZE        6

/*! \brief      Size for ADVB buffer allocation. */
#define LCTR_ADVB_BUF_SIZE              (WSF_MAX(BB_FIXED_ADVB_PKT_LEN, LL_ADVB_MAX_LEN) + LCTR_ADVB_BUF_EXTRA_SIZE)

/*! \brief      RSSI offset of extra data in ADVB buffer. */
#define LCTR_ADVB_BUF_OFFSET_RSSI       ((LCTR_ADVB_BUF_SIZE - LCTR_ADVB_BUF_EXTRA_SIZE) + 0)

/*! \brief      RPA offset of extra data in ADVB buffer. */
#define LCTR_ADVB_BUF_OFFSET_RX_RPA     ((LCTR_ADVB_BUF_SIZE - LCTR_ADVB_BUF_EXTRA_SIZE) + 1)

/*! \brief      CRC offset of extra data in ADVB buffer. */
#define LCTR_ADVB_BUF_OFFSET_CRC        ((LCTR_ADVB_BUF_SIZE - LCTR_ADVB_BUF_EXTRA_SIZE) + 2)

/*! \brief      LCTR Maximum span of scheduler elements. */
#define         LCTR_SCH_MAX_SPAN            0x80000000

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Channel parameters. */
typedef struct
{
  /* Channel parameters */
  uint8_t       lastChanIdx;        /*!< Current channel index. */
  uint8_t       numUsedChan;        /*!< Number of used channels. */
  uint64_t      chanMask;           /*!< Channel mask. */
  uint8_t       chanRemapTbl[LL_CHAN_DATA_MAX_IDX + 1]; /*!< Channel remapping table. */

  uint8_t       usedChSel;          /*!< Used channel selection. */
  uint16_t      chIdentifier;       /*!< Channel identifier. */
} lctrChanParam_t;

/*! \brief      Call signature of a reset handler. */
typedef void (*LctrResetHdlr_t)(void);

/*! \brief      Call signature of a message dispatch handler. */
typedef void (*LctrMsgDisp_t)(lctrMsgHdr_t *pMsg);

/*! \brief      Call signature of a message dispatch handler. */
typedef void (*LctrEvtHdlr_t)(void);

/*! \brief      Reservation manager callback signature. */
typedef void (*LctrRmCback_t)(uint32_t rsvnOffs[], uint32_t refTime);

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern LctrResetHdlr_t lctrResetHdlrTbl[LCTR_DISP_TOTAL];
extern LctrMsgDisp_t lctrMsgDispTbl[LCTR_DISP_TOTAL];
extern LctrEvtHdlr_t lctrEventHdlrTbl[LCTR_EVENT_TOTAL];
extern lctrMsgHdr_t *pLctrMsg;
extern LctrRmCback_t lctrGetConnOffsetsCback;
extern LctrRmCback_t lctrGetPerOffsetsCback;

/**************************************************************************************************
  Functions
**************************************************************************************************/

/* Helper routines. */
uint32_t lctrComputeAccessAddr(void);
uint8_t lctrComputeHopInc(void);
uint8_t lctrPeriodicSelectNextChannel(lctrChanParam_t *pChanParam, uint16_t eventCounter);
void lctrPeriodicBuildRemapTable(lctrChanParam_t *pChanParam);
uint16_t lctrCalcTotalAccuracy(uint8_t mstScaIdx);
uint32_t lctrComputeCrcInit(void);

/* Host events */
void lctrNotifyHostHwErrInd(uint8_t code);
void lctrNotifyHostConnectInd(uint16_t handle, uint8_t role, lctrConnInd_t *pConnInd,
                              uint8_t peerIdAddrType, uint64_t peerIdAddr, uint64_t peerRpa,
                              uint64_t localRpa, uint8_t status, uint8_t usedChSel);

/* State machine */
void lctrMstScanExecuteSm(uint8_t event);
void lctrMstInitExecuteSm(uint8_t event);
void lctrSlvAdvExecuteSm(uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief      Get operational mode flag.
 *
 *  \param      flag  Flag to check.
 *
 *  \return     TRUE if flag is set.
 *
 *  Get mode flag governing LL operations.
 */
/*************************************************************************************************/
static inline bool_t lctrGetOpFlag(uint32_t flag)
{
  return (lmgrCb.opModeFlags & flag) ? TRUE : FALSE;
}

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_H */
