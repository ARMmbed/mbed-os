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

#include "lctr_int_adv_slave_ae.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended action function call signature. */
typedef void (*lctrExtActFn_t)(lctrAdvSet_t *pAdvSet);

/*! \brief      Periodic action function call signature. */
typedef void (*lctrPerActFn_t)(lctrAdvSet_t *pAdvSet);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Extended advertising state machine action table. */
static const lctrExtActFn_t lctrExtAdvActionTbl[LCTR_EXT_ADV_STATE_TOTAL][LCTR_EXT_ADV_MSG_TOTAL] =
{
  { /* LCTR_EXT_ADV_STATE_DISABLED */
    lctrExtAdvActReset,             /* LCTR_EXT_ADV_MSG_RESET     */
    lctrExtAdvActStart,             /* LCTR_EXT_ADV_MSG_START     */
    lctrExtAdvActAdvCnf,            /* LCTR_EXT_ADV_MSG_STOP      */
    NULL,                           /* LCTR_EXT_ADV_MSG_INT_START */
    NULL,                           /* LCTR_EXT_ADV_MSG_TERMINATE */
    NULL,                           /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  },
  { /* LCTR_EXT_ADV_STATE_ENABLED  */
    lctrExtAdvActShutdown,          /* LCTR_EXT_ADV_MSG_RESET     */
    lctrExtAdvActRestart,           /* LCTR_EXT_ADV_MSG_START     */
    lctrExtAdvActShutdown,          /* LCTR_EXT_ADV_MSG_STOP      */
    NULL,                           /* LCTR_EXT_ADV_MSG_INT_START */
    lctrExtAdvActSelfTerm,          /* LCTR_EXT_ADV_MSG_TERMINATE */
    lctrExtAdvActDurationExpired,   /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  },
  { /* LCTR_EXT_ADV_STATE_SHUTDOWN */
    NULL,                           /* LCTR_EXT_ADV_MSG_RESET     */
    lctrExtAdvActDisallowAdvCnf,    /* LCTR_EXT_ADV_MSG_START     */
    lctrExtAdvActDisallowAdvCnf,    /* LCTR_EXT_ADV_MSG_STOP      */
    NULL,                           /* LCTR_EXT_ADV_MSG_INT_START */
    lctrExtAdvActAdvTerm,           /* LCTR_EXT_ADV_MSG_TERMINATE */
    NULL,                           /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  },
  { /* LCTR_EXT_ADV_STATE_RESET    */
    NULL,                           /* LCTR_EXT_ADV_MSG_RESET     */
    lctrExtAdvActDisallowAdvCnf,    /* LCTR_EXT_ADV_MSG_START     */
    lctrExtAdvActDisallowAdvCnf,    /* LCTR_EXT_ADV_MSG_STOP      */
    NULL,                           /* LCTR_EXT_ADV_MSG_INT_START */
    lctrExtAdvActResetTerm,         /* LCTR_EXT_ADV_MSG_TERMINATE */
    NULL,                           /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  }
};

