/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  System hooks.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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
#include "stack/platform/include/pal_bb.h"
#include "stack/platform/include/pal_rtc.h"
#include "stack/platform/include/pal_sys.h"
#include "stack/platform/include/pal_led.h"
#include "platform/mbed_power_mgmt.h"

#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

extern uint8_t *SystemHeapStart;
extern uint32_t SystemHeapSize;

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)

/*! \brief      Stack initial values. */
#define INIT_STACK_VAL    0xAFAFAFAF

extern unsigned long __stack_top__;
extern unsigned long __stack_limit__;

#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Number of assertions. */
static uint32_t palSysAssertCount;

/*! \brief      Trap enabled flag. */
static volatile bool_t PalSysAssertTrapEnable;

static uint32_t palSysBusyCount;

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void palEnterCs(void)
{
  #ifdef __IAR_SYSTEMS_ICC__
      __disable_interrupt();
  #endif
  #if defined(__GNUC__) && !defined(__ARMCC_VERSION)
      __asm volatile ("cpsid i");
  #endif
  #if defined(__ARMCC_VERSION)
      __disable_irq();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief  Exit a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
static inline void palExitCs(void)
{
  #ifdef __IAR_SYSTEMS_ICC__
      __enable_interrupt();
  #endif
  #if defined(__GNUC__) && !defined(__ARMCC_VERSION)
      __asm volatile ("cpsie i");
  #endif
  #if defined(__ARMCC_VERSION)
      __enable_irq();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief      Common platform initialization.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysInit(void)
{
  /* enable Flash cache */
  NRF_NVMC->ICACHECNF |= (NVMC_ICACHECNF_CACHEEN_Enabled << NVMC_ICACHECNF_CACHEEN_Pos);

  /* switch to more accurate 16 MHz crystal oscillator (system starts up using 16MHz RC oscillator) */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART    = 1;
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
  {
  }

  /* configure low-frequency clock */
  NRF_CLOCK->LFCLKSRC             = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
  NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
  NRF_CLOCK->TASKS_LFCLKSTART     = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
  {
  }
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;

  /* cycle radio peripheral power to guarantee known radio state */
  NRF_RADIO->POWER = 0;
  NRF_RADIO->POWER = 1;

  palSysAssertCount = 0;
  PalSysAssertTrapEnable = TRUE;
  palSysBusyCount = 0;

  PalRtcInit();

#ifdef DEBUG
  /* Reset free memory. */
  memset(SystemHeapStart, 0, SystemHeapSize);
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      System fault trap.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysAssertTrap(void)
{
  palSysAssertCount++;

  while (PalSysAssertTrapEnable);
}

/*************************************************************************************************/
/*!
 *  \brief      Set system trap.
 *
 *  \param      enable    Enable assert trap or not.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysSetTrap(bool_t enable)
{
  PalSysAssertTrapEnable = enable;
}

/*************************************************************************************************/
/*!
 *  \brief      Get assert count.
 *
 *  \return     None.
 */
/*************************************************************************************************/
uint32_t PalSysGetAssertCount(void)
{
  return palSysAssertCount;
}

/*************************************************************************************************/
/*!
 *  \brief      Count stack usage.
 *
 *  \return     Number of bytes used by the stack.
 */
/*************************************************************************************************/
uint32_t PalSysGetStackUsage(void)
{
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
  unsigned long *pUnused = &__stack_limit__;

  while (pUnused < &__stack_top__)
  {
    if (*pUnused != INIT_STACK_VAL)
    {
      break;
    }

    pUnused++;
  }

  return (uint32_t)(&__stack_top__ - pUnused) * sizeof(*pUnused);
#else
  /* Not available; stub routine. */
  return 0;
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      System sleep.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSleep(void)
{
    sleep();
}

/*************************************************************************************************/
/*!
 *  \brief      Check if system is busy.
 *
 *  \return     True if system is busy.
 */
/*************************************************************************************************/
bool_t PalSysIsBusy(void)
{
  bool_t sysIsBusy = FALSE;
  palEnterCs();
  sysIsBusy = ((palSysBusyCount == 0) ? FALSE : TRUE);
  palExitCs();
  return sysIsBusy;
}

/*************************************************************************************************/
/*!
 *  \brief      Set system busy.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSetBusy(void)
{
  palEnterCs();
  palSysBusyCount++;
  palExitCs();
}

/*************************************************************************************************/
/*!
 *  \brief      Set system idle.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSetIdle(void)
{
  palEnterCs();
  if (palSysBusyCount)
  {
    palSysBusyCount--;
  }
  palExitCs();
}
