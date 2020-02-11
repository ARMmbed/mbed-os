;/*
; * Copyright (c) 2018-2019 ARM Limited
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

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

/* Linker script to configure memory regions. */
/* This file will be run trough the pre-processor. */

#include "../../../partition/region_defs.h"

#if !defined(TFM_LVL)
  #define TFM_LVL   1
#endif

MEMORY
{
  FLASH   (rx)  : ORIGIN = S_CODE_START, LENGTH = S_CODE_SIZE
  RAM     (rwx) : ORIGIN = S_DATA_START, LENGTH = S_DATA_SIZE
  VENEERS (rx)  : ORIGIN = CMSE_VENEER_REGION_START, LENGTH = CMSE_VENEER_REGION_SIZE
}

HEAP_SIZE = S_HEAP_SIZE;
__heap_size__  = S_HEAP_SIZE;
__psp_stack_size__ = S_PSP_STACK_SIZE;
__msp_init_stack_size__ = S_MSP_STACK_SIZE;

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a libc_nano.a)

ENTRY(Reset_Handler)

SECTIONS
{
    .TFM_VECTORS : ALIGN(4)
    {
        __vectors_start__ = .;
        KEEP(*(.vectors))
        *startup*(.text*)
        . = ALIGN(4);
        __vectors_end__ = .;
    } > FLASH

#if TFM_LVL == 1
    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (LOADADDR(.TFM_DATA))
        LONG (ADDR(.TFM_DATA))
        LONG (SIZEOF(.TFM_DATA))
        __copy_table_end__ = .;
    } > FLASH

    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (ADDR(.TFM_BSS))
        LONG (SIZEOF(.TFM_BSS))
        LONG (ADDR(.TFM_SECURE_STACK))
        LONG (SIZEOF(.TFM_SECURE_STACK))
        LONG (ADDR(.TFM_UNPRIV_SCRATCH))
        LONG (SIZEOF(.TFM_UNPRIV_SCRATCH))
        __zero_table_end__ = .;
    } > FLASH

