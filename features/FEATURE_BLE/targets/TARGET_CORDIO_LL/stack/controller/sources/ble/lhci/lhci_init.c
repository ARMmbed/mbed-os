/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Link layer (LL) Host Controller Interface (HCI) initialization implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "chci_api.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL HCI handler.
 *
 *  \param      handlerId  WSF handler ID.
 *
 *  \return     None.
 *
 *  This function initializes the LL HCI subsystem.  It is typically called once upon system
 *  initialization.
 */
/*************************************************************************************************/
void LhciHandlerInit(wsfHandlerId_t handlerId)
{
  ChciTrSetCbacks(CHCI_TR_PROT_BLE, lhciRecv, lhciSendComplete, lhciService);
  ChciTrSetSendHwErrorCback(lhciSendHwError);

  memset(&lhciPersistCb, 0, sizeof(lhciPersistCb));
  lhciPersistCb.handlerId = handlerId;
  memset(&lhciCb, 0, sizeof(lhciCb));

  LlEvtRegister(lhciLlEvtHandler);

  lhciReset();
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize VS extended command decoder.
 *
 *  \param      decodeCmd  Command decoder.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LhciVsExtInit(lhciCmdHandler_t decodeCmd)
{
  lhciCmdTbl[LHCI_MSG_VS_EXT] = decodeCmd;
}
