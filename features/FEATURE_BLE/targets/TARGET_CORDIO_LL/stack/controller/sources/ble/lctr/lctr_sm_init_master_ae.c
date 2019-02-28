/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master initiate state machine implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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
