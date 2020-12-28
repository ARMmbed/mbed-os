//*****************************************************************************
//
//! @file startup_gcc.c
//!
//! @brief Definitions for interrupt handlers, the vector table, and the stack.
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision v2.2.0-7-g63f7c2ba1 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#include "apollo3.h"
#include <stdint.h>

//*****************************************************************************
//
// Forward declaration of interrupt handlers.
//
//*****************************************************************************
extern void Reset_Handler(void) __attribute((naked));
extern void NMI_Handler(void) __attribute((weak));
extern void HardFault_Handler(void) __attribute((weak));
extern void MemManage_Handler(void) __attribute((weak, alias("HardFault_Handler")));
extern void BusFault_Handler(void) __attribute((weak, alias("HardFault_Handler")));
extern void UsageFault_Handler(void) __attribute((weak, alias("HardFault_Handler")));
extern void SecureFault_Handler(void) __attribute((weak));
extern void SVC_Handler(void) __attribute((weak, alias("am_default_isr")));
extern void DebugMon_Handler(void) __attribute((weak, alias("am_default_isr")));
extern void PendSV_Handler(void) __attribute((weak, alias("am_default_isr")));
extern void SysTick_Handler(void) __attribute((weak, alias("am_default_isr")));

extern void am_brownout_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_watchdog_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_rtc_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_vcomp_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_ioslave_ios_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_ioslave_acc_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_iomaster0_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_iomaster1_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_iomaster2_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_iomaster3_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_iomaster4_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_iomaster5_isr(void) __attribute((weak, alias("am_default_isr")));
extern void HciDrvIntService(void) __attribute((weak, alias("am_default_isr")));
extern void am_gpio_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_ctimer_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_uart_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_uart1_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_scard_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_adc_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_pdm0_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_mspi0_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_software0_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr0_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr1_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr2_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr3_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr4_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr5_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr6_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_stimer_cmpr7_isr(void) __attribute((weak, alias("am_default_isr")));
extern void am_clkgen_isr(void) __attribute((weak, alias("am_default_isr")));

extern void am_default_isr(void) __attribute((weak));

