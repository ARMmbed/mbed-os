/*
 * Portions Copyright (c) 2016 - 2018 Analog Devices, Inc.
 *
 * Based on Device/ARM/ARMCM4/Source/GCC/gcc_arm.ld file in
 * ARM.CMSIS.4.5.0.pack.
 */

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 0x7F000
#endif

#define ADUCM_SECTOR_SIZE 0x800

/* Linker script to configure memory regions. */
MEMORY
{
  /* The first 0x800 bytes of flash */
  FLASH0 (rx) : ORIGIN = MBED_APP_START, LENGTH = ADUCM_SECTOR_SIZE
  /* The rest of the flash */
  FLASH (rx) : ORIGIN = MBED_APP_START + ADUCM_SECTOR_SIZE, LENGTH = MBED_APP_SIZE - ADUCM_SECTOR_SIZE
  /* SRAM bank 0 */
  DSRAM_A (rwx) : ORIGIN = 0x20000200, LENGTH = 28k - 0x200
  /* SRAM bank 3+4+5+6+7 */
  DSRAM_B (rwx) : ORIGIN = 0x20048000, LENGTH = 64k
  /* stack must not be in bank 1,2,7 where ISRAM or CACHE
     are set at power on */
}

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)

/* Custom stack and heap sizes */
__stack_size__ = 0x2000;
__heap_size__  = 0x6000;

/* select custom or default sizes for stack and heap */
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0400;
HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0C00;


/* Linker script to place sections and symbol values.
 * It references the following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines the following symbols, which code can use without definition:
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
    .vectors :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;
        __end__ = .;
        KEEP(*(.checksum))
    } > FLASH0

    .text :
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

        KEEP(*(.jcr*))
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > DSRAM_A

    .bss :
    {
        . = ALIGN(16);
        __bss2_start__ = .;
        *(COMMON)
        . = ALIGN(16);
        __bss2_end__ = .;
        __bss_start__ = .;
        *(.bss*)
        . = ALIGN(16);
        __bss_end__ = .;
    } > DSRAM_B

    __StackTop = ORIGIN(DSRAM_B);
    __StackLimit = __StackTop  - STACK_SIZE;
    __HeapLimit  = __StackLimit;
    __HeapBase   = __HeapLimit - HEAP_SIZE;
    __end__      = __HeapBase;
    PROVIDE(end  = __end__);
    PROVIDE(__stack = __StackTop);
}
