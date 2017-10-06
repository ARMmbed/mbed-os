/**
 *******************************************************************************
 * @file    startup_TMPM066.S
 * @brief   CMSIS Cortex-M0 Core Device Startup File for the
 *          TOSHIBA 'TMPM066' Device Series
 * @version V2.0.2.1 (Tentative)
 * @date    2015/08/05
 *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

.syntax unified
.arch armv6-m

.section .stack
.align 3

/*
// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

.section .stack
.align 3
#ifdef __STACK_SIZE
.equ    Stack_Size, __STACK_SIZE
#else
.equ    Stack_Size, 0x400
#endif
.globl    __StackTop
.globl    __StackLimit
__StackLimit:
.space    Stack_Size
.size __StackLimit, . - __StackLimit
__StackTop:
.size __StackTop, . - __StackTop

/*
// <h> Heap Configuration
//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

.section .heap
.align 3
#ifdef __HEAP_SIZE
.equ    Heap_Size, __HEAP_SIZE
#else
.equ    Heap_Size, 0
#endif
.globl    __HeapBase
.globl    __HeapLimit
__HeapBase:
.if    Heap_Size
.space    Heap_Size
.endif
.size __HeapBase, . - __HeapBase
__HeapLimit:
.size __HeapLimit, . - __HeapLimit

/* Vector Table */

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                  /* Top of Stack                  */
    .long   Reset_Handler               /* Reset Handler                 */
    .long   NMI_Handler                 /* NMI Handler                   */
    .long   HardFault_Handler           /* Hard Fault Handler            */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   SVC_Handler                 /* SVCall Handler                */
    .long   0                           /* Debug Monitor Handler         */
    .long   0                           /* Reserved                      */
    .long   PendSV_Handler              /* PendSV Handler                */
    .long   SysTick_Handler             /* SysTick Handler               */

    /* External Interrupts */
    .long     INT0_IRQHandler           // 0:	Interrupt Pin0
    .long     INT1_IRQHandler           // 1:	Interrupt Pin1
    .long     INT2_IRQHandler           // 2:	Interrupt Pin2
    .long     INT3_IRQHandler           // 3:	Interrupt Pin3
    .long     INT4_IRQHandler           // 4:	Interrupt Pin4
    .long     INT5_IRQHandler           // 5:	Interrupt Pin5
    .long     INTRX0_IRQHandler         // 6:	Serial reception interrupt(channel0)
    .long     INTTX0_IRQHandler         // 7:	Serial transmission interrupt(channel0)
    .long     INTRX1_IRQHandler         // 8:	Serial reception interrupt(channel1)
    .long     INTTX1_IRQHandler         // 9:	Serial transmission interrupt(channel1)
    .long     INTSPIRX_IRQHandler       // 10:	SPI serial reception interrupt
    .long     INTSPITX_IRQHandler       // 11:	SPI serial transmission interrupt
    .long     INTSPIERR_IRQHandler      // 12:	SPI serial error interrupt
    .long     INTI2C0_IRQHandler        // 13:	Serial bus interface (channel.0)
    .long     INTI2C1_IRQHandler        // 14:	Serial bus interface (channel.1)
    .long     INTDMA_IRQHandler         // 15:	DMAC interrupt
    .long     INT16A0_IRQHandler        // 16:	16-bit TMR16A match detection (channel.0)
    .long     INT16A1_IRQHandler        // 17:	16-bit TMR16A match detection (channel.1)
    .long     INTTB0_IRQHandler         // 18:	16-bit TMRB interrupt(channel.0)
    .long     INTTB1_IRQHandler         // 19:	16-bit TMRB interrupt(channel.1)
    .long     INTTB2_IRQHandler         // 20:	16-bit TMRB interrupt(channel.2)
    .long     INTTB3_IRQHandler         // 21:	16-bit TMRB interrupt(channel.3)
    .long     INTTB4_IRQHandler         // 22:	16-bit TMRB interrupt(channel.4)
    .long     INTTB5_IRQHandler         // 23:	16-bit TMRB interrupt(channel.5)
    .long     INTTB6_IRQHandler         // 24:	16-bit TMRB interrupt(channel.6)
    .long     INTTB7_IRQHandler         // 25:	16-bit TMRB interrupt(channel.7)
    .long     INTI2CS_IRQHandler        // 26:	Serial bus interface for Wakeup(channel.1)
    .long     INTTMRD_IRQHandler        // 27:	TMRD interrupt
    .long     INTUSB_IRQHandler         // 28:	USB interrupt
    .long     INTUSBWKUP_IRQHandler     // 29:	USB wakeup interrupt
    .long     INTADHP_IRQHandler        // 30:	High Priority A/D conversion interrupt
    .long     INTAD_IRQHandler          // 31:	Normal A/D conversion completion interrupt
    .size    __isr_vector, . - __isr_vector

