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
 *  \brief SMP Secure Connections initiator state machine.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "smp_api.h"
#include "smp_main.h"
#include "smpi_main.h"
#include "smp_sc_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Action function enumeration */
enum
{
  SMPI_SC_ACT_NONE,                 /*!< No Action */
  SMPI_SC_ACT_CLEANUP,              /*!< Process Pairing Cleanup */
  SMPI_SC_ACT_PAIRING_FAILED,       /*!< Process Pairing Failed */
  SMPI_SC_ACT_PAIRING_CANCEL,       /*!< Process Pairing Canceled */
  SMPI_SC_ACT_PAIR_CNF_CALC_1,      /*!< Process Confirm Value Calculation 1 */
  SMPI_SC_ACT_PAIR_CNF_CALC_2,      /*!< Process Confirm Value Calculation 2 */
  SMPI_SC_ACT_SEND_PAIR_CNF,        /*!< Process Send Confirm Value */
  SMPI_SC_ACT_PAIR_CNF_VER_CALC_1,  /*!< Process Received Confirm Value Verification Calculation 1 */
  SMPI_SC_ACT_PAIR_CNF_VER_CALC_2,  /*!< Process Received Confirm Value Verification Calculation 2 */
  SMPI_SC_ACT_MAX_ATTEMPTS,         /*!< Process Maximum Attempts */
  SMPI_SC_ACT_ATTEMPT_RCVD,         /*!< Process Attempts Received */
  SMPI_SC_ACT_CHECK_ATTEMPTS,       /*!< Process Check Attempts */
  SMPI_SC_ACT_NOTIFY_DM_ATTEMPTS,   /*!< Process Notify DM of Attempts Failure */
  SMPI_SC_ACT_NOTIFY_DM_RSP_TO,     /*!< Process Notify DM of Response Timeout Failure */
  SMPI_SC_ACT_PAIRING_CMPL,         /*!< Process Pairing Complete */
  SMPI_SC_ACT_PAIR_REQ,             /*!< Process Send Pairing Request */
  SMPI_SC_ACT_CHECK_SECURITY_REQ,   /*!< Process Check Slave Security Request */
  SMPI_SC_ACT_PROC_SECURITY_REQ,    /*!< Process Slave Security Request */
  SMPI_SC_ACT_PROC_PAIR_RSP,        /*!< Process Pairing Response */
  SMPI_SC_ACT_PROC_PAIR_CNF,        /*!< Process Pairing Confirmation */
  SMPI_SC_ACT_CNF_VERIFY,           /*!< Process Verify Received Confirm Value */
  SMPI_SC_ACT_STK_ENCRYPT,          /*!< Process STK Encryption */
  SMPI_SC_ACT_SETUP_KEY_DIST,       /*!< Process Setup Key Distribution */
  SMPI_SC_ACT_RCV_KEY,              /*!< Process Received Key */
  SMPI_SC_ACT_SEND_KEY,             /*!< Process Send Key */

  SMPI_SC_ACT_SEND_PUB_KEY,         /*!< Process Send Public Key */
  SMPI_SC_ACT_SC_AUTH_SELECT,       /*!< Process Select Authentication Method */

  SMPI_SC_ACT_JWNC_SETUP,           /*!< Process Just Works/Numeric Comparison Setup */
  SMPI_SC_ACT_JWNC_SEND_RAND,       /*!< Process JW/NC Send Random Value */
  SMPI_SC_ACT_JWNC_CALC_F4,         /*!< Process JW/NC Calculate F4 */
  SMPI_SC_ACT_JWNC_CALC_G2,         /*!< Process JW/NC Calculate G2 */
  SMPI_SC_ACT_JWNC_DISPLAY,         /*!< Process Display Numeric Comparison */

  SMPI_SC_ACT_PK_SETUP,             /*!< Process Passkey Setup */
  SMPI_SC_ACT_PK_KEYPRESS,          /*!< Process Passkey Keypress */
  SMPI_SC_ACT_PK_SEND_KEYPRESS,     /*!< Process Passkey Send Keypress */
  SMPI_SC_ACT_PK_CALC_CA,           /*!< Process Passkey Calcuate Ca */
  SMPI_SC_ACT_PK_CALC_CB,           /*!< Process Passkey Calculate Cb */
  SMPI_SC_ACT_PK_SEND_CNF,          /*!< Process Passkey Send Confirm Value */
  SMPI_SC_ACT_PK_SEND_RAND,         /*!< Process Passkey Send Random Value */
  SMPI_SC_ACT_PK_CHECK,             /*!< Process Passkey Check Passkey Complete */

