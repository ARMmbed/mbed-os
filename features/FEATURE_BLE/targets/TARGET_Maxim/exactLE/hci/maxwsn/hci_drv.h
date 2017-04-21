/*************************************************************************************************/
/*!
 *  \file   hci_drv.h
 *
 *  \brief  HCI driver interface.
 *
 *          $Date: 2013-01-02 22:19:17 -0800 (Wed, 02 Jan 2013) $
 *          $Revision: 405 $
 *
 *  Copyright (c) 2012-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
 */
/*************************************************************************************************/
#ifndef HCI_DRV_H
#define HCI_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PinNames.h"

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     hciDrvInit
 *
 *  \brief  Initialize the driver.
 *
 *  \param  csn      name of the pin connected to CSN
 *  \param  irq      name of the pin conntected to IRQ
 */
/*************************************************************************************************/
void hciDrvInit(PinName csn, PinName rst, PinName irq);

/*************************************************************************************************/
/*!
 *  \fn     hciDrvWrite
 *
 *  \brief  Write data to the driver.
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
uint16_t hciDrvRead(uint16_t len, uint8_t *pData, bool_t last);

/*************************************************************************************************/
/*!
 *  \fn     hciDrvIsr
 *
 *  \brief  Interrupt service routine for IRQ
 */
/*************************************************************************************************/
void hciDrvIsr(void);

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

void hciDrvResume(void);

/*************************************************************************************************/
/*!
 *  \fn     hciDrvReset
 *
 *  \brief  Resets the controller
 */
/*************************************************************************************************/
void hciDrvReset(void);

#ifdef __cplusplus
};
#endif

#endif /* HCI_DRV_H */