/*! \brief      Extended advertising state machine next state table. */
static const uint8_t lctrExtAdvNextStateTbl[LCTR_EXT_ADV_STATE_TOTAL][LCTR_EXT_ADV_MSG_TOTAL] =
{
  { /* LCTR_EXT_ADV_STATE_DISABLED */
    LCTR_EXT_ADV_STATE_DISABLED,    /* LCTR_EXT_ADV_MSG_RESET     */
    LCTR_EXT_ADV_STATE_ENABLED,     /* LCTR_EXT_ADV_MSG_START     */
    LCTR_EXT_ADV_STATE_DISABLED,    /* LCTR_EXT_ADV_MSG_STOP      */
    LCTR_EXT_ADV_STATE_ENABLED,     /* LCTR_EXT_ADV_MSG_INT_START */
    LCTR_EXT_ADV_STATE_DISABLED,    /* LCTR_EXT_ADV_MSG_TERMINATE */
    LCTR_EXT_ADV_STATE_DISABLED     /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  },
  { /* LCTR_EXT_ADV_STATE_ENABLED  */
    LCTR_EXT_ADV_STATE_RESET,       /* LCTR_EXT_ADV_MSG_RESET     */
    LCTR_EXT_ADV_STATE_ENABLED,     /* LCTR_EXT_ADV_MSG_START     */
    LCTR_EXT_ADV_STATE_SHUTDOWN,    /* LCTR_EXT_ADV_MSG_STOP      */
    LCTR_EXT_ADV_STATE_ENABLED,     /* LCTR_EXT_ADV_MSG_INT_START */
    LCTR_EXT_ADV_STATE_DISABLED,    /* LCTR_EXT_ADV_MSG_TERMINATE */
    LCTR_EXT_ADV_STATE_ENABLED      /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  },
  { /* LCTR_EXT_ADV_STATE_SHUTDOWN */
    LCTR_EXT_ADV_STATE_RESET,       /* LCTR_EXT_ADV_MSG_RESET     */
    LCTR_EXT_ADV_STATE_SHUTDOWN,    /* LCTR_EXT_ADV_MSG_START     */
    LCTR_EXT_ADV_STATE_SHUTDOWN,    /* LCTR_EXT_ADV_MSG_STOP      */
    LCTR_EXT_ADV_STATE_SHUTDOWN,    /* LCTR_EXT_ADV_MSG_INT_START */
    LCTR_EXT_ADV_STATE_DISABLED,    /* LCTR_EXT_ADV_MSG_TERMINATE */
    LCTR_EXT_ADV_STATE_SHUTDOWN     /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  },
  { /* LCTR_EXT_ADV_STATE_RESET    */
    LCTR_EXT_ADV_STATE_RESET,       /* LCTR_EXT_ADV_MSG_RESET     */
    LCTR_EXT_ADV_STATE_RESET,       /* LCTR_EXT_ADV_MSG_START     */
    LCTR_EXT_ADV_STATE_RESET,       /* LCTR_EXT_ADV_MSG_STOP      */
    LCTR_EXT_ADV_STATE_RESET,       /* LCTR_EXT_ADV_MSG_INT_START */
    LCTR_EXT_ADV_STATE_DISABLED,    /* LCTR_EXT_ADV_MSG_TERMINATE */
    LCTR_EXT_ADV_STATE_RESET        /* LCTR_EXT_ADV_MSG_TMR_DUR_EXP */
  }
};

/*! \brief      Periodic advertising state machine action table. */
static const lctrPerActFn_t lctrPerAdvActionTbl[LCTR_PER_ADV_STATE_TOTAL][LCTR_PER_ADV_MSG_TOTAL] =
{
  { /* LCTR_PER_ADV_STATE_DISABLED */
    NULL,                           /* LCTR_PER_ADV_MSG_RESET     */
    lctrPeriodicAdvActStart,        /* LCTR_PER_ADV_MSG_START     */
    NULL,                           /* LCTR_PER_ADV_MSG_STOP      */
    NULL                            /* LCTR_PER_ADV_MSG_TERMINATE */
  },
  { /* LCTR_PER_ADV_STATE_ENABLED  */
    lctrPeriodicAdvActShutdown,     /* LCTR_PER_ADV_MSG_RESET     */
    lctrPeriodicAdvActUpdate,       /* LCTR_PER_ADV_MSG_START     */
    lctrPeriodicAdvActShutdown,     /* LCTR_PER_ADV_MSG_STOP      */
    NULL,                           /* LCTR_PER_ADV_MSG_TERMINATE */
  },
  { /* LCTR_PER_ADV_STATE_SHUTDOWN */
    NULL,                           /* LCTR_PER_ADV_MSG_RESET     */
    lctrPeriodicAdvActDisallowAdvCnf,/* LCTR_PER_ADV_MSG_START     */
    lctrPeriodicAdvActDisallowAdvCnf,/* LCTR_PER_ADV_MSG_STOP      */
    lctrPeriodicAdvActAdvTerm        /* LCTR_PER_ADV_MSG_TERMINATE */
  },
  { /* LCTR_PER_ADV_STATE_RESET    */
    NULL,                           /* LCTR_PER_ADV_MSG_RESET     */
    lctrPeriodicAdvActDisallowAdvCnf,/* LCTR_PER_ADV_MSG_START     */
    lctrPeriodicAdvActDisallowAdvCnf,/* LCTR_PER_ADV_MSG_STOP      */
    lctrPeriodicAdvActResetTerm     /* LCTR_PER_ADV_MSG_TERMINATE */
  }
};

