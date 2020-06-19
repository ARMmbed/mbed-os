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
#define MBED_ROM_SIZE  0x20000  // 128 KB
 //   0x8000 STM32G431M6Tx STM32G431K6Tx STM32G431C6Ux STM32G431R6Tx STM32G431V6Tx STM32G431K6Ux STM32G431C6Tx STM32G431R6Ix
 //  0x20000 STM32G431KBTx STM32G431CBTx STM32G431RBIx STM32G431CBYx STM32G431KBUx STM32G431VBTx STM32G431CBUx STM32G431RBTx
 //  0x10000 STM32G431V8Tx STM32G431R8Tx STM32G431K8Tx STM32G431C8Tx STM32G431R8Ix STM32G431C8Ux STM32G431K8Ux STM32G431M8Tx
#warning "check MBED_ROM_SIZE value in cmsis_nvic.h"
#endif

#if !defined(MBED_RAM_START)
#define MBED_RAM_START  0x20000000
#endif

#if !defined(MBED_RAM_SIZE)
#define MBED_RAM_SIZE  0x8000  // 32 KB
#endif

#define NVIC_NUM_VECTORS        118
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_START

#endif
