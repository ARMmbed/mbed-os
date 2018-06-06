/*
 * Copyright (c) 2015 ARM Limited
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

/* Linker script to configure memory regions. */

/* Default to no softdevice */
#if !defined(MBED_APP_START)
  #define MBED_APP_START 0x0
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 0x80000
#endif

/* If app_start is 0, do not set aside space for the softdevice */
#if MBED_APP_START == 0
  #define MBED_RAM_START  0x20000000
  #define MBED_RAM_SIZE   0x10000
#else
  #define MBED_RAM_START  0x200031D0
  #define MBED_RAM_SIZE   0xCE30
#endif

#define MBED_RAM0_START MBED_RAM_START
#define MBED_RAM0_SIZE  0xDC
#define MBED_RAM1_START (MBED_RAM_START + MBED_RAM0_SIZE)
#define MBED_RAM1_SIZE  (MBED_RAM_SIZE - MBED_RAM0_SIZE)

MEMORY
{
  FLASH (rx)     : ORIGIN = MBED_APP_START, LENGTH = MBED_APP_SIZE
  RAM_NVIC (rwx) : ORIGIN = MBED_RAM0_START, LENGTH = MBED_RAM0_SIZE
  RAM (rwx)      : ORIGIN = MBED_RAM1_START, LENGTH = MBED_RAM1_SIZE
}


OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

/* Linker script to place sections and symbol values. Should be used together
 * with the other linker script that defines memory regions FLASH and RAM.
 * It references the following symbols that must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines the following symbols that the code can use without definition:
 *   __exidx_start
 *   __exidx_end
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
 */
ENTRY(Reset_Handler)


SECTIONS
{
    .text :
    {
        KEEP(*(.Vectors))
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

    .sdh_soc_observers :
    {
        PROVIDE(__start_sdh_soc_observers = .);
        KEEP(*(SORT(.sdh_soc_observers*)))
        PROVIDE(__stop_sdh_soc_observers = .);
    } > FLASH

    .sdh_stack_observers :
    {
        PROVIDE(__start_sdh_stack_observers = .);
        KEEP(*(SORT(.sdh_stack_observers*)))
        PROVIDE(__stop_sdh_stack_observers = .);
    } > FLASH

    .sdh_req_observers :
    {
        PROVIDE(__start_sdh_req_observers = .);
        KEEP(*(SORT(.sdh_req_observers*)))
        PROVIDE(__stop_sdh_req_observers = .);
    } > FLASH

    .sdh_state_observers :
    {
        PROVIDE(__start_sdh_state_observers = .);
        KEEP(*(SORT(.sdh_state_observers*)))
        PROVIDE(__stop_sdh_state_observers = .);
    } > FLASH

    .sdh_ble_observers :
    {
        PROVIDE(__start_sdh_ble_observers = .);
        KEEP(*(SORT(.sdh_ble_observers*)))
        PROVIDE(__stop_sdh_ble_observers = .);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . = ALIGN(4);
    } > FLASH
    __exidx_end = .;

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

        . = ALIGN(4);
        PROVIDE(__start_fs_data = .);
        KEEP(*(.fs_data))
        PROVIDE(__stop_fs_data = .);
        
        *(.jcr)
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > RAM

    __edata = .;

    .noinit (NOLOAD) :
    {
      PROVIDE(__start_noinit = .);
      KEEP(*(.noinit))
      PROVIDE(__stop_noinit = .);
    } > RAM_NVIC
    
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
        __end__ = .;
        end = __end__;
        *(.heap*);

        /* Expand the heap to reach the stack boundary. */
        ASSERT(. <= (ORIGIN(RAM) + LENGTH(RAM) - 0x800), "heap region overflowed into stack");
        . += (ORIGIN(RAM) + LENGTH(RAM) - 0x800) - .;
    } > RAM
    PROVIDE(__heap_start = ADDR(.heap));
    PROVIDE(__heap_size = SIZEOF(.heap));
    PROVIDE(__mbed_sbrk_start = ADDR(.heap));
    PROVIDE(__mbed_krbs_start = ADDR(.heap) + SIZEOF(.heap));

    /* .stack_dummy section does not contain any symbols. It is only
     * used for the linker script to calculate the size of stack sections
     * and assign values to stack symbols later. */
    .stack (NOLOAD):
    {
        __StackLimit = .;
        *(.stack*)
        . += (ORIGIN(RAM) + LENGTH(RAM) - .);
    } > RAM

    /* Set the stack top to the end of RAM and move down the stack limit by
     * the size of the stack_dummy section. */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - SIZEOF(.stack);
    PROVIDE(__stack = __StackTop);
}
