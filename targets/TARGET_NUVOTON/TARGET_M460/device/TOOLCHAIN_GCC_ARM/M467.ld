/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
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
 * Nuvoton M467 GCC linker script file
 */

#include "../M460_mem.h"

#if !defined(MBED_BOOT_STACK_SIZE)
#define MBED_BOOT_STACK_SIZE        0x400
#endif

#if !defined(NU_HYPERRAM_START)
#define NU_HYPERRAM_START           0x80000000
#endif

#if !defined(NU_HYPERRAM_SIZE)
#define NU_HYPERRAM_SIZE            0x800000
#endif

M_CRASH_DATA_RAM_SIZE = 0x100;
StackSize = MBED_BOOT_STACK_SIZE;

MEMORY
{
  VECTORS (rx)          : ORIGIN = MBED_APP_START, LENGTH = 0x00000400
  FLASH (rx)            : ORIGIN = MBED_APP_START + 0x400, LENGTH = MBED_APP_SIZE - 0x00000400
  RAM_INTERN (rwx)      : ORIGIN = MBED_RAM_APP_START, LENGTH = MBED_RAM_APP_SIZE
  HYPERRAM (rwx)        : ORIGIN = NU_HYPERRAM_START, LENGTH = NU_HYPERRAM_SIZE
}

/**
 * Must match cmsis_nvic.h
 */
__vector_size = 4 * (16 + 128);

 
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

        /* .data located at internal SRAM */
        LONG (LOADADDR(.data))
        LONG (ADDR(.data))
        LONG (SIZEOF(.data))

        /* .text.nu.hyperram located at HyperRAM */
        LONG (LOADADDR(.text.nu.hyperram))
        LONG (ADDR(.text.nu.hyperram))
        LONG (SIZEOF(.text.nu.hyperram))

        /* .data.nu.hyperram located at HyperRAM */
        LONG (LOADADDR(.data.nu.hyperram))
        LONG (ADDR(.data.nu.hyperram))
        LONG (SIZEOF(.data.nu.hyperram))

        __copy_table_end__ = .;
    } > FLASH

    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;

        /* .bss located at internal SRAM */
        LONG (ADDR(.bss))
        LONG (SIZEOF(.bss))

        /* .bss.nu.hyperram located at HyperRAM */
        LONG (ADDR(.bss.nu.hyperram))
        LONG (SIZEOF(.bss.nu.hyperram))

        __zero_table_end__ = .;
    } > FLASH

    /* First match used, so place in front of .text */
    .text.nu.hyperram :
    {
        *(.text.nu.hyperram)
    } >HYPERRAM AT>FLASH

    Image$$NU_HYPERRAM$$RO$$Base = ADDR(.text.nu.hyperram);
    Image$$NU_HYPERRAM$$RO$$Limit = ADDR(.text.nu.hyperram) + SIZEOF(.text.nu.hyperram);
    Image$$NU_HYPERRAM$$RO$$Length = SIZEOF(.text.nu.hyperram);

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

    /* .stack section doesn't contains any symbols. It is only
     * used for linker to reserve space for the main stack section
     * WARNING: .stack should come immediately after the last secure memory
     * section.  This provides stack overflow detection. */
    .stack (NOLOAD):
    {
        __StackLimit = .;
        *(.stack*);
        . += StackSize - (. - __StackLimit);
    } > RAM_INTERN

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ADDR(.stack) + SIZEOF(.stack);
    __StackLimit = ADDR(.stack);
    PROVIDE(__stack = __StackTop);

    /* Relocate vector table in SRAM */
    .isr_vector.reloc (NOLOAD) :
    {
        . = ALIGN(1 << LOG2CEIL(__vector_size));
        PROVIDE(__start_vector_table__ = .);
        . += __vector_size;
        PROVIDE(__end_vector_table__ = .);
    } > RAM_INTERN
    
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
    } > RAM_INTERN 

    /* First match used, so place in front of .data */
    .data.nu.hyperram :
    {
        *(.data.nu.hyperram)
    } >HYPERRAM AT>FLASH

    Image$$NU_HYPERRAM$$RW$$Base = ADDR(.data.nu.hyperram);
    Image$$NU_HYPERRAM$$RW$$Limit = ADDR(.data.nu.hyperram) + SIZEOF(.data.nu.hyperram);
    Image$$NU_HYPERRAM$$RW$$Length = SIZEOF(.data.nu.hyperram);

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


    /* Uninitialized data section
     * This region is not initialized by the C/C++ library and can be used to
     * store state across soft reboots. */
    .uninitialized (NOLOAD):
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > RAM_INTERN

    /* First match used, so place in front of .bss */
    /* If a variable defined with __attribute__((section())) keyword the
     * variable is treated like an initialized variable. To not waste memory
     * NOLOAD attribute used here. The whole section is zero initialized by
     * adding section information to .zero.table */
    .bss.nu.hyperram (NOLOAD):
    {
        *(.bss.nu.hyperram)
    } > HYPERRAM

    Image$$NU_HYPERRAM$$ZI$$Base = ADDR(.bss.nu.hyperram);
    Image$$NU_HYPERRAM$$ZI$$Limit = ADDR(.bss.nu.hyperram) + SIZEOF(.bss.nu.hyperram);
    Image$$NU_HYPERRAM$$ZI$$Length = SIZEOF(.bss.nu.hyperram);

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM_INTERN
    
    .heap (NOLOAD):
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        *(.heap*);
        . += (ORIGIN(RAM_INTERN) + LENGTH(RAM_INTERN) - .);
        __HeapLimit = .;
    } > RAM_INTERN
    PROVIDE(__heap_size = SIZEOF(.heap));
    PROVIDE(__mbed_sbrk_start = ADDR(.heap));
    PROVIDE(__mbed_krbs_start = ADDR(.heap) + SIZEOF(.heap));
}
