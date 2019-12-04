/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "system_msp432p401r.h"
#include "msp.h"
#include <stdint.h>

/*******************************************/
/* Macro Definitions depending on compiler */
/*******************************************/
#if defined(__CC_ARM)

#define WEAK_FUNC(FUN) \
void FUN(void) __attribute__ ((weak));
#define WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) \
void FUN(void) __attribute__ ((weak, alias(#FUN_ALIAS)));

#elif defined(__ICCARM__)

#pragma diag_suppress=Pe1665
#define WEAK_FUNC(FUN) void FUN(void); \
_Pragma(_STRINGIFY(_WEAK_FUNC(FUN)))
#define _WEAK_FUNC(FUN) weak FUN
#define WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) void FUN(void); \
_Pragma(_STRINGIFY(_WEAK_ALIAS_FUNC(FUN, FUN_ALIAS)))
#define _WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) weak __WEAK_ALIAS_FUNC(FUN, FUN_ALIAS)
#define __WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) FUN##=##FUN_ALIAS

#elif defined(__GNUC__)

#define WEAK_FUNC(FUN) \
void FUN(void) __attribute__ ((weak));
#define WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) \
void FUN(void) __attribute__ ((weak, alias(#FUN_ALIAS)));

#endif

/**********************************************************/
/* External variables and functions depending on compiler */
/**********************************************************/
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
// Entry point for the application.
extern void __main(void);
/* External symbols */
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#elif defined(__ICCARM__)
// Entry point for the application.
extern void __iar_program_start(void);
/* External symbols */
extern uint32_t CSTACK$$Limit;
#elif defined(__GNUC__) 
/* Entry point for the application. */
extern int _start();
/* External symbols */
extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __StackTop;
#endif

typedef void(*pFunc)(void);

// Forward declaration of the EUSCI dispatch handlers.
static void EUSCIA0_IRQHandler(void);
static void EUSCIA1_IRQHandler(void);
static void EUSCIA2_IRQHandler(void);
static void EUSCIA3_IRQHandler(void);
static void EUSCIB0_IRQHandler(void);
static void EUSCIB1_IRQHandler(void);
static void EUSCIB2_IRQHandler(void);
static void EUSCIB3_IRQHandler(void);

// Forward declaration of the default fault/reset handlers.
WEAK_FUNC(Default_Handler)
WEAK_FUNC(Reset_Handler)

// Cortex-M4 Processor Exceptions
WEAK_ALIAS_FUNC(NMI_Handler, Default_Handler)
WEAK_ALIAS_FUNC(HardFault_Handler, Default_Handler)
WEAK_ALIAS_FUNC(MemManage_Handler, Default_Handler)
WEAK_ALIAS_FUNC(BusFault_Handler, Default_Handler)
WEAK_ALIAS_FUNC(UsageFault_Handler, Default_Handler)
WEAK_ALIAS_FUNC(SVC_Handler, Default_Handler)
WEAK_ALIAS_FUNC(DebugMon_Handler, Default_Handler)
WEAK_ALIAS_FUNC(PendSV_Handler, Default_Handler)
WEAK_ALIAS_FUNC(SysTick_Handler, Default_Handler)

// Device specific interrupt handler
WEAK_ALIAS_FUNC(PSS_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(CS_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(PCM_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(WDT_A_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(FPU_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(FLCTL_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(COMP_E0_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(COMP_E1_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA0_0_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA0_N_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA1_0_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA1_N_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA2_0_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA2_N_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA3_0_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(TA3_N_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(ADC14_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(T32_INT1_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(T32_INT2_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(T32_INTC_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(AES256_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(RTC_C_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(DMA_ERR_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(DMA_INT3_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(DMA_INT2_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(DMA_INT1_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(DMA_INT0_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(PORT1_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(PORT2_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(PORT3_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(PORT4_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(PORT5_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(PORT6_IRQHandler, Default_Handler)

// EUSCI specific interrupt handlers
WEAK_ALIAS_FUNC(EUSCIA0_UART_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIA1_UART_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIA2_UART_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIA3_UART_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB0_I2C_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB1_I2C_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB2_I2C_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB3_I2C_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIA0_SPI_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIA1_SPI_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIA2_SPI_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIA3_SPI_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB0_SPI_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB1_SPI_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB2_SPI_IRQHandler, Default_Handler)
WEAK_ALIAS_FUNC(EUSCIB3_SPI_IRQHandler, Default_Handler)

// Interrupt vector table.  Note that the proper constructs must be placed on this
// to ensure that it ends up at physical address 0x0000.0000 or at the start of the
// program if located at a start address other than 0.
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
void (* const interruptVectors[])(void) __attribute__((section("RESET"), used)) = {
    (pFunc) &Image$$ARM_LIB_STACK$$ZI$$Limit,  // The initial stack pointer
#elif defined(__ICCARM__)
void (* const __vector_table[])(void) @ ".intvec" = {
    (pFunc) &CSTACK$$Limit,                // The initial stack pointer
#elif defined(__GNUC__) 
void (* const interruptVectors[])(void) __attribute__((section(".isr_vector"))) = {
    (pFunc) &__StackTop,                   // The initial stack pointer
#endif
    Reset_Handler,                         // The reset handler
    NMI_Handler,                           // The NMI handler
    HardFault_Handler,                     // The hard fault handler
    MemManage_Handler,                     // The MPU fault handler
    BusFault_Handler,                      // The bus fault handler
    UsageFault_Handler,                    // The usage fault handler
    0,                                     // Reserved
    0,                                     // Reserved
    0,                                     // Reserved
    0,                                     // Reserved
    SVC_Handler,                           // SVCall handler
    DebugMon_Handler,                      // Debug monitor handler
    0,                                     // Reserved
    PendSV_Handler,                        // The PendSV handler
    SysTick_Handler,                       // The SysTick handler

    PSS_IRQHandler,                        // PSS Interrupt
    CS_IRQHandler,                         // CS Interrupt
    PCM_IRQHandler,                        // PCM Interrupt
    WDT_A_IRQHandler,                      // WDT_A Interrupt
    FPU_IRQHandler,                        // FPU Interrupt
    FLCTL_IRQHandler,                      // Flash Controller Interrupt
    COMP_E0_IRQHandler,                    // COMP_E0 Interrupt
    COMP_E1_IRQHandler,                    // COMP_E1 Interrupt
    TA0_0_IRQHandler,                      // TA0_0 Interrupt
    TA0_N_IRQHandler,                      // TA0_N Interrupt
    TA1_0_IRQHandler,                      // TA1_0 Interrupt
    TA1_N_IRQHandler,                      // TA1_N Interrupt
    TA2_0_IRQHandler,                      // TA2_0 Interrupt
    TA2_N_IRQHandler,                      // TA2_N Interrupt
    TA3_0_IRQHandler,                      // TA3_0 Interrupt
    TA3_N_IRQHandler,                      // TA3_N Interrupt
    EUSCIA0_IRQHandler,                    // EUSCIA0 Interrupt
    EUSCIA1_IRQHandler,                    // EUSCIA1 Interrupt
    EUSCIA2_IRQHandler,                    // EUSCIA2 Interrupt
    EUSCIA3_IRQHandler,                    // EUSCIA3 Interrupt
    EUSCIB0_IRQHandler,                    // EUSCIB0 Interrupt
    EUSCIB1_IRQHandler,                    // EUSCIB1 Interrupt
    EUSCIB2_IRQHandler,                    // EUSCIB2 Interrupt
    EUSCIB3_IRQHandler,                    // EUSCIB3 Interrupt
    ADC14_IRQHandler,                      // ADC14 Interrupt
    T32_INT1_IRQHandler,                   // T32_INT1 Interrupt
    T32_INT2_IRQHandler,                   // T32_INT2 Interrupt
    T32_INTC_IRQHandler,                   // T32_INTC Interrupt
    AES256_IRQHandler,                     // AES256 Interrupt
    RTC_C_IRQHandler,                      // RTC_C Interrupt
    DMA_ERR_IRQHandler,                    // DMA_ERR Interrupt
    DMA_INT3_IRQHandler,                   // DMA_INT3 Interrupt
    DMA_INT2_IRQHandler,                   // DMA_INT2 Interrupt
    DMA_INT1_IRQHandler,                   // DMA_INT1 Interrupt
    DMA_INT0_IRQHandler,                   // DMA_INT0 Interrupt
    PORT1_IRQHandler,                      // Port1 Interrupt
    PORT2_IRQHandler,                      // Port2 Interrupt
    PORT3_IRQHandler,                      // Port3 Interrupt
    PORT4_IRQHandler,                      // Port4 Interrupt
    PORT5_IRQHandler,                      // Port5 Interrupt
    PORT6_IRQHandler                       // Port6 Interrupt
};

// The reset irq handler
void Reset_Handler(void)
{
    // Halt the WDT already here, because the following copy process
    // from flash to SRAM (in case of GCC) might take so long that the
    // WDT kicks in
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    // Call system initialization routine
    SystemInit();
    // Jump to the main initialization routine.
    __main();
#elif defined(__ICCARM__)
    // Call system initialization routine
    SystemInit();
    // Jump to the main initialization routine.
    __iar_program_start();
#elif defined(__GNUC__) 
    uint32_t *pui32Src, *pui32Dest;
    // Copy the data segment initializers from flash to SRAM.
    pui32Src = &__data_load__;
    for (pui32Dest = &__data_start__; pui32Dest < &__data_end__;) {
        *pui32Dest++ = *pui32Src++;
    }
    // Call system initialization routine
    SystemInit();
    // Jump to the main initialization routine.
    _start();
#endif
}

// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
void Default_Handler(void)
{
    // Enter an infinite loop.
    while (1) { }
}

// EUSCI interrupt dispatch handlers. These handlers will look at the current
// EUSCI mode (SPI, UART, I2C), and forward the call to the specific handlers.
void EUSCIA0_IRQHandler(void)
{
    if (EUSCI_A0->CTLW0 & EUSCI_A_CTLW0_SYNC) {
        EUSCIA0_SPI_IRQHandler();
    } else {
        EUSCIA0_UART_IRQHandler();
    }
}

void EUSCIA1_IRQHandler(void)
{
    if (EUSCI_A1->CTLW0 & EUSCI_A_CTLW0_SYNC) {
        EUSCIA1_SPI_IRQHandler();
    } else {
        EUSCIA1_UART_IRQHandler();
    }
}

void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->CTLW0 & EUSCI_A_CTLW0_SYNC) {
        EUSCIA2_SPI_IRQHandler();
    } else {
        EUSCIA2_UART_IRQHandler();
    }
}

void EUSCIA3_IRQHandler(void)
{
    if (EUSCI_A3->CTLW0 & EUSCI_A_CTLW0_SYNC) {
        EUSCIA3_SPI_IRQHandler();
    } else {
        EUSCIA3_UART_IRQHandler();
    }
}

void EUSCIB0_IRQHandler(void)
{
    if ((EUSCI_B0->CTLW0 & EUSCI_A_CTLW0_MODE_MASK) == EUSCI_A_CTLW0_MODE_3) {
        EUSCIB0_I2C_IRQHandler();
    } else {
        EUSCIB0_SPI_IRQHandler();
    }
}

void EUSCIB1_IRQHandler(void)
{
    if ((EUSCI_B1->CTLW0 & EUSCI_A_CTLW0_MODE_MASK) == EUSCI_A_CTLW0_MODE_3) {
        EUSCIB1_I2C_IRQHandler();
    } else {
        EUSCIB1_SPI_IRQHandler();
    }
}

void EUSCIB2_IRQHandler(void)
{
    if ((EUSCI_B2->CTLW0 & EUSCI_A_CTLW0_MODE_MASK) == EUSCI_A_CTLW0_MODE_3) {
        EUSCIB2_I2C_IRQHandler();
    } else {
        EUSCIB2_SPI_IRQHandler();
    }
}

void EUSCIB3_IRQHandler(void)
{
    if ((EUSCI_B3->CTLW0 & EUSCI_A_CTLW0_MODE_MASK) == EUSCI_A_CTLW0_MODE_3) {
        EUSCIB3_I2C_IRQHandler();
    } else {
        EUSCIB3_SPI_IRQHandler();
    }
}
