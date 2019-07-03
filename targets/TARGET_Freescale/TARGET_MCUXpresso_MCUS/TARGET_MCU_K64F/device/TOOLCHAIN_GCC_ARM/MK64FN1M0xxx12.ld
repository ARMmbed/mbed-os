/*
** ###################################################################
**     Processors:          MK64FN1M0CAJ12
**                          MK64FN1M0VDC12
**                          MK64FN1M0VLL12
**                          MK64FN1M0VLQ12
**                          MK64FN1M0VMD12
**
**     Compiler:            GNU C Compiler
**     Reference manual:    K64P144M120SF5RM, Rev.2, January 2014
**     Version:             rev. 2.9, 2016-03-21
**     Build:               b160613
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright (c) 2016 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

__ram_vector_table__ = 1;

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 0x100000
#endif

#if !defined(MBED_BOOT_STACK_SIZE)
    #define MBED_BOOT_STACK_SIZE 0x400
#endif

__stack_size__ = MBED_BOOT_STACK_SIZE;

STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0400 : 0x0;
M_CRASH_DATA_RAM_SIZE = 0x100;

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = MBED_APP_START, LENGTH = 0x400
  m_flash_config        (RX)  : ORIGIN = MBED_APP_START + 0x400, LENGTH = 0x10
  m_text                (RX)  : ORIGIN = MBED_APP_START + 0x410, LENGTH = MBED_APP_SIZE - 0x410
  m_data                (RW)  : ORIGIN = 0x1FFF0000, LENGTH = 0x00010000
  m_data_2              (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00030000
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(8);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(8);
  } > m_interrupts

  .flash_config :
  {
    . = ALIGN(8);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
    . = ALIGN(8);
  } > m_flash_config

  .text :
  {

    . = ALIGN(8);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(8);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  .interrupts_ram :
  {
    . = ALIGN(8);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(8);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data

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
  } > m_data

  .heap_0 :
  {
    . = ALIGN(8);
    __mbed_sbrk_start_0 = .;
    . += (ORIGIN(m_data) + LENGTH(m_data) - .);
    __mbed_krbs_start_0 = .;
  } > m_data

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data :
  {
    PROVIDE(__etext = LOADADDR(.data));    /* Define a global symbol at end of code, */
    PROVIDE(__DATA_ROM = LOADADDR(.data)); /* Symbol is used by startup for data initialization. */
    . = ALIGN(8);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(8);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data_2 AT > m_text

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")


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
  } > m_data_2

  USB_RAM_GAP = DEFINED(__usb_ram_size__) ? __usb_ram_size__ : 0x800;

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(8);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    . = ALIGN(512);
    USB_RAM_START = .;
    . += USB_RAM_GAP;
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data_2

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __mbed_sbrk_start = .;
    __HeapBase = .;
    . = ORIGIN(m_data_2) + LENGTH(m_data_2) - STACK_SIZE;
    __mbed_krbs_start = .;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data_2

  m_usb_bdt USB_RAM_START (NOLOAD) :
  {
    *(m_usb_bdt)
    USB_RAM_BDT_END = .;
  }

  m_usb_global USB_RAM_BDT_END (NOLOAD) :
  {
    *(m_usb_global)
  }

  /* Initializes stack on the end of block */
  __StackTop = ORIGIN(m_data_2) + LENGTH(m_data_2);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "Region m_data_2 overflowed with stack and heap")

}
