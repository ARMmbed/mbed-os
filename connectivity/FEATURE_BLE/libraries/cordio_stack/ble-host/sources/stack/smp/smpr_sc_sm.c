/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP Secure Connections initiator state machine.
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
#include "smp_sc_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Action function enumeration */
enum
{
  SMPR_SC_ACT_NONE,                 /*!< No Action */
  SMPR_SC_ACT_CLEANUP,              /*!< Process Pairing Cleanup */
  SMPR_SC_ACT_PAIRING_FAILED,       /*!< Process Pairing Failed */
  SMPR_SC_ACT_SEC_REQ_TO,           /*!< Process Response timeout on security request */
  SMPR_SC_ACT_PAIRING_CANCEL,       /*!< Process Pairing Canceled */
  SMPR_SC_ACT_SEND_PUB_KEY,         /*!< Process Send Public Key */
  SMPR_SC_ACT_STORE_LEGACY_PIN,     /*!< Process Store Legacy Pin */
  SMPR_SC_ACT_STORE_LESC_PIN,       /*!< Process Store LESC Pin */

  SMPR_SC_ACT_PAIR_CNF_CALC_1,      /*!< Process Confirm Value Calculation 1 */
  SMPR_SC_ACT_PAIR_CNF_CALC_2,      /*!< Process Confirm Value Calculation 2 */
  SMPR_SC_ACT_SEND_PAIR_CNF,        /*!< Process Send Confirm Value */
  SMPR_SC_ACT_PAIR_CNF_VER_CALC_1,  /*!< Process Received Confirm Value Verification Calculation 1 */
  SMPR_SC_ACT_PAIR_CNF_VER_CALC_2,  /*!< Process Received Confirm Value Verification Calculation 2 */
  SMPR_SC_ACT_MAX_ATTEMPTS,         /*!< Process Maximum Attempts */
  SMPR_SC_ACT_PAIRING_CMPL,         /*!< Process Pairing Complete */
  SMPR_SC_ACT_CHECK_ATTEMPTS,       /*!< Process Check Attempts */
  SMPR_SC_ACT_NOTIFY_DM_ATTEMPTS,   /*!< Process Notify DM of Attempts Failure */
  SMPR_SC_ACT_NOTIFY_DM_RSP_TO,     /*!< Process Notify DM of Response Timeout Failure */
  SMPR_SC_ACT_ATTEMPT_RCVD,         /*!< Process Attempts Received */

  SMPR_SC_ACT_SEND_SECURITY_REQ,    /*!< Process Send Slave Security Request */
  SMPR_SC_ACT_PROC_PAIR_REQ,        /*!< Process Pairing Request */
  SMPR_SC_ACT_SEND_PAIR_RSP,        /*!< Process Send Pairing Response */
  SMPR_SC_ACT_PROC_PAIR_CNF,        /*!< Process Received Confirm Value */

  SMPR_SC_ACT_JWNC_SETUP,           /*!< Process Just Works/Numeric Comparison Setup */
  SMPR_SC_ACT_JWNC_SEND_CNF,        /*!< Process JW/NC Send Confirm Value */
  SMPR_SC_ACT_JWNC_CALC_G2,         /*!< Process JW/NC Calculate G2 */
  SMPR_SC_ACT_JWNC_DISPLAY,         /*!< Process JW/NC Display Numeric Comparison */

  SMPR_SC_ACT_PK_SETUP,             /*!< Process Passkey Setup */
  SMPR_SC_ACT_PK_KEYPRESS,          /*!< Process Passkey Keypress */
  SMPR_SC_ACT_PK_SEND_KEYPRESS,     /*!< Process Passkey Send Keypress */
  SMPR_SC_ACT_PK_STORE_CNF,         /*!< Process Passkey Store Received Confirm Value */
  SMPR_SC_ACT_PK_STORE_CNF_CALC_CB, /*!< Process Passkey Store Confirm Value Calculation Cb */
  SMPR_SC_ACT_PK_STORE_PIN_CALC_CB, /*!< Process Passkey Store Pin Value Calculation Cb */
  SMPR_SC_ACT_PK_CALC_CB,           /*!< Process Passkey Calculate Cb */
  SMPR_SC_ACT_PK_SEND_CNF,          /*!< Process Passkey Send Confirm Value */
  SMPR_SC_ACT_PK_CALC_CA,           /*!< Process Passkey Calculate Ca */
  SMPR_SC_ACT_PK_SEND_RAND,         /*!< Process Passkey Send Random Value */

