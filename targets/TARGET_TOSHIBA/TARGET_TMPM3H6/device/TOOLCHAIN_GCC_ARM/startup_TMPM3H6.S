/**
 *******************************************************************************
 * @file    startup_TMPM3H6.s
 * @brief   CMSIS Cortex-M3 Core Device Startup File for the
 *          TOSHIBA 'TMPM3H6' Device Series
 * @version V5.00
 * @date    2016/03/02
 *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
 *
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

.syntax unified
.arch    armv7-m

.section .stack
.align 3

/*
// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

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

    .section .vectors
    .align 2
    .globl    __Vectors
__Vectors:
    .long   __StackTop                  /* Top of Stack          */
    .long   Reset_Handler               /* Reset Handler         */
    .long   NMI_Handler                 /* NMI Handler           */
    .long   HardFault_Handler           /* Hard Fault Handler    */
    .long   MemManage_Handler           /* MPU Fault Handler     */
    .long   BusFault_Handler            /* Bus Fault Handler     */
    .long   UsageFault_Handler          /* Usage Fault Handler   */
    .long   0                           /* Reserved              */
    .long   0                           /* Reserved              */
    .long   0                           /* Reserved              */
    .long   0                           /* Reserved              */
    .long   SVC_Handler                 /* SVCall Handler        */
    .long   DebugMon_Handler            /* Debug Monitor Handler */
    .long   0                           /* Reserved              */
    .long   PendSV_Handler              /* PendSV Handler        */
    .long   SysTick_Handler             /* SysTick Handler       */

    /* External interrupts */
    .long   INT00_IRQHandler            // 0:   Interrupt Pin0
    .long   INT01_IRQHandler            // 1:   Interrupt Pin1
    .long   INT02_IRQHandler            // 2:   Interrupt Pin2
    .long   INT03_IRQHandler            // 3:   Interrupt Pin3
    .long   INT04_IRQHandler            // 4:   Interrupt Pin4
    .long   INT05_IRQHandler            // 5:   Interrupt Pin5
    .long   INT06_IRQHandler            // 6:   Interrupt Pin6
    .long   INT07_IRQHandler            // 7:   Interrupt Pin7
    .long   INT08_IRQHandler            // 8:   Interrupt Pin8
    .long   INT09_IRQHandler            // 9:   Interrupt Pin9
    .long   INT10_IRQHandler            // 10:  Interrupt Pin10
    .long   INT11_IRQHandler            // 11:  Interrupt Pin11
    .long   INT12_IRQHandler            // 12:  Interrupt Pin12
    .long   INT13_IRQHandler            // 13:  Interrupt Pin13
    .long   INT14_IRQHandler            // 14:  Interrupt Pin14
    .long   INT15_IRQHandler            // 15:  Interrupt Pin15
    .long   INTEMG0_IRQHandler          // 16:  PMD0 EMG interrupt
    .long   INTOVV0_IRQHandler          // 17:  PMD0 OVV interrupt
    .long   INTPMD0_IRQHandler          // 18:  PMD0 interrupt
    .long   INTENC00_IRQHandler         // 19:  Encoder 0 interrupt 0
    .long   INTENC01_IRQHandler         // 20:  Encoder 0 interrupt 1
    .long   INTADAPDA_IRQHandler        // 21:  ADC conversion triggered by PMD is finished A
    .long   INTADAPDB_IRQHandler        // 22:  ADC conversion triggered by PMD is finished B
    .long   INTADACP0_IRQHandler        // 23:  ADC conversion monitoring function interrupt A
    .long   INTADACP1_IRQHandler        // 24:  ADC conversion monitoring function interrupt B
    .long   INTADATRG_IRQHandler        // 25:  ADC conversion triggered by General purpose is finished
    .long   INTADASGL_IRQHandler        // 26:  ADC conversion triggered by Single program is finished
    .long   INTADACNT_IRQHandler        // 27:  ADC conversion triggered by Continuity program is finished
    .long   INTT0RX_IRQHandler          // 28:  TSPI/SIO reception (channel 0)
    .long   INTT0TX_IRQHandler          // 29:  TSPI/SIO transmit (channel 0)
    .long   INTT0ERR_IRQHandler         // 30:  TSPI/SIO error (channel 0)
    .long   INTT1RX_IRQHandler          // 31:  TSPI/SIO reception (channel 1)
    .long   INTT1TX_IRQHandler          // 32:  TSPI/SIO transmit (channel 1)
    .long   INTT1ERR_IRQHandler         // 33:  TSPI/SIO error (channel 1)
    .long   INTI2CWUP_IRQHandler        // 34:  Serial bus interface (WakeUp) interrupt (channel 0)
    .long   INTI2C0_IRQHandler          // 35:  I2C0 transmission and reception interrupt
    .long   INTI2C0AL_IRQHandler        // 36:  I2C0 arbitration lost interrupt
    .long   INTI2C0BF_IRQHandler        // 37:  I2C0 bus free interrupt
    .long   INTI2C0NA_IRQHandler        // 38:  I2C0 no ack interrupt
    .long   INTI2C1_IRQHandler          // 39:  I2C1 transmission and reception interrupt
    .long   INTI2C1AL_IRQHandler        // 40:  I2C1 arbitration lost interrupt
    .long   INTI2C1BF_IRQHandler        // 41:  I2C1 bus free interrupt
    .long   INTI2C1NA_IRQHandler        // 42:  I2C1 no ack interrupt
    .long   INTI2C2_IRQHandler          // 43:  I2C2 transmission and reception interrupt
    .long   INTI2C2AL_IRQHandler        // 44:  I2C2 arbitration lost interrupt
    .long   INTI2C2BF_IRQHandler        // 45:  I2C2 bus free interrupt
    .long   INTI2C2NA_IRQHandler        // 46:  I2C2 no ack interrupt
    .long   INTUART0RX_IRQHandler       // 47:  UART reception (channel 0)
    .long   INTUART0TX_IRQHandler       // 48:  UART transmit (channel 0)
    .long   INTUART0ERR_IRQHandler      // 49:  UART error (channel 0)
    .long   INTUART1RX_IRQHandler       // 50:  UART reception (channel 1)
    .long   INTUART1TX_IRQHandler       // 51:  UART transmit (channel 1)
    .long   INTUART1ERR_IRQHandler      // 52:  UART error (channel 1)
    .long   INTUART2RX_IRQHandler       // 53:  UART reception (channel 2)
    .long   INTUART2TX_IRQHandler       // 54:  UART transmit (channel 2)
    .long   INTUART2ERR_IRQHandler      // 55:  UART error (channel 2)
    .long   INTT32A00A_IRQHandler       // 56:  32bit T32A0A compare match detection 0 / Over flow / under flow
    .long   INTT32A00ACAP0_IRQHandler   // 57:  32bit T32A0A input capture 0
    .long   INTT32A00ACAP1_IRQHandler   // 58:  32bit T32A0A input capture 1
    .long   INTT32A00B_IRQHandler       // 59:  32bit T32A0B compare match detection 0 / Over flow / under flow
    .long   INTT32A00BCAP0_IRQHandler   // 60:  32bit T32A0B input capture 0
    .long   INTT32A00BCAP1_IRQHandler   // 61:  32bit T32A0B input capture 1
    .long   INTT32A00C_IRQHandler       // 62:  32bit T32A0C compare match detection 0 / Over flow / under flow
    .long   INTT32A00CCAP0_IRQHandler   // 63:  32bit T32A0C input capture 0
    .long   INTT32A00CCAP1_IRQHandler   // 64:  32bit T32A0C input capture 1
    .long   INTT32A01A_IRQHandler       // 65:  32bit T32A1A compare match detection 0 / Over flow / under flow
    .long   INTT32A01ACAP0_IRQHandler   // 66:  32bit T32A1A input capture 0
    .long   INTT32A01ACAP1_IRQHandler   // 67:  32bit T32A1A input capture 1
    .long   INTT32A01B_IRQHandler       // 68:  32bit T32A1B compare match detection 0 / Over flow / under flow
    .long   INTT32A01BCAP0_IRQHandler   // 69:  32bit T32A1B input capture 0
    .long   INTT32A01BCAP1_IRQHandler   // 70:  32bit T32A1B input capture 1
    .long   INTT32A01C_IRQHandler       // 71:  32bit T32A1C compare match detection 0 / Over flow / under flow
    .long   INTT32A01CCAP0_IRQHandler   // 72:  32bit T32A1C input capture 0
    .long   INTT32A01CCAP1_IRQHandler   // 73:  32bit T32A1C input capture 1
    .long   INTT32A02A_IRQHandler       // 74:  32bit T32A2A compare match detection 0 / Over flow / under flow
    .long   INTT32A02ACAP0_IRQHandler   // 75:  32bit T32A2A input capture 0
    .long   INTT32A02ACAP1_IRQHandler   // 76:  32bit T32A2A input capture 1
    .long   INTT32A02B_IRQHandler       // 77:  32bit T32A2B compare match detection 0 / Over flow / under flow
    .long   INTT32A02BCAP0_IRQHandler   // 78:  32bit T32A2B input capture 0
    .long   INTT32A02BCAP1_IRQHandler   // 79:  32bit T32A2B input capture 1
    .long   INTT32A02C_IRQHandler       // 80:  32bit T32A2C compare match detection 0 / Over flow / under flow
    .long   INTT32A02CCAP0_IRQHandler   // 81:  32bit T32A2C input capture 0
    .long   INTT32A02CCAP1_IRQHandler   // 82:  32bit T32A2C input capture 1
    .long   INTT32A03A_IRQHandler       // 83:  32bit T32A3A compare match detection 0 / Over flow / under flow
    .long   INTT32A03ACAP0_IRQHandler   // 84:  32bit T32A3A input capture 0
    .long   INTT32A03ACAP1_IRQHandler   // 85:  32bit T32A3A input capture 1
    .long   INTT32A03B_IRQHandler       // 86:  32bit T32A3B compare match detection 0 / Over flow / under flow
    .long   INTT32A03BCAP0_IRQHandler   // 87:  32bit T32A3B input capture 0
    .long   INTT32A03BCAP1_IRQHandler   // 88:  32bit T32A3B input capture 1
    .long   INTT32A03C_IRQHandler       // 89:  32bit T32A3C compare match detection 0 / Over flow / under flow
    .long   INTT32A03CCAP0_IRQHandler   // 90:  32bit T32A3C input capture 0
    .long   INTT32A03CCAP1_IRQHandler   // 91:  32bit T32A3C input capture 1
    .long   INTT32A04A_IRQHandler       // 92:  32bit T32A4A compare match detection 0 / Over flow / under flow
    .long   INTT32A04ACAP0_IRQHandler   // 93:  32bit T32A4A input capture 0
    .long   INTT32A04ACAP1_IRQHandler   // 94:  32bit T32A4A input capture 1
    .long   INTT32A04B_IRQHandler       // 95:  32bit T32A4B compare match detection 0 / Over flow / under flow
    .long   INTT32A04BCAP0_IRQHandler   // 96:  32bit T32A4B input capture 0
    .long   INTT32A04BCAP1_IRQHandler   // 97:  32bit T32A4B input capture 1
    .long   INTT32A04C_IRQHandler       // 98:  32bit T32A4C compare match detection 0 / Over flow / under flow
    .long   INTT32A04CCAP0_IRQHandler   // 99:  32bit T32A4C input capture 0
    .long   INTT32A04CCAP1_IRQHandler   // 100: 32bit T32A4C input capture 1
    .long   INTT32A05A_IRQHandler       // 101: 32bit T32A5A compare match detection 0 / Over flow / under flow
    .long   INTT32A05ACAP0_IRQHandler   // 102: 32bit T32A5A input capture 0
    .long   INTT32A05ACAP1_IRQHandler   // 103: 32bit T32A5A input capture 1
    .long   INTT32A05B_IRQHandler       // 104: 32bit T32A5B compare match detection 0 / Over flow / under flow
    .long   INTT32A05BCAP0_IRQHandler   // 105: 32bit T32A5B input capture 0
    .long   INTT32A05BCAP1_IRQHandler   // 106: 32bit T32A5B input capture 1
    .long   INTT32A05C_IRQHandler       // 107: 32bit T32A5C compare match detection 0 / Over flow / under flow
    .long   INTT32A05CCAP0_IRQHandler   // 108: 32bit T32A5C input capture 0
    .long   INTT32A05CCAP1_IRQHandler   // 109: 32bit T32A5C input capture 1
    .long   INTDMAATC_IRQHandler        // 110: DMA end of transfer
    .long   INTDMAAERR_IRQHandler       // 111: DMA transfer error
    .long   INTRTC_IRQHandler           // 112: Real time clock(XHz) interrupt
    .long   0                           // 113: Reserved
    .long   INTRMC0_IRQHandler          // 114: Remote control reception interrupt
    .long   INTFLCRDY_IRQHandler        // 115: Code FLASH Ready interrupt
    .long   INTFLDRDY_IRQHandler        // 116: Data FLASH Ready interrupt

    .size    __Vectors, . - __Vectors

    .text
    .thumb
    .thumb_func
    .align    2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r4, =__copy_table_start__
    ldr    r5, =__copy_table_end__

