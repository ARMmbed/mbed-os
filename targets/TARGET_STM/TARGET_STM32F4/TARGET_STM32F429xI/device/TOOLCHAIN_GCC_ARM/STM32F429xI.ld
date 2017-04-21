M_VECTOR_RAM_SIZE = 0x400;

/* With the RTOS in use, this does not affect the main stack size. The size of
 * the stack where main runs is determined via the RTOS. */
STACK_SIZE = 0x400;

/* This is the guaranteed minimum available heap size for an application. When
 * uVisor is enabled, this is also the maximum available heap size. The
 * HEAP_SIZE value is set by uVisor porters to balance the size of the legacy
 * heap and the page heap in uVisor applications. */
HEAP_SIZE = 0x6000;

#if !defined(MBED_APP_START)
  #define MBED_APP_START 0x08000000
#endif

#if !defined(MBED_APP_SIZE)
  #define MBED_APP_SIZE 2048k
#endif

/* Specify the memory areas */
MEMORY
{
    VECTORS (rx) : ORIGIN = MBED_APP_START, LENGTH = 0x400
    FLASH (rx)   : ORIGIN = MBED_APP_START + 0x400, LENGTH = MBED_APP_SIZE - 0x400
    CCM (rwx)    : ORIGIN = 0x10000000, LENGTH = 64K
    RAM (rwx)    : ORIGIN = 0x20000000, LENGTH = 192k
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
 *   _estack
 */
ENTRY(Reset_Handler)

SECTIONS
{
    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.isr_vector))
         . = ALIGN(4);
    } > VECTORS

    /* Note: The uVisor expects this section at a fixed location, as specified
     *              by the porting process configuration parameter:
     *              FLASH_OFFSET. */
    __UVISOR_FLASH_OFFSET = 0x400;
    __UVISOR_FLASH_START = ORIGIN(VECTORS) + __UVISOR_FLASH_OFFSET;
    .text __UVISOR_FLASH_START :
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

    /* uVisor own memory and private box memories
    /* Note: The uVisor expects this section at a fixed location, as specified
             by the porting process configuration parameter: SRAM_OFFSET. */
    __UVISOR_SRAM_OFFSET = 0x0;
    __UVISOR_SRAM_START = ORIGIN(CCM) + __UVISOR_SRAM_OFFSET;
    .uvisor.bss __UVISOR_SRAM_START (NOLOAD):
    {
        . = ALIGN(32);
        __uvisor_bss_start = .;

        /* Protected uVisor own BSS section */
        . = ALIGN(32);
        __uvisor_bss_main_start = .;
        KEEP(*(.keep.uvisor.bss.main))
        . = ALIGN(32);
        __uvisor_bss_main_end = .;

        /* Protected uVisor boxes' static memories */
        . = ALIGN(32);
        __uvisor_bss_boxes_start = .;
        KEEP(*(.keep.uvisor.bss.boxes))
        . = ALIGN(32);
        __uvisor_bss_boxes_end = .;

        . = ALIGN(32);
        __uvisor_bss_end = .;
    } > CCM

    /* Heap space for the page allocator
    /* If uVisor shares the SRAM with the OS/app, ensure that this section is
     * the first one after the uVisor BSS section. Otherwise, ensure it is the
     * first one after the VTOR relocation section. */
    .page_heap (NOLOAD) :
    {
        . = ALIGN(32);
        __uvisor_page_start = .;
        KEEP(*(.keep.uvisor.page_heap))
        . = ALIGN((1 << LOG2CEIL(LENGTH(RAM))) / 8);
        __uvisor_page_end = .;
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

    /* uVisor configuration section
     * This section must be located after all other flash regions. */
    .uvisor.secure :
    {
        . = ALIGN(32);
        __uvisor_secure_start = .;

        /* uVisor secure boxes configuration tables */
        . = ALIGN(32);
        __uvisor_cfgtbl_start = .;
        KEEP(*(.keep.uvisor.cfgtbl))
        . = ALIGN(32);
        __uvisor_cfgtbl_end = .;

        /* Pointers to the uVisor secure boxes configuration tables */
        /* Note: Do not add any further alignment here, as uVisor will need to
         * have access to the exact list of pointers. */
        __uvisor_cfgtbl_ptr_start = .;
        KEEP(*(.keep.uvisor.cfgtbl_ptr_first))
        KEEP(*(.keep.uvisor.cfgtbl_ptr))
        __uvisor_cfgtbl_ptr_end = .;

        /* Pointers to all boxes register gateways. These are grouped here to
           allow discoverability and firmware verification. */
        __uvisor_register_gateway_ptr_start = .;
        KEEP(*(.keep.uvisor.register_gateway_ptr))
        __uvisor_register_gateway_ptr_end = .;

        . = ALIGN(32);
        __uvisor_secure_end = .;
    } > FLASH

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

    .heap (NOLOAD):
    {
        __uvisor_heap_start = .;
        __end__ = .;
        end = __end__;
        . += HEAP_SIZE;
        __HeapLimit = .;
        __uvisor_heap_end = .;
    } > RAM

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __stack = __StackTop;
    __StackLimit = __StackTop - STACK_SIZE;

    ASSERT(__StackLimit >= __HeapLimit, "Region RAM overflowed with stack and heap")

    /* Provide physical memory boundaries for uVisor. */
    __uvisor_flash_start = ORIGIN(VECTORS);
    __uvisor_flash_end = ORIGIN(FLASH) + LENGTH(FLASH);
    __uvisor_sram_start = ORIGIN(CCM);
    __uvisor_sram_end = ORIGIN(CCM) + LENGTH(CCM);
    __uvisor_public_sram_start = ORIGIN(RAM);
    __uvisor_public_sram_end = ORIGIN(RAM) + LENGTH(RAM);
}
