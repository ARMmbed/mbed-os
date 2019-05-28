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
 * \brief Link layer controller master scan state machine implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_adv_master_ae.h"
#include "sch_api.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Action function call signature. */
typedef void (*lctrActFn_t)(lctrExtScanCtx_t *pExtScanCtx);

/*! \brief      Create sync action function call signature. */
typedef void (*lctrCreateSyncActFn_t)(void);

/*! \brief      Create sync action function call signature. */
typedef void (*lctrPerScanActFn_t)(lctrPerScanCtx_t *pPerScanCtx);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      State machine action table.
 */
static const lctrActFn_t lctrExtScanActionTbl[LCTR_EXT_SCAN_STATE_TOTAL][LCTR_EXT_SCAN_MSG_TOTAL] =
{
  { /* LCTR_EXT_SCAN_STATE_DISABLED */
    NULL,                           /* LCTR_EXT_SCAN_MSG_RESET */
    lctrExtScanActDiscover,         /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    lctrExtScanActScanCnf,          /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    NULL                            /* LCTR_EXT_SCAN_MSG_TERMINATE */
  },
  { /* LCTR_EXT_SCAN_STATE_DISCOVER */
    lctrExtScanActShutdown,         /* LCTR_EXT_SCAN_MSG_RESET */
    lctrExtScanActUpdateDiscover,   /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    lctrExtScanActShutdown,         /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    NULL                            /* LCTR_EXT_SCAN_MSG_TERMINATE */
  },
  { /* LCTR_EXT_SCAN_STATE_SHUTDOWN */
    NULL,                           /* LCTR_EXT_SCAN_MSG_RESET */
    lctrExtScanActDisallowScan,     /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    lctrExtScanActDisallowScan,     /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    lctrExtScanActScanTerm          /* LCTR_EXT_SCAN_MSG_TERMINATE */
  },
  { /* LCTR_EXT_SCAN_STATE_RESET */
    NULL,                           /* LCTR_EXT_SCAN_MSG_RESET */
    lctrExtScanActDisallowScan,     /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    lctrExtScanActDisallowScan,     /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    lctrExtScanActScanTerm          /* LCTR_EXT_SCAN_MSG_TERMINATE */
  }
};

/*! \brief      State machine next state table. */
static const uint8_t lctrExtScanNextStateTbl[LCTR_EXT_SCAN_STATE_TOTAL][LCTR_EXT_SCAN_MSG_TOTAL] =
{
  { /* LCTR_EXT_SCAN_STATE_DISABLED */
    LCTR_EXT_SCAN_STATE_DISABLED,   /* LCTR_EXT_SCAN_MSG_RESET */
    LCTR_EXT_SCAN_STATE_DISCOVER,   /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_EXT_SCAN_STATE_DISABLED,   /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_EXT_SCAN_STATE_DISABLED    /* LCTR_EXT_SCAN_MSG_TERMINATE */
  },
  { /* LCTR_EXT_SCAN_STATE_DISCOVER */
    LCTR_EXT_SCAN_STATE_RESET,      /* LCTR_EXT_SCAN_MSG_RESET */
    LCTR_EXT_SCAN_STATE_DISCOVER,   /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_EXT_SCAN_STATE_SHUTDOWN,   /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_EXT_SCAN_STATE_DISABLED    /* LCTR_EXT_SCAN_MSG_TERMINATE */
  },
  { /* LCTR_EXT_SCAN_STATE_SHUTDOWN */
    LCTR_EXT_SCAN_STATE_RESET,      /* LCTR_EXT_SCAN_MSG_RESET */
    LCTR_EXT_SCAN_STATE_SHUTDOWN,   /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_EXT_SCAN_STATE_SHUTDOWN,   /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_EXT_SCAN_STATE_DISABLED    /* LCTR_EXT_SCAN_MSG_TERMINATE */
  },
  { /* LCTR_EXT_SCAN_STATE_RESET */
    LCTR_EXT_SCAN_STATE_RESET,      /* LCTR_EXT_SCAN_MSG_RESET */
    LCTR_EXT_SCAN_STATE_RESET,      /* LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_EXT_SCAN_STATE_RESET,      /* LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_EXT_SCAN_STATE_DISABLED    /* LCTR_EXT_SCAN_MSG_TERMINATE */
  }
};

