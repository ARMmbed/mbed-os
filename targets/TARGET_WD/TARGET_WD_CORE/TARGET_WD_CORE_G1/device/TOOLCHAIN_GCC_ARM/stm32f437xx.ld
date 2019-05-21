M_VECTOR_RAM_SIZE = 0x400;

/* With the RTOS in use, this does not affect the main stack size. The size of
 * the stack where main runs is determined via the RTOS. */
STACK_SIZE = 0x400;
HEAP_SIZE = 0x6000;

// https://sourceware.org/binutils/docs/ld/Scripts.html

/* Specify the memory areas */
MEMORY
{
	/* BOOTLOADER:
		To allow the application to be updated without affecting the bootloader,
		the bootloader has be resist in an own sector of the flash.
		It also has to be the first sector, so that the reset-vector is in the bootloader.

		On the STM32F437 the Sector 1 is 16k in size.
	*/

	BL_VECTORS (rx) :		ORIGIN = 0x08000000, LENGTH = 0x0200
	BL_ABI_HDR (rx) :		ORIGIN = 0x08000200, LENGTH = 0x0100
	BL_TEXT (rx) :			ORIGIN = 0x08000300, LENGTH = 0x3D00
    VECTORS (rx) :			ORIGIN = 0x08004000, LENGTH = 0x0200
	ABI_HDR (rw):			ORIGIN = 0x08004200, LENGTH = 0x0100
    FLASH (rx)   :			ORIGIN = 0x08004300, LENGTH = 0x100000 - 0x4000 - 0x300 /* = 0xFBC00 = 1031168 = 1007k) */

	CONFIG (r) :			ORIGIN = 0x08100000, LENGTH = 0x4000 
	UPDATE (rw) :			ORIGIN = 0x08104000, LENGTH = 0xFC000

    CCM (rwx)    :			ORIGIN = 0x10000000, LENGTH = 64K /* len: 0x10000 */
    RAM (rwx)    :			ORIGIN = 0x20000000, LENGTH = 192k /* len : 0x30000 */
}

ENTRY(bl_start)

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

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __stack = __StackTop;
    __StackLimit = __StackTop - STACK_SIZE;

    ASSERT(__StackLimit >= __HeapLimit, "Region RAM overflowed with stack and heap")
	
	__bank2_start = ORIGIN(CONFIG);
	__flashconfig_end = ORIGIN(CONFIG) + LENGTH(CONFIG);
	__update_image_start = ORIGIN(UPDATE);
	__application_max_size = LENGTH(UPDATE);

	__bootloader_length = 0x4000;

}
