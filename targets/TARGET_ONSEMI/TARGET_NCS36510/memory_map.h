/**
******************************************************************************
* @file memory_map.h
* @brief Defines the silicon memory map.  All peripheral devices shall be mapped in structures.
* @internal
* @author ON Semiconductor
* $Rev: 3525 $
* $Date: 2015-07-20 15:24:25 +0530 (Mon, 20 Jul 2015) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup bsp
@verbatim
         +-----------------+
         |                 |             ,_________________________
         | Private Per.    |             |PMUREG         0x4001D000|
0xE0000000 +-----------------+             |PADREG         0x4001C000|
         |                 |_____________|CLOCKREG       0x4001B000|
         |  PERIPHERALS    |             |RFANAREG       0x40019000|
         +-----------------+             |RESETREG       0x40018000|
         |                 |             |FLASHREG       0x40017000|
0x3FFF8000 |SRAM A     32K   |             |AESREG         0x40016000|
         +-----------------+             |ADCREG         0x40015000|
         |                 |             |MACHWREG       0x40014000|
         |SRAM B     16K   |             |RANDREG        0x40011000|
0x3FFF4000 +-----------------+             |CROSSBREG      0x40010000|
         |                 |             |RTCREG         0x4000F000|
0x24000100 |SRAM DMA    7B   |             |GPIOREG        0x4000C000|
         +-----------------+             |PWMREG         0x4000B000|
0x24000000 |SRAM MAC  256B   |             |WDTREG         0x4000A000|
         +-----------------+             |UARTREG        0x40008000|
         |          320K   |             |I2CREG         0x40007000|
0x00102000 |FLASHB           |             |SPIREG         0x40006000|
0x00100000 |FLASHB Inf Block |             |UARTREG        0x40005000|
         +-----------------+             |TIM2REG        0x40002000|
         |          320K   |             |TIM1REG        0x40001000|
0x00002000 |FLASHA           |             |TIM0REG        0x40000000|
0x00000000 |FLASHA Inf Block |             '`''''''''''''''''''''''''
         '`'''''''''''''''''

@endverbatim
*/

#ifndef _MEMORY_MAP_H_
#define _MEMORY_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include <stdint.h>

#include "architecture.h"

// Register maps of HW modules controlled with device drivers
#include "adc_sar_map.h"
#include "aes_map.h"
#include "flash_map.h"
#include "gpio_map.h"
#include "i2c_ipc7208_map.h"
#include "pwm_map.h"
#include "rtc_map.h"
#include "spi_ipc7207_map.h"
#include "timer_map.h"
#include "uart_16c550_map.h"
#include "wdt_map.h"

// Register maps of HW modules controlled with specific functions
#include "clock_map.h"
#include "crossbar_map.h"
#include "dma_map.h"
#include "macHw_map.h"
#include "pad_map.h"
#include "pmu_map.h"
#include "random_map.h"
#include "reset_map.h"
#include "rfAna_map.h"
#include "test_map.h"

// Trim structure map
#include "trim_map.h"

/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/

/** Trim structure mapping
 *
 */
#define TRIMREG_BASE        ((uint32_t)0x1FA0)
#define TRIMREG             ((TrimReg_t *)TRIMREG_BASE)

/** User trim structure mapping
 *
 */
#define USRETRIMREG_BASE     ((uint32_t)0x2800)
#define USERTRIMREG          ((UserTrimReg_t *)USRETRIMREG_BASE)

/** DMA HW Registers Offset */
#define DMAREG_BASE         ((uint32_t)0x24000400)
/** DMA HW Structure Overlay */
#define DMAREG              ((DmaReg_pt)DMAREG_BASE)

/** MAC MATCH HW Registers Offset */
#define MACMATCHREG_BASE    ((uint32_t)0x24000100)
/** MAC MATCH HW Structure Overlay */
#define MACMATCHREG         ((volatile uint8_t *)MACMATCHREG_BASE)

