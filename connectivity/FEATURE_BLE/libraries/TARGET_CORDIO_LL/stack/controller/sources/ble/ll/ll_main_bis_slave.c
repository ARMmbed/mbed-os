/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) slave control interface implementation file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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

#include "lctr_api_bis_slave.h"
#include "lctr_api_adv_slave_ae.h"
#include "wsf_trace.h"
#include <string.h>

/**************************************************************************************************
  Internal functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Check if the create BIG parameters are valid or not.
 *
 *  \param      pParam      Create BIG command parameters.
 *
 *  \return     TRUE if parameter is valid, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t LlIsBigParamsValid(LlCreateBig_t *pParam)
{
  const uint8_t MAX_BIG_HANDLE = 0xEF;
  const uint8_t MAX_ADV_HANDLE = 0xEF;
  const uint8_t MIN_NUM_BIS = 0x01;
  const uint8_t MAX_NUM_BIS = 0x1F;
  const uint32_t MIN_SDU_INTERVAL = 0x00100;
  const uint32_t MAX_SDU_INTERVAL = 0xFFFFF;
  const uint16_t MAX_SDU = 0x0FFF;
  const uint16_t MAX_TRANSPORT_LATENCY = 0x0FA0;
  const uint8_t MAX_RTN = 0x0F;
  const uint8_t MIN_PHY = 0x01;
  const uint8_t MAX_PACKING = 0x01;
  const uint8_t MAX_FRAMING = 0x01;
  const uint8_t MAX_ENCRYPTION = 0x01;

  if (pParam->bigHandle > MAX_BIG_HANDLE)
  {
    LL_TRACE_WARN1("bigHandle=%u out of range", pParam->bigHandle);
    return FALSE;
  }
  if (pParam->advHandle > MAX_ADV_HANDLE)
  {
    LL_TRACE_WARN1("advHandle=%u out of range", pParam->advHandle);
    return FALSE;
  }
  if ((pParam->numBis < MIN_NUM_BIS) || (pParam->numBis > MAX_NUM_BIS))
  {
    LL_TRACE_WARN1("numBis=%u out of range", pParam->numBis);
    return FALSE;
  }
  if ((pParam->sduInterUsec < MIN_SDU_INTERVAL) || (pParam->sduInterUsec > MAX_SDU_INTERVAL))
  {
    LL_TRACE_WARN1("sduIntervUsec=%u out of range", pParam->sduInterUsec);
    return FALSE;
  }
  if (pParam->maxSdu > MAX_SDU)
  {
    LL_TRACE_WARN1("maxSdu=%u value out of range", pParam->maxSdu);
    return FALSE;
  }
  if (pParam->mtlMs > MAX_TRANSPORT_LATENCY)
  {
    LL_TRACE_WARN1("mtl=%u value out of range", pParam->mtlMs);
    return FALSE;
  }
  if (pParam->rtn > MAX_RTN)
  {
    LL_TRACE_WARN1("rtn=%u value out of range", pParam->rtn);
    return FALSE;
  }
  if (pParam->phys < MIN_PHY)
  {
    LL_TRACE_WARN1("phy=0x%02x value out of range", pParam->phys);
    return FALSE;
  }
  if (pParam->packing > MAX_PACKING)
  {
    LL_TRACE_WARN1("packing=%u value out of range", pParam->packing);
    return FALSE;
  }
  if (pParam->framing > MAX_FRAMING)
  {
    LL_TRACE_WARN1("framing=%u value out of range", pParam->framing);
    return FALSE;
  }
  if (pParam->encrypt > MAX_ENCRYPTION)
  {
    LL_TRACE_WARN1("encryption=%u value out of range", pParam->encrypt);
    return FALSE;
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Check if the create BIG test parameters are valid or not.
 *
 *  \param      pParam      Create BIG test parameters.
 *
 *  \return     True if parameter is valid, False otherwise.
 */
