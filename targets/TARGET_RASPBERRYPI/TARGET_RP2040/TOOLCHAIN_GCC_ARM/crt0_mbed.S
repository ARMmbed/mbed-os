/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico.h"
#include "pico/asm_helper.S"

#include "hardware/regs/m0plus.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"
#include "pico/binary_info/defs.h"

#ifdef NDEBUG
#ifndef COLLAPSE_IRQS
#define COLLAPSE_IRQS
#endif
#endif

pico_default_asm_setup

.section .vectors, "ax"
.align 2

.global __vectors, __VECTOR_TABLE
__VECTOR_TABLE:
__vectors:
.word __StackTop
.word _reset_handler
.word NMI_Handler
.word HardFault_Handler
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word SVC_Handler
.word isr_invalid // Reserved, should never fire
.word isr_invalid // Reserved, should never fire
.word PendSV_Handler
.word SysTick_Handler
.word TIMER_IRQ_0_Handler
.word TIMER_IRQ_1_Handler
.word TIMER_IRQ_2_Handler
.word TIMER_IRQ_3_Handler
.word PWM_IRQ_WRAP_Handler
.word USBCTRL_IRQ_Handler
.word XIP_IRQ_Handler
.word PIO0_IRQ_0_Handler
.word PIO0_IRQ_1_Handler
.word PIO1_IRQ_0_Handler
.word TIMER_IRQ_1_Handler0
.word TIMER_IRQ_1_Handler1
.word TIMER_IRQ_1_Handler2
.word TIMER_IRQ_1_Handler3
.word TIMER_IRQ_1_Handler4
.word TIMER_IRQ_1_Handler5
.word TIMER_IRQ_1_Handler6
.word TIMER_IRQ_1_Handler7
.word TIMER_IRQ_1_Handler8
.word TIMER_IRQ_1_Handler9
.word TIMER_IRQ_2_Handler0
.word TIMER_IRQ_2_Handler1
.word TIMER_IRQ_2_Handler2
.word TIMER_IRQ_2_Handler3
.word TIMER_IRQ_2_Handler4
.word TIMER_IRQ_2_Handler5
.word TIMER_IRQ_2_Handler6
.word TIMER_IRQ_2_Handler7
.word TIMER_IRQ_2_Handler8
.word TIMER_IRQ_2_Handler9
.word TIMER_IRQ_3_Handler0
.word TIMER_IRQ_3_Handler1

// all default exception handlers do nothing, and we can check for them being set to our
// default values by seeing if they point to somewhere between __defaults_isrs_start and __default_isrs_end
.global __default_isrs_start
__default_isrs_start:

// Declare a weak symbol for each ISR.
// By default, they will fall through to the undefined IRQ handler below (breakpoint),
// but can be overridden by C functions with correct name.

.macro decl_isr_bkpt name
.weak \name
.type \name,%function
.thumb_func
\name:
    bkpt #0
.endm

// these are separated out for clarity
decl_isr_bkpt isr_invalid
decl_isr_bkpt NMI_Handler
decl_isr_bkpt HardFault_Handler
decl_isr_bkpt SVC_Handler
decl_isr_bkpt PendSV_Handler
decl_isr_bkpt SysTick_Handler

.global __default_isrs_end
__default_isrs_end:

.macro decl_isr name
.weak \name
.type \name,%function
.thumb_func
\name:
.endm

decl_isr TIMER_IRQ_0_Handler
decl_isr TIMER_IRQ_1_Handler
decl_isr TIMER_IRQ_2_Handler
decl_isr TIMER_IRQ_3_Handler
decl_isr PWM_IRQ_WRAP_Handler
decl_isr USBCTRL_IRQ_Handler
decl_isr XIP_IRQ_Handler
decl_isr PIO0_IRQ_0_Handler
decl_isr PIO0_IRQ_1_Handler
decl_isr PIO1_IRQ_0_Handler
decl_isr TIMER_IRQ_1_Handler0
decl_isr TIMER_IRQ_1_Handler1
decl_isr TIMER_IRQ_1_Handler2
decl_isr TIMER_IRQ_1_Handler3
decl_isr TIMER_IRQ_1_Handler4
decl_isr TIMER_IRQ_1_Handler5
decl_isr TIMER_IRQ_1_Handler6
decl_isr TIMER_IRQ_1_Handler7
decl_isr TIMER_IRQ_1_Handler8
decl_isr TIMER_IRQ_1_Handler9
decl_isr TIMER_IRQ_2_Handler0
decl_isr TIMER_IRQ_2_Handler1
decl_isr TIMER_IRQ_2_Handler2
decl_isr TIMER_IRQ_2_Handler3
decl_isr TIMER_IRQ_2_Handler4
decl_isr TIMER_IRQ_2_Handler5
decl_isr TIMER_IRQ_2_Handler6
decl_isr TIMER_IRQ_2_Handler7
decl_isr TIMER_IRQ_2_Handler8
decl_isr TIMER_IRQ_2_Handler9
decl_isr TIMER_IRQ_3_Handler0
decl_isr TIMER_IRQ_3_Handler1

// All unhandled USER IRQs fall through to here
.global __unhandled_user_irq
.thumb_func
__unhandled_user_irq:
    mrs  r0, ipsr
    subs r0, #16
