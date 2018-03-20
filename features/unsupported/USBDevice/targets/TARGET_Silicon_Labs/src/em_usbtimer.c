/***************************************************************************//**
 * @file em_usbtimer.c
 * @brief USB protocol stack library, timer API.
 * @version 3.20.14
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "em_device.h"
#if defined( USB_PRESENT ) && ( USB_COUNT == 1 )
#include "em_usb.h"
#if defined( USB_DEVICE ) || defined( USB_HOST )
#include "em_cmu.h"
#include "em_core.h"
#include "em_timer.h"
#include "em_usbtypes.h"
#include "em_usbhal.h"

#include "device_peripherals.h"

/*
 *  Use one HW timer to serve n software milisecond timers.
 *  A timer is, when running, in a linked list of timers.
 *  A given timers timeout period is the acculmulated timeout
 *  of all timers preceeding it in the queue.
 *  This makes timer start (linked list insertion) computing intensive,
 *  but the checking of the queue at each tick very effective.
 *             ______          ______          ______
 *            |      |    --->|      |    --->|      |
 *   head --> |      |   |    |      |   |    |      |
 *            |______|---     |______|---     |______|---/ NULL
 */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#ifndef USB_TIMER
#error HW platform must define the timer to use for USB
#endif

#if ( USB_TIMER == USB_TIMER0 ) && ( TIMER_COUNT >= 1 )
  #define TIMER             TIMER0
  #define TIMER_CLK         cmuClock_TIMER0
  #define TIMER_IRQ         TIMER0_IRQn
  #define TIMER_IRQHandler  TIMER0_IRQHandler

#elif ( USB_TIMER == USB_TIMER1 ) && ( TIMER_COUNT >= 2 )
  #define TIMER             TIMER1
  #define TIMER_CLK         cmuClock_TIMER1
  #define TIMER_IRQ         TIMER1_IRQn
  #define TIMER_IRQHandler  TIMER1_IRQHandler

#elif ( USB_TIMER == USB_TIMER2 ) && ( TIMER_COUNT >= 3 )
  #define TIMER             TIMER2
  #define TIMER_CLK         cmuClock_TIMER2
  #define TIMER_IRQ         TIMER2_IRQn
  #define TIMER_IRQHandler  TIMER2_IRQHandler

#elif ( USB_TIMER == USB_TIMER3 ) && ( TIMER_COUNT == 4 )
  #define TIMER             TIMER3
  #define TIMER_CLK         cmuClock_TIMER3
  #define TIMER_IRQ         TIMER3_IRQn
  #define TIMER_IRQHandler  TIMER3_IRQHandler

#else
#error "Illegal USB TIMER definition"
#endif

typedef struct _timer
{
  uint32_t                  timeout;  /* Delta value relative to prev. timer */
  struct _timer             *next;
  USBTIMER_Callback_TypeDef callback;
  bool                      running;
} USBTIMER_Timer_TypeDef;

#if ( NUM_QTIMERS > 0 )
static USBTIMER_Timer_TypeDef timers[ NUM_QTIMERS ];
static USBTIMER_Timer_TypeDef *head = NULL;
#endif

static uint32_t ticksPrMs, ticksPr1us, ticksPr10us, ticksPr100us;

#if ( NUM_QTIMERS > 0 )

static void TimerTick( void );

void TIMER_IRQHandler( void )
{
  uint32_t flags;

  flags = TIMER_IntGet( TIMER );

  if ( flags & TIMER_IF_CC0 )
  {
    TIMER_IntClear( TIMER, TIMER_IFC_CC0 );
    TIMER_CompareSet( TIMER, 0, TIMER_CaptureGet( TIMER, 0 ) + ticksPrMs );
    TimerTick();
  }
}
#endif /* ( NUM_QTIMERS > 0 ) */

static void DelayTicks( uint16_t ticks )
{
  uint16_t startTime;
  volatile uint16_t now;

  if ( ticks )
  {
    startTime = TIMER_CounterGet( TIMER );
    do
    {
      now = TIMER_CounterGet(TIMER);
    } while ( (uint16_t)( now - startTime ) < ticks );
  }
}

/** @endcond */

/** @addtogroup USB_COMMON
 *  @{*/

/***************************************************************************//**
 * @brief
 *   Active wait millisecond delay function. Can also be used inside
 *   interrupt handlers.
 *
 * @param[in] msec
 *   Number of milliseconds to wait.
 ******************************************************************************/
void USBTIMER_DelayMs( uint32_t msec )
{
  uint64_t totalTicks;

  totalTicks = (uint64_t)ticksPrMs * msec;
  while ( totalTicks > 20000 )
  {
    DelayTicks( 20000 );
    totalTicks -= 20000;
  }
  DelayTicks( (uint16_t)totalTicks );
}

/***************************************************************************//**
 * @brief
 *   Active wait microsecond delay function. Can also be used inside
 *   interrupt handlers.
 *
 * @param[in] usec
 *   Number of microseconds to wait.
 ******************************************************************************/
void USBTIMER_DelayUs( uint32_t usec )
{
  uint64_t totalTicks;

  totalTicks = (uint64_t)ticksPr1us * usec;
  if ( totalTicks == 0 )
  {
    usec /= 10;
    totalTicks = (uint64_t)ticksPr10us * usec;

    if ( totalTicks == 0 )
    {
      usec /= 10;
      totalTicks = (uint64_t)ticksPr100us * usec;
    }
  }

  while ( totalTicks > 60000 )
  {
    DelayTicks( 60000 );
    totalTicks -= 60000;
  }
  DelayTicks( (uint16_t)totalTicks );
}

