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
 //  0x40000 STM32G473CCUx STM32G473MCTx STM32G473VCHx STM32G473CCTx STM32G473VCTx STM32G473QCTx STM32G473RCTx STM32G473VCIx
 //  0x20000 STM32G473RBTx STM32G473QBTx STM32G473MBTx STM32G473VBIx STM32G473CBUx STM32G473VBTx STM32G473VBHx STM32G473CBTx
 //  0x80000 STM32G473CETx STM32G473RETx STM32G473MEUx STM32G473VEIx STM32G473CEUx STM32G473METx STM32G473VEHx STM32G473VETx STM32G473QETx
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
