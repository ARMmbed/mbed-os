/*
** ###################################################################
**     Processors:          MIMXRT1052CVJ5B
**                          MIMXRT1052CVL5B
**                          MIMXRT1052DVJ6B
**                          MIMXRT1052DVL6B
**
**     Compiler:            GNU C Compiler
**     Reference manual:    IMXRT1050RM Rev.1, 03/2018
**     Version:             rev. 1.0, 2018-09-21
**     Build:               b180921
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2018 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

__ram_vector_table__ = 1;

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0x60000000
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 0x400000
#endif

#if !defined(MBED_CONF_TARGET_BOOT_STACK_SIZE)
    #define MBED_CONF_TARGET_BOOT_STACK_SIZE 0x400
#endif

__stack_size__ = MBED_CONF_TARGET_BOOT_STACK_SIZE;

STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0400 : 0x0;

/* Specify the memory areas */
MEMORY
{
#if !defined(MBED_APP_COMPILE)
  m_flash_config        (RX)  : ORIGIN = MBED_APP_START, LENGTH = 0x00001000
  m_ivt                 (RX)  : ORIGIN = MBED_APP_START + 0x1000, LENGTH = 0x00001000
  m_interrupts          (RX)  : ORIGIN = MBED_APP_START + 0x2000, LENGTH = 0x00000400
  m_text                (RX)  : ORIGIN = MBED_APP_START + 0x2400, LENGTH = MBED_APP_SIZE - 0x2400
#else
  m_interrupts          (RX)  : ORIGIN = MBED_APP_START, LENGTH = 0x00000400
  m_text                (RX)  : ORIGIN = MBED_APP_START + 0x400, LENGTH = MBED_APP_SIZE - 0x400
#endif

#if MIMXRT105X_BOARD_HAS_EXTERNAL_RAM
  /* Use the external RAM as main memory */
  m_data                (RW)  : ORIGIN = 0x80000000, LENGTH = MIMXRT105X_EXTERNAL_RAM_SIZE

  /* DTCM memory.
     Startup code configures size to 256k (stealing space from OCRAM). */
  m_dtcm                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00040000
#else
  /* Use DTCM as main memory (significantly faster than OCRAM).
     Startup code configures size to 256k (stealing space from OCRAM). */
  m_dtcm                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00040000

  /* No external data memory, store data in DTCM */
#define m_data m_dtcm

#endif

  /* ITCM bank -- used for functions that need to execute from RAM
     (which is faster than having to load them from flash).
     Startup code configures size to 128k. */
  m_itcm               (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00020000

  /* OCRAM bank -- extra RAM, available for misc storage but slower to access.
     Startup code configures size to 128k.
     Note that address is different on the 105x and the 106x. */
#if MBED_TARGET_MIMXRT1050
  m_ocram              (RW)  : ORIGIN = 0x20200000, LENGTH = 0x00020000
#else /* MIMXRT1060 */
  m_ocram              (RW)  : ORIGIN = 0x20280000, LENGTH = 0x00020000
#endif

#if MBED_TARGET_MIMXRT1060
  /* OCRAM2 bank -- extra RAM, available on MIMXRT106x only. */
  m_ocram2             (RW)  : ORIGIN = 0x20200000, LENGTH = 0x00080000
#endif
}

/* Define output sections */
SECTIONS
{
#if !defined(MBED_APP_COMPILE)
  .flash_config :
  {
    . = ALIGN(8);
    __FLASH_BASE = .;
    KEEP(* (.boot_hdr.conf))     /* flash config section */
    . = ALIGN(8);
  } > m_flash_config

  ivt_begin = ORIGIN(m_flash_config) + LENGTH(m_flash_config);

  .ivt :
  {
    . = ALIGN(8);
    KEEP(* (.boot_hdr.ivt))           /* ivt section */
    KEEP(* (.boot_hdr.boot_data))     /* boot section */
    KEEP(* (.boot_hdr.dcd_data))      /* dcd section */
    . = ALIGN(8);
  } > m_ivt
#endif
  /* The startup code goes first into internal RAM */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(8);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(8);
  } > m_interrupts

  /* The program code and other data goes into internal RAM */
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

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .interrupts_ram :
  {
    . = ALIGN(8);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(8);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_dtcm

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data :
  {
    . = ALIGN(8);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(m_usb_dma_init_data)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(8);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data AT> m_text
  __ram_function_flash_start = __DATA_ROM + (__data_end__ - __data_start__); /* Symbol is used by startup for TCM data initialization */

  .ram_function :
  {
    . = ALIGN(32);
    __ram_function_start__ = .;
    *(CodeQuickAccess)
    . = ALIGN(128);
    __ram_function_end__ = .;
  } > m_itcm AT> m_text

  __ram_function_size = SIZEOF(.ram_function);

  __NDATA_ROM = __ram_function_flash_start + SIZEOF(.ram_function);

  /* Always store noncacheable data (e.g. DMA descriptors) in DTCM, since this memory
     does not use a cache. */
  .ncache.init :
  {
    __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
    *(NonCacheable.init)
    . = ALIGN(8);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
  } > m_dtcm AT> m_text
  . = __noncachedata_init_end__;
  .ncache :
  {
    *(NonCacheable)
    . = ALIGN(8);
    __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
  } > m_dtcm

  __TEXT_CSF_ROM = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);

  .csf :
  {
    FILL(0xFF)
    . = ALIGN(1024);
    KEEP(*(.csf))
    __text_csf_end = .;
  } > m_text


  /* __USED_FLASH_END gives the end of all data written to the flash memory, including code and data
      initializers. */
  PROVIDE(__USED_FLASH_END = __TEXT_CSF_ROM + SIZEOF(.csf));

  /* __USED_FLASH_SIZE gives the size of all data written to the flash memory, including code and data
      initializers. */
  PROVIDE(__USED_FLASH_SIZE = __USED_FLASH_END - ORIGIN(m_text));

  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__USED_FLASH_END <= text_end, "Code and data initializers exceed flash size!")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(8);
    __START_BSS = .;
    __bss_start__ = .;
    *(m_usb_dma_noninit_data)
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . =  ORIGIN(m_data) + LENGTH(m_data) - STACK_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data

  /* Reserve space for stack (even though stack is always at the end of DTCM regardless
     of where this section is located) */
  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_dtcm

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_dtcm) + LENGTH(m_dtcm);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /* Teensy model identifier symbol (used by Teensy Loader) */
  /* See here https://forum.pjrc.com/threads/72107-Testers-Needed-Mbed-OS-Port-for-Teensy-4-0?p=320414&viewfull=1#post320414 */
#if defined(TARGET_TEENSY_40)
	_teensy_model_identifier = 0x24;
#elif defined(TARGET_TEENSY_41)
	_teensy_model_identifier = 0x25;
#endif
}

