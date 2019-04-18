/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Link layer controller master initiate state machine implementation file.
 */
/*************************************************************************************************/

#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "lctr_int_init_master_ae.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Action function call signature. */
typedef void (*lctrActFn_t)(lctrExtScanCtx_t *pExtInitCtx);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
/*! \brief      Initiate state machine action table.
 */
static const lctrActFn_t lctrExtInitActionTbl[LCTR_EXT_INIT_STATE_TOTAL][LCTR_EXT_INIT_MSG_TOTAL] =
{
  { /* LCTR_EXT_INIT_STATE_DISABLED */
    NULL,                           /* LCTR_EXT_INIT_MSG_RESET */
    NULL,                           /* LCTR_EXT_INIT_MSG_INITIATE */                /* Handled by lctrMstExtInitExecuteCommonSm. */
    lctrExtInitActDisallowCancel,   /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    NULL,                           /* LCTR_EXT_INIT_MSG_TERMINATE */
  },
  { /* LCTR_EXT_INIT_STATE_ENABLED */
    lctrExtInitActShutdown,         /* LCTR_EXT_INIT_MSG_RESET */
    lctrExtInitActDisallowInitiate, /* LCTR_EXT_INIT_MSG_INITIATE */
    lctrExtInitActShutdown,         /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    lctrExtInitActConnect,          /* LCTR_EXT_INIT_MSG_TERMINATE */
  },
  { /* LCTR_EXT_INIT_STATE_SHUTDOWN */
    NULL,                           /* LCTR_EXT_INIT_MSG_RESET */
    lctrExtInitActDisallowInitiate, /* LCTR_EXT_INIT_MSG_INITIATE */
    NULL,                           /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    lctrExtInitActScanTerm,         /* LCTR_EXT_INIT_MSG_TERMINATE */

  },
  { /* LCTR_EXT_INIT_STATE_RESET */
    NULL,                           /* LCTR_EXT_INIT_MSG_RESET */
    lctrExtInitActDisallowInitiate, /* LCTR_EXT_INIT_MSG_INITIATE */
    lctrExtInitActDisallowCancel,   /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    lctrExtInitActScanTerm,         /* LCTR_EXT_INIT_MSG_TERMINATE */
  }
};

/*! \brief      Initiation state machine next state table. */
static const uint8_t lctrExtInitNextStateTbl[LCTR_EXT_INIT_STATE_TOTAL][LCTR_EXT_INIT_MSG_TOTAL] =
{
  { /* LCTR_EXT_INIT_STATE_DISABLED */
    LCTR_EXT_INIT_STATE_DISABLED,   /* LCTR_EXT_INIT_MSG_RESET */
    LCTR_EXT_INIT_STATE_ENABLED,   /* LCTR_EXT_INIT_MSG_INITIATE */
    LCTR_EXT_INIT_STATE_DISABLED,   /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    LCTR_EXT_INIT_STATE_DISABLED,   /* LCTR_EXT_INIT_MSG_TERMINATE */
  },
  { /* LCTR_EXT_INIT_STATE_ENABLED */
    LCTR_EXT_INIT_STATE_RESET,      /* LCTR_EXT_INIT_MSG_RESET */
    LCTR_EXT_INIT_STATE_ENABLED,    /* LCTR_EXT_INIT_MSG_INITIATE */
    LCTR_EXT_INIT_STATE_SHUTDOWN,   /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    LCTR_EXT_INIT_STATE_DISABLED,   /* LCTR_EXT_INIT_MSG_TERMINATE */
  },
  { /* LCTR_EXT_INIT_STATE_SHUTDOWN */
    LCTR_EXT_INIT_STATE_RESET,      /* LCTR_EXT_INIT_MSG_RESET */
    LCTR_EXT_INIT_STATE_SHUTDOWN,   /* LCTR_EXT_INIT_MSG_INITIATE */
    LCTR_EXT_INIT_STATE_SHUTDOWN,   /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    LCTR_EXT_INIT_STATE_DISABLED,   /* LCTR_EXT_INIT_MSG_TERMINATE */
  },
  { /* LCTR_EXT_INIT_STATE_RESET */
    LCTR_EXT_INIT_STATE_RESET,      /* LCTR_EXT_INIT_MSG_RESET */
    LCTR_EXT_INIT_STATE_RESET,      /* LCTR_EXT_INIT_MSG_INITIATE */
    LCTR_EXT_INIT_STATE_RESET,      /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL */
    LCTR_EXT_INIT_STATE_DISABLED,   /* LCTR_EXT_INIT_MSG_TERMINATE */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Execute master scan and initiate state machine.
 *
 *  \param      pExtInitCtx   Extended scanning context.
 *  \param      event         State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstExtInitExecuteSm(lctrExtScanCtx_t *pExtInitCtx, uint8_t event)
{
  LL_TRACE_INFO2("lctrMstExtInitExecuteSm: state=%u, event=%u", pExtInitCtx->state, event);

  if (lctrExtInitActionTbl[pExtInitCtx->state][event])
  {
    lctrExtInitActionTbl[pExtInitCtx->state][event](pExtInitCtx);
  }

  pExtInitCtx->state = lctrExtInitNextStateTbl[pExtInitCtx->state][event];
}
