/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Timer driver
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *  Arm Ltd. confidential and proprietary.
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

/*
 * Notes:
 *
 *    This is timer driver dedicated to scheduler, an interrupt will be triggered to do scheduler task
 *    when timer hits compare value. Timer1 is used here.
 *
 *    Timer1's compare/capture registers are assigned specific uses:
 *      CC[0] - Compare value for timer expiry interrupt
 *      CC[1] - manual capture of current time
 *
 */

#include "nrf.h"
#include "pal_timer.h"
#include "nrf_gpio.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#ifdef DEBUG

/*! \brief      Parameter and state check. */
#define PAL_TIMER_CHECK(expr)         { if (!(expr)) { palTimerCb.state = PAL_TIMER_STATE_ERROR; while(1); } }

#else

/*! \brief      Parameter and state check (disabled). */
#define PAL_TIMER_CHECK(expr)

#endif

/* Scheduler timer resolution is fixed at 1us per tick. */
#define PAL_TIMER_1MHZ_PRESCALER        4

#ifdef DEBUG
#define PAL_TIMER_DEBUG_0_PIN                35   /* P1.03 */
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Scheduler timer driver control block. */
static struct
{
  PalTimerState_t    state;            /*!< State. */
  uint32_t           compareVal;       /*!<  Absolute compare value for timer expiry interrupt. */
  PalTimerCompCback_t expCback;         /*!< Timer expiry call back function. */
} palTimerCb;

/*************************************************************************************************/
/*!
 *  \brief      Get the current tick of the scheduler timer.
 *
 *  \return     Current tick of the scheduler timer.
 */
/*************************************************************************************************/
uint32_t PalTimerGetCurrentTime(void)
{
  /* Only valid for initialized scheduler timer. */

  if (palTimerCb.state)
  {
    /* Capture current TIMER1 count to capture register 1 */
    NRF_TIMER1->TASKS_CAPTURE[1] = 1;
    /* Read and return the captured count value from capture register 1 */
    return NRF_TIMER1->CC[1];
  }

  return 0;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize the scheduler timer.
 *
 *  \param      expCback   Timer expire call back function.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalTimerInit(PalTimerCompCback_t expCback)
{
#ifdef DEBUG
  nrf_gpio_cfg_output(PAL_TIMER_DEBUG_0_PIN);
#endif
  PAL_TIMER_CHECK(palTimerCb.state == PAL_TIMER_STATE_UNINIT);
  PAL_TIMER_CHECK(expCback != NULL);

  /* Give scheduler timer the highest priority. */
  NVIC_SetPriority(TIMER1_IRQn, 0);

  /* stop timer if it was somehow running (timer must be stopped for configuration) */
  NRF_TIMER1->TASKS_STOP  = 1;

  /* clear timer to zero count */
  NRF_TIMER1->TASKS_CLEAR = 1;

  /* configure timer */
  NRF_TIMER1->MODE      = TIMER_MODE_MODE_Timer;
  NRF_TIMER1->BITMODE   = TIMER_BITMODE_BITMODE_32Bit;
  NRF_TIMER1->PRESCALER = PAL_TIMER_1MHZ_PRESCALER;  /* f = 16MHz / (2 ^ TIMER_PRESCALER) */

  /* timer1 is a free running clock. */
  NRF_TIMER1->TASKS_START = 1;

  /* Clear out and enable timer1 interrupt at system level. */
  NRF_TIMER1->INTENCLR = 0xFFFFFFFF;
  NRF_TIMER1->EVENTS_COMPARE[0] = 0;
  NVIC_ClearPendingIRQ(TIMER1_IRQn);
  NVIC_EnableIRQ(TIMER1_IRQn);

  palTimerCb.compareVal = 0;
  palTimerCb.expCback = expCback;
  palTimerCb.state = PAL_TIMER_STATE_READY;
}

/*************************************************************************************************/
/*!
 *  \brief      De-initialize the scheduler timer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalTimerDeInit(void)
{

  NVIC_DisableIRQ(TIMER1_IRQn);

  /* stop timer */
  NRF_TIMER1->TASKS_STOP  = 1;

  palTimerCb.state = PAL_TIMER_STATE_UNINIT;
}

/*************************************************************************************************/
/*!
 *  \brief      Return scheduler timer state.
 *
 *  \return     state.
 */
/*************************************************************************************************/
PalTimerState_t PalTimerGetState(void)
{
  return palTimerCb.state;
}

/*************************************************************************************************/
/*!
 *  \brief      Start the scheduler timer.
 *
 *  \param      expTimeUsec      Set timer expiry in microseconds.
 *
 *  \return     None
 */
/*************************************************************************************************/

void PalTimerStart(uint32_t expTimeUsec)
{
  PAL_TIMER_CHECK(palTimerCb.state == PAL_TIMER_STATE_READY);
  PAL_TIMER_CHECK(expTimeUsec != 0);

  uint32_t startTimeTick = PalTimerGetCurrentTime() + PAL_TIMER_US_TO_TICKS(expTimeUsec);

  /* Clear pending events. */
  NRF_TIMER1->EVENTS_COMPARE[0] = 0;

  /* Set compare value. */
  NRF_TIMER1->CC[0] = startTimeTick;

  /* Enable timer1 interrupt source for CC[0].  */
  NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE0_Msk;

  palTimerCb.compareVal = startTimeTick;
  palTimerCb.state = PAL_TIMER_STATE_BUSY;
}

/*************************************************************************************************/
/*!
 *  \brief      Stop the scheduler timer.
 *
 *  \return     None
 */
/*************************************************************************************************/
void PalTimerStop()
{
  /* Disable this interrupt */
  NRF_TIMER1->INTENCLR = TIMER_INTENCLR_COMPARE0_Msk;

  palTimerCb.state = PAL_TIMER_STATE_READY;
}

/*************************************************************************************************/
/*!
 *  \brief      TIMER1 interrupt handler dedicated to scheduler timer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void TIMER1_IRQHandler(void)
{
#ifdef DEBUG
  nrf_gpio_pin_set(PAL_TIMER_DEBUG_0_PIN);
#endif

  PAL_TIMER_CHECK(palTimerCb.state == PAL_TIMER_STATE_BUSY);
  /* Check hardware status */
  PAL_TIMER_CHECK(NRF_TIMER1->EVENTS_COMPARE[0]);
  PAL_TIMER_CHECK(NRF_TIMER1->CC[0] == palTimerCb.compareVal);
  PAL_TIMER_CHECK(NRF_TIMER1->INTENSET == TIMER_INTENSET_COMPARE0_Msk);

  /* Callback function could restart timer1. However, we blindly stop timer1 first. */
  NRF_TIMER1->INTENCLR = TIMER_INTENCLR_COMPARE0_Msk;
  /* Clear event again just in case. */
  NRF_TIMER1->EVENTS_COMPARE[0] = 0;

  palTimerCb.state = PAL_TIMER_STATE_READY;

  if (palTimerCb.expCback)
  {
    palTimerCb.expCback();
  }

#ifdef DEBUG
  nrf_gpio_pin_clear(PAL_TIMER_DEBUG_0_PIN);
#endif
}
