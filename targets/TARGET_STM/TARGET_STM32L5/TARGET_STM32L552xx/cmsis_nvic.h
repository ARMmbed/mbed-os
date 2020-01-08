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
 //  0x40000 STM32L552CCTx STM32L552RCTx STM32L552QCIxQ STM32L552ZCTxQ STM32L552CCUx STM32L552VCTxQ
 //  0x80000 STM32L552CEUx STM32L552MEYxP STM32L552MEYxQ STM32L552RETx STM32L552ZETx STM32L552QEIxQ STM32L552VETxQ STM32L552QEIxP STM32L552CETx STM32L552CETxP STM32L552QEIx STM32L552ZETxQ STM32L552VETx STM32L552RETxP STM32L552RETxQ STM32L552CEUxP
#endif

#if !defined(MBED_RAM_START)
#define MBED_RAM_START  0x20000000
#endif

#if !defined(MBED_RAM_SIZE)
#define MBED_RAM_SIZE  0x40000  // 256 KB
#endif

#define NVIC_NUM_VECTORS        124
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_START

#endif
