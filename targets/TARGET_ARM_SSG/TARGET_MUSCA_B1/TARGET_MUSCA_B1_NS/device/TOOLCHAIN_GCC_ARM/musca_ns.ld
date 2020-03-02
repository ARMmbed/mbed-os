;/*
; * Copyright (c) 2019-2020 Arm Limited
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 gcc_arm.ld
; */

/* Linker script to configure memory regions. */
/* This file will be run trough the pre-processor. */

#include "../../../partition/region_defs.h"
#include "../cmsis_nvic.h"

/* Stack size is 1K for Mbed-OS */
#if !defined(MBED_BOOT_STACK_SIZE)
  #define MBED_BOOT_STACK_SIZE      0x400
#endif

MEMORY
{
  CODE_RAM (rx)  : ORIGIN = NS_CODE_START, LENGTH = ((TOTAL_CODE_SRAM_SIZE / 2) - BL2_HEADER_SIZE)
  /* Vector table is copied to RAM, so RAM address needs to be adjusted */
  RAM   (rwx) : ORIGIN = NVIC_RAM_VECTOR_LIMIT, LENGTH = (NS_DATA_SIZE - NVIC_RAM_VECTOR_SIZE)
}

__stack_size__ = MBED_BOOT_STACK_SIZE;

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)

ENTRY(Reset_Handler)

SECTIONS
{
    /* Startup section is loaded to Flash and runs from Flash */
    .startup :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;
        __end__ = .;

        KEEP(*(.init))
        KEEP(*(.fini))

    } > CODE_RAM

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > CODE_RAM

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > CODE_RAM
    __exidx_end = .;

    /* The text section is loaded into Flash but runs from SRAM
     * The load address is aligned to 8 bytes
     */
    .text : ALIGN(8)
    {
        *(.text*)

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
    } >CODE_RAM
    . = ALIGN(8); /* This alignment is needed to make the section size 8 bytes aligned */

    /* Create same symbols that armclang does so that in the c files
     * we do not have to deal with the compiler type
     */
    Image$$ER_CODE_SRAM$$Base = ADDR(.text);
    Image$$ER_CODE_SRAM$$Limit = .;
    Image$$ER_CODE_SRAM$$Length = Image$$ER_CODE_SRAM$$Limit - Image$$ER_CODE_SRAM$$Base;
    Load$$ER_CODE_SRAM$$Base = LOADADDR(.text);
    Load$$ER_CODE_SRAM$$Length = Image$$ER_CODE_SRAM$$Length;
    Load$$ER_CODE_SRAM$$Limit = Load$$ER_CODE_SRAM$$Base + Load$$ER_CODE_SRAM$$Length;

    /* To copy multiple ROM to RAM sections,
     * define etext2/data2_start/data2_end and
     * define __STARTUP_COPY_MULTIPLE in startup_cmsdk_musca_ns.S */
    .copy.table :
    {
        . = ALIGN(8);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        LONG (DEFINED(__etext2) ? __etext2 : 0)
        LONG (DEFINED(__data2_start__) ? __data2_start__ : 0)
        LONG (DEFINED(__data2_start__) ? __data2_end__ - __data2_start__ : 0)
        __copy_table_end__ = .;
    } > CODE_RAM

    /* To clear multiple BSS sections,
     * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_cmsdk_musca_ns.S */
    .zero.table :
    {
        . = ALIGN(8);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        LONG (DEFINED(__bss2_start__) ? __bss2_start__ : 0)
        LONG (DEFINED(__bss2_start__) ? __bss2_end__ - __bss2_start__ : 0)
        __zero_table_end__ = .;
    } > CODE_RAM

    __etext = .;

    .data : ALIGN(8)
    {
        __data_start__ = .;
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

    } > RAM AT>CODE_RAM

    .bss :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
    } > RAM

    bss_size = __bss_end__ - __bss_start__;

    .stack :
    {
        . = ALIGN(8);
        __StackLimit = .;
        KEEP(*(.stack*))
        . += __stack_size__;
        __StackTop = .;
    } > RAM
    PROVIDE(__stack = __StackTop);

    .heap (COPY):
    {
        . = ALIGN(8);
        __HeapBase = .;
        __end__ = .;
        end = __end__;
        KEEP(*(.heap*))
        . += (ORIGIN(RAM) + LENGTH(RAM) - .);
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
    } > RAM

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackTop <= (NS_DATA_START + NS_DATA_SIZE), "RAM region overflowed")
}
