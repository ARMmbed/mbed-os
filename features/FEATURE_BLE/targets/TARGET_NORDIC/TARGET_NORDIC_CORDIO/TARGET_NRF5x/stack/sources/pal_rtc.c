/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Tickless timer implementation.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#include "nrf.h"
#include "stack/platform/include/pal_rtc.h"

/*************************************************************************************************/
/*!
 *  \brief  Function for starting the RTC timer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalRtcEnableCompareIrq(void)
{
  NRF_RTC0->EVENTS_COMPARE[0] = 0;
  NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;
  NRF_RTC0->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
}

/*************************************************************************************************/
/*!
 *  \brief  Function for stopping the RTC timer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalRtcDisableCompareIrq(void)
{
  NRF_RTC0->INTENCLR = RTC_INTENCLR_COMPARE0_Msk;
  NRF_RTC0->EVTENCLR = RTC_EVTENCLR_COMPARE0_Msk;
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
  NRF_RTC0->TASKS_STOP = 0;

  return NRF_RTC0->COUNTER;
}

/*************************************************************************************************/
/*!
 *  \brief  Set the RTC capture compare value.
 *
 *  \param  value   Set new value for compare value.
 *
 *  \return None
 */
/*************************************************************************************************/
void PalRtcCompareSet(uint32_t value)
{
  NRF_RTC0->CC[0] = value;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the current value of the RTC capture compare.
 *
 *  \return Current value of the capture compare.
 */
/*************************************************************************************************/
uint32_t PalRtcCompareGet(void)
{
  return NRF_RTC0->CC[0];
}

/*************************************************************************************************/
/*!
 *  \brief  RTC interrupt handler.
 *
 *  \return None.
 *
 *  This handler stores the RTC start time which is used as a reference to compute the receive
 *  packet timestamp using the HFCLK.
 *
 */
/*************************************************************************************************/
void RTC0_IRQHandler(void)
{
  if (NRF_RTC0->EVENTS_COMPARE[0])
  {
    /* clear the interrupt */
    NRF_RTC0->EVENTS_COMPARE[0] = 0;
    (void)NRF_RTC0->EVENTS_COMPARE[0];    /* wait for write buffer to empty */
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Tickless timer initialization routine.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalRtcInit(void)
{
  /* Stop RTC to prevent any running timers from expiring. */
  PalRtcDisableCompareIrq();

  NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
  NRF_CLOCK->TASKS_LFCLKSTART     = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0) {}
  NRF_RTC0->TASKS_STOP = 0;

  NRF_RTC0->TASKS_STOP  = 1;
  NRF_RTC0->TASKS_CLEAR = 1;
  NRF_RTC0->PRESCALER   = 0;            /* clear prescaler */
  NRF_RTC0->TASKS_START = 1;

  NVIC_SetPriority(RTC0_IRQn, 0x80);    /* medium priority */
  NVIC_ClearPendingIRQ(RTC0_IRQn);
  NVIC_EnableIRQ(RTC0_IRQn);
}
