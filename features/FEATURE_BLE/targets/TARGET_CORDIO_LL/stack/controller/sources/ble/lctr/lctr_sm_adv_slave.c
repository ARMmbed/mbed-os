/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller slave advertising state machine implementation file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
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
