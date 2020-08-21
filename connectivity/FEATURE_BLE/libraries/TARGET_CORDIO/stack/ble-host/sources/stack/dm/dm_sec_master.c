/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM security module for master.
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
#include "util/calc128.h"
#include "smp_api.h"
#include "dm_api.h"
#include "dm_sec.h"
#include "dm_conn.h"
#include "dm_main.h"

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  This function is called by SMP to request encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  secLevel  Security level of pairing when key was exchanged.
 *  \param  pKey      Pointer to key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSmpEncryptReq(dmConnId_t connId, uint8_t secLevel, uint8_t *pKey)
{
  dmConnCcb_t *pCcb;

  if ((pCcb = dmConnCcbById(connId)) != NULL)
  {
    /* store security level */
    pCcb->tmpSecLevel = secLevel;

    /* not using LTK */
    pCcb->usingLtk = FALSE;

    /* start encryption; note EDIV and RAND are zero */
    HciLeStartEncryptionCmd(pCcb->handle, (uint8_t *) calc128Zeros, 0, pKey);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by a master device to initiate pairing.
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
void DmSecPairReq(dmConnId_t connId, uint8_t oob, uint8_t auth, uint8_t iKeyDist, uint8_t rKeyDist)
{
  smpDmPair_t  *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(smpDmPair_t))) != NULL)
  {
    pMsg->hdr.event = SMP_MSG_API_PAIR_REQ;
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
 *  \brief  This function is called by a master device to initiate link encryption.
 *
 *  \param  connId    DM connection ID.
 *  \param  secLevel  Security level of pairing when key was exchanged.
 *  \param  pLtk      Pointer to LTK parameter structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecEncryptReq(dmConnId_t connId, uint8_t secLevel, dmSecLtk_t *pLtk)
{
  dmSecApiEncryptReq_t  *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmSecApiEncryptReq_t))) != NULL)
  {
    pMsg->hdr.event = DM_SEC_MSG_API_ENCRYPT_REQ;
    pMsg->hdr.param = connId;
    memcpy(&pMsg->ltk, pLtk, sizeof(dmSecLtk_t));
    pMsg->secLevel = secLevel;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

