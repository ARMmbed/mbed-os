/* Copyright (c) 2009-2019 Arm Limited
 * Copyright (c) 2019-2020 Packetcraft, Inc.
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
 *  \brief SMP subsystem API.
 */
/*************************************************************************************************/
#ifndef SMP_API_H
#define SMP_API_H

#include "wsf_os.h"
#include "smp_defs.h"
#include "dm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_SMP_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name SMP Events
 * Events recognized and handled by the SMP state machine.
 */
/**@{*/
/*! \brief Event handler messages for SMP state machines */
enum
{
  SMP_MSG_API_PAIR_REQ = 1,               /*!< \brief API pairing request */
  SMP_MSG_API_PAIR_RSP,                   /*!< \brief API pairing response */
  SMP_MSG_API_CANCEL_REQ,                 /*!< \brief API cancel request */
  SMP_MSG_API_AUTH_RSP,                   /*!< \brief API pin response */
  SMP_MSG_API_SECURITY_REQ,               /*!< \brief API security request */
  SMP_MSG_CMD_PKT,                        /*!< \brief SMP command packet received */
  SMP_MSG_CMD_PAIRING_FAILED,             /*!< \brief SMP pairing failed packet received */
  SMP_MSG_DM_ENCRYPT_CMPL,                /*!< \brief Link encrypted */
  SMP_MSG_DM_ENCRYPT_FAILED,              /*!< \brief Link encryption failed */
  SMP_MSG_DM_CONN_CLOSE,                  /*!< \brief Connection closed */
  SMP_MSG_WSF_AES_CMPL,                   /*!< \brief AES calculation complete */
  SMP_MSG_INT_SEND_NEXT_KEY,              /*!< \brief Send next key to be distributed */
  SMP_MSG_INT_MAX_ATTEMPTS,               /*!< \brief Maximum pairing attempts reached */
  SMP_MSG_INT_PAIRING_CMPL,               /*!< \brief Pairing complete */
  SMP_MSG_INT_RSP_TIMEOUT,                /*!< \brief Pairing protocol response timeout */
  SMP_MSG_INT_WI_TIMEOUT,                 /*!< \brief Pairing protocol wait interval timeout */
  SMP_MSG_INT_LESC,                       /*!< \brief Pair with Secure Connections */
  SMP_MSG_INT_LEGACY,                     /*!< \brief Pair with Legacy Security */
  SMP_MSG_INT_JW_NC,                      /*!< \brief LESC Just-Works/Numeric Comparison pairing */
  SMP_MSG_INT_PASSKEY,                    /*!< \brief LESC Passkey pairing */
  SMP_MSG_INT_OOB,                        /*!< \brief LESC Out-of-Band Pairing */
  SMP_MSG_API_USER_CONFIRM,               /*!< \brief User confirms valid numeric comparison */
  SMP_MSG_API_USER_KEYPRESS,              /*!< \brief User keypress in passkey pairing */
  SMP_MSG_API_KEYPRESS_CMPL,              /*!< \brief User keypress complete in passkey pairing */
  SMP_MSG_WSF_ECC_CMPL,                   /*!< \brief WSF ECC operation complete */
  SMP_MSG_INT_PK_NEXT,                    /*!< \brief Continue to next passkey bit */
  SMP_MSG_INT_PK_CMPL,                    /*!< \brief Passkey operation complete */
  SMP_MSG_WSF_CMAC_CMPL,                  /*!< \brief WSF CMAC operation complete */
  SMP_MSG_DH_CHECK_FAILURE,               /*!< \brief DHKey check failure */
  SMP_MSG_EARLY_CNF,                      /*!< \brief An early Confirm from the initiator in passkey pairing */
  SMP_NUM_MSGS                            /*!< \brief Number of SMP message types. */
};
/**@}*/

/**@{*/
/*! \brief Additional SMP messages */
enum
{
  SMP_DB_SERVICE_IND = SMP_NUM_MSGS       /*!< \brief SMP DB Service timer indication */
};
/**@}*/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Configurable parameters */
typedef struct
{
  uint32_t            attemptTimeout;     /*!< \brief 'Repeated attempts' timeout in msec */
  uint8_t             ioCap;              /*!< \brief I/O Capability */
  uint8_t             minKeyLen;          /*!< \brief Minimum encryption key length */
  uint8_t             maxKeyLen;          /*!< \brief Maximum encryption key length */
  uint8_t             maxAttempts;        /*!< \brief Attempts to trigger 'repeated attempts' timeout */
  uint8_t             auth;               /*!< \brief Device authentication requirements */
  uint32_t            maxAttemptTimeout;  /*!< \brief Maximum 'Repeated attempts' timeout in msec */
  uint32_t            attemptDecTimeout;  /*!< \brief Time msec before attemptExp decreases */
  uint16_t            attemptExp;         /*!< \brief Exponent to raise attemptTimeout on maxAttempts */
} smpCfg_t;