#else /* TFM_LVL == 1 */
    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (LOADADDR(.TFM_DATA))
        LONG (ADDR(.TFM_DATA))
        LONG (SIZEOF(.TFM_DATA))
        LONG (LOADADDR(.TFM_UNPRIV_RO_DATA))
        LONG (ADDR(.TFM_UNPRIV_RO_DATA))
        LONG (SIZEOF(.TFM_UNPRIV_RO_DATA))
        LONG (LOADADDR(.TFM_SP_PLATFORM_DATA))
        LONG (ADDR(.TFM_SP_PLATFORM_DATA))
        LONG (SIZEOF(.TFM_SP_PLATFORM_DATA))
        __copy_table_end__ = .;
    } > FLASH

    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (ADDR(.TFM_BSS))
        LONG (SIZEOF(.TFM_BSS))
        LONG (ADDR(.TFM_UNPRIV_RO_BSS))
        LONG (SIZEOF(.TFM_UNPRIV_RO_BSS))
        LONG (ADDR(.TFM_SP_PLATFORM_BSS))
        LONG (SIZEOF(.TFM_SP_PLATFORM_BSS))
        LONG (ADDR(.TFM_SP_PLATFORM_STACK))
        LONG (SIZEOF(.TFM_SP_PLATFORM_STACK))
        LONG (ADDR(.TFM_UNPRIV_SCRATCH))
        LONG (SIZEOF(.TFM_UNPRIV_SCRATCH))
        __zero_table_end__ = .;
    } > FLASH

    .TFM_UNPRIV_CODE : ALIGN(32)
    {
        *libc_nano*:*(.text*)
        *libc_nano*:*(.rodata*)
        *tfm_spm_services.o(.text*)
        *tfm_spm_services.o(.rodata*)
        *platform_retarget_dev.o(.text*)
        *platform_retarget_dev.o(.rodata*)
        *(SFN)
        *libgcc*:*(.text*)
        *libgcc*:*(.rodata*)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_UNPRIV_CODE$$RO$$Base = ADDR(.TFM_UNPRIV_CODE);
    Image$$TFM_UNPRIV_CODE$$RO$$Limit = ADDR(.TFM_UNPRIV_CODE) + SIZEOF(.TFM_UNPRIV_CODE);

    .TFM_SP_PLATFORM : ALIGN(32)
    {
        *tfm_platform*:*(.text*)
        *tfm_platform*:*(.rodata*)
        *(TFM_SP_PLATFORM_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_PLATFORM$$RO$$Base = ADDR(.TFM_SP_PLATFORM);
    Image$$TFM_SP_PLATFORM$$RO$$Limit = ADDR(.TFM_SP_PLATFORM) + SIZEOF(.TFM_SP_PLATFORM);
    Image$$TFM_SP_PLATFORM$$Base = ADDR(.TFM_SP_PLATFORM);
    Image$$TFM_SP_PLATFORM$$Limit = ADDR(.TFM_SP_PLATFORM) + SIZEOF(.TFM_SP_PLATFORM);


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

#endif /* TFM_LVL == 1 */

    .ER_TFM_CODE :
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

    OVERLAY S_DATA_START :
    {
        /* shared_data and msp_stack are overlapping on purpose when
        * msp_stack is extended until the beginning of RAM, when shared_date
        * was read out by partitions
        */
        .tfm_bl2_shared_data
        {
            . = ALIGN(32);
            . += BOOT_TFM_SHARED_DATA_SIZE;
        }

        .msp_stack
        {
            . =  ALIGN(32);
            . += S_MSP_STACK_SIZE;
        }
    } > RAM

    Image$$ARM_LIB_STACK_MSP$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);

    .psp_stack :
    {
        . =  ALIGN(32);
        . += S_PSP_STACK_SIZE;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.psp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.psp_stack) + SIZEOF(.psp_stack);

#if TFM_LVL == 1

    .heap :
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += S_HEAP_SIZE;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
    } > RAM
    Image$$ARM_LIB_HEAP$$ZI$$Base = ADDR(.heap);
    Image$$ARM_LIB_HEAP$$ZI$$Limit = ADDR(.heap) + SIZEOF(.heap);

    .TFM_SECURE_STACK :
    {
        . = ALIGN(128);
        . += 0x1000;
    } > RAM
    Image$$TFM_SECURE_STACK$$ZI$$Base = ADDR(.TFM_SECURE_STACK);
    Image$$TFM_SECURE_STACK$$ZI$$Limit = ADDR(.TFM_SECURE_STACK) + SIZEOF(.TFM_SECURE_STACK);

    .TFM_UNPRIV_SCRATCH :
    {
        . = ALIGN(32);
        . += 0x400;
    } > RAM
    Image$$TFM_UNPRIV_SCRATCH$$ZI$$Base = ADDR(.TFM_UNPRIV_SCRATCH);
    Image$$TFM_UNPRIV_SCRATCH$$ZI$$Limit = ADDR(.TFM_UNPRIV_SCRATCH) + SIZEOF(.TFM_UNPRIV_SCRATCH);
#else /* TFM_LVL == 1 */
    .TFM_UNPRIV_RO_DATA :
    {
        */tfm_spm_services.o(.data*)
        */platform_retarget_dev.o(.data*)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_UNPRIV_RO_DATA$$RW$$Base = ADDR(.TFM_UNPRIV_RO_DATA);
    Image$$TFM_UNPRIV_RO_DATA$$RW$$Limit = ADDR(.TFM_UNPRIV_RO_DATA) + SIZEOF(.TFM_UNPRIV_RO_DATA);

    .TFM_UNPRIV_RO_BSS : ALIGN(32)
    {
        */tfm_spm_services.o(.bss*)
        */platform_retarget_dev.o(.bss*)
        */tfm_spm_services.o(COMMON)
        */platform_retarget_dev.o(COMMON)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_UNPRIV_RO_DATA$$ZI$$Base = ADDR(.TFM_UNPRIV_RO_BSS);
    Image$$TFM_UNPRIV_RO_DATA$$ZI$$Limit = ADDR(.TFM_UNPRIV_RO_BSS) + SIZEOF(.TFM_UNPRIV_RO_BSS);

    .TFM_UNPRIV_SCRATCH : ALIGN(32)
    {
        . += 0x400;
    } > RAM AT> FLASH
    Image$$TFM_UNPRIV_SCRATCH$$ZI$$Base = ADDR(.TFM_UNPRIV_SCRATCH);
    Image$$TFM_UNPRIV_SCRATCH$$ZI$$Limit = ADDR(.TFM_UNPRIV_SCRATCH) + SIZEOF(.TFM_UNPRIV_SCRATCH);

    .TFM_SP_PLATFORM_DATA : ALIGN(32)
    {
        *tfm_platform*:*(.data*)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_PLATFORM_DATA$$RW$$Base = ADDR(.TFM_SP_PLATFORM_DATA);
    Image$$TFM_SP_PLATFORM_DATA$$RW$$Limit = ADDR(.TFM_SP_PLATFORM_DATA) + SIZEOF(.TFM_SP_PLATFORM_DATA);

    .TFM_SP_PLATFORM_BSS : ALIGN(32)
    {
        *tfm_platform*:*(.bss*)
        *tfm_platform*:*(COMMON)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_PLATFORM_DATA$$ZI$$Base = ADDR(.TFM_SP_PLATFORM_BSS);
    Image$$TFM_SP_PLATFORM_DATA$$ZI$$Limit = ADDR(.TFM_SP_PLATFORM_BSS) + SIZEOF(.TFM_SP_PLATFORM_BSS);

    .TFM_SP_PLATFORM_STACK : ALIGN(128)
    {
        . += 0x0400;
    } > RAM AT> FLASH
    Image$$TFM_SP_PLATFORM_STACK$$ZI$$Base = ADDR(.TFM_SP_PLATFORM_STACK);
    Image$$TFM_SP_PLATFORM_STACK$$ZI$$Limit = ADDR(.TFM_SP_PLATFORM_STACK) + SIZEOF(.TFM_SP_PLATFORM_STACK);

#endif /* TFM_LVL == 1 */

    .TFM_DATA :
    {
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

    } > RAM AT> FLASH
    Image$$ER_TFM_DATA$$RW$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$RW$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA);

    .TFM_BSS :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM
    Image$$ER_TFM_DATA$$ZI$$Base = ADDR(.TFM_BSS);
    Image$$ER_TFM_DATA$$ZI$$Limit = ADDR(.TFM_BSS) + SIZEOF(.TFM_BSS);

    Image$$ER_TFM_DATA$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA) + SIZEOF(.TFM_BSS);

    ASSERT(Image$$ER_TFM_DATA$$Limit <= S_DATA_START + S_DATA_SIZE, "Exceeding secure RAM")

    /*
     * Place the CMSE Veneers (containing the SG instruction) after the code, in a
     * separate 32 bytes aligned region so that the SAU can programmed to just set
     * this region as Non-Secure Callable.
     */
    .gnu.sgstubs : ALIGN(32)
    {
        *(.gnu.sgstubs*)
        . = ALIGN(32);
    } > VENEERS AT> VENEERS
    Image$$ER_CODE_CMSE_VENEER$$Base = ADDR(.gnu.sgstubs);
    Image$$ER_CODE_CMSE_VENEER$$Limit = ADDR(.gnu.sgstubs) + SIZEOF(.gnu.sgstubs);

    Load$$LR$$LR_NS_PARTITION$$Base = NS_PARTITION_START;

    Load$$LR$$LR_SECONDARY_PARTITION$$Base = SECONDARY_PARTITION_START;

    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
    PROVIDE(__StackTop = __stack);
    PROVIDE(__StackLimit = __StackTop - SIZEOF(.psp_stack));
}
