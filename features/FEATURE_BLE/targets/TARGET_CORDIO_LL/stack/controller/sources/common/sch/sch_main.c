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
 * \brief Operation list maintenance implementation file.
 */
/*************************************************************************************************/

#include "sch_int.h"
#include "pal_timer.h"
#include "bb_api.h"
#include "wsf_trace.h"
#include <string.h>

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief  Scheduler task events. */
enum
{
  SCH_EVENT_BOD_COMPLETE = (1 << 0),    /*!< BOD completion event. */
  SCH_EVENT_BOD_ABORT    = (1 << 1),    /*!< BOD abort event. */
  SCH_EVENT_BOD_CURTAIL  = (1 << 2),    /*!< BOD curtail event. */
  SCH_EVENT_BOD_LOAD     = (1 << 3),    /*!< BOD load event. */
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Scheduler control block. */
SchCtrlBlk_t schCb;

/*! \brief      Handler duration watermark in microseconds. */
static uint16_t schHandlerWatermarkUsec = 0;

/*************************************************************************************************/
/*!
 *  \brief      BOD completion handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schBodCompHandler(void)
{
  WsfSetEvent(schCb.handlerId, SCH_EVENT_BOD_COMPLETE);
  schCb.eventSetFlagCount++;
}

/*************************************************************************************************/
/*!
 *  \brief      BOD abortion handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schBodAbortHandler(void)
{
  WsfSetEvent(schCb.handlerId, SCH_EVENT_BOD_ABORT);
  schCb.eventSetFlagCount++;
}

/*************************************************************************************************/
/*!
 *  \brief      BOD curtail handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schBodCurtailHandler(void)
{
  WsfSetEvent(schCb.handlerId, SCH_EVENT_BOD_CURTAIL);
  schCb.eventSetFlagCount++;
}

/*************************************************************************************************/
/*!
 *  \brief      BOD load handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void schBodLoadHandler(void)
{
  BbOpDesc_t* pNextBod = schCb.pHead;

  if (schCb.eventSetFlagCount)
  {
    WSF_ASSERT(pNextBod);
    /* Delay loading after event flag is cleared. */
    WsfSetEvent(schCb.handlerId, SCH_EVENT_BOD_LOAD);
    return;
  }
  else
  {
    /* Try load head if scheduler is idle. */
    if (schCb.state == SCH_STATE_IDLE)
    {
      if (!schTryLoadHead())
      {
        /* Head load failed. */
        schBodAbortHandler();
      }
      /* Move to next BOD. */
      pNextBod = pNextBod->pNext;
    }
#if SCH_TIMER_REQUIRED == TRUE
    /* If head is executed, check cur tail operation is needed or not. */
    else
    {
      /* Head BOD and next BOD must exist. */
      WSF_ASSERT(schCb.pHead);
      WSF_ASSERT(schCb.pHead->pNext);
      pNextBod = pNextBod->pNext;

      /* Skip curtail load if next BOD has same or lower priority than current BOD. */
      if ((pNextBod->reschPolicy) >= (schCb.pHead->reschPolicy))
      {
        /* Delay loading until idle state. */
        WsfSetEvent(schCb.handlerId, SCH_EVENT_BOD_LOAD);
        return;
      }

      if (!schTryCurTailLoadNext())
      {
        /* Curtail load failed. */
        schBodAbortHandler();
      }
      /* Move to the next next BOD. */
      pNextBod = pNextBod->pNext;
    }
  }

  /* If pNextBod exists, it should start scheduler timer. */
  if (pNextBod)
  {
    uint32_t execTimeUsec = schGetTimeToExecBod(pNextBod);

    if (execTimeUsec)
    {
      /* Always stop existing timer first for simplicity. */
      PalTimerStop();
      PalTimerStart(execTimeUsec);
    }
    else
    {
      /* If this happens, it means there's something wrong with the scheduler list. */
      LL_TRACE_WARN0(" Next BOD overlaps with current BOD. ");
      /* Send scheduler load event. */
      SchLoadHandler();
    }
#endif
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Scheduler load handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SchLoadHandler(void)
{
  WsfSetEvent(schCb.handlerId, SCH_EVENT_BOD_LOAD);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize the scheduler subsystem.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SchInit(void)
{
  memset(&schCb, 0, sizeof(schCb));
  SchReset();
  PalTimerInit(schBodLoadHandler);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize the scheduler subsystem.
 *
 *  \param      handlerId  WSF handler ID.
 *
 *  \return     None.
 *
 *  \note       This initialization to used to enable task-based scheduler completions. For
 *              ISR-based scheduler completions, do not call this routine. Instead install an
 *              ISR which calls SchHandler() on BOD completions.
 */
/*************************************************************************************************/
void SchHandlerInit(wsfHandlerId_t handlerId)
{
  SchInit();

  schCb.handlerId = handlerId;

  BbRegister(schBodCompHandler);
}

/*************************************************************************************************/
/*!
 *  \brief      Reset the scheduler subsystem.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SchReset(void)
{
  schCb.state = SCH_STATE_IDLE;
  schCb.pHead = NULL;
  schCb.pTail = NULL;
}

/*************************************************************************************************/
/*!
 *  \brief      Scheduler message dispatch handler.
 *
 *  \param      event       WSF event.
 *  \param      pMsg        WSF message.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void SchHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  /* Unused parameters */
  (void)pMsg;

  /* Assume scheduler clock started. */
  uint32_t startTime = PalTimerGetCurrentTime();

  BbOpDesc_t *pBod = schCb.pHead;

  if (!pBod)
  {
    schCb.state = SCH_STATE_IDLE;
    return;
  }

  if (event & SCH_EVENT_BOD_COMPLETE)
  {
    WSF_ASSERT(schCb.state == SCH_STATE_EXEC);
    WSF_ASSERT(schCb.eventSetFlagCount);

    /*** Complete current BOD ***/

    schCb.state = SCH_STATE_IDLE;
    schRemoveHead();
    if (pBod->endCback)
    {
      pBod->endCback(pBod);
    }
    schCb.eventSetFlagCount--;

#if SCH_TIMER_REQUIRED == FALSE
    schBodLoadHandler();
#endif
  }

  if (event & SCH_EVENT_BOD_ABORT)
  {
    WSF_ASSERT(schCb.state == SCH_STATE_IDLE);
    WSF_ASSERT(schCb.eventSetFlagCount);

    /*** Abort current BOD ***/

    schRemoveHead();
    if (pBod->abortCback)
    {
      pBod->abortCback(pBod);
    }
    schCb.eventSetFlagCount--;

#if SCH_TIMER_REQUIRED == FALSE
    schBodLoadHandler();
#endif
  }

  if (event & SCH_EVENT_BOD_CURTAIL)
  {
    WSF_ASSERT(schCb.state == SCH_STATE_EXEC);
    WSF_ASSERT(schCb.eventSetFlagCount);

    /*** Complete previous BOD ***/
    schRemoveHead();
    if (pBod->endCback)
    {
      pBod->endCback(pBod);
    }
    schCb.eventSetFlagCount--;
  }

  if (event & SCH_EVENT_BOD_LOAD)
  {
    schBodLoadHandler();
  }

  uint32_t curTick = PalTimerGetCurrentTime();
  /* Consider both count-up or counter-down timer type. */
  uint32_t durTick = (curTick- startTime < 0x80000000) ? (curTick - startTime) : (startTime - curTick);
  uint16_t durUsec = PAL_TIMER_TICKS_TO_US(durTick);
  if (schHandlerWatermarkUsec < durUsec)
  {
    schHandlerWatermarkUsec = durUsec;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Load BOD if not already started.
 *
 *  \return     TRUE if loaded, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t schLoadBod(BbOpDesc_t *pBod)
{
  bool_t      loaded = FALSE;

  if (schDueTimeInFuture(pBod))
  {
    /* Setup BB services. */
    BbExecuteBod(pBod);

    if (!BbGetBodTerminateFlag())
    {
      loaded = TRUE;
    }
    else
    {
      LL_TRACE_WARN1("!!! BOD terminated on startup, pBod=0x%08x", pBod);

      if (schCb.eventSetFlagCount)
      {
        /* Termination or failure is scheduled to complete at next task event. */
        loaded = TRUE;
      }

      BbCancelBod();
    }
  }
  else
  {
    /* This might occur due to the delay of conflict resolution. */
    LL_TRACE_WARN1("!!! Head element in the past, pBod=0x%08x", pBod);
  }

  return loaded;
}

/*************************************************************************************************/
/*!
 *  \brief      Load next BOD by curtailing the executed head BOD first.
 *
 *  \return     TRUE if curtail loading succeeded.
 */
/*************************************************************************************************/
bool_t schTryCurTailLoadNext(void)
{
  bool_t loaded = TRUE;

  /* It should only be called when scheduler is in execute state. */
  WSF_ASSERT(schCb.state == SCH_STATE_EXEC);
  /* Head BOD and next BOD must exist. */
  WSF_ASSERT(schCb.pHead);
  WSF_ASSERT(schCb.pHead->pNext);

  if (schCb.pHead->pNext)
  {
    /* Hard stop head BOD and load next BOD. */
    BbCancelBod();
    schBodCurtailHandler();

    schCb.state = SCH_STATE_EXEC;

    /* Try load next BOD. */
    if (!(schLoadBod(schCb.pHead->pNext)))
    {
      schCb.state = SCH_STATE_IDLE;
      loaded = FALSE;
    }
  }

  return loaded;
}

/*************************************************************************************************/
/*!
 *  \brief      Try to load head BOD if not already started.
 *
 *  \return     TRUE if load succeeded.
 */
/*************************************************************************************************/
bool_t schTryLoadHead(void)
{
  bool_t loaded = TRUE;

  /* It should only be called when scheduler is in idle state. */
  WSF_ASSERT(schCb.state == SCH_STATE_IDLE);

#if SCH_TIMER_REQUIRED == TRUE
  /* Head BOD must exist. */
  WSF_ASSERT(schCb.pHead);
#endif

  if (schCb.pHead)
  {
    schCb.state = SCH_STATE_EXEC;

    if (!schLoadBod(schCb.pHead))
    {
      schCb.state = SCH_STATE_IDLE;
      loaded = FALSE;
    }
  }

  return loaded;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the scheduler handler watermark level.
 *
 *  \return     Watermark level in microseconds.
 */
/*************************************************************************************************/
uint16_t SchStatsGetHandlerWatermarkUsec(void)
{
  return schHandlerWatermarkUsec;
}
