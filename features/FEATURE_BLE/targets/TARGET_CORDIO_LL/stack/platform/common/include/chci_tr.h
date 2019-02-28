/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI transport interface file.
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

#ifndef CHCI_TR_H
#define CHCI_TR_H

#include "wsf_types.h"
#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Signal the completion of a message write.
 *
 *  \return None.
 *
 *  This routine is used for asynchronous write operations. When the driver has completed the
 *  use of the write buffer, this routine is called to free the buffer and release flow control.
 */
/*************************************************************************************************/
void chciTrSendComplete(void);

/*************************************************************************************************/
/*!
 *  \brief  Signal the completion of a message receive.
 *
 *  \param  prot    Protocol.
 *  \param  type    Message type.
 *  \param  pBuf    Message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrRecv(uint8_t prot, uint8_t type, uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief  Signal a hardware error.
 *
 *  \param  code    Error code.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrHwError(uint8_t code);

#ifdef __cplusplus
};
#endif

#endif /* CHCI_TR_H */
