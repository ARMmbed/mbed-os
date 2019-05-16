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
 *  \brief ATT server main module.
 */
/*************************************************************************************************/
#ifndef ATTS_MAIN_H
#define ATTS_MAIN_H

#include "wsf_queue.h"
#include "wsf_timer.h"
#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* ATTS event handler messages */
enum
{
  ATTS_MSG_IDLE_TIMEOUT = ATTS_MSG_START,
  ATTS_MSG_API_VALUE_IND_NTF,
  ATTS_MSG_IND_TIMEOUT,
  ATTS_MSG_SIGN_CMAC_CMPL,
  ATTS_MSG_DBH_CMAC_CMPL
};

/*!
 * Data buffer format for API request messages:
 *
 * | attsPktParam_t | ATT request data |
 * | bytes 0 to 7   | bytes 8 -        |
 */

/* API parameters */
typedef struct
{
  uint16_t                    len;
  uint16_t                    handle;
} attsPktParam_t;

/* verify attsPktParam_t will work in data buffer format described above */
WSF_CT_ASSERT(sizeof(attsPktParam_t) <= L2C_PAYLOAD_START);

/* API message structure */
typedef struct
{
  wsfMsgHdr_t                 hdr;
  attsPktParam_t              *pPkt;
} attsApiMsg_t;

/* Connection control block for indications/notifications */
typedef struct
{
  wsfTimer_t        outIndTimer;                      /* Outstanding indication timer */
  attCcb_t          *pMainCcb;                        /* Pointer to ATT main CCB */
  uint16_t          outIndHandle;                     /* Waiting for confirm from peer for this indication handle */
  uint16_t          pendIndHandle;                    /* Callback to application pending for this indication handle */
  uint16_t          pendNtfHandle[ATT_NUM_SIMUL_NTF]; /* Callback to application pending for this notification handle */
} attsIndCcb_t;

/* Control block for indications/notifications */
typedef struct
{
  attsIndCcb_t         ccb[DM_CONN_MAX];
} attsIndCb_t;

/* Client characteristic configuration descriptor callback type
 *
 *  \param  connId      DM connection ID.
 *  \param  method      Read or write.
 *  \param  handle      Attribute handle of the descriptor.
 *  \param  pValue      Pointer to the attribute value of the descriptor.
 *
 *  \return ATT_SUCCESS if successful otherwise error.
 */
typedef uint8_t (*attsCccFcn_t)(dmConnId_t connId, uint8_t method, uint16_t handle, uint8_t *pValue);

/* Main control block of the ATTS subsystem */
typedef struct
{
  wsfQueue_t        groupQueue;       /* Queue of attribute groups */
  attFcnIf_t const  *pInd;            /* Indication callback interface */
  attMsgHandler_t   signMsgCback;     /* Signed data callback interface */
  attsAuthorCback_t authorCback;      /* Authorization callback */
  attsCccFcn_t      cccCback;         /* CCC callback */
} attsCb_t;

/* PDU processing function type */
typedef void (*attsProcFcn_t)(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);

/* CSF Control block */
typedef struct
{
  attsCsfRec_t  attsCsfTable[DM_CONN_MAX];  /* connected clients' supported features record table. */
  attsCsfWriteCback_t writeCback;           /* Write callback. */
  uint8_t isHashUpdating;                   /* Database hash update status. */
} attsCsfCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* PDU processing function lookup table, indexed by method */
extern attsProcFcn_t attsProcFcnTbl[ATT_METHOD_SIGNED_WRITE_CMD+1];

/* Control block for indications/notifications */
extern attsIndCb_t attsIndCb;

/* Control block */
extern attsCb_t attsCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void attsErrRsp(uint16_t handle, uint8_t opcode, uint16_t attHandle, uint8_t reason);
void attsClearPrepWrites(attCcb_t *pCcb);
bool_t attsUuidCmp(attsAttr_t *pAttr, uint8_t uuidLen, uint8_t *pUuid);
bool_t attsUuid16Cmp(uint8_t *pUuid16, uint8_t uuidLen, uint8_t *pUuid);
attsAttr_t *attsFindByHandle(uint16_t handle, attsGroup_t **pAttrGroup);
uint16_t attsFindInRange(uint16_t startHandle, uint16_t endHandle, attsAttr_t **pAttr);
uint16_t attsFindUuidInRange(uint16_t startHandle, uint16_t endHandle, uint8_t uuidLen,
                             uint8_t *pUuid, attsAttr_t **pAttr, attsGroup_t **pAttrGroup);
uint8_t attsPermissions(dmConnId_t connId, uint8_t permit, uint16_t handle, uint8_t permissions);
void attsDiscBusy(attCcb_t *pCcb);
void attsCheckPendDbHashReadRsp(void);
void attsProcessDatabaseHashUpdate(secCmacMsg_t *pMsg);
uint16_t attsIsHashableAttr(attsAttr_t *pAttr);

void attsProcMtuReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcFindInfoReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcFindTypeReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadTypeReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadBlobReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadMultReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadGroupTypeReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcWrite(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcPrepWriteReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcExecWriteReq(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcValueCnf(attCcb_t *pCcb, uint16_t len, uint8_t *pPacket);

uint8_t attsCsfActClientState(uint16_t handle, uint8_t opcode, uint8_t *pPacket);
uint8_t attsCsfIsClientChangeAware(dmConnId_t connId, uint16_t handle);
void attsCsfSetHashUpdateStatus(bool_t isUpdating);
uint8_t attsCsfGetHashUpdateStatus(void);

#ifdef __cplusplus
};
#endif

#endif /* ATTS_MAIN_H */
