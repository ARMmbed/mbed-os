/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

#if TFM_LVL > 0

/* Shared data area between bootloader and runtime firmware
 *
 * Shared data area is allocated at the beginning of the RAM. It is overlapping
 * with TF-M Secure code's MSP stack on purpose:
 * 1. MSP stack grows to the beginning of RAM
 * 2. Shared data reads out by partitions from the beginning of RAM
 *
 * In Nuvoton's TFM port, TFM's bootloader is not used. These macros defined here are
 * just to pass TFM compile.
 */
#if (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
extern uint32_t Image$$ARM_LIB_STACK_MSP$$ZI$$Base;
extern uint32_t Image$$ARM_LIB_STACK_MSP$$ZI$$Length;
#define MSP_STACK_LIMIT     ((uint32_t) &Image$$ARM_LIB_STACK_MSP$$ZI$$Base)
#define MSP_STACK_SIZE      ((uint32_t) &Image$$ARM_LIB_STACK_MSP$$ZI$$Length)

#elif defined(__ICCARM__)
extern uint32_t CSTACK$$Base;
extern uint32_t CSTACK$$Length;
#define MSP_STACK_LIMIT     ((uint32_t) &CSTACK$$Base)
#define MSP_STACK_SIZE      ((uint32_t) &CSTACK$$Length)

#elif defined(__GNUC__)
extern uint32_t __StackLimit;
extern uint32_t __StackTop;
#define MSP_STACK_LIMIT     ((uint32_t) &__StackLimit)
#define MSP_STACK_SIZE      (((uint32_t) &__StackTop) - ((uint32_t) &__StackLimit))

#endif

#define BOOT_TFM_SHARED_DATA_BASE   MSP_STACK_LIMIT
#define BOOT_TFM_SHARED_DATA_SIZE   0x400

#endif /* #if TFM_LVL > 0 */
#endif /* __FLASH_LAYOUT_H__ */
