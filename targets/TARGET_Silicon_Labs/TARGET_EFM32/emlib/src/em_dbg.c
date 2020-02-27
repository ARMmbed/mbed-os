/***************************************************************************//**
 * @file
 * @brief Debug (DBG) Peripheral API
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

#include "em_dbg.h"

#if defined(CoreDebug_DHCSR_C_DEBUGEN_Msk)

#include "em_assert.h"
#include "em_cmu.h"
#include "em_gpio.h"

#include "em_assert.h"
#include "em_se.h"
#if !defined(_SILICON_LABS_32B_SERIES_2)
#include "em_msc.h"
#endif
/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup DBG
 * @brief Debug (DBG) Peripheral API
 * @details
 *  This module contains functions to control the DBG peripheral of Silicon
 *  Labs 32-bit MCUs and SoCs. The Debug Interface is used to program and debug
 *  Silicon Labs devices.
 * @{
 ******************************************************************************/

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

#if defined(GPIO_ROUTE_SWOPEN) || defined(GPIO_ROUTEPEN_SWVPEN) \
  || defined(GPIO_TRACEROUTEPEN_SWVPEN)
/***************************************************************************//**
 * @brief
 *   Enable Serial Wire Output (SWO) pin.
 *
 * @details
 *   The SWO pin (sometimes denoted SWV, serial wire viewer) allows for
 *   miscellaneous output to be passed from the Cortex-M3 debug trace module to
 *   an external debug probe. By default, the debug trace module and pin output
 *   may be disabled.
 *
 *   Since the SWO pin is only useful when using a debugger, a suggested use
 *   of this function during startup may be:
 * @verbatim
 * if (DBG_Connected())
 * {
 * DBG_SWOEnable(1);
 * }
 * @endverbatim
 *   By checking if the debugger is attached, a setup leading to a higher energy
 *   consumption when the debugger is attached can be avoided when not using
 *   a debugger.
 *
 *   Another alternative may be to set the debugger tool chain to configure
 *   the required setup (similar to the content of this function) by some
 *   sort of toolchain scripting during its attach/reset procedure. In that
 *   case, the above suggested code for enabling the SWO pin is not required
 *   in the application.
 *
 * @param[in] location
 *   A pin location used for SWO pin on the application in use.
 ******************************************************************************/
void DBG_SWOEnable(unsigned int location)
{
  int port;
  int pin;

#if defined(GPIO_SWV_PORT)

  port = GPIO_SWV_PORT;
  pin = GPIO_SWV_PIN;

#else
  EFM_ASSERT(location < AFCHANLOC_MAX);
  #if defined (AF_DBG_SWO_PORT)
  port = AF_DBG_SWO_PORT(location);
  pin  = AF_DBG_SWO_PIN(location);
  #elif defined (AF_DBG_SWV_PORT)
  port = AF_DBG_SWV_PORT(location);
  pin  = AF_DBG_SWV_PIN(location);

  #else
  #warning "AF debug port is not defined."
  #endif
#endif

  /* Port/pin location not defined for the device. */
  if ((pin < 0) || (port < 0)) {
    EFM_ASSERT(0);
    return;
  }

  /* Ensure that the auxiliary clock going to the Cortex debug trace module is enabled. */
#if !defined(_SILICON_LABS_32B_SERIES_2)
  CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, false);
#endif

  /* Set the selected pin location for the SWO pin and enable it. */
  GPIO_DbgLocationSet(location);
  GPIO_DbgSWOEnable(true);

  /* Configure the SWO pin for output. */
  GPIO_PinModeSet((GPIO_Port_TypeDef)port, pin, gpioModePushPull, 0);
}
#endif

#if defined(LOCKBITS_BASE) && !defined(_EFM32_GECKO_FAMILY)

/***************************************************************************//**
 * @brief
 *   Disable debug access.
 *
 * @if DOXYDOC_S2_DEVICE
 * @details
 *   SE interface is used to disable debug access. By choosing
 *   @ref dbgLockModePermanent, debug access is blocked permanently. SE disables
 *   the device erase command and thereafter disables debug access.
 * @else
 * @details
 *   Debug access is blocked using debug lock word. On series 1 devices,
 *   if @ref dbgLockModePermanent is chosen, debug access is blocked
 *   permanently using AAP lock word.
 * @endif
 * @param[in] lockMode
 *   Debug lock mode to be used.
 *
 * @warning
 *   If @ref dbgLockModePermanent is chosen as the lock mode, the debug port
 *   will be closed permanently and is irreversible.
 ******************************************************************************/
void DBG_DisableDebugAccess(DBG_LockMode_TypeDef lockMode)
{
#if defined(SEMAILBOX_PRESENT)
  if (lockMode == dbgLockModeAllowErase) {
    SE_debugLockApply();
  } else if (lockMode == dbgLockModePermanent) {
    SE_deviceEraseDisable();
    SE_debugLockApply();
  } else {
    /* Invalid input */
    EFM_ASSERT(0);
  }
#else
#if defined(_SILICON_LABS_32B_SERIES_0)
  if (lockMode != dbgLockModeAllowErase) {
    EFM_ASSERT(0);
  }
#else
  if ((lockMode != dbgLockModeAllowErase) && (lockMode != dbgLockModePermanent)) {
    EFM_ASSERT(0);
  }
#endif

  bool wasLocked;
  uint32_t lockWord = 0x0;
  wasLocked = ((MSC->LOCK & _MSC_LOCK_MASK) != 0U);
  MSC_Init();

  uint32_t *dlw = (uint32_t*)(LOCKBITS_BASE + (127 * 4));

  if (*dlw == 0xFFFFFFFF) {
    MSC_WriteWord(dlw, &lockWord, sizeof(lockWord));
  }
#if !defined(_SILICON_LABS_32B_SERIES_0)
  uint32_t *alw = (uint32_t*)(LOCKBITS_BASE + (124 * 4));

  if (lockMode == dbgLockModePermanent) {
    if (*alw == 0xFFFFFFFF) {
      MSC_WriteWord(alw, &lockWord, sizeof(lockWord));
    }
  }
#endif

  if (wasLocked) {
    MSC_Deinit();
  }
#endif
}

#endif /* defined(LOCKBITS_BASE) && !defined(_EFM32_GECKO_FAMILY) */

/** @} (end addtogroup DBG) */
/** @} (end addtogroup emlib) */
#endif /* defined( CoreDebug_DHCSR_C_DEBUGEN_Msk ) */
