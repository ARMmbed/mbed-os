/*
 * Portions Copyright (c) 2016 - 2018 Analog Devices, Inc.
 *
 * Based on Device/ARM/ARMCM3/Source/GCC/gcc_arm.ld file in
 * ARM.CMSIS.4.5.0.pack.
 */

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 0x40000
#endif

#define ADUCM_SECTOR_SIZE 0x800

/* Linker script to configure memory regions. */
MEMORY
{
  /* The first 0x800 bytes of flash */
  FLASH0 (rx) : ORIGIN = MBED_APP_START, LENGTH = ADUCM_SECTOR_SIZE
  /* The rest of the flash */
  FLASH (rx) : ORIGIN = MBED_APP_START + ADUCM_SECTOR_SIZE, LENGTH = MBED_APP_SIZE - ADUCM_SECTOR_SIZE
  /* SRAM bank 0+1 */
  DSRAM_V (rwx) : ORIGIN = 0x20000000, LENGTH = 0x200
  DSRAM_A (rwx) : ORIGIN = 0x20000200, LENGTH = 16k - 0x200
  DSRAM_C (rwx) : ORIGIN = 0x20004000, LENGTH = 16k
  /* SRAM bank 3 */
  DSRAM_B (rwx) : ORIGIN = 0x20040000, LENGTH = 28k
}

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)
/* Custom stack and heap sizes */
#if !defined(MBED_BOOT_STACK_SIZE)
    #define MBED_BOOT_STACK_SIZE 0x400
#endif
__stack_size__ = MBED_BOOT_STACK_SIZE;
__heap_size__ = 0x2000;

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


    __etext = .;

    .data : AT (__etext)
    {
        __data_start__ = .;
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

    } > DSRAM_A

    .bss :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        KEEP(*(.bss.gChannelControlDataArray))
        KEEP(*(.bss.thread_stack_main))
        KEEP(*(.bss.UartDeviceMem))
        KEEP(*(.bss.os_thread_def_stack_event_loop_thread))
        *(COMMON)
        *(.bss)
        . = ALIGN(8);
        __bss_end__ = .;
    } > DSRAM_C

    .bss2 :
    {
        . = ALIGN(8);
        __bss2_start__ = .;
        *(.bss*)
        . = ALIGN(8);
        __bss2_end__ = .;
    } > DSRAM_B

    .heap (COPY):
    {
        __HeapBase = .;
        __end__ = .;
        end = __end__;
        . = ORIGIN(DSRAM_A) + LENGTH(DSRAM_A) - STACK_SIZE;
        __HeapLimit = .;
    } > DSRAM_A

    /* Set stack top to end of DSRAM_A, and move stack limit down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(DSRAM_A) + LENGTH(DSRAM_A);
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds DSRAM_A limit when they are both in DSRAM_A
    ASSERT(__StackLimit >= __HeapLimit, "region DSRAM_A overflowed with stack") */
}
