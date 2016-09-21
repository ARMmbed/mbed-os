/*
 * ********************* ROM ALLOCATION ***********************
 *
 * Used solely by the Security Core ROM image built into the chip.
 */
/* 64K ROM, but 1 word is reserved for digits use */
/*
 * ********************* FLASH ALLOCATION *********************
 *
 * Flash is shared between the cores, and also handles a limited number of
 * non-volatile storage areas. Split is follows:
 *
 *      Config Data             1 page (4K)
 *      Non Volatile storage    8 pages (32K)
 *      Apps Code               64K  (0x10000)
 *      Protocol Code           860K (0xD7000)
 *      Security Code           64K  (0x10000)
 */
/* Reserve 9 * 4K for non-volatile data. */
/* 200K for Security core - matches eventual ROM */
/* 200K for Apps core - enough to allow testing */
/* Remainder for Protocol core. */
/* ROM based security code */
/* Security Configuration pages in FLASH reserved and used in ROM Code: The last flash page is used for security core configuration.*/
/*
 * ********************* RAM ALLOCATION *********************
 *
 * Two main RAM areas, the 'security core' RAM, and the 'shared' RAM.
 * As the name implies, the apps and protocol cores can only access the
 * 'shared' RAM, while the security core can access everything.
 *
 * The security core RAM is used by security code running from ROM and RAM,
 * so a section is reserved for ROM use to avoid memory corruption when
 * calling ROM based utility functions.
 *
 * SECURITY RAM             SECURITY IPC mailbox
 *                          ROM code RAM area
 *                          FLASH code RAM area
 *
 * The 'shared' RAM is used by the application and protocol cores, with a
 * small area visible to both to exchange larger amounts of data.
 *
 * SHARED RAM               SHARED
 *                          PROTOCOL code RAM
 *                          APPS code RAM
 */
 
/* 16K 'dedicated' for Security core. Outside of 192K total. */
/* Allocated a small amount of the 'security' ram for use by the code in ROM. */
/* Each core sees RAM starting at the same relative location (using region 1). */
/* 192K total 'shared' RAM. Split across APPS & PROTOCOL core + common 'shared' area. */
/* Allocate 16K for 'shared' use. */
/* 16K for APPS core. */
/* Allocate majority of RAM to PROTOCOL core. L3 is very hungry. */
/* Security core has it's own dedicated RAM */
/*
 * ********************* OTP REGIONS *********************
 * OTP Regions, Start at 0 and has a maximum length of OTP_AMOUNT.
 * By default assign the whole OTP to the security core and make the other
 * 2 cores OTP of length 0 and start at the last byte of the OTP.
 */
 
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)
/* FLASH sector size is 2K */
/* make this table consistent with the configuration given by the security core */
MEMORY
{
    VECTORS : ORIGIN = 0, LENGTH = 192
    FLASH_PROGRAM : ORIGIN = (0 + 192), LENGTH = (0x10000 - 192)
    /* AC_RAM_LENGTH is total RAM available, including area reserved at the top for IPC Mailboxes. */
    SRAM : ORIGIN = 0x01000000, LENGTH = 0x04000 - 256
    IPC_MAILBOX : ORIGIN = (0x01000000 + 0x04000 - 256), LENGTH = 256
}
SECTIONS
{
    /* Vectors */
    . = 0;
    startup :
    {
        KEEP (*(.isr_vector))
    } > VECTORS
    
    /* Code and const data */
    .text :
    {
        . = ALIGN(4);
        *(.text)
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

        *(.rodata)
        *(.rodata*)
        
        KEEP(*(.eh_frame*))
    } > FLASH_PROGRAM
    
    /* exidx and extab are debugging information to allow the unwinding of the
     * stack. See the "backtrace" module. exidx appears to be generated even if we
     * don't ask for full unwinding information (with the -funwind-tables option to
     * the compiler), so was present before backtracing was added. */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH_PROGRAM
    
    __etext = .;

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH_PROGRAM
    
    __RAM_START__ = ORIGIN(SRAM);
    __RAM_SIZE__ = LENGTH(SRAM);
    
    /* Space for vectors mapped to RAM */
    .ram_vectors (NOLOAD):
    {
        KEEP (*(.isr_vector))
    } > SRAM
    
    /* Initialised data */
    .data :
    {
        FILL(0xFF)
        __data_load__ = LOADADDR(.data);
        __data_start__ = .;
        *(.data)
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
        
        __data_end__ = .;
    } > SRAM AT > FLASH_PROGRAM
    
    __data_size__ = __data_end__ - __data_start__;
    
    /* Uninitialised data */
    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > SRAM
    
    __bss_size__ = __bss_end__ - __bss_start__;
    
    /* Like Uninitialised data, but we don't want to 0 it */
    .resume (NOLOAD):
    {
        . = ALIGN(4);
        *(preserve)
    } > SRAM

    /* Heap fills the rest of the space up to the start of the stack */
    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        __HeapBase = .;
        *(.heap*)
        . = ORIGIN(SRAM) + LENGTH(SRAM) - Stack_Size; /* Remember that LENGTH(SRAM) is already reduced by the IPC block */
        __HeapLimit = .;
    } > SRAM

    PROVIDE(__heap_start = ADDR(.heap));
    PROVIDE(__heap_size = SIZEOF(.heap));
    PROVIDE(__mbed_sbrk_start = ADDR(.heap));
    PROVIDE(__mbed_krbs_start = ADDR(.heap) + SIZEOF(.heap));
        
    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (NOLOAD):
    {
        __SYSTEM_STACK_START__ = .;
        *(.stack*)
        __SYSTEM_STACK_END__ = .;
    } > SRAM

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(SRAM) + LENGTH(SRAM); /* Remember that LENGTH(SRAM) is already reduced by the IPC block */
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds SRAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

    /* The IPC mailboxes are at the top of the SRAM */
    /* The size of this section (256 bytes) is already taken off the size of SRAM so there is no danger of the heap overflowing into it */
    .ipc_mailbox (NOLOAD):
    {
        . = ALIGN(4);
        __ipc_mailbox_start__ = .;
        *(.ipc_mailbox)
        *(.ipc_mailbox*)
        __ipc_mailbox_end__ = .;
    } > IPC_MAILBOX
}
