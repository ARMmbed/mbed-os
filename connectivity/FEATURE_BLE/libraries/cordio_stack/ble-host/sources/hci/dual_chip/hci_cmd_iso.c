/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI Isochronous (ISO) data path command module.
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

#include <string.h>
#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "hci_cmd.h"
#include "hci_api.h"
#include "hci_main.h"

/*************************************************************************************************/
/*!
 *  \brief      HCI LE enable ISO Tx test.
 *
 *  \param      handle      CIS or BIS handle.
 *  \param      pldType     Payload type.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeIsoTxTest(uint16_t handle, uint8_t pldType)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ISO_TX_TEST, HCI_LEN_LE_ISO_TX_TEST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT8_TO_BSTREAM(p, pldType);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE enable ISO Rx test.
 *
 *  \param      handle      CIS or BIS handle.
 *  \param      pldType     Payload type.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeIsoRxTest(uint16_t handle, uint8_t pldType)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ISO_RX_TEST, HCI_LEN_LE_ISO_RX_TEST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT8_TO_BSTREAM(p, pldType);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read ISO test counter.
 *
 *  \param      handle      CIS or BIS handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeIsoReadTestCounters(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ISO_READ_TEST_COUNTERS, HCI_LEN_LE_ISO_READ_TEST_COUNTERS)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE ISO test end.
 *
 *  \param      handle      CIS or BIS handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeIsoTestEnd(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ISO_TEST_END, HCI_LEN_LE_ISO_TEST_END)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE setup ISO data path command.
 *
 *  \param      pDataPathParam  Parameters for setup ISO data path.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetupIsoDataPathCmd(HciIsoSetupDataPath_t *pDataPathParam)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SETUP_ISO_DATA_PATH, 
                          HCI_LEN_LE_SETUP_ISO_DATA_PATH(pDataPathParam->codecConfigLen))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, pDataPathParam->handle);
    UINT8_TO_BSTREAM(p, pDataPathParam->dpDir);
    UINT8_TO_BSTREAM(p, pDataPathParam->dpId);
    UINT8_TO_BSTREAM(p, pDataPathParam->codingFmt);
    UINT16_TO_BSTREAM(p, pDataPathParam->compId);
    UINT16_TO_BSTREAM(p, pDataPathParam->vsCodecId);
    UINT24_TO_BSTREAM(p, pDataPathParam->ctrDly);
    UINT8_TO_BSTREAM(p, pDataPathParam->codecConfigLen);

    memcpy(p, pDataPathParam->pCodecConfig, pDataPathParam->codecConfigLen);

    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE remove ISO data path command.
 *
 *  \param      handle          Connection handle of the CIS or BIS.
 *  \param      directionBits   Data path direction bits.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeRemoveIsoDataPathCmd(uint16_t handle, uint8_t directionBits)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_REMOVE_ISO_DATA_PATH, 
                          HCI_LEN_LE_REMOVE_ISO_DATA_PATH)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT8_TO_BSTREAM(p, directionBits);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI configure data path command.
 *
 *  \param      pDataPathParam  Parameters for configuring data path.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciConfigDataPathCmd(HciConfigDataPath_t *pDataPathParam)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_CONFIG_DATA_PATH,
                          HCI_LEN_CONFIG_DATA_PATH(pDataPathParam->configLen))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, pDataPathParam->dpDir);
    UINT8_TO_BSTREAM(p, pDataPathParam->dpId);
    UINT8_TO_BSTREAM(p, pDataPathParam->configLen);

    memcpy(p, pDataPathParam->pConfig, pDataPathParam->configLen);

    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI read local supported codecs command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciReadLocalSupCodecsCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_LOCAL_SUP_CODECS,
                          HCI_LEN_READ_LOCAL_SUP_CODECS)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI read local supported codec capabilities command.
 *
 *  \param      pCodecParam     Parameters to read codec capablilties.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciReadLocalSupCodecCapCmd(HciReadLocalSupCodecCaps_t *pCodecParam)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_LOCAL_SUP_CODEC_CAP,
                          HCI_LEN_READ_LOCAL_SUP_CODEC_CAP)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, pCodecParam->codingFmt);
    UINT16_TO_BSTREAM(p, pCodecParam->compId);
    UINT16_TO_BSTREAM(p, pCodecParam->vsCodecId);
    UINT8_TO_BSTREAM(p, pCodecParam->transType);
    UINT8_TO_BSTREAM(p, pCodecParam->direction);

    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI read local supported controller delay command.
 *
 *  \param      pDelayParam     Parameters to read controller delay.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciReadLocalSupControllerDlyCmd(HciReadLocalSupControllerDly_t *pDelayParam)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_LOCAL_SUP_CONTROLLER_DLY,
                          HCI_LEN_READ_LOCAL_SUP_CONTROLLER_DLY(pDelayParam->codecConfigLen))) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, pDelayParam->codingFmt);
    UINT16_TO_BSTREAM(p, pDelayParam->compId);
    UINT16_TO_BSTREAM(p, pDelayParam->vsCodecId);
    UINT8_TO_BSTREAM(p, pDelayParam->transType);
    UINT8_TO_BSTREAM(p, pDelayParam->direction);
    UINT8_TO_BSTREAM(p, pDelayParam->codecConfigLen);

    memcpy(p, pDelayParam->pCodecConfig, pDelayParam->codecConfigLen);

    hciCmdSend(pBuf);
  }
}
