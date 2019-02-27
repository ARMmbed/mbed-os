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
 *  \brief SMP initiator state machine.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "smp_api.h"
#include "smp_main.h"
#include "smpi_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Action function enumeration */
enum
{
  SMPI_ACT_NONE,                /*!< No Action */
  SMPI_ACT_CLEANUP,             /*!< Process Pairing Cleanup  */
  SMPI_ACT_PAIRING_FAILED,      /*!< Process Pairing Failed */
  SMPI_ACT_PAIRING_CANCEL,      /*!< Process Pairing Canceled */
  SMPI_ACT_PAIR_CNF_CALC_1,     /*!< Process Confirm Value Calculation 1 */
  SMPI_ACT_PAIR_CNF_CALC_2,     /*!< Process Confirm Value Calculation 2 */
  SMPI_ACT_SEND_PAIR_CNF,       /*!< Process Send Confirm Value */
  SMPI_ACT_PAIR_CNF_VER_CALC_1, /*!< Process Received Confirm Value Verification Calculation 1 */
  SMPI_ACT_PAIR_CNF_VER_CALC_2, /*!< Process Received Confirm Value Verification Calculation 2 */
  SMPI_ACT_MAX_ATTEMPTS,        /*!< Process Maximum Attempts */
  SMPI_ACT_ATTEMPT_RCVD,        /*!< Process Attempts Received */
  SMPI_ACT_CHECK_ATTEMPTS,      /*!< Process Check Attempts */
  SMPI_ACT_NOTIFY_DM_ATTEMPTS,  /*!< Process Notify DM of Attempts Failure */
  SMPI_ACT_NOTIFY_DM_RSP_TO,    /*!< Process Notify DM of Response Timeout Failure */
  SMPI_ACT_PAIRING_CMPL,        /*!< Process Pairing Complete */
  SMPI_ACT_PAIR_REQ,            /*!< Process Send Pairing Request */
  SMPI_ACT_CHECK_SECURITY_REQ,  /*!< Process Check Security Request */
  SMPI_ACT_PROC_SECURITY_REQ,   /*!< Process Security Request */
  SMPI_ACT_PROC_PAIR_RSP,       /*!< Process Pairing Response */
  SMPI_ACT_PROC_PAIR_CNF,       /*!< Process Pairing Confirmation */
  SMPI_ACT_CNF_VERIFY,          /*!< Process Verify Received Confirm Value */
  SMPI_ACT_STK_ENCRYPT,         /*!< Process STK Encryption */
  SMPI_ACT_SETUP_KEY_DIST,      /*!< Process Setup Key Distribution */
  SMPI_ACT_RCV_KEY,             /*!< Process Received Key */
  SMPI_ACT_SEND_KEY             /*!< Process Send Key */
};

/**************************************************************************************************
  Static Variables
**************************************************************************************************/

/*! Action function table; order matches action function enumeration */
static const smpAct_t smpiActionTbl[] =
{
  smpActNone,
  smpActCleanup,
  smpActPairingFailed,
  smpActPairingCancel,
  smpActPairCnfCalc1,
  smpActPairCnfCalc2,
  smpActSendPairCnf,
  smpActPairCnfVerCalc1,
  smpActPairCnfVerCalc2,
  smpActMaxAttempts,
  smpActAttemptRcvd,
  smpActCheckAttempts,
  smpActNotifyDmAttemptsFailure,
  smpActNotifyDmRspToFailure,
  smpActPairingCmpl,
  smpiActPairReq,
  smpiActCheckSecurityReq,
  smpiActProcSecurityReq,
  smpiActProcPairRsp,
  smpiActProcPairCnf,
  smpiActCnfVerify,
  smpiActStkEncrypt,
  smpiActSetupKeyDist,
  smpiActRcvKey,
  smpiActSendKey
};

/*! State table for common actions */
static const smpTblEntry_t smpiStateTblCommon[SMP_STATE_TBL_COMMON_MAX] =
{
/* Event                        Next state                  Action */
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SM_ST_IDLE,            SMPI_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SM_ST_IDLE,            SMPI_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SM_ST_IDLE,            SMPI_ACT_PAIRING_CANCEL},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SM_ST_RSP_TO,          SMPI_ACT_PAIRING_FAILED},
  {0,                           0,                          0}
};


/*! State table for IDLE */
static const smpTblEntry_t smpiStateTblIdle[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_PAIR_REQ,        SMPI_SM_ST_PAIR_RSP,        SMPI_ACT_PAIR_REQ},
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SM_ST_IDLE,            SMPI_ACT_CLEANUP},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SM_ST_IDLE,            SMPI_ACT_CHECK_SECURITY_REQ},
  {SMP_MSG_CMD_PKT,             SMPI_SM_ST_IDLE,            SMPI_ACT_PROC_SECURITY_REQ},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SM_ST_IDLE,            SMPI_ACT_NONE},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SM_ST_IDLE,            SMPI_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for PAIR_RSP */
static const smpTblEntry_t smpiStateTblPairRsp[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SM_ST_PIN,             SMPI_ACT_PROC_PAIR_RSP},
  {0,                           0,                          0}
};

