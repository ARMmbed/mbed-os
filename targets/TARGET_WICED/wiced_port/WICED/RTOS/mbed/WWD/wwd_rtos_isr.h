/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef W_WWD_RTOS_ISR_H
#define W_WWD_RTOS_ISR_H

#include "w_platform_isr.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
 *                      Macros
 ******************************************************/

/* Use this macro to define an RTOS-aware interrupt handler where RTOS
 * primitives can be safely accessed.
 * Current port has no vectored interrupt controller, so single entry point
 * to all interrupts is used. And this entry is wrapped with RTOS-aware code,
 * so all interrupts are actually RTOS-aware.
 *
 * @usage:
 * WWD_RTOS_DEFINE_ISR( my_irq_handler )
 * {
 *     // Do something here
 * }
 */
#define WWD_RTOS_DEFINE_ISR( function ) PLATFORM_DEFINE_ISR( function )

/* Macro for mapping a function defined using WWD_RTOS_DEFINE_ISR
 * to an interrupt handler declared in
 * <Wiced-SDK>/WICED/platform/MCU/<Family>/platform_isr_interface.h
 *
 * @usage:
 * WWD_RTOS_MAP_ISR( my_IRQHandler, USART1_IRQHandler )
 */
#define WWD_RTOS_MAP_ISR( function, isr ) PLATFORM_MAP_ISR( function, isr )

/* Use this macro to define function which serves as ISR demuxer.
 * It is used when no vectored interrupt controller, and single
 * vector triggered for all interrupts.
 *
 * @usage:
 * WWD_RTOS_DEFINE_ISR_DEMUXER( my_irq_handler )
 * {
 *     // Do something here
 * }
 */
#define WWD_RTOS_DEFINE_ISR_DEMUXER( function )  PLATFORM_DEFINE_ISR( function )

/* Macro to declare that function is ISR demuxer.
 * Function has to be defined via WWD_RTOS_DEFINE_ISR_DEMUXER
 *
 * @usage:
 * WWD_RTOS_MAP_ISR_DEMUXER( my_irq_demuxer )
 */
#define WWD_RTOS_MAP_ISR_DEMUXER( function ) PLATFORM_MAP_ISR( function, )

/******************************************************
 *                    Constants
 ******************************************************/

/* Define interrupt handlers. These defines are used by the vector table. */
// mbed handles isrs seperately, these are just to create linking errors if
// get pulled in somewhere
extern void __null_isr( void );
#define SVC_IRQHandler       __null_isr
#define PENDSV_IRQHandler    __null_isr
#define SYSTICK_IRQHandler   __null_isr

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
