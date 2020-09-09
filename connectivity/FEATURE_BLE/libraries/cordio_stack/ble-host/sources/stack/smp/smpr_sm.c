/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP responder state machine.
 *
 *  Copyright (c) 2010-2019 Arm Ltd. All Rights Reserved.
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
#include "smp_api.h"
#include "smp_main.h"
#include "smpr_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Action function enumeration */
enum
{
  SMPR_ACT_NONE,                 /*!< No Action */
  SMPR_ACT_CLEANUP,              /*!< Process Pairing Cleanup */
  SMPR_ACT_PAIRING_FAILED,       /*!< Process Pairing Failed */
  SMPR_ACT_SEC_REQ_TO,           /*!< Process Response timeout on security request */
  SMPR_ACT_PAIRING_CANCEL,       /*!< Process Pairing Canceled */
  SMPR_ACT_STORE_PIN,            /*!< Process Store Pin */
  SMPR_ACT_PAIR_CNF_CALC_1,      /*!< Process Confirm Value Calculation 1 */
  SMPR_ACT_PAIR_CNF_CALC_2,      /*!< Process Confirm Value Calculation 2 */
  SMPR_ACT_SEND_PAIR_CNF,        /*!< Process Send Confirm Value */
  SMPR_ACT_PAIR_CNF_VER_CALC_1,  /*!< Process Received Confirm Value Verification Calculation 1 */
  SMPR_ACT_PAIR_CNF_VER_CALC_2,  /*!< Process Received Confirm Value Verification Calculation 2 */
  SMPR_ACT_MAX_ATTEMPTS,         /*!< Process Maximum Attempts */
  SMPR_ACT_ATTEMPT_RCVD,         /*!< Process Attempts Received */
  SMPR_ACT_CHECK_ATTEMPTS,       /*!< Process Check Attempts */
  SMPR_ACT_NOTIFY_DM_ATTEMPTS,   /*!< Process Notify DM of Attempts Failure */
  SMPR_ACT_NOTIFY_DM_RSP_TO,     /*!< Process Notify DM of Response Timeout Failure */
  SMPR_ACT_PAIRING_CMPL,         /*!< Process Pairing Complete */
  SMPR_ACT_SEND_SECURITY_REQ,    /*!< Process Send Slave Security Request */
  SMPR_ACT_PROC_PAIR_REQ,        /*!< Process Pairing Request */
  SMPR_ACT_SEND_PAIR_RSP,        /*!< Process Send Pairing Response */
  SMPR_ACT_PROC_PAIR_CNF,        /*!< Process Pair Confirm Value */
  SMPR_ACT_PROC_PAIR_CNF_CALC_1, /*!< Process Received Confirm Value */
  SMPR_ACT_CNF_VERIFY,           /*!< Process Recevied Confirm Value Verification */
  SMPR_ACT_SEND_PAIR_RANDOM,     /*!< Process Send Random Value */
  SMPR_ACT_SETUP_KEY_DIST,       /*!< Process Setup Key Distribution */
  SMPR_ACT_SEND_KEY,             /*!< Process Send Key */
  SMPR_ACT_RCV_KEY,              /*!< Process Received Key */
};


/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Static Variables
**************************************************************************************************/

/*! Action function table; order matches action function enumeration */
static const smpAct_t smprActionTbl[] =
{
  smpActNone,
  smpActCleanup,
  smpActPairingFailed,
  smpActSecReqTimeout,
  smpActPairingCancel,
  smpActStorePin,
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
  smprActSendSecurityReq,
  smprActProcPairReq,
  smprActSendPairRsp,
  smprActProcPairCnf,
  smprActProcPairCnfCalc1,
  smprActCnfVerify,
  smprActSendPairRandom,
  smprActSetupKeyDist,
  smprActSendKey,
  smprActRcvKey
};

/*! State table for common actions */
static const smpTblEntry_t smprStateTblCommon[SMP_STATE_TBL_COMMON_MAX] =
{
/* Event                        Next state                  Action */
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SM_ST_IDLE,            SMPR_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SM_ST_IDLE,            SMPR_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SM_ST_IDLE,            SMPR_ACT_PAIRING_CANCEL},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SM_ST_RSP_TO,          SMPR_ACT_PAIRING_FAILED},
  {0,                           0,                          0}
};

/*! State table for IDLE */
static const smpTblEntry_t smprStateTblIdle[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_SECURITY_REQ,    SMPR_SM_ST_API_PAIR_REQ,    SMPR_ACT_SEND_SECURITY_REQ},
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_CMD_PKT,             SMPR_SM_ST_API_PAIR_RSP,    SMPR_ACT_PROC_PAIR_REQ},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SM_ST_IDLE,            SMPR_ACT_NONE},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SM_ST_IDLE,            SMPR_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for API_PAIR_REQ */
static const smpTblEntry_t smprStateTblApiPairReq[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_CMD_PKT,             SMPR_SM_ST_API_PAIR_RSP,    SMPR_ACT_PROC_PAIR_REQ},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SM_ST_IDLE,            SMPR_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_DM_ENCRYPT_CMPL,     SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_DM_ENCRYPT_FAILED,   SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SM_ST_API_PAIR_REQ,    SMPR_ACT_SEC_REQ_TO},
  {SMP_MSG_INT_CLEANUP,         SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {0,                           0,                          0}
};

/*! State table for API_PAIR_RSP */
static const smpTblEntry_t smprStateTblApiPairRsp[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_PAIR_RSP,        SMPR_SM_ST_PIN_PAIR_1,      SMPR_ACT_SEND_PAIR_RSP},
  {0,                           0,                          0}
};

