/* Linker script for Silicon Labs EFM32GG devices */
/*                                                                  */
/* This file is subject to the license terms as defined in ARM's    */
/* CMSIS END USER LICENSE AGREEMENT.pdf, governing the use of       */
/* Example Code.                                                    */
/*                                                                  */
/* Silicon Laboratories, Inc. 2015                                  */
/*                                                                  */
/* Version 4.2.0 */
/*                                                                  */

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0x00000000
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 1048576
#endif

#if !defined(MBED_BOOT_STACK_SIZE)
    #define MBED_BOOT_STACK_SIZE 0x400
#endif

/* With the RTOS in use, this does not affect the main stack size. The size of
 * the stack where main runs is determined via the RTOS. */
STACK_SIZE = MBED_BOOT_STACK_SIZE;

HEAP_SIZE = 0x6000;

MEMORY
{
  FLASH (rx) : ORIGIN = MBED_APP_START, LENGTH = MBED_APP_SIZE
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 131072
}
/* MBED: mbed needs to be able to dynamically set the interrupt vector table.
 * We make room for the table at the very beginning of RAM, i.e. at
 * 0x20000000. We need (16+39) * sizeof(uint32_t) = 220 4(8-byte aligned) bytes for EFM32GG */
__vector_size = 0xE0;

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __copy_table_start__
 *   __copy_table_end__
 *   __zero_table_start__
 *   __zero_table_end__
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
 *   __Vectors_End
 *   __Vectors_Size
 */
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
   * uncomment .copy.table section and,
   * define __STARTUP_COPY_MULTIPLE in startup_ARMCMx.S */
  /*
  .copy.table :
  {
    . = ALIGN(8);
    __copy_table_start__ = .;
    LONG (__etext)
    LONG (__data_start__)
    LONG (__data_end__ - __data_start__)
    LONG (__etext2)
    LONG (__data2_start__)
    LONG (__data2_end__ - __data2_start__)
    __copy_table_end__ = .;
  } > FLASH
  */

  /* To clear multiple BSS sections,
   * uncomment .zero.table section and,
   * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_ARMCMx.S */
  /*
  .zero.table :
  {
    . = ALIGN(8);
    __zero_table_start__ = .;
    LONG (__bss_start__)
    LONG (__bss_end__ - __bss_start__)
    LONG (__bss2_start__)
    LONG (__bss2_end__ - __bss2_start__)
    __zero_table_end__ = .;
  } > FLASH
  */


  .data :
  {
    PROVIDE(__etext = LOADADDR(.data));    /* Define a global symbol at end of code, */
    PROVIDE(__DATA_ROM = LOADADDR(.data)); /* Symbol is used by startup for data initialization. */

    __data_start__ = .;
    *("dma")
    PROVIDE( __start_vector_table__ = .);
    . += __vector_size;
    PROVIDE( __end_vector_table__ = .);
    *(vtable)
    *(.data*)
    . = ALIGN (8);
    *(.ram)

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

  } > RAM AT > FLASH


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

  .bss :
  {
    . = ALIGN(8);
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
  } > RAM

  .heap (NOLOAD):
  {
    __HeapBase = .;
    __end__ = .;
    end = __end__;
    _end = __end__;
    . = ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE;
    __HeapLimit = .;
  } > RAM

  __StackTop = ORIGIN(RAM) + LENGTH(RAM);
  __stack = __StackTop;
  __StackLimit = __StackTop - STACK_SIZE;

  ASSERT(__StackLimit >= __HeapLimit, "Region RAM overflowed with stack and heap")

}
