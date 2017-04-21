/* mbed Microcontroller Library */
/* Copyright (c) 2016 u-blox. */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)
MEMORY
{
    FLASH_VECTORS : ORIGIN = 0, LENGTH = 192
    FLASH : ORIGIN = (0 + 192), LENGTH = (0x20000 - 192)
    RAM_VECTORS : ORIGIN = 0, LENGTH = 128 /* Reserve space for dynamic vectors mapped to RAM (see cmsis_nvic.c) */
    RAM : ORIGIN = 0x01000000 + 128, LENGTH = 0x5000 - 256
    IPC_MAILBOX : ORIGIN = (0x01000000 + 128 + 0x5000 - 256), LENGTH = 256
}
SECTIONS
{
    /* Vector table in FLASH */
    . = 0;
    startup :
    {
        KEEP (*(.isr_vector))
    } > FLASH_VECTORS
    
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
    } > FLASH
    
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH
    
    __etext = .;

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
    
    __RAM_START__ = ORIGIN(RAM);
    __RAM_SIZE__ = LENGTH(RAM);
    
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
    } > RAM AT > FLASH
    
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
    } > RAM
    
    __bss_size__ = __bss_end__ - __bss_start__;
    
    .resume (NOLOAD):
    {
        . = ALIGN(4);
        *(preserve)
    } > RAM

    /* Heap fills the rest of the space up to the start of the stack */
    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        __HeapBase = .;
        *(.heap*)
        . = ORIGIN(RAM) + LENGTH(RAM) - Stack_Size; /* Remember that LENGTH(RAM) is already reduced by the IPC block */
        __HeapLimit = .;
    } > RAM

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
    } > RAM

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM); /* Remember that LENGTH(RAM) is already reduced by the IPC block */
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

    /* IPC mailboxes at the top of RAM */
    /* The size of this section (256 bytes) is already taken off the size of RAM so there is no danger of the heap overflowing into it */
    .ipc_mailbox (NOLOAD):
    {
        . = ALIGN(4);
        __ipc_mailbox_start__ = .;
        *(.ipc_mailbox)
        *(.ipc_mailbox*)
        __ipc_mailbox_end__ = .;
    } > IPC_MAILBOX
}