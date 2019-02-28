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
 *  \brief  Decode an HCI command packet.
 *
 *  \param  pHdr    Decoded packet header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing is organized by urgency then by frequency of command calls during normal
 *  operation. The following grouping is used:
 *      - data & flow control
 *      - status
 *      - advertising control
 *      - remote device management
 *      - local device management
 *      - test
 *      - default/unused
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstEncDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- Encryption --- */

    case HCI_OPCODE_LE_START_ENCRYPTION:
    {
      uint16_t handle;
      uint8_t *pRand = pBuf;
      uint16_t diversifier;
      uint8_t *pKey;

      BSTREAM_TO_UINT16(handle, pBuf);
      pRand = pBuf;
      pBuf += HCI_RAND_LEN;
      BSTREAM_TO_UINT16(diversifier, pBuf);
      pKey  = pBuf;

      status = LlStartEncryption(handle, pRand, diversifier, pKey);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    /* --- default --- */

    default:
      /* Fall back to slave encryption handler. */
      return lhciSlvEncDecodeCmdPkt(pHdr, pBuf);    /* exit dispatcher routine */
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }

  return TRUE;
}
