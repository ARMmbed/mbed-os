/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
 *
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

/*
 * Nuvoton M2351 GCC linker script file
 */

#include "../../../device/partition_M2351_mem.h"

#if !defined(MBED_BOOT_STACK_SIZE)
#define MBED_BOOT_STACK_SIZE    0x400
#endif

StackSize = MBED_BOOT_STACK_SIZE;

MEMORY
{
  VECTORS (rx)          : ORIGIN = MBED_APP_START,          LENGTH = 0x00000400
  FLASH (rx)            : ORIGIN = MBED_APP_START + 0x400,  LENGTH = MBED_APP_SIZE - 0x400
  RAM_INTERN (rwx)      : ORIGIN = MBED_RAM_APP_START,      LENGTH = MBED_RAM_APP_SIZE
}

/**
 * Must match cmsis_nvic.h
 */
__vector_size = 4 * (16 + 102);

 
/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 * Reset_Handler : Entry of reset handler
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
    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.vector_table))
         . = ALIGN(8);
    } > VECTORS

    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (LOADADDR(.data))
        LONG (ADDR(.data))
        LONG (SIZEOF(.data))
        __copy_table_end__ = .;
    } > FLASH

    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (ADDR(.bss))
        LONG (SIZEOF(.bss))
        __zero_table_end__ = .;
    } > FLASH

    .text :
    {
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

    /* Stack Pointer (SP) configuration with GCC_ARM
     *
     * SP would go through the following configuration sequence:
     * (1) SP(MSP) <- Entry 0 of vector table (on H/W reset)
     * (2) Switch SP from MSP to PSP in startup file (as TFM requests)
     * (3) SP(PSP) <- __stack (in _start(), C/C++ runtime initialization)
     *
     * S(3) may cause SP(PSP) incorrectly configured. To avoid this error, __stack
     * is assigned conditionally according to Secure PSA or not.
     */

    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        __StackLimit = .;
        . += StackSize;
        __StackTop = .;
    } > RAM_INTERN

    PROVIDE(__stack = __StackTop);

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx :
    {
       __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
       __exidx_end = .;
    } > FLASH

    /* Relocate vector table in SRAM */
    .isr_vector.reloc (NOLOAD) :
    {
        . = ALIGN(1 << LOG2CEIL(__vector_size));
        PROVIDE(__start_vector_table__ = .);
        . += __vector_size;
        PROVIDE(__end_vector_table__ = .);
    } > RAM_INTERN

    .data :
    {
        PROVIDE( __etext = LOADADDR(.data) );

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

        /* All data end */
        . = ALIGN(32);
        __data_end__ = .;

    } >RAM_INTERN AT>FLASH

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM_INTERN

    .heap (NOLOAD) :
    {
        . = ALIGN(8);
        __end__ = .;
        end = __end__;
        *(.heap*);
        . += (ORIGIN(RAM_INTERN) + LENGTH(RAM_INTERN) - .);        
        __HeapLimit = .;
    } > RAM_INTERN
    Image$$ARM_LIB_HEAP$$ZI$$Base = ADDR(.heap);
    Image$$ARM_LIB_HEAP$$ZI$$Limit = ADDR(.heap) + SIZEOF(.heap);

    PROVIDE(__heap_size = SIZEOF(.heap));
    PROVIDE(__mbed_sbrk_start = ADDR(.heap));
    PROVIDE(__mbed_krbs_start = ADDR(.heap) + SIZEOF(.heap));
}
