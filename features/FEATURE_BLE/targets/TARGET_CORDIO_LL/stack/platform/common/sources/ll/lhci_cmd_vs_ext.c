/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Extended vendor specific HCI command module implementation file.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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

#include "lhci_api.h"

/*************************************************************************************************/
/*!
 *  \brief  Decode extended vendor specific HCI command packet.
 *
 *  \param  pHdr    Unpacked command header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing for vendor specific commands.
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciVsExtDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  return FALSE;
}
