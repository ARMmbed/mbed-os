/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Link layer (LL) Host Controller Interface (HCI) initialization implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "chci_api.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "bb_api.h"
#include "bb_drv.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Dispatcher table. */
lhciCmdHandler_t lhciCmdTbl[LHCI_MSG_TOTAL];

/*! \brief      Dispatcher table. */
lhciEvtHandler_t lhciEvtTbl[LHCI_MSG_TOTAL];

/*! \brief      Receive pending handler. */
lhciServiceAcl_t lhciServiceAcl;

/*! \brief      Persistent control block */
lhciPersistCb_t lhciPersistCb;

/*! \brief      LL HCI subsystem control block. */
lhciCb_t lhciCb;

/*! \brief      Handler duration watermark in microseconds. */
uint16_t lhciHandlerWatermarkUsec = 0;

/*************************************************************************************************/
/*!
 *  \brief  Unpack an advertising channel PDU header.
 *
 *  \param  pHdr        Unpacked header.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Header length.
 */
/*************************************************************************************************/
static inline uint8_t lhciUnpackHdr(LhciHdr_t *pHdr, const uint8_t *pBuf)
{
  const uint8_t len = sizeof(uint16_t) + sizeof(uint8_t);

  BSTREAM_TO_UINT16(pHdr->opCode, pBuf);
  pHdr->len = *pBuf;

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief      LL HCI message dispatch handler.
 *
 *  \param      event       WSF event.
 *  \param      pMsg        WSF message.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LhciHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  bool_t startTimeValid;
  uint32_t startTime;
  uint32_t endTime;

  startTimeValid = BbDrvGetTimestamp(&startTime);

  if (event & LHCI_EVT_ACL_RCVD)
  {
    uint8_t *pAclBuf;
    wsfHandlerId_t handlerId;

    while ((pAclBuf = WsfMsgDeq(&lhciPersistCb.aclQ, &handlerId)) != NULL)
    {
      LlSendAclData(pAclBuf);
    }
  }

  if (event & LHCI_EVT_CMD_RCVD)
  {
    uint8_t *pCmdBuf;
    wsfHandlerId_t handlerId;
    LhciHdr_t hdr;

    while ((pCmdBuf = WsfMsgDeq(&lhciPersistCb.cmdQ, &handlerId)) != NULL)
    {
      uint8_t *pPldBuf = pCmdBuf + lhciUnpackHdr(&hdr, pCmdBuf);

      if (!lhciCommonDecodeCmdPkt(&hdr, pPldBuf))
      {
        unsigned int msg = 0;
        do
        {
          if (lhciCmdTbl[msg] && lhciCmdTbl[msg](&hdr, pPldBuf))
          {
            break;
          }
        } while (++msg < LHCI_MSG_TOTAL);

        /* Unhandled command. */
        if (msg == LHCI_MSG_TOTAL)
        {
          uint8_t *pEvtBuf;

          /* Send command status with unknown command status. */
          if ((pEvtBuf = lhciAllocEvt(HCI_CMD_STATUS_EVT, HCI_LEN_CMD_STATUS)) != NULL)
          {
            lhciPackCmdStatusEvt(pEvtBuf, HCI_ERR_UNKNOWN_CMD, hdr.opCode);
            lhciSendEvt(pEvtBuf);
          }
        }
      }

      WsfMsgFree(pCmdBuf);
    }
  }

  if (event & LHCI_EVT_SEND_CMPL)
  {
    lhciPersistCb.evtTrPending = FALSE;
    ChciTrNeedsService(CHCI_TR_PROT_BLE);
  }

  if (event & LHCI_EVT_HW_ERR)
  {
    uint8_t *pEvtBuf;

    if ((pEvtBuf = lhciAllocEvt(HCI_HW_ERROR_EVT, HCI_LEN_HW_ERR)) != NULL)
    {
      uint8_t *pBuf = (uint8_t *)pEvtBuf;
      UINT8_TO_BSTREAM(pBuf, lhciCb.hwErrorCode);

      lhciSendEvt(pEvtBuf);
    }
  }

  if (startTimeValid &&
      BbDrvGetTimestamp(&endTime))
  {
    uint32_t durUsec = BB_TICKS_TO_US(endTime - startTime);
    if (lhciHandlerWatermarkUsec < durUsec)
    {
      lhciHandlerWatermarkUsec = durUsec;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Queue a received buffer for processing by the message handler.
 *
 *  \param  type        Type of message.
 *  \param  pBuf        Pointer to received message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lhciRecv(uint8_t type, uint8_t *pBuf)
{
  switch (type)
  {
    case CHCI_TR_TYPE_DATA:
      WsfMsgEnq(&lhciPersistCb.aclQ, 0, pBuf);
      WsfSetEvent(lhciPersistCb.handlerId, LHCI_EVT_ACL_RCVD);
      break;

    case CHCI_TR_TYPE_CMD:
      WsfMsgEnq(&lhciPersistCb.cmdQ, 0, pBuf);
      WsfSetEvent(lhciPersistCb.handlerId, LHCI_EVT_CMD_RCVD);
      break;

    default:
      WsfMsgFree(pBuf);
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Signal transport send completion.
 *
 *  \param  type    Type of message.
 *  \param  pBuf    Pointer to transmitted message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lhciSendComplete(uint8_t type, uint8_t *pBuf)
{
  switch (type)
  {
    case CHCI_TR_TYPE_EVT:
      if (pBuf[0] == HCI_LE_META_EVT)
      {
        switch (pBuf[2])
        {
          case HCI_LE_ADV_REPORT_EVT:
          case HCI_LE_DIRECT_ADV_REPORT_EVT:
          case HCI_LE_EXT_ADV_REPORT_EVT:
          case HCI_LE_PER_ADV_REPORT_EVT:
            lhciCb.numAdvReport--;
            break;
          case HCI_LE_SCAN_REQ_RCVD_EVT:
            lhciCb.numScanReqRcvd--;
            break;
          default:
            break;
        }
      }
      WsfMsgFree(pBuf);
      break;

    case CHCI_TR_TYPE_DATA:
      WsfMsgFree(pBuf);
      LlRecvAclDataComplete(1);
      break;

    default:
      WSF_ASSERT(FALSE);
      break;
  }

  WsfSetEvent(lhciPersistCb.handlerId, LHCI_EVT_SEND_CMPL);
}

/*************************************************************************************************/
/*!
 *  \brief  Service LHCI for transport.
 *
 *  \param  pType   Storage for type of message.
 *  \param  pLen    Storage for length of message.
 *  \param  pBuf    Storage for pointer to transmitted message.
 *
 *  \return TRUE if message ready.
 */
/*************************************************************************************************/
bool_t lhciService(uint8_t *pType, uint16_t *pLen, uint8_t **pBuf)
{
  uint8_t  *pBufTemp;
  uint16_t len;

  if (!lhciPersistCb.evtTrPending)
  {
    wsfHandlerId_t handlerId;

    if ((pBufTemp = WsfMsgDeq(&lhciPersistCb.evtQ, &handlerId)) != NULL)
    {
      len = pBufTemp[1] + HCI_EVT_HDR_LEN;

      lhciPersistCb.evtTrPending = TRUE;
      *pType = CHCI_TR_TYPE_EVT;
      *pLen  = len;
      *pBuf  = pBufTemp;
      return TRUE;
    }
    else
    {
      if (lhciServiceAcl != NULL)
      {
        /* Additionally check if ACL data needs servicing. */
        if ((pBufTemp = lhciServiceAcl()) != NULL)
        {
          BYTES_TO_UINT16(len, &pBufTemp[2]);
          len += HCI_ACL_HDR_LEN;

          lhciPersistCb.evtTrPending = TRUE;
          *pType = CHCI_TR_TYPE_DATA;
          *pLen  = len;
          *pBuf  = pBufTemp;
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Signal transport hardware error.
 *
 *  \param  code        Error code.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lhciSendHwError(uint8_t code)
{
  lhciCb.hwErrorCode = code;
  WsfSetEvent(lhciPersistCb.handlerId, LHCI_EVT_HW_ERR);
}

/*************************************************************************************************/
/*!
 *  \brief      Reset HCI state.
 *
 *  \return     None.
 *
 *  This function is called at the end of a HCI reset.
 */
/*************************************************************************************************/
void lhciReset(void)
{
  memset(&lhciCb, 0, sizeof(lhciCb));

  lhciCb.evtMsk = LHCI_DEF_EVT_MASK;
  lhciCb.evtMskPg2 = LHCI_DEF_EVT_PG2_MASK;
  lhciCb.leEvtMsk = LHCI_DEF_LE_EVT_MASK;
}