  SMPR_SC_ACT_OOB_SETUP,            /*!< Process OOB Setup */
  SMPR_SC_ACT_OOB_CALC_CA,          /*!< Process OOB Calculate Ca */
  SMPR_SC_ACT_OOB_SEND_RAND,        /*!< Process OOB Send Random Value */

  SMPR_SC_ACT_STORE_DH_CHECK,       /*!< Process Store DH Key Check */
  SMPR_SC_ACT_WAIT_DH_CHECK,        /*!< Process Wait DH Key Check */
  SMPR_SC_ACT_CALC_DHKEY,           /*!< Process Calculate DH Key Check */
  SMPR_SC_ACT_CALC_F5_TKEY,         /*!< Process Calculate F5 Temporary Key */
  SMPR_SC_ACT_CALC_F5_MACKEY,       /*!< Process Calculate F5 MAC Key */
  SMPR_SC_ACT_CALC_F5_LTK,          /*!< Process Calculate LTK */
  SMPR_SC_ACT_CALC_F6_EA,           /*!< Process Calculate Ea */
  SMPR_SC_ACT_CALC_F6_EB,           /*!< Process Calculate Eb */
  SMPR_SC_ACT_SEND_DH_CHECK,        /*!< Process Send DH Key Check */

  SMPR_SC_ACT_PROC_PAIR_CNF_CALC_1, /*!< Process Confirm Value Calcuation 1 */
  SMPR_SC_ACT_CNF_VERIFY,           /*!< Process Confirm Value Verification */
  SMPR_SC_ACT_SEND_PAIR_RANDOM,     /*!< Process Send Random Value */
  SMPR_SC_ACT_SETUP_KEY_DIST,       /*!< Processs Setup Key Distribution */
  SMPR_SC_ACT_RCV_KEY,              /*!< Process Received Key */
  SMPR_SC_ACT_SEND_KEY              /*!< Process Send Key */
};

/**************************************************************************************************
  Static Variables
**************************************************************************************************/

/*! Action function table; order matches action function enumeration */
static const smpAct_t smprScActionTbl[] =
{
  smpActNone,
  smpScActCleanup,
  smpScActPairingFailed,
  smpActSecReqTimeout,
  smpScActPairingCancel,
  smprScActSendPubKey,
  smpActStorePin,
  smprScActStoreLescPin,

  smpActPairCnfCalc1,
  smpActPairCnfCalc2,
  smpActSendPairCnf,
  smpActPairCnfVerCalc1,
  smpActPairCnfVerCalc2,
  smpActMaxAttempts,
  smpActPairingCmpl,
  smpActCheckAttempts,
  smpActNotifyDmAttemptsFailure,
  smpActNotifyDmRspToFailure,
  smpActAttemptRcvd,

  smprActSendSecurityReq,
  smprActProcPairReq,
  smprActSendPairRsp,
  smprActProcPairCnf,

  smprScActJwncSetup,
  smprScActJwncSendCnf,
  smprScActJwncCalcG2,
  smprScActJwncDisplay,

  smpScActPkSetup,
  smpScActPkKeypress,
  smpScActPkSendKeypress,
  smprScActPkStoreCnf,
  smprScActPkStoreCnfAndCalcCb,
  smprScActPkStorePinAndCalcCb,
  smprScActPkCalcCb,
  smprScActPkSendCnf,
  smprScActPkCalcCa,
  smprScActPkSendRand,

  smprScActOobSetup,
  smprScActOobCalcCa,
  smprScActOobSendRand,

  smprScActStoreDhCheck,
  smprScActWaitDhCheck,
  smprScActCalcDHKey,
  smpScActCalcF5TKey,
  smpScActCalcF5MacKey,
  smpScActCalcF5Ltk,
  smpScActDHKeyCalcF6Ea,
  smpScActDHKeyCalcF6Eb,
  smprScActDHKeyCheckSend,

  smprActProcPairCnfCalc1,
  smprActCnfVerify,
  smprActSendPairRandom,
  smprActSetupKeyDist,
  smprActRcvKey,
  smprActSendKey
};

