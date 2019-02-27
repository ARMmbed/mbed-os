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
 *  \brief HCI main module.
 */
/*************************************************************************************************/
#ifndef HCI_MAIN_H
#define HCI_MAIN_H

#include "wsf_os.h"
#include "wsf_queue.h"
#include "hci_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Message types for HCI event handler */
#define HCI_MSG_CMD_TIMEOUT       1         /* Command timeout timer expired */

/* Event types for HCI event handler */
#define HCI_EVT_RX                0x01      /* Data received on rx queue */

/* Number of times to send HCI_RAND_CMD on reset to prefill random buffer */
#define HCI_RESET_RAND_CNT        4

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Type used in number of completed packets event type */
typedef struct
{
  uint16_t              handle;
  uint16_t              num;
} hciNumPkts_t;

/* Type for HCI number of completed packets event */
typedef struct
{
  uint8_t               numHandles;
  hciNumPkts_t          numPkts[1];
} hciNumCmplPktsEvt_t;

/* Main control block of the HCI subsystem */
typedef struct
{
  wsfQueue_t            rxQueue;
  hciEvtCback_t         evtCback;
  hciSecCback_t         secCback;
  hciAclCback_t         aclCback;
  hciFlowCback_t        flowCback;
  wsfHandlerId_t        handlerId;
  bool_t                resetting;
} hciCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Control block */
extern hciCb_t hciCb;

#ifdef __cplusplus
};
#endif

#endif /* HCI_MAIN_H */
