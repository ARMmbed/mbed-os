/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI command module implementation file.
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

#include "lhci_int.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "ll_defs.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack a read resolvable address event.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Status code.
 *  \param  pAddr       Resolvable address.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadResAddrEvt(uint8_t *pBuf, uint8_t status, const uint8_t *pAddr)
{
  const uint8_t len = LHCI_LEN_LE_READ_PEER_RES_ADDR_EVT;

  UINT8_TO_BSTREAM(pBuf, status);
  BDA_TO_BSTREAM  (pBuf, pAddr);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  pParam      Pointer to parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciPrivConnSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t *pParam)
{
  uint8_t *pBuf;
  uint8_t *pEvtBuf;

  if ((pEvtBuf = lhciAllocCmdCmplEvt(paramLen, pCmdHdr->opCode)) == NULL)
  {
    return;
  }
  pBuf = pEvtBuf;

  switch (pCmdHdr->opCode)
  {
    case HCI_OPCODE_LE_READ_PEER_RES_ADDR:
    case HCI_OPCODE_LE_READ_LOCAL_RES_ADDR:
      lhciPackReadResAddrEvt(pBuf, status, pParam);
      break;

    /* --- default --- */

    default:
      break;
  }

  lhciSendCmdCmplEvt(pEvtBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  Decode an HCI command packet.
 *
 *  \param  pHdr    Decoded packet header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciPrivConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint8_t *pParam = NULL;
  bdAddr_t rpa = {0};

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_READ_PEER_RES_ADDR:
    {
      status = LlReadPeerResolvableAddr(pBuf[0], pBuf + 1, rpa);
      paramLen = LHCI_LEN_LE_READ_PEER_RES_ADDR_EVT;
      pParam = (uint8_t *)rpa;
      break;
    }
    case HCI_OPCODE_LE_READ_LOCAL_RES_ADDR:
    {
      status = LlReadLocalResolvableAddr(pBuf[0], pBuf + 1, rpa);
      paramLen = LHCI_LEN_LE_READ_LOCAL_RES_ADDR_EVT;
      pParam = (uint8_t *)rpa;
      break;
    }

    default:
      return lhciPrivAdvDecodeCmdPkt(pHdr, pBuf);
  }

  lhciPrivConnSendCmdCmplEvt(pHdr, status, paramLen, pParam);

  return TRUE;
}
