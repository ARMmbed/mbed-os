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
 *  \brief HCI Periodic Advertising Sync Transfer (PAST) command module.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "hci_cmd.h"
#include "hci_api.h"
#include "hci_main.h"

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising receive enable command.
 *
 *  \param      syncHandle   Periodic sync handle.
 *  \param      enable       TRUE to enable reports, FALSE to disable reports.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvRcvEnableCmd(uint16_t syncHandle, uint8_t enable)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_PER_ADV_RCV_ENABLE,
                          HCI_LEN_LE_SET_PER_ADV_RCV_ENABLE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, syncHandle);
    UINT8_TO_BSTREAM(p, enable);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE periodic advertising sync transfer command.
 *
 *  \param      connHandle   Connection handle.
 *  \param      serviceData  Service data provided by the host.
 *  \param      syncHandle   Periodic sync handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvSyncTrsfCmd(uint16_t connHandle, uint16_t serviceData, uint16_t syncHandle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_PER_ADV_SYNC_TRANSFER,
                          HCI_LEN_LE_PER_ADV_SYNC_TRANSFER)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, connHandle);
    UINT16_TO_BSTREAM(p, serviceData);
    UINT16_TO_BSTREAM(p, syncHandle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising set info transfer command.
 *
 *  \param      connHandle   Connection handle.
 *  \param      serviceData  Service data provided by the host.
 *  \param      advHandle    Handle to identify an advertising set.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvSetInfoTrsfCmd(uint16_t connHandle, uint16_t serviceData, uint8_t advHandle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_PER_ADV_SET_INFO_TRANSFER,
                          HCI_LEN_LE_PER_ADV_SET_INFO_TRANSFER)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, connHandle);
    UINT16_TO_BSTREAM(p, serviceData);
    UINT8_TO_BSTREAM(p, advHandle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising sync transfer parameters command.
 *
 *  \param      connHandle   Connection handle.
 *  \param      mode         Periodic sync advertising sync transfer mode.
 *  \param      skip         The number of periodic advertising packets that can be skipped after
 *                           a successful receive.
 *  \param      syncTimeout  Synchronization timeout for the periodic advertising.
 *  \param      cteType      Constant tone extension type(Used in AoD/AoA).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvSyncTrsfParamsCmd(uint16_t connHandle, uint8_t mode, uint16_t skip,
                                     uint16_t syncTimeout, uint8_t cteType)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_PAST_PARAM, HCI_LEN_LE_SET_PAST_PARAM)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, connHandle);
    UINT8_TO_BSTREAM(p, mode);
    UINT16_TO_BSTREAM(p, skip);
    UINT16_TO_BSTREAM(p, syncTimeout);
    UINT8_TO_BSTREAM(p, cteType);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set default periodic advertising sync transfer parameters command.
 *
 *  \param      mode         Periodic sync advertising sync transfer mode.
 *  \param      skip         The number of periodic advertising packets that can be skipped after
 *                           a successful receive.
 *  \param      syncTimeout  Synchronization timeout for the periodic advertising.
 *  \param      cteType      Constant tone extension type(Used in AoD/AoA).
 *
 *  \return     None.
*/
/*************************************************************************************************/
void HciLeSetDefaultPerAdvSyncTrsfParamsCmd(uint8_t mode, uint16_t skip, uint16_t syncTimeout,
                                           uint8_t cteType)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_DEFAULT_PAST_PARAM,
                          HCI_LEN_LE_SET_DEFAULT_PAST_PARAM)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, mode);
    UINT16_TO_BSTREAM(p, skip);
    UINT16_TO_BSTREAM(p, syncTimeout);
    UINT8_TO_BSTREAM(p, cteType);
    hciCmdSend(pBuf);
  }
}