/** MAC RX HW Registers Offset */
#define MACRXREG_BASE       ((uint32_t)0x24000080)
/** MAC RX HW Structure Overlay */
#define MACRXREG            ((volatile uint8_t *)MACRXREG_BASE)

/** MAC TX HW Registers Offset */
#define MACTXREG_BASE       ((uint32_t)0x24000000)
/** MAC TX HW Structure Overlay */
#define MACTXREG            ((volatile uint8_t *)MACTXREG_BASE)

/** TEST Interface for flash HW Registers Offset */
#define TESTNVMREG_BASE     ((uint32_t)0x4001F140)
/** TEST Interface for flash HW Structure Overlay */
#define TESTNVMREG          ((TestNvmReg_pt)TESTNVMREG_BASE)

/** Test Interface for digital HW Registers Offset */
#define TESTDIGREG_BASE     ((uint32_t)0x4001F100)
/** Test Interface for digital HW Structure Overlay */
#define TESTDIGREG          ((TestDigReg_pt)TESTDIGREG_BASE)

/** Test Interface HW Registers Offset */
#define TESTREG_BASE        ((uint32_t)0x4001F000)
/** Test Interface HW Structure Overlay */
#define TESTREG             ((TestReg_pt)TESTREG_BASE)

/** Device option HW Registers Offset */
#define DEVOPTREG_BASE      ((uint32_t)0x4001E000)
/** MAC TX HW Structure Overlay */
#define DEVOPTREG           ((volatile uint32_t *)DEVOPTREG_BASE)

/** PMU HW Registers Offset */
#define PMUREG_BASE         ((uint32_t)0x4001D000)
/** PMU HW Structure Overlay */
#define PMUREG              ((PmuReg_pt)PMUREG_BASE)

/** PAD Control HW Registers Offset */
#define PADREG_BASE         ((uint32_t)0x4001C000)
/** PAD Control HW Structure Overlay */
#define PADREG              ((PadReg_pt)PADREG_BASE)

/** Clock Control HW Registers Offset */
#define CLOCKREG_BASE       ((uint32_t)0x4001B000)
/** Clock Control HW Structure Overlay */
#define CLOCKREG            ((ClockReg_pt)CLOCKREG_BASE)

/** Analogue Trim HW Registers Offset */
#define RFANATRIMREG_BASE   ((uint32_t)0x40019080)
/** Analogue Trim HW Structure Overlay */
#define RFANATRIMREG        ((RfAnaTrimReg_pt)RFANATRIMREG_BASE)

/** Analogue RF HW Registers Offset */
#define RFANAREG_BASE       ((uint32_t)0x40019000)
/** Analogue RF HW Structure Overlay */
#define RFANAREG            ((RfAnaReg_pt)RFANAREG_BASE)

/** Reset Cause HW Registers Offset */
#define RESETREG_BASE       ((uint32_t)0x40018000)
/** Reset Cause HW Structure Overlay */
#define RESETREG            ((ResetReg_pt)RESETREG_BASE)

/** FLASH Control HW Registers Offset */
#define FLASHREG_BASE       ((uint32_t)0x40017000)
/** FLASH Control HW Structure Overlay */
#define FLASHREG            ((FlashReg_pt)FLASHREG_BASE)

/** AES Encryption HW Registers Offset */
#define AESREG_BASE         ((uint32_t)0x40016000)
/** AES Encryption HW Structure Overlay */
#define AESREG              ((AesReg_pt)AESREG_BASE)

/** SAR ADC HW Registers Offset */
#define ADCREG_BASE         ((uint32_t)0x40015000)
/** SAR ADC HW Structure Overlay */
#define ADCREG              ((AdcReg_pt)ADCREG_BASE)

/** Demodulator HW Registers Offset */
#define DMDREG_BASE         ((uint32_t)0x40014100)
/** Demodulator HW Structure Overlay */
#define DMDREG              ((DmdReg_pt)DMDREG_BASE)

