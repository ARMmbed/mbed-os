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
 *  \brief HCI vendor specific AE functions for generic controllers.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "util/bstream.h"
#include "hci_core.h"

#if HCI_VS_TARGET == HCI_VS_GENERIC

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

static void hciCoreReadMaxAdvDataLen(void);
static void hciCoreReadNumSupAdvSets(void);
static void hciCoreReadPerAdvListSize(void);

/*************************************************************************************************/
/*!
 *  \brief  Read maximum advertising data length command.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void hciCoreReadMaxAdvDataLen(void)
{
  /* if LE Extended Advertising is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_LE_EXT_ADV) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_LE_EXT_ADV))
  {
    /* send next command in sequence */
    HciLeReadMaxAdvDataLen();
  }
  else
  {
    hciCoreCb.maxAdvDataLen = 0;

    /* send next command in sequence */
    hciCoreReadNumSupAdvSets();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Read read number of supported advertising sets command.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void hciCoreReadNumSupAdvSets(void)
{
  /* if LE Extended Advertising is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_LE_EXT_ADV) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_LE_EXT_ADV))
  {
    /* send next command in sequence */
    HciLeReadNumSupAdvSets();
  }
  else
  {
    hciCoreCb.numSupAdvSets = 0;

    /* send next command in sequence */
    hciCoreReadPerAdvListSize();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Read periodic advertiser list size command.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void hciCoreReadPerAdvListSize(void)
{
  /* if LE Extended Advertising is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_LE_PER_ADV) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_LE_PER_ADV))
  {
    /* send next command in sequence */
    HciLeReadPerAdvListSizeCmd();
  }
  else
  {
    hciCoreCb.perAdvListSize = 0;

    /* send next command in sequence */
    HciLeRandCmd();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Implement the HCI extended reset sequence.
 *
 *  \param  pMsg    HCI event message from previous command in the sequence.
 *  \param  opcode  HCI event message opcode.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void hciCoreExtResetSequence(uint8_t *pMsg, uint16_t opcode)
{
  /* decode opcode */
  switch (opcode)
  {
    case HCI_OPCODE_READ_LOCAL_VER_INFO:
      /* send next command in sequence */
      hciCoreReadMaxAdvDataLen();
      break;

    case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
      BSTREAM_TO_UINT16(hciCoreCb.maxAdvDataLen, pMsg);

      /* send next command in sequence */
      hciCoreReadNumSupAdvSets();
      break;

    case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
      /* parse and store event parameters */
      BSTREAM_TO_UINT8(hciCoreCb.numSupAdvSets, pMsg);

      /* send next command in sequence */
      hciCoreReadPerAdvListSize();
      break;

    case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
      /* parse and store event parameters */
      BSTREAM_TO_UINT8(hciCoreCb.perAdvListSize, pMsg);

      /* send next command in sequence */
      HciLeRandCmd();
      break;

    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Vendor-specific controller AE initialization function.
 *
 *  \param  param    Vendor-specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVsAeInit(uint8_t param)
{
  hciCoreCb.extResetSeq = hciCoreExtResetSequence;
}

#endif
