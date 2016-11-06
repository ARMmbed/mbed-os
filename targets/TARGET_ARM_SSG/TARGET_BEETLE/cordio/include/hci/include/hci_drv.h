/*************************************************************************************************/
/*!
 *  \file   hci_drv.h
 *
 *  \brief  HCI driver interface.
 *
 *          $Date: 2013-01-02 22:19:17 -0800 (Wed, 02 Jan 2013) $
 *          $Revision: 405 $
 *
 *  Copyright (c) 2012 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef HCI_DRV_H
#define HCI_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     hciDrvWrite
 *
 *  \brief  Write data the driver.
 *
 *  \param  type     HCI packet type
 *  \param  len      Number of bytes to write.
 *  \param  pData    Byte array to write.
 *
 *  \return Return actual number of data bytes written.
 *
 *  \note   The type parameter allows the driver layer to prepend the data with a header on the
 *          same write transaction.
 */
/*************************************************************************************************/
uint16_t hciDrvWrite(uint8_t type, uint16_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \fn     hciDrvRead
 *
 *  \brief  Read data bytes from the driver.
 *
 *  \param  len      Number of bytes to read.
 *  \param  pData    Byte array to store data.
 *
 *  \return Return actual number of data bytes read.
 */
/*************************************************************************************************/
uint16_t hciDrvRead(uint16_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \fn     hciDrvReadyToSleep
 *
 *  \brief  Returns TRUE if driver allows MCU to enter low power sleep mode.
 *
 *  \return TRUE if ready to sleep, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t hciDrvReadyToSleep(void);

#ifdef __cplusplus
};
#endif

#endif /* HCI_DRV_H */