/*! \brief      Create sync state machine action table. */
static const lctrCreateSyncActFn_t lctrCreateSyncActionTbl[LCTR_CREATE_SYNC_STATE_TOTAL][LCTR_CREATE_SYNC_MSG_TOTAL] =
{
  { /* LCTR_CREATE_SYNC_STATE_DISABLED */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_RESET       */
    lctrCreateSyncActCreate,        /* LCTR_CREATE_SYNC_MSG_CREATE      */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_FAILED      */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_DONE        */
    NULL                            /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_CREATE_SYNC_STATE_DISCOVER */
    lctrCreateSyncActCancel,        /* LCTR_CREATE_SYNC_MSG_RESET       */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_CREATE      */  /* Handled by API. */
    lctrCreateSyncActCancel,        /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    lctrCreateSyncActFailed,        /* LCTR_CREATE_SYNC_MSG_FAILED      */
    lctrCreateSyncActDone,          /* LCTR_CREATE_SYNC_MSG_DONE        */
    NULL                            /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_CREATE_SYNC_STATE_SHUTDOWN */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_RESET       */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_CREATE      */  /* Handled by API. */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_FAILED      */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_DONE        */
    lctrCreateSyncActTerminate      /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_CREATE_SYNC_STATE_RESET */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_RESET       */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_CREATE      */  /* Handled by API. */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_FAILED      */
    NULL,                           /* LCTR_CREATE_SYNC_MSG_DONE        */
    lctrCreateSyncActTerminate      /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  }
};

/*! \brief      Create sync state machine next state table. */
static const uint8_t lctrCreateSyncNextStateTbl[LCTR_CREATE_SYNC_STATE_TOTAL][LCTR_CREATE_SYNC_MSG_TOTAL] =
{
  { /* LCTR_CREATE_SYNC_STATE_DISABLED */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_RESET       */
    LCTR_CREATE_SYNC_STATE_DISCOVER,        /* LCTR_CREATE_SYNC_MSG_CREATE      */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_FAILED      */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_DONE        */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_CREATE_SYNC_STATE_DISCOVER  */
    LCTR_CREATE_SYNC_STATE_RESET,           /* LCTR_CREATE_SYNC_MSG_RESET       */
    LCTR_CREATE_SYNC_STATE_DISCOVER,        /* LCTR_CREATE_SYNC_MSG_CREATE      */
    LCTR_CREATE_SYNC_STATE_SHUTDOWN,        /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    LCTR_CREATE_SYNC_STATE_SHUTDOWN,        /* LCTR_CREATE_SYNC_MSG_FAILED      */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_DONE        */
    LCTR_CREATE_SYNC_STATE_DISCOVER         /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_CREATE_SYNC_STATE_SHUTDOWN */
    LCTR_CREATE_SYNC_STATE_SHUTDOWN,        /* LCTR_CREATE_SYNC_MSG_RESET       */
    LCTR_CREATE_SYNC_STATE_SHUTDOWN,        /* LCTR_CREATE_SYNC_MSG_CREATE      */
    LCTR_CREATE_SYNC_STATE_SHUTDOWN,        /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    LCTR_CREATE_SYNC_STATE_SHUTDOWN,        /* LCTR_CREATE_SYNC_MSG_FAILED      */
    LCTR_CREATE_SYNC_STATE_SHUTDOWN,        /* LCTR_CREATE_SYNC_MSG_DONE        */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  },

  { /* LCTR_CREATE_SYNC_STATE_RESET    */
    LCTR_CREATE_SYNC_STATE_RESET,           /* LCTR_CREATE_SYNC_MSG_RESET       */
    LCTR_CREATE_SYNC_STATE_RESET,           /* LCTR_CREATE_SYNC_MSG_CREATE      */
    LCTR_CREATE_SYNC_STATE_RESET,           /* LCTR_CREATE_SYNC_MSG_CANCEL      */
    LCTR_CREATE_SYNC_STATE_RESET,           /* LCTR_CREATE_SYNC_MSG_FAILED      */
    LCTR_CREATE_SYNC_STATE_DISABLED,        /* LCTR_CREATE_SYNC_MSG_DONE        */
    LCTR_CREATE_SYNC_STATE_DISABLED         /* LCTR_CREATE_SYNC_MSG_TERMINATE   */
  }
};

