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
 *  \brief SMP responder main module.
 */
/*************************************************************************************************/
#ifndef SMPR_MAIN_H
#define SMPR_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* state machine interface */
extern const smpSmIf_t smprSmIf;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void smprActSendSecurityReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActProcPairReq(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSendPairRsp(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActProcPairCnf(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActProcPairCnfCalc1(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActCnfVerify(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSendPairRandom(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSetupKeyDist(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActSendKey(smpCcb_t *pCcb, smpMsg_t *pMsg);
void smprActRcvKey(smpCcb_t *pCcb, smpMsg_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* SMPR_MAIN_H */
