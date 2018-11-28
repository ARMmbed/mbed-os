/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI driver interface.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