.L_loop0:
    cmp    r4, r5
    bge    .L_loop0_done
    ldr    r1, [r4]
    ldr    r2, [r4, #4]
    ldr    r3, [r4, #8]

.L_loop0_0:
    subs    r3, #4
    ittt    ge
    ldrge    r0, [r1, r3]
    strge    r0, [r2, r3]
    bge    .L_loop0_0

    adds    r4, #12
    b    .L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

.L_loop1:
    cmp    r2, r3
    ittt    lt
    ldrlt    r0, [r1], #4
    strlt    r0, [r2], #4
    blt    .L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
    ldr    r3, =__zero_table_start__
    ldr    r4, =__zero_table_end__

.L_loop2:
    cmp    r3, r4
    bge    .L_loop2_done
    ldr    r1, [r3]
    ldr    r2, [r3, #4]
    movs    r0, 0

.L_loop2_0:
    subs    r2, #4
    itt    ge
    strge    r0, [r1, r2]
    bge    .L_loop2_0

    adds    r3, #8
    b    .L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__bss_start__
    ldr    r2, =__bss_end__

    movs    r0, 0
.L_loop3:
    cmp    r1, r2
    itt    lt
    strlt    r0, [r1], #4
    blt    .L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
    bl    SystemInit
#endif

#ifndef __START
#define __START _start
#endif
    bl    __START

    .pool
    .size    Reset_Handler, . - Reset_Handler

    .align    1
    .thumb_func
    .weak    Default_Handler
    .type    Default_Handler, %function
Default_Handler:
    b    .
    .size    Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_irq_handler    handler_name
    .weak    \handler_name
    .set    \handler_name, Default_Handler
    .endm

    def_irq_handler     NMI_Handler
    def_irq_handler     HardFault_Handler
    def_irq_handler     MemManage_Handler
    def_irq_handler     BusFault_Handler
    def_irq_handler     UsageFault_Handler
    def_irq_handler     SVC_Handler
    def_irq_handler     DebugMon_Handler
    def_irq_handler     PendSV_Handler
    def_irq_handler     SysTick_Handler

    def_irq_handler        INT00_IRQHandler
    def_irq_handler        INT01_IRQHandler
    def_irq_handler        INT02_IRQHandler
    def_irq_handler        INT03_IRQHandler
    def_irq_handler        INT04_IRQHandler
    def_irq_handler        INT05_IRQHandler
    def_irq_handler        INT06_IRQHandler
    def_irq_handler        INT07_IRQHandler
    def_irq_handler        INT08_IRQHandler
    def_irq_handler        INT09_IRQHandler
    def_irq_handler        INT10_IRQHandler
    def_irq_handler        INT11_IRQHandler
    def_irq_handler        INT12_IRQHandler
    def_irq_handler        INT13_IRQHandler
    def_irq_handler        INT14_IRQHandler
    def_irq_handler        INT15_IRQHandler
    def_irq_handler        INTEMG0_IRQHandler
    def_irq_handler        INTOVV0_IRQHandler
    def_irq_handler        INTPMD0_IRQHandler
    def_irq_handler        INTENC00_IRQHandler
    def_irq_handler        INTENC01_IRQHandler
    def_irq_handler        INTADAPDA_IRQHandler
    def_irq_handler        INTADAPDB_IRQHandler
    def_irq_handler        INTADACP0_IRQHandler
    def_irq_handler        INTADACP1_IRQHandler
    def_irq_handler        INTADATRG_IRQHandler
    def_irq_handler        INTADASGL_IRQHandler
    def_irq_handler        INTADACNT_IRQHandler
    def_irq_handler        INTT0RX_IRQHandler
    def_irq_handler        INTT0TX_IRQHandler
    def_irq_handler        INTT0ERR_IRQHandler
    def_irq_handler        INTT1RX_IRQHandler
    def_irq_handler        INTT1TX_IRQHandler
    def_irq_handler        INTT1ERR_IRQHandler
    def_irq_handler        INTI2CWUP_IRQHandler
    def_irq_handler        INTI2C0_IRQHandler
    def_irq_handler        INTI2C0AL_IRQHandler
    def_irq_handler        INTI2C0BF_IRQHandler
    def_irq_handler        INTI2C0NA_IRQHandler
    def_irq_handler        INTI2C1_IRQHandler
    def_irq_handler        INTI2C1AL_IRQHandler
    def_irq_handler        INTI2C1BF_IRQHandler
    def_irq_handler        INTI2C1NA_IRQHandler
    def_irq_handler        INTI2C2_IRQHandler
    def_irq_handler        INTI2C2AL_IRQHandler
    def_irq_handler        INTI2C2BF_IRQHandler
    def_irq_handler        INTI2C2NA_IRQHandler
    def_irq_handler        INTUART0RX_IRQHandler
    def_irq_handler        INTUART0TX_IRQHandler
    def_irq_handler        INTUART0ERR_IRQHandler
    def_irq_handler        INTUART1RX_IRQHandler
    def_irq_handler        INTUART1TX_IRQHandler
    def_irq_handler        INTUART1ERR_IRQHandler
    def_irq_handler        INTUART2RX_IRQHandler
    def_irq_handler        INTUART2TX_IRQHandler
    def_irq_handler        INTUART2ERR_IRQHandler
    def_irq_handler        INTT32A00A_IRQHandler     
    def_irq_handler        INTT32A00ACAP0_IRQHandler 
    def_irq_handler        INTT32A00ACAP1_IRQHandler 
    def_irq_handler        INTT32A00B_IRQHandler     
    def_irq_handler        INTT32A00BCAP0_IRQHandler 
    def_irq_handler        INTT32A00BCAP1_IRQHandler 
    def_irq_handler        INTT32A00C_IRQHandler     
    def_irq_handler        INTT32A00CCAP0_IRQHandler 
    def_irq_handler        INTT32A00CCAP1_IRQHandler 
    def_irq_handler        INTT32A01A_IRQHandler     
    def_irq_handler        INTT32A01ACAP0_IRQHandler 
    def_irq_handler        INTT32A01ACAP1_IRQHandler 
    def_irq_handler        INTT32A01B_IRQHandler     
    def_irq_handler        INTT32A01BCAP0_IRQHandler 
    def_irq_handler        INTT32A01BCAP1_IRQHandler 
    def_irq_handler        INTT32A01C_IRQHandler     
    def_irq_handler        INTT32A01CCAP0_IRQHandler 
    def_irq_handler        INTT32A01CCAP1_IRQHandler 
    def_irq_handler        INTT32A02A_IRQHandler     
    def_irq_handler        INTT32A02ACAP0_IRQHandler 
    def_irq_handler        INTT32A02ACAP1_IRQHandler 
    def_irq_handler        INTT32A02B_IRQHandler     
    def_irq_handler        INTT32A02BCAP0_IRQHandler 
    def_irq_handler        INTT32A02BCAP1_IRQHandler 
    def_irq_handler        INTT32A02C_IRQHandler     
    def_irq_handler        INTT32A02CCAP0_IRQHandler 
    def_irq_handler        INTT32A02CCAP1_IRQHandler 
    def_irq_handler        INTT32A03A_IRQHandler     
    def_irq_handler        INTT32A03ACAP0_IRQHandler 
    def_irq_handler        INTT32A03ACAP1_IRQHandler 
    def_irq_handler        INTT32A03B_IRQHandler     
    def_irq_handler        INTT32A03BCAP0_IRQHandler 
    def_irq_handler        INTT32A03BCAP1_IRQHandler 
    def_irq_handler        INTT32A03C_IRQHandler     
    def_irq_handler        INTT32A03CCAP0_IRQHandler 
    def_irq_handler        INTT32A03CCAP1_IRQHandler 
    def_irq_handler        INTT32A04A_IRQHandler     
    def_irq_handler        INTT32A04ACAP0_IRQHandler 
    def_irq_handler        INTT32A04ACAP1_IRQHandler 
    def_irq_handler        INTT32A04B_IRQHandler     
    def_irq_handler        INTT32A04BCAP0_IRQHandler 
    def_irq_handler        INTT32A04BCAP1_IRQHandler 
    def_irq_handler        INTT32A04C_IRQHandler     
    def_irq_handler        INTT32A04CCAP0_IRQHandler 
    def_irq_handler        INTT32A04CCAP1_IRQHandler 
    def_irq_handler        INTT32A05A_IRQHandler     
    def_irq_handler        INTT32A05ACAP0_IRQHandler 
    def_irq_handler        INTT32A05ACAP1_IRQHandler 
    def_irq_handler        INTT32A05B_IRQHandler     
    def_irq_handler        INTT32A05BCAP0_IRQHandler 
    def_irq_handler        INTT32A05BCAP1_IRQHandler 
    def_irq_handler        INTT32A05C_IRQHandler     
    def_irq_handler        INTT32A05CCAP0_IRQHandler 
    def_irq_handler        INTT32A05CCAP1_IRQHandler 
    def_irq_handler        INTDMAATC_IRQHandler      
    def_irq_handler        INTDMAAERR_IRQHandler     
    def_irq_handler        INTRTC_IRQHandler         
    def_irq_handler        INTRMC0_IRQHandler
    def_irq_handler        INTFLCRDY_IRQHandler
    def_irq_handler        INTFLDRDY_IRQHandler

    .end
