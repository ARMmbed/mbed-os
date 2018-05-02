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
 *   _estack
 */

/* Entry Point */
ENTRY(bl_start)

M_VECTOR_RAM_SIZE = 0x400;

STACK_SIZE = 0x400;
HEAP_SIZE = 0x6000;

/* Specify the memory areas */
MEMORY
{
	/* BOOTLOADER:
		To allow the application to be updated without affecting the bootloader,
		the bootloader has be resist in an own sector of the flash.
		It also has to be the first sector, so that the reset-vector is in the bootloader.

		On the STM32103 the Sector 1 is 2k in size.
		To enable Write-Protection of the bootloader, we have to use 2 regions, because
		"The write-protection granularity is two pages." (see PM0068: STM32F10xxx XL-density Flash programming p14)

		Each bank is 0x80000 bytes in length
	*/

	BL_VECTORS (rx) :		ORIGIN = 0x08000000, LENGTH = 0x0300
	BL_ABI_HDR (rx) :		ORIGIN = 0x08000200, LENGTH = 0x0100
	BL_TEXT (rx) :			ORIGIN = 0x08000300, LENGTH = 0x08002000 - 0x100 - 0x300
    VECTORS (rx) :			ORIGIN = 0x08002000, LENGTH = 0x0200
	ABI_HDR (rw):			ORIGIN = 0x08002200, LENGTH = 0x0100
    FLASH (rx)   :			ORIGIN = 0x08002400, LENGTH = 0x80000 - 0x2000 - 0x400 

	CONFIG (r) :			ORIGIN = 0x08080000, LENGTH = 0x2000 
	UPDATE (rw) :			ORIGIN = 0x08082000, LENGTH = 0x80000 - 0x2000
	// trail: 0x08100000
	RAM (rwx)    :			ORIGIN = 0x20000000, LENGTH = 0x18000
}


/* Define output sections */
SECTIONS
{

	.bl_vectors :
	{
		__bl_start = .;

		KEEP(*(.bl_vectors))
         . = ALIGN(4);
	} > BL_VECTORS

	.bl_abi_hdr :
	{	
		KEEP(*(.bl_abi_hdr))
         . = ALIGN(4);
	} > BL_ABI_HDR

	.bl_text :
	{
		*(.bl_text)
         . = ALIGN(4);
	} > BL_TEXT

	.isr_vector :
    {
		__image_start = .;

        __vector_table = .;
        KEEP(*(.isr_vector))
         . = ALIGN(4);

    } > VECTORS

    .abi_hdr :
    {
        KEEP(*(.abi_hdr))
         . = ALIGN(4);
    } > ABI_HDR

	.metadata :
	{
		__metadata_start = .;
		KEEP(*(.metadata))
		. = ALIGN(4);
	} > METADATA

    .text :
    {
        . = ALIGN(4);
        
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

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
    _sidata = .;

	.interrupts_ram :
    {
        . = ALIGN(4);
        __VECTOR_RAM__ = .;
        __interrupts_ram_start__ = .; /* Create a global symbol at data start */
        *(.m_interrupts_ram)     /* This is a user defined section */
        . += M_VECTOR_RAM_SIZE;
        . = ALIGN(4);
        __interrupts_ram_end__ = .; /* Define a global symbol at data end */
    } > RAM

    .data : 
    {
		PROVIDE( __etext = LOADADDR(.data) );

        __data_start__ = .;
        _sdata = .;
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
        _edata = .;

    } > RAM AT > FLASH

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        _ebss = .;
    } > RAM

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
    } > RAM
    
    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        . += HEAP_SIZE;
        __HeapLimit = .;
    } > RAM

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    _estack = __StackTop;
    __StackLimit = __StackTop - STACK_SIZE;
    PROVIDE(__stack = __StackTop);
    
    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
	
	__bank2_start = ORIGIN(CONFIG);
	__flashconfig_end = ORIGIN(CONFIG) + LENGTH(CONFIG);
	__update_image_start = ORIGIN(UPDATE);
	__application_max_size = LENGTH(UPDATE);

	__bootloader_length = 0x2000;

}
