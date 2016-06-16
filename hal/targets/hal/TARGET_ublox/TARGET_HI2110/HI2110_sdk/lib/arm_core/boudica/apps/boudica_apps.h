/**************************************************************************//**
 * @file     boudica_apps.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File for
 *           Device Boudica Apps
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include "neulfw.h"
#ifndef BOUDICA_APPS_H
#define BOUDICA_APPS_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup  Boudica_Apps_Definitions
  This file defines all structures and symbols for Boudica Apps:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup Boudica_Apps_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M# Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  Thread_mode                   = -16,      /*!<  0 Thread mode                                   */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  HardFault_IRQn                = -13,      /*!<  3 Hard Fault Interrupt                          */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

/******  Device Specific Interrupt Numbers ********************************************************/
  RTC_IRQn                      = 0,        /*!< RTC Interrupt                                    */
  Timer_IRQn                    = 1,        /*!< Timer Interrupt                                  */
  Security_IRQn                 = 2,        /*!< From Security Interrupt                          */
  Protocol_IRQn                 = 3,        /*!< From Protocol Interrupt                          */
  Apps_IRQn                     = 4,        /*!< Core Self Interrupt                              */
  GPIO_IRQn                     = 5,        /*!< GPIO Interrupt                                   */
  DMA_IRQn                      = 6,        /*!< DMA Interrupt                                    */
  UART0_IRQn                    = 7,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 8,        /*!< UART1 Interrupt                                  */
  SSP0_IRQn                     = 9,        /*!< SPI0 Interrupt                                   */
  SSP1_IRQn                     = 10,       /*!< SPI1 Interrupt                                   */
  PWM0_Inner_IRQn               = 11,       /*!< PW0 Inner Interrupt                              */
  PWM0_Outer_IRQn               = 12,       /*!< PW0 Outer Interrupt                              */
  PWM1_Inner_IRQn               = 13,       /*!< PW1 Inner Interrupt                              */
  PWM1_Outer_IRQn               = 14,       /*!< PW1 Outer Interrupt                              */
  I2C_IRQn                      = 15,       /*!< I2C Interrupt                                    */
  LPUART_IRQn                   = 16,       /*!< Low Power UART Interrupt                         */
  CAP_IRQn                      = 17,       /*!< CAP Interrupt                                    */
  COMP_IRQn                     = 18,       /*!< COMP Interrupt                                   */
  EDGE_IRQn                     = 19,       /*!< EDGE Interrupt                                   */
  Pulse_SWD_IRQn                = 23,       /*!< SWD Pulse Interrupt                              */

} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M# Processor and Core Peripherals */
#define __CM0_REV                 0x0000    /*!< Core Revision r2p1                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __FPU_PRESENT             0        /*!< FPU present or not                                */

/*@}*/ /* end of group Boudica_Apps_CMSIS */

#include <core_cm0.h>                       /* Cortex-M# processor and core peripherals */
/* "system_boudica_apps.h" not created because Initialization is done in code  */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup Boudica_Apps_Peripherals Boudica_Apps Peripherals
  Boudica_Apps Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*------------- DMA -----------------------------*/
#define DMA_CHANS (4)

// DMA registers
typedef struct
{
    uint32 DMA_STATUS;
    uint32 DMA_CFG;
    uint32 CTRL_BASE_PTR;
    uint32 ALT_CTRL_BASE_PTR;
    uint32 DMA_WAITONREQ_STATUS;
    uint32 CHNL_SW_REQEST;
    uint32 CHNL_USEBURST_SET;
    uint32 CHNL_USEBURST_CLR;
    uint32 CHNL_REQ_MASK_SET;
    uint32 CHNL_REQ_MASK_CLR;
    uint32 CHNL_ENABLE_SET;
    uint32 CHNL_ENABLE_CLR;
    uint32 CHNL_PRI_ALT_SET;
    uint32 CHNL_PRI_ALT_CLR;
    uint32 CHNL_PRIORITY_SET;
    uint32 CHNL_PRIORITY_CLR;
    uint32 res0;
    uint32 res1;
    uint32 res2;
    uint32 ERR_CLR;
}dma_ctrl_t;

typedef struct
{
    uint32 PerID0;
    uint32 PerID1;
    uint32 PerID2;
    uint32 PerID3;
    uint32 PCeID0;
    uint32 PCeID1;
    uint32 PCeID2;
    uint32 PCeID3;
}dma_id_t;

// Channel Control structure
typedef struct
{
    uint32 SrcEndAddr;
    uint32 DestEndAddr;
    volatile uint32 Ctrl;
    uint32 Empty;
}dma_ch_ctrl_t;


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/*@}*/ /* end of group Boudica_Apps_Peripherals */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* ToDo: add here your device peripherals base addresses
         following is an example for timer                                    */
/** @addtogroup Boudica_Apps_MemoryMap Boudica_Apps Memory Mapping
  @{
*/

// Added in a separate file

/*@}*/ /* end of group Boudica_Apps_MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer                                    */

/** @addtogroup Boudica_Apps_PeripheralDecl Boudica_Apps Peripheral Declaration
  @{
*/

/* DMA Defines */
#define DMA_BASE_ADDR 0x50000000

/* UART Defines */
#define UART0_BASE 0x50003000
#define UART1_BASE 0x50004000

/*@}*/ /* end of group Boudica_Apps_Definitions */

#ifdef __cplusplus
}
#endif

#endif  /* BOUDICA_APPS_H */
