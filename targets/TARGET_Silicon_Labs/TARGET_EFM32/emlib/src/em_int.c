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

#include <stdint.h>
#include "em_int.h"

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup INT
 * @brief Safe nesting of interrupt disable/enable API
 * @{
 * @deprecated
 *   These functions are deprecated and marked for removal in a later release.
 *   Use the @ref CORE module instead. See @ref core_porting for
 *   information on how to convert existing code bases to use @ref CORE.
 *
 * @details
 *  This module contains functions to safely disable and enable interrupts
 *  at the CPU level. INT_Disable() disables interrupts globally and increments a lock
 *  level counter (counting semaphore). INT_Enable() decrements the lock level
 *  counter and enables interrupts if the counter reaches zero.
 *
 *  These functions would normally be used to secure critical regions and
 *  to make sure that a critical section that calls into another critical
 *  section does not unintentionally terminate the callee critical section.
 *
 *  These functions should also be used inside interrupt handlers:
 *  @verbatim
 *  void SysTick_Handler(void)
 *  {
 *    INT_Disable();
 *      .
 *      .
 *      .
 *    INT_Enable();
 *  }
 * @endverbatim
 ******************************************************************************/

/** An interrupt lock level counter. Set to zero initially because main is
 *  normally entered with interrupts enabled.  */
uint32_t INT_LockCnt = 0;

/** @} (end addtogroup INT) */
/** @} (end addtogroup emlib) */
