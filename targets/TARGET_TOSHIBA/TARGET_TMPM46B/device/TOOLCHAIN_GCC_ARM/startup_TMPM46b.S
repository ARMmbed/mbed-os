/**
 *******************************************************************************
 * @file    startup_TMPM46b.s
 * @brief   CMSIS Cortex-M4F Core Device Startup File for the
 *          TOSHIBA 'TMPM46B' Device Series
 * @version V5.00
 * @date    2016/03/02
 *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
    .long     INT0_IRQHandler           // 0:  Interrupt pin 0
    .long     INT1_IRQHandler           // 1:  Interrupt pin 1
    .long     INT2_IRQHandler           // 2:  Interrupt pin 2
    .long     INT3_IRQHandler           // 3:  Interrupt pin 3
    .long     INT4_IRQHandler           // 4:  Interrupt pin 4
    .long     INT5_IRQHandler           // 5:  Interrupt pin 5
    .long     INT6_IRQHandler           // 6:  Interrupt pin 6
    .long     INT7_IRQHandler           // 7:  Interrupt pin 7
    .long     INT8_IRQHandler           // 8:  Interrupt pin 8
    .long     INT9_IRQHandler           // 9:  Interrupt pin 9
    .long     INTA_IRQHandler           // 10: Interrupt pin A
    .long     INTB_IRQHandler           // 11: Interrupt pin B
    .long     INTC_IRQHandler           // 12: Interrupt pin C
    .long     INTD_IRQHandler           // 13: Interrupt pin D
    .long     INTE_IRQHandler           // 14: Interrupt pin E
    .long     INTF_IRQHandler           // 15: Interrupt pin F
    .long     INTRX0_IRQHandler         // 16: Serial0 reception interrupt
    .long     INTTX0_IRQHandler         // 17: Serial0 transmission interrupt
    .long     INTRX1_IRQHandler         // 18: Serial1 reception interrupt
    .long     INTTX1_IRQHandler         // 19: Serial1 transmission interrupt
    .long     INTRX2_IRQHandler         // 20: Serial2 reception interrupt
    .long     INTTX2_IRQHandler         // 21:  Serial2 transmission interrupt
    .long     INTRX3_IRQHandler         // 22:  Serial3 reception interrupt
    .long     INTTX3_IRQHandler         // 23:  Serial3 transmission interrupt
    .long     INTUART0_IRQHandler       // 24:  Full UART0 transmission and reception interrupt
    .long     INTUART1_IRQHandler       // 25:  Full UART1 transmission and reception interrupt
    .long     INTI2C0_IRQHandler        // 26:  I2C0 transmission and reception interrupt
    .long     INTI2C1_IRQHandler        // 27:  I2C1 transmission and reception interrupt
    .long     INTI2C2_IRQHandler        // 28:  I2C2 transmission and reception interrupt
    .long     INTSSP0_IRQHandler        // 29:  SSP(SPI) Serial interface 0 interrupt
    .long     INTSSP1_IRQHandler        // 30:  SSP(SPI) Serial interface 1 interrupt
    .long     INTSSP2_IRQHandler        // 31:  SSP(SPI) Serial interface 2 interrupt
    .long     INTADHP_IRQHandler        // 32:  High Priority AD conversion interrupt
    .long     INTADM0_IRQHandler        // 33:  AD conversion monitor interrupt 0
    .long     INTADM1_IRQHandler        // 34:  AD conversion monitor interrupt 1
    .long     INTAD_IRQHandler          // 35:  AD conversion interrupt
    .long     INTAES_IRQHandler         // 36:  AES completion interrupt
    .long     INTSHA_IRQHandler         // 37:  SHA completion interrupt
    .long     INTMLA_IRQHandler         // 38:  MLA completion interrupt
    .long     INTESG_IRQHandler         // 39:  ESG completion interrupt
    .long     INTSNFCSEQ_IRQHandler     // 40:  SNFC command sequence end interrupt
    .long     INTSNFCPRTAE_IRQHandler   // 41:  SNFC page lead RAM transfer end interrupt
    .long     INTSNFCPRTCE_IRQHandler   // 42:  SNFC decode data RAM transmission end interrupt
    .long     INTSNFCFAIL_IRQHandler    // 43:  SNFC decode fail interrupt
    .long     0                         // 44:  Reserved1
    .long     0                         // 45:  Reserved2
    .long     0                         // 46:  Reserved3
    .long     INTMTEMG0_IRQHandler      // 47:  MPT0 EMG interrupt
    .long     INTMTPTB00_IRQHandler     // 48:  MPT0 compare match0/overflow,IGBT cycle interrupt
    .long     INTMTPTB01_IRQHandler     // 49:  MPT0 compare match1/overflow,IGBT cycle interrupt
    .long     INTMTCAP00_IRQHandler     // 50:  MPT0 input capture0 interrupt
    .long     INTMTCAP01_IRQHandler     // 51:  MPT0 input capture1 interrupt
    .long     INTMTEMG1_IRQHandler      // 52:  MPT1 EMG interrupt
    .long     INTMTPTB10_IRQHandler     // 53:  MPT1 compare match0/overflow,IGBT cycle interrupt
    .long     INTMTPTB11_IRQHandler     // 54:  MPT1 compare match1/overflow,IGBT cycle interrupt
    .long     INTMTCAP10_IRQHandler     // 55:  MPT1 input capture0 interrupt
    .long     INTMTCAP11_IRQHandler     // 56:  MPT1 input capture1 interrupt
    .long     INTMTEMG2_IRQHandler      // 57:  MPT2 EMG interrupt
    .long     INTMTPTB20_IRQHandler     // 58:  MPT2 compare match0/overflow,IGBT cycle interrupt
    .long     INTMTTTB21_IRQHandler     // 59:  MPT2 compare match1/overflow,IGBT cycle interrupt
    .long     INTMTCAP20_IRQHandler     // 60:  MPT2 input capture0 interrupt
    .long     INTMTCAP21_IRQHandler     // 61:  MPT2 input capture1 interrupt
    .long     INTMTEMG3_IRQHandler      // 62:  MPT3 EMG interrupt
    .long     INTMTPTB30_IRQHandler     // 63:  MPT3 compare match0/overflow,IGBT cycle interrupt
    .long     INTMTTTB31_IRQHandler     // 64:  MPT3 compare match1/overflow,IGBT cycle interrupt
    .long     INTMTCAP30_IRQHandler     // 65:  MPT3 input capture0 interrupt
    .long     INTMTCAP31_IRQHandler     // 66:  MPT3 input capture1 interrupt
    .long     INTTB0_IRQHandler         // 67:  TMRB0 compare match detection interrupt
    .long     INTCAP00_IRQHandler       // 68:  TMRB0 input capture 0 interrupt
    .long     INTCAP01_IRQHandler       // 69:  TMRB0 input capture 1 interrupt
    .long     INTTB1_IRQHandler         // 70:  TMRB1 compare match detection interrupt
    .long     INTCAP10_IRQHandler       // 71:  TMRB1 input capture 0 interrupt
    .long     INTCAP11_IRQHandler       // 72:  TMRB1 input capture 1 interrupt
    .long     INTTB2_IRQHandler         // 73:  TMRB2 compare match detection interrupt
    .long     INTCAP20_IRQHandler       // 74:  TMRB2 input capture 0 interrupt
    .long     INTCAP21_IRQHandler       // 75:  TMRB2 input capture 1 interrupt
    .long     INTTB3_IRQHandler         // 76:  TMRB3 compare match detection interrupt
    .long     INTCAP30_IRQHandler       // 77:  TMRB3 input capture 0 interrupt
    .long     INTCAP31_IRQHandler       // 78:  TMRB3 input capture 1 interrupt
    .long     INTTB4_IRQHandler         // 79:  TMRB4 compare match detection interrupt
    .long     INTCAP40_IRQHandler       // 80:  TMRB4 input capture 0 interrupt
    .long     INTCAP41_IRQHandler       // 81:  TMRB4 input capture 1 interrupt
    .long     INTTB5_IRQHandler         // 82:  TMRB5 compare match detection interrupt
    .long     INTCAP50_IRQHandler       // 83:  TMRB5 input capture 0 interrupt
    .long     INTCAP51_IRQHandler       // 84:  TMRB5 input capture 1 interrupt
    .long     INTTB6_IRQHandler         // 85:  TMRB6 compare match detection interrupt
    .long     INTCAP60_IRQHandler       // 86:  TMRB6 input capture 0 interrupt
    .long     INTCAP61_IRQHandler       // 87:  TMRB6 input capture 1 interrupt
    .long     INTTB7_IRQHandler         // 88:  TMRB7 compare match detection interrupt
    .long     INTCAP70_IRQHandler       // 89:  TMRB7 input capture 0 interrupt
    .long     INTCAP71_IRQHandler       // 90:  TMRB7 input capture 1 interrupt
    .long     INTRTC_IRQHandler         // 91:  Real time clock interrupt
    .long     INTDMAA_IRQHandler        // 92:  DMAC unitA transmission completion interrupt(ch4-31)
    .long     INTDMAB_IRQHandler        // 93:  DMAC unitB transmission completion interrupt(ch24-31)
    .long     INTDMAC_IRQHandler        // 94:  DMAC unitC transmission completion interrupt(ch12-31)
    .long     INTDMACTC8_IRQHandler     // 95:  DMAC unitC transmission completion interrupt(ch8)
    .long     INTDMACTC9_IRQHandler     // 96:  DMAC unitC transmission completion interrupt(ch9)
    .long     INTDMACTC10_IRQHandler    // 97:  DMAC unitC transmission completion interrupt(ch10)
    .long     INTDMACTC11_IRQHandler    // 98:  DMAC unitC transmission completion interrupt(ch11)
    .long     INTDMAAERR_IRQHandler     // 99:  DMAC transmission error interrupt(unitA)
    .long     INTDMABERR_IRQHandler     // 100: DMAC transmission error interrupt(unitB)
    .long     INTDMACERR_IRQHandler     // 101: DMAC transmission error interrupt(unitC)
    .long     INTFLRDY_IRQHandler       // 102: Flash Ready interrupt

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

    def_irq_handler    INT0_IRQHandler
    def_irq_handler    INT1_IRQHandler
    def_irq_handler    INT2_IRQHandler
    def_irq_handler    INT3_IRQHandler
    def_irq_handler    INT4_IRQHandler
    def_irq_handler    INT5_IRQHandler
    def_irq_handler    INT6_IRQHandler
    def_irq_handler    INT7_IRQHandler
    def_irq_handler    INT8_IRQHandler
    def_irq_handler    INT9_IRQHandler
    def_irq_handler    INTA_IRQHandler
    def_irq_handler    INTB_IRQHandler
    def_irq_handler    INTC_IRQHandler
    def_irq_handler    INTD_IRQHandler
    def_irq_handler    INTE_IRQHandler
    def_irq_handler    INTF_IRQHandler
    def_irq_handler    INTRX0_IRQHandler
    def_irq_handler    INTTX0_IRQHandler
    def_irq_handler    INTRX1_IRQHandler
    def_irq_handler    INTTX1_IRQHandler
    def_irq_handler    INTRX2_IRQHandler
    def_irq_handler    INTTX2_IRQHandler
    def_irq_handler    INTRX3_IRQHandler
    def_irq_handler    INTTX3_IRQHandler
    def_irq_handler    INTUART0_IRQHandler
    def_irq_handler    INTUART1_IRQHandler
    def_irq_handler    INTI2C0_IRQHandler
    def_irq_handler    INTI2C1_IRQHandler
    def_irq_handler    INTI2C2_IRQHandler
    def_irq_handler    INTSSP0_IRQHandler
    def_irq_handler    INTSSP1_IRQHandler
    def_irq_handler    INTSSP2_IRQHandler
    def_irq_handler    INTADHP_IRQHandler
    def_irq_handler    INTADM0_IRQHandler
    def_irq_handler    INTADM1_IRQHandler
    def_irq_handler    INTAD_IRQHandler
    def_irq_handler    INTAES_IRQHandler
    def_irq_handler    INTSHA_IRQHandler
    def_irq_handler    INTMLA_IRQHandler
    def_irq_handler    INTESG_IRQHandler
    def_irq_handler    INTSNFCSEQ_IRQHandler
    def_irq_handler    INTSNFCPRTAE_IRQHandler
    def_irq_handler    INTSNFCPRTCE_IRQHandler
    def_irq_handler    INTSNFCFAIL_IRQHandler
    def_irq_handler    INTMTEMG0_IRQHandler
    def_irq_handler    INTMTPTB00_IRQHandler
    def_irq_handler    INTMTPTB01_IRQHandler
    def_irq_handler    INTMTCAP00_IRQHandler
    def_irq_handler    INTMTCAP01_IRQHandler
    def_irq_handler    INTMTEMG1_IRQHandler
    def_irq_handler    INTMTPTB10_IRQHandler
    def_irq_handler    INTMTPTB11_IRQHandler
    def_irq_handler    INTMTCAP10_IRQHandler
    def_irq_handler    INTMTCAP11_IRQHandler
    def_irq_handler    INTMTEMG2_IRQHandler
    def_irq_handler    INTMTPTB20_IRQHandler
    def_irq_handler    INTMTTTB21_IRQHandler
    def_irq_handler    INTMTCAP20_IRQHandler
    def_irq_handler    INTMTCAP21_IRQHandler
    def_irq_handler    INTMTEMG3_IRQHandler
    def_irq_handler    INTMTPTB30_IRQHandler
    def_irq_handler    INTMTTTB31_IRQHandler
    def_irq_handler    INTMTCAP30_IRQHandler
    def_irq_handler    INTMTCAP31_IRQHandler
    def_irq_handler    INTTB0_IRQHandler
    def_irq_handler    INTCAP00_IRQHandler
    def_irq_handler    INTCAP01_IRQHandler
    def_irq_handler    INTTB1_IRQHandler
    def_irq_handler    INTCAP10_IRQHandler
    def_irq_handler    INTCAP11_IRQHandler
    def_irq_handler    INTTB2_IRQHandler
    def_irq_handler    INTCAP20_IRQHandler
    def_irq_handler    INTCAP21_IRQHandler
    def_irq_handler    INTTB3_IRQHandler
    def_irq_handler    INTCAP30_IRQHandler
    def_irq_handler    INTCAP31_IRQHandler
    def_irq_handler    INTTB4_IRQHandler
    def_irq_handler    INTCAP40_IRQHandler
    def_irq_handler    INTCAP41_IRQHandler
    def_irq_handler    INTTB5_IRQHandler
    def_irq_handler    INTCAP50_IRQHandler
    def_irq_handler    INTCAP51_IRQHandler
    def_irq_handler    INTTB6_IRQHandler
    def_irq_handler    INTCAP60_IRQHandler
    def_irq_handler    INTCAP61_IRQHandler
    def_irq_handler    INTTB7_IRQHandler
    def_irq_handler    INTCAP70_IRQHandler
    def_irq_handler    INTCAP71_IRQHandler
    def_irq_handler    INTRTC_IRQHandler
    def_irq_handler    INTDMAA_IRQHandler
    def_irq_handler    INTDMAB_IRQHandler
    def_irq_handler    INTDMAC_IRQHandler
    def_irq_handler    INTDMACTC8_IRQHandler
    def_irq_handler    INTDMACTC9_IRQHandler
    def_irq_handler    INTDMACTC10_IRQHandler
    def_irq_handler    INTDMACTC11_IRQHandler
    def_irq_handler    INTDMAAERR_IRQHandler
    def_irq_handler    INTDMABERR_IRQHandler
    def_irq_handler    INTDMACERR_IRQHandler
    def_irq_handler    INTFLRDY_IRQHandler

    .end
