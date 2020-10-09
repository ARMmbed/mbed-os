/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Internal multi-protocol scheduler interface file.
 *
 *  Copyright (c) 2013-2019 ARM Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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

/*! \brief      Maximum span of scheduler elements. Half of the boundary time. */
#define SCH_MAX_SPAN            ((BbGetBbTimerBoundaryUs() >> 1) + 1)

/*! \brief      Typical time needed for loading BOD. */
#define SCH_LOAD_DELAY_US       300

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Scheduler states. */
typedef enum
{
  SCH_STATE_IDLE,               /*!< Scheduler idle. */
  SCH_STATE_EXEC                /*!< Scheduler executing BOD. */
} schState_t;

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Scheduler control block. */
typedef struct
{
  schState_t state:8;               /*!< Current scheduler state. */
  uint8_t eventSetFlagCount;        /*!< Scheduler event set count. */
  wsfHandlerId_t handlerId;         /*!< System event handler ID. */

  BbOpDesc_t *pHead;                /*!< Head element of scheduled list of BOD. */
  BbOpDesc_t *pTail;                /*!< Tail element of scheduled list of BOD. */

  uint16_t schHandlerWatermarkUsec; /*!< Statistics: Handler duration watermark in microseconds. */
  uint16_t delayLoadWatermarkCount; /*!< Statistics: Delay loading watermark count. */
  uint16_t delayLoadCount;          /*!< Statistics: Delay loading count. */
  uint32_t delayLoadTotalCount;     /*!< Statistics: Delay loading total count. */
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
  const uint32_t curTime = PalBbGetCurrentTime();

  return (BbGetTargetTimeDelta(pBod->dueUsec, curTime) > 0);
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

  const uint32_t curTime = PalBbGetCurrentTime();

  result = BbGetTargetTimeDelta(pBod->dueUsec, curTime);

  if (result >= SCH_LOAD_DELAY_US)
  {
    result -= SCH_LOAD_DELAY_US;
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
