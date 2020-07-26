/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) power control interface implementation file.
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
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lctr_api_conn.h"
#include "lmgr_api.h"
#include "lmgr_api_conn.h"
#include "wsf_assert.h"
#include "wsf_cs.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include <string.h>
#include "lctr_api_pc.h"

/*************************************************************************************************/
/*!
 *  \brief  Set path loss monitoring parameters.
 *
 *  \param  handle      Handle identifier for connection.
 *  \param  highThresh  High extreme threshold.
 *  \param  highHyst    High extreme hysteresis.
 *  \param  lowThresh   Low extreme threshold.
 *  \param  lowHyst     Low extreme hysteresis.
 *  \param  minTime     Minimum time spent to trigger event generation.
 *
 *  \return Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetPathLossReportingParams(uint16_t handle, uint8_t highThresh, uint8_t highHyst, uint8_t lowThresh, uint8_t lowHyst, uint16_t minTime)
{
  LL_TRACE_INFO0("### LlApi ###  LlSetPathLossReportingParams");

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if (highThresh == LL_PC_PATH_LOSS_UNUSED_HIGH_THRESHOLD)
  {
    highHyst = 0;
  }

  if ((((uint16_t) highThresh + (uint16_t) highHyst) > 0x00FF) ||
      (lowThresh < lowHyst))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  if ((lowThresh > highThresh) ||
      ((lowThresh + lowHyst) > (highThresh - highHyst)))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  return lctrSetPathLossReportingParams(handle, highThresh, highHyst, lowThresh, lowHyst, minTime);
}

/*************************************************************************************************/
/*!
 *  \brief      Set path loss enable.
 *
 *  \param      handle          Connection handle.
 *  \param      enable          Enable flag.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetPathLossReportingEnable(uint16_t handle, uint8_t enable)
{
  LL_TRACE_INFO0("### LlApi ###  LlSetPathLossReportingEnable");

  if ((LL_API_PARAM_CHECK == 1) &&
    ((handle >= pLctrRtCfg->maxConn) ||
    !LctrIsConnHandleEnabled(handle)))
    {
     return LL_ERROR_CODE_UNKNOWN_CONN_ID;
    }

  return lctrSetPathLossReportingEnable(handle, enable);
}
