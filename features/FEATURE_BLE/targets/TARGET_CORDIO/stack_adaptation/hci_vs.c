/*************************************************************************************************/
/*!
 *  \file   hci_vs.c
 *
 *  \brief  HCI vendor specific functions for generic controllers.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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
 *
 *  This module implements vendor-specific features for the reset sequence and vendor-specific
 *  HCI commands and events.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bda.h"
#include "util/bstream.h"
#include "hci_core.h"
#include "hci_api.h"
#include "hci_main.h"
#include "hci_cmd.h"

#if HCI_VS_TARGET == HCI_VS_GENERIC

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

static void hciCoreReadResolvingListSize(void);
static void hciCoreReadMaxDataLen(void);

/*************************************************************************************************/
/*!
 *  \fn     hciCoreReadResolvingListSize
 *
 *  \brief  Read resolving list command.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void hciCoreReadResolvingListSize(void)
{
  /* if LL Privacy is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_PRIVACY) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_PRIVACY))
  {
    /* send next command in sequence */
    HciLeReadResolvingListSize();
  }
  else
  {
    hciCoreCb.resListSize = 0;

    /* send next command in sequence */
    hciCoreReadMaxDataLen();
  }
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreReadMaxDataLen
 *
 *  \brief  Read maximum data length command.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void hciCoreReadMaxDataLen(void)
{
  /* if LE Data Packet Length Extensions is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_DATA_LEN_EXT) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_DATA_LEN_EXT))
  {
    /* send next command in sequence */
    HciLeReadMaxDataLen();
  }
  else
  {
    /* send next command in sequence */
    HciLeRandCmd();
  }
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreResetStart
 *
 *  \brief  Start the HCI reset sequence.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreResetStart(void)
{
  /* send an HCI Reset command to start the sequence */
  HciResetCmd();
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreResetSequence
 *
 *  \brief  Implement the HCI reset sequence.
 *
 *  \param  pMsg    HCI event message from previous command in the sequence.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreResetSequence(uint8_t *pMsg)
{
  uint16_t       opcode;
  wsfMsgHdr_t    hdr;
  static uint8_t randCnt;

  /* if event is a command complete event */
  if (*pMsg == HCI_CMD_CMPL_EVT)
  {
    /* parse parameters */
    pMsg += HCI_EVT_HDR_LEN;
    pMsg++;                   /* skip num packets */
    BSTREAM_TO_UINT16(opcode, pMsg);
    pMsg++;                   /* skip status */

    /* decode opcode */
    switch (opcode)
    {
      case HCI_OPCODE_RESET:
        /* initialize rand command count */
        randCnt = 0;

        /* send next command in sequence */
        HciSetEventMaskCmd((uint8_t *) hciEventMask);
        break;

      case HCI_OPCODE_SET_EVENT_MASK:
        /* send next command in sequence */
        HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask);
        break;

      case HCI_OPCODE_LE_SET_EVENT_MASK:
        /* send next command in sequence */
        HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2);
        break;

      case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
        /* send next command in sequence */
        HciReadBdAddrCmd();
        break;

      case HCI_OPCODE_READ_BD_ADDR:
        /* parse and store event parameters */
        BdaCpy(hciCoreCb.bdAddr, pMsg);

        /* send next command in sequence */
        HciLeReadBufSizeCmd();
        break;

      case HCI_OPCODE_LE_READ_BUF_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
        BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

        /* initialize ACL buffer accounting */
        hciCoreCb.availBufs = hciCoreCb.numBufs;

        /* send next command in sequence */
        HciLeReadSupStatesCmd();
        break;

      case HCI_OPCODE_LE_READ_SUP_STATES:
        /* parse and store event parameters */
        memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

        /* send next command in sequence */
        HciLeReadWhiteListSizeCmd();
        break;

      case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

        /* send next command in sequence */
        HciLeReadLocalSupFeatCmd();
        break;

      case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
        /* parse and store event parameters */
        BSTREAM_TO_UINT64(hciCoreCb.leSupFeat, pMsg);

        /* if Isochronous Channels (Host support) is supported and included */
        if (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_ISO_HOST_SUPPORT)
        {
          HciLeSetHostFeatureCmd(HCI_LE_FEAT_BIT_ISO_HOST_SUPPORT, TRUE);
          break;
        }
        /* Fallthrough */

      case HCI_OPCODE_LE_SET_HOST_FEATURE:
        /* send next command in sequence */
        hciCoreReadResolvingListSize();
        break;

      case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT8(hciCoreCb.resListSize, pMsg);

        /* send next command in sequence */
        hciCoreReadMaxDataLen();
        break;

      case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
        {
          uint16_t maxTxOctets;
          uint16_t maxTxTime;

          BSTREAM_TO_UINT16(maxTxOctets, pMsg);
          BSTREAM_TO_UINT16(maxTxTime, pMsg);

          /* use Controller's maximum supported payload octets and packet duration times
           * for transmission as Host's suggested values for maximum transmission number
           * of payload octets and maximum packet transmission time for new connections.
           */
          HciLeWriteDefDataLen(maxTxOctets, maxTxTime);
        }
        break;

      case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
        /* send next command in sequence */
        HciReadLocalVerInfoCmd();
        break;

      case HCI_OPCODE_READ_LOCAL_VER_INFO:
        /* parse and store event parameters */
        BSTREAM_TO_UINT8(hciCoreCb.locVerInfo.hciVersion, pMsg);
        BSTREAM_TO_UINT16(hciCoreCb.locVerInfo.hciRevision, pMsg);
        BSTREAM_TO_UINT8(hciCoreCb.locVerInfo.lmpVersion, pMsg);
        BSTREAM_TO_UINT16(hciCoreCb.locVerInfo.manufacturerName, pMsg);
        BSTREAM_TO_UINT16(hciCoreCb.locVerInfo.lmpSubversion, pMsg);

        if (hciCoreCb.extResetSeq)
        {
          /* send first extended command */
          (*hciCoreCb.extResetSeq)(pMsg, opcode);
        }
        else
        {
          /* initialize extended parameters */
          hciCoreCb.maxAdvDataLen = 0;
          hciCoreCb.numSupAdvSets = 0;
          hciCoreCb.perAdvListSize = 0;

          /* send next command in sequence */
          HciLeRandCmd();
        }
        break;

      case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
      case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
      case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
        if (hciCoreCb.extResetSeq)
        {
          /* send next extended command in sequence */
          (*hciCoreCb.extResetSeq)(pMsg, opcode);
        }
        break;

      case HCI_OPCODE_LE_RAND:
        /* check if need to send second rand command */
        if (randCnt < (HCI_RESET_RAND_CNT-1))
        {
          randCnt++;
          HciLeRandCmd();
        }
        else
        {
          /* last command in sequence; set resetting state and call callback */
          hciCb.resetting = FALSE;
          hdr.param = 0;
          hdr.event = HCI_RESET_SEQ_CMPL_CBACK_EVT;
          (*hciCb.evtCback)((hciEvt_t *) &hdr);
        }
        break;

      default:
        break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreVsCmdCmplRcvd
 *
 *  \brief  Perform internal HCI processing of vendor specific command complete events.
 *
 *  \param  opcode  HCI command opcode.
 *  \param  pMsg    Pointer to input HCI event parameter byte stream.
 *  \param  len     Parameter byte stream length.
 *
 *  \return HCI callback event code or zero.
 */
/*************************************************************************************************/
uint8_t hciCoreVsCmdCmplRcvd(uint16_t opcode, uint8_t *pMsg, uint8_t len)
{
  return HCI_VENDOR_SPEC_CMD_CMPL_CBACK_EVT;
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreVsEvtRcvd
 *
 *  \brief  Perform internal HCI processing of vendor specific HCI events.
 *
 *  \param  p       Pointer to input HCI event parameter byte stream.
 *  \param  len     Parameter byte stream length.
 *
 *  \return HCI callback event code or zero.
 */
/*************************************************************************************************/
uint8_t hciCoreVsEvtRcvd(uint8_t *p, uint8_t len)
{
  return HCI_VENDOR_SPEC_EVT;
}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreHwErrorRcvd
 *
 *  \brief  Perform internal HCI processing of hardware error event.
 *
 *  \param  p       Pointer to input HCI event parameter byte stream.
 *
 *  \return HCI callback event code or zero.
 */
/*************************************************************************************************/
uint8_t hciCoreHwErrorRcvd(uint8_t *p)
{
  return 0;
}

/*************************************************************************************************/
/*!
 *  \fn     HciVsInit
 *
 *  \brief  Vendor-specific controller initialization function.
 *
 *  \param  param    Vendor-specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVsInit(uint8_t param)
{
  hciCoreCb.extResetSeq = NULL;
}

#endif
