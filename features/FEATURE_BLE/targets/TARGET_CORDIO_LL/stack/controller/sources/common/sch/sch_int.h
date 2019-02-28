/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief Internal multi-protocol scheduler interface file.
 */
/*************************************************************************************************/

#ifndef SCH_INT_H
#define SCH_INT_H

#include "sch_api.h"
#include "bb_drv.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum span of scheduler elements. */
#define SCH_MAX_SPAN            0x80000000

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Scheduler states. */
enum
{
  SCH_STATE_IDLE,               /*!< Scheduler idle. */
  SCH_STATE_LOAD,               /*!< Scheduler loading next BOD. */
  SCH_STATE_EXEC                /*!< Scheduler executing BOD. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef struct
{
  bool_t active;                  /*!< Whether background task is active. */
  BbOpDesc_t *pBod;               /*!< Head element of scheduled list of BOD. */
} SchBackground_t;

/*! \brief      Scheduler control block. */
typedef struct
{
  bool_t state;                 /*!< Current scheduler state. */
  bool_t eventSetFlag;          /*!< Scheduler event set (BB terminated BOD). */
  wsfHandlerId_t handlerId;     /*!< System event handler ID. */

  BbOpDesc_t *pHead;            /*!< Head element of scheduled list of BOD. */
  BbOpDesc_t *pTail;            /*!< Tail element of scheduled list of BOD. */
  SchBackground_t background;   /*!< Background BOD. */
} SchCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern SchCtrlBlk_t schCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Load */
void schLoadNext(void);
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

  const uint32_t curTime = BbDrvGetCurrentTime();
  const uint32_t delta = pBod->due - curTime;

  if (delta < SCH_MAX_SPAN)     /* due time has not passed */
  {
    result = TRUE;
  }

  return result;
}

#ifdef __cplusplus
};
#endif

#endif /* SCH_INT_H */
