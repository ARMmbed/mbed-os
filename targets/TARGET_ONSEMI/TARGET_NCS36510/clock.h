/**
******************************************************************************
* @file clock.h
* @brief Header of clock hw module functions
* @internal
* @author ON Semiconductor
* $Rev: 3414 $
* $Date: 2015-06-05 13:27:04 +0530 (Fri, 05 Jun 2015) $
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
* @ingroup clock
*/

#ifndef CLOCK_H_
#define CLOCK_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "types.h"

/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/

/** Peripherals clock disable defines  /
 * @details
 */
#define CLOCK_TIMER0            (0x0)  /**< <b> Timer 0 clock enable offset </b>*/
#define CLOCK_TIMER1            (0x1)  /**< <b> Timer 1 clock enable offset </b>: */
#define CLOCK_TIMER2            (0x2)  /**< <b> Timer 2 clock enable offset </b>: */
#define CLOCK_PAD0_0            (0x3)  /**< <b> Unused offset </b> */
#define CLOCK_PAD0_1            (0x4)  /**< <b> Unused offset </b> */
#define CLOCK_UART1             (0x5)  /**< <b> UART 1 clock enable offset </b> */
#define CLOCK_SPI               (0x6)  /**< <b> SPI clock enable offset </b> */
#define CLOCK_I2C               (0x7)  /**< <b> I2C clock enable offset </b> */
#define CLOCK_UART2             (0x8)  /**< <b> UART 2 clock enable offset </b> */
#define CLOCK_SPI2              (0x9)  /**< <b> Unused offset </b>: */
#define CLOCK_WDOG              (0xA)  /**< <b> Watchdog clock enable offset </b> */
#define CLOCK_PWM               (0xB)  /**< <b> PWM clock enable offset </b> */
#define CLOCK_GPIO              (0xC)  /**< <b> GPIO clock enable offset </b> */
#define CLOCK_I2C2              (0xD)  /**< <b> Unused offset </b> */
#define CLOCK_PAD2_1            (0xE)  /**< <b> Unused offset </b> */
#define CLOCK_RTC               (0xF)  /**< <b> RTC clock enable offset </b> */
#define CLOCK_CROSSB            (0x10) /**< <b> Crossbar clock enable offset </b> */
#define CLOCK_RAND              (0x11) /**< <b> Randomizer clock enable offset </b> */
#define CLOCK_PAD3_0            (0x12) /**< <b> Unused offset </b> */
#define CLOCK_PAD3_1            (0x13) /**< <b> Unused offset </b> */
#define CLOCK_MACHW             (0x14) /**< <b> macHw clock enable offset </b> */
#define CLOCK_ADC               (0x15) /**< <b> ADC clock enable offset </b> */
#define CLOCK_AES               (0x16) /**< <b> AES clock enable offset </b> */
#define CLOCK_FLASH             (0x17) /**< <b> Flash controller clock enable offset</b> */
#define CLOCK_PAD4_0            (0x18) /**< <b> Unused offset </b> */
#define CLOCK_RFANA             (0x19) /**< <b> rfAna clock enable offset </b> */
#define CLOCK_IO                (0x1A) /**< <b> IO clock enable offset </b> */
#define CLOCK_PAD5_0            (0x1B) /**< <b> Unused offset </b> */
#define CLOCK_PAD               (0x1C) /**< <b> Pad clock enable offset </b> */
#define CLOCK_PMU               (0x1D) /**< <b> Pmu clock enable offset </b> */
#define CLOCK_DMA               (0x1E) /**< <b> DMA clock enable offset </b> */
#define CLOCK_TEST              (0x1F) /**< <b> Test controller clock enable offset </b> */

#define CLOCK_ENABLE(a)         CLOCKREG->PDIS.WORD &= ~(1 << a)
#define CLOCK_DISABLE(a)        CLOCKREG->PDIS.WORD |= (uint32_t)(1 << a)
#define CLOCK_IS_ENABLED(a)     (((CLOCKREG->PDIS.WORD >> a) & 1)?0:1)

/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/

/** Function to initialize clocks
 * @details
 * The function initializes clocks.
 * This initialization includes:
 * - Enable of external 32mHz oscillator
 * - Disable of all peripheral clocks (to be turned on selectively when used later in the application)
 * - Setting core frequency
 */
void fClockInit(void);

/** Function to get peripheral clock frequency
 * @details
 * The function checks and returns peripheral clock frequency
 * @return Peripheral clock frequency
 */
uint32_t fClockGetPeriphClockfrequency();

/** Function to get peripheral clock frequency
 * @details
 * The function checks which input clock is sourcing 32kHz clock domain.
 * This domain can be either sourced by:
 * - Internal 32kHz oscillator
 * - External 32.768kHz oscillator
 * @return 32kHz clock domain frequency
 */
uint16_t fClockGet32kClockfrequency();

/** Function to enable peripheral clock
 * @details
 * The function enables clock of peripheral indicated by parameter
 * @param deviceId Peripheral ID whose clock must be enabled.
 */
void fClockEnablePeriph(uint8_t deviceId);

/** Function to disable peripheral clock
 * @details
 * The function disables clock of peripheral indicated by parameter
 * @param deviceId ID Peripheral whose clock must be disabled.
 */
void fClockDisablePeriph(uint8_t deviceId);

#endif /* CLOCK_H_ */
