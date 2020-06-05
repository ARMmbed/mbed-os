/*************************************************************************************************/
/*!
 *  \file   wsf_cs.c
 *
 *  \brief  Software foundation OS main module.
 *
 *  Copyright (c) 2009-2019 Arm Ltd. All Rights Reserved.
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
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_cs.h"

#include "wsf_assert.h"
#include "pal_sys.h"

#if (WSF_CS_STATS == TRUE)
#include "pal_bb.h"
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief  Critical section nesting level. */
uint8_t wsfCsNesting = 0;

#if (WSF_CS_STATS == TRUE)

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
 *  \brief  Mark the beginning of a CS.
 */
/*************************************************************************************************/
static void wsfCsStatsEnter(void)
{
  /* N.B. Code path must not use critical sections. */

  wsfCsStatsStartTimeValid = PalBbGetTimestamp(&wsfCsStatsStartTime);
}

/*************************************************************************************************/
/*!
 *  \brief  Record the CS watermark.
 */
/*************************************************************************************************/
static void wsfCsStatsExit(void)
{
  /* N.B. Code path must not use critical sections. */

  if (wsfCsStatsStartTimeValid != TRUE)
  {
    return;
  }

  uint32_t exitTime;

  if (PalBbGetTimestamp(&exitTime))
  {
    uint32_t durUsec = exitTime - wsfCsStatsStartTime;
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
 */
/*************************************************************************************************/
void WsfCsEnter(void)
{
  if (wsfCsNesting == 0)
  {
    PalEnterCs();

#if (WSF_CS_STATS == TRUE)
    wsfCsStatsEnter();
#endif
  }
  wsfCsNesting++;
}

/*************************************************************************************************/
/*!
 *  \brief  Exit a critical section.
 */
/*************************************************************************************************/
void WsfCsExit(void)
{
  WSF_ASSERT(wsfCsNesting != 0);

  wsfCsNesting--;
  if (wsfCsNesting == 0)
  {
#if (WSF_CS_STATS == TRUE)
    wsfCsStatsExit();
#endif

    PalExitCs();
  }
}
