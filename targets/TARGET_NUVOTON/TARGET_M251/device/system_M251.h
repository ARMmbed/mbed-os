/**************************************************************************//**
 * @file     system_M251.h
 * @version  V0.10
 * @brief    System Setting Header File
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Nuvoton Technology Corp. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#ifndef __SYSTEM_M251_H__
#define __SYSTEM_M251_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __HXT       (12000000UL)              /*!< External Crystal Clock Frequency */
#define __LXT       (32768UL)                 /*!< External Crystal Clock Frequency 32.768KHz */
#define __LIRC      (38400UL)                 /*!< Internal 38.4KHz RC Oscillator Frequency */
#define __MIRC      (4032000UL)               /*!< Internal 4.032M RC Oscillator Frequency */
#define __HIRC      (48000000UL)              /*!< Internal 48M RC Oscillator Frequency */
#define __HSI       (48000000UL)              /*!< PLL default output is 48MHz */
#define __SYS_OSC_CLK     (    ___HSI)        /*!< Main oscillator frequency */

#if MBED_CONF_TARGET_HXT_PRESENT
#define __SYSTEM_CLOCK    (1UL*__HXT)
#else
#define __SYSTEM_CLOCK    (1UL*__HIRC)
#endif

extern uint32_t SystemCoreClock;    /*!< System Clock Frequency (Core Clock)  */
extern uint32_t CyclesPerUs;        /*!< Cycles per micro second              */
extern uint32_t PllClock;           /*!< PLL Output Clock Frequency           */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the micro controller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit(void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate(void);

/**
 * Set UART0 default multi function pin
 *
 * @param  none
 * @return none
 *
 * @brief  The initialization of uart0 default multiple-function pin.
 */
extern void Uart0DefaultMPF(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_M251_H__ */

/*** (C) COPYRIGHT 2018 Nuvoton Technology Corp. ***/
