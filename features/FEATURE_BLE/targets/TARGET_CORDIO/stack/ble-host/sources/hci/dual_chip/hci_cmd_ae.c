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
 *  \brief HCI Advertising Extensions (AE) command module.
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
 *  \brief      HCI LE set advertising set random device address command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      pAddr       Random device address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetAdvSetRandAddrCmd(uint8_t advHandle, const uint8_t *pAddr)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_ADV_SET_RAND_ADDR, HCI_LEN_LE_SET_ADV_SET_RAND_ADDR)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advHandle);
    BDA_TO_BSTREAM(p, pAddr);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended advertising parameters command.
 *
 *  \param      advHandle    Advertising handle.
 *  \param      pExtAdvParam Extended advertising parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetExtAdvParamCmd(uint8_t advHandle, hciExtAdvParam_t *pExtAdvParam)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_EXT_ADV_PARAM, HCI_LEN_LE_SET_EXT_ADV_PARAM)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advHandle);
    UINT16_TO_BSTREAM(p, pExtAdvParam->advEventProp);
    UINT24_TO_BSTREAM(p, pExtAdvParam->priAdvInterMin);
    UINT24_TO_BSTREAM(p, pExtAdvParam->priAdvInterMax);
    UINT8_TO_BSTREAM(p, pExtAdvParam->priAdvChanMap);
    UINT8_TO_BSTREAM(p, pExtAdvParam->ownAddrType);
    UINT8_TO_BSTREAM(p, pExtAdvParam->peerAddrType);
    BDA_TO_BSTREAM(p, pExtAdvParam->pPeerAddr);
    UINT8_TO_BSTREAM(p, pExtAdvParam->advFiltPolicy);
    UINT8_TO_BSTREAM(p, pExtAdvParam->advTxPwr);
    UINT8_TO_BSTREAM(p, pExtAdvParam->priAdvPhy);
    UINT8_TO_BSTREAM(p, pExtAdvParam->secAdvMaxSkip);
    UINT8_TO_BSTREAM(p, pExtAdvParam->secAdvPhy);
    UINT8_TO_BSTREAM(p, pExtAdvParam->advSetId);
    UINT8_TO_BSTREAM(p, pExtAdvParam->scanReqNotifEna);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended advertising data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      fragPref    Fragment preference.
 *  \param      len         Data buffer length.
 *  \param      pData       Advertising data buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtAdvDataCmd(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len,
                           const uint8_t *pData)
{
  uint8_t *pBuf;
  uint8_t *p;

  if (len > HCI_EXT_ADV_DATA_LEN)
  {
    len = HCI_EXT_ADV_DATA_LEN;
  }

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_EXT_ADV_DATA, HCI_LEN_LE_SET_EXT_ADV_DATA(len))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advHandle);
    UINT8_TO_BSTREAM(p, op);
    UINT8_TO_BSTREAM(p, fragPref);
    UINT8_TO_BSTREAM(p, len);
    memcpy(p, pData, len);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended scan response data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      fragPref    Fragment preference.
 *  \param      len         Data buffer length.
 *  \param      pData       Scan response data buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtScanRespDataCmd(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len,
                                const uint8_t *pData)
{
  uint8_t *pBuf;
  uint8_t *p;

  if (len > HCI_EXT_ADV_DATA_LEN)
  {
    len = HCI_EXT_ADV_DATA_LEN;
  }

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_EXT_SCAN_RESP_DATA,
                          HCI_LEN_LE_SET_EXT_SCAN_RESP_DATA(len))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advHandle);
    UINT8_TO_BSTREAM(p, op);
    UINT8_TO_BSTREAM(p, fragPref);
    UINT8_TO_BSTREAM(p, len);
    memcpy(p, pData, len);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended advertising enable command.
 *
 *  \param      enable      Set to TRUE to enable advertising, FALSE to disable advertising.
 *  \param      numSets     Number of advertising sets.
 *  \param      pScanParam  Advertising enable parameter array.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtAdvEnableCmd(uint8_t enable, uint8_t numSets, hciExtAdvEnableParam_t *pEnableParam)
{
  uint8_t *pBuf;
  uint8_t *p;
  uint8_t i;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_EXT_ADV_ENABLE, HCI_LEN_LE_EXT_ADV_ENABLE(numSets))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, enable);
    UINT8_TO_BSTREAM(p, numSets);

    for (i = 0; i < numSets; i++)
    {
      UINT8_TO_BSTREAM(p, pEnableParam[i].advHandle);
      UINT16_TO_BSTREAM(p, pEnableParam[i].duration);
      UINT8_TO_BSTREAM(p, pEnableParam[i].maxEaEvents);
    }

    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read maximum advertising data length command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeReadMaxAdvDataLen(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN, HCI_LEN_LE_READ_MAX_ADV_DATA_LEN)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read number of supported advertising sets command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeReadNumSupAdvSets(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS, HCI_LEN_LE_READ_NUM_OF_SUP_ADV_SETS)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE remove advertising set command.
 *
 *  \param      advHandle    Advertising handle.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