  SMPI_SC_ACT_OOB_CALC_CB,          /*!< Process OOB Calculate Cb */
  SMPI_SC_ACT_OOB_SEND_RAND,        /*!< Process OOB Send Random Value */
  SMPI_SC_ACT_OOB_PROC_RAND,        /*!< Process OOB Process Received Random Value */

  SMPI_SC_ACT_CALC_DHKEY,           /*!< Process DH Key Calculation */
  SMPI_SC_ACT_CALC_F5_TKEY,         /*!< Process Calculate F5 Temporary Key */
  SMPI_SC_ACT_CALC_F5_MACKEY,       /*!< Process Calculate F5 MAC Key */
  SMPI_SC_ACT_CALC_F5_LTK,          /*!< Process Calculate F5 LTK */
  SMPI_SC_ACT_CALC_F6_EA,           /*!< Process Calculate F6 Ea */
  SMPI_SC_ACT_CALC_F6_EB,           /*!< Process Calculate F6 Eb */
  SMPI_SC_ACT_SEND_DH_CHECK,        /*!< Process Send DH Key Check */
  SMPI_SC_ACT_VERIFY_DH_CHECK,      /*!< Process Verify Received DH Key Check */
};

/**************************************************************************************************
  Static Variables
**************************************************************************************************/

/*! Action function table; order matches action function enumeration */
static const smpAct_t smpiScActionTbl[] =
{
  smpActNone,
  smpScActCleanup,
  smpScActPairingFailed,
  smpScActPairingCancel,
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
  smpiActSendKey,

  smpiScActSendPubKey,
  smpiScActAuthSelect,

  smpiScActJwncSetup,
  smpiScActJwncSendRand,
  smpiScActJwncCalcF4,
  smpiScActJwncCalcG2,
  smpScActJwncDisplay,

  smpScActPkSetup,
  smpScActPkKeypress,
  smpScActPkSendKeypress,
  smpiScActPkCalcCa,
  smpiScActPkCalcCb,
  smpiScActPkSendCnf,
  smpiScActPkSendRand,
  smpiScActPkCheck,

  smpiScActOobCalcCb,
  smpiScActOobSendRand,
  smpiScActOobProcRand,

  smpScActCalcSharedSecret,
  smpScActCalcF5TKey,
  smpScActCalcF5MacKey,
  smpScActCalcF5Ltk,
  smpScActDHKeyCalcF6Ea,
  smpScActDHKeyCalcF6Eb,
  smpiScActDHKeyCheckSend,
  smpiScActDHKeyCheckVerify,
};

/*! State table for common actions */
static const smpTblEntry_t smpiScStateTblCommon[SMP_STATE_TBL_COMMON_MAX] =
{
/* Event                        Next state                  Action */
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_PAIRING_CANCEL},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SC_SM_ST_RSP_TO,          SMPI_SC_ACT_PAIRING_FAILED},
  {0,                           0,                          0}
};


