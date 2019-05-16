/*
 * NCS36510 ARM GCC linker script file
 */

#if !defined(MBED_BOOT_STACK_SIZE)
    #define MBED_BOOT_STACK_SIZE 0x400
#endif

STACK_SIZE = MBED_BOOT_STACK_SIZE;

MEMORY {
    FIB (rx)              : ORIGIN = 0x00002000, LENGTH = 0x00000800
    TRIM (rx)             : ORIGIN = 0x00002800, LENGTH = 0x00000800
    VECTORS (rx)          : ORIGIN = 0x00003000, LENGTH = 0x00000090
    FLASH (rx)            : ORIGIN = 0x00003090, LENGTH = 320K - 4K - 0x90
    RAM (rwx)             : ORIGIN = 0x3FFF4090, LENGTH = 48K - 0x90 /* 8_byte_aligned(35 vectors * 4 bytes each) = 0x90 */
}

    /* Linker script to place sections and symbol values. Should be used together
     * with other linker script that defines memory regions FLASH and RAM.
     * It references following symbols, which must be defined in code:
     * _reset_init : Entry of reset handler
     *
     * It defines following symbols, which code can use without definition:
     * __exidx_start
     * __exidx_end
     * __etext
     * __data_start__
     * __preinit_array_start
     * __preinit_array_end
     * __init_array_start
     * __init_array_end
     * __fini_array_start
     * __fini_array_end
     * __data_end__
     * __bss_start__
     * __bss_end__
     * __end__
     * end
     * __HeapLimit
     * __StackLimit
     * __StackTop
     * __stack
     */
    ENTRY(Reset_Handler)

    SECTIONS {
    .fib :
    {
        KEEP(*(.fib))
    } > FIB

    .trim :
    {
        KEEP(*(.trim))
    } > TRIM

    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.vector_table))
        . = ALIGN(8);
    } > VECTORS


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

.ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

.data :
    {
        PROVIDE( __etext = LOADADDR(.data) );

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

        /* All data end */
        . = ALIGN(32);
        __data_end__ = .;

    } >RAM AT>FLASH


    .uninitialized (NOLOAD):
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > RAM

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM

    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        *(.heap*);
        . = ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE;
        __HeapLimit = .;
    } > RAM
    PROVIDE(__heap_size = SIZEOF(.heap));
    PROVIDE(__mbed_sbrk_start = ADDR(.heap));
    PROVIDE(__mbed_krbs_start = ADDR(.heap) + SIZEOF(.heap));

    /* .stack section doesn't contains any symbols. It is only
     * used for linker to reserve space for the main stack section
     * WARNING: .stack should come immediately after the last secure memory
     * section.  This provides stack overflow detection. */
    .stack (NOLOAD):
    {
        __StackLimit = .;
        *(.stack*);
        . += STACK_SIZE - (. - __StackLimit);
    } > RAM

    /* Set stack top to end of RAM */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);

}
