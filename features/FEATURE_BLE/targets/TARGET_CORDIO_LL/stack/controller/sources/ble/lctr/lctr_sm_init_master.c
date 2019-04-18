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
#include "lctr_int_init_master.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Action function call signature. */
typedef void (*lctrActFn_t)(void);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
/*! \brief      Initiate state machine action table.
 */
static const lctrActFn_t lctrInitActionTbl[LCTR_INIT_STATE_TOTAL][LCTR_INIT_MSG_TOTAL] =
{
  { /* LCTR_INIT_STATE_DISABLED */
    NULL,                       /* LCTR_INIT_MSG_RESET */
    lctrInitActInitiate,        /* LCTR_INIT_MSG_INITIATE */
    lctrInitActDisallowCancel,  /* LCTR_INIT_MSG_INITIATE_CANCEL */
    NULL,                       /* LCTR_INIT_MSG_TERMINATE */
  },
  { /* LCTR_INIT_STATE_ENABLED */
    lctrInitActShutdown,        /* LCTR_INIT_MSG_RESET */
    lctrInitActDisallowInitiate,/* LCTR_INIT_MSG_INITIATE */
    lctrInitActShutdown,        /* LCTR_INIT_MSG_INITIATE_CANCEL */
    lctrInitActConnect,         /* LCTR_INIT_MSG_TERMINATE */
  },
  { /* LCTR_INIT_STATE_SHUTDOWN */
    NULL,                       /* LCTR_INIT_MSG_RESET */
    lctrInitActDisallowInitiate,/* LCTR_INIT_MSG_INITIATE */
    NULL,                       /* LCTR_INIT_MSG_INITIATE_CANCEL */
    lctrInitActScanTerm,        /* LCTR_INIT_MSG_TERMINATE */
  },
  { /* LCTR_INIT_STATE_RESET */
    NULL,                       /* LCTR_INIT_MSG_RESET */
    lctrInitActDisallowInitiate,/* LCTR_INIT_MSG_INITIATE */
    lctrInitActDisallowCancel,  /* LCTR_INIT_MSG_INITIATE_CANCEL */
    lctrInitActResetTerm,       /* LCTR_INIT_MSG_TERMINATE */
  }
};

/*! \brief      Initiation state machine next state table. */
static const uint8_t lctrInitNextStateTbl[LCTR_INIT_STATE_TOTAL][LCTR_INIT_MSG_TOTAL] =
{
  { /* LCTR_INIT_STATE_DISABLED */
    LCTR_INIT_STATE_DISABLED,   /* LCTR_INIT_MSG_RESET */
    LCTR_INIT_STATE_ENABLED,    /* LCTR_INIT_MSG_INITIATE */
    LCTR_INIT_STATE_DISABLED,   /* LCTR_INIT_MSG_INITIATE_CANCEL */
    LCTR_INIT_STATE_DISABLED,   /* LCTR_INIT_MSG_TERMINATE */
  },
  { /* LCTR_INIT_STATE_ENABLED */
    LCTR_INIT_STATE_RESET,      /* LCTR_INIT_MSG_RESET */
    LCTR_INIT_STATE_ENABLED,    /* LCTR_INIT_MSG_INITIATE */
    LCTR_INIT_STATE_SHUTDOWN,   /* LCTR_INIT_MSG_INITIATE_CANCEL */
    LCTR_INIT_STATE_DISABLED,   /* LCTR_INIT_MSG_TERMINATE */
  },
  { /* LCTR_INIT_STATE_SHUTDOWN */
    LCTR_INIT_STATE_RESET,      /* LCTR_INIT_MSG_RESET */
    LCTR_INIT_STATE_SHUTDOWN,   /* LCTR_INIT_MSG_INITIATE */
    LCTR_INIT_STATE_SHUTDOWN,   /* LCTR_INIT_MSG_INITIATE_CANCEL */
    LCTR_INIT_STATE_DISABLED,   /* LCTR_INIT_MSG_TERMINATE */
  },
  { /* LCTR_INIT_STATE_RESET */
    LCTR_INIT_STATE_RESET,      /* LCTR_INIT_MSG_RESET */
    LCTR_INIT_STATE_RESET,      /* LCTR_INIT_MSG_INITIATE */
    LCTR_INIT_STATE_RESET,      /* LCTR_INIT_MSG_INITIATE_CANCEL */
    LCTR_INIT_STATE_DISABLED,   /* LCTR_INIT_MSG_TERMINATE */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Execute master scan and initiate state machine.
 *
 *  \param      event   State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstInitExecuteSm(uint8_t event)
{
  LL_TRACE_INFO2("lctrMstInitExecuteSm: state=%u, event=%u", lctrMstInit.state, event);

  if (lctrInitActionTbl[lctrMstInit.state][event])
  {
    lctrInitActionTbl[lctrMstInit.state][event]();
  }

  lctrMstInit.state = lctrInitNextStateTbl[lctrMstInit.state][event];
}