/*! State table for IDLE */
static const smpTblEntry_t smpiScStateTblIdle[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_PAIR_REQ,        SMPI_SC_SM_ST_PAIR_RSP,        SMPI_SC_ACT_PAIR_REQ},
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_CLEANUP},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_CHECK_SECURITY_REQ},
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_PROC_SECURITY_REQ},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_NONE},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for PAIR_RSP */
static const smpTblEntry_t smpiScStateTblPairRsp[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_MODE_SELECT,     SMPI_SC_ACT_PROC_PAIR_RSP},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_MODE_SELECT */
static const smpTblEntry_t smpiScStateTblModeSelect[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_INT_LESC,            SMPI_SC_SM_ST_LESC_PIN,        SMPI_SC_ACT_NONE},
  {SMP_MSG_INT_LEGACY,          SMPI_SC_SM_ST_LEGACY_PIN,      SMPI_SC_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_LESC_PIN */
static const smpTblEntry_t smpiScStateTblLescPin[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPI_SC_SM_ST_PUB_KEY,         SMPI_SC_ACT_SEND_PUB_KEY},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_PUB_KEY */
static const smpTblEntry_t smpiScStateTblPubKey[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_AUTH_SELECT,     SMPI_SC_ACT_SC_AUTH_SELECT},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_AUTH_SELECT */
static const smpTblEntry_t smpiScStateTblAuthSelect[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_INT_JW_NC,           SMPI_SC_SM_ST_JWNC_WAIT_CNF,   SMPI_SC_ACT_JWNC_SETUP},
  {SMP_MSG_INT_PASSKEY,         SMPI_SC_SM_ST_PK_KEYPRESS,     SMPI_SC_ACT_PK_SETUP},
  {SMP_MSG_INT_OOB,             SMPI_SC_SM_ST_OOB_SEND_RAND,   SMPI_SC_ACT_OOB_CALC_CB},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_JWNC_WAIT_CNF */
static const smpTblEntry_t smpiScStateTblJwNcWaitCnf[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_JWNC_RAND,       SMPI_SC_ACT_JWNC_SEND_RAND},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_JWNC_RAND */
static const smpTblEntry_t smpiScStateTblJwNcRand[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_JWNC_CHECK_1,    SMPI_SC_ACT_JWNC_CALC_F4},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_JWNC_CHECK_1 */
static const smpTblEntry_t smpiScStateTblJwNcCheck1[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_JWNC_CHECK_2,    SMPI_SC_ACT_JWNC_CALC_G2},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_JWNC_CHECK_2 */
static const smpTblEntry_t smpiScStateTblJwNcCheck2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_JWNC_WAIT_USER,  SMPI_SC_ACT_JWNC_DISPLAY},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_JWNC_WAIT_USER */
static const smpTblEntry_t smpiScStateTblJwNcWaitUser[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_USER_CONFIRM,    SMPI_SC_SM_ST_CALC_DHKEY,      SMPI_SC_ACT_CALC_DHKEY},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_PK_KEYPRESS */
static const smpTblEntry_t smprScStateTblPasskeyKeypress[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_PK_KEYPRESS,     SMPI_SC_ACT_PK_KEYPRESS},
  {SMP_MSG_API_USER_KEYPRESS,   SMPI_SC_SM_ST_PK_KEYPRESS,     SMPI_SC_ACT_PK_SEND_KEYPRESS},
  {SMP_MSG_API_AUTH_RSP,        SMPI_SC_SM_ST_PK_CALC,         SMPI_SC_ACT_PK_CALC_CA},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_PK_CALC */
static const smpTblEntry_t smpiScStateTblPasskeyCalc[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_PK_CNF,          SMPI_SC_ACT_PK_SEND_CNF},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_PK_CNF */
static const smpTblEntry_t smpiScStateTblPasskeyCnf[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_PK_RAND,         SMPI_SC_ACT_PK_SEND_RAND},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_PK_RAND */
static const smpTblEntry_t smpiScStateTblPasskeyRand[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_PK_CHECK,        SMPI_SC_ACT_PK_CALC_CB},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_PK_CHECK */
static const smpTblEntry_t smpiScStateTblPasskeyCheck[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_PK_REPEAT,       SMPI_SC_ACT_PK_CHECK},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_PK_REPEAT */
static const smpTblEntry_t smpiScStateTblPasskeyRepeat[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_INT_PK_NEXT,         SMPI_SC_SM_ST_PK_CALC,         SMPI_SC_ACT_PK_CALC_CA},
  {SMP_MSG_INT_PK_CMPL,         SMPI_SC_SM_ST_CALC_DHKEY,      SMPI_SC_ACT_CALC_DHKEY},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_OOB_SEND_RAND */
static const smpTblEntry_t smpiScStateTblOobSendRand[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_OOB_WAIT_RAND,   SMPI_SC_ACT_OOB_SEND_RAND},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_OOB_WAIT_RAND */
static const smpTblEntry_t smpiScStateTblOobWaitRand[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_CALC_DHKEY,      SMPI_SC_ACT_OOB_PROC_RAND},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_CALC_DHKEY */
static const smpTblEntry_t smpiScStateTblCalcDHKey[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_ECC_CMPL,        SMPI_SC_SM_ST_CALC_F5_TKEY,    SMPI_SC_ACT_CALC_F5_TKEY},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_CALC_F5_TKEY */
static const smpTblEntry_t smpiScStateTblCalcF5TKey[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_CALC_F5_MACKEY,  SMPI_SC_ACT_CALC_F5_MACKEY},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_CALC_F5_MACKEY */
static const smpTblEntry_t smpiScStateTblCalcF5MacKey[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_CALC_F5_LTK,     SMPI_SC_ACT_CALC_F5_LTK},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_CALC_F5_LTK */
static const smpTblEntry_t smpiScStateTblCalcF5LTK[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_CALC_F6_EA,      SMPI_SC_ACT_CALC_F6_EA},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_CALC_F6_EA */
static const smpTblEntry_t smpiScStateTblDhCalcF6Ea[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_CALC_F6_EB,      SMPI_SC_ACT_CALC_F6_EB},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_CALC_F6_EB */
static const smpTblEntry_t smpiScStateTblDhCalcF6Eb[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_CMAC_CMPL,       SMPI_SC_SM_ST_VERIFY_DH_CHECK, SMPI_SC_ACT_SEND_DH_CHECK},
  {0,                           0,                          0}
};