// // Entry Point for mbed boot sequence
// extern void __main(void);
// extern void _start(void);
extern void mbed_init(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

// '__stack' accesses the linker-provided address for the start of the stack
// (which is a high address - stack goes top to bottom)
extern void *__stack;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
// Note: Aliasing and weakly exporting am_mpufault_isr, am_busfault_isr, and
// am_usagefault_isr does not work if am_fault_isr is defined externally.
// Therefore, we'll explicitly use am_fault_isr in the table for those vectors.
//
//*****************************************************************************
__attribute__((section(".isr_vector"))) void (*const g_am_pfnVectors[])(void) =
    {
        (void (*)(void))(&__stack), // The initial stack pointer (provided by linker script)
        Reset_Handler,              // The reset handler
        NMI_Handler,                // The NMI handler
        HardFault_Handler,          // The hard fault handler
        MemManage_Handler,          // The MemManage_Handler
        BusFault_Handler,           // The BusFault_Handler
        UsageFault_Handler,         // The UsageFault_Handler
        SecureFault_Handler,        // The SecureFault_Handler
        0,                          // Reserved
        0,                          // Reserved
        0,                          // Reserved
        SVC_Handler,                // SVCall handler
        DebugMon_Handler,           // Debug monitor handler
        0,                          // Reserved
        PendSV_Handler,             // The PendSV handler
        SysTick_Handler,            // The SysTick handler

        //
        // Peripheral Interrupts
        //
        am_brownout_isr,     //  0: Brownout (rstgen)
        am_watchdog_isr,     //  1: Watchdog
        am_rtc_isr,          //  2: RTC
        am_vcomp_isr,        //  3: Voltage Comparator
        am_ioslave_ios_isr,  //  4: I/O Slave general
        am_ioslave_acc_isr,  //  5: I/O Slave access
        am_iomaster0_isr,    //  6: I/O Master 0
        am_iomaster1_isr,    //  7: I/O Master 1
        am_iomaster2_isr,    //  8: I/O Master 2
        am_iomaster3_isr,    //  9: I/O Master 3
        am_iomaster4_isr,    // 10: I/O Master 4
        am_iomaster5_isr,    // 11: I/O Master 5
        HciDrvIntService,    // 12: BLEIF
        am_gpio_isr,         // 13: GPIO
        am_ctimer_isr,       // 14: CTIMER
        am_uart_isr,         // 15: UART0
        am_uart1_isr,        // 16: UART1
        am_scard_isr,        // 17: SCARD
        am_adc_isr,          // 18: ADC
        am_pdm0_isr,         // 19: PDM
        am_mspi0_isr,        // 20: MSPI0
        am_software0_isr,    // 21: SOFTWARE0
        am_stimer_isr,       // 22: SYSTEM TIMER
        am_stimer_cmpr0_isr, // 23: SYSTEM TIMER COMPARE0
        am_stimer_cmpr1_isr, // 24: SYSTEM TIMER COMPARE1
        am_stimer_cmpr2_isr, // 25: SYSTEM TIMER COMPARE2
        am_stimer_cmpr3_isr, // 26: SYSTEM TIMER COMPARE3
        am_stimer_cmpr4_isr, // 27: SYSTEM TIMER COMPARE4
        am_stimer_cmpr5_isr, // 28: SYSTEM TIMER COMPARE5
        am_stimer_cmpr6_isr, // 29: SYSTEM TIMER COMPARE6
        am_stimer_cmpr7_isr, // 30: SYSTEM TIMER COMPARE7
        am_clkgen_isr,       // 31: CLKGEN
};

//******************************************************************************
//
// Place code immediately following vector table.
//
//******************************************************************************
//******************************************************************************
//
// The Patch table.
//
// The patch table should pad the vector table size to a total of 64 entries
// (16 core + 48 periph) such that code begins at offset 0x100.
//
//******************************************************************************
__attribute__((section(".ble_patch")))
uint32_t const __Patchable[] =
    {
        0, // 32
        0, // 33
        0, // 34
        0, // 35
        0, // 36
        0, // 37
        0, // 38
        0, // 39
        0, // 40
        0, // 41
        0, // 42
        0, // 43
        0, // 44
        0, // 45
        0, // 46
        0, // 47
};

// //*****************************************************************************
// //
// // The following are constructs created by the linker, indicating where the
// // the "data" and "bss" segments reside in memory.  The initializers for the
// // "data" segment resides immediately following the "text" segment.
// //
// //*****************************************************************************
// extern uint32_t _etext;
// extern uint32_t __data_start__;
// extern uint32_t __data_end__;
// extern uint32_t _sbss;
// extern uint32_t _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.
//
//*****************************************************************************
#if defined(__GNUC_STDC_INLINE__)
void Reset_Handler(void)
{
    //
    // Set the vector table pointer.
    //
    __asm("    ldr    r0, =0xE000ED08\n"
          "    ldr    r1, =g_am_pfnVectors\n"
          "    str    r1, [r0]");

    //     //
    //     // Set the stack pointer.
    //     //
    __asm("    ldr    sp, [r1]");

#ifndef NOFPU
    //     //
    //     // Enable the FPU.
    //     //
    __asm("ldr  r0, =0xE000ED88\n"
          "ldr  r1,[r0]\n"
          "orr  r1,#(0xF << 20)\n"
          "str  r1,[r0]\n"
          "dsb\n"
          "isb\n");
#endif

    // // Start mbed boot sequence https://os.mbed.com/docs/mbed-os/v5.15/reference/bootstrap.html
    // SystemInit();
    // _start();

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    __asm("    ldr     r0, =__etext\n"
          "    ldr     r1, =__data_start__\n"
          "    ldr     r2, =__data_end__\n"
          "copy_loop:\n"
          "        cmp   r1, r2\n"
          "        beq   copy_end\n"
          "        ldr   r3, [r0], #4\n"
          "        str   r3, [r1], #4\n"
          "        b     copy_loop\n"
          "copy_end:\n");

    // //
    // // Zero fill the bss segment.
    // //
    __asm("LDR     R0, =_start\n"
          "BX      R0\n");
    //
    // If main returns then execute a break point instruction
    //
    __asm("    bkpt     ");
}
#else
#error GNU STDC inline not supported.
#endif

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
void NMI_Handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
void HardFault_Handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
void am_default_isr(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