/** MAC Control HW Registers Offset */
#define MACHWREG_BASE       ((uint32_t)0x40014000)
/** MAC Control HW Structure Overlay */
#define MACHWREG            ((MacHwReg_pt)MACHWREG_BASE)

/** Random Generator HW Registers Offset */
#define RANDREG_BASE        ((uint32_t)0x40011000)
/** Random Generator HW Structure Overlay */
#define RANDREG             ((RandReg_pt)RANDREG_BASE)

/** Cross Bar HW Registers Offset */
#define CROSSBREG_BASE      ((uint32_t)0x40010000)
/** Cross Bar HW Structure Overlay */
#define CROSSBREG           ((CrossbReg_pt)CROSSBREG_BASE)

/** Real Time Clock HW Registers Offset */
#define RTCREG_BASE         ((uint32_t)0x4000F000)
/** Real Time Clock HW Structure Overlay */
#define RTCREG              ((RtcReg_pt)RTCREG_BASE)

/** GPIO HW Registers Offset */
#define GPIOREG_BASE        ((uint32_t)0x4000C000)
/** GPIO HW Structure Overlay */
#define GPIOREG             ((GpioReg_pt)GPIOREG_BASE)

/** PWM HW Registers Offset */
#define PWMREG_BASE         ((uint32_t)0x4000B000)
/** PWM HW Structure Overlay */
#define PWMREG              ((PwmReg_pt)PWMREG_BASE)

/** Watchdog Timer HW Registers Offset */
#define WDTREG_BASE         ((uint32_t)0x4000A000)
/** Watchdog Timer HW Structure Overlay */
#define WDTREG              ((WdtReg_pt)WDTREG_BASE)

/** UART 2 HW Registers Offset */
#define UART2REG_BASE       ((uint32_t)0x40008000)
/** UART 2 HW Structure Overlay */
#define UART2REG            ((Uart16C550Reg_pt)UART2REG_BASE)

/** I2C HW Registers Offset */
#define I2C1REG_BASE        ((uint32_t)0x40007000)
/** I2C HW Structure Overlay */
#define I2C1REG             ((I2cIpc7208Reg_pt)I2C1REG_BASE)

/** SPI HW Registers Offset */
#define SPI1REG_BASE        ((uint32_t)0x40006000)
/** SPI HW Structure Overlay */
#define SPI1REG             ((SpiIpc7207Reg_pt)SPI1REG_BASE)

/** UART1 HW Registers Offset */
#define UART1REG_BASE       ((uint32_t)0x40005000)
/** UART1 HW Structure Overlay */
#define UART1REG            ((Uart16C550Reg_pt)UART1REG_BASE)

#define UARTREG_BASES       { UART1REG_BASE, UART2REG_BASE}

/** Timer 2 HW Registers Offset */
#define TIM2REG_BASE        ((uint32_t)0x40002000)
/** Timer 2 HW Structure Overlay */
#define TIM2REG             ((TimerReg_pt)TIM2REG_BASE)

/** Timer 1 HW Registers Offset */
#define TIM1REG_BASE        ((uint32_t)0x40001000)
/** Timer 1 HW Structure Overlay */
#define TIM1REG             ((TimerReg_pt)TIM1REG_BASE)

/** Timer 0 HW Registers Offset */
#define TIM0REG_BASE        ((uint32_t)0x40000000)
/** Timer 0 HW Structure Overlay */
#define TIM0REG             ((TimerReg_pt)TIM0REG_BASE)

/** I2C2 HW Registers Offset */
#define I2C2REG_BASE        ((uint32_t)0x4000D000)
/** I2C2 HW Structure Overlay */
#define I2C2REG             ((I2cIpc7208Reg_pt)I2C2REG_BASE)

/** SPI2 HW Registers Offset */
#define SPI2REG_BASE        ((uint32_t)0x40009000)
/** SPI2 HW Structure Overlay */
#define SPI2REG             ((SpiIpc7207Reg_pt)SPI2REG_BASE)

#endif /*_MEMORY_MAP_H_*/
