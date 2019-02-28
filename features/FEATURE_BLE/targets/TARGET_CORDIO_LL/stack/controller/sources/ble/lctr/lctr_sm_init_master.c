/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master initiate state machine implementation file.
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
