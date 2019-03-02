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
 *  \brief HCI command module implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "hci_defs.h"
#include "bb_ble_drv.h"
#include "ll_api.h"
#include "ll_defs.h"
#include "ll_math.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Unpack a set event mask command packet (either general and LE event masks).
 *
 *  \param  pEvtMsk     Event mask.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Payload length.
 */
/*************************************************************************************************/
static uint8_t lhciUnpackSetEventMaskCmd(uint64_t *pEvtMsk, const uint8_t *pBuf)
{
  const uint8_t len = sizeof(uint64_t);

  BSTREAM_TO_UINT64(*pEvtMsk, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a read buffer size event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  pktLen      Data packet length.
 *  \param  numPkts     Number of controller data packet buffers.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadBufSizeEvt(uint8_t *pBuf, uint8_t status, uint16_t pktLen, uint16_t numPkts)
{
  const uint8_t len = LHCI_LEN_LE_READ_BUF_SIZE_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, pktLen);
  UINT8_TO_BSTREAM (pBuf, numPkts);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a local version info event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackLocalVersionInfo(uint8_t *pBuf, uint8_t status)
{
  const uint8_t len = LHCI_LEN_READ_LOCAL_VER_EVT;

  uint16_t compId;
  uint8_t btVer;
  uint16_t implRev;

  LlGetVersion(&compId, &btVer, &implRev);

  UINT8_TO_BSTREAM (pBuf, status);
  UINT8_TO_BSTREAM (pBuf, btVer);           /* HCI_Version */
  UINT16_TO_BSTREAM(pBuf, implRev);         /* HCI_Revision */
  UINT8_TO_BSTREAM (pBuf, btVer);           /* LMP_Version */
  UINT16_TO_BSTREAM(pBuf, compId);          /* Manufacturer_Name */
  UINT16_TO_BSTREAM(pBuf, implRev);         /* LMP_Subversion */

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a local supported commands event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackLocalSupCmds(uint8_t *pBuf, uint8_t status)
{
  const uint8_t len = LHCI_LEN_READ_LOCAL_SUP_CMDS_EVT;

  memset(pBuf, 0, len);

  UINT8_TO_BSTREAM (pBuf, status);

  pBuf[5]  = HCI_SUP_SET_EVENT_MASK |                           /* mandatory */
             HCI_SUP_RESET;                                     /* mandatory */
  pBuf[14] = HCI_SUP_READ_LOCAL_VER_INFO |                      /* mandatory */
             HCI_SUP_READ_LOCAL_SUP_FEAT;                       /* mandatory */
  pBuf[15] = HCI_SUP_READ_BD_ADDR;                              /* mandatory */
  pBuf[22] = HCI_SUP_SET_EVENT_MASK_PAGE2;                      /* optional or LE ping */
  pBuf[25] = HCI_SUP_LE_SET_EVENT_MASK |                        /* mandatory */
             HCI_SUP_LE_READ_BUF_SIZE |                         /* mandatory */
             HCI_SUP_LE_READ_LOCAL_SUP_FEAT |                   /* mandatory */
             HCI_SUP_LE_SET_RAND_ADDR;                          /* Tx device */
  pBuf[26] = HCI_SUP_LE_READ_WHITE_LIST_SIZE |                  /* mandatory */
             HCI_SUP_LE_CLEAR_WHITE_LIST;                       /* mandatory */
  pBuf[27] = HCI_SUP_LE_ADD_DEV_WHITE_LIST |                    /* mandatory */
             HCI_SUP_LE_REMOVE_DEV_WHITE_LIST |                 /* mandatory */
             HCI_SUP_LE_RAND;                                   /* LL encryption or optional */
  pBuf[28] = HCI_SUP_LE_READ_SUP_STATES |                       /* mandatory (4.1+) */
             HCI_SUP_LE_RECEIVER_TEST |                         /* Rx device */
             HCI_SUP_LE_TRANSMITTER_TEST |                      /* Tx device */
             HCI_SUP_LE_TEST_END;                               /* mandatory */
  pBuf[38] = HCI_SUP_LE_READ_TX_POWER;                          /* mandatory (5.0) */

  if (lhciCmdTbl[LHCI_MSG_CONN])
  {
    pBuf[0]  |= HCI_SUP_DISCONNECT;                             /* Master or slave */
    pBuf[2]  |= HCI_SUP_READ_REMOTE_VER_INFO;                   /* Master or slave */
    pBuf[10] |= HCI_SUP_READ_TX_PWR_LVL;                        /* Master or slave */
    pBuf[15] |= HCI_SUP_READ_RSSI;                              /* Master or slave */

    pBuf[27] |= HCI_SUP_LE_CONN_UPDATE |                        /* Master role or initiating conn param req */
                HCI_SUP_LE_READ_CHAN_MAP |                      /* Master or slave */
                HCI_SUP_LE_READ_REMOTE_FEAT;                    /* Master or optional */

    pBuf[33] |= HCI_SUP_LE_SET_DATA_LEN |                       /* Data length extensions */
                HCI_SUP_LE_READ_DEF_DATA_LEN |                  /* Data length extensions */
                HCI_SUP_LE_REM_CONN_PARAM_REQ_REPL |            /* Accepting conn param req */
                HCI_SUP_LE_REM_CONN_PARAM_REQ_NEG_REPL;         /* Accepting conn param req */
    pBuf[34] |= HCI_SUP_LE_WRITE_DEF_DATA_LEN;                  /* Data length extensions */
    pBuf[35] |= HCI_SUP_LE_READ_MAX_DATA_LEN;                   /* Data length extensions */
  }

  if (lhciCmdTbl[LHCI_MSG_SCAN]) /* i.e. master */
  {
    pBuf[26] |= HCI_SUP_LE_SET_SCAN_PARAM |                     /* Rx device */
                HCI_SUP_LE_SET_SCAN_ENABLE;                     /* Rx device */

    if (lhciCmdTbl[LHCI_MSG_CONN])
    {
      pBuf[26] |= HCI_SUP_LE_CREATE_CONN |                      /* Master role */
                  HCI_SUP_LE_CREATE_CONN_CANCEL;                /* Master role */
      pBuf[27] |= HCI_SUP_LE_SET_HOST_CHAN_CLASS;               /* Master role */
    }
  }

  if (lhciCmdTbl[LHCI_MSG_ADV]) /* i.e. slave */
  {
    pBuf[25] |= HCI_SUP_LE_SET_ADV_PARAM |                      /* Tx device */
                HCI_SUP_LE_READ_ADV_TX_POWER |                  /* Tx device */
                HCI_SUP_LE_SET_ADV_DATA;                        /* Tx device */
    pBuf[26] |= HCI_SUP_LE_SET_SCAN_RESP_DATA |                 /* Tx + Rx device */
                HCI_SUP_LE_SET_ADV_ENABLE;                      /* Tx device */
  }

  if (lhciCmdTbl[LHCI_MSG_ENC] &&
      lhciCmdTbl[LHCI_MSG_CONN])
  {
    pBuf[27] |= HCI_SUP_LE_ENCRYPT;                             /* LL encryption */
    pBuf[28] |= HCI_SUP_LE_LTK_REQ_REPL |                       /* LL encryption + slave */
                HCI_SUP_LE_LTK_REQ_NEG_REPL |                   /* LL encryption + slave */
                HCI_SUP_LE_START_ENCRYPTION;                    /* LL encryption + master */
    pBuf[32] |= HCI_SUP_READ_AUTH_PAYLOAD_TO |                  /* LE ping */
                HCI_SUP_WRITE_AUTH_PAYLOAD_TO;                  /* LE ping */
  }

  if (lhciCmdTbl[LHCI_MSG_PRIV])
  {
    pBuf[34] |= HCI_SUP_LE_ADD_DEV_RES_LIST_EVT |               /* LE privacy */
                HCI_SUP_LE_REMOVE_DEV_RES_LIST |                /* LE privacy */
                HCI_SUP_LE_CLEAR_RES_LIST |                     /* LE privacy */
                HCI_SUP_LE_READ_RES_LIST_SIZE;                  /* LE privacy */
    pBuf[35] |= HCI_SUP_LE_SET_ADDR_RES_ENABLE |                /* LE privacy */
                HCI_SUP_LE_SET_RES_PRIV_ADDR_TO;                /* LE privacy */
    pBuf[39] |= HCI_SUP_LE_SET_PRIVACY_MODE;                    /* LE privacy */

    if (lhciCmdTbl[LHCI_MSG_CONN])
    {
      pBuf[34] |= HCI_SUP_LE_READ_PEER_RES_ADDR;                /* LE privacy + master or slave */
      pBuf[35] |= HCI_SUP_LE_READ_LOCAL_RES_ADDR;               /* LE privacy + master or slave */
    }
  }

  if (lhciCmdTbl[LHCI_MSG_SC])
  {
    pBuf[34] |= HCI_SUP_LE_READ_LOCAL_P256_PUB_KEY |            /* Secure connections */
                HCI_SUP_LE_GENERATE_DHKEY;                      /* Secure connections */
  }

  if (lhciCmdTbl[LHCI_MSG_PHY])
  {
    pBuf[35] |= HCI_SUP_LE_READ_PHY |                           /* LE PHY features */
                HCI_SUP_LE_SET_DEF_PHY |                        /* LE PHY features */
                HCI_SUP_LE_SET_PHY |                            /* LE PHY features */
                HCI_SUP_LE_ENHANCED_RECEIVER_TEST;              /* LE PHY features */
    pBuf[36] |= HCI_SUP_LE_ENHANCED_TRANSMITTER_TEST;           /* LE PHY features */
  }

  if (lhciCmdTbl[LHCI_MSG_EXT_ADV])
  {
    pBuf[36] |= HCI_SUP_LE_SET_ADV_SET_RAND_ADDR |              /* Extended advertising */
                HCI_SUP_LE_SET_EXT_ADV_PARAM |                  /* Extended advertising */
                HCI_SUP_LE_SET_EXT_ADV_DATA |                   /* Extended advertising */
                HCI_SUP_LE_SET_EXT_SCAN_RESP_DATA |             /* Extended advertising */
                HCI_SUP_LE_SET_EXT_ADV_ENABLE |                 /* Extended advertising */
                HCI_SUP_LE_READ_MAX_ADV_DATA_LEN |              /* Extended advertising */
                HCI_SUP_LE_READ_NUM_OF_SUP_ADV_SETS;            /* Extended advertising */
    pBuf[37] |= HCI_SUP_LE_REMOVE_ADV_SET |                     /* Extended advertising */
                HCI_SUP_LE_CLEAR_ADV_SETS;                      /* Extended advertising */
    pBuf[39] |= HCI_SUP_LE_READ_RF_PATH_COMP |                  /* Extended advertising */
                HCI_SUP_LE_WRITE_RF_PATH_COMP;                  /* Extended advertising */
  }

  if (lhciCmdTbl[LHCI_MSG_EXT_SCAN])
  {
    pBuf[37] |= HCI_SUP_LE_SET_EXT_SCAN_PARAM |                 /* Extended scanning */
                HCI_SUP_LE_SET_EXT_SCAN_ENABLE;                 /* Extended scanning */

    if (lhciCmdTbl[LHCI_MSG_CONN])
    {
      pBuf[37] |= HCI_SUP_LE_EXT_CREATE_CONN;                   /* Extended initiate. */
    }
  }

  if (lhciCmdTbl[LHCI_MSG_PER_ADV])
  {
    pBuf[37] |= HCI_SUP_LE_SET_PER_ADV_PARAM |                  /* Periodic advertising */
                HCI_SUP_LE_SET_PER_ADV_DATA  |                  /* Periodic advertising */
                HCI_SUP_LE_SET_PER_ADV_ENABLE;
  }

  if (lhciCmdTbl[LHCI_MSG_PER_SCAN])
  {
    pBuf[38] |= HCI_SUP_LE_PER_ADV_CREATE_SYNC |                /* Periodic scanning */
                HCI_SUP_LE_PER_ADV_CREATE_SYNC_CANCEL  |        /* Periodic scanning */
                HCI_SUP_LE_PER_ADV_TERMINATE_SYNC  |            /* Periodic scanning */
                HCI_SUP_LE_ADD_DEV_PER_ADV_LIST  |              /* Periodic scanning */
                HCI_SUP_LE_REMOVE_DEV_PER_ADV_LIST  |           /* Periodic scanning */
                HCI_SUP_LE_CLEAR_PER_ADV_LIST  |                /* Periodic scanning */
                HCI_SUP_LE_READ_PER_ADV_LIST_SIZE;              /* Periodic scanning */
  }

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a local supported features event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackLocalSupFeat(uint8_t *pBuf, uint8_t status)
{
  const uint8_t len = LHCI_LEN_LE_READ_LOCAL_SUP_FEAT_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT64_TO_BSTREAM(pBuf, LHCI_LOCAL_SUP_FEAT_VAL);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send command status event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lhciSendCmdStatusEvt(LhciHdr_t *pCmdHdr, uint8_t status)
{
  uint8_t *pEvtBuf;

  if ((pEvtBuf = lhciAllocEvt(HCI_CMD_STATUS_EVT, HCI_LEN_CMD_STATUS)) == NULL)
  {
    return;
  }

  lhciPackCmdStatusEvt(pEvtBuf, status, pCmdHdr->opCode);
  lhciSendEvt(pEvtBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciCommonSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen)
{
  uint8_t *pBuf;
  uint8_t *pEvtBuf;

  if ((pEvtBuf = lhciAllocCmdCmplEvt(paramLen, pCmdHdr->opCode)) == NULL)
  {
    return;
  }
  pBuf = pEvtBuf;

  switch (pCmdHdr->opCode)
  {
    /* --- command completion with status only parameter --- */

    case HCI_OPCODE_SET_EVENT_MASK:
    case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
    case HCI_OPCODE_LE_SET_EVENT_MASK:
    case HCI_OPCODE_LE_SET_RAND_ADDR:
    case HCI_OPCODE_LE_CLEAR_WHITE_LIST:
    case HCI_OPCODE_LE_ADD_DEV_WHITE_LIST:
    case HCI_OPCODE_LE_REMOVE_DEV_WHITE_LIST:
    case HCI_OPCODE_LE_WRITE_RF_PATH_COMP:
    case HCI_OPCODE_NOP:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;

    /* --- status --- */

    case HCI_OPCODE_LE_RAND:
    {
      const uint8_t size = sizeof(uint64_t) / sizeof(uint8_t);

      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      BbBleDrvRand(pBuf, size);
      /* pBuf += size; */
      break;
    }
    /* --- device management --- */

    case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      *pBuf = LlGetWhitelistSize();
      break;
    case HCI_OPCODE_READ_LOCAL_VER_INFO:
      lhciPackLocalVersionInfo(pBuf, status);
      break;
    case HCI_OPCODE_READ_LOCAL_SUP_CMDS:
      lhciPackLocalSupCmds(pBuf, status);
      break;
    case HCI_OPCODE_READ_LOCAL_SUP_FEAT:
      lhciPackLocalSupFeat(pBuf, status);
      break;
    case HCI_OPCODE_READ_BD_ADDR:
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      LlGetBdAddr(pBuf);
      break;
    case HCI_OPCODE_LE_READ_BUF_SIZE:
      lhciPackReadBufSizeEvt(pBuf, status, LlGetAclMaxSize(), LlGetAclTxBufs());
      break;
    case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      LlGetFeatures(pBuf);
      break;
    case HCI_OPCODE_LE_READ_SUP_STATES:
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      LlGetSupStates(pBuf);
      break;
    case HCI_OPCODE_LE_READ_TX_POWER:
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      LlReadSupTxPower((int8_t *)&pBuf[0], (int8_t *)&pBuf[1]);
      break;
    case HCI_OPCODE_LE_READ_RF_PATH_COMP:
    {
      int16_t txPathComp;
      int16_t rxPathComp;
      LlReadRfPathComp(&txPathComp, &rxPathComp);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT16_TO_BSTREAM(pBuf, txPathComp);
      UINT16_TO_BSTREAM(pBuf, rxPathComp);
      break;
    }

    /* --- test --- */

    case HCI_OPCODE_LE_RECEIVER_TEST:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;
    case HCI_OPCODE_LE_TRANSMITTER_TEST:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;
    case HCI_OPCODE_LE_TEST_END:
    {
      LlTestReport_t rpt = {0};
      status = LlEndTest(&rpt);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT16_TO_BSTREAM(pBuf, rpt.numRxSuccess);
      break;
    }

    /* --- default --- */

    default:
      break;
  }

  lhciSendCmdCmplEvt(pEvtBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  Decode an HCI command packet.
 *
 *  \param  pHdr    Decoded packet header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing is organized by urgency then by frequency of command calls during normal
 *  operation. The following grouping is used:
 *      - status
 *      - local device management
 *      - test
 *      - default/unused
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciCommonDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciCommonVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- status --- */

    case HCI_OPCODE_LE_RAND:
      paramLen = LHCI_LEN_LE_RAND_EVT;
      break;

    /* --- local device management --- */

    case HCI_OPCODE_LE_SET_RAND_ADDR:
      status = LlSetRandAddr(pBuf);
      paramLen = LHCI_LEN_LE_SET_RAND_ADDR_EVT;
      break;
    case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
      paramLen = LHCI_LEN_LE_READ_WL_SIZE_EVT;
      break;
    case HCI_OPCODE_LE_CLEAR_WHITE_LIST:
      status = LlClearWhitelist();
      paramLen = LHCI_LEN_LE_CLEAR_WHITE_LIST_EVT;
      break;
    case HCI_OPCODE_LE_ADD_DEV_WHITE_LIST:
      status = LlAddDeviceToWhitelist(*pBuf, pBuf + 1);
      paramLen = LHCI_LEN_LE_ADD_DEV_WHITE_LIST_EVT;
      break;
    case HCI_OPCODE_LE_REMOVE_DEV_WHITE_LIST:
      status = LlRemoveDeviceFromWhitelist(*pBuf, pBuf + 1);
      paramLen = LHCI_LEN_LE_REMOVE_DEV_WHITE_LIST_EVT;
      break;
    case HCI_OPCODE_SET_EVENT_MASK:
      lhciUnpackSetEventMaskCmd(&lhciCb.evtMsk, pBuf);
      paramLen = LHCI_LEN_SET_EVENT_MASK_EVT;
      break;
    case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
      lhciUnpackSetEventMaskCmd(&lhciCb.evtMskPg2, pBuf);
      paramLen = LHCI_LEN_SET_EVENT_MASK_EVT;
      break;
    case HCI_OPCODE_READ_LOCAL_VER_INFO:
      paramLen = LHCI_LEN_READ_LOCAL_VER_EVT;
      break;
    case HCI_OPCODE_READ_LOCAL_SUP_CMDS:
      paramLen = LHCI_LEN_READ_LOCAL_SUP_CMDS_EVT;
      break;
    case HCI_OPCODE_READ_LOCAL_SUP_FEAT:
      paramLen = LHCI_LEN_READ_LOCAL_SUP_FEAT_EVT;
      break;
    case HCI_OPCODE_READ_BD_ADDR:
      paramLen = LHCI_LEN_READ_BD_ADDR_EVT;
      break;
    case HCI_OPCODE_LE_SET_EVENT_MASK:
      lhciUnpackSetEventMaskCmd(&lhciCb.leEvtMsk, pBuf);
      paramLen = LHCI_LEN_LE_SET_EVENT_MASK_EVT;
      break;
    case HCI_OPCODE_LE_READ_BUF_SIZE:
      paramLen = LHCI_LEN_LE_READ_BUF_SIZE_EVT;
      break;
    case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
      paramLen = LHCI_LEN_LE_READ_LOCAL_SUP_FEAT_EVT;
      break;
    case HCI_OPCODE_LE_READ_SUP_STATES:
      paramLen = LHCI_LEN_LE_READ_SUP_STATES_EVT;
      break;
    case HCI_OPCODE_LE_READ_TX_POWER:
      paramLen = LHCI_LEN_LE_READ_SUP_TX_POWER;
      break;
    case HCI_OPCODE_LE_WRITE_RF_PATH_COMP:
    {
      int16_t txPathComp;
      int16_t rxPathComp;
      BSTREAM_TO_UINT16(txPathComp, pBuf);
      BSTREAM_TO_UINT16(rxPathComp, pBuf);
      status = LlWriteRfPathComp(txPathComp, rxPathComp);
      paramLen = LHCI_LEN_LE_WRITE_RF_PATH_COMP;
      break;
    }
    case HCI_OPCODE_LE_READ_RF_PATH_COMP:
    {
      paramLen = LHCI_LEN_LE_READ_RF_PATH_COMP;
      break;
    }
    case HCI_OPCODE_RESET:
      LlReset();

      /* No events during reset. */
      lhciCb.evtMsk = 0;
      lhciCb.evtMskPg2 = 0;
      lhciCb.leEvtMsk = 0;

      return TRUE;       /* LL event handler sends command completion */
    case HCI_OPCODE_NOP:
      paramLen = 1;
      break;

    /* --- test --- */

    case HCI_OPCODE_LE_RECEIVER_TEST:
      status = LlRxTest(pBuf[0], 0);
      paramLen = LHCI_LEN_LE_RECEIVER_TEST_EVT;
      break;
    case HCI_OPCODE_LE_TRANSMITTER_TEST:
      status = LlTxTest(pBuf[0], pBuf[1], pBuf[2], 0);
      paramLen = LHCI_LEN_LE_TRANSMITTER_TEST_EVT;
      break;
    case HCI_OPCODE_LE_TEST_END:
      paramLen = LHCI_LEN_LE_TEST_END_EVT;
      break;

    /* --- default --- */

    default:
      return FALSE;       /* exit dispatcher routine */
  }

  lhciCommonSendCmdCmplEvt(pHdr, status, paramLen);

  return TRUE;
}
