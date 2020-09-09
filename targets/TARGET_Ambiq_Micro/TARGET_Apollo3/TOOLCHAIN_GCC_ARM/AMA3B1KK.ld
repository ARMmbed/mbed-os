/* 
 * Copyright (c) 2019-2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* stack: dynamic */
/* heap: dynamic */
#define MBED_APP_START 0x0000C000
#define MBED_APP_LENGTH 0x000F4000
#define MBED_RAM_START 0x10000000
#define MBED_RAM_SIZE   384K
#define MBED_BOOT_STACK_SIZE 0x400
#define MBED_RAM0_START MBED_RAM_START
#define MBED_RAM0_SIZE  0x100
#define MBED_RAM1_START (MBED_RAM0_START + MBED_RAM0_SIZE)
#define MBED_RAM1_SIZE  (MBED_RAM_SIZE - (MBED_RAM0_SIZE))

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx) : ORIGIN = MBED_APP_START, LENGTH = MBED_APP_LENGTH /*Modified from 0xC000 to work with SparkFun SVL*/
  RAM_NVIC (rwx) : ORIGIN = MBED_RAM0_START, LENGTH = MBED_RAM0_SIZE
  RAM (rwx)      : ORIGIN = MBED_RAM1_START, LENGTH = MBED_RAM1_SIZE
}

SECTIONS
{
  /* text: executable code */
  /* located in _flash_ */
  .text :
  {
    . = ALIGN(4);
    _stext = .;
    KEEP(*(.isr_vector))
    KEEP(*(.ble_patch))
    *(.text)
    *(.text*)

/*    __init_array_start = .;
    KEEP(*(.init_array*))
    __init_array_end = .; */ /* does this mess up _init()?' it was from Arduinoland (aka owen) */

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

    /* .rodata */
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)

    KEEP(*(.eh_frame*))

    . = ALIGN(4);
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

  __etext = ALIGN(8);

    .data : AT (__etext)
    {
        __data_start__ = .;
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

    } > RAM

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
    } > RAM


  /* bss: zero-initialized symbols */
  /* don't require flash memory to remember their value */
  .bss :
  {
    . = ALIGN(8);
    _sbss = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    _ebss = .;
    __bss_end__ = .;
  } > RAM

  /* heap: RAM memory that can be dynamically allocated in the upward direction (increasing memory addresses) */
  /* _sheap is used to identify the beginning of available dynamic memory */
  .heap (NOLOAD):
  {
    . = ALIGN(4);
    __end__ = .;
    PROVIDE( end = . );
    _sheap = .;
    . = ORIGIN(RAM) + LENGTH(RAM) - MBED_BOOT_STACK_SIZE-8;
    __HeapLimit = .;
  } >RAM

    /* .stack_dummy section doesn't contains any symbols. It is only
    * used for linker to calculate size of stack sections, and assign
    * values to stack symbols later*/
  .stack_dummy (NOLOAD):
  {
      . = ALIGN(8);
      *(.stack*)
  } > RAM


  /* Set stack top to end of RAM, and stack limit move down by
    * size of stack_dummy section*/
  __StackTop = ORIGIN(RAM) + LENGTH(RAM)-8;
  __StackLimit = __StackTop - MBED_BOOT_STACK_SIZE;
  PROVIDE(__stack = __StackTop);
  PROVIDE(_sstack = __StackTop);
}
