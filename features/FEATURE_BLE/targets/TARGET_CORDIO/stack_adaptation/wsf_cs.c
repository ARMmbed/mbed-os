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

#ifdef __IAR_SYSTEMS_ICC__
#include <intrinsics.h>
#endif
#include "wsf_types.h"
#include "wsf_cs.h"
#include "wsf_assert.h"
#include "wsf_mbed_os_adaptation.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief  Critical section nesting level. */
uint8_t wsfCsNesting = 0;

#if (WSF_CS_STATS == TRUE)

/*! \brief  Critical section entrance callback. */
WsfCsTimestamp_t wsfCsTimestampCback = NULL;

/*! \brief  Critical section exit callback. */
WsfCsTimebase_t wsfCsTimebaseCback = NULL;

/*! \brief      Critical section start time. */
static uint32_t wsfCsStatsStartTime = 0;

/*! \brief      Critical section start time valid. */
static bool_t wsfCsStatsStartTimeValid = FALSE;

/*! \brief  Critical section duration watermark level. */
uint16_t wsfCsStatsWatermarkUsec = 0;

#endif

#if (WSF_CS_STATS == TRUE)

/*************************************************************************************************/
/*!
 *  \brief  Get critical section duration watermark level.
 *
 *  \return Critical section duration watermark level.
 */
/*************************************************************************************************/
uint32_t WsfCsStatsGetCsWaterMark(void)
{
  return wsfCsStatsWatermarkUsec;
}

/*************************************************************************************************/
/*!
 *  \brief  Register critical section statistics hooks.
 *
 *  \param  timestampCback  Callback for obtaining the current timestamp.
 *  \param  timebaseCback   Callback for converting timestamp delta into microseconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsStatsRegister(WsfCsTimestamp_t timestampCback, WsfCsTimebase_t timebaseCback)
{
  WSF_ASSERT(timestampCback != NULL);
  WSF_ASSERT(timebaseCback != NULL);

  wsfCsTimestampCback = timestampCback;
  wsfCsTimebaseCback = timebaseCback;
  wsfCsStatsWatermarkUsec = 0;
}

/*************************************************************************************************/
/*!
 *  \brief  Mark the beginning of a CS.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void wsfCsStatsEnter(void)
{
  /* N.B. Code path must not use critical sections. */

  if (wsfCsTimestampCback)
  {
    wsfCsStatsStartTimeValid = wsfCsTimestampCback(&wsfCsStatsStartTime);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Record the CS watermark.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void wsfCsStatsExit(void)
{
  /* N.B. Code path must not use critical sections. */

  if (wsfCsStatsStartTimeValid != TRUE)
  {
    return;
  }

  /* Valid wsfCsStatsStartTimeValid assumes valid WsfCsStatsRegister(). */
  WSF_ASSERT(wsfCsTimestampCback != NULL);
  WSF_ASSERT(wsfCsTimebaseCback != NULL);

  uint32_t exitTime;

  if (wsfCsTimestampCback(&exitTime))
  {
    uint32_t durUsec = wsfCsTimebaseCback(exitTime - wsfCsStatsStartTime);
    if (durUsec > wsfCsStatsWatermarkUsec)
    {
      wsfCsStatsWatermarkUsec = durUsec;
    }
  }
}

#endif

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsEnter(void)
{
  wsf_mbed_os_critical_section_enter();
}

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsExit(void)
{
  wsf_mbed_os_critical_section_exit();
}
