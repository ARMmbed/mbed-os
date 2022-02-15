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
// 0x0x08000000-0x080FFFFF Bank1 (8 x 128K sectors)
// 0x0x08100000-0x081FFFFF Bank2 (8 x 128K sectors)
#define MBED_ROM_SIZE  0x200000  // 2.0 MB
#endif

#if !defined(MBED_RAM_START)
#define MBED_RAM_START  0x20000000
#endif

#if !defined(MBED_RAM_SIZE)
// 0x38000000 - 0x3800FFFF 64K  SRAM4
// 0x30040000 - 0x30047FFF 32K  SRAM3
// 0x30020000 - 0x3003FFFF 128K SRAM2
// 0x30000000 - 0x3001FFFF 128K SRAM1
// 0x24000000 - 0x2407FFFF 512K AXI SRAM
// 0x20000000 - 0x2001FFFF 128K DTCM
#define MBED_RAM_SIZE  0x20000  // 128 KB
#endif

#if !defined(MBED_RAM1_START)
#define MBED_RAM1_START  0x24000000
#endif

#if !defined(MBED_RAM1_SIZE)
#define MBED_RAM1_SIZE  0x80000  // 512 KB
#endif

#define NVIC_NUM_VECTORS        166
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_START

#endif
