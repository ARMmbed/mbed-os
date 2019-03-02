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
 *  \brief ATT main module.
 */
/*************************************************************************************************/
#ifndef ATT_MAIN_H
#define ATT_MAIN_H

#include "wsf_queue.h"
#include "wsf_timer.h"
#include "l2c_api.h"
#include "dm_api.h"
#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* ATT protocol methods */
#define ATT_METHOD_ERR                0             /* Error response */
#define ATT_METHOD_MTU                1             /* Exchange mtu */
#define ATT_METHOD_FIND_INFO          2             /* Find information */
#define ATT_METHOD_FIND_TYPE          3             /* Find by type value */
#define ATT_METHOD_READ_TYPE          4             /* Read by type value */
#define ATT_METHOD_READ               5             /* Read */
#define ATT_METHOD_READ_BLOB          6             /* Read long */
#define ATT_METHOD_READ_MULTIPLE      7             /* Read multiple */
#define ATT_METHOD_READ_GROUP_TYPE    8             /* Read group type */
#define ATT_METHOD_WRITE              9             /* Write */
#define ATT_METHOD_WRITE_CMD          10            /* Write command */
#define ATT_METHOD_PREPARE_WRITE      11            /* Prepare write */
#define ATT_METHOD_EXECUTE_WRITE      12            /* Execute write */
#define ATT_METHOD_VALUE_NTF          13            /* Handle value notification */
#define ATT_METHOD_VALUE_IND          14            /* Handle value indication */
#define ATT_METHOD_VALUE_CNF          15            /* Handle value confirm */
#define ATT_METHOD_SIGNED_WRITE_CMD   16            /* Signed write command */

/* Convert opcode to method */
#define ATT_OPCODE_2_METHOD(op)       (((op) & ~ATT_PDU_MASK_SERVER) / 2)

/* Client and server message macros */
#define ATTC_MSG_START                0x00
#define ATTS_MSG_START                0x20
#define ATT_MSG_MASK(msg)             ((msg) & 0x1F)

/* Buffer lengths for messages */
#define ATT_VALUE_IND_NTF_BUF_LEN     (ATT_VALUE_NTF_LEN + L2C_PAYLOAD_START)

/* attCcb_t control bits */
#define ATT_CCB_STATUS_MTU_SENT       (1<<0)        /* MTU req or rsp sent */
#define ATT_CCB_STATUS_FLOW_DISABLED  (1<<1)        /* Data flow disabled */
#define ATT_CCB_STATUS_TX_TIMEOUT     (1<<2)        /* ATT transaction timed out */
#define ATT_CCB_STATUS_RSP_PENDING    (1<<3)        /* ATTS write rsp pending */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef struct
{
  uint16_t startHandle; /* Start handle of the requested operation.  Used if an Error Response is sent. */
  uint16_t handle;      /* Attribute handle of pending response. */
 } attPendDbHashRsp_t;

/* Connection control block */
typedef struct
{
  wsfQueue_t         prepWriteQueue;    /* prepare write queue */
  wsfTimer_t         idleTimer;         /* service discovery idle timer */
  uint16_t           handle;            /* connection handle */
  uint16_t           mtu;               /* connection mtu */
  dmConnId_t         connId;            /* DM connection ID */
  uint8_t            control;           /* Control bitfield */
  attPendDbHashRsp_t *pPendDbHashRsp;   /* Pending ATT Response information. */
} attCcb_t;

/* ATT message handling function type */
typedef void (*attMsgHandler_t)(void *pMsg);

/* ATT connection callback type */
typedef void (*attConnCback_t)(attCcb_t *pCcb, dmEvt_t *pDmEvt);

/* Callback interface for client and server */
typedef struct
{
  l2cDataCback_t    dataCback;   /* Data callback */
  l2cCtrlCback_t    ctrlCback;   /* Control callback */
  attMsgHandler_t   msgCback;    /* Message handling callback */
  attConnCback_t    connCback;   /* Connection callback */
} attFcnIf_t;

/* Main control block of the ATT subsystem */
typedef struct
{
  attCcb_t          ccb[DM_CONN_MAX];  /* Connection control blocks */
  attFcnIf_t const  *pClient;          /* Client callback interface */
  attFcnIf_t const  *pServer;          /* Server callback interface */
  attCback_t        cback;             /* ATT callback function */
  dmCback_t         connCback;         /* ATT connection callback function */
  wsfHandlerId_t    handlerId;         /* WSF handler ID */
  uint8_t           errTest;           /* Status code for error testing */
} attCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Default component function inteface */
extern const attFcnIf_t attFcnDefault;

/* Control block */
extern attCb_t attCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void attEmptyHandler(wsfMsgHdr_t *pMsg);
void attEmptyDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket);
void attEmptyConnCback(attCcb_t *pCcb, dmEvt_t *pDmEvt);

attCcb_t *attCcbByHandle(uint16_t handle);
attCcb_t *attCcbByConnId(dmConnId_t connId);

bool_t attUuidCmp16to128(const uint8_t *pUuid16, const uint8_t *pUuid128);
void attSetMtu(attCcb_t *pCcb, uint16_t peerMtu, uint16_t localMtu);
void attExecCallback(dmConnId_t connId, uint8_t event, uint16_t handle, uint8_t status, uint16_t mtu);
void *attMsgAlloc(uint16_t len);

#ifdef __cplusplus
};
#endif

#endif /* ATT_MAIN_H */