/*************************************************************************************************/
static bool_t LlIsBigTestParamsValid(LlCreateBigTest_t *pParam)
{
  const uint8_t MAX_BIG_HANDLE = 0xEF;
  const uint8_t MAX_ADV_HANDLE = 0xEF;
  const uint8_t MIN_NUM_BIS = 0x01;
  const uint8_t MAX_NUM_BIS = 0x1F;
  const uint32_t MIN_SDU_INTERVAL = 0x00100;
  const uint32_t MAX_SDU_INTERVAL = 0xFFFFF;
  const uint16_t MIN_ISO_INTERVAL = 0x0004;
  const uint16_t MAX_ISO_INTERVAL = 0x0C80;
  const uint8_t MIN_NUM_NSE = 0x01;
  const uint8_t MAX_NUM_NSE = 0x1F;
  const uint16_t MAX_SDU = 0x0FFF;
  const uint8_t MIN_PDU = 0x01;
  const uint8_t MAX_PDU = 0xFB;
  const uint8_t MIN_PHY = 0x01;
  const uint8_t MAX_PACKING = 0x01;
  const uint8_t MAX_FRAMING = 0x01;
  const uint8_t MIN_BN = 0x01;
  const uint8_t MAX_BN = 0x07;
  const uint8_t MIN_IRC = 0x01;
  const uint8_t MAX_IRC = 0x0F;
  const uint8_t MAX_PTO = 0x0F;
  const uint8_t MAX_ENCRYPTION = 0x01;

  if (pParam->bigHandle > MAX_BIG_HANDLE)
  {
    LL_TRACE_WARN1("bigHandle=%u out of range", pParam->bigHandle);
    return FALSE;
  }
  if (pParam->advHandle > MAX_ADV_HANDLE)
  {
    LL_TRACE_WARN1("advHandle=%u out of range", pParam->advHandle);
    return FALSE;
  }
  if ((pParam->numBis < MIN_NUM_BIS) || (pParam->numBis > MAX_NUM_BIS))
  {
    LL_TRACE_WARN1("numBis=%u out of range", pParam->numBis);
    return FALSE;
  }
  if ((pParam->sduInterUsec < MIN_SDU_INTERVAL) || (pParam->sduInterUsec > MAX_SDU_INTERVAL))
  {
    LL_TRACE_WARN1("sduInterval=%u out of range", pParam->sduInterUsec);
    return FALSE;
  }
  if ((pParam->isoInter < MIN_ISO_INTERVAL) || (pParam->isoInter > MAX_ISO_INTERVAL))
  {
    LL_TRACE_WARN1("isoInterval=%u out of range", pParam->isoInter);
    return FALSE;
  }
  if ((pParam->nse < MIN_NUM_NSE) || (pParam->nse > MAX_NUM_NSE))
  {
    LL_TRACE_WARN1("NSE=%u out of range", pParam->nse);
    return FALSE;
  }
  if (pParam->maxSdu > MAX_SDU)
  {
    LL_TRACE_WARN1("maxSdu=%u out of range", pParam->maxSdu);
    return FALSE;
  }
  if ((pParam->maxPdu < MIN_PDU) || (pParam->maxPdu > MAX_PDU))
  {
    LL_TRACE_WARN1("maxPdu=%u out of range", pParam->maxPdu);
    return FALSE;
  }
  if (pParam->phys < MIN_PHY)
  {
    LL_TRACE_WARN1("phy=0x%02x out of range", pParam->phys);
    return FALSE;
  }
  if (pParam->packing > MAX_PACKING)
  {
    LL_TRACE_WARN1("packing=%u out of range", pParam->packing);
    return FALSE;
  }
  if (pParam->framing > MAX_FRAMING)
  {
    LL_TRACE_WARN1("framing=%u out of range", pParam->framing);
    return FALSE;
  }
  if ((pParam->bn < MIN_BN) || (pParam->bn > MAX_BN))
  {
    LL_TRACE_WARN1("BN=%u out of range", pParam->bn);
    return FALSE;
  }
  if ((pParam->irc < MIN_IRC) || (pParam->irc > MAX_IRC))
  {
    LL_TRACE_WARN1("IRC=%u out of range", pParam->irc);
    return FALSE;
  }
  if (pParam->pto > MAX_PTO)
  {
    LL_TRACE_WARN1("PTO=%u out of range", pParam->pto);
    return FALSE;
  }
  if (pParam->encrypt > MAX_ENCRYPTION)
  {
    LL_TRACE_WARN1("encrypt=%u out of range", pParam->encrypt);
    return FALSE;
  }

  if ((pParam->framing == LL_ISO_PDU_TYPE_UNFRAMED) &&
      (((pParam->isoInter * 1250) % pParam->sduInterUsec) > 0))
  {
    LL_TRACE_WARN0("Unframed PDUs must have ISO_Interval that are multiples of the SDU_Interval");
    return FALSE;
  }

  return TRUE;
}