/*! State table for SMPI_SC_SM_ST_VERIFY_DH_CHECK */
static const smpTblEntry_t smpiScStateTblVerifyDHCheck[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_ENCRYPT,         SMPI_SC_ACT_VERIFY_DH_CHECK},
  {0,                           0,                          0}
};

/*! State table for Legacy SMPI_SC_SM_ST_LEGACY_PIN */
static const smpTblEntry_t smpiScStateTblLegacyPin[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_API_AUTH_RSP,        SMPI_SC_SM_ST_CNF_CALC_1,      SMPI_SC_ACT_PAIR_CNF_CALC_1},
  {0,                           0,                          0}
};

/*! State table for Legacy CNF_CALC_1 */
static const smpTblEntry_t smpiScStateTblCnfCalc1[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SC_SM_ST_CNF_CALC_2,      SMPI_SC_ACT_PAIR_CNF_CALC_2},
  {0,                           0,                          0}
};

/*! State table for Legacy CNF_CALC_2 */
static const smpTblEntry_t smpiScStateTblCnfCalc2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SC_SM_ST_PAIR_CNF,        SMPI_SC_ACT_SEND_PAIR_CNF},
  {0,                           0,                          0}
};

/*! State table for Legacy PAIR_CNF */
static const smpTblEntry_t smpiScStateTblPairCnf[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_PAIR_RAND,       SMPI_SC_ACT_PROC_PAIR_CNF},
  {0,                           0,                          0}
};

/*! State table for Legacy PAIR_RAND */
static const smpTblEntry_t smpiScStateTblPairRand[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_CNF_VER_CALC_1,  SMPI_SC_ACT_PAIR_CNF_VER_CALC_1},
  {0,                           0,                          0}
};

/*! State table for Legacy CNF_VER_CALC_1 */
static const smpTblEntry_t smpiScStateTblCnfVerCalc1[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SC_SM_ST_CNF_VER_CALC_2,  SMPI_SC_ACT_PAIR_CNF_VER_CALC_2},
  {0,                           0,                          0}
};

/*! State table for Legacy CNF_VER_CALC_2 */
static const smpTblEntry_t smpiScStateTblCnfVerCalc2[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SC_SM_ST_STK_CALC,        SMPI_SC_ACT_CNF_VERIFY},
  {0,                           0,                          0}
};

/*! State table for Legacy STK_CALC */
static const smpTblEntry_t smpiScStateTblStkCalc[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_WSF_AES_CMPL,        SMPI_SC_SM_ST_ENCRYPT,         SMPI_SC_ACT_STK_ENCRYPT},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for ENCRYPT */
static const smpTblEntry_t smpiScStateTblEncrypt[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_DM_ENCRYPT_CMPL,     SMPI_SC_SM_ST_KEY_DIST,        SMPI_SC_ACT_SETUP_KEY_DIST},
  {SMP_MSG_DM_ENCRYPT_FAILED,   SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SC_SM_ST_ENCRYPT,         SMPI_SC_ACT_NONE},
  {SMP_MSG_DH_CHECK_FAILURE,    SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_PAIRING_CANCEL},
  {SMP_MSG_INT_MAX_ATTEMPTS,    SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_MAX_ATTEMPTS},
  {0,                           0,                          0}
};

/*! State table for KEY_DIST */
static const smpTblEntry_t smpiScStateTblKeyDist[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_KEY_DIST,        SMPI_SC_ACT_RCV_KEY},
  {SMP_MSG_INT_SEND_NEXT_KEY,   SMPI_SC_SM_ST_KEY_DIST,        SMPI_SC_ACT_SEND_KEY},
  {SMP_MSG_INT_PAIRING_CMPL,    SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_PAIRING_CMPL},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SC_SM_ST_KEY_DIST,        SMPI_SC_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for ATTEMPTS */
