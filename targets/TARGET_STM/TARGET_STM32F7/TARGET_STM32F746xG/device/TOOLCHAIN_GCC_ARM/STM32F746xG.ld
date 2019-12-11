/* Linker script to configure memory regions. */
/*
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016-2019 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
*/
#include "../cmsis_nvic.h"

/* STM32F746xG:
FLASH:   1024K (0x100000) @0x08000000
SRAM1:    240K (0x3C000)  @0x20010000
SRAM2:     16K (0x4000)   @0x2004C000
DTCM-RAM:  64K (0x10000)  @0x20000000
ITCM-RAM:  16K (0x3FFF)   @0x00000000
*/

#define MBED_ROM_START   0x8000000
#define MBED_ROM_SIZE    0x100000
#define MBED_RAM_START   0x20010000
#define MBED_RAM_SIZE    0x40000
#define MBED_RAM1_START  0x20000000
#define MBED_RAM1_SIZE   0x10000

#if !defined(MBED_APP_START)
  #define MBED_APP_START MBED_ROM_START
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE MBED_ROM_SIZE
#endif

M_CRASH_DATA_RAM_SIZE = 0x100;

#if !defined(MBED_BOOT_STACK_SIZE)
    #define MBED_BOOT_STACK_SIZE 0x400
#endif
STACK_SIZE = MBED_BOOT_STACK_SIZE;

#define MBED_VECTTABLE_RAM_SIZE (((NVIC_NUM_VECTORS * 4) + 7) & 0xFFFFFFF8)
#define MBED_RAM0_START  (MBED_RAM1_START + MBED_VECTTABLE_RAM_SIZE)
#define MBED_RAM0_SIZE   (MBED_RAM_SIZE + MBED_RAM1_SIZE - MBED_VECTTABLE_RAM_SIZE)

MEMORY
{ 
  FLASH (rx) : ORIGIN = MBED_APP_START, LENGTH = MBED_APP_SIZE
  RAM (rwx)  : ORIGIN = MBED_RAM0_START, LENGTH = MBED_RAM0_SIZE
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 * 
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   _estack
 */
ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    __etext = .;
    _sidata = .;
    
    .crash_data_ram :
    {
        . = ALIGN(8);
        __CRASH_DATA_RAM__ = .;
        __CRASH_DATA_RAM_START__ = .; /* Create a global symbol at data start */
        KEEP(*(.keep.crash_data_ram))
        *(.m_crash_data_ram)     /* This is a user defined section */
        . += M_CRASH_DATA_RAM_SIZE;
        . = ALIGN(8);
        __CRASH_DATA_RAM_END__ = .; /* Define a global symbol at data end */
    } > RAM

    .data : AT (__etext)
    {
        __data_start__ = .;
        _sdata = .;
        *(vtable)
        *(.data*)

        . = ALIGN(8);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(8);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);


        . = ALIGN(8);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(8);
        /* All data end */
        __data_end__ = .;
        _edata = .;

    } > RAM

    .bss :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
        _ebss = .;
    } > RAM

    .heap (COPY):
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
        . = ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE;
        __HeapLimit = .;
    } > RAM

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (COPY):
    {
        *(.stack*)
    } > RAM

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    _estack = __StackTop;
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
