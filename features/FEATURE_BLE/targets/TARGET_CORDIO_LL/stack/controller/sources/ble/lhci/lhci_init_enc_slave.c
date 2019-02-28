/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) Host Controller Interface (HCI) initialization implementation file.
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

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL HCI subsystem for slave encryption mode.
 *
 *  \return     None.
 *
 *  This function initializes the LL HCI subsystem for slave encryption commands.  It is typically
 *  called once upon system initialization.
 */
/*************************************************************************************************/
void LhciEncSlaveInit(void)
{
  /* Don't override master's handler if set. */
  if (!lhciCmdTbl[LHCI_MSG_ENC])
  {
    lhciCmdTbl[LHCI_MSG_ENC] = lhciSlvEncDecodeCmdPkt;
    lhciEvtTbl[LHCI_MSG_ENC] = lhciSlvEncEncodeEvtPkt;
  }
}
