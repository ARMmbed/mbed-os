/*!
 * @file       startup_apm32f407xG.ld
 *
 * @brief      Linker script for APM32F40xxG series
 *             1024Kbytes FLASH, 128KByte RAM, 64KByte CCMRAM
 *
 * @version    V1.0.0
 *
 * @date       2023-11-30
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Geehy Semiconductor.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0x08000000
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 1024k
#endif

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE 0x400
#endif

/* specify memory regions */
MEMORY
{
FLASH (rx)      : ORIGIN = MBED_APP_START,    LENGTH = MBED_APP_SIZE
RAM (xrw)       : ORIGIN = 0x20000198,    LENGTH = 128K - 0x198
CCMRAM (xrw)    : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Entry Point */
ENTRY(Reset_Handler)

SECTIONS
{
  .text :
  {
    KEEP(*(.apm32_isr_vector))
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
  } >FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH

  __exidx_start = .;
  .ARM.exidx :
  {
     *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH
  __exidx_end = .;

  __etext = .;
  _start_address_init_data = .;

  .data : AT (__etext)
  {
    __data_start__ = .;
    _start_address_data = .;
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
    _end_address_data = .;

  } > RAM

  .bss :
  {
    . = ALIGN(8);
    __bss_start__ = .;
    _start_address_bss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    _end_address_bss = .;
  } > RAM

  .heap (COPY):
  {
      __end__ = .;
      end = __end__;
      *(.heap*)
      . = ORIGIN(RAM) + LENGTH(RAM) - MBED_CONF_TARGET_BOOT_STACK_SIZE;
      __HeapLimit = .;
  } > RAM

    .stack_dummy (COPY):
    {
        *(.stack*)
    } > RAM

    /* initializes stack on the end of block */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    _end_stack = __StackTop;
    __StackLimit = __StackTop - MBED_CONF_TARGET_BOOT_STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}


