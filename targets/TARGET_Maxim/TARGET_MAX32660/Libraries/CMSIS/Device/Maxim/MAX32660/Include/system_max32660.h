/**
 * @file    system_max32660.h
 * @brief   System-specific header file
 */


/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *
 ******************************************************************************/

#ifndef _SYSTEM_MAX32660_H_
#define _SYSTEM_MAX32660_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

#ifndef HFX_FREQ
#define HFX_FREQ                32768
#endif

#ifndef NANORING_FREQ
#define NANORING_FREQ           8000
#endif

#ifndef HIRC96_FREQ
#define HIRC96_FREQ             96000000
#endif

extern uint32_t SystemCoreClock;    /*!< System Clock Frequency (Core Clock)  */
#ifndef PeripheralClock
#define         PeripheralClock     (SystemCoreClock /2)    /*!< Peripheral Clock Frequency */
#endif

/*
 * Initialize the system
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit(void);

/*
 * Update SystemCoreClock variable
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_MAX32660_H_ */