/*! \brief      Transfer sync state machine action table. */
static const lctrCreateSyncActFn_t lctrTransferSyncActionTbl[LCTR_TRANSFER_SYNC_STATE_TOTAL][LCTR_TRANSFER_SYNC_MSG_TOTAL] =
{
  { /* LCTR_TRANSFER_SYNC_STATE_DISABLED */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_RESET       */
    lctrTransferSyncActStart,         /* LCTR_TRANSFER_SYNC_MSG_START       */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_CANCEL      */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_FAILED      */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_DONE        */
    NULL                              /* LCTR_TRANSFER_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_TRANSFER_SYNC_STATE_DISCOVER */
    lctrTransferSyncActCancel,        /* LCTR_TRANSFER_SYNC_MSG_RESET       */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_START       */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_CANCEL      */
    lctrTransferSyncActFailed,        /* LCTR_TRANSFER_SYNC_MSG_FAILED      */
    lctrTransferSyncActDone,          /* LCTR_TRANSFER_SYNC_MSG_DONE        */
    NULL                              /* LCTR_TRANSFER_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_TRANSFER_SYNC_STATE_SHUTDOWN */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_RESET       */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_START       */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_CANCEL      */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_FAILED      */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_DONE        */
    lctrTransferSyncActTerminate      /* LCTR_TRANSFER_SYNC_MSG_TERMINATE   */
  },
  { /* LCTR_TRANSFER_SYNC_STATE_RESET */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_RESET       */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_START       */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_CANCEL      */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_FAILED      */
    NULL,                             /* LCTR_TRANSFER_SYNC_MSG_DONE        */
    lctrTransferSyncActTerminate      /* LCTR_TRANSFER_SYNC_MSG_TERMINATE   */
  }
};

/*! \brief      Transfer sync state machine next state table. */
static const uint8_t lctrTransferSyncNextStateTbl[LCTR_TRANSFER_SYNC_STATE_TOTAL][LCTR_TRANSFER_SYNC_MSG_TOTAL] =
{
  { /* LCTR_TRANSFER_SYNC_STATE_DISABLED  */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_RESET     */
    LCTR_TRANSFER_SYNC_STATE_DISCOVER,      /* LCTR_TRANSFER_SYNC_MSG_START     */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_CANCEL    */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_FAILED    */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_DONE      */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_TERMINATE */
  },
  { /* LCTR_TRANSFER_SYNC_STATE_DISCOVER  */
    LCTR_TRANSFER_SYNC_STATE_RESET,         /* LCTR_TRANSFER_SYNC_MSG_RESET     */
    LCTR_TRANSFER_SYNC_STATE_DISCOVER,      /* LCTR_TRANSFER_SYNC_MSG_START     */
    LCTR_TRANSFER_SYNC_STATE_SHUTDOWN,      /* LCTR_TRANSFER_SYNC_MSG_CANCEL    */
    LCTR_TRANSFER_SYNC_STATE_SHUTDOWN,      /* LCTR_TRANSFER_SYNC_MSG_FAILED    */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_DONE      */
    LCTR_TRANSFER_SYNC_STATE_DISCOVER       /* LCTR_TRANSFER_SYNC_MSG_TERMINATE */
  },
  { /* LCTR_TRANSFER_SYNC_STATE_SHUTDOWN  */
    LCTR_TRANSFER_SYNC_STATE_SHUTDOWN,      /* LCTR_TRANSFER_SYNC_MSG_RESET     */
    LCTR_TRANSFER_SYNC_STATE_SHUTDOWN,      /* LCTR_TRANSFER_SYNC_MSG_START     */
    LCTR_TRANSFER_SYNC_STATE_SHUTDOWN,      /* LCTR_TRANSFER_SYNC_MSG_CANCEL    */
    LCTR_TRANSFER_SYNC_STATE_SHUTDOWN,      /* LCTR_TRANSFER_SYNC_MSG_FAILED    */
    LCTR_TRANSFER_SYNC_STATE_SHUTDOWN,      /* LCTR_TRANSFER_SYNC_MSG_DONE      */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_TERMINATE */
  },

  { /* LCTR_TRANSFER_SYNC_STATE_RESET     */
    LCTR_TRANSFER_SYNC_STATE_RESET,         /* LCTR_TRANSFER_SYNC_MSG_RESET     */
    LCTR_TRANSFER_SYNC_STATE_RESET,         /* LCTR_TRANSFER_SYNC_MSG_START     */
    LCTR_TRANSFER_SYNC_STATE_RESET,         /* LCTR_TRANSFER_SYNC_MSG_CANCEL    */
    LCTR_TRANSFER_SYNC_STATE_RESET,         /* LCTR_TRANSFER_SYNC_MSG_FAILED    */
    LCTR_TRANSFER_SYNC_STATE_DISABLED,      /* LCTR_TRANSFER_SYNC_MSG_DONE      */
    LCTR_TRANSFER_SYNC_STATE_DISABLED       /* LCTR_TRANSFER_SYNC_MSG_TERMINATE */
  }
};

