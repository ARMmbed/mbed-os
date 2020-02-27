/***************************************************************************//**
 * @file
 * @brief Debug (DBG) API
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

#ifndef EM_DBG_H
#define EM_DBG_H

#include <stdbool.h>
#include "em_device.h"

#if defined(CoreDebug_DHCSR_C_DEBUGEN_Msk)

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup DBG
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Lock modes */
typedef enum {
  dbgLockModeAllowErase = 1UL, /**< Lock debug access. */
#if !defined(_SILICON_LABS_32B_SERIES_0)
  dbgLockModePermanent  = 2UL  /**< Lock debug access permanently. */
#endif
} DBG_LockMode_TypeDef;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

#if defined(GPIO_ROUTE_SWCLKPEN)        \
  || defined(GPIO_ROUTEPEN_SWCLKTCKPEN) \
  || defined(GPIO_DBGROUTEPEN_SWCLKTCKPEN)
/***************************************************************************//**
 * @brief
 *   Check if a debugger is connected (and debug session activated).
 *
 * @details
 *   Used to make run-time decisions depending on whether or not a debug session
 *   has been active since last reset, i.e., using a debug probe or similar. In
 *   some cases, special handling is required in that scenario.
 *
 * @return
 *   True if a debug session is active since last reset, otherwise false.
 ******************************************************************************/
__STATIC_INLINE bool DBG_Connected(void)
{
  return (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) ? true : false;
}
#endif

#if defined(GPIO_ROUTE_SWOPEN)     \
  || defined(GPIO_ROUTEPEN_SWVPEN) \
  || defined(GPIO_TRACEROUTEPEN_SWVPEN)
void DBG_SWOEnable(unsigned int location);
#endif

void DBG_DisableDebugAccess(DBG_LockMode_TypeDef lockMode);

#if defined (EMU_CTRL_EM2DBGEN)
/***************************************************************************//**
 * @brief
 *   Enable or disable debug support while in EM2 mode.
 *
 * @warning
 *   Disabling debug support in EM2 will reduce current consumption with 1-2 uA,
 *   but some debuggers will have problems regaining control over a device which
 *   is in EM2 and has debug support disabled.
 *
 *   To remedy this, set the WSTK switch next to the battery holder to USB
 *   (powers down the EFR). Execute Simplicity Commander with command line
 *   parameters:
 *     "./commander.exe device recover"
 *   and then immediately move the switch to the AEM postion. An additional
 *     "./commander.exe device masserase"
 *   command completes the recovery procedure.
 *
 * @param[in] enable
 *   Boolean true enables EM2 debug support, false disables.
 ******************************************************************************/
__STATIC_INLINE void DBG_EM2DebugEnable(bool enable)
{
  if (enable) {
    EMU->CTRL_SET = EMU_CTRL_EM2DBGEN;
  } else {
    EMU->CTRL_CLR = EMU_CTRL_EM2DBGEN;
  }
}
#endif

/** @} (end addtogroup DBG) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif /* defined( CoreDebug_DHCSR_C_DEBUGEN_Msk ) */

#endif /* EM_DBG_H */