.global unhandled_user_irq_num_in_r0
unhandled_user_irq_num_in_r0:
    bkpt #0

// ----------------------------------------------------------------------------

.section .binary_info_header, "a"

// Header must be in first 256 bytes of main image (i.e. excluding flash boot2).
// For flash builds we put it immediately after vector table; for NO_FLASH the
// vectors are at a +0x100 offset because the bootrom enters RAM images directly
// at their lowest address, so we put the header in the VTOR alignment hole.

#if !PICO_NO_BINARY_INFO
binary_info_header:
.word BINARY_INFO_MARKER_START
.word __binary_info_start
.word __binary_info_end
.word data_cpy_table // we may need to decode pointers that are in RAM at runtime.
.word BINARY_INFO_MARKER_END
#endif

// ----------------------------------------------------------------------------

.section .reset, "ax"

// On flash builds, the vector table comes first in the image (conventional).
// On NO_FLASH builds, the reset handler section comes first, as the entry
// point is at offset 0 (fixed due to bootrom), and VTOR is highly-aligned.
// Image is entered in various ways:
//
// - NO_FLASH builds are entered from beginning by UF2 bootloader
//
// - Flash builds vector through the table into _reset_handler from boot2
//
// - Either type can be entered via _entry_point by the debugger, and flash builds
//   must then be sent back round the boot sequence to properly initialise flash

// ELF entry point:
.type _entry_point,%function
.thumb_func
.global _entry_point
_entry_point:

#if PICO_NO_FLASH
    // Vector through our own table (SP, VTOR will not have been set up at
    // this point). Same path for debugger entry and bootloader entry.
    ldr r0, =__vectors
#else
    // Debugger tried to run code after loading, so SSI is in 03h-only mode.
    // Go back through bootrom + boot2 to properly initialise flash.
    movs r0, #0
#endif
    ldr r1, =(PPB_BASE + M0PLUS_VTOR_OFFSET)
    str r0, [r1]
    ldmia r0!, {r1, r2}
    msr msp, r1
    bx r2

// Reset handler:
// - initialises .data
// - clears .bss
// - calls runtime_init
// - calls main
// - calls exit (which should eventually hang the processor via _exit)

.type _reset_handler,%function
.thumb_func
_reset_handler:
    // Only core 0 should run the C runtime startup code; core 1 is normally
    // sleeping in the bootrom at this point but check to be sure
    ldr r0, =(SIO_BASE + SIO_CPUID_OFFSET)
    ldr r0, [r0]
    cmp r0, #0
    bne hold_non_core0_in_bootrom

    // In a NO_FLASH binary, don't perform .data copy, since it's loaded
    // in-place by the SRAM load. Still need to clear .bss
#if !PICO_NO_FLASH
    adr r4, data_cpy_table

    // assume there is at least one entry
1:
    ldmia r4!, {r1-r3}
    cmp r1, #0
    beq 2f
    bl data_cpy
    b 1b
2:
#endif

    // Zero out the BSS
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__
    movs r0, #0
    b bss_fill_test
bss_fill_loop:
    stm r1!, {r0}
bss_fill_test:
    cmp r1, r2
    bne bss_fill_loop

platform_entry: // symbol for stack traces
    // Use 32-bit jumps, in case these symbols are moved out of branch range
    // (e.g. if main is in SRAM and crt0 in flash)
    // Mbed OS: Changed this to call _start() instead of runtime_init() and main()
    ldr r1, =_start
    blx r1
    ldr r1, =exit
    blx r1
    // exit should not return.  If it does, hang the core.
    // (fall thru into our hang _exit impl
1: // separate label because _exit can be moved out of branch range
    bkpt #0
    b 1b

#if !PICO_NO_FLASH
data_cpy_loop:
    ldm r1!, {r0}
    stm r2!, {r0}
data_cpy:
    cmp r2, r3
    blo data_cpy_loop
    bx lr
#endif

// Note the data copy table is still included for NO_FLASH builds, even though
// we skip the copy, because it is listed in binary info

.align 2
data_cpy_table:
#if PICO_COPY_TO_RAM
.word __ram_text_source__
.word __ram_text_start__
.word __ram_text_end__
#endif
.word __etext
.word __data_start__
.word __data_end__

.word __scratch_x_source__
.word __scratch_x_start__
.word __scratch_x_end__

.word __scratch_y_source__
.word __scratch_y_start__
.word __scratch_y_end__

.word 0 // null terminator

// ----------------------------------------------------------------------------
// Provide safe defaults for _exit and runtime_init
// Full implementations usually provided by platform.c

.weak runtime_init
.type runtime_init,%function
.thumb_func
runtime_init:
    bx lr

// ----------------------------------------------------------------------------
// If core 1 somehow gets into crt0 due to a spectacular VTOR mishap, we need to
// catch it and send back to the sleep-and-launch code in the bootrom. Shouldn't
// happen (it should sleep in the ROM until given an entry point via the
// cross-core FIFOs) but it's good to be defensive.

hold_non_core0_in_bootrom:
    ldr r0, = 'W' | ('V' << 8)
    bl rom_func_lookup
    bx r0
