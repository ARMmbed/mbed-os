/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM security module for slave.
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

#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/calc128.h"
#include "dm_api.h"
#include "dm_sec.h"
#include "dm_main.h"

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a slave device to proceed with pairing after a
 *          DM_SEC_PAIR_IND event is received.
 *
 *  \param  connId    DM connection ID.
 *  \param  oob       Out-of-band pairing data present or not present.
 *  \param  auth      Authentication and bonding flags.
 *  \param  iKeyDist  Initiator key distribution flags.
 *  \param  rKeyDist  Responder key distribution flags.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecPairRsp(dmConnId_t connId, uint8_t oob, uint8_t auth, uint8_t iKeyDist, uint8_t rKeyDist)
{
  smpDmPair_t  *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(smpDmPair_t))) != NULL)
  {
    pMsg->hdr.event = SMP_MSG_API_PAIR_RSP;
    pMsg->hdr.param = connId;
    pMsg->oob = oob;
    pMsg->auth = auth;

    /* clear any erroneous key dist bits set by app */
    pMsg->iKeyDist = iKeyDist & SMP_KEY_DIST_MASK;
    pMsg->rKeyDist = rKeyDist & SMP_KEY_DIST_MASK;

    /* note we're sending this to SMP */
    SmpDmMsgSend((smpDmMsg_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a slave device to request that the master initiates
 *          pairing or link encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  auth      Authentication flags.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSlaveReq(dmConnId_t connId, uint8_t auth)
{
  smpDmSecurityReq_t  *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(smpDmSecurityReq_t))) != NULL)
  {
    pMsg->hdr.event = SMP_MSG_API_SECURITY_REQ;
    pMsg->hdr.param = connId;
    pMsg->auth = auth;

    /* note we're sending this to SMP */
    SmpDmMsgSend((smpDmMsg_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a slave in response to a DM_SEC_LTK_REQ_IND event
 *          to provide the key used for encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  keyFound  TRUE if key found.
 *  \param  secLevel  Security level of pairing when key was exchanged.
 *  \param  *pKey     Pointer to the key, if found.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecLtkRsp(dmConnId_t connId, bool_t keyFound, uint8_t secLevel, uint8_t *pKey)
{
  dmSecApiLtkRsp_t  *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmSecApiLtkRsp_t))) != NULL)
  {
    pMsg->hdr.event = DM_SEC_MSG_API_LTK_RSP;
    pMsg->hdr.param = connId;
    pMsg->keyFound = keyFound;
    pMsg->secLevel = secLevel;
    if (keyFound)
    {
      Calc128Cpy(pMsg->key, pKey);
    }

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}
