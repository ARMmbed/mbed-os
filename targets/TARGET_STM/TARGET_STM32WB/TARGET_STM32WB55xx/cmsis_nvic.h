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
#define MBED_ROM_SIZE  0x100000
//  0x40000 STM32WB55VCYx STM32WB55RCVx STM32WB55CCUx
//  0x80000 STM32WB55REVx STM32WB55CEUx STM32WB55VEYx
// 0x100000 STM32WB55RGVx STM32WB55VGYx STM32WB55CGUx
#warning "check MBED_ROM_SIZE value in cmsis_nvic.h"
#endif

#if !defined(MBED_RAM_START)
#define MBED_RAM_START  0x20000000
#endif

#if !defined(MBED_RAM_SIZE)
#define MBED_RAM_SIZE  0x40000
//  0x40000 STM32WB55RGVx STM32WB55REVx STM32WB55CEUx STM32WB55VEYx STM32WB55VGYx STM32WB55CGUx
//  0x20000 STM32WB55VCYx STM32WB55RCVx STM32WB55CCUx
#warning "check MBED_RAM_SIZE value in cmsis_nvic.h"
#endif

#define NVIC_NUM_VECTORS        79
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_START

#endif
