/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI command module implementation file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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
#include "hci_defs.h"
#include "ll_api.h"
#include "ll_defs.h"
#include "pal_codec.h"
#include "wsf_msg.h"
#include "wsf_math.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define LHCI_MAX_CODEC  5       /*!< Maximum number of codecs to read from audio subsystem. */

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief  ISO return parameters. */
typedef union
{
  struct
  {
    uint32_t minDly;        /*!< Minimum controller delay. */
    uint32_t maxDly;        /*!< Maximum controller delay. */
  } ctrDly;                 /*!< Read local supported controller delay return parameters. */
} lhciIsoReturnParam_t;

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Pack a read ISO buffer size event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  aclPktLen  ACL data packet length.
 *  \param  aclNumPkts  Number of controller ACL data packet buffers.
 *  \param  isoBufSize  ISO data packet length.
 *  \param  numIsoPkts  Number of controller ISO data packet buffers.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadBufSizeV2Evt(uint8_t *pBuf, uint8_t status, uint16_t aclPktLen, uint8_t aclNumPkts,
                                        uint16_t isoBufSize, uint8_t numIsoPkts)
{
  const uint8_t len = LHCI_LEN_LE_READ_BUF_SIZE_V2;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, aclPktLen);
  UINT8_TO_BSTREAM (pBuf, aclNumPkts);
  UINT16_TO_BSTREAM(pBuf, isoBufSize);
  UINT8_TO_BSTREAM (pBuf, numIsoPkts);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a read ISO buffer size event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *  \param  pktSn       Packet sequence number.
 *  \param  ts          Timestamp.
 *  \param  timeOffs    Time offset.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadIsoTxSyncEvt(uint8_t *pBuf, uint8_t status, uint16_t handle, uint16_t pktSn,
                                        uint32_t ts, uint32_t timeOffs)
{
  const uint8_t len = LHCI_LEN_LE_READ_TX_SYNC;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);
  UINT16_TO_BSTREAM(pBuf, pktSn);
  UINT32_TO_BSTREAM(pBuf, ts);
  UINT24_TO_BSTREAM(pBuf, timeOffs);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a read ISO test counter command complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *  \param  pCtr        ISO test counters.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackIsoReadTestCounters(uint8_t *pBuf, uint8_t status, uint16_t handle, LlIsoTestCtrs_t *pCtr)
{
  const uint8_t len = LHCI_LEN_LE_ISO_READ_TEST_COUNTER;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);
  UINT32_TO_BSTREAM(pBuf, pCtr->numSuccess);
  UINT32_TO_BSTREAM(pBuf, pCtr->numMissed);
  UINT32_TO_BSTREAM(pBuf, pCtr->numFailed);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a read ISO link quality command complete event packet.
 *
 *  \param  pBuf           Packed packet buffer.
 *  \param  status         Completion status.
 *  \param  handle         Connection handle.
 *  \param  LinkQualStats  Link quality stats.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadIsoLinkQuality(uint8_t *pBuf, uint8_t status, uint16_t handle, LlIsoLinkQual_t LinkQualStats)
{
  const uint8_t len = LHCI_LEN_LE_READ_ISO_LINK_QUAL;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);
  UINT32_TO_BSTREAM(pBuf, LinkQualStats.txUnAckPkt);
  UINT32_TO_BSTREAM(pBuf, LinkQualStats.txFlushedPkt);
  UINT32_TO_BSTREAM(pBuf, LinkQualStats.txLastSubEventPkt);
  UINT32_TO_BSTREAM(pBuf, LinkQualStats.retransmitPkt);
  UINT32_TO_BSTREAM(pBuf, LinkQualStats.crcErrPkt);
  UINT32_TO_BSTREAM(pBuf, LinkQualStats.rxUnreceivedPkt);
  UINT32_TO_BSTREAM(pBuf, LinkQualStats.duplicatePkt);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack setup data path event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackSetupDataPathEvt(uint8_t *pBuf, uint8_t status, uint16_t handle)
{
  const uint8_t len = LHCI_LEN_LE_SETUP_ISO_DATA_PATH;

  UINT8_TO_BSTREAM(pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack remove data path event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackRemoveDataPathEvt(uint8_t *pBuf, uint8_t status, uint16_t handle)
{
  const uint8_t len = LHCI_LEN_LE_REMOVE_ISO_DATA_PATH;

  UINT8_TO_BSTREAM(pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack read local supported codecs event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  numStd      Number of standard codecs.
 *  \param  stdCodecs   Table of standard codec info.
 *  \param  numVs       Number of vendor specific codecs.
 *  \param  vsCodecs    Table of vendor specific codec info.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadLocalSupportedCodecs(uint8_t *pBuf, uint8_t status,
                                                uint8_t numStd, AudioStdCodecInfo_t stdCodecs[],
                                                uint8_t numVs, AudioVsCodecInfo_t vsCodecs[])
{
  uint8_t len = LHCI_LEN_READ_LOCAL_SUP_CODECS;

  UINT8_TO_BSTREAM(pBuf, status);
  UINT8_TO_BSTREAM(pBuf, numStd);

  for (size_t i = 0; i < numStd; i++)
  {
    UINT8_TO_BSTREAM(pBuf, stdCodecs[i].codecId);
    UINT8_TO_BSTREAM(pBuf, LL_CODEC_TRANS_CIS_BIT | LL_CODEC_TRANS_BIS_BIT);
    len += sizeof(uint8_t) + sizeof(uint8_t);
  }

  UINT8_TO_BSTREAM(pBuf, numVs);

  for (size_t i = 0; i < numVs; i++)
  {
    UINT16_TO_BSTREAM(pBuf, vsCodecs[i].compId);
    UINT16_TO_BSTREAM(pBuf, vsCodecs[i].codecId);
    UINT8_TO_BSTREAM(pBuf, LL_CODEC_TRANS_CIS_BIT | LL_CODEC_TRANS_BIS_BIT);
    len += sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint8_t);
  }

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  handle      Connection handle.
 *  \param  pRet        Return parameters.
 */
/*************************************************************************************************/
static void lhciIsoSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint16_t handle, const lhciIsoReturnParam_t *pRet)
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
    case HCI_OPCODE_LE_READ_BUF_SIZE_V2:
    {
      lhciPackReadBufSizeV2Evt(pBuf, status, LlGetAclMaxSize(), LlGetAclTxBufs(),
                               LlGetIsoMaxBufSize(), LlGetIsoTxBufs());
      break;
    }
    case HCI_OPCODE_LE_READ_ISO_TX_SYNC:
    {
      uint16_t pktSn;
      uint32_t ts;
      uint32_t timeOffs;

      status = LlReadIsoTxSync(handle, &pktSn, &ts, &timeOffs);
      lhciPackReadIsoTxSyncEvt(pBuf, status, handle, pktSn, ts, timeOffs);
      break;
    }
    case HCI_OPCODE_LE_REMOVE_ISO_DATA_PATH:
    {
      lhciPackRemoveDataPathEvt(pBuf, status, handle);
      break;
    }
    case HCI_OPCODE_LE_SETUP_ISO_DATA_PATH:
    {
      lhciPackSetupDataPathEvt(pBuf, status, handle);
      break;
    }
    case HCI_OPCODE_LE_ISO_READ_TEST_COUNTERS:
    {
      LlIsoTestCtrs_t ctr;

      status = LlIsoReadTestCounter(handle, &ctr);

      lhciPackIsoReadTestCounters(pBuf, status, handle, &ctr);
      break;
    }
    case HCI_OPCODE_LE_ISO_TEST_END:
    {
      LlIsoTestCtrs_t ctr;
      status = LlIsoTestEnd(handle, &ctr);

      if (status == LL_SUCCESS)
      {
        lhciIsoCb.isoRxTest = FALSE;
      }

      /* ISO Test End command complete event is identical to ISO Read Test Counters. */
      lhciPackIsoReadTestCounters(pBuf, status, handle, &ctr);
      break;
    }
    case HCI_OPCODE_LE_READ_ISO_LINK_QUAL:
    {
      LlIsoLinkQual_t qualStats;
      status = LlReadIsoLinkQual(handle, &qualStats);
      lhciPackReadIsoLinkQuality(pBuf, status, handle, qualStats);
      break;
    }
    case HCI_OPCODE_READ_LOCAL_SUP_CODECS:
    {
      uint8_t numStdCodecs = LHCI_MAX_CODEC;
      AudioStdCodecInfo_t stdCodecs[LHCI_MAX_CODEC];
      uint8_t numVsCodecs = LHCI_MAX_CODEC;
      AudioVsCodecInfo_t vsCodecs[LHCI_MAX_CODEC];

      PalCodecReadLocalSupportedCodecs(&numStdCodecs, stdCodecs,
                                       &numVsCodecs, vsCodecs);

      paramLen = lhciPackReadLocalSupportedCodecs(pBuf, status,
                                                  numStdCodecs, stdCodecs,
                                                  numVsCodecs, vsCodecs);

      /* Re-pack the event header now that the actual length is known. */
      lhciPackEvtHdr(pEvtBuf - (HCI_EVT_HDR_LEN + HCI_LEN_CMD_CMPL), HCI_CMD_CMPL_EVT, HCI_LEN_CMD_CMPL + paramLen);
      break;
    }
    case HCI_OPCODE_READ_LOCAL_SUP_CODEC_CAP:
    {
      UINT8_TO_BSTREAM(pBuf, status);
      UINT8_TO_BSTREAM(pBuf, 0);      /* Num_Codec_Capabilities */
      break;
    }
    case HCI_OPCODE_READ_LOCAL_SUP_CONTROLLER_DLY:
    {
      UINT8_TO_BSTREAM (pBuf, status);
      UINT24_TO_BSTREAM(pBuf, pRet->ctrDly.minDly);     /* Min_Controller_Delay */
      UINT24_TO_BSTREAM(pBuf, pRet->ctrDly.maxDly);     /* Max_Controller_Delay */
      break;
    }

    /* --- command completion with status only parameter --- */

    case HCI_OPCODE_LE_ISO_TX_TEST:
    case HCI_OPCODE_LE_ISO_RX_TEST:
    case HCI_OPCODE_CONFIG_DATA_PATH:
    {
      LhciPackCmdCompleteEvtStatus(pBuf, status);
      break;
    }

    /* --- default --- */

    default:
      break;
  }

  lhciSendCmdCmplEvt(pEvtBuf);
}

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Decode an HCI command packet.
 *
 *  \param  pHdr    Decoded packet header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciIsoDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciIsoVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint16_t handle = 0;
  lhciIsoReturnParam_t ret;

  switch (pHdr->opCode)
  {
    case HCI_OPCODE_LE_READ_BUF_SIZE_V2:
    {
      paramLen = LHCI_LEN_LE_READ_BUF_SIZE_V2;
      break;
    }
    case HCI_OPCODE_LE_READ_ISO_TX_SYNC:
    {
      BSTREAM_TO_UINT16(handle, pBuf);

      paramLen = LHCI_LEN_LE_READ_TX_SYNC;
      break;
    }
    case HCI_OPCODE_LE_ISO_TX_TEST:
    {
      uint8_t pldType;

      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (pldType, pBuf);

      status = LlIsoTxTest(handle, pldType);
      paramLen = LHCI_LEN_LE_ISO_TX_TEST;
      break;
    }
    case HCI_OPCODE_LE_ISO_RX_TEST:
    {
      uint8_t pldType;

      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (pldType, pBuf);

      status = LlIsoRxTest(handle, pldType);
      paramLen = LHCI_LEN_LE_ISO_RX_TEST;

      if (status == LL_SUCCESS)
      {
        lhciIsoCb.isoRxTest = TRUE;
      }
      break;
    }
    case HCI_OPCODE_LE_ISO_READ_TEST_COUNTERS:
    {
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_LE_ISO_READ_TEST_COUNTER;
      break;
    }
    case HCI_OPCODE_LE_ISO_TEST_END:
    {
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_LE_ISO_TEST_END;
      break;
    }
    case HCI_OPCODE_LE_READ_ISO_LINK_QUAL:
    {
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_LE_READ_ISO_LINK_QUAL;
      break;
    }
    case HCI_OPCODE_LE_SETUP_ISO_DATA_PATH:
    {
      LlIsoSetupDataPath_t param;

      BSTREAM_TO_UINT16(param.handle, pBuf);
      BSTREAM_TO_UINT8 (param.dpDir, pBuf);
      BSTREAM_TO_UINT8 (param.dpId, pBuf);
      BSTREAM_TO_UINT8 (param.codecFormat, pBuf);
      BSTREAM_TO_UINT16(param.codecCompId, pBuf);
      BSTREAM_TO_UINT16(param.codecId, pBuf);
      BSTREAM_TO_UINT24(param.ctrDly, pBuf);
      BSTREAM_TO_UINT8 (param.codecConfigLen, pBuf);
      param.pCodecConfig = pBuf;
      status = LlSetupIsoDataPath(&param);
      handle = param.handle;
      paramLen = LHCI_LEN_LE_SETUP_ISO_DATA_PATH;
      break;
    }
    case HCI_OPCODE_LE_REMOVE_ISO_DATA_PATH:
    {
      uint8_t dpDir;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (dpDir, pBuf);
      status = LlRemoveIsoDataPath(handle, dpDir);
      paramLen = LHCI_LEN_LE_REMOVE_ISO_DATA_PATH;
      break;
    }
    case HCI_OPCODE_CONFIG_DATA_PATH:
    {
      uint8_t dir;
      uint8_t dataPathId;
      BSTREAM_TO_UINT8 (dir, pBuf);
      BSTREAM_TO_UINT8 (dataPathId, pBuf);
      if (!PalCodecConfigureDataPath(dir, dataPathId))
      {
        status = HCI_ERR_INVALID_PARAM;
      }
      paramLen = LHCI_LEN_CONFIG_DATA_PATH;
      break;
    }
    case HCI_OPCODE_READ_LOCAL_SUP_CODECS:
    {
      /* Reserve maximum buffer size, re-pack length once size is known. */
      paramLen = LHCI_LEN_READ_LOCAL_SUP_CODECS +
                 (LHCI_MAX_CODEC * 2) +     /* Max standard codecs */
                 (LHCI_MAX_CODEC * 5);      /* Max VS codecs */
      break;
    }
    case HCI_OPCODE_READ_LOCAL_SUP_CODEC_CAP:
    {
      uint8_t codingFmt;
      uint16_t compId;
      uint16_t vsCodecId;
      uint8_t transType;
      uint8_t dir;
      BSTREAM_TO_UINT8 (codingFmt, pBuf);
      BSTREAM_TO_UINT16(compId, pBuf);
      BSTREAM_TO_UINT16(vsCodecId, pBuf);
      BSTREAM_TO_UINT8 (transType, pBuf);
      BSTREAM_TO_UINT8 (dir, pBuf);

      if (PalCodecReadLocalSupportedCodecCapabilities(codingFmt, compId, vsCodecId, dir))
      {
        if ((transType & (LL_CODEC_TRANS_CIS_BIT | LL_CODEC_TRANS_BIS_BIT)) == 0)
        {
          status = HCI_ERR_INVALID_PARAM;
        }
      }
      else
      {
        status = HCI_ERR_INVALID_PARAM;
      }

      paramLen = LHCI_LEN_READ_LOCAL_SUP_CODEC_CAP;
      break;
    }
    case HCI_OPCODE_READ_LOCAL_SUP_CONTROLLER_DLY:
    {
      uint8_t codingFmt;
      uint16_t compId;
      uint16_t vsCodecId;
      uint8_t transType;
      uint8_t dir;
      BSTREAM_TO_UINT8 (codingFmt, pBuf);
      BSTREAM_TO_UINT16(compId, pBuf);
      BSTREAM_TO_UINT16(vsCodecId, pBuf);
      BSTREAM_TO_UINT8 (transType, pBuf);
      BSTREAM_TO_UINT8 (dir, pBuf);
      ret.ctrDly.minDly = ret.ctrDly.maxDly = 0;
      if (PalCodecReadLocalSupportedControllerDelay(codingFmt, compId, vsCodecId, dir,
                                                    &ret.ctrDly.minDly, &ret.ctrDly.maxDly))
      {
        if ((transType & (LL_CODEC_TRANS_CIS_BIT | LL_CODEC_TRANS_BIS_BIT)) == 0)
        {
          status = HCI_ERR_INVALID_PARAM;
        }
      }
      else
      {
        status = HCI_ERR_INVALID_PARAM;
      }
      paramLen = LHCI_LEN_READ_LOCAL_SUP_CONTROLLER_DLY;
      break;
    }
    default:
      return FALSE;     /* exit dispatcher routine */
  }

  if (paramLen > 0)
  {
    lhciIsoSendCmdCmplEvt(pHdr, status, paramLen, handle, &ret);
  }

  return TRUE;
}