/* Reset Handler */
    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
    Reset_Handler:
    /*     Loop to copy data from read only memory to RAM. The ranges
     *      of copy from/to are specified by following symbols evaluated in
     *      linker script.
     *      __etext: End of code section, i.e., begin of data sections to copy from.
     *      __data_start__/__data_end__: RAM address range that data should be
     *      copied to. Both must be aligned to 4 bytes boundary.  */

        ldr    r1, =__etext
        ldr    r2, =__data_start__
        ldr    r3, =__data_end__

        subs    r3, r2
        ble    .Lflash_to_ram_loop_end

        movs    r4, 0
    .Lflash_to_ram_loop:
        ldr    r0, [r1,r4]
        str    r0, [r2,r4]
        adds    r4, 4
        cmp    r4, r3
        blt    .Lflash_to_ram_loop
    .Lflash_to_ram_loop_end:

        ldr    r0, =SystemInit
        blx    r0
        ldr    r0, =_start
        bx    r0
        .pool
        .size Reset_Handler, . - Reset_Handler

        .text
    /*    Macro to define default handlers. Default handler
     *    will be weak symbol and just dead loops. They can be
     *    overwritten by other handlers */

        .macro    def_default_handler    handler_name
        .align 1
        .thumb_func
        .weak    \handler_name
        .type    \handler_name, %function
    \handler_name :
        b    .
        .size    \handler_name, . - \handler_name
        .endm

        def_default_handler     NMI_Handler
        def_default_handler     HardFault_Handler
        def_default_handler     SVC_Handler
        def_default_handler     PendSV_Handler
        def_default_handler     SysTick_Handler
        def_default_handler     Default_Handler

        .macro    def_irq_default_handler    handler_name
        .weak     \handler_name
        .set      \handler_name, Default_Handler
        .endm

        def_irq_default_handler        INT0_IRQHandler
        def_irq_default_handler        INT1_IRQHandler
        def_irq_default_handler        INT2_IRQHandler
        def_irq_default_handler        INT3_IRQHandler
        def_irq_default_handler        INT4_IRQHandler
        def_irq_default_handler        INT5_IRQHandler
        def_irq_default_handler        INTRX0_IRQHandler
        def_irq_default_handler        INTTX0_IRQHandler
        def_irq_default_handler        INTRX1_IRQHandler
        def_irq_default_handler        INTTX1_IRQHandler
        def_irq_default_handler        INTSPIRX_IRQHandler
        def_irq_default_handler        INTSPITX_IRQHandler
        def_irq_default_handler        INTSPIERR_IRQHandler
        def_irq_default_handler        INTI2C0_IRQHandler
        def_irq_default_handler        INTI2C1_IRQHandler
        def_irq_default_handler        INTDMA_IRQHandler
        def_irq_default_handler        INT16A0_IRQHandler
        def_irq_default_handler        INT16A1_IRQHandler
        def_irq_default_handler        INTTB0_IRQHandler
        def_irq_default_handler        INTTB1_IRQHandler
        def_irq_default_handler        INTTB2_IRQHandler
        def_irq_default_handler        INTTB3_IRQHandler
        def_irq_default_handler        INTTB4_IRQHandler
        def_irq_default_handler        INTTB5_IRQHandler
        def_irq_default_handler        INTTB6_IRQHandler
        def_irq_default_handler        INTTB7_IRQHandler
        def_irq_default_handler        INTI2CS_IRQHandler
        def_irq_default_handler        INTTMRD_IRQHandler
        def_irq_default_handler        INTUSB_IRQHandler
        def_irq_default_handler        INTUSBWKUP_IRQHandler
        def_irq_default_handler        INTADHP_IRQHandler
        def_irq_default_handler        INTAD_IRQHandler

        .end
