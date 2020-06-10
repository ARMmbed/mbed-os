/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  System hooks.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#include "nrf.h"
#include "pal_rtc.h"
#include "pal_sys.h"
#include "pal_led.h"
#include "pal_timer.h"
#include "pal_bb.h"
#include "pal_uart.h"

#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifdef __GNUC__

/*! \brief      Stack initial values. */
#define INIT_STACK_VAL      0xAFAFAFAF

/*! \brief      Starting memory location of free memory. */
#define FREE_MEM_START      ((uint8_t *)&__heap_start__)

/*! \brief      Total size in bytes of free memory. */
#define FREE_MEM_SIZE       ((uint32_t)&__heap_end__ - (uint32_t)&__heap_start__)

extern uint8_t *SystemHeapStart;
extern uint32_t SystemHeapSize;

extern unsigned long __text_end__;
extern unsigned long __data_start__;
extern unsigned long __data_end__;
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;
extern unsigned long __stack_top__;
extern unsigned long __stack_limit__;
extern unsigned long __heap_end__;
extern unsigned long __heap_start__;

#else

/*! \brief      Starting memory location of free memory. */
#define FREE_MEM_START      ((uint8_t *)palSysFreeMem)

/*! \brief      Total size in bytes of free memory. */
#define FREE_MEM_SIZE       (1024 * 196)

#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

#ifdef __GNUC__

uint8_t *SystemHeapStart;
uint32_t SystemHeapSize;

#else

/*! \brief      Free memory for pool buffers (align to word boundary). */
uint32_t palSysFreeMem[FREE_MEM_SIZE/sizeof(uint32_t)];

uint8_t *SystemHeapStart = (uint8_t *) palSysFreeMem;
uint32_t SystemHeapSize = FREE_MEM_SIZE;

#endif

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
 */
/*************************************************************************************************/
void PalEnterCs(void)
{
  #ifdef __IAR_SYSTEMS_ICC__
      __disable_interrupt();
  #endif
  #ifdef __GNUC__
      __asm volatile ("cpsid i");
  #endif
  #ifdef __CC_ARM
      __disable_irq();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief  Exit a critical section.
 */
/*************************************************************************************************/
void PalExitCs(void)
{
  #ifdef __IAR_SYSTEMS_ICC__
      __enable_interrupt();
  #endif
  #ifdef __GNUC__
      __asm volatile ("cpsie i");
  #endif
  #ifdef __CC_ARM
      __enable_irq();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief      Common platform initialization.
 */
/*************************************************************************************************/
void PalSysInit(void)
{
  /* Enable Flash cache */
  NRF_NVMC->ICACHECNF |= (NVMC_ICACHECNF_CACHEEN_Enabled << NVMC_ICACHECNF_CACHEEN_Pos);

  /* Use 16 MHz crystal oscillator (system starts up using 16MHz RC oscillator). */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART    = 1;
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) { }

  palSysAssertCount = 0;
  PalSysAssertTrapEnable = TRUE;
  palSysBusyCount = 0;

  PalRtcInit();

  PalLedInit();
  PalLedOff(PAL_LED_ID_ERROR);
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);

#ifdef DEBUG
  /* Reset free memory. */
  memset(SystemHeapStart, 0, SystemHeapSize);
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      System fault trap.
 */
/*************************************************************************************************/
void PalSysAssertTrap(void)
{
  PalLedOn(PAL_LED_ID_ERROR);

  palSysAssertCount++;

  while (PalSysAssertTrapEnable);
}

/*************************************************************************************************/
/*!
 *  \brief      Set system trap.
 *
 *  \param      enable    Enable assert trap or not.
 */
/*************************************************************************************************/
void PalSysSetTrap(bool_t enable)
{
  PalSysAssertTrapEnable = enable;
}

/*************************************************************************************************/
/*!
 *  \brief      Get assert count.
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
#ifdef __GNUC__
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
 */
/*************************************************************************************************/
void PalSysSleep(void)
{
  /* Clock management for low power mode. */
  #if BB_CLK_RATE_HZ == 32768
    uint32_t rtcNow = NRF_RTC1->COUNTER;

    if ((BbGetCurrentBod() == NULL) && PalUartGetState(PAL_UART_ID_CHCI) == PAL_UART_STATE_UNINIT)
    {
      if ((PalTimerGetState() == PAL_TIMER_STATE_BUSY &&
          ((NRF_RTC1->CC[3] - rtcNow) & PAL_MAX_RTC_COUNTER_VAL) > PAL_HFCLK_OSC_SETTLE_TICKS) ||
          (PalTimerGetState() == PAL_TIMER_STATE_READY))
      {
        /* disable HFCLK */
        NRF_CLOCK->TASKS_HFCLKSTOP = 1;
        NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
        (void)NRF_CLOCK->EVENTS_HFCLKSTARTED;
      }
    }
  #endif

  /* CPU sleep. */
  #ifdef __IAR_SYSTEMS_ICC__
    __wait_for_interrupt();
  #endif
  #ifdef __GNUC__
          __asm volatile ("wfi");
  #endif
  #ifdef __CC_ARM
    __wfi();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief      Check if system is busy.
 *
 *  \return     TRUE if system is busy.
 */
/*************************************************************************************************/
bool_t PalSysIsBusy(void)
{
  bool_t sysIsBusy = FALSE;
  PalEnterCs();
  sysIsBusy = ((palSysBusyCount == 0) ? FALSE : TRUE);
  PalExitCs();
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
  PalEnterCs();
  palSysBusyCount++;
  PalExitCs();
}

/*************************************************************************************************/
/*!
 *  \brief      Set system idle.
 */
/*************************************************************************************************/
void PalSysSetIdle(void)
{
  PalEnterCs();
  if (palSysBusyCount)
  {
    palSysBusyCount--;
  }
  PalExitCs();
}
