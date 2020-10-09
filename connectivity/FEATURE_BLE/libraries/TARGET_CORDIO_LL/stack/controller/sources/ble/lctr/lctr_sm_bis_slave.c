/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller BIG state machine implementation file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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

#include "lctr_int_bis_slave.h"
#include "sch_api.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended action function call signature. */
typedef void (*lctrSlvBigActFn_t)(lctrBigCtx_t *pBigCtx);

/**************************************************************************************************
  Local Functions
**************************************************************************************************/

/*! \brief      BIS slave state machine action table. */
static const lctrSlvBigActFn_t lctrSlvBigActionTbl[LCTR_SLV_BIG_STATE_TOTAL][LCTR_SLV_BIG_MSG_TOTAL] =
{
  { /* LCTR_SLV_BIG_STATE_DISABLED */
    NULL,                           /* LCTR_SLV_BIG_MSG_RESET */
    lctrSlvBigActStart,             /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    NULL,                           /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    NULL,                           /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    NULL                            /* LCTR_SLV_BIG_MSG_TERMINATED */
  },
  { /* LCTR_SLV_BIG_STATE_ENABLED */
    lctrSlvBigActShutdown,          /* LCTR_SLV_BIG_MSG_RESET */
    NULL,                           /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    lctrSlvBigActSendChMapUpd,      /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    lctrSlvBigActSendTerm,          /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    NULL                            /* LCTR_SLV_BIG_MSG_TERMINATED */
  },
  { /* LCTR_SLV_BIG_STATE_SHUTDOWN */
    NULL,                           /* LCTR_SLV_BIG_MSG_RESET */
    NULL,                           /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    NULL,                           /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    lctrSlvBigActSendTerm,          /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    lctrSlvBigActCleanup            /* LCTR_SLV_BIG_MSG_TERMINATED */
  },
  { /* LCTR_EXT_ADV_STATE_RESET */
    NULL,                           /* LCTR_SLV_BIG_MSG_RESET */
    NULL,                           /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    NULL,                           /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    NULL,                           /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    lctrSlvBigActCleanup            /* LCTR_SLV_BIG_MSG_TERMINATED */
  }
};

/*! \brief      BIS slave state machine next state table. */
static const uint8_t lctrSlvBigNextStateTbl[LCTR_SLV_BIG_STATE_TOTAL][LCTR_SLV_BIG_MSG_TOTAL] =
{
  { /* LCTR_SLV_BIG_STATE_DISABLED */
    LCTR_SLV_BIG_STATE_DISABLED,    /* LCTR_SLV_BIG_MSG_RESET */
    LCTR_SLV_BIG_STATE_ENABLED,     /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    LCTR_SLV_BIG_STATE_DISABLED,    /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    LCTR_SLV_BIG_STATE_DISABLED,    /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    LCTR_SLV_BIG_STATE_DISABLED     /* LCTR_SLV_BIG_MSG_TERMINATED */
  },
  { /* LCTR_SLV_BIG_STATE_ENABLED */
    LCTR_SLV_BIG_STATE_RESET,       /* LCTR_SLV_BIG_MSG_RESET */
    LCTR_SLV_BIG_STATE_ENABLED,     /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    LCTR_SLV_BIG_STATE_ENABLED,     /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    LCTR_SLV_BIG_STATE_SHUTDOWN,    /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    LCTR_SLV_BIG_STATE_DISABLED,    /* LCTR_SLV_BIG_MSG_TERMINATED */
  },
  { /* LCTR_SLV_BIG_STATE_SHUTDOWN */
    LCTR_SLV_BIG_STATE_RESET,       /* LCTR_SLV_BIG_MSG_RESET */
    LCTR_SLV_BIG_STATE_SHUTDOWN,    /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    LCTR_SLV_BIG_STATE_SHUTDOWN,    /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    LCTR_SLV_BIG_STATE_SHUTDOWN,    /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    LCTR_SLV_BIG_STATE_DISABLED     /* LCTR_SLV_BIG_MSG_TERMINATED */
  },
  { /* LCTR_SLV_BIG_STATE_RESET */
    LCTR_SLV_BIG_STATE_RESET,       /* LCTR_SLV_BIG_MSG_RESET */
    LCTR_SLV_BIG_STATE_RESET,       /* LCTR_SLV_BIG_MSG_CREATE_BIG */
    LCTR_SLV_BIG_STATE_RESET,       /* LCTR_SLV_BIG_MSG_CH_MAP_UPD */
    LCTR_SLV_BIG_STATE_RESET,       /* LCTR_SLV_BIG_MSG_TERMINATE_BIG */
    LCTR_SLV_BIG_STATE_DISABLED     /* LCTR_SLV_BIG_MSG_TERMINATED */
  }
};

/**************************************************************************************************
  External Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Execute BIG main state machine.
 *
 *  \param      pBigCtx         BIS context.
 *  \param      event           State machine event.
 */
/*************************************************************************************************/
void lctrSlvBigExecuteSm(lctrBigCtx_t *pBigCtx, uint8_t event)
{
  LL_TRACE_INFO3("lctrSlvBigExecuteSm: bigHandle=%u, state=%u, event=%u", pBigCtx->handle, pBigCtx->state, event);

  if (lctrSlvBigActionTbl[pBigCtx->state][event])
  {
    lctrSlvBigActionTbl[pBigCtx->state][event](pBigCtx);
  }

  pBigCtx->state = lctrSlvBigNextStateTbl[pBigCtx->state][event];
}
