/*
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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

INCLUDE "rtl8195a_rom.h"
/* DATA_RAM: We cannot put Code(.text) in DATA_RAM, this region is reserved for Image1(boot loader).
   But we can put .data/.bss of Image2 in this region */
MEMORY
{
  TCM (rwx)        : ORIGIN = 0x1FFF0000, LENGTH = 0x00010000
  DATA_RAM (rwx)   : ORIGIN = 0x10002100, LENGTH = 0x10007000 - 0x10002100
  SRAM1 (rwx)      : ORIGIN = 0x10007000, LENGTH = 0x10070000 - 0x10007000
  SRAM2 (rwx)      : ORIGIN = 0x30000000, LENGTH = 2M
}

/* Stack sizes: */
StackSize = 0x1000;

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 * _reset_init : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 * __exidx_start
 * __exidx_end
 * __etext
 * __data_start__
 * __preinit_array_start
 * __preinit_array_end
 * __init_array_start
 * __init_array_end
 * __fini_array_start
 * __fini_array_end
 * __data_end__
 * __bss_start__
 * __bss_end__
 * __end__
 * end
 * __HeapLimit
 * __StackLimit
 * __StackTop
 * __stack
 */
ENTRY(Reset_Handler)

SECTIONS
{
    .image2.table :
    {
        KEEP(*(SORT(.image2.ram.data*)))
        KEEP(*(.image2.validate.rodata*))
    } > SRAM2

    .text.sram1 :
    {
        . = ALIGN(4);
        *rtl8195a_crypto*.o (.text* .rodata*)
        *mbedtls*.o (.text* .rodata*)
        *libc.a: (.text* .rodata*)
        *lib_peripheral_mbed_gcc.a: (.text* .rodata*)
        *_api*.o (.text* .rodata*)
        *main*.o (.text* .rodata*)
    } > SRAM1

    .text.sram2 :
    {
        . = ALIGN(4);
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
    } > SRAM2
    __etext = .;

    __data_start__ = .;

    .data.sram1 :
    {
        . = ALIGN(4);
        __sram_data_start__ = .;
        *rtl8195a_crypto*.o (.data*)
        *mbedtls*.o (.data*)
        __sram_data_end__ = .;
    } > SRAM1

    .data.sram2 :
    {
        __sdram_data_start__ = .;
        *(vtable)
        *(.data*)
        *(.sdram.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);

        . = ALIGN(4);

        __sdram_data_end__ = .;
        /* All data end */
    } > SRAM2
    __data_end__ = .;
    __image2_end__ = .;

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > SRAM2

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > SRAM2
    __exidx_end = .;

    .bss.sram1 (NOLOAD) :
    {
        __bss_sram_start__ = .;
        *rtl8195a_crypto*.o (.bss* COMMON)
        *mbedtls*.o (.bss* COMMON)
        *(.bss.thread_stack_main)
        *lib_peripheral_mbed_gcc.a: (.bss* COMMON)
        *mbed_boot*.o (.bss* COMMON)
        __bss_sram_end__ = .;
    } > SRAM1

    .bss.sram2 (NOLOAD) :
    {
        __bss_start__ = .;
        __bss_dram_start__ = .;
        *(.bss*)
        *(COMMON)
        *(.bdsram.data*)
        __bss_dram_end__ = .;
        __bss_end__ = .;
    } > SRAM2

    .bf_data :
    {
        __buffer_data_start__ = .;
        *(.bfsram.data*)
        __buffer_data_end__ = .;
    } > SRAM2

    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
    . = ORIGIN(SRAM1) + LENGTH(SRAM1) - StackSize;
        __HeapLimit = .;
    } > SRAM1
    
    .TCM_overlay (NOLOAD):
    {
        __bss_dtcm_start__ = .;
        *lwip_mem*.o (.bss* COMMON)
        *lwip_memp*.o (.bss* COMMON)
        *(.tcm.heap*)
        __bss_dtcm_end__ = .;
    } > TCM
    
    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (NOLOAD):
    {
        __StackLimit = .;
        *(.stack)
    . += StackSize - (. - __StackLimit);
    } > SRAM1

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(SRAM1) + LENGTH(SRAM1);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM exceeds ram limit")
}
