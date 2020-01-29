/* stack: dynamic */
/* heap: dynamic */
#define MBED_RAM_START 0x10000000
#define MBED_RAM_SIZE   384K
#define MBED_BOOT_STACK_SIZE 0x400
#define MBED_RAM0_START MBED_RAM_START
#define MBED_RAM0_SIZE  0x100
#define MBED_RAM1_START (MBED_RAM0_START + MBED_RAM0_SIZE)
#define MBED_RAM1_SIZE  (MBED_RAM_SIZE - (MBED_RAM0_SIZE))

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00010000, LENGTH = 960K /*Modified from 0xC000 to work with SparkFun SVL*/
  RAM_NVIC (rwx) : ORIGIN = MBED_RAM0_START, LENGTH = MBED_RAM0_SIZE
  RAM (rwx)      : ORIGIN = MBED_RAM1_START, LENGTH = MBED_RAM1_SIZE
}

SECTIONS
{
  /* text: executable code */
  /* located in _flash_ */
  .text :
  {
    . = ALIGN(4);
    _stext = .;
    KEEP(*(.isr_vector))
    KEEP(*(.ble_patch))
    *(.text)
    *(.text*)

/*    __init_array_start = .;
    KEEP(*(.init_array*))
    __init_array_end = .; */ /* does this mess up _init()?' it was from Arduinoland (aka owen) */

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

    /* .rodata */
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)

    KEEP(*(.eh_frame*))

    . = ALIGN(4);
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

/* space for nvic table is reserved above by RAM_NVIC section. Do we need this?
Table is copied into this section at boot.
    .nvictable (NOLOAD) :
    {
      PROVIDE(__start_nvictable = .);
      KEEP(*(.nvictable))
      PROVIDE(__stop_nvictable = .);
    } > RAM_NVIC */

  /* data: uniquely initialized symbols */
  /* loaded into flash region, copied to sram region at startup */
  /* VMA appears in sram region, LMA is in flash region for initialization */
  /* _init_data used by startup to locate flash region copy of data */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    __data_start__ = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
    __data_end__ = .;
  } > RAM AT>FLASH
  _init_data = LOADADDR(.data); /* used by startup to initialize data */



  /* bss: zero-initialized symbols */
  /* don't require flash memory to remember their value */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = .;
  } > RAM

  /* heap: RAM memory that can be dynamically allocated in the upward direction (increasing memory addresses) */
  /* _sheap is used to identify the beginning of available dynamic memory */
  .heap (NOLOAD):
  {
    . = ALIGN(4);
    __end__ = .;
    PROVIDE( end = . );
    _sheap = .;
    . = ORIGIN(RAM) + LENGTH(RAM) - MBED_BOOT_STACK_SIZE-8;
    __HeapLimit = .;
  } >RAM

    /* .stack_dummy section doesn't contains any symbols. It is only
    * used for linker to calculate size of stack sections, and assign
    * values to stack symbols later*/
  .stack_dummy (NOLOAD):
  {
      . = ALIGN(8);
      *(.stack*)
  } > RAM


  /* Set stack top to end of RAM, and stack limit move down by
    * size of stack_dummy section*/
  __StackTop = ORIGIN(RAM) + LENGTH(RAM)-8;
  __StackLimit = __StackTop - MBED_BOOT_STACK_SIZE;
  PROVIDE(__stack = __StackTop);
  PROVIDE(_sstack = __StackTop);
  /* Check if data + heap + stack exceeds RAM limit*/
  /*ASSERT(1, "region RAM overflowed with stack")*/
  /* test test test */
}