/*! State table for PIN_PAIR_1 */
static const smpTblEntry_t smprStateTblPinPair1[] =
{
/* Event                        Next state                  Action */
  {SMP_MSG_API_AUTH_RSP,        SMPR_SM_ST_PIN_PAIR_2,      SMPR_ACT_STORE_PIN},
  {SMP_MSG_CMD_PKT,             SMPR_SM_ST_PIN_PAIR_2,      SMPR_ACT_PROC_PAIR_CNF},
  {0,                           0,                          0}
};

/*! State table for PIN_PAIR_2 */
static const smpTblEntry_t smprStateTblPinPair2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPR_SM_ST_CNF_CALC_1,      SMPR_ACT_PAIR_CNF_CALC_1},
  {SMP_MSG_CMD_PKT,             SMPR_SM_ST_CNF_CALC_1,      SMPR_ACT_PROC_PAIR_CNF_CALC_1},
  {0,                           0,                          0}
};

/*! State table for CNF_CALC_1 */
static const smpTblEntry_t smprStateTblCnfCalc1[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SM_ST_CNF_CALC_2,      SMPR_ACT_PAIR_CNF_CALC_2},
  {0,                           0,                          0}
};

/*! State table for CNF_CALC_2 */
static const smpTblEntry_t smprStateTblCnfCalc2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SM_ST_PAIR_RAND,       SMPR_ACT_SEND_PAIR_CNF},
  {0,                           0,                          0}
};


/*! State table for PAIR_RAND */
static const smpTblEntry_t smprStateTblPairRand[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SM_ST_CNF_VER_CALC_1,  SMPR_ACT_PAIR_CNF_VER_CALC_1},
  {0,                           0,                          0}
};

/*! State table for CNF_VER_CALC_1 */
static const smpTblEntry_t smprStateTblCnfVerCalc1[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SM_ST_CNF_VER_CALC_2,  SMPR_ACT_PAIR_CNF_VER_CALC_2},
  {0,                           0,                          0}
};

/*! State table for CNF_VER_CALC_2 */
static const smpTblEntry_t smprStateTblCnfVerCalc2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SM_ST_STK_CALC,        SMPR_ACT_CNF_VERIFY},
  {0,                           0,                          0}
};

/*! State table for STK_CALC */
static const smpTblEntry_t smprStateTblStkCalc[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SM_ST_ENCRYPT,         SMPR_ACT_SEND_PAIR_RANDOM},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SM_ST_ATTEMPTS,        SMPR_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for ENCRYPT */
static const smpTblEntry_t smprStateTblEncrypt[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_DM_ENCRYPT_CMPL,     SMPR_SM_ST_KEY_DIST,        SMPR_ACT_SETUP_KEY_DIST},
  {SMP_MSG_DM_ENCRYPT_FAILED,   SMPR_SM_ST_IDLE,            SMPR_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SM_ST_ENCRYPT,         SMPR_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for KEY_DIST */
static const smpTblEntry_t smprStateTblKeyDist[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_INT_SEND_NEXT_KEY,   SMPR_SM_ST_KEY_DIST,        SMPR_ACT_SEND_KEY},
  {SMP_MSG_CMD_PKT,             SMPR_SM_ST_KEY_DIST,        SMPR_ACT_RCV_KEY},
  {SMP_MSG_INT_PAIRING_CMPL,    SMPR_SM_ST_IDLE,            SMPR_ACT_PAIRING_CMPL},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SM_ST_KEY_DIST,        SMPR_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for ATTEMPTS */
static const smpTblEntry_t smprStateTblAttempts[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_INT_WI_TIMEOUT,      SMPR_SM_ST_IDLE,            SMPR_ACT_CHECK_ATTEMPTS},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SM_ST_RSP_TO,          SMPR_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PKT,             SMPR_SM_ST_ATTEMPTS,        SMPR_ACT_ATTEMPT_RCVD},
  {SMP_MSG_API_SECURITY_REQ,    SMPR_SM_ST_IDLE,            SMPR_ACT_NOTIFY_DM_ATTEMPTS},
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SM_ST_ATTEMPTS,        SMPR_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for RSP_TO */
static const smpTblEntry_t smprStateTblRspTo[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SM_ST_IDLE,            SMPR_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SM_ST_RSP_TO,          SMPR_ACT_NONE},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SM_ST_RSP_TO,          SMPR_ACT_NONE},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SM_ST_RSP_TO,          SMPR_ACT_NONE},
  {SMP_MSG_API_SECURITY_REQ,    SMPR_SM_ST_RSP_TO,          SMPR_ACT_NOTIFY_DM_RSP_TO},
  {0,                           0,                          0}
};

/*! Table of individual state tables */
const smpTblEntry_t * const smprStateTbl[] =
{
  smprStateTblIdle,
  smprStateTblApiPairReq,
  smprStateTblApiPairRsp,
  smprStateTblPinPair1,
  smprStateTblPinPair2,
  smprStateTblCnfCalc1,
  smprStateTblCnfCalc2,
  smprStateTblPairRand,
  smprStateTblCnfVerCalc1,
  smprStateTblCnfVerCalc2,
  smprStateTblStkCalc,
  smprStateTblEncrypt,
  smprStateTblKeyDist,
  smprStateTblAttempts,
  smprStateTblRspTo
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! state machine interface */
const smpSmIf_t smprSmIf =
{
  smprStateTbl,
  smprActionTbl,
  smprStateTblCommon
};

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP responder role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmprInit(void)
{
  /* set up state machine interface */
  smpCb.pSlave = &smprSmIf;
  smpCb.procPairing = smpProcPairing;
  smpCb.procAuthReq = smpAuthReq;
}
