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
 * \brief Link layer (LL) test interface implementation file.
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lmgr_api.h"
#include "bb_api.h"
#include "pal_bb.h"

/*************************************************************************************************/
/*!
 *  \brief      Get the current FRC time.
 *
 *  \param      pTime   Pointer to return the current time.
 *
 *  \return     Status error code.
 *
 *  Get the current FRC time.
 *
 *  \note       FRC is limited to the same bit-width as the BB clock. Return value is available
 *              only when the BB is active.
 */
/*************************************************************************************************/
uint8_t LlGetTime(uint32_t *pTime)
{
  /* Only use BB clock when the BB is active. */
  if ((lmgrCb.numConnEnabled == 0) &&
      !lmgrCb.advEnabled &&
      !lmgrCb.numExtAdvEnabled &&
      !lmgrCb.numScanEnabled)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  *pTime = PalBbGetCurrentTime(USE_RTC_BB_CLK);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Calculate the difference between two timestamps in microseconds.
 *
 *  \param      endTime     Ending time.
 *  \param      startTime   Starting time.
 *
 *  \return     Time in microseconds.
 *
 *  Calculate the difference between two timestamps in microseconds.
 */
/*************************************************************************************************/
uint32_t LlCalcDeltaTimeUsec(uint32_t endTime, uint32_t startTime)
{
  return BB_TICKS_TO_US(endTime - startTime);
}
