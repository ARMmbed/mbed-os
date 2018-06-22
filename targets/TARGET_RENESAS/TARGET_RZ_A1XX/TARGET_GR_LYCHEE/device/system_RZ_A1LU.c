/******************************************************************************
 * @file     system_RZ_A1LU.c
 * @brief    CMSIS Device System Source File for ARM Cortex-A9 Device Series
 * @version  V1.00
 * @date     10 Mar 2017
 *
 * @note
 *
 ******************************************************************************/
/*
 * Copyright (c) 2013-2014 Renesas Electronics Corporation. All rights reserved.
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <RZ_A1LU.h>
#include "RZ_A1_Init.h"
#include "irq_ctrl.h"

#define CS2_SDRAM_MODE_16BIT_CAS2_BR_BW (*(volatile uint16_t*)0x3FFFD040)
#define CS3_SDRAM_MODE_16BIT_CAS2_BR_BW (*(volatile uint16_t*)0x3FFFE040)
#define GPIO_PORT0_BOOTMODE_BITMASK (0x000fu)

/*
 Port 0 (P0) MD pin assignment
 P0_0: MD_BOOT0
 P0_1: MD_BOOT1
 P0_2: MD_CLK
 P0_3: MD_CLKS
 */

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = CM1_RENESAS_RZ_A1_P0_CLK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
  uint32_t freq;
  uint16_t mode;
  uint16_t ifc;

  mode = (GPIO.PPR0 >> 2U) & 0x01U;

  if (mode == 0) {
    /* Clock Mode 0 */
    /* CLKIN is between 10MHz and 13.33MHz */
    /* Divider 1 uses 1/1 ratio, PLL x30 is ON */
    freq = CM0_RENESAS_RZ_A1_CLKIN * 30U;
  } else {
    /* Clock Mode 1 */
    /* CLKIN is 48MHz */
    /* Divider 1 uses 1/4 ratio, PLL x32 is ON */
    freq = (CM1_RENESAS_RZ_A1_CLKIN * 32U) / 4U;
  }

  /* Get CPG.FRQCR[IFC] bits */
  ifc = (CPG.FRQCR >> 8U) & 0x03U;

  /* Determine Divider 2 output clock */
  if (ifc == 0x03U) {
    /* Division ratio is 1/3 */
    freq = (freq / 3U);
  }
  else {
    if (ifc == 0x01U) {
      /* Division ratio is 2/3 */
      freq = (freq * 2U) / 3U;
    }
  }

  SystemCoreClock = freq;
}

/*----------------------------------------------------------------------------
  IRQ Handler Register/Unregister
 *----------------------------------------------------------------------------*/
uint32_t InterruptHandlerRegister (IRQn_Type irq, IRQHandler handler)
{
    return IRQ_SetHandler(irq, handler);
}

uint32_t InterruptHandlerUnregister (IRQn_Type irq)
{
    return IRQ_SetHandler(irq, (IRQHandler_t)NULL);
}

/*----------------------------------------------------------------------------
  System Initialization
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
/*       do not use global variables because this function is called before
         reaching pre-main. RW section may be overwritten afterwards.          */
  
  // Enable SRAM write access
  CPG.SYSCR3 = 0x0F;

  RZ_A1_InitClock();
  RZ_A1_InitBus();

  // Invalidate entire Unified TLB
  __set_TLBIALL(0);

  // Invalidate entire branch predictor array
  __set_BPIALL(0);
  __DSB();
  __ISB();

  //  Invalidate instruction cache and flush branch target cache
  __set_ICIALLU(0);
  __DSB();
  __ISB();

  //  Invalidate data cache
  L1C_InvalidateDCacheAll();

  // Create Translation Table
  MMU_CreateTranslationTable();

  // Enable MMU
  MMU_Enable();

  // Enable Caches
  L1C_EnableCaches();
  L1C_EnableBTAC();

#if (__L2C_PRESENT == 1) 
  L2C_InvAllByWay();
  // Enable L2C
  L2C_Enable();
#endif

#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  // Enable FPU
  __FPU_Enable();
#endif

  // IRQ Initialize
  IRQ_Initialize();
}

void mbed_sdk_init(void) {
    L1C_CleanDCacheAll();
    L1C_InvalidateICacheAll();
}

