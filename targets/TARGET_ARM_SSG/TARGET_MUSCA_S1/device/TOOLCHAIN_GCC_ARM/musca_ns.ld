;/*
; * Copyright (c) 2019 ARM Limited
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

#include "../../partition/region_defs.h"
#include "../cmsis_nvic.h"

/* Stack size is 1K for Mbed-OS */
#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
  #define MBED_CONF_TARGET_BOOT_STACK_SIZE      0x400
#endif

MEMORY
{
  FLASH (rx)  : ORIGIN = NS_CODE_START, LENGTH = NS_CODE_SIZE
  /* Vector table is copied to RAM, so RAM address needs to be adjusted */
  RAM   (rwx) : ORIGIN = NVIC_RAM_VECTOR_LIMIT, LENGTH = (NS_DATA_SIZE - NVIC_RAM_VECTOR_SIZE)
}

__stack_size__ = MBED_CONF_TARGET_BOOT_STACK_SIZE;

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;
        __end__ = .;

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

    /* To copy multiple ROM to RAM sections,
     * define etext2/data2_start/data2_end and
     * define __STARTUP_COPY_MULTIPLE in startup_cmsdk_musca_ns.S */
    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        LONG (DEFINED(__etext2) ? __etext2 : 0)
        LONG (DEFINED(__data2_start__) ? __data2_start__ : 0)
        LONG (DEFINED(__data2_start__) ? __data2_end__ - __data2_start__ : 0)
        __copy_table_end__ = .;
    } > FLASH

    /* To clear multiple BSS sections,
     * uncomment .zero.table section and,
     * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_cmsdk_musca_ns.S */
    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        LONG (DEFINED(__bss2_start__) ? __bss2_start__ : 0)
        LONG (DEFINED(__bss2_start__) ? __bss2_end__ - __bss2_start__ : 0)
        __zero_table_end__ = .;
    } > FLASH

    __etext = .;

    .data : AT (__etext)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > RAM

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
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
