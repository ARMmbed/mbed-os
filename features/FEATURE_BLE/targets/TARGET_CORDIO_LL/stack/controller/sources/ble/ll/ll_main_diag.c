/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) test interface implementation file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
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

#include "ll_api.h"
#include "lmgr_api.h"
#include "bb_api.h"
#include "bb_drv.h"

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

  *pTime = BbDrvGetCurrentTime();

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
