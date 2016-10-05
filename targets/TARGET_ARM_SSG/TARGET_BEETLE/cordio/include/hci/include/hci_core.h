/*************************************************************************************************/
/*!
 *  \file   hci_core.h
 *
 *  \brief  HCI core interfaces.
 *
 *          $Date: 2015-06-12 07:19:18 -0400 (Fri, 12 Jun 2015) $
 *          $Revision: 3061 $
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*************************************************************************************************/
#ifndef HCI_CORE_H
#define HCI_CORE_H

#include "hci_core_ps.h"
#include "wsf_queue.h"
#include "wsf_os.h"
#include "hci_api.h"
#include "cfg_stack.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Per-connection structure for ACL packet accounting */
typedef struct
{
  uint8_t         *pTxAclPkt;                   /* Fragmenting TX ACL packet pointer */
  uint8_t         *pNextTxFrag;                 /* Next TX ACL packet fragment */
  uint8_t         *pRxAclPkt;                   /* RX ACL packet pointer */
  uint8_t         *pNextRxFrag;                 /* Next RX ACL packet fragment */
  uint16_t        handle;                       /* Connection handle */
  uint16_t        txAclRemLen;                  /* Fragmenting TX ACL packet remaining length */
  uint16_t        rxAclRemLen;                  /* Fragmented RX ACL packet remaining length */
  bool_t          fragmenting;                  /* TRUE if fragmenting a TX ACL packet */
  bool_t          flowDisabled;                 /* TRUE if data flow disabled */
  uint8_t         queuedBufs;                   /* Queued ACL buffers on this connection */
  uint8_t         outBufs;                      /* Outstanding ACL buffers sent to controller */
} hciCoreConn_t;

/* Main control block for dual-chip implementation */
typedef struct
{
  hciCoreConn_t   conn[DM_CONN_MAX];            /* Connection structures */
  uint8_t         leStates[HCI_LE_STATES_LEN];  /* Controller LE supported states */
  bdAddr_t        bdAddr;                       /* Bluetooth device address */
  wsfQueue_t      aclQueue;                     /* HCI ACL TX queue */
  hciCoreConn_t   *pConnRx;                     /* Connection struct for current transport RX packet */
  uint16_t        maxRxAclLen;                  /* Maximum reassembled RX ACL packet length */
  uint16_t        bufSize;                      /* Controller ACL data buffer size */
  uint8_t         aclQueueHi;                   /* Disable flow when this many ACL buffers queued */
  uint8_t         aclQueueLo;                   /* Enable flow when this many ACL buffers queued */
  uint8_t         availBufs;                    /* Current avail ACL data buffers */
  uint8_t         numBufs;                      /* Controller number of ACL data buffers */
  uint8_t         whiteListSize;                /* Controller white list size */
  uint8_t         numCmdPkts;                   /* Controller command packed count */
  uint8_t         leSupFeat;                    /* Controller LE supported features */
  int8_t          advTxPwr;                     /* Controller advertising TX power */
} hciCoreCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Control block */
extern hciCoreCb_t hciCoreCb;

/* LE event mask */
extern const uint8_t hciLeEventMask[HCI_LE_EVT_MASK_LEN];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void hciCoreInit(void);
void hciCoreResetStart(void);
void hciCoreConnOpen(uint16_t handle);
void hciCoreConnClose(uint16_t handle);
hciCoreConn_t *hciCoreConnByHandle(uint16_t handle);
void hciCoreSendAclData(hciCoreConn_t *pConn, uint8_t *pData);
void hciCoreTxReady(uint8_t bufs);
void hciCoreTxAclStart(hciCoreConn_t *pConn, uint16_t len, uint8_t *pData);
bool_t hciCoreTxAclContinue(hciCoreConn_t *pConn);
void hciCoreTxAclComplete(hciCoreConn_t *pConn, uint8_t *pData);
uint8_t *hciCoreAclReassembly(uint8_t *pData);

#ifdef __cplusplus
};
#endif

#endif /* HCI_CORE_H */
