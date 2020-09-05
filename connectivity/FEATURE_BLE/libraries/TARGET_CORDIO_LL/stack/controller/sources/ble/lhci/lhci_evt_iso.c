/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI event module implementation file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#include "lhci_int.h"
#include "chci_api.h"
#include "hci_defs.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack a number of completed packets event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  numHandles  Number of handles.
 *  \param  pHandle     Array of CIS or BIS handle.
 *  \param  pNumPkts    Array of number of completed packets.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackNumCompPktsEvt(uint8_t *pBuf, uint8_t numHandles, uint16_t *pHandle, uint16_t *pNumPkts)
{
  const uint8_t len = HCI_LEN_NUM_CMPL_PKTS(numHandles);

  UINT8_TO_BSTREAM (pBuf, numHandles);

  for (unsigned int i = 0; i < numHandles; i++)
  {
    UINT16_TO_BSTREAM(pBuf, pHandle[i]);
    UINT16_TO_BSTREAM(pBuf, pNumPkts[i]);
  }

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Free the received ISO data for test Rx.
 *
 *  \param  pIsoData    ISO data buffer.
 */
/*************************************************************************************************/
static void lhciFreeRcvdIsoData(uint8_t *pIsoData)
{
  WsfMsgFree(pIsoData);
  LlRecvIsoDataComplete(1);
}

/*************************************************************************************************/
/*!
 *  \brief  LL ISO send complete event handler.
 *
 *  \param  numHandles  Number of handles.
 *  \param  pHandle     Array of CIS or BIS handle.
 *  \param  pNumPkts    Array of number of completed packets.
 */
/*************************************************************************************************/
void lhciIsoSendComplete(uint8_t numHandles, uint16_t *pHandle, uint16_t *pNumPkts)
{
  uint8_t *pEvtBuf;

  if ((pEvtBuf = lhciAllocEvt(HCI_NUM_CMPL_PKTS_EVT, HCI_LEN_NUM_CMPL_PKTS(numHandles))) != NULL)
  {
    lhciPackNumCompPktsEvt(pEvtBuf, numHandles, pHandle, pNumPkts);

    lhciSendEvt(pEvtBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Sink LL ACL.
 *
 *  \param  pBuf      Buffer to sink.
 *
 *  \return TRUE if buffer sink.
 */
/*************************************************************************************************/
static bool_t lhciSinkIso(uint8_t *pBuf)
{
  if (lhciIsoCb.recvIsoSink)
  {
    uint8_t len;

    len = pBuf[2];
    lhciIsoCb.recvIsoPktCnt++;
    lhciIsoCb.recvIsoOctetCnt += len;

    WsfMsgFree(pBuf);
    LlRecvIsoDataComplete(1);
    return TRUE;
  }
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Receive LL ISO.
 *
 *  \return Pointer to buffer for transport.
 */
/*************************************************************************************************/
uint8_t *lhciRecvIso(void)
{
  uint8_t *pIsoData;

  if ((pIsoData = LlRecvIsoData()) != NULL)
  {
    if (!lhciSinkIso(pIsoData))
    {
      return pIsoData;
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  LL ISO receive pending event handler.
 *
 *  \param  numHandles  Number of handles.
 *  \param  pHandle     Array of CIS or BIS handle.
 *  \param  pNumPkts    Array of number of pending packets.
 */
/*************************************************************************************************/
void lhciIsoRecvPending(uint8_t numHandles, uint16_t *pHandle, uint16_t *pNumPkts)
{
  uint8_t *pIsoData;

  if (lhciIsoCb.isoRxTest == TRUE)
  {
    if ((pIsoData = LlRecvIsoData()) != NULL)
    {
      lhciFreeRcvdIsoData(pIsoData);
      return;
    }
  }
  else
  {
    if (lhciIsoCb.recvIsoSink && ((pIsoData = LlRecvIsoData()) != NULL))
    {
      lhciSinkIso(pIsoData);
      return;
    }
  }

  ChciTrNeedsService(CHCI_TR_PROT_BLE);
}

/*************************************************************************************************/
/*!
 *  \brief  LL ISO event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciIsoEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {

    default:
      break;
  }

  if (pEvtBuf)
  {
    lhciSendEvt(pEvtBuf);
    return TRUE;
  }

  return FALSE;
}

