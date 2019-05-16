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
 *  \brief HCI PHY command module.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "hci_cmd.h"
#include "hci_api.h"
#include "hci_main.h"

/*************************************************************************************************/
/*!
*  \brief  HCI read PHY command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeReadPhyCmd(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_PHY, HCI_LEN_LE_READ_PHY)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI set default PHY command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeSetDefaultPhyCmd(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_DEF_PHY, HCI_LEN_LE_SET_DEF_PHY)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, allPhys);
    UINT8_TO_BSTREAM(p, txPhys);
    UINT8_TO_BSTREAM(p, rxPhys);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI set PHY command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeSetPhyCmd(uint16_t handle, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_PHY, HCI_LEN_LE_SET_PHY)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT8_TO_BSTREAM(p, allPhys);
    UINT8_TO_BSTREAM(p, txPhys);
    UINT8_TO_BSTREAM(p, rxPhys);
    UINT16_TO_BSTREAM(p, phyOptions);
    hciCmdSend(pBuf);
  }
}