/*! \brief      Periodic advertising state machine action table. */
static const lctrPerScanActFn_t lctrPerScanActionTbl[LCTR_PER_SCAN_STATE_TOTAL][LCTR_PER_SCAN_MSG_TOTAL] =
{
  { /* LCTR_PER_SCAN_STATE_DISABLE */
    NULL,                           /* LCTR_PER_SCAN_MSG_RESET          */
    lctrPerScanActSyncEstd,         /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    NULL,                           /* LCTR_PER_SCAN_MSG_TERMINATE      */
    NULL,                           /* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    NULL                            /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  },
  { /* LCTR_PER_SCAN_STATE_SYNC_ESTD  */
    lctrPerScanActSyncTerminate,    /* LCTR_PER_SCAN_MSG_RESET          */
    NULL,                           /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    lctrPerScanActSyncTerminate,    /* LCTR_PER_SCAN_MSG_TERMINATE      */
    NULL,                           /* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    lctrPerScanActSyncTimeout       /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  },
  { /* LCTR_PER_SCAN_STATE_SYNC_TERMINATE  */
    NULL,                           /* LCTR_PER_SCAN_MSG_RESET          */
    NULL,                           /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    NULL,                           /* LCTR_PER_SCAN_MSG_TERMINATE      */
    lctrPerScanActSyncTerminateDone,/* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    NULL                            /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  },
  { /* LCTR_PER_SCAN_STATE_RESET    */
    NULL,                           /* LCTR_PER_SCAN_MSG_RESET          */
    NULL,                           /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    NULL,                           /* LCTR_PER_SCAN_MSG_TERMINATE      */
    lctrPerScanActSyncTerminateDone,/* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    NULL                            /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  }
};

/*! \brief      Periodic advertising state machine next state table. */
static const uint8_t lctrPerScanNextStateTbl[LCTR_PER_SCAN_STATE_TOTAL][LCTR_PER_SCAN_MSG_TOTAL] =
{
  { /* LCTR_PER_SCAN_STATE_DISABLE */
    LCTR_PER_SCAN_STATE_DISABLE,             /* LCTR_PER_SCAN_MSG_RESET          */
    LCTR_PER_SCAN_STATE_SYNC_ESTD,           /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    LCTR_PER_SCAN_STATE_DISABLE,             /* LCTR_PER_SCAN_MSG_TERMINATE      */
    LCTR_PER_SCAN_STATE_DISABLE,             /* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    LCTR_PER_SCAN_STATE_DISABLE              /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  },
  { /* LCTR_PER_SCAN_STATE_SYNC_ESTD  */
    LCTR_PER_SCAN_STATE_RESET,               /* LCTR_PER_SCAN_MSG_RESET          */
    LCTR_PER_SCAN_STATE_SYNC_ESTD,           /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    LCTR_PER_SCAN_STATE_SYNC_TERMINATE,      /* LCTR_PER_SCAN_MSG_TERMINATE      */
    LCTR_PER_SCAN_STATE_SYNC_ESTD,           /* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    LCTR_PER_SCAN_STATE_SYNC_TERMINATE       /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  },
  { /* LCTR_PER_SCAN_STATE_SYNC_TERMINATE */
    LCTR_PER_SCAN_STATE_RESET,               /* LCTR_PER_SCAN_MSG_RESET          */
    LCTR_PER_SCAN_STATE_SYNC_TERMINATE,      /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    LCTR_PER_SCAN_STATE_SYNC_TERMINATE,      /* LCTR_PER_SCAN_MSG_TERMINATE      */
    LCTR_PER_SCAN_STATE_DISABLE,             /* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    LCTR_PER_SCAN_STATE_SYNC_TERMINATE       /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  },

  { /* LCTR_PER_SCAN_STATE_RESET    */
    LCTR_PER_SCAN_STATE_RESET,               /* LCTR_PER_SCAN_MSG_RESET          */
    LCTR_PER_SCAN_STATE_RESET,               /* LCTR_PER_SCAN_MSG_SYNC_ESTD      */
    LCTR_PER_SCAN_STATE_RESET,               /* LCTR_PER_SCAN_MSG_TERMINATE      */
    LCTR_PER_SCAN_STATE_DISABLE,             /* LCTR_PER_SCAN_MSG_TERMINATE_DONE */
    LCTR_PER_SCAN_STATE_RESET                /* LCTR_PER_SCAN_SUP_TIMEOUT        */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Execute master scan state machine.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *  \param      event           State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstExtScanExecuteSm(lctrExtScanCtx_t *pExtScanCtx, uint8_t event)
{
  LL_TRACE_INFO3("lctrMstExtScanExecuteSm: phy=%u, state=%u, event=%u", LCTR_GET_EXT_SCAN_HANDLE(pExtScanCtx), pExtScanCtx->state, event);

  if (lctrExtScanActionTbl[pExtScanCtx->state][event])
  {
    lctrExtScanActionTbl[pExtScanCtx->state][event](pExtScanCtx);
  }

  pExtScanCtx->state = lctrExtScanNextStateTbl[pExtScanCtx->state][event];
}

/*************************************************************************************************/
/*!
 *  \brief      Execute master create sync state machine.
 *
 *  \param      event       State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstCreateSyncExecuteSm(uint8_t event)
{
  LL_TRACE_INFO2("lctrMstCreateSyncExecuteSm: state=%u, event=%u", lctrPerCreateSync.state, event);

  if (lctrCreateSyncActionTbl[lctrPerCreateSync.state][event])
  {
    lctrCreateSyncActionTbl[lctrPerCreateSync.state][event]();
  }

  lctrPerCreateSync.state = lctrCreateSyncNextStateTbl[lctrPerCreateSync.state][event];
}

/*************************************************************************************************/
/*!
 *  \brief      Execute master transfer sync state machine.
 *
 *  \param      event       State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstTransferSyncExecuteSm(uint8_t event)
{
  LL_TRACE_INFO2("lctrMstTransferSyncExecuteSm: state=%u, event=%u", lctrPerTransferSync.state, event);

  if (lctrTransferSyncActionTbl[lctrPerTransferSync.state][event])
  {
    lctrTransferSyncActionTbl[lctrPerTransferSync.state][event]();
  }

  lctrPerTransferSync.state = lctrTransferSyncNextStateTbl[lctrPerTransferSync.state][event];
}

/*************************************************************************************************/
/*!
 *  \brief      Execute master periodic scanning state machine.
 *
 *  \param      pPerScanCtx   Periodic scanning context.
 *  \param      event         State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstPerScanExecuteSm(lctrPerScanCtx_t *pPerScanCtx, uint8_t event)
{
  LL_TRACE_INFO2("lctrMstPerScanExecuteSm: state=%u, event=%u", pPerScanCtx->state, event);

  if (lctrPerScanActionTbl[pPerScanCtx->state][event])
  {
    lctrPerScanActionTbl[pPerScanCtx->state][event](pPerScanCtx);
  }

  pPerScanCtx->state = lctrPerScanNextStateTbl[pPerScanCtx->state][event];
}
