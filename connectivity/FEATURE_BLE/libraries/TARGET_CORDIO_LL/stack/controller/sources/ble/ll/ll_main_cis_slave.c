/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) master control interface implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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
#include "lmgr_api.h"
#include "lctr_api_cis_slave.h"
#include "util/bstream.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Used by the slave host to inform the controller to accept the request for the
 *              connected isochronous stream.
 *
 *  \param      cisHandle       Handle of the connected isochronous stream.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlAcceptCisReq(uint16_t cisHandle)
{
  uint8_t status = LL_SUCCESS;

  LL_TRACE_INFO1("### LlApi ###  LlAcceptCisReq cisHandle=%d", cisHandle);

  status = LctrAcceptCisReq(cisHandle);

  return status;
}

/*************************************************************************************************/
/*!
 *  \brief      Used by the slave host to inform the controller to reject the request for the
 *              connected isochronous stream.
 *
 *  \param      cisHandle       Handle of the connected isochronous stream.
 *  \param      reason          Indicate the reason for rejecting the request.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlRejectCisReq(uint16_t cisHandle, uint8_t reason)
{
  uint8_t status = LL_SUCCESS;

  LL_TRACE_INFO2("### LlApi ###  LlRejectCisReq cisHandle=%d reason=%d", cisHandle, reason);

  status = LctrRejectCisReq(cisHandle, reason);

  return status;
}
