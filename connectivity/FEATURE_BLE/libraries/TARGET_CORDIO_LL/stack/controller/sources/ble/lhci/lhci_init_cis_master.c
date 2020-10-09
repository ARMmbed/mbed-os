/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) Host Controller Interface (HCI) initialization implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL HCI subsystem for CIS master mode.
 *
 *  This function initializes the LL HCI subsystem for CIS master commands.  It is typically called
 *  once upon system initialization.
 */
/*************************************************************************************************/
void LhciCisMasterInit(void)
{
  lhciCmdTbl[LHCI_MSG_CIS_MST] = lhciMstCisDecodeCmdPkt;
  lhciEvtTbl[LHCI_MSG_CIS_MST] = lhciMstCisEncodeEvtPkt;
}
