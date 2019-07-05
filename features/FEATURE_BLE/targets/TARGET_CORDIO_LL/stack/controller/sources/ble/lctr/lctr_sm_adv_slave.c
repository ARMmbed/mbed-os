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
 * \brief Link layer controller slave advertising state machine implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_adv_slave.h"
#include "sch_api.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Action function call signature. */
typedef void (*lctrActFn_t)(void);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      State machine action table. */
static const lctrActFn_t lctrAdvActionTbl[LCTR_ADV_STATE_TOTAL][LCTR_ADV_MSG_TOTAL] =
{
  { /* LCTR_ADV_STATE_DISABLED */
    NULL,                       /* LCTR_ADV_MSG_RESET     */
    lctrAdvActStart,            /* LCTR_ADV_MSG_START     */
    lctrAdvActAdvCnf,           /* LCTR_ADV_MSG_STOP      */
    lctrAdvActSelfStart,        /* LCTR_ADV_MSG_INT_START */
    NULL,                       /* LCTR_ADV_MSG_TERMINATE */
    lctrAdvActUpdateAdvParam    /* LCTR_ADV_MSG_PARAM_UPD */
  },
  { /* LCTR_ADV_STATE_ENABLED  */
    lctrAdvActShutdown,         /* LCTR_ADV_MSG_RESET     */
    lctrAdvActAdvCnf,           /* LCTR_ADV_MSG_START     */
    lctrAdvActShutdown,         /* LCTR_ADV_MSG_STOP      */
    NULL,                       /* LCTR_ADV_MSG_INT_START */
    lctrAdvActSelfTerm,         /* LCTR_ADV_MSG_TERMINATE */
    NULL                        /* LCTR_ADV_MSG_PARAM_UPD */
  },
  { /* LCTR_ADV_STATE_SHUTDOWN */
    NULL,                       /* LCTR_ADV_MSG_RESET     */
    lctrAdvActDisallowAdvCnf,   /* LCTR_ADV_MSG_START     */
    lctrAdvActDisallowAdvCnf,   /* LCTR_ADV_MSG_STOP      */
    NULL,                       /* LCTR_ADV_MSG_INT_START */
    lctrAdvActAdvTerm,          /* LCTR_ADV_MSG_TERMINATE */
    lctrAdvActUpdateAdvParam    /* LCTR_ADV_MSG_PARAM_UPD */
  },
  { /* LCTR_ADV_STATE_RESET    */
    NULL,                       /* LCTR_ADV_MSG_RESET     */
    lctrAdvActDisallowAdvCnf,   /* LCTR_ADV_MSG_START     */
    lctrAdvActDisallowAdvCnf,   /* LCTR_ADV_MSG_STOP      */
    NULL,                       /* LCTR_ADV_MSG_INT_START */
    lctrAdvActResetTerm,        /* LCTR_ADV_MSG_TERMINATE */
    lctrAdvActUpdateAdvParam    /* LCTR_ADV_MSG_PARAM_UPD */
  }
};

/*! \brief      State machine next state table. */
static const uint8_t lctrAdvNextStateTbl[LCTR_ADV_STATE_TOTAL][LCTR_ADV_MSG_TOTAL] =
{
  { /* LCTR_ADV_STATE_DISABLED */
    LCTR_ADV_STATE_DISABLED,    /* LCTR_ADV_MSG_RESET     */
    LCTR_ADV_STATE_ENABLED,     /* LCTR_ADV_MSG_START     */
    LCTR_ADV_STATE_DISABLED,    /* LCTR_ADV_MSG_STOP      */
    LCTR_ADV_STATE_ENABLED,     /* LCTR_ADV_MSG_INT_START */
    LCTR_ADV_STATE_DISABLED,    /* LCTR_ADV_MSG_TERMINATE */
    LCTR_ADV_STATE_DISABLED     /* LCTR_ADV_MSG_PARAM_UPD */
  },
  { /* LCTR_ADV_STATE_ENABLED  */
    LCTR_ADV_STATE_RESET,       /* LCTR_ADV_MSG_RESET     */
    LCTR_ADV_STATE_ENABLED,     /* LCTR_ADV_MSG_START     */
    LCTR_ADV_STATE_SHUTDOWN,    /* LCTR_ADV_MSG_STOP      */
    LCTR_ADV_STATE_ENABLED,     /* LCTR_ADV_MSG_INT_START */
    LCTR_ADV_STATE_DISABLED,    /* LCTR_ADV_MSG_TERMINATE */
    LCTR_ADV_STATE_ENABLED      /* LCTR_ADV_MSG_PARAM_UPD */
  },
  { /* LCTR_ADV_STATE_SHUTDOWN */
    LCTR_ADV_STATE_RESET,       /* LCTR_ADV_MSG_RESET     */
    LCTR_ADV_STATE_SHUTDOWN,    /* LCTR_ADV_MSG_START     */
    LCTR_ADV_STATE_SHUTDOWN,    /* LCTR_ADV_MSG_STOP      */
    LCTR_ADV_STATE_SHUTDOWN,    /* LCTR_ADV_MSG_INT_START */
    LCTR_ADV_STATE_DISABLED,    /* LCTR_ADV_MSG_TERMINATE */
    LCTR_ADV_STATE_SHUTDOWN     /* LCTR_ADV_MSG_PARAM_UPD */
  },
  { /* LCTR_ADV_STATE_RESET    */
    LCTR_ADV_STATE_RESET,       /* LCTR_ADV_MSG_RESET     */
    LCTR_ADV_STATE_RESET,       /* LCTR_ADV_MSG_START     */
    LCTR_ADV_STATE_RESET,       /* LCTR_ADV_MSG_STOP      */
    LCTR_ADV_STATE_RESET,       /* LCTR_ADV_MSG_INT_START */
    LCTR_ADV_STATE_DISABLED,    /* LCTR_ADV_MSG_TERMINATE */
    LCTR_ADV_STATE_RESET        /* LCTR_ADV_MSG_PARAM_UPD */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Execute slave advertising state machine.
 *
 *  \param      event   State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSlvAdvExecuteSm(uint8_t event)
{
  LL_TRACE_INFO2("lctrSlvAdvExecuteSm: state=%u, event=%u", lctrSlvAdv.state, event);

  if (lctrAdvActionTbl[lctrSlvAdv.state][event])
  {
    lctrAdvActionTbl[lctrSlvAdv.state][event]();
  }

  lctrSlvAdv.state = lctrAdvNextStateTbl[lctrSlvAdv.state][event];
}
