/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) master control interface implementation file.
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

#include "lctr_api_bis_master.h"
#include "wsf_math.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Internal functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Check if the create BIG parameters are valid or not.
 *
 *  \param      pParam     Create BIG test parameters.
 *
 *  \return     True if parameter is valid, False otherwise.
 */
/*************************************************************************************************/
static bool_t LlIsBigCreateSyncParamsValid(LlBigCreateSync_t *pParam)
{
  const uint8_t MAX_BIG_HANDLE = 0xEF;
  const uint16_t MAX_SYNC_HANDLE = 0x0EFF;
  const uint8_t MAX_ENCRYPTION = 0x01;
  const uint8_t MAX_MSE = 0x1F;
  const uint16_t MIN_BIG_SYNC_TIMEOUT = 0x000A;
  const uint16_t MAX_BIG_SYNC_TIMEOUT = 0x4000;
  const uint8_t MIN_BIS = LL_MIN_BIS;
  const uint8_t MAX_BIS = 0x1F;

  if (pParam->bigHandle > MAX_BIG_HANDLE)
  {
    LL_TRACE_WARN1("bigHandle=%u out of range", pParam->bigHandle);
    return FALSE;
  }
  if (pParam->syncHandle > MAX_SYNC_HANDLE)
  {
    LL_TRACE_WARN1("syncHandle=%u out of range", pParam->syncHandle);
    return FALSE;
  }
  if (pParam->encrypt > MAX_ENCRYPTION)
  {
    LL_TRACE_WARN1("encrypt=%u out of range", pParam->encrypt);
    return FALSE;
  }
  if (pParam->mse > MAX_MSE)
  {
    LL_TRACE_WARN1("mse=%u out of range", pParam->mse);
    return FALSE;
  }
  if ((pParam->bigSyncTimeout < MIN_BIG_SYNC_TIMEOUT) || (pParam->bigSyncTimeout > MAX_BIG_SYNC_TIMEOUT))
  {
    LL_TRACE_WARN1("bigSyncTimeout=%u out of range", pParam->bigSyncTimeout);
    return FALSE;
  }
  if ((pParam->numBis < MIN_BIS) || (pParam->numBis > MAX_BIS))
  {
    LL_TRACE_WARN1("numBis=%u out of range", pParam->numBis);
    return FALSE;
  }

  return TRUE;
}

/**************************************************************************************************
  External functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Used to synchronize and receive PDUs from one or more BISes within a BIG.
 *
 *  \param      pCreateSync     BIG Create Sync parameters.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlBigCreateSync(LlBigCreateSync_t *pCreateSync)
{
  LL_TRACE_INFO2("### LlApi ###  LlBigCreateSync bigHandle=%u syncHandle=%u", pCreateSync->bigHandle, pCreateSync->syncHandle);

  if ((LL_API_PARAM_CHECK == 1) &&
      (LlIsBigCreateSyncParamsValid(pCreateSync) == FALSE))
  {
    return LL_ERROR_CODE_PARAM_OUT_OF_MANDATORY_RANGE;
  }

  return LctrMstBigCreateSync(pCreateSync);
}

/*************************************************************************************************/
/*!
 *  \brief      Used to stop synchronization with the BIG or to cancel the process of synchronizing
 *              to BISes invoked by the HCI_LE_BIG_Create_Sync command
 *
 *  \param      bigHandle     Used to identify the BIG.
 */
/*************************************************************************************************/
void LlBigTerminateSync(uint8_t bigHandle)
{
  LL_TRACE_INFO1("### LlApi ###  LlBigTerminateSync bigHandle=%u", bigHandle);

  LctrMstBigTerminateSync(bigHandle);
}
