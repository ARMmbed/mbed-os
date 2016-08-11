/* Entry Point */
ENTRY(Reset_Handler)

__ram_vector_table__ = 1;

/* Heap 1/4 of ram and stack 1/8 */
__stack_size__ = 0x80000;
__heap_size__ = 0x100000;

HEAP_SIZE = DEFINED(__heap_size__) ? __heap_size__ : 0x400;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x400;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x400 : 0x0;

/* Specify the memory areas */
MEMORY
{
  m_interrupts (RX) : ORIGIN = 0x00000000, LENGTH = 0x00000400
  m_text       (RX) : ORIGIN = 0x00000400, LENGTH = 0x00400000 - 0x00000400
  m_data       (RW) : ORIGIN = 0x20000000, LENGTH = 0x00400000
}

/* Define output sections */
SECTIONS
{
    /* The startup code goes first into internal flash */
    .interrupts :
    {
        __VECTOR_RAM__ = .;
        . = ALIGN(4);
        KEEP(*(.isr_vector))    /* Startup code */
         . = ALIGN(4);
    } > m_interrupts

    .text :
    {
        *(.text)                 /* .text sections (code) */
        *(.text*)                /* .text* sections (code) */
        *(.rodata)               /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
        *(.glue_7)               /* glue arm to thumb code */
        *(.glue_7t)              /* glue thumb to arm code */
        *(.eh_frame)
        KEEP (*(.init))
        KEEP (*(.fini))
        . = ALIGN(4);
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
        . = ALIGN(4);
        __VECTOR_RAM__ = .;
        __interrupts_ram_start__ = .; /* Create a global symbol at data start */
        *(.m_interrupts_ram)     /* This is a user defined section */
        . += M_VECTOR_RAM_SIZE;
        . = ALIGN(4);
        __interrupts_ram_end__ = .; /* Define a global symbol at data end */
    } > m_data

    __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
    __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

    .data :
    {
        PROVIDE(__etext = LOADADDR(.data));    /* Define a global symbol at end of code, */
        PROVIDE(__DATA_ROM = LOADADDR(.data)); /* Symbol is used by startup for data initialization. */
        . = ALIGN(4);
        __DATA_RAM = .;
        __data_start__ = .;      /* create a global symbol at data start */
        *(.data)                 /* .data sections */
        *(.data*)                /* .data* sections */
        KEEP(*(.jcr*))
        . = ALIGN(4);
        __data_end__ = .;        /* define a global symbol at data end */
    } > m_data AT > m_text

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
    } > m_data

    /* Uninitialized data section */
    .bss :
    {
        /* This is used by the startup in order to initialize the .bss section */
        . = ALIGN(4);
        __START_BSS = .;
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        __END_BSS = .;
    } > m_data

    .heap :
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += HEAP_SIZE;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
    } > m_data

    .stack :
    {
      . = ALIGN(8);
      . += STACK_SIZE;
      __StackTop = .;
    } > m_data

    /* Initializes stack on the end of block */
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    .ARM.attributes 0 : { *(.ARM.attributes) }

    ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}
