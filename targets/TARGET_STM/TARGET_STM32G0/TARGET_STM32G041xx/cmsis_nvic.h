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
#define MBED_ROM_SIZE  0x10000
 //  0x10000 STM32G041Y8Yx STM32G041K8Ux STM32G041C8Ux STM32G041C8Tx STM32G041K8Tx STM32G041G8Ux STM32G041F8Px 
 //   0x8000 STM32G041G6Ux STM32G041K6Tx STM32G041C6Tx STM32G041J6Mx STM32G041K6Ux STM32G041C6Ux STM32G041F6Px 
#endif

#if !defined(MBED_RAM_START)
#define MBED_RAM_START  0x20000000
#endif

#if !defined(MBED_RAM_SIZE)
#define MBED_RAM_SIZE  0x2000
#endif

#define NVIC_NUM_VECTORS        48
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_START

#endif
