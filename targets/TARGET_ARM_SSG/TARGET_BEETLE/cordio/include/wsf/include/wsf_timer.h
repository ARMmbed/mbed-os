/*************************************************************************************************/
/*!
 *  \file   wsf_timer.h
 *
 *  \brief  Timer service.
 *
 *          $Date: 2015-09-11 14:14:44 -0700 (Fri, 11 Sep 2015) $
 *          $Revision: 3856 $
 *
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef WSF_TIMER_H
#define WSF_TIMER_H

#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef WSF_MS_PER_TICK
/*! Default milliseconds per tick rate */
#define WSF_MS_PER_TICK   10
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Timer ticks data type */
typedef uint32_t wsfTimerTicks_t;

/*! Timer structure */
typedef struct wsfTimer_tag
{
  struct wsfTimer_tag *pNext;             /*! pointer to next timer in queue */
  wsfTimerTicks_t     ticks;              /*! number of ticks until expiration */
  wsfHandlerId_t      handlerId;          /*! event handler for this timer */
  bool_t              isStarted;          /*! TRUE if timer has been started */
  wsfMsgHdr_t         msg;                /*! application-defined timer event parameters */
} wsfTimer_t;


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfTimerInit
 *
 *  \brief  Initialize the timer service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTimerInit(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfTimerStartSec
 *
 *  \brief  Start a timer in units of seconds.  Before this function is called parameter
 *          pTimer->handlerId must be set to the event handler for this timer and parameter
 *          pTimer->msg must be set to any application-defined timer event parameters.
 *
 *  \param  pTimer  Pointer to timer.
 *  \param  sec     Seconds until expiration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTimerStartSec(wsfTimer_t *pTimer, wsfTimerTicks_t sec);

/*************************************************************************************************/
/*!
 *  \fn     WsfTimerStartMs
 *
 *  \brief  Start a timer in units of milliseconds.
 *
 *  \param  pTimer  Pointer to timer.
 *  \param  ms      Milliseconds until expiration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTimerStartMs(wsfTimer_t *pTimer, wsfTimerTicks_t ms);

/*************************************************************************************************/
/*!
 *  \fn     WsfTimerStop
 *
 *  \brief  Stop a timer.
 *
 *  \param  pTimer  Pointer to timer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTimerStop(wsfTimer_t *pTimer);

/*************************************************************************************************/
/*!
 *  \fn     WsfTimerUpdate
 *
 *  \brief  Update the timer service with the number of elapsed ticks.  This function is
 *          typically called only from timer porting code.
 *
 *  \param  ticks  Number of ticks since last update.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTimerUpdate(wsfTimerTicks_t ticks);

/*************************************************************************************************/
/*!
 *  \fn     WsfTimerNextExpiration
 *
 *  \brief  Return the number of ticks until the next timer expiration.  Note that this
 *          function can return zero even if a timer is running, indicating the timer
 *          has expired but has not yet been serviced.
 *
 *  \param  pTimerRunning   Returns TRUE if a timer is running, FALSE if no timers running.
 *
 *  \return The number of ticks until the next timer expiration.
 */
/*************************************************************************************************/
wsfTimerTicks_t WsfTimerNextExpiration(bool_t *pTimerRunning);

/*************************************************************************************************/
/*!
 *  \fn     WsfTimerServiceExpired
 *
 *  \brief  Service expired timers for the given task.  This function is typically called only
 *          WSF OS porting code.
 *
 *  \param  taskId      OS Task ID of task servicing timers.
 *
 *  \return Pointer to next expired timer or NULL if there are no expired timers.
 */
/*************************************************************************************************/
wsfTimer_t *WsfTimerServiceExpired(wsfTaskId_t taskId);

#ifdef __cplusplus
};
#endif

#endif /* WSF_TIMER_H */