/*! \brief      Periodic advertising state machine next state table. */
static const uint8_t lctrPerAdvNextStateTbl[LCTR_PER_ADV_STATE_TOTAL][LCTR_PER_ADV_MSG_TOTAL] =
{
  { /* LCTR_PER_ADV_STATE_DISABLED */
    LCTR_PER_ADV_STATE_DISABLED,   /* LCTR_PER_ADV_MSG_RESET     */
    LCTR_PER_ADV_STATE_ENABLED,    /* LCTR_PER_ADV_MSG_START     */
    LCTR_PER_ADV_STATE_DISABLED,   /* LCTR_PER_ADV_MSG_STOP      */
    LCTR_PER_ADV_STATE_DISABLED    /* LCTR_PER_ADV_MSG_TERMINATE */
  },
  { /* LCTR_PER_ADV_STATE_ENABLED  */
    LCTR_PER_ADV_STATE_RESET,      /* LCTR_PER_ADV_MSG_RESET     */
    LCTR_PER_ADV_STATE_ENABLED,    /* LCTR_PER_ADV_MSG_START     */
    LCTR_PER_ADV_STATE_SHUTDOWN,   /* LCTR_PER_ADV_MSG_STOP      */
    LCTR_PER_ADV_STATE_DISABLED    /* LCTR_PER_ADV_MSG_TERMINATE */
  },
  { /* LCTR_PER_ADV_STATE_SHUTDOWN */
    LCTR_PER_ADV_STATE_RESET,      /* LCTR_PER_ADV_MSG_RESET     */
    LCTR_PER_ADV_STATE_SHUTDOWN,   /* LCTR_PER_ADV_MSG_START     */
    LCTR_PER_ADV_STATE_SHUTDOWN,   /* LCTR_PER_ADV_MSG_STOP      */
    LCTR_PER_ADV_STATE_DISABLED    /* LCTR_PER_ADV_MSG_TERMINATE */
  },
  { /* LCTR_PER_ADV_STATE_RESET    */
    LCTR_PER_ADV_STATE_RESET,      /* LCTR_PER_ADV_MSG_RESET     */
    LCTR_PER_ADV_STATE_RESET,      /* LCTR_PER_ADV_MSG_START     */
    LCTR_PER_ADV_STATE_RESET,      /* LCTR_PER_ADV_MSG_STOP      */
    LCTR_PER_ADV_STATE_DISABLED    /* LCTR_PER_ADV_MSG_TERMINATE */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Execute slave extended advertising state machine.
 *
 *  \param      pAdvSet     Advertising set.
 *  \param      event       State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSlvExtAdvExecuteSm(lctrAdvSet_t *pAdvSet, uint8_t event)
{
  LL_TRACE_INFO3("lctrSlvExtAdvExecuteSm: handle=%u, state=%u, event=%u", pAdvSet->handle, pAdvSet->state, event);

  if (lctrExtAdvActionTbl[pAdvSet->state][event])
  {
    lctrExtAdvActionTbl[pAdvSet->state][event](pAdvSet);
  }

  pAdvSet->state = lctrExtAdvNextStateTbl[pAdvSet->state][event];
}

/*************************************************************************************************/
/*!
 *  \brief      Execute slave periodic advertising state machine.
 *
 *  \param      pAdvSet     Advertising set.
 *  \param      event       State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSlvPeriodicAdvExecuteSm(lctrAdvSet_t *pAdvSet, uint8_t event)
{
  LL_TRACE_INFO3("lctrSlvPeriodicAdvExecuteSm: handle=%u, state=%u, event=%u", pAdvSet->handle, pAdvSet->perParam.perState, event);

  if (lctrPerAdvActionTbl[pAdvSet->perParam.perState][event])
  {
    lctrPerAdvActionTbl[pAdvSet->perParam.perState][event](pAdvSet);
  }

  pAdvSet->perParam.perState = lctrPerAdvNextStateTbl[pAdvSet->perParam.perState][event];
}
