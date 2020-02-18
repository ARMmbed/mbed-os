/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016-2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
*/

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#if !defined(MBED_ROM_START)
#define MBED_ROM_START  0x8000000
#endif

#if !defined(MBED_ROM_SIZE)
#define MBED_ROM_SIZE  0x80000  // 512 KB
 //  0x40000 STM32G471RCTx STM32G471CCUx STM32G471VCHx STM32G471CCTx STM32G471VCIx STM32G471VCTx STM32G471MCTx STM32G471QCTx
 //  0x80000 STM32G471RE STM32G471VETx STM32G471METx STM32G471VEHx STM32G471CEUx STM32G471QETx STM32G471MEYx STM32G471VEIx STM32G471CETx
#warning "check MBED_ROM_SIZE value in cmsis_nvic.h"
#endif

#if !defined(MBED_RAM_START)
#define MBED_RAM_START  0x20000000
#endif

#if !defined(MBED_RAM_SIZE)
#define MBED_RAM_SIZE  0x20000  // 128 KB
#endif

#define NVIC_NUM_VECTORS        118
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_START

#endif
