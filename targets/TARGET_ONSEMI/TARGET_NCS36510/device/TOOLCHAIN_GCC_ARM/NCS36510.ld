/*
 * NCS36510 ARM GCC linker script file
 */

MEMORY {
    VECTORS (rx)          : ORIGIN = 0x00003000, LENGTH = 0x00000090
    FLASHA (rx)           : ORIGIN = 0x00003090, LENGTH = 320K - 4K - 0x90
    FLASHB (rx)           : ORIGIN = 0x00102000, LENGTH = 320K
    RAM (rwx)             : ORIGIN = 0x3FFF4000, LENGTH = 48K
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
    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.vector_table))
        . = ALIGN(4);
    } > VECTORS

    mbed_rtos_start = .;
    .text_mbed_rtos : /* Place mbed-os and HAL code in FlashA */
    {
        
        *drivers/*.o(.text* .rodata*)
        *events/*.o(.text* .rodata*)
        *hal/*.o(.text* .rodata*)
        *platform/*.o(.text* .rodata*)
        *rtos/*.o(.text* .rodata*)
        *targets/*.o(.text* .rodata*)
        
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

        KEEP(*(.eh_frame*))

    } > FLASHA
    mbed_rtos_end = .;
    size_of_mbed_rtos = mbed_rtos_end - mbed_rtos_start;

.ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASHA

.ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASHA
    
.text : /* Place the remaining application and feature specific code in FlashB */
    {
        *(.text*)
        *(.rodata*)
    } > FLASHB

.data :
    {
        PROVIDE( __etext = LOADADDR(.data) );

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

        /* All data end */
        . = ALIGN(32);
        __data_end__ = .;

    } >RAM AT>FLASHB 

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
        . += 0x800;
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
        . += 0x800 - (. - __StackLimit);
    } > RAM

    /* Set stack top to end of RAM */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - SIZEOF(.stack);
    PROVIDE(__stack = __StackTop);

}
