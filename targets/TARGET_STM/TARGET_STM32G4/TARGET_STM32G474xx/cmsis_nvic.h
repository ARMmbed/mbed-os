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
 //  0x40000 STM32G474VCIx STM32G474CCTx STM32G474QCTx STM32G474VCHx STM32G474CCUx STM32G474MCTx STM32G474VCTx STM32G474RCTx
 //  0x20000 STM32G474VBIx STM32G474MBTx STM32G474CBTx STM32G474RBTx STM32G474QBTx STM32G474VBHx STM32G474VBTx STM32G474CBUx
 //  0x80000 STM32G474CEUx STM32G474VEHx STM32G474RETx STM32G474QETx STM32G474METx STM32G474CETx STM32G474VEIx STM32G474VETx STM32G474MEYx
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