/*! \brief Data type for SMP_MSG_API_PAIR_REQ and SMP_MSG_API_PAIR_RSP */
typedef struct
{
  wsfMsgHdr_t         hdr;        /*!< \brief Message header */
  uint8_t             oob;        /*!< \brief Out-of-band data present flag */
  uint8_t             auth;       /*!< \brief authentication flags */
  uint8_t             iKeyDist;   /*!< \brief Initiator key distribution flags */
  uint8_t             rKeyDist;   /*!< \brief Responder key distribution flags */
} smpDmPair_t;

/*! \brief Data type for SMP_MSG_API_AUTH_RSP */
typedef struct
{
  wsfMsgHdr_t         hdr;                    /*!< \brief Message header */
  uint8_t             authData[SMP_OOB_LEN];  /*!< \brief Authentication data to display */
  uint8_t             authDataLen;            /*!< \brief Length of authentication data */
} smpDmAuthRsp_t;

/*! \brief Data type for SMP_MSG_API_USER_KEYPRESS */
typedef struct
{
  wsfMsgHdr_t         hdr;        /*!< \brief Message header */
  uint8_t             keypress;   /*!< \brief Keypress */
} smpDmKeypress_t;

/*! \brief Data type for SMP_MSG_API_SECURITY_REQ */
typedef struct
{
  wsfMsgHdr_t         hdr;   /*!< \brief Message header */
  uint8_t             auth;  /*!< \brief Authentication flags */
} smpDmSecurityReq_t;

/*! \brief Union SMP DM message data types */
typedef union
{
  wsfMsgHdr_t         hdr;           /*!< \brief Message header */
  smpDmPair_t         pair;          /*!< \brief Pairing request/response message */
  smpDmAuthRsp_t      authRsp;       /*!< \brief Authentication message */
  smpDmSecurityReq_t  securityReq;   /*!< \brief Security Request message */
  smpDmKeypress_t     keypress;      /*!< \brief Keypress message */
} smpDmMsg_t;

/*! \} */    /* STACK_SMP_API */

/**************************************************************************************************
  Global Variables;
**************************************************************************************************/

/*! \addtogroup STACK_INIT
 *  \{ */

/** \name SMP Configuration Structure
 * Pointer to structure containing initialization details of the SMP Subsystem.  To be configured
 * by Application.
 */
/**@{*/
/*! \brief Configuration pointer */
extern smpCfg_t *pSmpCfg;
/**@}*/

/*! \} */    /* STACK_INIT */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup STACK_SMP_API
 *  \{ */

/** \name SMP Initialization Functions
 * Legacy and Secure Connections initialization for Initiator and Responder roles.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP initiator role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpiInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP responder role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmprInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP initiator role utilizing BTLE Secure Connections.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpiScInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP responder role utilizing BTLE Secure Connections.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmprScInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Use this SMP init function when SMP is not supported.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpNonInit(void);

/**@}*/

/** \name SMP DM Interface Functions
 * Functions that allow the DM to send messages to SMP.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to send a message to SMP.
 *
 *  \param  pMsg      Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDmMsgSend(smpDmMsg_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to notify SMP of encrypted link status.
 *
 *  \param  pMsg    Pointer to HCI message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDmEncryptInd(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Check if LE Secure Connections is enabled on the connection.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return TRUE is Secure Connections is enabled, else FALSE
 */
/*************************************************************************************************/
bool_t SmpDmLescEnabled(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Return the STK for the given connection.
 *
 *  \param  connId    Connection identifier.
 *  \param  pSecLevel Returns the security level of pairing when STK was created.
 *
 *  \return Pointer to STK or NULL if not available.
 */
/*************************************************************************************************/
uint8_t *SmpDmGetStk(dmConnId_t connId, uint8_t *pSecLevel);

/*************************************************************************************************/
/*!
 *  \brief  Format a cancel message with consideration for the attempts counter
 *
 *  \param  connId  Connection Id.
 *  \param  pHdr    Pointer to header of message to fill.
 *  \param  status  Status to include.
 *
 *  \return none.
 */
/*************************************************************************************************/
void SmpScGetCancelMsgWithReattempt(dmConnId_t connId, wsfMsgHdr_t *pHdr, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the SMP Database.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to force the DhKey to zero for qualification test purposes.
 *
 *  \param  enable  TRUE - Force DhKey to zero.  FALSE - Use calculated key
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpScEnableZeroDhKey(bool_t enable);

/**@}*/

/*! \} */    /* STACK_SMP_API */

#ifdef __cplusplus
};
#endif

#endif /* SMP_API_H */