/*! State table for common actions */
static const smpTblEntry_t smprScStateTblCommon[SMP_STATE_TBL_COMMON_MAX] =
{
/* Event                        Next state                                Action */
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_PAIRING_CANCEL},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SC_SM_ST_RSP_TO,                        SMPR_SC_ACT_PAIRING_FAILED},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_IDLE */
static const smpTblEntry_t smprScStateTblIdle[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_SECURITY_REQ,    SMPR_SC_SM_ST_API_PAIR_REQ,                  SMPR_SC_ACT_SEND_SECURITY_REQ},
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_API_PAIR_RSP,                  SMPR_SC_ACT_PROC_PAIR_REQ},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_NONE},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_NONE},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_API_PAIR_REQ */
static const smpTblEntry_t smprScStateTblApiPairReq[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_API_PAIR_RSP,                  SMPR_SC_ACT_PROC_PAIR_REQ},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_DM_ENCRYPT_CMPL,     SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_DM_ENCRYPT_FAILED,   SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SC_SM_ST_API_PAIR_REQ,                  SMPR_SC_ACT_SEC_REQ_TO},
  {SMP_MSG_INT_CLEANUP,         SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_API_PAIR_RSP */
static const smpTblEntry_t smprScStateTblApiPairRsp[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_PAIR_RSP,        SMPR_SC_SM_ST_MODE_SELECT,                   SMPR_SC_ACT_SEND_PAIR_RSP},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_MODE_SELECT */
static const smpTblEntry_t smprScStateTblModeSelect[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_INT_LESC,            SMPR_SC_SM_ST_LESC_PIN,                      SMPR_SC_ACT_NONE},
  {SMP_MSG_INT_LEGACY,          SMPR_SC_SM_ST_PIN_PAIR_1,                    SMPR_SC_ACT_NONE},
  {0,                           0,                                        0}
};

/*! State table for LESC SMPR_SC_SM_ST_LESC_PIN */
static const smpTblEntry_t smprScStateTblLescPin[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPR_SC_SM_ST_PUB_KEY,                       SMPR_SC_ACT_STORE_LESC_PIN},
  {0,                           0,                                        0}
};

/*! State table for LESC SMPR_SC_SM_ST_PUB_KEY */
static const smpTblEntry_t smprScStateTblPubKey[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_AUTH_SELECT,                   SMPR_SC_ACT_SEND_PUB_KEY},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_AUTH_SELECT */
static const smpTblEntry_t smprScStateTblAuthSelect[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_INT_JW_NC,           SMPR_SC_SM_ST_JWNC_SETUP,                    SMPR_SC_ACT_JWNC_SETUP},
  {SMP_MSG_INT_PASSKEY,         SMPR_SC_SM_ST_PK_KEYPRESS,                   SMPR_SC_ACT_PK_SETUP},
  {SMP_MSG_INT_OOB,             SMPR_SC_SM_ST_OOB_WAIT_RAND,                 SMPR_SC_ACT_OOB_SETUP},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_JWNC_SETUP */
static const smpTblEntry_t smprScStateTblJwNcSetup[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_JWNC_WAIT_RAND,                SMPR_SC_ACT_JWNC_SEND_CNF},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_JWNC_WAIT_RAND */
static const smpTblEntry_t smprScStateTblJwNcWaitRand[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_JWNC_CALC_G2,                  SMPR_SC_ACT_JWNC_CALC_G2},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_JWNC_CALC_G2 */
static const smpTblEntry_t smprScStateTblJwNcCalcG2[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_JWNC_WAIT_USER,                SMPR_SC_ACT_JWNC_DISPLAY},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_JWNC_WAIT_USER */
static const smpTblEntry_t smprScStateTblJwNcWaitUser[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_USER_CONFIRM,    SMPR_SC_SM_ST_WAIT_DH_CHECK,                 SMPR_SC_ACT_WAIT_DH_CHECK},
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_JWNC_WAIT_USER_DH_CHECK_RCVD,  SMPR_SC_ACT_STORE_DH_CHECK},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_JWNC_WAIT_USER_DH_CHECK_RCVD */
static const smpTblEntry_t smprScStateTblJwNcWaitUserDhCheckRcvd[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_USER_CONFIRM,    SMPR_SC_SM_ST_CALC_DHKEY,                    SMPR_SC_ACT_CALC_DHKEY},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_PK_KEYPRESS */
static const smpTblEntry_t smprScStateTblPassKeyKeypress[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_PK_KEYPRESS,                   SMPR_SC_ACT_PK_KEYPRESS},
  {SMP_MSG_EARLY_CNF,           SMPR_SC_SM_ST_PK_WAIT_AUTH,                  SMPR_SC_ACT_PK_STORE_CNF},
  {SMP_MSG_API_USER_KEYPRESS,   SMPR_SC_SM_ST_PK_KEYPRESS,                   SMPR_SC_ACT_PK_SEND_KEYPRESS},
  {SMP_MSG_API_AUTH_RSP,        SMPR_SC_SM_ST_PK_WAIT_CNF,                   SMPR_SC_ACT_STORE_LESC_PIN},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_PK_WAIT_AUTH */