/**************************************************************************************************
  External functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Used by a broadcaster host to command is used to create one or more BISes of a BIG
 *              in the controller.
 *
 *  \param      pCreateBig       Create BIG parameters.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlCreateBig(LlCreateBig_t *pCreateBig)
{
  LL_TRACE_INFO2("### LlApi ###  LlCreateBig bigHandle=%u advHandle=%u", pCreateBig->bigHandle, pCreateBig->advHandle);

  if ((LL_API_PARAM_CHECK == 1) &&
      (LlIsBigParamsValid(pCreateBig) == FALSE))
  {
    return LL_ERROR_CODE_PARAM_OUT_OF_MANDATORY_RANGE;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      (LctrIsPerAdvEnabled(pCreateBig->advHandle) == FALSE))
  {
    LL_TRACE_WARN1("Periodic advertiser not found, advHandle=%u", pCreateBig->advHandle);
    return LL_ERROR_CODE_UNKNOWN_ADV_ID;
  }

  uint8_t supPhyMask =                            LL_PHYS_LE_1M_BIT |
                       (pLctrRtCfg->phy2mSup    ? LL_PHYS_LE_2M_BIT : 0) |
                       (pLctrRtCfg->phyCodedSup ? LL_PHYS_LE_CODED_BIT : 0);

  if ((LL_API_PARAM_CHECK == 1) &&
      (pCreateBig->phys & (~supPhyMask)))
  {
    LL_TRACE_WARN1("Unknown PHY bits, phys=%u", pCreateBig->phys);
    return LL_ERROR_CODE_UNSUPPORTED_FEATURE_PARAM_VALUE;
  }

  return LctrSlvBisCreateBig(pCreateBig);
}

/*************************************************************************************************/
/*!
 *  \brief      Used by a broadcaster host to command is used to create one or more BISes of a BIG
 *              in the ISO test mode.
 *
 *  \param      pCreateBigTest   Create BIG parameters.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlCreateBigTest(LlCreateBigTest_t *pCreateBigTest)
{
  uint8_t status = LL_SUCCESS;

  LL_TRACE_INFO2("### LlApi ###  LlCreateBigTest bigHandle=%u advHandle=%u", pCreateBigTest->bigHandle, pCreateBigTest->advHandle);

  if ((LL_API_PARAM_CHECK == 1) &&
      (LlIsBigTestParamsValid(pCreateBigTest) == FALSE))
  {
    return LL_ERROR_CODE_PARAM_OUT_OF_MANDATORY_RANGE;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      (pCreateBigTest->nse < (pCreateBigTest->irc * pCreateBigTest->bn)))
  {
    LL_TRACE_WARN0("NSE not less than 'IRC * BN'");
    return LL_ERROR_CODE_UNSUPPORTED_FEATURE_PARAM_VALUE;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      (LctrIsPerAdvEnabled(pCreateBigTest->advHandle) == FALSE))
  {
    LL_TRACE_WARN1("Periodic advertiser not found, advHandle=%u", pCreateBigTest->advHandle);
    return LL_ERROR_CODE_UNKNOWN_ADV_ID;
  }

  uint8_t supPhyMask =                            LL_PHYS_LE_1M_BIT |
                       (pLctrRtCfg->phy2mSup    ? LL_PHYS_LE_2M_BIT : 0) |
                       (pLctrRtCfg->phyCodedSup ? LL_PHYS_LE_CODED_BIT : 0);

  if ((LL_API_PARAM_CHECK == 1) &&
      (pCreateBigTest->phys & (~supPhyMask)))
  {
    LL_TRACE_WARN1("Unknown PHY bits, phys=%u", pCreateBigTest->phys);
    return LL_ERROR_CODE_UNSUPPORTED_FEATURE_PARAM_VALUE;
  }

  status = LctrSlvBisCreateBigTest(pCreateBigTest);

  return status;
}

/*************************************************************************************************/
/*!
 *  \brief      Used to terminate the transmission of all BISes of a BIG, or to cancel the process
 *              of creating a BIG using the HCI_LE_Create_BIG command from the Isochronous Broadcaster.
 *
 *  \param      bigHandle     Used to identify the BIG.
 *  \param      reason        Termination reason.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlTerminateBig(uint8_t bigHandle, uint8_t reason)
{
  uint8_t status = LL_SUCCESS;

  LL_TRACE_INFO2("### LlApi ###  LlTerminateBig bigHandle=%u advHandle=%u", bigHandle, reason);

  status = LctrSlvBisTerminateBig(bigHandle, reason);

  return status;
}