static const smpTblEntry_t smpiScStateTblAttempts[] =
{
/* Event                        Next state                  Action  */
  {SMP_MSG_INT_WI_TIMEOUT,      SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_CHECK_ATTEMPTS},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SC_SM_ST_RSP_TO,          SMPI_SC_ACT_PAIRING_FAILED},
  {SMP_MSG_CMD_PKT,             SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_ATTEMPT_RCVD},
  {SMP_MSG_API_PAIR_REQ,        SMPI_SC_SM_ST_PAIR_RSP,        SMPI_SC_ACT_NOTIFY_DM_ATTEMPTS},
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_CLEANUP},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SC_SM_ST_ATTEMPTS,        SMPI_SC_ACT_NONE},
  {0,                           0,                          0}
};

/*! State table for RSP_TO */
static const smpTblEntry_t smpiScStateTblRspTo[] =
{
/* Event                        Next state                     Action  */
  {SMP_MSG_DM_CONN_CLOSE,       SMPI_SC_SM_ST_IDLE,            SMPI_SC_ACT_CLEANUP},
  {SMP_MSG_CMD_PAIRING_FAILED,  SMPI_SC_SM_ST_RSP_TO,          SMPI_SC_ACT_NONE},
  {SMP_MSG_API_CANCEL_REQ,      SMPI_SC_SM_ST_RSP_TO,          SMPI_SC_ACT_NONE},
  {SMP_MSG_INT_RSP_TIMEOUT,     SMPI_SC_SM_ST_RSP_TO,          SMPI_SC_ACT_NONE},
  {SMP_MSG_API_PAIR_REQ,        SMPI_SC_SM_ST_RSP_TO,          SMPI_SC_ACT_NOTIFY_DM_RSP_TO},
  {0,                           0,                             0}
};