/*! State table for PIN */
static const smpTblEntry_t smpiStateTblPin[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPI_SM_ST_CNF_CALC_1,      SMPI_ACT_PAIR_CNF_CALC_1},
  {0,                           0,                          0}
};

/*! State table for CNF_CALC_1 */
static const smpTblEntry_t smpiStateTblCnfCalc1[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SM_ST_CNF_CALC_2,      SMPI_ACT_PAIR_CNF_CALC_2},
  {0,                           0,                          0}
};

/*! State table for CNF_CALC_2 */
static const smpTblEntry_t smpiStateTblCnfCalc2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SM_ST_PAIR_CNF,        SMPI_ACT_SEND_PAIR_CNF},
  {0,                           0,                          0}
};

/*! State table for PAIR_CNF */
static const smpTblEntry_t smpiStateTblPairCnf[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SM_ST_PAIR_RAND,       SMPI_ACT_PROC_PAIR_CNF},
  {0,                           0,                          0}
};

/*! State table for PAIR_RAND */
static const smpTblEntry_t smpiStateTblPairRand[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SM_ST_CNF_VER_CALC_1,  SMPI_ACT_PAIR_CNF_VER_CALC_1},
  {0,                           0,                          0}
};

/*! State table for CNF_VER_CALC_1 */
static const smpTblEntry_t smpiStateTblCnfVerCalc1[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SM_ST_CNF_VER_CALC_2,  SMPI_ACT_PAIR_CNF_VER_CALC_2},
  {0,                           0,                          0}
};

/*! State table for CNF_VER_CALC_2 */
static const smpTblEntry_t smpiStateTblCnfVerCalc2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SM_ST_STK_CALC,        SMPI_ACT_CNF_VERIFY},
  {0,                           0,                          0}
};

/*! State table for STK_CALC */
static const smpTblEntry_t smpiStateTblStkCalc[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SM_ST_ENCRYPT,         SMPI_ACT_STK_ENCRYPT},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SM_ST_ATTEMPTS,        SMPI_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for ENCRYPT */
static const smpTblEntry_t smpiStateTblEncrypt[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_DM_ENCRYPT_CMPL,     SMPI_SM_ST_KEY_DIST,        SMPI_ACT_SETUP_KEY_DIST},
  {SMP_MSG_DM_ENCRYPT_FAILED,   SMPI_SM_ST_IDLE,            SMPI_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SM_ST_ENCRYPT,         SMPI_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for KEY_DIST */
static const smpTblEntry_t smpiStateTblKeyDist[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SM_ST_KEY_DIST,        SMPI_ACT_RCV_KEY},
  {SMP_MSG_INT_SEND_NEXT_KEY,   SMPI_SM_ST_KEY_DIST,        SMPI_ACT_SEND_KEY},
  {SMP_MSG_INT_PAIRING_CMPL,    SMPI_SM_ST_IDLE,            SMPI_ACT_PAIRING_CMPL},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SM_ST_KEY_DIST,        SMPI_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for ATTEMPTS */
static const smpTblEntry_t smpiStateTblAttempts[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_INT_WI_TIMEOUT,      SMPI_SM_ST_IDLE,            SMPI_ACT_CHECK_ATTEMPTS},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SM_ST_RSP_TO,          SMPI_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PKT,             SMPI_SM_ST_ATTEMPTS,        SMPI_ACT_ATTEMPT_RCVD},
  {SMP_MSG_API_PAIR_REQ,        SMPI_SM_ST_PAIR_RSP,        SMPI_ACT_NOTIFY_DM_ATTEMPTS},
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SM_ST_IDLE,            SMPI_ACT_CLEANUP},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SM_ST_IDLE,            SMPI_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SM_ST_ATTEMPTS,        SMPI_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for RSP_TO */
static const smpTblEntry_t smpiStateTblRspTo[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SM_ST_IDLE,            SMPI_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SM_ST_RSP_TO,          SMPI_ACT_NONE},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SM_ST_RSP_TO,          SMPI_ACT_NONE},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SM_ST_RSP_TO,          SMPI_ACT_NONE},
  {SMP_MSG_API_PAIR_REQ,        SMPI_SM_ST_RSP_TO,          SMPI_ACT_NOTIFY_DM_RSP_TO},
  {0,                           0,                          0}
};

/*! Table of individual state tables */
static const smpTblEntry_t * const smpiStateTbl[] =
{
  smpiStateTblIdle,
  smpiStateTblPairRsp,
  smpiStateTblPin,
  smpiStateTblCnfCalc1,
  smpiStateTblCnfCalc2,
  smpiStateTblPairCnf,
  smpiStateTblPairRand,
  smpiStateTblCnfVerCalc1,
  smpiStateTblCnfVerCalc2,
  smpiStateTblStkCalc,
  smpiStateTblEncrypt,
  smpiStateTblKeyDist,
  smpiStateTblAttempts,
  smpiStateTblRspTo
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! state machine interface */
const smpSmIf_t smpiSmIf =
{
  smpiStateTbl,
  smpiActionTbl,
  smpiStateTblCommon
};

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP initiator role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpiInit(void)
{
  /* set up callback interface */
  smpCb.pMaster = &smpiSmIf;
  smpCb.procPairing = smpProcPairing;
  smpCb.procAuthReq = smpAuthReq;
}