/***************************************************************************//**
 * @brief
 *   Activate the hardware timer used to pace the 1 millisecond timer system.
 *
 * @details
 *   Call this function whenever the HFPERCLK frequency is changed.
 *   This function is initially called by HOST and DEVICE stack xxxx_Init()
 *   functions.
 ******************************************************************************/
void USBTIMER_Init( void )
{
  uint32_t freq;
  TIMER_Init_TypeDef timerInit     = TIMER_INIT_DEFAULT;
  TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

  freq = CMU_ClockFreqGet( cmuClock_HFPER );
  ticksPrMs = ( freq + 500 ) / 1000;
  ticksPr1us = ( freq + 500000 ) / 1000000;
  ticksPr10us = ( freq + 50000 ) / 100000;
  ticksPr100us = ( freq + 5000 ) / 10000;

  timerCCInit.mode = timerCCModeCompare;
  CMU_ClockEnable( TIMER_CLK, true );
  TIMER_TopSet( TIMER, 0xFFFF );
  TIMER_InitCC( TIMER, 0, &timerCCInit );
  TIMER_Init( TIMER, &timerInit );

#if ( NUM_QTIMERS > 0 )
  TIMER_IntClear( TIMER, 0xFFFFFFFF );
  TIMER_IntEnable( TIMER, TIMER_IEN_CC0 );
  TIMER_CompareSet( TIMER, 0, TIMER_CounterGet( TIMER ) + ticksPrMs );
  NVIC_ClearPendingIRQ( TIMER_IRQ );
  NVIC_EnableIRQ( TIMER_IRQ );
#endif /* ( NUM_QTIMERS > 0 ) */
}

#if ( NUM_QTIMERS > 0 ) || defined( DOXY_DOC_ONLY )
/***************************************************************************//**
 * @brief
 *   Start a timer.
 *
 * @details
 *   If the timer is already running, it will be restarted with new timeout.
 *
 * @param[in] id
 *   Timer id (0..).
 *
 * @param[in] timeout
 *   Number of milliseconds before timer will elapse.
 *
 * @param[in] callback
 *   Function to be called on timer elapse, ref. @ref USBTIMER_Callback_TypeDef.
 ******************************************************************************/
void USBTIMER_Start( uint32_t id, uint32_t timeout,
                     USBTIMER_Callback_TypeDef callback )
{
  uint32_t accumulated;
  USBTIMER_Timer_TypeDef *this, **last;
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  if ( timers[ id ].running )
  {
    USBTIMER_Stop( id );
  }

  if ( timeout == 0 )
  {
    callback();
    CORE_EXIT_CRITICAL();
    return;
  }

  timers[ id ].running  = true;
  timers[ id ].callback = callback;
  timers[ id ].next     = NULL;

  if ( !head )                                        /* Queue empty ? */
  {
    timers[ id ].timeout  = timeout;
    head = &timers[ id ];
  }
  else
  {
    this = head;
    last = &head;
    accumulated = 0;

    /* Do a sorted insert */
    while ( this  )
    {
      if ( timeout < accumulated + this->timeout )  /* Insert before "this" ? */
      {
        timers[ id ].timeout  = timeout - accumulated;
        timers[ id ].next     = this;
        *last = &timers[ id ];
        this->timeout -= timers[ id ].timeout;        /* Adjust timeout     */
        break;
      }
      else if ( this->next == NULL )                  /* At end of queue ?  */
      {
        timers[ id ].timeout  = timeout - accumulated - this->timeout;
        this->next = &timers[ id ];
        break;
      }
      accumulated += this->timeout;
      last = &this->next;
      this = this->next;
    }
  }

  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 *   Stop a timer.
 *
 * @param[in] id
 *   Timer id (0..).
 ******************************************************************************/
void USBTIMER_Stop( uint32_t id )
{
  USBTIMER_Timer_TypeDef *this, **last;
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  if ( head )                                           /* Queue empty ?    */
  {
    this = head;
    last = &head;
    timers[ id ].running = false;

    while ( this  )
    {
      if ( this == &timers[ id ] )                      /* Correct timer ?  */
      {
        if ( this->next )
        {
          this->next->timeout += timers[ id ].timeout;  /* Adjust timeout   */
        }
        *last = this->next;
        break;
      }
      last = &this->next;
      this = this->next;
    }
  }

  CORE_EXIT_CRITICAL();
}
#endif /* ( NUM_QTIMERS > 0 ) */

/** @} (end addtogroup USB_COMMON) */

#if ( NUM_QTIMERS > 0 )
/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

static void TimerTick( void )
{
  USBTIMER_Callback_TypeDef cb;
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  if ( head )
  {
    head->timeout--;

    while ( head  )
    {
      if ( head->timeout == 0 )
      {
        cb = head->callback;
        head->running = false;
        head = head->next;
        /* The callback may place new items in the queue !!! */
        if ( cb )
        {
          (cb)();
        }
        continue; /* There might be more than one timeout pr. tick */
      }
      break;
    }
  }

  CORE_EXIT_CRITICAL();
}
/** @endcond */
#endif /* ( NUM_QTIMERS > 0 ) */

#endif /* defined( USB_DEVICE ) || defined( USB_HOST ) */
#endif /* defined( USB_PRESENT ) && ( USB_COUNT == 1 ) */