/*! Table of individual state tables */
static const smpTblEntry_t * const smpiScStateTbl[] =
{
  smpiScStateTblIdle,
  smpiScStateTblPairRsp,
  smpiScStateTblModeSelect,
  smpiScStateTblLescPin,
  smpiScStateTblPubKey,
  smpiScStateTblAuthSelect,
  smpiScStateTblJwNcWaitCnf,
  smpiScStateTblJwNcRand,
  smpiScStateTblJwNcCheck1,
  smpiScStateTblJwNcCheck2,
  smpiScStateTblJwNcWaitUser,

  smprScStateTblPasskeyKeypress,
  smpiScStateTblPasskeyCalc,
  smpiScStateTblPasskeyCnf,
  smpiScStateTblPasskeyRand,
  smpiScStateTblPasskeyCheck,
  smpiScStateTblPasskeyRepeat,

  smpiScStateTblOobSendRand,
  smpiScStateTblOobWaitRand,

  smpiScStateTblCalcDHKey,
  smpiScStateTblCalcF5TKey,
  smpiScStateTblCalcF5MacKey,
  smpiScStateTblCalcF5LTK,
  smpiScStateTblDhCalcF6Ea,
  smpiScStateTblDhCalcF6Eb,
  smpiScStateTblVerifyDHCheck,

  smpiScStateTblLegacyPin,
  smpiScStateTblCnfCalc1,
  smpiScStateTblCnfCalc2,
  smpiScStateTblPairCnf,
  smpiScStateTblPairRand,
  smpiScStateTblCnfVerCalc1,
  smpiScStateTblCnfVerCalc2,
  smpiScStateTblStkCalc,
  smpiScStateTblEncrypt,
  smpiScStateTblKeyDist,
  smpiScStateTblAttempts,
  smpiScStateTblRspTo
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! state machine interface */
const smpSmIf_t smpiScSmIf =
{
  smpiScStateTbl,
  smpiScActionTbl,
  smpiScStateTblCommon
};

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP initiator role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpiScInit(void)
{
  /* set up callback interface */
  smpCb.pMaster = &smpiScSmIf;

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
uint8_t *smpiStateStr(uint8_t state)
{
  switch(state)
  {
    case SMPI_SC_SM_ST_IDLE: return (uint8_t*) "I_IDLE";
    case SMPI_SC_SM_ST_PAIR_RSP: return (uint8_t*) "I_PAIR_RSP";
    case SMPI_SC_SM_ST_MODE_SELECT: return (uint8_t*) "I_MODE_SELECT";
    case SMPI_SC_SM_ST_LESC_PIN: return (uint8_t*) "I_LESC_PIN";
    case SMPI_SC_SM_ST_PUB_KEY: return (uint8_t*) "I_PUB_KEY";
    case SMPI_SC_SM_ST_AUTH_SELECT: return (uint8_t*) "I_AUTH_SELECT";
    case SMPI_SC_SM_ST_JWNC_WAIT_CNF: return (uint8_t*) "I_JWNC_WAIT_CNF";
    case SMPI_SC_SM_ST_JWNC_RAND: return (uint8_t*) "I_JWNC_RAND";
    case SMPI_SC_SM_ST_JWNC_CHECK_1: return (uint8_t*) "I_JWNC_CHECK_1";
    case SMPI_SC_SM_ST_JWNC_CHECK_2: return (uint8_t*) "I_JWNC_CHECK_2";
    case SMPI_SC_SM_ST_JWNC_WAIT_USER: return (uint8_t*) "I_JWNC_WAIT_USER";
    case SMPI_SC_SM_ST_PK_KEYPRESS: return (uint8_t*) "I_PK_KEYPRESS";
    case SMPI_SC_SM_ST_PK_CALC: return (uint8_t*) "I_PK_CALC";
    case SMPI_SC_SM_ST_PK_CNF: return (uint8_t*) "I_PK_CNF";
    case SMPI_SC_SM_ST_PK_RAND: return (uint8_t*) "I_PK_RAND";
    case SMPI_SC_SM_ST_PK_CHECK: return (uint8_t*) "I_PK_CHECK";
    case SMPI_SC_SM_ST_PK_REPEAT: return (uint8_t*) "I_PK_REPEAT";

    case SMPI_SC_SM_ST_OOB_SEND_RAND: return (uint8_t*) "I_OOB_SEND_RAND";
    case SMPI_SC_SM_ST_OOB_WAIT_RAND: return (uint8_t*) "I_OOB_WAIT_RAND";

    case SMPI_SC_SM_ST_CALC_DHKEY: return (uint8_t*) "I_CALC_DHKEY";
    case SMPI_SC_SM_ST_CALC_F5_TKEY: return (uint8_t*) "I_CALC_F5_TKEY";
    case SMPI_SC_SM_ST_CALC_F5_MACKEY: return (uint8_t*) "I_CALC_F5_MACKEY";
    case SMPI_SC_SM_ST_CALC_F5_LTK: return (uint8_t*) "I_CALC_F5_LTK";
    case SMPI_SC_SM_ST_CALC_F6_EA: return (uint8_t*) "I_CALC_F6_EA";
    case SMPI_SC_SM_ST_CALC_F6_EB: return (uint8_t*) "I_CALC_F6_EB";
    case SMPI_SC_SM_ST_VERIFY_DH_CHECK: return (uint8_t*) "I_VERIFY_DH_CHECK";

    case SMPI_SC_SM_ST_LEGACY_PIN: return (uint8_t*) "I_LEGACY_PIN";
    case SMPI_SC_SM_ST_CNF_CALC_1: return (uint8_t*) "I_CNF_CALC_1";
    case SMPI_SC_SM_ST_CNF_CALC_2: return (uint8_t*) "I_CNF_CALC_2";
    case SMPI_SC_SM_ST_PAIR_CNF: return (uint8_t*) "I_PAIR_CNF";
    case SMPI_SC_SM_ST_PAIR_RAND: return (uint8_t*) "I_PAIR_RAND";
    case SMPI_SC_SM_ST_CNF_VER_CALC_1: return (uint8_t*) "I_CNF_VER_CALC_1";
    case SMPI_SC_SM_ST_CNF_VER_CALC_2: return (uint8_t*) "I_CNF_VER_CALC_2";
    case SMPI_SC_SM_ST_STK_CALC: return (uint8_t*) "I_STK_CALC";
    case SMPI_SC_SM_ST_ENCRYPT: return (uint8_t*) "I_ENCRYPT";
    case SMPI_SC_SM_ST_KEY_DIST: return (uint8_t*) "I_KEY_DIST";
    case SMPI_SC_SM_ST_ATTEMPTS: return (uint8_t*) "I_ATTEMPTS";
    case SMPI_SC_SM_ST_RSP_TO: return (uint8_t*) "I_RSP_TO";

    default: return (uint8_t*) "I_Unknown";
  }
}
