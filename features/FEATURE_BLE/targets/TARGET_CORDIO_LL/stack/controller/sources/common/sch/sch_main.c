/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Operation list maintenance implementation file.
 *
 *  Copyright (c) 2013-2018 ARM Ltd. All Rights Reserved.
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

#include "sch_int.h"
#include "bb_api.h"
#include "wsf_trace.h"
#include <string.h>

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief  Scheduler task events. */
enum
{
  SCH_EVENT_BOD_COMPLETE = (1 << 1)     /*!< BOD completion event. */
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
  schCb.eventSetFlag = TRUE;
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
  /* Assume BB clock started. */
  uint32_t startTime = BbDrvGetCurrentTime();

  WSF_ASSERT(schCb.state == SCH_STATE_EXEC);

  BbOpDesc_t *pBod = schCb.pHead;

  if (!pBod)
  {
    schCb.state = SCH_STATE_IDLE;
    return;
  }

  /*** Complete current BOD ***/

  schRemoveHead();

  schCb.state = SCH_STATE_LOAD;

  if (pBod->endCback)
  {
    pBod->endCback(pBod);
  }

  schCb.state = SCH_STATE_IDLE;
  schLoadNext();

  uint16_t durUsec = BB_TICKS_TO_US(BbDrvGetCurrentTime() - startTime);
  if (schHandlerWatermarkUsec < durUsec)
  {
    schHandlerWatermarkUsec = durUsec;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Load head BOD if not already started.
 *
 *  \return     TRUE if loaded, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t schLoadHead(void)
{
  bool_t      loaded = FALSE;
  BbOpDesc_t *pBod   = schCb.pHead;

  if (schDueTimeInFuture(pBod))
  {
    schCb.eventSetFlag = FALSE;

    /* Setup BB services. */
    BbExecuteBod(pBod);

    if (!BbGetBodTerminateFlag())
    {
      loaded = TRUE;
    }
    else
    {
      LL_TRACE_WARN1("!!! BOD terminated on startup, pBod=0x%08x", pBod);

      if (schCb.eventSetFlag)
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
 *  \brief      Load next BOD if not already started.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void schLoadNext(void)
{
  /* Only load if scheduler is idle. */
  if (schCb.state == SCH_STATE_IDLE)
  {
    while (TRUE)
    {
      if (!schCb.pHead)
      {
        schCb.state = SCH_STATE_IDLE;
        if ((schCb.background.pBod != NULL) &&
            !schCb.background.active)
        {
          /* Reactivate background BOD */
          BbExecuteBod(schCb.background.pBod);
          schCb.background.active = TRUE;
        }
        break;
      }

      schCb.state = SCH_STATE_EXEC;

      if (schLoadHead())
      {
        break;
      }

      schCb.state = SCH_STATE_LOAD;

      SchRemove(schCb.pHead);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Try to load head BOD if not already started.
 *
 *  \return     TRUE if load succeeded (or head BOD already started).
 */
/*************************************************************************************************/
bool_t schTryLoadHead(void)
{
  bool_t loaded = TRUE;

  /* Only load if scheduler is idle. */
  if (schCb.state == SCH_STATE_IDLE)
  {
    if (schCb.pHead)
    {
      schCb.state = SCH_STATE_EXEC;

      if (!schLoadHead())
      {
        schRemoveHead();

        schCb.state = SCH_STATE_IDLE;
        loaded = FALSE;
      }
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

/*************************************************************************************************/
/*!
 *  \brief      Terminate background.
 *
 *  Completely terminates the background BDO and removes it from record. The end callback should
 *  free the BOD if necessary. It will subsequently need to be fully restarted.
 *
 *  \return     None.
 *
 */
/*************************************************************************************************/
void SchTerminateBackground(void)
{
  if ((schCb.background.pBod != NULL) &&
      (schCb.background.pBod->endCback != NULL))
  {
    schCb.background.pBod->endCback(schCb.background.pBod);
    schCb.background.pBod = NULL;
    schCb.background.active = FALSE;
  }
}
