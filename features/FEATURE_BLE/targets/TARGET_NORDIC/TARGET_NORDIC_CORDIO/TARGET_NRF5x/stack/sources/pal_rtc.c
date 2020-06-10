/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Tickless timer implementation.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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
 *
 * Notes:
 *
 *     This is timer driver is used by wsf_timer.c and pal_timer.c.
 *
 */
/*************************************************************************************************/

#include "nrf.h"
#include "pal_rtc.h"
#include "pal_timer.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifdef DEBUG

/*! \brief      Parameter and state check. */
#define PAL_RTC_CHECK(expr)         { if (!(expr)) { while(1); } }

#else

/*! \brief      Parameter and state check (disabled). */
#define PAL_RTC_CHECK(expr)

#endif

#define RTC_TIMER_TOTAL_CHANNEL         4

/**************************************************************************************************
  Variables
**************************************************************************************************/

static palRtcIrqCback_t palRtcTimerCback[RTC_TIMER_TOTAL_CHANNEL];

/*************************************************************************************************/
/*!
 *  \brief  Function for clearing rtc events.
 *
 *  \param  channelId   Channel ID Number.
 */
/*************************************************************************************************/
void PalRtcClearCompareEvents(uint8_t channelId)
{
  PAL_RTC_CHECK(channelId < RTC_TIMER_TOTAL_CHANNEL);

  NRF_RTC1->EVENTS_COMPARE[channelId] = 0;
  (void)NRF_RTC1->EVENTS_COMPARE[channelId];
}

/*************************************************************************************************/
/*!
 *  \brief  Function for starting the RTC timer.
 *
 *  \param  channelId   Channel ID Number.
 */
/*************************************************************************************************/
void PalRtcEnableCompareIrq(uint8_t channelId)
{
  PAL_RTC_CHECK(channelId < RTC_TIMER_TOTAL_CHANNEL);

  PalRtcClearCompareEvents(channelId);

  switch (channelId)
  {
    case 0:
      NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk;
      NRF_RTC1->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
      break;
    case 1:
      NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE1_Msk;
      NRF_RTC1->EVTENSET = RTC_EVTENSET_COMPARE1_Msk;
      break;
    case 2:
      NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE2_Msk;
      NRF_RTC1->EVTENSET = RTC_EVTENSET_COMPARE2_Msk;
      break;
    case 3:
      NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE3_Msk;
      NRF_RTC1->EVTENSET = RTC_EVTENSET_COMPARE3_Msk;
      break;
    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Function for stopping the RTC timer.
 *
 *  \param  channelId   Channel ID Number.
 */
/*************************************************************************************************/
void PalRtcDisableCompareIrq(uint8_t channelId)
{
  PAL_RTC_CHECK(channelId < RTC_TIMER_TOTAL_CHANNEL);

  switch (channelId)
  {
    case 0:
      NRF_RTC1->INTENCLR = RTC_INTENCLR_COMPARE0_Msk;
      NRF_RTC1->EVTENCLR = RTC_EVTENCLR_COMPARE0_Msk;
      break;
    case 1:
      NRF_RTC1->INTENCLR = RTC_INTENCLR_COMPARE1_Msk;
      NRF_RTC1->EVTENCLR = RTC_EVTENCLR_COMPARE1_Msk;
      break;
    case 2:
      NRF_RTC1->INTENCLR = RTC_INTENCLR_COMPARE2_Msk;
      NRF_RTC1->EVTENCLR = RTC_EVTENCLR_COMPARE2_Msk;
      break;
    case 3:
      NRF_RTC1->INTENCLR = RTC_INTENCLR_COMPARE3_Msk;
      NRF_RTC1->EVTENCLR = RTC_EVTENCLR_COMPARE3_Msk;
      break;
    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Get the current value of the RTC counter.
 *
 *  \return Current value of the RTC counter.
 */
/*************************************************************************************************/
uint32_t PalRtcCounterGet(void)
{
  NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
  NRF_CLOCK->TASKS_LFCLKSTART     = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0) {}
  NRF_RTC1->TASKS_STOP = 0;

  return NRF_RTC1->COUNTER;
}

/*************************************************************************************************/
/*!
 *  \brief  Set the RTC capture compare value.
 *
 *  \param  channelId   Channel ID Number.
 *  \param  value   Set new value for compare value.
 */
/*************************************************************************************************/
void PalRtcCompareSet(uint8_t channelId, uint32_t value)
{
  PAL_RTC_CHECK(channelId < RTC_TIMER_TOTAL_CHANNEL);
  NRF_RTC1->CC[channelId] = value;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the current value of the RTC capture compare.
 *
 *  \param  channelId   Channel ID Number.
 *
 *  \return Current value of the capture compare.
 */
/*************************************************************************************************/
uint32_t PalRtcCompareGet(uint8_t channelId)
{
  PAL_RTC_CHECK(channelId < RTC_TIMER_TOTAL_CHANNEL);
  return NRF_RTC1->CC[channelId];
}

/*************************************************************************************************/
/*!
 *  \brief  RTC interrupt handler.
 *
 *  This handler stores the RTC start time which is used as a reference to compute the receive
 *  packet timestamp using the HFCLK.
 *
 */
/*************************************************************************************************/
void RTC1_IRQHandler(void)
{
  for (unsigned int channelId = 0; channelId < RTC_TIMER_TOTAL_CHANNEL; channelId++)
  {
    if (NRF_RTC1->EVENTS_COMPARE[channelId])
    {
      PalRtcClearCompareEvents(channelId);

      if (palRtcTimerCback[channelId])
      {
        palRtcTimerCback[channelId]();
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Tickless timer initialization routine.
 */
/*************************************************************************************************/
void PalRtcInit(void)
{
  /* Stop RTC to prevent any running timers from expiring. */
  for (unsigned int channelId = 0; channelId < RTC_TIMER_TOTAL_CHANNEL; channelId++)
  {
    PalRtcDisableCompareIrq(channelId);
  }

  /* Configure low-frequency clock. */
  NRF_CLOCK->LFCLKSRC             = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);

  NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
  NRF_CLOCK->TASKS_LFCLKSTART     = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0) { }
  NRF_RTC1->TASKS_STOP = 0;

  NRF_RTC1->TASKS_STOP  = 1;
  NRF_RTC1->TASKS_CLEAR = 1;
  NRF_RTC1->PRESCALER   = 0;            /* clear prescaler */
  NRF_RTC1->TASKS_START = 1;

  NVIC_SetPriority(RTC1_IRQn, 0x80);    /* medium priority */
  NVIC_ClearPendingIRQ(RTC1_IRQn);
  NVIC_EnableIRQ(RTC1_IRQn);
}

/*************************************************************************************************/
/*!
 *  \brief  Register rtc IRQ callback.
 *
 *  \param  channelId   Channel ID Number.
 *  \param  cback       Call back for ISR.
 *
 *  This callback is dedicated for scheduler timer in low power code.
 *
 */
/*************************************************************************************************/
void PalRtcIrqRegister(uint8_t channelId, palRtcIrqCback_t cback)
{
  PAL_RTC_CHECK(channelId < RTC_TIMER_TOTAL_CHANNEL);
  palRtcTimerCback[channelId] = cback;
}
