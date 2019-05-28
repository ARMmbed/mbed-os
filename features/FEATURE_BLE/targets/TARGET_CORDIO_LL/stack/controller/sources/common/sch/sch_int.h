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
 * \brief Internal multi-protocol scheduler interface file.
 */
/*************************************************************************************************/

#ifndef SCH_INT_H
#define SCH_INT_H

#include "sch_api.h"
#include "pal_bb.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum span of scheduler elements. */
#define SCH_MAX_SPAN            0x80000000

/*! \brief      Typical time needed for loading BOD. */
#define SCH_LOAD_DELAY_US       300

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Scheduler states. */
enum
{
  SCH_STATE_IDLE,               /*!< Scheduler idle. */
  SCH_STATE_EXEC                /*!< Scheduler executing BOD. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Scheduler control block. */
typedef struct
{
  bool_t state;                 /*!< Current scheduler state. */
  uint8_t eventSetFlagCount;    /*!< Scheduler event set count. */
  wsfHandlerId_t handlerId;     /*!< System event handler ID. */

  BbOpDesc_t *pHead;            /*!< Head element of scheduled list of BOD. */
  BbOpDesc_t *pTail;            /*!< Tail element of scheduled list of BOD. */
} SchCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern SchCtrlBlk_t schCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Load */
bool_t schTryCurTailLoadNext(void);
bool_t schTryLoadHead(void);

/* List management */
void schRemoveHead(void);

/*************************************************************************************************/
/*!
 *  \brief      Is BOD due time in the future.
 *
 *  \param      pBod    Target BOD.
 *
 *  \return     TRUE if BOD time is in the future, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t schDueTimeInFuture(BbOpDesc_t *pBod)
{
  bool_t result = FALSE;

  const uint32_t curTime = PalBbGetCurrentTime(USE_RTC_BB_CLK);
  const uint32_t delta = pBod->due - curTime;

  if (delta < SCH_MAX_SPAN)     /* due time has not passed */
  {
    result = TRUE;
  }

  return result;
}


/*************************************************************************************************/
/*!
 *  \brief      Return the time between now and the BOD to be executed.
 *
 *  \param      pBod    Target BOD.
 *
 *  \return     usec.
 */
/*************************************************************************************************/
static inline uint32_t schGetTimeToExecBod(BbOpDesc_t *pBod)
{
  uint32_t result = 0;

  const uint32_t curTime = PalBbGetCurrentTime(USE_RTC_BB_CLK);
  const uint32_t delta = pBod->due - curTime;

  if ((delta >= BB_US_TO_BB_TICKS(SCH_LOAD_DELAY_US)) &&   /* sufficient time to cancel */
      (delta < SCH_MAX_SPAN))                                   /* due time has not passed */
  {
    result = BB_TICKS_TO_US(delta - BB_US_TO_BB_TICKS(SCH_LOAD_DELAY_US));
  }
  else
  {
    result = 0;
  }

  return result;
}

#ifdef __cplusplus
};
#endif

#endif /* SCH_INT_H */