static const smpTblEntry_t smprScStateTblPassWaitAuthRsp[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPR_SC_SM_ST_PK_CALC,                       SMPR_SC_ACT_PK_STORE_PIN_CALC_CB},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_PK_WAIT_CNF */
static const smpTblEntry_t smprScStateTblPasskeyWaitCnf[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_PK_CALC,                       SMPR_SC_ACT_PK_STORE_CNF_CALC_CB},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_PK_CALC */
static const smpTblEntry_t smprScStateTblPasskeyCalc[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_PK_RAND,                       SMPR_SC_ACT_PK_SEND_CNF},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_PK_RAND */
static const smpTblEntry_t smprScStateTblPasskeyRand[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_PK_CHECK,                      SMPR_SC_ACT_PK_CALC_CA},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_PK_CHECK */
static const smpTblEntry_t smprScStateTblPasskeyCheck[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_PK_REPEAT,                     SMPR_SC_ACT_PK_SEND_RAND},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_PK_REPEAT */
static const smpTblEntry_t smprScStateTblPasskeyRepeat[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_INT_PK_NEXT,         SMPR_SC_SM_ST_PK_WAIT_CNF,                   SMPR_SC_ACT_NONE},
  {SMP_MSG_INT_PK_CMPL,         SMPR_SC_SM_ST_WAIT_DH_CHECK,                 SMPR_SC_ACT_WAIT_DH_CHECK},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_OOB_WAIT_RAND */
static const smpTblEntry_t smprScStateTblOobWaitRand[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_OOB_SEND_RAND,                 SMPR_SC_ACT_OOB_CALC_CA},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_OOB_SEND_RAND */
static const smpTblEntry_t smprScStateTblOobSendRand[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_WAIT_DH_CHECK,                 SMPR_SC_ACT_OOB_SEND_RAND},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_WAIT_DH_CHECK */
static const smpTblEntry_t smprScStateTblWaitDhCheck[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_CALC_DHKEY,                    SMPR_SC_ACT_CALC_DHKEY},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_CALC_DHKEY */
static const smpTblEntry_t smprScStateTblCalcDHKey[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_ECC_CMPL,        SMPR_SC_SM_ST_CALC_F5_TKEY,                  SMPR_SC_ACT_CALC_F5_TKEY},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_CALC_F5_TKEY */
static const smpTblEntry_t smprScStateTblCalcF5TKey[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_CALC_F5_MACKEY,                SMPR_SC_ACT_CALC_F5_MACKEY},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_CALC_F5_MACKEY */
static const smpTblEntry_t smprScStateTblCalcF5MacKey[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_CALC_F5_LTK,                   SMPR_SC_ACT_CALC_F5_LTK},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_CALC_F5_LTK */
static const smpTblEntry_t smprScStateTblCalcF5LTK[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_CALC_F6_EA,                    SMPR_SC_ACT_CALC_F6_EA},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_CALC_F6_EA */
static const smpTblEntry_t smprScStateTblDhCalcF6Ea[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_CALC_F6_EB,                    SMPR_SC_ACT_CALC_F6_EB},
  {0,                           0,                                        0}
};

/*! State table for SMPR_SC_SM_ST_CALC_F6_EB */
static const smpTblEntry_t smprScStateTblDhCalcF6Eb[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPR_SC_SM_ST_ENCRYPT,                       SMPR_SC_ACT_SEND_DH_CHECK},
  {0,                           0,                                        0}
};

