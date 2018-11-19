/**
 *******************************************************************************
 * @file    startup_TMPM3HQ.s
 * @brief   CMSIS Cortex-M3 Core Device Startup File for the
 *          TOSHIBA 'TMPM3HQ' Device Series
 * @version V5.00
 * @date    2016/03/02
 *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
 *
 * (C)Copyright TOSHIBA CORPORATION 2015 All rights reserved
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
    .long   __StackTop                  // Top of Stack          
    .long   Reset_Handler               // Reset Handler         
    .long   NMI_Handler                 // NMI Handler           
    .long   HardFault_Handler           // Hard Fault Handler    
    .long   MemManage_Handler           // MPU Fault Handler     
    .long   BusFault_Handler            // Bus Fault Handler     
    .long   UsageFault_Handler          // Usage Fault Handler   
    .long   0                           // Reserved              
    .long   0                           // Reserved              
    .long   0                           // Reserved              
    .long   0                           // Reserved              
    .long   SVC_Handler                 // SVCall Handler        
    .long   DebugMon_Handler            // Debug Monitor Handler 
    .long   0                           // Reserved              
    .long   PendSV_Handler              // PendSV Handler        
    .long   SysTick_Handler             // SysTick Handler       

    // External interrupts
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
    .long   INT16_IRQHandler            // 16:  Interrupt Pin 16
    .long   INT17_18_IRQHandler         // 17:  Interrupt Pin 17_18
    .long   INT19_22_IRQHandler         // 18:  Interrupt Pin 19_22
    .long   INT23_26_IRQHandler         // 19:  Interrupt Pin 23_26
    .long   INT27_28_IRQHandler         // 20:  Interrupt Pin 27_28
    .long   INT29_IRQHandler            // 21:  Interrupt Pin 29
    .long   INT30_31_IRQHandler         // 22:  Interrupt Pin 30_31
    .long   INTEMG0_IRQHandler          // 23:  PMD0 EMG interrupt
    .long   INTOVV0_IRQHandler          // 24:  PMD0 OVV interrupt
    .long   INTPMD0_IRQHandler          // 25:  PMD0 interrupt
    .long   INTENC00_IRQHandler         // 26:  Encoder 0 interrupt 0
    .long   INTENC01_IRQHandler         // 27:  Encoder 0 interrupt 1
    .long   INTADAPDA_IRQHandler        // 28:  ADC conversion triggered by PMD is finished A
    .long   INTADAPDB_IRQHandler        // 29:  ADC conversion triggered by PMD is finished B
    .long   INTADACP0_IRQHandler        // 30:  ADC conversion monitoring function interrupt 0
    .long   INTADACP1_IRQHandler        // 31:  ADC conversion monitoring function interrupt 1
    .long   INTADATRG_IRQHandler        // 32:  ADC conversion triggered by General purpose is finished
    .long   INTADASGL_IRQHandler        // 33:  ADC conversion triggered by Single program is finished
    .long   INTADACNT_IRQHandler        // 34:  ADC conversion triggered by Continuity program is finished
    .long   INTT0RX_IRQHandler          // 35:  TSPI/SIO reception (channel 0)
    .long   INTT0TX_IRQHandler          // 36:  TSPI/SIO transmit (channel 0)
    .long   INTT0ERR_IRQHandler         // 37:  TSPI/SIO error (channel 0)
    .long   INTT1RX_IRQHandler          // 38:  TSPI/SIO reception (channel 1)
    .long   INTT1TX_IRQHandler          // 39:  TSPI/SIO transmit (channel 1)
    .long   INTT1ERR_IRQHandler         // 40:  TSPI/SIO error (channel 1)
    .long   INTT2RX_IRQHandler          // 41:  TSPI/SIO reception (channel 2)
    .long   INTT2TX_IRQHandler          // 42:  TSPI/SIO transmit (channel 2)
    .long   INTT2ERR_IRQHandler         // 43:  TSPI/SIO error (channel 2)
    .long   INTT3RX_IRQHandler          // 44:  TSPI/SIO reception (channel 3)
    .long   INTT3TX_IRQHandler          // 45:  TSPI/SIO transmit (channel 3)
    .long   INTT3ERR_IRQHandler         // 46:  TSPI/SIO error (channel 3)
    .long   INTT4RX_IRQHandler          // 47:  TSPI/SIO reception (channel 4)
    .long   INTT4TX_IRQHandler          // 48:  TSPI/SIO transmit (channel 4)
    .long   INTT4ERR_IRQHandler         // 49:  TSPI/SIO error (channel 4)
    .long   INTI2CWUP_IRQHandler        // 50:  Serial bus interface (WakeUp) interrupt (channel 0)
    .long   INTI2C0_IRQHandler          // 51:  I2C0 transmission and reception interrupt
    .long   INTI2C0AL_IRQHandler        // 52:  I2C0 arbitration lost interrupt
    .long   INTI2C0BF_IRQHandler        // 53:  I2C0 bus free interrupt
    .long   INTI2C0NA_IRQHandler        // 54:  I2C0 no ack interrupt
    .long   INTI2C1_IRQHandler          // 55:  I2C1 transmission and reception interrupt
    .long   INTI2C1AL_IRQHandler        // 56:  I2C1 arbitration lost interrupt
    .long   INTI2C1BF_IRQHandler        // 57:  I2C1 bus free interrupt
    .long   INTI2C1NA_IRQHandler        // 58:  I2C1 no ack interrupt
    .long   INTI2C2_IRQHandler          // 59:  I2C2 transmission and reception interrupt
    .long   INTI2C2AL_IRQHandler        // 60:  I2C2 arbitration lost interrupt
    .long   INTI2C2BF_IRQHandler        // 61:  I2C2 bus free interrupt
    .long   INTI2C2NA_IRQHandler        // 62:  I2C2 no ack interrupt
    .long   INTI2C3_IRQHandler          // 63:  I2C3 transmission and reception interrupt
    .long   INTI2C3AL_IRQHandler        // 64:  I2C3 arbitration lost interrupt
    .long   INTI2C3BF_IRQHandler        // 65:  I2C3 bus free interrupt
    .long   INTI2C3NA_IRQHandler        // 66:  I2C3 no ack interrupt
    .long   INTUART0RX_IRQHandler       // 67:  UART reception (channel 0)
    .long   INTUART0TX_IRQHandler       // 68:  UART transmit (channel 0)
    .long   INTUART0ERR_IRQHandler      // 69:  UART error (channel 0)
    .long   INTUART1RX_IRQHandler       // 70:  UART reception (channel 1)
    .long   INTUART1TX_IRQHandler       // 71:  UART transmit (channel 1)
    .long   INTUART1ERR_IRQHandler      // 72:  UART error (channel 1)
    .long   INTUART2RX_IRQHandler       // 73:  UART reception (channel 2)
    .long   INTUART2TX_IRQHandler       // 74:  UART transmit (channel 2)
    .long   INTUART2ERR_IRQHandler      // 75:  UART error (channel 2)
    .long   INTUART3RX_IRQHandler       // 76:  UART reception (channel 3)
    .long   INTUART3TX_IRQHandler       // 77:  UART transmit (channel 3)
    .long   INTUART3ERR_IRQHandler      // 78:  UART error (channel 3)
    .long   INTUART4RX_IRQHandler       // 79:  UART reception (channel 4)
    .long   INTUART4TX_IRQHandler       // 80:  UART transmit (channel 4)
    .long   INTUART4ERR_IRQHandler      // 81:  UART error (channel 4)
    .long   INTUART5RX_IRQHandler       // 82:  UART reception (channel 5)
    .long   INTUART5TX_IRQHandler       // 83:  UART transmit (channel 5)
    .long   INTUART5ERR_IRQHandler      // 84:  UART error (channel 5)
    .long   INTT32A00A_IRQHandler       // 85:  32bit T32A00A compare match detection 0 / Over flow / under flow
    .long   INTT32A00ACAP0_IRQHandler   // 86:  32bit T32A00A input capture 0
    .long   INTT32A00ACAP1_IRQHandler   // 87:  32bit T32A00A input capture 1
    .long   INTT32A00B_IRQHandler       // 88:  32bit T32A00B compare match detection 0 / Over flow / under flow
    .long   INTT32A00BCAP0_IRQHandler   // 89:  32bit T32A00B input capture 0
    .long   INTT32A00BCAP1_IRQHandler   // 90:  32bit T32A00B input capture 1
    .long   INTT32A00C_IRQHandler       // 91:  32bit T32A00C compare match detection 0 / Over flow / under flow
    .long   INTT32A00CCAP0_IRQHandler   // 92:  32bit T32A00C input capture 0
    .long   INTT32A00CCAP1_IRQHandler   // 93:  32bit T32A00C input capture 1
    .long   INTT32A01A_IRQHandler       // 94:  32bit T32A01A compare match detection 0 / Over flow / under flow
    .long   INTT32A01ACAP0_IRQHandler   // 95:  32bit T32A01A input capture 0
    .long   INTT32A01ACAP1_IRQHandler   // 96:  32bit T32A01A input capture 1
    .long   INTT32A01B_IRQHandler       // 97:  32bit T32A01B compare match detection 0 / Over flow / under flow
    .long   INTT32A01BCAP0_IRQHandler   // 98:  32bit T32A01B input capture 0
    .long   INTT32A01BCAP1_IRQHandler   // 99:  32bit T32A01B input capture 1
    .long   INTT32A01C_IRQHandler       // 100: 32bit T32A01C compare match detection 0 / Over flow / under flow
    .long   INTT32A01CCAP0_IRQHandler   // 101: 32bit T32A01C input capture 0
    .long   INTT32A01CCAP1_IRQHandler   // 102: 32bit T32A01C input capture 1
    .long   INTT32A02A_IRQHandler       // 103: 32bit T32A02A compare match detection 0 / Over flow / under flow
    .long   INTT32A02ACAP0_IRQHandler   // 104: 32bit T32A02A input capture 0
    .long   INTT32A02ACAP1_IRQHandler   // 105: 32bit T32A02A input capture 1
    .long   INTT32A02B_IRQHandler       // 106: 32bit T32A02B compare match detection 0 / Over flow / under flow
    .long   INTT32A02BCAP0_IRQHandler   // 107: 32bit T32A02B input capture 0
    .long   INTT32A02BCAP1_IRQHandler   // 108: 32bit T32A02B input capture 1
    .long   INTT32A02C_IRQHandler       // 109: 32bit T32A02C compare match detection 0 / Over flow / under flow
    .long   INTT32A02CCAP0_IRQHandler   // 110: 32bit T32A02C input capture 0
    .long   INTT32A02CCAP1_IRQHandler   // 111: 32bit T32A02C input capture 1
    .long   INTT32A03A_IRQHandler       // 112: 32bit T32A03A compare match detection 0 / Over flow / under flow
    .long   INTT32A03ACAP0_IRQHandler   // 113: 32bit T32A03A input capture 0
    .long   INTT32A03ACAP1_IRQHandler   // 114: 32bit T32A03A input capture 1
    .long   INTT32A03B_IRQHandler       // 115: 32bit T32A03B compare match detection 0 / Over flow / under flow
    .long   INTT32A03BCAP0_IRQHandler   // 116: 32bit T32A03B input capture 0
    .long   INTT32A03BCAP1_IRQHandler   // 117: 32bit T32A03B input capture 1
    .long   INTT32A03C_IRQHandler       // 118: 32bit T32A03C compare match detection 0 / Over flow / under flow
    .long   INTT32A03CCAP0_IRQHandler   // 119: 32bit T32A03C input capture 0
    .long   INTT32A03CCAP1_IRQHandler   // 120: 32bit T32A03C input capture 1
    .long   INTT32A04A_IRQHandler       // 121: 32bit T32A04A compare match detection 0 / Over flow / under flow
    .long   INTT32A04ACAP0_IRQHandler   // 122: 32bit T32A04A input capture 0
    .long   INTT32A04ACAP1_IRQHandler   // 123: 32bit T32A04A input capture 1
    .long   INTT32A04B_IRQHandler       // 124: 32bit T32A04B compare match detection 0 / Over flow / under flow
    .long   INTT32A04BCAP0_IRQHandler   // 125: 32bit T32A04B input capture 0
    .long   INTT32A04BCAP1_IRQHandler   // 126: 32bit T32A04B input capture 1
    .long   INTT32A04C_IRQHandler       // 127: 32bit T32A04C compare match detection 0 / Over flow / under flow
    .long   INTT32A04CCAP0_IRQHandler   // 128: 32bit T32A04C input capture 0
    .long   INTT32A04CCAP1_IRQHandler   // 129: 32bit T32A04C input capture 1
    .long   INTT32A05A_IRQHandler       // 130: 32bit T32A05A compare match detection 0 / Over flow / under flow
    .long   INTT32A05ACAP0_IRQHandler   // 131: 32bit T32A05A input capture 0
    .long   INTT32A05ACAP1_IRQHandler   // 132: 32bit T32A05A input capture 1
    .long   INTT32A05B_IRQHandler       // 133: 32bit T32A05B compare match detection 0 / Over flow / under flow
    .long   INTT32A05BCAP0_IRQHandler   // 134: 32bit T32A05B input capture 0
    .long   INTT32A05BCAP1_IRQHandler   // 135: 32bit T32A05B input capture 1
    .long   INTT32A05C_IRQHandler       // 136: 32bit T32A05C compare match detection 0 / Over flow / under flow
    .long   INTT32A05CCAP0_IRQHandler   // 137: 32bit T32A05C input capture 0
    .long   INTT32A05CCAP1_IRQHandler   // 138: 32bit T32A05C input capture 1
    .long   INTT32A06A_IRQHandler       // 139: 32bit T32A06A compare match detection 0 / Over flow / under flow
    .long   INTT32A06ACAP0_IRQHandler   // 140: 32bit T32A06A input capture 0
    .long   INTT32A06ACAP1_IRQHandler   // 141: 32bit T32A06A input capture 1
    .long   INTT32A06B_IRQHandler       // 142: 32bit T32A06B compare match detection 0 / Over flow / under flow
    .long   INTT32A06BCAP0_IRQHandler   // 143: 32bit T32A06B input capture 0
    .long   INTT32A06BCAP1_IRQHandler   // 144: 32bit T32A06B input capture 1
    .long   INTT32A06C_IRQHandler       // 145: 32bit T32A06C compare match detection 0 / Over flow / under flow
    .long   INTT32A06CCAP0_IRQHandler   // 146: 32bit T32A06C input capture 0
    .long   INTT32A06CCAP1_IRQHandler   // 147: 32bit T32A06C input capture 1
    .long   INTT32A07A_IRQHandler       // 148: 32bit T32A07A compare match detection 0 / Over flow / under flow
    .long   INTT32A07ACAP0_IRQHandler   // 149: 32bit T32A07A input capture 0
    .long   INTT32A07ACAP1_IRQHandler   // 150: 32bit T32A07A input capture 1
    .long   INTT32A07B_IRQHandler       // 151: 32bit T32A07B compare match detection 0 / Over flow / under flow
    .long   INTT32A07BCAP0_IRQHandler   // 152: 32bit T32A07B input capture 0
    .long   INTT32A07BCAP1_IRQHandler   // 153: 32bit T32A07B input capture 1
    .long   INTT32A07C_IRQHandler       // 154: 32bit T32A07C compare match detection 0 / Over flow / under flow
    .long   INTT32A07CCAP0_IRQHandler   // 155: 32bit T32A07C input capture 0
    .long   INTT32A07CCAP1_IRQHandler   // 156: 32bit T32A07C input capture 1
    .long   INTPARI_IRQHandler          // 157: RAM parity interrupt
    .long   INTDMAATC_IRQHandler        // 158: DMAA end of transfer
    .long   INTDMAAERR_IRQHandler       // 159: DMAA transfer error
    .long   INTDMABTC_IRQHandler        // 160: DMAB end of transfer
    .long   INTDMABERR_IRQHandler       // 161: DMAB transfer error
    .long   INTRTC_IRQHandler           // 162: Real time clock interrupt
    .long   INTRMC0_IRQHandler          // 163: Remote control reception interrupt
    .long   INTFLCRDY_IRQHandler        // 164: Code FLASH Ready interrupt
    .long   INTFLDRDY_IRQHandler        // 165: Data FLASH Ready interrupt

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

    def_irq_handler     INT00_IRQHandler            
    def_irq_handler     INT01_IRQHandler            
    def_irq_handler     INT02_IRQHandler            
    def_irq_handler     INT03_IRQHandler            
    def_irq_handler     INT04_IRQHandler            
    def_irq_handler     INT05_IRQHandler            
    def_irq_handler     INT06_IRQHandler            
    def_irq_handler     INT07_IRQHandler            
    def_irq_handler     INT08_IRQHandler            
    def_irq_handler     INT09_IRQHandler            
    def_irq_handler     INT10_IRQHandler            
    def_irq_handler     INT11_IRQHandler            
    def_irq_handler     INT12_IRQHandler            
    def_irq_handler     INT13_IRQHandler            
    def_irq_handler     INT14_IRQHandler            
    def_irq_handler     INT15_IRQHandler            
    def_irq_handler     INT16_IRQHandler            
    def_irq_handler     INT17_18_IRQHandler         
    def_irq_handler     INT19_22_IRQHandler         
    def_irq_handler     INT23_26_IRQHandler         
    def_irq_handler     INT27_28_IRQHandler         
    def_irq_handler     INT29_IRQHandler            
    def_irq_handler     INT30_31_IRQHandler         
    def_irq_handler     INTEMG0_IRQHandler          
    def_irq_handler     INTOVV0_IRQHandler          
    def_irq_handler     INTPMD0_IRQHandler          
    def_irq_handler     INTENC00_IRQHandler         
    def_irq_handler     INTENC01_IRQHandler         
    def_irq_handler     INTADAPDA_IRQHandler        
    def_irq_handler     INTADAPDB_IRQHandler        
    def_irq_handler     INTADACP0_IRQHandler        
    def_irq_handler     INTADACP1_IRQHandler        
    def_irq_handler     INTADATRG_IRQHandler        
    def_irq_handler     INTADASGL_IRQHandler        
    def_irq_handler     INTADACNT_IRQHandler        
    def_irq_handler     INTT0RX_IRQHandler          
    def_irq_handler     INTT0TX_IRQHandler
    def_irq_handler     INTT0ERR_IRQHandler         
    def_irq_handler     INTT1RX_IRQHandler          
    def_irq_handler     INTT1TX_IRQHandler          
    def_irq_handler     INTT1ERR_IRQHandler         
    def_irq_handler     INTT2RX_IRQHandler          
    def_irq_handler     INTT2TX_IRQHandler          
    def_irq_handler     INTT2ERR_IRQHandler         
    def_irq_handler     INTT3RX_IRQHandler          
    def_irq_handler     INTT3TX_IRQHandler          
    def_irq_handler     INTT3ERR_IRQHandler         
    def_irq_handler     INTT4RX_IRQHandler          
    def_irq_handler     INTT4TX_IRQHandler          
    def_irq_handler     INTT4ERR_IRQHandler         
    def_irq_handler     INTI2CWUP_IRQHandler        
    def_irq_handler     INTI2C0_IRQHandler          
    def_irq_handler     INTI2C0AL_IRQHandler        
    def_irq_handler     INTI2C0BF_IRQHandler        
    def_irq_handler     INTI2C0NA_IRQHandler        
    def_irq_handler     INTI2C1_IRQHandler          
    def_irq_handler     INTI2C1AL_IRQHandler        
    def_irq_handler     INTI2C1BF_IRQHandler        
    def_irq_handler     INTI2C1NA_IRQHandler        
    def_irq_handler     INTI2C2_IRQHandler          
    def_irq_handler     INTI2C2AL_IRQHandler        
    def_irq_handler     INTI2C2BF_IRQHandler        
    def_irq_handler     INTI2C2NA_IRQHandler        
    def_irq_handler     INTI2C3_IRQHandler          
    def_irq_handler     INTI2C3AL_IRQHandler        
    def_irq_handler     INTI2C3BF_IRQHandler        
    def_irq_handler     INTI2C3NA_IRQHandler        
    def_irq_handler     INTUART0RX_IRQHandler       
    def_irq_handler     INTUART0TX_IRQHandler       
    def_irq_handler     INTUART0ERR_IRQHandler      
    def_irq_handler     INTUART1RX_IRQHandler       
    def_irq_handler     INTUART1TX_IRQHandler       
    def_irq_handler     INTUART1ERR_IRQHandler      
    def_irq_handler     INTUART2RX_IRQHandler       
    def_irq_handler     INTUART2TX_IRQHandler       
    def_irq_handler     INTUART2ERR_IRQHandler      
    def_irq_handler     INTUART3RX_IRQHandler       
    def_irq_handler     INTUART3TX_IRQHandler       
    def_irq_handler     INTUART3ERR_IRQHandler      
    def_irq_handler     INTUART4RX_IRQHandler       
    def_irq_handler     INTUART4TX_IRQHandler       
    def_irq_handler     INTUART4ERR_IRQHandler      
    def_irq_handler     INTUART5RX_IRQHandler       
    def_irq_handler     INTUART5TX_IRQHandler       
    def_irq_handler     INTUART5ERR_IRQHandler      
    def_irq_handler     INTT32A00A_IRQHandler       
    def_irq_handler     INTT32A00ACAP0_IRQHandler   
    def_irq_handler     INTT32A00ACAP1_IRQHandler   
    def_irq_handler     INTT32A00B_IRQHandler       
    def_irq_handler     INTT32A00BCAP0_IRQHandler   
    def_irq_handler     INTT32A00BCAP1_IRQHandler   
    def_irq_handler     INTT32A00C_IRQHandler       
    def_irq_handler     INTT32A00CCAP0_IRQHandler   
    def_irq_handler     INTT32A00CCAP1_IRQHandler   
    def_irq_handler     INTT32A01A_IRQHandler       
    def_irq_handler     INTT32A01ACAP0_IRQHandler   
    def_irq_handler     INTT32A01ACAP1_IRQHandler   
    def_irq_handler     INTT32A01B_IRQHandler       
    def_irq_handler     INTT32A01BCAP0_IRQHandler   
    def_irq_handler     INTT32A01BCAP1_IRQHandler   
    def_irq_handler     INTT32A01C_IRQHandler       
    def_irq_handler     INTT32A01CCAP0_IRQHandler   
    def_irq_handler     INTT32A01CCAP1_IRQHandler   
    def_irq_handler     INTT32A02A_IRQHandler       
    def_irq_handler     INTT32A02ACAP0_IRQHandler   
    def_irq_handler     INTT32A02ACAP1_IRQHandler   
    def_irq_handler     INTT32A02B_IRQHandler       
    def_irq_handler     INTT32A02BCAP0_IRQHandler   
    def_irq_handler     INTT32A02BCAP1_IRQHandler   
    def_irq_handler     INTT32A02C_IRQHandler       
    def_irq_handler     INTT32A02CCAP0_IRQHandler   
    def_irq_handler     INTT32A02CCAP1_IRQHandler   
    def_irq_handler     INTT32A03A_IRQHandler       
    def_irq_handler     INTT32A03ACAP0_IRQHandler   
    def_irq_handler     INTT32A03ACAP1_IRQHandler   
    def_irq_handler     INTT32A03B_IRQHandler       
    def_irq_handler     INTT32A03BCAP0_IRQHandler   
    def_irq_handler     INTT32A03BCAP1_IRQHandler   
    def_irq_handler     INTT32A03C_IRQHandler       
    def_irq_handler     INTT32A03CCAP0_IRQHandler   
    def_irq_handler     INTT32A03CCAP1_IRQHandler   
    def_irq_handler     INTT32A04A_IRQHandler       
    def_irq_handler     INTT32A04ACAP0_IRQHandler   
    def_irq_handler     INTT32A04ACAP1_IRQHandler   
    def_irq_handler     INTT32A04B_IRQHandler       
    def_irq_handler     INTT32A04BCAP0_IRQHandler   
    def_irq_handler     INTT32A04BCAP1_IRQHandler   
    def_irq_handler     INTT32A04C_IRQHandler       
    def_irq_handler     INTT32A04CCAP0_IRQHandler   
    def_irq_handler     INTT32A04CCAP1_IRQHandler   
    def_irq_handler     INTT32A05A_IRQHandler       
    def_irq_handler     INTT32A05ACAP0_IRQHandler   
    def_irq_handler     INTT32A05ACAP1_IRQHandler   
    def_irq_handler     INTT32A05B_IRQHandler       
    def_irq_handler     INTT32A05BCAP0_IRQHandler   
    def_irq_handler     INTT32A05BCAP1_IRQHandler   
    def_irq_handler     INTT32A05C_IRQHandler       
    def_irq_handler     INTT32A05CCAP0_IRQHandler   
    def_irq_handler     INTT32A05CCAP1_IRQHandler   
    def_irq_handler     INTT32A06A_IRQHandler       
    def_irq_handler     INTT32A06ACAP0_IRQHandler   
    def_irq_handler     INTT32A06ACAP1_IRQHandler   
    def_irq_handler     INTT32A06B_IRQHandler       
    def_irq_handler     INTT32A06BCAP0_IRQHandler   
    def_irq_handler     INTT32A06BCAP1_IRQHandler   
    def_irq_handler     INTT32A06C_IRQHandler       
    def_irq_handler     INTT32A06CCAP0_IRQHandler   
    def_irq_handler     INTT32A06CCAP1_IRQHandler   
    def_irq_handler     INTT32A07A_IRQHandler       
    def_irq_handler     INTT32A07ACAP0_IRQHandler   
    def_irq_handler     INTT32A07ACAP1_IRQHandler   
    def_irq_handler     INTT32A07B_IRQHandler       
    def_irq_handler     INTT32A07BCAP0_IRQHandler   
    def_irq_handler     INTT32A07BCAP1_IRQHandler   
    def_irq_handler     INTT32A07C_IRQHandler       
    def_irq_handler     INTT32A07CCAP0_IRQHandler   
    def_irq_handler     INTT32A07CCAP1_IRQHandler   
    def_irq_handler     INTPARI_IRQHandler          
    def_irq_handler     INTDMAATC_IRQHandler        
    def_irq_handler     INTDMAAERR_IRQHandler       
    def_irq_handler     INTDMABTC_IRQHandler        
    def_irq_handler     INTDMABERR_IRQHandler       
    def_irq_handler     INTRTC_IRQHandler           
    def_irq_handler     INTRMC0_IRQHandler          
    def_irq_handler     INTFLCRDY_IRQHandler        
    def_irq_handler     INTFLDRDY_IRQHandler        
              
    .end 
