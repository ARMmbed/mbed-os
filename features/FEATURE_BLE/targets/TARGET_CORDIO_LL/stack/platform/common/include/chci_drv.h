/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Controller HCI driver interface.
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