/*! State table for Legacy PIN_PAIR_1 */
static const smpTblEntry_t smprStateTblPinPair1[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPR_SC_SM_ST_PIN_PAIR_2,                    SMPR_SC_ACT_STORE_LEGACY_PIN},
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_PIN_PAIR_2,                    SMPR_SC_ACT_PROC_PAIR_CNF},
  {0,                           0,                                        0}
};

/*! State table for Legacy PIN_PAIR_2 */
static const smpTblEntry_t smprStateTblPinPair2[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPR_SC_SM_ST_CNF_CALC_1,                    SMPR_SC_ACT_PAIR_CNF_CALC_1},
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_CNF_CALC_1,                    SMPR_SC_ACT_PROC_PAIR_CNF_CALC_1},
  {0,                           0,                                        0}
};

/*! State table for Legacy CNF_CALC_1 */
static const smpTblEntry_t smprStateTblCnfCalc1[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SC_SM_ST_CNF_CALC_2,                    SMPR_SC_ACT_PAIR_CNF_CALC_2},
  {0,                           0,                                        0}
};

/*! State table for Legacy CNF_CALC_2 */
static const smpTblEntry_t smprStateTblCnfCalc2[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SC_SM_ST_PAIR_RAND,                     SMPR_SC_ACT_SEND_PAIR_CNF},
  {0,                           0,                                        0}
};


/*! State table for Legacy PAIR_RAND */
static const smpTblEntry_t smprStateTblPairRand[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_CNF_VER_CALC_1,                SMPR_SC_ACT_PAIR_CNF_VER_CALC_1},
  {0,                           0,                                        0}
};

/*! State table for Legacy CNF_VER_CALC_1 */
static const smpTblEntry_t smprStateTblCnfVerCalc1[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SC_SM_ST_CNF_VER_CALC_2,                SMPR_SC_ACT_PAIR_CNF_VER_CALC_2},
  {0,                           0,                                        0}
};

/*! State table for Legacy CNF_VER_CALC_2 */
static const smpTblEntry_t smprStateTblCnfVerCalc2[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SC_SM_ST_STK_CALC,                      SMPR_SC_ACT_CNF_VERIFY},
  {0,                           0,                                        0}
};

/*! State table for Legacy STK_CALC */
static const smpTblEntry_t smprScStateTblStkCalc[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPR_SC_SM_ST_ENCRYPT,                       SMPR_SC_ACT_SEND_PAIR_RANDOM},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                                        0}
};

/*! State table for ENCRYPT */
static const smpTblEntry_t smprScStateTblEncrypt[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_DM_ENCRYPT_CMPL,     SMPR_SC_SM_ST_KEY_DIST,                      SMPR_SC_ACT_SETUP_KEY_DIST},
  {SMP_MSG_DM_ENCRYPT_FAILED,   SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SC_SM_ST_ENCRYPT,                       SMPR_SC_ACT_NONE},
  {SMP_MSG_DH_CHECK_FAILURE,    SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_PAIRING_CANCEL},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                                        0}
};

/*! State table for KEY_DIST */
static const smpTblEntry_t smprScStateTblKeyDist[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_KEY_DIST,                      SMPR_SC_ACT_RCV_KEY},
  {SMP_MSG_INT_SEND_NEXT_KEY,   SMPR_SC_SM_ST_KEY_DIST,                      SMPR_SC_ACT_SEND_KEY},
  {SMP_MSG_INT_PAIRING_CMPL,    SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_PAIRING_CMPL},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SC_SM_ST_KEY_DIST,                      SMPR_SC_ACT_NONE},
  {0,                           0,                                        0}
};

/*! State table for ATTEMPTS */
static const smpTblEntry_t smprScStateTblAttempts[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_INT_WI_TIMEOUT,      SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CHECK_ATTEMPTS},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SC_SM_ST_RSP_TO,                        SMPR_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PKT,             SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_ATTEMPT_RCVD},
  {SMP_MSG_API_SECURITY_REQ,    SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_NOTIFY_DM_ATTEMPTS},
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SC_SM_ST_IDLE,                          SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SC_SM_ST_ATTEMPTS,                      SMPR_SC_ACT_NONE},
  {0,                           0,                                        0}
};

