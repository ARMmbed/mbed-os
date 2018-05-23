/** @file XMC4500x1024.ld
 *  @brief xmc4500 linker script for the mbed platform
 *
 *  @author Franz Ney (es16m013@technikum-wien.at)
 *  @bug No known bugs.
 */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
    FLASH_1_cached(rx) : ORIGIN = 0x08000000, LENGTH = 0x100000
    FLASH_1_uncached(rx) : ORIGIN = 0x0C000000, LENGTH = 0x100000
    PSRAM_1(xrw) : ORIGIN = 0x10000000, LENGTH = 0x10000
    DSRAM_1_system(xrw) : ORIGIN = 0x20000000 + 512, LENGTH = 0x10000 - 512
    DSRAM_2_comm(xrw) : ORIGIN = 0x30000000, LENGTH = 0x8000
}

/*
 * 3 types of on chip RAM found in XMC4000 are,
 *
 * Code RAM (PSRAM)
 * The Code RAM is intended for user code or Operating System data storage.
 * The memory is accessed via the Bus Matrix and provides zero-wait-state access for the CPU for code execution or data access.
 *
 * System RAM (DSRAM1)
 * The System RAM is intended for general user data storage.
 * The System RAM is accessed via the Bus Matrix and provides zero-wait-state access for data.
 *
 * Communication RAM (DSRAM2)
 * The Communication RAM is intended for use by communication interface units like the USB and Ethernet modules.
 */

/* The stack size used by the application. NOTE: you need to adjust according to your application. */
no_init_size = 64;
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x4000;


SECTIONS
{
    .text :
    {
        _sfixed = .;
        __Vectors = .;
        KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        
        /* Support C constructors, and C destructors in both user code
        and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
        
        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;
        
        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))
        
        . = ALIGN(4);
        KEEP(*(.fini))
        
        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;
        
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))
        
        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > FLASH_1_cached AT > FLASH_1_uncached
  
    /* Exception handling, exidx needs a dedicated section */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH_1_cached AT > FLASH_1_uncached

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . = ALIGN(4);
    } > FLASH_1_cached AT > FLASH_1_uncached
    __exidx_end = .;
    
    . = ALIGN(4);
    _etext = .;
    
    . = ALIGN(16);

    .data : AT (_etext)
    {
        . = ALIGN(4);
        __data_start__ = .;
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
        /* All data end */
        __data_end__ = .;
    } > PSRAM_1
    
    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        * (.bss);
        * (.bss*);
        * (COMMON);
        *(.gnu.linkonce.b*)
        . = ALIGN(4);
        __bss_end__ = .;
        . = ALIGN(8);
    } > PSRAM_1
    
    .heap :
    {
        . = ALIGN(4);
        __end__ = .;
        end = __end__;
        . = ORIGIN(PSRAM_1) + LENGTH(PSRAM_1) - STACK_SIZE;
        __HeapLimit = .;
    } > PSRAM_1
    
     /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy :
    {
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
    } > PSRAM_1
    
    /* .no_init section contains chipid, SystemCoreClock and trimming data. See system.c file*/
    .no_init ORIGIN(DSRAM_1_system) + LENGTH(DSRAM_1_system) - no_init_size (NOLOAD) : 
    {
        * (.no_init);
    } > DSRAM_1_system
    
    DSRAM2_BSS (NOLOAD) :
    {
        . = ALIGN(4);
        __bss2_start = .;
        *(ETH_RAM)
        *(USB_RAM)
        *(DSRAM2_BSS)   
        . = ALIGN(4);
        __bss2_end = .;
        . = ALIGN(8);
    } > DSRAM_2_comm
    
    . = ALIGN(4);
    
    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(PSRAM_1) + LENGTH(PSRAM_1);
    _estack = __StackTop;
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);
    
    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
