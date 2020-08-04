/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Simple implementation of SMP when not supported.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
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

#include <string.h>
#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "dm_api.h"
#include "l2c_api.h"
#include "smp_api.h"
#include "smp_main.h"

/*************************************************************************************************/
/*!
 *  \brief  L2C control callback for SMP.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void smpNonL2cCtrlCback(wsfMsgHdr_t *pMsg)
{
  return;
}

/*************************************************************************************************/
/*!
 *  \brief  L2CAP data callback for SMP when SMP not supported.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void smpNonL2cDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  uint8_t *pRsp;
  uint8_t *p;
  uint8_t role;
  dmConnId_t connId;

  if ((connId = DmConnIdByHandle(handle)) == DM_CONN_ID_NONE)
  {
    return;
  }

  role = DmConnRole(connId);

  p = pPacket + L2C_PAYLOAD_START;

  /* SMP is not supported so fail gracefully */

  /* if slave and pairing request received, or master and security request received */
  if ((role == DM_ROLE_SLAVE && *p == SMP_CMD_PAIR_REQ) ||
      (role == DM_ROLE_MASTER && *p == SMP_CMD_SECURITY_REQ))
  {
    /* send pairing failed */
    if ((pRsp = smpMsgAlloc(L2C_PAYLOAD_START + SMP_PAIR_FAIL_LEN)) != NULL)
    {
      p = pRsp + L2C_PAYLOAD_START;
      UINT8_TO_BSTREAM(p, SMP_CMD_PAIR_FAIL);
      UINT8_TO_BSTREAM(p, SMP_ERR_PAIRING_NOT_SUP);

      L2cDataReq(L2C_CID_SMP, handle, SMP_PAIR_FAIL_LEN, pRsp);
    }
  }

  /* all other messages are ignored */
}

/*************************************************************************************************/
/*!
 *  \brief  Use this SMP init function when SMP is not supported.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpNonInit(void)
{
  /* Register with L2C */
  L2cRegister(L2C_CID_SMP, smpNonL2cDataCback, smpNonL2cCtrlCback);
}
