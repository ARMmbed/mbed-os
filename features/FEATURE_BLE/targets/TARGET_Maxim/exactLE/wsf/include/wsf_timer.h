/*************************************************************************************************/
/*!
 *  \file   wsf_timer.h
 *
 *  \brief  Timer service.
 *
 *          $Date: 2013-07-19 17:17:05 -0700 (Fri, 19 Jul 2013) $
 *          $Revision: 843 $
 *
 *  Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
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

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Timer ticks data type */
typedef uint16_t wsfTimerTicks_t;

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
 *  \param  msPerTick   Sets the number of milliseconds per timer tick.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTimerInit(uint8_t msPerTick);

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
