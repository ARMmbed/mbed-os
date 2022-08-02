/**
 *******************************************************************************
 * @file    startup_TMPM4KNA.s
 * @brief   CMSIS Cortex-M4F Core Device Startup File for the
 *          TOSHIBA 'TMPM4KNA' Device Series
 * @version 
 * @date    
 *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
 *  DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2021 All rights reserved
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

    // External Interrupts
    .long   INT00_IRQHandler          // 0:	Interrupt pin 00
    .long   INT01_IRQHandler          // 1:	Interrupt pin 01a/00b
    .long   INT02_IRQHandler          // 2:	Interrupt pin 02a/00b
    .long   INT03_IRQHandler          // 3:	Interrupt pin 03a/03b
    .long   INT04_IRQHandler          // 4:	Interrupt pin 04a/04b
    .long   INT05_IRQHandler          // 5:	Interrupt pin 05a/05b
    .long   INT06_IRQHandler          // 6:	Interrupt pin 06a/06b
    .long   INT07_IRQHandler          // 7:	Interrupt pin 07a/07b
    .long   INT08_IRQHandler          // 8:	Interrupt pin 08a/08b
    .long   INT09_IRQHandler          // 9:	Interrupt pin 09
    .long   INT10_IRQHandler          // 10:	Interrupt pin 10
    .long   INT11_IRQHandler          // 11:	Interrupt pin 11a/11b
    .long   INT12_IRQHandler          // 12:	Interrupt pin 12
    .long   INT13_IRQHandler          // 13:	Interrupt pin 13
    .long   INT14_IRQHandler          // 14:	Interrupt pin 14a/14b
    .long   INT15_IRQHandler          // 15:	Interrupt pin 15
    .long   INT16_IRQHandler          // 16:	Interrupt pin 16a/16b
    .long   INT17_IRQHandler          // 17:	Interrupt pin 17a/17b
    .long   INT18_IRQHandler          // 18:	Interrupt pin 18a/18b
    .long   0                         // 19:	Reserved
    .long   0                         // 20:	Reserved
    .long   INT21_IRQHandler          // 21:	Interrupt pin 21
    .long   INTVCN0_IRQHandler        // 22:	A-VE+ ch0 Interrupt
    .long   INTVCT0_IRQHandler        // 23:	A-VE+ ch0 Task termination interrupt
    .long   INTEMG0_IRQHandler        // 24:	A-PMD ch0 EMG interrupt
    .long   INTEMG1_IRQHandler        // 25:	A-PMD ch1 EMG interrupt
    .long   INTEMG2_IRQHandler        // 26:	A-PMD ch2 EMG interrupt
    .long   INTOVV0_IRQHandler        // 27:	A-PMD ch0 OVV interrupt
    .long   INTOVV1_IRQHandler        // 28:	A-PMD ch1 OVV interrupt
    .long   INTOVV2_IRQHandler        // 29:	A-PMD ch2 OVV interrupt
    .long   INTPWM0_IRQHandler        // 30:	A-PMD ch0 PWM interrupt
    .long   INTPWM1_IRQHandler        // 31:	A-PMD ch1 PWM interrupt
    .long   INTPWM2_IRQHandler        // 32:	A-PMD ch2 PWM interrupt
    .long   INTENC00_IRQHandler       // 33:	A-ENC32 ch0 Encoder interrupt 0
    .long   INTENC01_IRQHandler       // 34:	A-ENC32 ch0 Encoder interrupt 1
    .long   INTENC10_IRQHandler       // 35:	A-ENC32 ch1 Encoder interrupt 0
    .long   INTENC11_IRQHandler       // 36:	A-ENC32 ch1 Encoder interrupt 1
    .long   INTENC20_IRQHandler       // 37:	A-ENC32 ch2 Encoder interrupt 0
    .long   INTENC21_IRQHandler       // 38:	A-ENC32 ch2 Encoder interrupt 1
    .long   INTADAPDA_IRQHandler      // 39:	ADC unit A PMD trigger program interrupt A
    .long   INTADAPDB_IRQHandler      // 40:	ADC unit A PMD trigger program interrupt B
    .long   INTADACP0_IRQHandler      // 41:	ADC unit A Monitor function 0 interrupt
    .long   INTADACP1_IRQHandler      // 42:	ADC unit A Monitor function 1 interrupt
    .long   INTADATRG_IRQHandler      // 43:	ADC unit A General purpose trigger program interrupt
    .long   INTADASGL_IRQHandler      // 44:	ADC unit A Single program interrupt
    .long   INTADACNT_IRQHandler      // 45:	ADC unit A Continuity program interrupt
    .long   INTADBPDA_IRQHandler      // 46:	ADC unit B PMD trigger program interrupt A
    .long   INTADBPDB_IRQHandler      // 47:	ADC unit B PMD trigger program interrupt B
    .long   INTADBCP0_IRQHandler      // 48:	ADC unit B Monitor function 0 interrupt
    .long   INTADBCP1_IRQHandler      // 49:	ADC unit B Monitor function 1 interrupt
    .long   INTADBTRG_IRQHandler      // 50:	ADC unit B General purpose trigger program interrupt
    .long   INTADBSGL_IRQHandler      // 51:	ADC unit B Single program interrupt
    .long   INTADBCNT_IRQHandler      // 52:	ADC unit B Continuity program interrupt
    .long   INTADCPDA_IRQHandler      // 53:	ADC unit C PMD trigger program interrupt A
    .long   INTADCPDB_IRQHandler      // 54:	ADC unit C PMD trigger program interrupt B
    .long   INTADCCP0_IRQHandler      // 55:	ADC unit C Monitor function 0 interrupt
    .long   INTADCCP1_IRQHandler      // 56:	ADC unit C Monitor function 1 interrupt
    .long   INTADCTRG_IRQHandler      // 57:	ADC unit C General purpose trigger program interrupt
    .long   INTADCSGL_IRQHandler      // 58:	ADC unit C Single program interrupt
    .long   INTADCCNT_IRQHandler      // 59:	ADC unit C Continuity program interrupt
    .long   INTSC0RX_IRQHandler       // 60:	TSPI/UART ch0 Reception interrupt
    .long   INTSC0TX_IRQHandler       // 61:	TSPI/UART ch0 Transmit interrupt
    .long   INTSC0ERR_IRQHandler      // 62:	TSPI/UART ch0 Error interrupt
    .long   INTSC1RX_IRQHandler       // 63:	TSPI/UART ch1 Reception interrupt
    .long   INTSC1TX_IRQHandler       // 64:	TSPI/UART ch1 Transmit interrupt
    .long   INTSC1ERR_IRQHandler      // 65:	TSPI/UART ch1 Error interrupt
    .long   INTSC2RX_IRQHandler       // 66:	UART ch2 Reception interrupt
    .long   INTSC2TX_IRQHandler       // 67:	UART ch2 Transmit interrupt
    .long   INTSC2ERR_IRQHandler      // 68:	UART ch2 Error interrupt
    .long   INTSC3RX_IRQHandler       // 69:	UART ch3 Reception interrupt
    .long   INTSC3TX_IRQHandler       // 70:	UART ch3 Transmit interrupt
    .long   INTSC3ERR_IRQHandler      // 71:	UART ch3 Error interrupt
    .long   INTI2C0NST_IRQHandler     // 72:	I2C ch0 Interrupt / EI2C ch0 Status interrupt
    .long   INTI2C0ATX_IRQHandler     // 73:	I2C ch0 Arbitration lost detection interrupt / EI2C ch0 Transmission buffer empty interrupt
    .long   INTI2C0BRX_IRQHandler     // 74:	I2C ch0 Bus free detection interrupt / EI2C ch0 Reception buffer full interrupt
    .long   INTI2C0NA_IRQHandler      // 75:	I2C ch0 NACK detection interrupt
    .long   INTI2C1NST_IRQHandler     // 76:	I2C ch1 Interrupt / EI2C ch1 Status interrupt
    .long   INTI2C1ATX_IRQHandler     // 77:	I2C ch1 Arbitration lost detection interrupt / EI2C ch1 Transmission buffer empty interrupt
    .long   INTI2C1BRX_IRQHandler     // 78:	I2C ch1 Bus free detection interrupt / EI2C ch1 Reception buffer full interrupt
    .long   INTI2C1NA_IRQHandler      // 79:	I2C ch1 NACK detection interrupt
    .long   0                         // 80:	Reserved
    .long   0                         // 81:	Reserved
    .long   0                         // 82:	Reserved
    .long   INTT32A00AC_IRQHandler    // 83:	T32A ch0 Timer A/C Compare match detection / Over flow / Under flow
    .long   INTT32A00ACCAP0_IRQHandler// 84:	T32A ch0 Timer A/C Input capture 0
    .long   INTT32A00ACCAP1_IRQHandler// 85:	T32A ch0 Timer A/C Input capture 1
    .long   INTT32A00B_IRQHandler     // 86:	T32A ch0 Timer B Compare match detection / Over flow / Under flow
    .long   INTT32A00BCAP0_IRQHandler // 87:	T32A ch0 Timer B Input capture 0
    .long   INTT32A00BCAP1_IRQHandler // 88:	T32A ch0 Timer B Input capture 1
    .long   INTT32A01AC_IRQHandler    // 89:	T32A ch1 Timer A/C Compare match detection / Over flow / Under flow
    .long   INTT32A01ACCAP0_IRQHandler// 90:	T32A ch1 Timer A/C Input capture 0
    .long   INTT32A01ACCAP1_IRQHandler// 91:	T32A ch1 Timer A/C Input capture 1
    .long   INTT32A01B_IRQHandler     // 92:	T32A ch1 Timer B Compare match detection / Over flow / Under flow
    .long   INTT32A01BCAP0_IRQHandler // 93:	T32A ch1 Timer B Input capture 0
    .long   INTT32A01BCAP1_IRQHandler // 94:	T32A ch1 Timer B Input capture 1
    .long   INTT32A02AC_IRQHandler    // 95:	T32A ch2 Timer A/C Compare match detection / Over flow / Under flow
    .long   INTT32A02ACCAP0_IRQHandler// 96:	T32A ch2 Timer A/C Input capture 0
    .long   INTT32A02ACCAP1_IRQHandler// 97:	T32A ch2 Timer A/C Input capture 1
    .long   INTT32A02B_IRQHandler     // 98:	T32A ch2 Timer B Compare match detection / Over flow / Under flow
    .long   INTT32A02BCAP0_IRQHandler // 99:	T32A ch2 Timer B Input capture 0
    .long   INTT32A02BCAP1_IRQHandler // 100:	T32A ch2 Timer B Input capture 1
    .long   INTT32A03AC_IRQHandler    // 101:	T32A ch3 Timer A/C Compare match detection / Over flow / Under flow
    .long   INTT32A03ACCAP0_IRQHandler// 102:	T32A ch3 Timer A/C Input capture 0
    .long   INTT32A03ACCAP1_IRQHandler// 103:	T32A ch3 Timer A/C Input capture 1
    .long   INTT32A03B_IRQHandler     // 104:	T32A ch3 Timer B Compare match detection / Over flow / Under flow
    .long   INTT32A03BCAP0_IRQHandler // 105:	T32A ch3 Timer B Input capture 0
    .long   INTT32A03BCAP1_IRQHandler // 106:	T32A ch3 Timer B Input capture 1
    .long   INTT32A04AC_IRQHandler    // 107:	T32A ch4 Timer A/C Compare match detection / Over flow / Under flow
    .long   INTT32A04ACCAP0_IRQHandler// 108:	T32A ch4 Timer A/C Input capture 0
    .long   INTT32A04ACCAP1_IRQHandler// 109:	T32A ch4 Timer A/C Input capture 1
    .long   INTT32A04B_IRQHandler     // 110:	T32A ch4 Timer B Compare match detection / Over flow / Under flow
    .long   INTT32A04BCAP0_IRQHandler // 111:	T32A ch4 Timer B Input capture 0
    .long   INTT32A04BCAP1_IRQHandler // 112:	T32A ch4 Timer B Input capture 1
    .long   INTT32A05AC_IRQHandler    // 113:	T32A ch5 Timer A/C Compare match detection / Over flow / Under flow
    .long   INTT32A05ACCAP0_IRQHandler// 114:	T32A ch5 Timer A/C Input capture 0
    .long   INTT32A05ACCAP1_IRQHandler// 115:	T32A ch5 Timer A/C Input capture 1
    .long   INTT32A05B_IRQHandler     // 116:	T32A ch5 Timer B Compare match detection / Over flow / Under flow
    .long   INTT32A05BCAP0_IRQHandler // 117:	T32A ch5 Timer B Input capture 0
    .long   INTT32A05BCAP1_IRQHandler // 118:	T32A ch5 Timer B Input capture 1
    .long   INTPARI0_IRQHandler       // 119:	RAMP ch0 Parity error interrupt
    .long   INTPARI1_IRQHandler       // 120:	RAMP ch1 Parity error interrupt
    .long   INTDMAATC_IRQHandler      // 121:	DMAC unit A End of transfer (ch0 - 31)
    .long   INTDMAAERR_IRQHandler     // 122:	DMAC unit A Transfer error
    .long   INTFLCRDY_IRQHandler      // 123:	Code FLASH Ready interrupt
    .long   INTFLDRDY_IRQHandler      // 124:	Data FLASH Ready interrupt
	
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

    def_irq_handler    INT00_IRQHandler
    def_irq_handler    INT01_IRQHandler
    def_irq_handler    INT02_IRQHandler
    def_irq_handler    INT03_IRQHandler
    def_irq_handler    INT04_IRQHandler
    def_irq_handler    INT05_IRQHandler
    def_irq_handler    INT06_IRQHandler
    def_irq_handler    INT07_IRQHandler
    def_irq_handler    INT08_IRQHandler
    def_irq_handler    INT09_IRQHandler
    def_irq_handler    INT10_IRQHandler
    def_irq_handler    INT11_IRQHandler
    def_irq_handler    INT12_IRQHandler
    def_irq_handler    INT13_IRQHandler
    def_irq_handler    INT14_IRQHandler
    def_irq_handler    INT15_IRQHandler
    def_irq_handler    INT16_IRQHandler
    def_irq_handler    INT17_IRQHandler
    def_irq_handler    INT18_IRQHandler
    def_irq_handler    INT21_IRQHandler
    def_irq_handler    INTVCN0_IRQHandler
    def_irq_handler    INTVCT0_IRQHandler
    def_irq_handler    INTEMG0_IRQHandler
    def_irq_handler    INTEMG1_IRQHandler
    def_irq_handler    INTEMG2_IRQHandler
    def_irq_handler    INTOVV0_IRQHandler
    def_irq_handler    INTOVV1_IRQHandler
    def_irq_handler    INTOVV2_IRQHandler
    def_irq_handler    INTPWM0_IRQHandler
    def_irq_handler    INTPWM1_IRQHandler
    def_irq_handler    INTPWM2_IRQHandler
    def_irq_handler    INTENC00_IRQHandler
    def_irq_handler    INTENC01_IRQHandler
    def_irq_handler    INTENC10_IRQHandler
    def_irq_handler    INTENC11_IRQHandler
    def_irq_handler    INTENC20_IRQHandler
    def_irq_handler    INTENC21_IRQHandler
    def_irq_handler    INTADAPDA_IRQHandler
    def_irq_handler    INTADAPDB_IRQHandler
    def_irq_handler    INTADACP0_IRQHandler
    def_irq_handler    INTADACP1_IRQHandler
    def_irq_handler    INTADATRG_IRQHandler
    def_irq_handler    INTADASGL_IRQHandler
    def_irq_handler    INTADACNT_IRQHandler
    def_irq_handler    INTADBPDA_IRQHandler
    def_irq_handler    INTADBPDB_IRQHandler
    def_irq_handler    INTADBCP0_IRQHandler
    def_irq_handler    INTADBCP1_IRQHandler
    def_irq_handler    INTADBTRG_IRQHandler
    def_irq_handler    INTADBSGL_IRQHandler
    def_irq_handler    INTADBCNT_IRQHandler
    def_irq_handler    INTADCPDA_IRQHandler
    def_irq_handler    INTADCPDB_IRQHandler
    def_irq_handler    INTADCCP0_IRQHandler
    def_irq_handler    INTADCCP1_IRQHandler
    def_irq_handler    INTADCTRG_IRQHandler
    def_irq_handler    INTADCSGL_IRQHandler
    def_irq_handler    INTADCCNT_IRQHandler
    def_irq_handler    INTSC0RX_IRQHandler
    def_irq_handler    INTSC0TX_IRQHandler
    def_irq_handler    INTSC0ERR_IRQHandler
    def_irq_handler    INTSC1RX_IRQHandler
    def_irq_handler    INTSC1TX_IRQHandler
    def_irq_handler    INTSC1ERR_IRQHandler
    def_irq_handler    INTSC2RX_IRQHandler
    def_irq_handler    INTSC2TX_IRQHandler
    def_irq_handler    INTSC2ERR_IRQHandler
    def_irq_handler    INTSC3RX_IRQHandler
    def_irq_handler    INTSC3TX_IRQHandler
    def_irq_handler    INTSC3ERR_IRQHandler
    def_irq_handler    INTI2C0NST_IRQHandler
    def_irq_handler    INTI2C0ATX_IRQHandler
    def_irq_handler    INTI2C0BRX_IRQHandler
    def_irq_handler    INTI2C0NA_IRQHandler
    def_irq_handler    INTI2C1NST_IRQHandler
    def_irq_handler    INTI2C1ATX_IRQHandler
    def_irq_handler    INTI2C1BRX_IRQHandler
    def_irq_handler    INTI2C1NA_IRQHandler
    def_irq_handler    INTT32A00AC_IRQHandler
    def_irq_handler    INTT32A00ACCAP0_IRQHandler
    def_irq_handler    INTT32A00ACCAP1_IRQHandler
    def_irq_handler    INTT32A00B_IRQHandler
    def_irq_handler    INTT32A00BCAP0_IRQHandler
    def_irq_handler    INTT32A00BCAP1_IRQHandler
    def_irq_handler    INTT32A01AC_IRQHandler
    def_irq_handler    INTT32A01ACCAP0_IRQHandler
    def_irq_handler    INTT32A01ACCAP1_IRQHandler
    def_irq_handler    INTT32A01B_IRQHandler
    def_irq_handler    INTT32A01BCAP0_IRQHandler
    def_irq_handler    INTT32A01BCAP1_IRQHandler
    def_irq_handler    INTT32A02AC_IRQHandler
    def_irq_handler    INTT32A02ACCAP0_IRQHandler
    def_irq_handler    INTT32A02ACCAP1_IRQHandler
    def_irq_handler    INTT32A02B_IRQHandler
    def_irq_handler    INTT32A02BCAP0_IRQHandler
    def_irq_handler    INTT32A02BCAP1_IRQHandler
    def_irq_handler    INTT32A03AC_IRQHandler
    def_irq_handler    INTT32A03ACCAP0_IRQHandler
    def_irq_handler    INTT32A03ACCAP1_IRQHandler
    def_irq_handler    INTT32A03B_IRQHandler
    def_irq_handler    INTT32A03BCAP0_IRQHandler
    def_irq_handler    INTT32A03BCAP1_IRQHandler
    def_irq_handler    INTT32A04AC_IRQHandler
    def_irq_handler    INTT32A04ACCAP0_IRQHandler
    def_irq_handler    INTT32A04ACCAP1_IRQHandler
    def_irq_handler    INTT32A04B_IRQHandler
    def_irq_handler    INTT32A04BCAP0_IRQHandler
    def_irq_handler    INTT32A04BCAP1_IRQHandler
    def_irq_handler    INTT32A05AC_IRQHandler
    def_irq_handler    INTT32A05ACCAP0_IRQHandler
    def_irq_handler    INTT32A05ACCAP1_IRQHandler
    def_irq_handler    INTT32A05B_IRQHandler
    def_irq_handler    INTT32A05BCAP0_IRQHandler
    def_irq_handler    INTT32A05BCAP1_IRQHandler
    def_irq_handler    INTPARI0_IRQHandler
    def_irq_handler    INTPARI1_IRQHandler
    def_irq_handler    INTDMAATC_IRQHandler
    def_irq_handler    INTDMAAERR_IRQHandler
    def_irq_handler    INTFLCRDY_IRQHandler
    def_irq_handler    INTFLDRDY_IRQHandler
	
    .end
