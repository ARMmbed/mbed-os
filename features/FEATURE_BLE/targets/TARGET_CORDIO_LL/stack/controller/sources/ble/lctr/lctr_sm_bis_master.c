/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller BIG state machine implementation file.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include "lctr_int_bis_master.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended action function call signature. */
typedef void (*lctrMstBigActFn_t)(lctrBigCtx_t *pBigCtx);

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*! \brief      BIG master state machine action table. */
static const lctrMstBigActFn_t lctrMstBigActionTbl[LCTR_MST_BIG_STATE_TOTAL][LCTR_MST_BIG_MSG_TOTAL] =
{
  { /* LCTR_MST_BIG_STATE_DISABLED */
    NULL,                           /* LCTR_MST_BIG_MSG_RESET */
    lctrMstBigActStart,             /* LCTR_MST_BIG_API_CREATE_SYNC */
    NULL,                           /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    NULL,                           /* LCTR_MST_BIG_ACAD_BIG_INFO */
    NULL,                           /* LCTR_MST_BIG_PDU_TERM */
    NULL,                           /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    NULL,                           /* LCTR_MST_BIG_INT_MIC_FAILED */
    NULL                            /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_MST_BIG_STATE_SYNCING */
    lctrMstBigActShutdown,          /* LCTR_MST_BIG_MSG_RESET */
    NULL,                           /* LCTR_MST_BIG_API_CREATE_SYNC */
    lctrMstBigActShutdown,          /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    lctrMstBigActBigSync,           /* LCTR_MST_BIG_ACAD_BIG_INFO */
    NULL,                           /* LCTR_MST_BIG_PDU_TERM */
    NULL,                           /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    NULL,                           /* LCTR_MST_BIG_INT_MIC_FAILED */
    NULL                            /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_MST_BIG_STATE_SYNCED */
    lctrMstBigActShutdown,          /* LCTR_MST_BIG_MSG_RESET */
    NULL,                           /* LCTR_MST_BIG_API_CREATE_SYNC */
    lctrMstBigActShutdown,          /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    NULL,                           /* LCTR_MST_BIG_ACAD_BIG_INFO */
    lctrMstBigActTerm,              /* LCTR_MST_BIG_PDU_TERM */
    lctrMstBigActSyncLost,          /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    lctrMstBigActMicFailed,         /* LCTR_MST_BIG_INT_MIC_FAILED */
    lctrMstBigActCleanup            /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_MST_BIG_STATE_SHUTDOWN */
    NULL,                           /* LCTR_MST_BIG_MSG_RESET */
    NULL,                           /* LCTR_MST_BIG_API_CREATE_SYNC */
    NULL,                           /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    NULL,                           /* LCTR_MST_BIG_ACAD_BIG_INFO */
    NULL,                           /* LCTR_MST_BIG_PDU_TERM */
    NULL,                           /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    NULL,                           /* LCTR_MST_BIG_INT_MIC_FAILED */
    lctrMstBigActCleanup            /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_EXT_ADV_STATE_RESET */
    NULL,                           /* LCTR_MST_BIG_MSG_RESET */
    NULL,                           /* LCTR_MST_BIG_API_CREATE_SYNC */
    NULL,                           /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    NULL,                           /* LCTR_MST_BIG_ACAD_BIG_INFO */
    NULL,                           /* LCTR_MST_BIG_PDU_TERM */
    NULL,                           /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    NULL,                           /* LCTR_MST_BIG_INT_MIC_FAILED */
    lctrMstBigActCleanup            /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  }
};

/*! \brief      BIG master state machine next state table. */
static const uint8_t lctrMstBigNextStateTbl[LCTR_MST_BIG_STATE_TOTAL][LCTR_MST_BIG_MSG_TOTAL] =
{
  { /* LCTR_MST_BIG_STATE_DISABLED */
    LCTR_MST_BIG_STATE_DISABLED,    /* LCTR_MST_BIG_MSG_RESET */
    LCTR_MST_BIG_STATE_SYNCING,     /* LCTR_MST_BIG_API_CREATE_SYNC */
    LCTR_MST_BIG_STATE_DISABLED,    /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    LCTR_MST_BIG_STATE_DISABLED,    /* LCTR_MST_BIG_ACAD_BIG_INFO */
    LCTR_MST_BIG_STATE_DISABLED,    /* LCTR_MST_BIG_PDU_TERM */
    LCTR_MST_BIG_STATE_DISABLED,    /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    LCTR_MST_BIG_STATE_DISABLED,    /* LCTR_MST_BIG_INT_MIC_FAILED */
    LCTR_MST_BIG_STATE_DISABLED     /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_MST_BIG_STATE_SYNCING */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_MSG_RESET */
    LCTR_MST_BIG_STATE_SYNCING,     /* LCTR_MST_BIG_API_CREATE_SYNC */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    LCTR_MST_BIG_STATE_SYNCED,      /* LCTR_MST_BIG_ACAD_BIG_INFO */
    LCTR_MST_BIG_STATE_SYNCING,     /* LCTR_MST_BIG_PDU_TERM */
    LCTR_MST_BIG_STATE_SYNCING,     /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_INT_MIC_FAILED */
    LCTR_MST_BIG_STATE_SYNCING      /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_MST_BIG_STATE_SYNCED */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_MSG_RESET */
    LCTR_MST_BIG_STATE_SYNCED,      /* LCTR_MST_BIG_API_CREATE_SYNC */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    LCTR_MST_BIG_STATE_SYNCED,      /* LCTR_MST_BIG_ACAD_BIG_INFO */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_PDU_TERM */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_INT_MIC_FAILED */
    LCTR_MST_BIG_STATE_DISABLED     /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_MST_BIG_STATE_SHUTDOWN */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_MSG_RESET */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_API_CREATE_SYNC */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_ACAD_BIG_INFO */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_PDU_TERM */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    LCTR_MST_BIG_STATE_SHUTDOWN,    /* LCTR_MST_BIG_INT_MIC_FAILED */
    LCTR_MST_BIG_STATE_SHUTDOWN     /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  },
  { /* LCTR_MST_BIG_STATE_RESET */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_MSG_RESET */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_API_CREATE_SYNC */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_API_TERMINATE_SYNC */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_ACAD_BIG_INFO */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_PDU_TERM */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_INT_SYNC_TIMEOUT */
    LCTR_MST_BIG_STATE_RESET,       /* LCTR_MST_BIG_INT_MIC_FAILED */
    LCTR_MST_BIG_STATE_RESET        /* LCTR_MST_BIG_INT_TERMINATED_SYNC */
  }
};

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Execute BIG main state machine.
 *
 *  \param      pBigCtx         BIG context.
 *  \param      event           State machine event.
 *************************************************************************************************/
void lctrMstBigExecuteSm(lctrBigCtx_t *pBigCtx, LctrMstBigMsg_t event)
{
  LL_TRACE_INFO3("lctrMstBigExecuteSm: bigHandle=%u, state=%u, event=%u", pBigCtx->handle, pBigCtx->state, event);

  if (lctrMstBigActionTbl[pBigCtx->state][event])
  {
    lctrMstBigActionTbl[pBigCtx->state][event](pBigCtx);
  }

  pBigCtx->state = lctrMstBigNextStateTbl[pBigCtx->state][event];
}
