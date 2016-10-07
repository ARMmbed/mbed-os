/*
 * BEETLE CMSIS Library
 */
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * This file is derivative of CMSIS V5.00 gcc_arm.ld
 */
/* Linker script for mbed BEETLE SoC */

/* Linker script to configure memory regions. */
MEMORY
{
  VECTORS (rx)          : ORIGIN = 0x00000000, LENGTH = 0x00000400
  FLASH (rx)            : ORIGIN = 0x00000400, LENGTH = 0x00040000 - 0x00000400
  RAM (rwx)             : ORIGIN = 0x20000000, LENGTH = 0x00020000
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
 */
ENTRY(Reset_Handler)

/* Heap 1/4 of ram and stack 1/8 */
__stack_size__ = 0x4000;
__heap_size__ = 0x8000;

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0400;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;

/* Size of the vector table in SRAM */
M_VECTOR_RAM_SIZE = 0x140;

SECTIONS
{
    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.vector_table))
         . = ALIGN(4);
    } > VECTORS

    /* Note: The uVisor expects this section at a fixed location, as specified
             by the porting process configuration parameter: FLASH_OFFSET. */
    __UVISOR_TEXT_OFFSET = 0x0;
    __UVISOR_TEXT_START = ORIGIN(FLASH) + __UVISOR_TEXT_OFFSET;
    .text __UVISOR_TEXT_START :
    {
        /* uVisor code and data */
        . = ALIGN(4);
        __uvisor_main_start = .;
        *(.uvisor.main)
        __uvisor_main_end = .;

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

    .cordio :
    {
        *CORDIO_RO_2.1.o
        *TRIM_2.1.o
    } > FLASH

    .interrupts_ram :
    {
        . = ALIGN(4);
        __VECTOR_RAM__ = .;
        __interrupts_ram_start__ = .;   /* Create a global symbol at data start */
        . += M_VECTOR_RAM_SIZE;
        . = ALIGN(4);
        __interrupts_ram_end__ = .;     /* Define a global symbol at data end */
    } > RAM

    /* ensure that uvisor bss is at the beginning of memory */
    /* Note: The uVisor expects this section at a fixed location, as specified by
     * the porting process configuration parameter: SRAM_OFFSET. */
    __UVISOR_SRAM_OFFSET = 0x140;
    __UVISOR_BSS_START = ORIGIN(RAM) + __UVISOR_SRAM_OFFSET;
    .uvisor.bss __UVISOR_BSS_START (NOLOAD):
    {
        . = ALIGN(32);
        __uvisor_bss_start = .;

        /* protected uvisor main bss */
        . = ALIGN(32);
        __uvisor_bss_main_start = .;
        KEEP(*(.keep.uvisor.bss.main))
        . = ALIGN(32);
        __uvisor_bss_main_end = .;

        /* protected uvisor secure boxes bss */
        . = ALIGN(32);
        __uvisor_bss_boxes_start = .;
        KEEP(*(.keep.uvisor.bss.boxes))
        . = ALIGN(32);
        __uvisor_bss_boxes_end = .;

        . = ALIGN((1 << LOG2CEIL(LENGTH(RAM))) / 8);
        __uvisor_bss_end = .;
    } > RAM

    /* Heap space for the page allocator */
    .page_heap (NOLOAD) :
    {
        . = ALIGN(32);
        __uvisor_page_start = .;
        KEEP(*(.keep.uvisor.page_heap))
        . = ALIGN(32);
        __uvisor_page_end = .;
    } > RAM

    .data :
    {
        PROVIDE(__etext = LOADADDR(.data));
        . = ALIGN(4);
        __data_start__ = .;
        *(vtable)
        *(.data)
        *(.data*)

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
        /* All data end */
        __data_end__ = .;

    } > RAM AT > FLASH

    /* uvisor configuration data */
    .uvisor.secure :
    {
        . = ALIGN(32);
        __uvisor_secure_start = .;

        /* uvisor secure boxes configuration tables */
        . = ALIGN(32);
        __uvisor_cfgtbl_start = .;
        KEEP(*(.keep.uvisor.cfgtbl))
        . = ALIGN(32);
        __uvisor_cfgtbl_end = .;

        __uvisor_cfgtbl_ptr_start = .;
        KEEP(*(.keep.uvisor.cfgtbl_ptr_first))
        KEEP(*(.keep.uvisor.cfgtbl_ptr))
        __uvisor_cfgtbl_ptr_end = .;

        /* Pointers to all boxes register gateways. These are grouped here to allow
         * discoverability and firmware verification. */
        __uvisor_register_gateway_ptr_start = .;
        KEEP(*(.keep.uvisor.register_gateway_ptr))
        __uvisor_register_gateway_ptr_end = .;

        . = ALIGN(32);
        __uvisor_secure_end = .;
    } > FLASH

    /* From now on you can insert any other SRAM region. */

    .uninitialized (NOLOAD):
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > RAM

    .bss :
    {
        . = ALIGN(4);
        __START_BSS = .;
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        __END_BSS = .;

    } > RAM

    bss_size = __bss_end__ - __bss_start__;

    .heap :
    {
        . = ALIGN(8);
        __uvisor_heap_start = .;
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += HEAP_SIZE;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
        __uvisor_heap_end = .;
    } > RAM

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
    /* Provide physical memory boundaries for uVisor. */
    __uvisor_flash_start = ORIGIN(VECTORS);
    __uvisor_flash_end = ORIGIN(FLASH) + LENGTH(FLASH);
    __uvisor_sram_start = ORIGIN(RAM);
    __uvisor_sram_end = ORIGIN(RAM) + LENGTH(RAM);

}   /* End of sections */
