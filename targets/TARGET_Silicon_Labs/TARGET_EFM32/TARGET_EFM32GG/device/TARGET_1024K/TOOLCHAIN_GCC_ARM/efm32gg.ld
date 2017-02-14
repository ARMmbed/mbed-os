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

/* With the RTOS in use, this does not affect the main stack size. The size of
 * the stack where main runs is determined via the RTOS. */
STACK_SIZE = 0x400;

/* This is the guaranteed minimum available heap size for an application. When
 * uVisor is enabled, this is also the maximum available heap size. The
 * HEAP_SIZE value is set by uVisor porters to balance the size of the legacy
 * heap and the page heap in uVisor applications. */
HEAP_SIZE = 0x6000;

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1048576
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 131072
}
/* MBED: mbed needs to be able to dynamically set the interrupt vector table.
 * We make room for the table at the very beginning of RAM, i.e. at
 * 0x20000000. We need (16+39) * sizeof(uint32_t) = 220 bytes for EFM32GG */
__vector_size = 0xDC;

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
  /* Note: The uVisor expects the text section at a fixed location, as specified
               by the porting process configuration parameter: FLASH_OFFSET. */
  __UVISOR_FLASH_OFFSET = 0x100;
  __UVISOR_FLASH_START = ORIGIN(FLASH) + __UVISOR_FLASH_OFFSET;
  .text :
  {
    KEEP(*(.vectors))
    __Vectors_End = .;
    __Vectors_Size = __Vectors_End - __Vectors;
    __end__ = .;

    /* uVisor code and data */
    . = __UVISOR_FLASH_OFFSET;
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

  /* To copy multiple ROM to RAM sections,
   * uncomment .copy.table section and,
   * define __STARTUP_COPY_MULTIPLE in startup_ARMCMx.S */
  /*
  .copy.table :
  {
    . = ALIGN(4);
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
    . = ALIGN(4);
    __zero_table_start__ = .;
    LONG (__bss_start__)
    LONG (__bss_end__ - __bss_start__)
    LONG (__bss2_start__)
    LONG (__bss2_end__ - __bss2_start__)
    __zero_table_end__ = .;
  } > FLASH
  */

  /* uVisor own memory and private box memories
  /* If uVisor shares the SRAM with the OS/app, ensure that this section is
   * the first one after the VTOR relocation section. */
  /* Note: The uVisor expects this section at a fixed location, as specified
           by the porting process configuration parameter: SRAM_OFFSET. */
  __UVISOR_SRAM_OFFSET = 0x0;
  __UVISOR_SRAM_START = ORIGIN(RAM) + __UVISOR_SRAM_OFFSET;
  .uvisor.bss __UVISOR_SRAM_START (NOLOAD):
  {
    . = ALIGN(32);
    __uvisor_bss_start = .;

    /* Protected uVisor own BSS section */
    . = ALIGN(32);
    __uvisor_bss_main_start = .;
    KEEP(*(.keep.uvisor.bss.main))
    . = ALIGN(32);
    __uvisor_bss_main_end = .;

    /* Protected uVisor boxes' static memories */
    . = ALIGN(32);
    __uvisor_bss_boxes_start = .;
    KEEP(*(.keep.uvisor.bss.boxes))
    . = ALIGN(32);
    __uvisor_bss_boxes_end = .;

    . = ALIGN(32);
    __uvisor_bss_end = .;
  } > RAM

  /* Heap space for the page allocator
  /* If uVisor shares the SRAM with the OS/app, ensure that this section is
   * the first one after the uVisor BSS section. Otherwise, ensure it is the
   * first one after the VTOR relocation section. */
  .page_heap (NOLOAD) :
  {
    . = ALIGN(32);
    __uvisor_page_start = .;
    KEEP(*(.keep.uvisor.page_heap))
    . = ALIGN( (1 << LOG2CEIL(LENGTH(RAM))) / 8);
    __uvisor_page_end = .;
  } > RAM

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
    . = ALIGN (4);
    *(.ram)

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

  } > RAM AT > FLASH

  /* uVisor configuration section
   * This section must be located after all other flash regions. */
  .uvisor.secure :
  {
    . = ALIGN(32);
    __uvisor_secure_start = .;

    /* uVisor secure boxes configuration tables */
    . = ALIGN(32);
    __uvisor_cfgtbl_start = .;
    KEEP(*(.keep.uvisor.cfgtbl))
    . = ALIGN(32);
    __uvisor_cfgtbl_end = .;

    /* Pointers to the uVisor secure boxes configuration tables */
    /* Note: Do not add any further alignment here, as uVisor will need to
             have access to the exact list of pointers. */
    __uvisor_cfgtbl_ptr_start = .;
    KEEP(*(.keep.uvisor.cfgtbl_ptr_first))
    KEEP(*(.keep.uvisor.cfgtbl_ptr))
    __uvisor_cfgtbl_ptr_end = .;

    /* Pointers to all boxes register gateways. These are grouped here to
       allow discoverability and firmware verification. */
    __uvisor_register_gateway_ptr_start = .;
    KEEP(*(.keep.uvisor.register_gateway_ptr))
    __uvisor_register_gateway_ptr_end = .;

    . = ALIGN(32);
    __uvisor_secure_end = .;
  } > FLASH

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
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > RAM

  .heap (NOLOAD):
  {
    __uvisor_heap_start = .;
    __HeapBase = .;
    __end__ = .;
    end = __end__;
    _end = __end__;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __uvisor_heap_end = .;
  } > RAM

  __StackTop = ORIGIN(RAM) + LENGTH(RAM);
  __stack = __StackTop;
  __StackLimit = __StackTop - STACK_SIZE;

  ASSERT(__StackLimit >= __HeapLimit, "Region RAM overflowed with stack and heap")

  /* Provide physical memory boundaries for uVisor. */
  __uvisor_flash_start = ORIGIN(FLASH);
  __uvisor_flash_end = ORIGIN(FLASH) + LENGTH(FLASH);
  __uvisor_sram_start = ORIGIN(RAM);
  __uvisor_sram_end = ORIGIN(RAM) + LENGTH(RAM);
  __uvisor_public_sram_start = __uvisor_sram_start;
  __uvisor_public_sram_end = __uvisor_sram_end;
}