/*! State table for RSP_TO */
static const smpTblEntry_t smprScStateTblRspTo[] =
{
/* Event                        Next state                                Action  */
  {SMP_MSG_DM_CONN_CLOSE,       SMPR_SC_SM_ST_IDLE,                       SMPR_SC_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPR_SC_SM_ST_RSP_TO,                     SMPR_SC_ACT_NONE},
  {SMP_MSG_API_CANCEL_REQ,      SMPR_SC_SM_ST_RSP_TO,                     SMPR_SC_ACT_NONE},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPR_SC_SM_ST_RSP_TO,                     SMPR_SC_ACT_NONE},
  {SMP_MSG_API_SECURITY_REQ,    SMPR_SC_SM_ST_RSP_TO,                     SMPR_SC_ACT_NOTIFY_DM_RSP_TO},
  {0,                           0,                                        0}
};

/*! Table of individual state tables */
static const smpTblEntry_t * const smprScStateTbl[] =
{
  smprScStateTblIdle,
  smprScStateTblApiPairReq,
  smprScStateTblApiPairRsp,
  smprScStateTblModeSelect,
  smprScStateTblPubKey,
  smprScStateTblLescPin,
  smprScStateTblAuthSelect,

  smprScStateTblJwNcSetup,
  smprScStateTblJwNcWaitRand,
  smprScStateTblJwNcCalcG2,
  smprScStateTblJwNcWaitUser,
  smprScStateTblJwNcWaitUserDhCheckRcvd,

  smprScStateTblPassKeyKeypress,
  smprScStateTblPassWaitAuthRsp,
  smprScStateTblPasskeyWaitCnf,
  smprScStateTblPasskeyCalc,
  smprScStateTblPasskeyRand,
  smprScStateTblPasskeyCheck,
  smprScStateTblPasskeyRepeat,

  smprScStateTblOobSendRand,
  smprScStateTblOobWaitRand,

  smprScStateTblWaitDhCheck,
  smprScStateTblCalcDHKey,
  smprScStateTblCalcF5TKey,
  smprScStateTblCalcF5MacKey,
  smprScStateTblCalcF5LTK,
  smprScStateTblDhCalcF6Ea,
  smprScStateTblDhCalcF6Eb,

  smprStateTblPinPair1,
  smprStateTblPinPair2,
  smprStateTblCnfCalc1,
  smprStateTblCnfCalc2,
  smprStateTblPairRand,
  smprStateTblCnfVerCalc1,
  smprStateTblCnfVerCalc2,

  smprScStateTblStkCalc,
  smprScStateTblEncrypt,
  smprScStateTblKeyDist,
  smprScStateTblAttempts,
  smprScStateTblRspTo
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! state machine interface */
const smpSmIf_t smprScSmIf =
{
  smprScStateTbl,
  smprScActionTbl,
  smprScStateTblCommon
};

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP initiator role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmprScInit(void)
{
  /* set up callback interface */
  smpCb.pSlave = &smprScSmIf;

  /* General SMP LESC Initialization */
  SmpScInit();
}

/*************************************************************************************************/
/*!
 *  \brief  Convert state into string for diagnostics.
 *
 *  \param  state    State ID
 *
 *  \return State string.
 */
/*************************************************************************************************/
uint8_t *smprStateStr(uint8_t state)
{
  switch(state)
  {
    case SMPR_SC_SM_ST_IDLE: return (uint8_t*) "R_IDLE";
    case SMPR_SC_SM_ST_API_PAIR_REQ: return (uint8_t*) "R_API_PAIR_REQ";
    case SMPR_SC_SM_ST_API_PAIR_RSP: return (uint8_t*) "R_API_PAIR_RSP";
    case SMPR_SC_SM_ST_MODE_SELECT: return (uint8_t*) "R_MODE_SELECT";
    case SMPR_SC_SM_ST_PUB_KEY: return (uint8_t*) "R_PUB_KEY";
    case SMPR_SC_SM_ST_LESC_PIN: return (uint8_t*) "R_LESC_PIN";
    case SMPR_SC_SM_ST_AUTH_SELECT: return (uint8_t*) "R_AUTH_SELECT";
    case SMPR_SC_SM_ST_JWNC_SETUP: return (uint8_t*) "R_JWNC_SETUP";
    case SMPR_SC_SM_ST_JWNC_WAIT_RAND: return (uint8_t*) "R_JWNC_WAIT_RAND";
    case SMPR_SC_SM_ST_JWNC_CALC_G2: return (uint8_t*) "R_JWNC_CALC_G2";
    case SMPR_SC_SM_ST_JWNC_WAIT_USER: return (uint8_t*) "R_JWNC_WAIT_USER";
    case SMPR_SC_SM_ST_JWNC_WAIT_USER_DH_CHECK_RCVD: return (uint8_t*) "R_JWNC_WAIT_USER_DH";

    case SMPR_SC_SM_ST_PK_KEYPRESS: return (uint8_t*) "R_PK_KEYPRESS";
    case SMPR_SC_SM_ST_PK_WAIT_AUTH: return (uint8_t*) "R_PK_WAIT_AUTH";
    case SMPR_SC_SM_ST_PK_WAIT_CNF: return (uint8_t*) "R_PK_WAIT_CNF";
    case SMPR_SC_SM_ST_PK_CALC: return (uint8_t*) "R_PK_CALC";
    case SMPR_SC_SM_ST_PK_RAND: return (uint8_t*) "R_PK_RAND";
    case SMPR_SC_SM_ST_PK_CHECK: return (uint8_t*) "R_PK_CHECK";
    case SMPR_SC_SM_ST_PK_REPEAT: return (uint8_t*) "R_PK_REPEAT";

    case SMPR_SC_SM_ST_OOB_SEND_RAND: return (uint8_t*) "R_OOB_SEND_RAND";
    case SMPR_SC_SM_ST_OOB_WAIT_RAND: return (uint8_t*) "R_OOB_WAIT_RAND";

    case SMPR_SC_SM_ST_WAIT_DH_CHECK: return (uint8_t*) "R_WAIT_DH_CHECK";
    case SMPR_SC_SM_ST_CALC_DHKEY: return (uint8_t*) "R_CALC_DHKEY";
    case SMPR_SC_SM_ST_CALC_F5_TKEY: return (uint8_t*) "R_CALC_F5_TKEY";
    case SMPR_SC_SM_ST_CALC_F5_MACKEY: return (uint8_t*) "R_CALC_F5_MACKEY";
    case SMPR_SC_SM_ST_CALC_F5_LTK: return (uint8_t*) "R_CALC_F5_LTK";
    case SMPR_SC_SM_ST_CALC_F6_EA: return (uint8_t*) "R_CALC_F6_EA";
    case SMPR_SC_SM_ST_CALC_F6_EB: return (uint8_t*) "R_CALC_F6_EB";
    case SMPR_SC_SM_ST_PIN_PAIR_1: return (uint8_t*) "R_PIN_PAIR_1";
    case SMPR_SC_SM_ST_PIN_PAIR_2: return (uint8_t*) "R_PIN_PAIR_2";
    case SMPR_SC_SM_ST_CNF_CALC_1: return (uint8_t*) "R_CNF_CALC_1";
    case SMPR_SC_SM_ST_CNF_CALC_2: return (uint8_t*) "R_CNF_CALC_2";
    case SMPR_SC_SM_ST_PAIR_RAND: return (uint8_t*) "R_PAIR_RAND";
    case SMPR_SC_SM_ST_CNF_VER_CALC_1: return (uint8_t*) "R_CNF_VER_CALC_1";
    case SMPR_SC_SM_ST_CNF_VER_CALC_2: return (uint8_t*) "R_CNF_VER_CALC_2";
    case SMPR_SC_SM_ST_STK_CALC: return (uint8_t*) "R_STK_CALC";
    case SMPR_SC_SM_ST_ENCRYPT: return (uint8_t*) "R_ENCRYPT";
    case SMPR_SC_SM_ST_KEY_DIST: return (uint8_t*) "R_KEY_DIST";
    case SMPR_SC_SM_ST_ATTEMPTS: return (uint8_t*) "R_ATTEMPTS";
    case SMPR_SC_SM_ST_RSP_TO: return (uint8_t*) "R_RSP_TO";

    default: return (uint8_t*) "R_Unknown";
  }
}

