/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI serial transport interface file.
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

#ifndef CHCI_TR_SERIAL_H
#define CHCI_TR_SERIAL_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************/
/*!
 *  \brief  Receive function.  Gets called by external code when bytes are received.
 *
 *  \param  pBuf   Pointer to buffer of incoming bytes.
 *  \param  len    Number of bytes in incoming buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrSerialRxIncoming(uint8_t *pBuf, uint8_t len);

#ifdef __cplusplus
};
#endif

#endif /* CHCI_TR_SERIAL_H */
