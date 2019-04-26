/*
 * Copyright (c) 2019-2020, Nuvoton Technology Corporation
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

#include "../partition_M2351_mem.h"

#ifndef NU_TZ_NSC_SIZE
#define NU_TZ_NSC_SIZE          (0x1000)
#endif

#if defined(DOMAIN_NS) && DOMAIN_NS

#ifndef MBED_APP_START
#define MBED_APP_START          NU_ROM_START_NS
#endif

#ifndef MBED_APP_SIZE
#define MBED_APP_SIZE           NU_ROM_SIZE_NS
#endif

#ifndef MBED_RAM_APP_START
#define MBED_RAM_APP_START      NU_RAM_START_NS
#endif

#ifndef MBED_RAM_APP_SIZE
#define MBED_RAM_APP_SIZE       NU_RAM_SIZE_NS
#endif

#if !defined(MBED_BOOT_STACK_SIZE)
#define MBED_BOOT_STACK_SIZE    0x400
#endif

#else

#ifndef MBED_APP_START
#define MBED_APP_START          NU_ROM_START_S
#endif

#ifndef MBED_APP_SIZE
#define MBED_APP_SIZE           NU_ROM_SIZE_S
#endif

#ifndef MBED_RAM_APP_START
#define MBED_RAM_APP_START      NU_RAM_START_S
#endif

#ifndef MBED_RAM_APP_SIZE
#define MBED_RAM_APP_SIZE       NU_RAM_SIZE_S
#endif

#ifndef MBED_BOOT_STACK_SIZE
#define MBED_BOOT_STACK_SIZE    0x400
#endif

#endif

StackSize = MBED_BOOT_STACK_SIZE;

/* Requirements for NSC location
 *
 * 1. By IDAU, 0~0x4000 is secure. NSC can only locate in 0x4000~0x10000000.
 * 2. Greentea flash IAP uses last 4 sectors for its test. Avoid this range.
 * 3. Greentea NVSTORE uses last 2 sectors or 4KiB x 2 for its test. Avoid this range.
 * 4. NSC region size defaults to 4KiB if not defined.
 */
#ifndef NU_TZ_NSC_START
#define NU_TZ_NSC_START         (MBED_APP_START + MBED_APP_SIZE - 0x2000 - NU_TZ_NSC_SIZE)
#endif


#if defined(DOMAIN_NS) && DOMAIN_NS

MEMORY
{
  VECTORS (rx)          : ORIGIN = MBED_APP_START,          LENGTH = 0x00000400
  FLASH (rx)            : ORIGIN = MBED_APP_START + 0x400,  LENGTH = MBED_APP_SIZE - 0x400
  RAM_INTERN (rwx)      : ORIGIN = MBED_RAM_APP_START,      LENGTH = MBED_RAM_APP_SIZE
}

#else

MEMORY
{
  VECTORS (rx)          : ORIGIN = MBED_APP_START,          LENGTH = 0x00000400
  FLASH (rx)            : ORIGIN = MBED_APP_START + 0x400,  LENGTH = NU_TZ_NSC_START - MBED_APP_START - 0x400
  NSC_FLASH (rx)        : ORIGIN = NU_TZ_NSC_START,         LENGTH = NU_TZ_NSC_SIZE
  RAM_INTERN (rwx)      : ORIGIN = MBED_RAM_APP_START,      LENGTH = MBED_RAM_APP_SIZE
}

#endif

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

    /* ensure that uvisor bss is at the beginning of memory */
    
    .uvisor.bss (NOLOAD):
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

        /* Ensure log2(size) alignment of the uvisor region, to ensure that the region can be effectively protected by the MPU. */
        . = ALIGN(1 << LOG2CEIL(__uvisor_bss_boxes_end - __uvisor_bss_start));
        __uvisor_bss_end = .;
    } > RAM_INTERN

    .text :
    {
        /* uVisor code and data */
        . = ALIGN(8);
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

#if (! defined(DOMAIN_NS)) || (! DOMAIN_NS)
    /* Veneer$$CMSE : */
    .gnu.sgstubs :
    {
       __sgstubs_start = .;
        *(.gnu.sgstubs.*)
       __sgstubs_end = .;    
    } > NSC_FLASH

    __nu_tz_nsc_start = NU_TZ_NSC_START;
    __nu_tz_nsc_size = NU_TZ_NSC_SIZE;

    /* By IDAU, 0~0x4000 is secure. NSC can only locate in 0x4000~0x10000000. */
    ASSERT(__sgstubs_start >= 0x4000, "By IDAU, 0~0x4000 is secure. NSC can only locate in 0x4000~0x10000000.")
#endif
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

        /* pointers to uvisor secure boxes configuration tables */
        /* note: no further alignment here, we need to have the exact list of pointers */
        __uvisor_cfgtbl_ptr_start = .;
        KEEP(*(.keep.uvisor.cfgtbl_ptr_first))
        KEEP(*(.keep.uvisor.cfgtbl_ptr))
        __uvisor_cfgtbl_ptr_end = .;

        /* the following symbols are kept for backward compatibility and will be soon
         * deprecated; applications actively using uVisor (__uvisor_mode == UVISOR_ENABLED)
         * will need to use uVisor 0.8.x or above, or the security assertions will halt the
         * system */
        /************************/
        __uvisor_data_src = .;
        __uvisor_data_start = .;
        __uvisor_data_end = .;
        /************************/

        . = ALIGN(32);
        __uvisor_secure_end = .;
    } >FLASH

    .uninitialized (NOLOAD):
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > RAM_INTERN

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM_INTERN

    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        *(.heap*);
        . += (ORIGIN(RAM_INTERN) + LENGTH(RAM_INTERN) - .);        
        __HeapLimit = .;
    } > RAM_INTERN
    
    PROVIDE(__heap_size = SIZEOF(.heap));
    PROVIDE(__mbed_sbrk_start = ADDR(.heap));
    PROVIDE(__mbed_krbs_start = ADDR(.heap) + SIZEOF(.heap));
    
    /* Provide physical memory boundaries for uVisor. */    
    __uvisor_flash_start = ORIGIN(VECTORS);
    __uvisor_flash_end = ORIGIN(FLASH) + LENGTH(FLASH);
    __uvisor_sram_start = ORIGIN(RAM_INTERN);
    __uvisor_sram_end = ORIGIN(RAM_INTERN) + LENGTH(RAM_INTERN);
    
}
