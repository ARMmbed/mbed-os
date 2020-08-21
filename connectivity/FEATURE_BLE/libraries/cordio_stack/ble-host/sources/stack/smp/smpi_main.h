/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP initiator main module.
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
#ifndef SMPI_MAIN_H
#define SMPI_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* state machine interface */
extern const smpSmIf_t smpiSmIf;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* state machine action functions */
void smpiActPairReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActCheckSecurityReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActProcSecurityReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActProcPairRsp(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActProcPairCnf(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActCnfVerify(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActStkEncrypt(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActSetupKeyDist(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActRcvKey(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smpiActSendKey(smpCcb_t *pCcb, smpMsg_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* SMPI_MAIN_H */
