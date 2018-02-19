/******************************************************************************
 * @file     system_RZ_A1LU.h
 * @brief    CMSIS Device System Header File for ARM Cortex-A Device Series
 * @version  V1.00
 * @date     10 Mar 2017
 *
 * @note
 *
 ******************************************************************************/
/*
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SYSTEM_RZ_A1LU_H
#define __SYSTEM_RZ_A1LU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

typedef void(*IRQHandler)();         /*!< Type Definition for Interrupt Handlers */

/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

/**
  \brief  Interrupt Handler Register.

   Registers an Interrupt Handler into the IRQ Table.
 */
extern uint32_t InterruptHandlerRegister(IRQn_Type, IRQHandler);

/**
  \brief  Interrupt Handler Unregister.

   Unregisters an Interrupt Handler from the IRQ Table.
 */
extern uint32_t InterruptHandlerUnregister(IRQn_Type);

/**
  \brief  Create Translation Table.

   Creates Memory Management Unit Translation Table.
 */
extern void MMU_CreateTranslationTable(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_RZ_A1LU_H */