void HciLeRemoveAdvSet(uint8_t advHandle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_REMOVE_ADV_SET, HCI_LEN_LE_REMOVE_ADV_SET)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advHandle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE clear advertising sets command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeClearAdvSets(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_CLEAR_ADV_SETS, HCI_LEN_LE_CLEAR_ADV_SETS)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising parameters command.
 *
 *  \param      advHandle       Advertising handle.
 *  \param      advIntervalMin  Periodic advertising interval minimum.
 *  \param      advIntervalMax  Periodic advertising interval maximum.
 *  \param      advProps        Periodic advertising properties.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvParamCmd(uint8_t advHandle, uint16_t advIntervalMin, uint16_t advIntervalMax,
                            uint16_t advProps)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_PER_ADV_PARAM, HCI_LEN_LE_SET_PER_ADV_PARAM)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advHandle);
    UINT16_TO_BSTREAM(p, advIntervalMin);
    UINT16_TO_BSTREAM(p, advIntervalMax);
    UINT16_TO_BSTREAM(p, advProps);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      len         Data buffer length.
 *  \param      pData       Advertising data buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvDataCmd(uint8_t advHandle, uint8_t op, uint8_t len, const uint8_t *pData)
{
  uint8_t *pBuf;
  uint8_t *p;

  if (len > HCI_PER_ADV_DATA_LEN)
  {
    len = HCI_PER_ADV_DATA_LEN;
  }

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_PER_ADV_DATA, HCI_LEN_LE_SET_PER_ADV_DATA(len))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advHandle);
    UINT8_TO_BSTREAM(p, op);
    UINT8_TO_BSTREAM(p, len);
    memcpy(p, pData, len);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising enable command.
 *
 *  \param      enable      Set to TRUE to enable advertising, FALSE to disable advertising.
 *  \param      advHandle   Advertising handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvEnableCmd(uint8_t enable, uint8_t advHandle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_PER_ADV_ENABLE, HCI_LEN_LE_SET_PER_ADV_ENABLE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, enable);
    UINT8_TO_BSTREAM(p, advHandle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended scanning parameters command.
 *
 *  \param      ownAddrType     Address type used by this device.
 *  \param      scanFiltPolicy  Scan filter policy.
 *  \param      scanPhys        Scanning PHYs.
 *  \param      pScanParam      Scanning parameter array.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtScanParamCmd(uint8_t ownAddrType, uint8_t scanFiltPolicy, uint8_t scanPhys,
                             hciExtScanParam_t *pScanParam)
{
  uint8_t *pBuf;
  uint8_t *p;
  uint8_t i;
  uint8_t numPhys;

  /* find out number of scanning PHYs */
  for (i = 0, numPhys = 0; (i < 8) && (numPhys <= HCI_MAX_NUM_PHYS); i++)
  {
    if (scanPhys & (1 << i))
    {
      numPhys++;
    }
  }

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_EXT_SCAN_PARAM, HCI_LEN_LE_SET_EXT_SCAN_PARAM(numPhys))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, ownAddrType);
    UINT8_TO_BSTREAM(p, scanFiltPolicy);
    UINT8_TO_BSTREAM(p, scanPhys);

    for (i = 0; i < numPhys; i++)
    {
      UINT8_TO_BSTREAM(p, pScanParam[i].scanType);
      UINT16_TO_BSTREAM(p, pScanParam[i].scanInterval);
      UINT16_TO_BSTREAM(p, pScanParam[i].scanWindow)
    }

    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE extended scan enable command.
 *
 *  \param      enable          Set to TRUE to enable scanning, FALSE to disable scanning.
 *  \param      filterDup       Set to TRUE to filter duplicates.
 *  \param      duration        Duration.
 *  \param      period          Period.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeExtScanEnableCmd(uint8_t enable, uint8_t filterDup, uint16_t duration, uint16_t period)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_EXT_SCAN_ENABLE, HCI_LEN_LE_SET_EXT_SCAN_ENABLE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, enable);
    UINT8_TO_BSTREAM(p, filterDup);
    UINT16_TO_BSTREAM(p, duration);
    UINT16_TO_BSTREAM(p, period);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE extended create connection command.
 *
 *  \param      pInitParam      Initiating parameters.
 *  \param      pScanParam      Initiating scan parameters.
 *  \param      pConnSpec       Connection specification.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeExtCreateConnCmd(hciExtInitParam_t *pInitParam, hciExtInitScanParam_t *pScanParam,
                           hciConnSpec_t *pConnSpec)
{
  uint8_t *pBuf;
  uint8_t *p;
  uint8_t i;
  uint8_t numPhys;

  /* find out number of initiating PHYs */
  for (i = 0, numPhys = 0; (i < 8) && (numPhys <= HCI_MAX_NUM_PHYS); i++)
  {
    if (pInitParam->initPhys & (1 << i))
    {
      numPhys++;
    }
  }

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_EXT_CREATE_CONN, HCI_LEN_LE_EXT_CREATE_CONN(numPhys))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, pInitParam->filterPolicy);
    UINT8_TO_BSTREAM(p, pInitParam->ownAddrType);
    UINT8_TO_BSTREAM(p, pInitParam->peerAddrType);
    BDA_TO_BSTREAM(p, pInitParam->pPeerAddr);
    UINT8_TO_BSTREAM(p, pInitParam->initPhys);

    for (i = 0; i < numPhys; i++)
    {
      UINT16_TO_BSTREAM(p, pScanParam[i].scanInterval);
      UINT16_TO_BSTREAM(p, pScanParam[i].scanWindow);
      UINT16_TO_BSTREAM(p, pConnSpec[i].connIntervalMin);
      UINT16_TO_BSTREAM(p, pConnSpec[i].connIntervalMax);
      UINT16_TO_BSTREAM(p, pConnSpec[i].connLatency);
      UINT16_TO_BSTREAM(p, pConnSpec[i].supTimeout);
      UINT16_TO_BSTREAM(p, pConnSpec[i].minCeLen);
      UINT16_TO_BSTREAM(p, pConnSpec[i].maxCeLen);
    }

    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE periodic advertising create sync command.
 *
 *  \param      options         Options.
 *  \param      advSid          Advertising SID.
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      skip            Number of periodic advertising packets that can be skipped after
 *                              successful receive.
 *  \param      syncTimeout     Synchronization timeout.
 *  \param      unused          Reserved for future use (must be zero).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvCreateSyncCmd(uint8_t options, uint8_t advSid, uint8_t advAddrType,
                              uint8_t *pAdvAddr, uint16_t skip, uint16_t syncTimeout, uint8_t unused)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_PER_ADV_CREATE_SYNC, HCI_LEN_LE_PER_ADV_CREATE_SYNC)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, options);
    UINT8_TO_BSTREAM(p, advSid);
    UINT8_TO_BSTREAM(p, advAddrType);
    BDA_TO_BSTREAM(p, pAdvAddr);
    UINT16_TO_BSTREAM(p, skip);
    UINT16_TO_BSTREAM(p, syncTimeout);
    UINT8_TO_BSTREAM(p, unused);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE periodic advertising create sync cancel command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvCreateSyncCancelCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_PER_ADV_CREATE_SYNC_CANCEL, HCI_LEN_LE_PER_ADV_CREATE_SYNC_CANCEL)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE periodic advertising terminate sync command.
 *
 *  \param      syncHandle      Sync handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvTerminateSyncCmd(uint16_t syncHandle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_PER_ADV_TERMINATE_SYNC, HCI_LEN_LE_PER_ADV_TERMINATE_SYNC)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, syncHandle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE add device to periodic advertiser list command.
 *
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      advSid          Advertising SID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeAddDeviceToPerAdvListCmd(uint8_t advAddrType, uint8_t *pAdvAddr, uint8_t advSid)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ADD_DEV_PER_ADV_LIST, HCI_LEN_LE_ADD_DEV_PER_ADV_LIST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advAddrType);
    BDA_TO_BSTREAM(p, pAdvAddr);
    UINT8_TO_BSTREAM(p, advSid);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE remove device from periodic advertiser list command.
 *
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      advSid          Advertising SID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeRemoveDeviceFromPerAdvListCmd(uint8_t advAddrType, uint8_t *pAdvAddr, uint8_t advSid)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_REMOVE_DEV_PER_ADV_LIST, HCI_LEN_LE_REMOVE_DEV_PER_ADV_LIST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, advAddrType);
    BDA_TO_BSTREAM(p, pAdvAddr);
    UINT8_TO_BSTREAM(p, advSid);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE clear periodic advertiser list command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeClearPerAdvListCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_CLEAR_PER_ADV_LIST, HCI_LEN_LE_CLEAR_PER_ADV_LIST)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read periodic advertiser size command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeReadPerAdvListSizeCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE, HCI_LEN_LE_READ_PER_ADV_LIST_SIZE)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read transmit power command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeReadTxPower(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_TX_POWER, HCI_LEN_LE_READ_TX_POWER)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read RF path compensation command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeReadRfPathComp(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_RF_PATH_COMP, HCI_LEN_LE_READ_RF_PATH_COMP)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE write RF path compensation command.
 *
 *  \param      txPathComp      RF transmit path compensation value.
 *  \param      rxPathComp      RF receive path compensation value.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeWriteRfPathComp(int16_t txPathComp, int16_t rxPathComp)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_WRITE_RF_PATH_COMP, HCI_LEN_LE_WRITE_RF_PATH_COMP)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, txPathComp);
    UINT16_TO_BSTREAM(p, rxPathComp);
    hciCmdSend(pBuf);
  }
}
