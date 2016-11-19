/**************************************************************************/
/**
 * @file     NCS36510.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 *           for CM3 Device Series
 * @version  V1.05
 * @date     26. July 2011
 *
 * @note
 * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#ifndef ARMCM3_H
#define ARMCM3_H

/**
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn {
    /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn     = -14,        /*!<  2 Cortex-M3 Non Maskable Interrupt          */
    HardFault_IRQn          = -13,        /*!<  3 Cortex-M3 Hard Fault Interrupt            */
    MemoryManagement_IRQn   = -12,        /*!<  4 Cortex-M3 Memory Management Interrupt     */
    BusFault_IRQn           = -11,        /*!<  5 Cortex-M3 Bus Fault Interrupt             */
    UsageFault_IRQn         = -10,        /*!<  6 Cortex-M3 Usage Fault Interrupt           */
    SVCall_IRQn             = -5,         /*!< 11 Cortex-M3 SV Call Interrupt               */
    DebugMonitor_IRQn       = -4,         /*!< 12 Cortex-M3 Debug Monitor Interrupt         */
    PendSV_IRQn             = -2,         /*!< 14 Cortex-M3 Pend SV Interrupt               */
    SysTick_IRQn            = -1,         /*!< 15 Cortex-M3 System Tick Interrupt           */

    /******  ARMCM3 specific Interrupt Numbers ********************************************************/
    Tim0_IRQn               = 0,
    Tim1_IRQn               = 1,
    Tim2_IRQn               = 2,
    Uart1_IRQn              = 3,
    Spi_IRQn                = 4,
    I2C_IRQn                = 5,
    Gpio_IRQn               = 6,
    Rtc_IRQn                = 7,
    Flash_IRQn              = 8,
    MacHw_IRQn              = 9,
    Aes_IRQn                = 10,
    Adc_IRQn                = 11,
    ClockCal_IRQn           = 12,
    Uart2_IRQn              = 13,
    Uvi_IRQn                = 14,
    Dma_IRQn                = 15,
    DbgPwrUp_IRQn           = 16,
    Spi2_IRQn               = 17,
    I2C2_IRQn               = 18,
    FVDDHComp_IRQn          = 19
} IRQn_Type;

/**
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/** Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV                 0x0201    /*!< Core Revision r2p1                               */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          4         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

//#define YOTTA_CFG_CMSIS_NVIC_USER_IRQ_OFFSET    16
//#define YOTTA_CFG_CMSIS_NVIC_USER_IRQ_NUMBER    20
//#define NVIC_NUM_VECTORS     (NVIC_USER_IRQ_OFFSET + NVIC_USER_IRQ_NUMBER)

//#define YOTTA_CFG_CMSIS_NVIC_RAM_VECTOR_ADDRESS
//#define YOTTA_CFG_CMSIS_NVIC_FLASH_VECTOR_ADDRESS    0x3000

#include <core_cm3.h>                       /* Cortex-M3 processor and core peripherals           */
#include "system_NCS36510.h"                  /* System Header                                      */

#endif  /* ARMCM3_H */
