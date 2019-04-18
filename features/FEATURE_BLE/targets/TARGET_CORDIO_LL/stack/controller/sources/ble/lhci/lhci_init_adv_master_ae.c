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
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL HCI subsystem for master mode.
 *
 *  \return     None.
 *
 *  This function initializes the LL HCI subsystem for master commands.  It is typically called
 *  once upon system initialization.
 */
/*************************************************************************************************/
void LhciExtScanMasterInit(void)
{
  lhciCmdTbl[LHCI_MSG_EXT_SCAN] = lhciMstExtScanDecodeCmdPkt;
  lhciEvtTbl[LHCI_MSG_EXT_SCAN] = lhciMstExtScanEncodeEvtPkt;

  lhciCmdTbl[LHCI_MSG_PER_SCAN] = lhciMstExtScanDecodeCmdPkt;
  lhciEvtTbl[LHCI_MSG_PER_SCAN] = lhciMstExtScanEncodeEvtPkt;
}
