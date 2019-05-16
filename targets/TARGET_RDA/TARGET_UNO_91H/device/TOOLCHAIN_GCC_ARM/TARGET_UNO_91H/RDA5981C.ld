/* Linker script for mbed RDA5981C */

#if !defined(MBED_BOOT_STACK_SIZE)
    #define MBED_BOOT_STACK_SIZE 0x400
#endif

STACK_SIZE = MBED_BOOT_STACK_SIZE;

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0x18001000
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 2000K
#endif

/* Linker script to configure memory regions. */
MEMORY
{
    /* If ICache is enable, use virtual flash base address */
    /* Use partition index: 0 */
    FLASH (rx)      : ORIGIN = MBED_APP_START, LENGTH = MBED_APP_SIZE
    /* Use partition index: 1 */
    /* FLASH (rx)      : ORIGIN = 0x181F5000, LENGTH = 2000K */

    /* If ICache is disable, use real flash base address. Depends on macro: RDA_ICACHE_DISABLE */
    /* Use partition index: 0 */
    /* FLASH (rx)      : ORIGIN = 0x14001000, LENGTH = 2000K */
    /* Use partition index: 1 */
    /* FLASH (rx)      : ORIGIN = 0x141F5000, LENGTH = 2000K */

    IRAM (rwx)      : ORIGIN = 0x00100080, LENGTH = (128K - 0x80)
    DRAM (rwx)      : ORIGIN = 0x00180000, LENGTH = 160K
    MACLIB_RAM(rwx) : ORIGIN = 0x40100000, LENGTH = 99K
    AES_RAM(rwx)    : ORIGIN = 0x40118C00, LENGTH = 1K
    WLAN_RAM(rwx)   : ORIGIN = 0x40119000, LENGTH = 28K
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
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
        KEEP(*(.isr_vector))
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

    __etext = .;

    .data : AT (__etext)
    {
        __data_start__ = .;
        Image$$RW_IRAM1$$Base = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);

        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > IRAM


    .bss :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
        Image$$RW_IRAM1$$ZI$$Limit = . ;
    } > IRAM

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy :
    {
        *(.stack)
    } > IRAM

    /* Set stack top to end of IRAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(IRAM) + LENGTH(IRAM);
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Check if data + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __bss_end__, "region IRAM overflowed with stack")


    .heap :
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
        . = ORIGIN(DRAM) + LENGTH(DRAM);
        __HeapLimit = .;
    } > DRAM
    PROVIDE(__sbrk_start = ADDR(.heap));
    PROVIDE(__krbs_start = ADDR(.heap) + SIZEOF(.heap));

    /* Code can explicitly ask for data to be
       placed in these higher RAM banks where
       they will be left uninitialized.
    */
    .SECTIONRESERVED1 (NOLOAD):
    {
        Image$$RW_IRAM2$$Base = . ;
        *(SECTIONRESERVED1)
        Image$$RW_IRAM2$$ZI$$Limit = .;
    } > MACLIB_RAM

    .AHB1SMEM0 (NOLOAD):
    {
        Image$$RW_IRAM3$$Base = . ;
        *(AHB1SMEM0)
        Image$$RW_IRAM3$$ZI$$Limit = .;
    } > AES_RAM

    .AHB1SMEM1 (NOLOAD):
    {
        Image$$RW_IRAM4$$Base = . ;
        *(AHB1SMEM1)
        Image$$RW_IRAM4$$ZI$$Limit = .;
    } > WLAN_RAM
}
