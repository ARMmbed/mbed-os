/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI driver interface.
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

#ifndef CHCI_DRV_H
#define CHCI_DRV_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the driver.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciDrvInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Write data the driver.
 *
 *  \param  prot      Protocol.
 *  \param  type      Packet type.
 *  \param  len       Number of bytes to write.
 *  \param  pData     Byte array to write.
 *
 *  \return Return actual number of data bytes written.
 *
 *  \note   The type parameter allows the driver layer to prepend the data with a header on the
 *          same write transaction.
 */
/*************************************************************************************************/
uint16_t chciDrvWrite(uint8_t prot, uint8_t type, uint16_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Service the transport device.
 *
 *  \return TRUE if work pending, FALSE if no work is pending.
 */
/*************************************************************************************************/
bool_t chciDrvService(void);

#ifdef __cplusplus
};
#endif

#endif /* CHCI_DRV_H */
