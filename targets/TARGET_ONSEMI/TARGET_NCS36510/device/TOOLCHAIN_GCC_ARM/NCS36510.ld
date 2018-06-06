/*
 * NCS36510 ARM GCC linker script file
 */

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
        . = ALIGN(4);
    } > VECTORS

    /* ensure that uvisor bss is at the beginning of memory */
    .uvisor.bss (NOLOAD):
    {
        . = ALIGN(32);
        __uvisor_bss_start = .;

        /* protected uvisor main bss */
        . = ALIGN(32);
        __uvisor_bss_main_start = .;
        KEEP(*(.keep.uvisor.bss.main))
        . = ALIGN(32);
        __uvisor_bss_main_end = .;

        /* protected uvisor secure boxes bss */
        . = ALIGN(32);
        __uvisor_bss_boxes_start = .;
        KEEP(*(.keep.uvisor.bss.boxes))
        . = ALIGN(32);
        __uvisor_bss_boxes_end = .;

        . = ALIGN(32);
        __uvisor_bss_end = .;
    } > RAM

.text :
    {
        /* uVisor code and data */
        . = ALIGN(4);
        __uvisor_main_start = .;
        *(.uvisor.main)
        __uvisor_main_end = .;

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

    } >RAM AT>FLASH

    /* uvisor configuration data */
.uvisor.secure :
    {
        . = ALIGN(32);
        __uvisor_secure_start = .;

        /* uvisor secure boxes configuration tables */
        . = ALIGN(32);
        __uvisor_cfgtbl_start = .;
        KEEP(*(.keep.uvisor.cfgtbl))
        . = ALIGN(32);
        __uvisor_cfgtbl_end = .;

        /* pointers to uvisor secure boxes configuration tables */
        /* note: no further alignment here, we need to have the exact list of pointers */
        __uvisor_cfgtbl_ptr_start = .;
        KEEP(*(.keep.uvisor.cfgtbl_ptr_first))
        KEEP(*(.keep.uvisor.cfgtbl_ptr))
        __uvisor_cfgtbl_ptr_end = .;

        /* the following symbols are kept for backward compatibility and will be soon
         * deprecated; applications actively using uVisor (__uvisor_mode == UVISOR_ENABLED)
         * will need to use uVisor 0.8.x or above, or the security assetions will halt the
         * system */
        /************************/
        __uvisor_data_src = .;
        __uvisor_data_start = .;
        __uvisor_data_end = .;
        /************************/

        . = ALIGN(32);
        __uvisor_secure_end = .;
    } >FLASH

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
