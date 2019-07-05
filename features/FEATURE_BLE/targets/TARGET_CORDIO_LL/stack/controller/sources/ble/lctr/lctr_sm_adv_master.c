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

#include "lctr_int_adv_master.h"
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

/*! \brief      Scan state machine action table.
 */
static const lctrActFn_t lctrScanActionTbl[LCTR_SCAN_STATE_TOTAL][LCTR_SCAN_MSG_TOTAL] =
{
  { /* LCTR_SCAN_STATE_DISABLED */
    NULL,                       /* LCTR_SCAN_MSG_RESET */
    lctrScanActDiscover,        /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    lctrScanActScanCnf,         /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    NULL,                       /* LCTR_SCAN_MSG_TERMINATE */
    lctrScanActUpdateScanParam  /* LCTR_SCAN_MSG_PARAM_UPD */
  },
  { /* LCTR_SCAN_STATE_DISCOVER */
    lctrScanActShutdown,        /* LCTR_SCAN_MSG_RESET */
    lctrScanActUpdateScanFilt,  /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    lctrScanActShutdown,        /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    lctrScanActSelfTerm,        /* LCTR_SCAN_MSG_TERMINATE */
    NULL                        /* LCTR_SCAN_MSG_PARAM_UPD */
  },
  { /* LCTR_SCAN_STATE_SHUTDOWN */
    NULL,                       /* LCTR_SCAN_MSG_RESET */
    lctrScanActDisallowScan,    /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    lctrScanActDisallowScan,    /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    lctrScanActScanTerm,        /* LCTR_SCAN_MSG_TERMINATE */
    lctrScanActUpdateScanParam  /* LCTR_SCAN_MSG_PARAM_UPD */
  },
  { /* LCTR_SCAN_STATE_RESET */
    NULL,                       /* LCTR_SCAN_MSG_RESET */
    lctrScanActDisallowScan,    /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    lctrScanActDisallowScan,    /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    lctrScanActResetTerm,       /* LCTR_SCAN_MSG_TERMINATE */
    lctrScanActUpdateScanParam  /* LCTR_SCAN_MSG_PARAM_UPD */
  }
};

/*! \brief      Scan state machine next state table. */
static const uint8_t lctrScanNextStateTbl[LCTR_SCAN_STATE_TOTAL][LCTR_SCAN_MSG_TOTAL] =
{
  { /* LCTR_SCAN_STATE_DISABLED */
    LCTR_SCAN_STATE_DISABLED,   /* LCTR_SCAN_MSG_RESET */
    LCTR_SCAN_STATE_DISCOVER,   /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_SCAN_STATE_DISABLED,   /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_SCAN_STATE_DISABLED,   /* LCTR_SCAN_MSG_TERMINATE */
    LCTR_SCAN_STATE_DISABLED    /* LCTR_SCAN_MSG_PARAM_UPD */
  },
  { /* LCTR_SCAN_STATE_DISCOVER */
    LCTR_SCAN_STATE_RESET,      /* LCTR_SCAN_MSG_RESET */
    LCTR_SCAN_STATE_DISCOVER,   /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_SCAN_STATE_SHUTDOWN,   /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_SCAN_STATE_DISABLED,   /* LCTR_SCAN_MSG_TERMINATE */
    LCTR_SCAN_STATE_DISCOVER    /* LCTR_SCAN_MSG_PARAM_UPD */
  },
  { /* LCTR_SCAN_STATE_SHUTDOWN */
    LCTR_SCAN_STATE_RESET,      /* LCTR_SCAN_MSG_RESET */
    LCTR_SCAN_STATE_SHUTDOWN,   /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_SCAN_STATE_SHUTDOWN,   /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_SCAN_STATE_DISABLED,   /* LCTR_SCAN_MSG_TERMINATE */
    LCTR_SCAN_STATE_SHUTDOWN    /* LCTR_SCAN_MSG_PARAM_UPD */
  },
  { /* LCTR_SCAN_STATE_RESET */
    LCTR_SCAN_STATE_RESET,      /* LCTR_SCAN_MSG_RESET */
    LCTR_SCAN_STATE_RESET,      /* LCTR_SCAN_MSG_DISCOVER_ENABLE */
    LCTR_SCAN_STATE_RESET,      /* LCTR_SCAN_MSG_DISCOVER_DISABLE */
    LCTR_SCAN_STATE_DISABLED,   /* LCTR_SCAN_MSG_TERMINATE */
    LCTR_SCAN_STATE_RESET       /* LCTR_SCAN_MSG_PARAM_UPD */
  }
};

/*************************************************************************************************/
/*!
 *  \brief      Execute master scan state machine.
 *
 *  \param      event   State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstScanExecuteSm(uint8_t event)
{
  LL_TRACE_INFO2("lctrMstScanExecuteSm: state=%u, event=%u", lctrMstScan.state, event);

  if (lctrScanActionTbl[lctrMstScan.state][event])
  {
    lctrScanActionTbl[lctrMstScan.state][event]();
  }

  lctrMstScan.state = lctrScanNextStateTbl[lctrMstScan.state][event];
}
