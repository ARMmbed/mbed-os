/***************************************************************************//**
 * @file
 * @brief Interrupt enable/disable unit API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef EM_INT_H
#define EM_INT_H

#include "em_device.h"

extern uint32_t INT_LockCnt;

#ifdef __cplusplus
extern "C" {
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#ifndef UINT32_MAX
#define UINT32_MAX ((uint32_t)(0xFFFFFFFF))
#endif

#warning "The INT module is deprecated and marked for removal in a later release. Please use the new CORE module instead. See \"Porting from em_int\" in the CORE documentation for instructions."

/** @endcond */

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup INT
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Disable interrupts.
 *
 * @deprecated
 *   Deprecated and marked for removal in a later release.
 *   Use new CORE module instead.
 *
 * @details
 *   Disable interrupts and increment lock level counter.
 *
 * @return
 *   Resulting interrupt disable nesting level.
 *
 ******************************************************************************/
__STATIC_INLINE uint32_t INT_Disable(void)
{
  __disable_irq();
  if (INT_LockCnt < UINT32_MAX) {
    INT_LockCnt++;
  }

  return INT_LockCnt;
}

/***************************************************************************//**
 * @brief
 *   Enable interrupts.
 *
 * @deprecated
 *   Deprecated and marked for removal in a later release.
 *   Use the new CORE module instead.
 *
 * @return
 *   Resulting interrupt disable nesting level.
 *
 * @details
 *   Decrement interrupt lock level counter and enable interrupts if counter
 *   reached zero.
 *
 ******************************************************************************/
__STATIC_INLINE uint32_t INT_Enable(void)
{
  uint32_t retVal;

  if (INT_LockCnt > 0) {
    INT_LockCnt--;
    retVal = INT_LockCnt;
    if (retVal == 0) {
      __enable_irq();
    }
    return retVal;
  } else {
    return 0;
  }
}

/** @} (end addtogroup INT) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif /* EM_INT_H */
