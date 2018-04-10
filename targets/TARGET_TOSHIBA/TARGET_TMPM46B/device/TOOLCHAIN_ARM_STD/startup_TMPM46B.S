;/**
; *******************************************************************************
; * @file    startup_TMPM46B.s
; * @brief   CMSIS Cortex-M4 Core Device Startup File for the
; *          TOSHIBA 'TMPM46B' Device Series
; * @version V2.0.2.4
; * @date    2015/03/31
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; * 
; * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
; * 
; * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
; *******************************************************************************
; */

__initial_sp    EQU     0x20080000


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors   DCD __initial_sp              ; Top of Stack
            DCD Reset_Handler             ; Reset Handler
            DCD NMI_Handler               ; NMI Handler
            DCD HardFault_Handler         ; Hard Fault Handler
            DCD MemManage_Handler         ; MPU Fault Handler
            DCD BusFault_Handler          ; Bus Fault Handler
            DCD UsageFault_Handler        ; Usage Fault Handler
            DCD 0                         ; Reserved
            DCD 0                         ; Reserved
            DCD 0                         ; Reserved
            DCD 0                         ; Reserved
            DCD SVC_Handler               ; SVCall Handler
            DCD DebugMon_Handler          ; Debug Monitor Handler
            DCD 0                         ; Reserved
            DCD PendSV_Handler            ; PendSV Handler
            DCD SysTick_Handler           ; SysTick Handler

            ; External Interrupts
            DCD INT0_IRQHandler           ; 0: Interrupt pin 0
            DCD INT1_IRQHandler           ; 1: Interrupt pin 1
            DCD INT2_IRQHandler           ; 2: Interrupt pin 2
            DCD INT3_IRQHandler           ; 3: Interrupt pin 3
            DCD INT4_IRQHandler           ; 4: Interrupt pin 4
            DCD INT5_IRQHandler           ; 5: Interrupt pin 5
            DCD INT6_IRQHandler           ; 6: Interrupt pin 6
            DCD INT7_IRQHandler           ; 7: Interrupt pin 7
            DCD INT8_IRQHandler           ; 8: Interrupt pin 8
            DCD INT9_IRQHandler           ; 9: Interrupt pin 9
            DCD INTA_IRQHandler           ; 10: Interrupt pin A
            DCD INTB_IRQHandler           ; 11: Interrupt pin B
            DCD INTC_IRQHandler           ; 12: Interrupt pin C
            DCD INTD_IRQHandler           ; 13: Interrupt pin D
            DCD INTE_IRQHandler           ; 14: Interrupt pin E
            DCD INTF_IRQHandler           ; 15: Interrupt pin F
            DCD INTRX0_IRQHandler         ; 16: Serial0 reception interrupt
            DCD INTTX0_IRQHandler         ; 17: Serial0 transmission interrupt
            DCD INTRX1_IRQHandler         ; 18: Serial1 reception interrupt
            DCD INTTX1_IRQHandler         ; 19: Serial1 transmission interrupt
            DCD INTRX2_IRQHandler         ; 20: Serial2 reception interrupt
            DCD INTTX2_IRQHandler         ; 21: Serial2 transmission interrupt
            DCD INTRX3_IRQHandler         ; 22: Serial3 reception interrupt
            DCD INTTX3_IRQHandler         ; 23: Serial3 transmission interrupt
            DCD INTUART0_IRQHandler       ; 24: Full UART0 transmission and reception interrupt
            DCD INTUART1_IRQHandler       ; 25: Full UART1 transmission and reception interrupt
            DCD INTI2C0_IRQHandler        ; 26: I2C0 transmission and reception interrupt
            DCD INTI2C1_IRQHandler        ; 27: I2C1 transmission and reception interrupt
            DCD INTI2C2_IRQHandler        ; 28: I2C2 transmission and reception interrupt
            DCD INTSSP0_IRQHandler        ; 29: SSP(SPI) Serial interface 0 interrupt 
            DCD INTSSP1_IRQHandler        ; 30: SSP(SPI) Serial interface 1 interrupt 
            DCD INTSSP2_IRQHandler        ; 31: SSP(SPI) Serial interface 2 interrupt 
            DCD INTADHP_IRQHandler        ; 32: High Priority AD conversion interrupt
            DCD INTADM0_IRQHandler        ; 33: AD conversion monitor interrupt 0
            DCD INTADM1_IRQHandler        ; 34: AD conversion monitor interrupt 1
            DCD INTAD_IRQHandler          ; 35: AD conversion interrupt 
            DCD INTAES_IRQHandler         ; 36: AES completion interrupt
            DCD INTSHA_IRQHandler         ; 37: SHA completion interrupt
            DCD INTMLA_IRQHandler         ; 38: MLA completion interrupt
            DCD INTESG_IRQHandler         ; 39: ESG completion interrupt
            DCD INTSNFCSEQ_IRQHandler     ; 40: SNFC command sequence end interrupt
            DCD INTSNFCPRTAE_IRQHandler   ; 41: SNFC page lead RAM transfer end interrupt
            DCD INTSNFCPRTCE_IRQHandler   ; 42: SNFC decode data RAM transmission end interrupt
            DCD INTSNFCFAIL_IRQHandler    ; 43: SNFC decode fail interrupt
            DCD 0                         ; 44: Reserved1
            DCD 0                         ; 45: Reserved2
            DCD 0                         ; 46: Reserved3
            DCD INTMTEMG0_IRQHandler      ; 47: MPT0 EMG interrupt
            DCD INTMTPTB00_IRQHandler     ; 48: MPT0 compare match0/overflow,IGBT cycle interrupt
            DCD INTMTPTB01_IRQHandler     ; 49: MPT0 compare match1/overflow,IGBT cycle interrupt
            DCD INTMTCAP00_IRQHandler     ; 50: MPT0 input capture0 interrupt
            DCD INTMTCAP01_IRQHandler     ; 51: MPT0 input capture1 interrupt
            DCD INTMTEMG1_IRQHandler      ; 52: MPT1 EMG interrupt
            DCD INTMTPTB10_IRQHandler     ; 53: MPT1 compare match0/overflow,IGBT cycle interrupt
            DCD INTMTPTB11_IRQHandler     ; 54: MPT1 compare match1/overflow,IGBT cycle interrupt
            DCD INTMTCAP10_IRQHandler     ; 55: MPT1 input capture0 interrupt
            DCD INTMTCAP11_IRQHandler     ; 56: MPT1 input capture1 interrupt
            DCD INTMTEMG2_IRQHandler      ; 57: MPT2 EMG interrupt
            DCD INTMTPTB20_IRQHandler     ; 58: MPT2 compare match0/overflow,IGBT cycle interrupt
            DCD INTMTTTB21_IRQHandler     ; 59: MPT2 compare match1/overflow,IGBT cycle interrupt
            DCD INTMTCAP20_IRQHandler     ; 60: MPT2 input capture0 interrupt
            DCD INTMTCAP21_IRQHandler     ; 61: MPT2 input capture1 interrupt
            DCD INTMTEMG3_IRQHandler      ; 62: MPT3 EMG interrupt
            DCD INTMTPTB30_IRQHandler     ; 63: MPT3 compare match0/overflow,IGBT cycle interrupt
            DCD INTMTTTB31_IRQHandler     ; 64: MPT3 compare match1/overflow,IGBT cycle interrupt
            DCD INTMTCAP30_IRQHandler     ; 65: MPT3 input capture0 interrupt
            DCD INTMTCAP31_IRQHandler     ; 66: MPT3 input capture1 interrupt
            DCD INTTB0_IRQHandler         ; 67: TMRB0 compare match detection interrupt
            DCD INTCAP00_IRQHandler       ; 68: TMRB0 input capture 0 interrupt
            DCD INTCAP01_IRQHandler       ; 69: TMRB0 input capture 1 interrupt
            DCD INTTB1_IRQHandler         ; 70: TMRB1 compare match detection interrupt
            DCD INTCAP10_IRQHandler       ; 71: TMRB1 input capture 0 interrupt
            DCD INTCAP11_IRQHandler       ; 72: TMRB1 input capture 1 interrupt
            DCD INTTB2_IRQHandler         ; 73: TMRB2 compare match detection interrupt
            DCD INTCAP20_IRQHandler       ; 74: TMRB2 input capture 0 interrupt
            DCD INTCAP21_IRQHandler       ; 75: TMRB2 input capture 1 interrupt
            DCD INTTB3_IRQHandler         ; 76: TMRB3 compare match detection interrupt
            DCD INTCAP30_IRQHandler       ; 77: TMRB3 input capture 0 interrupt
            DCD INTCAP31_IRQHandler       ; 78: TMRB3 input capture 1 interrupt
            DCD INTTB4_IRQHandler         ; 79: TMRB4 compare match detection interrupt
            DCD INTCAP40_IRQHandler       ; 80: TMRB4 input capture 0 interrupt
            DCD INTCAP41_IRQHandler       ; 81: TMRB4 input capture 1 interrupt
            DCD INTTB5_IRQHandler         ; 82: TMRB5 compare match detection interrupt
            DCD INTCAP50_IRQHandler       ; 83: TMRB5 input capture 0 interrupt
            DCD INTCAP51_IRQHandler       ; 84: TMRB5 input capture 1 interrupt
            DCD INTTB6_IRQHandler         ; 85: TMRB6 compare match detection interrupt
            DCD INTCAP60_IRQHandler       ; 86: TMRB6 input capture 0 interrupt
            DCD INTCAP61_IRQHandler       ; 87: TMRB6 input capture 1 interrupt
            DCD INTTB7_IRQHandler         ; 88: TMRB7 compare match detection interrupt
            DCD INTCAP70_IRQHandler       ; 89: TMRB7 input capture 0 interrupt
            DCD INTCAP71_IRQHandler       ; 90: TMRB7 input capture 1 interrupt
            DCD INTRTC_IRQHandler         ; 91: Real time clock interrupt
            DCD INTDMAA_IRQHandler        ; 92: DMAC unitA transmission completion interrupt(ch4-31)
            DCD INTDMAB_IRQHandler        ; 93: DMAC unitB transmission completion interrupt(ch24-31)
            DCD INTDMAC_IRQHandler        ; 94: DMAC unitC transmission completion interrupt(ch12-31)
            DCD INTDMACTC8_IRQHandler     ; 95: DMAC unitC transmission completion interrupt(ch8)
            DCD INTDMACTC9_IRQHandler     ; 96: DMAC unitC transmission completion interrupt(ch9)
            DCD INTDMACTC10_IRQHandler    ; 97: DMAC unitC transmission completion interrupt(ch10)
            DCD INTDMACTC11_IRQHandler    ; 98: DMAC unitC transmission completion interrupt(ch11)
            DCD INTDMAAERR_IRQHandler     ; 99: DMAC transmission error interrupt(unitA)
            DCD INTDMABERR_IRQHandler     ; 100: DMAC transmission error interrupt(unitB)
            DCD INTDMACERR_IRQHandler     ; 101: DMAC transmission error interrupt(unitC)
            DCD INTFLRDY_IRQHandler       ; 102: Flash Ready interrupt



                AREA    |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler           [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler             [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler       [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler       [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler        [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler      [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler             [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler        [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler          [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler         [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT INT0_IRQHandler           [WEAK]
                EXPORT INT1_IRQHandler           [WEAK]
                EXPORT INT2_IRQHandler           [WEAK]
                EXPORT INT3_IRQHandler           [WEAK]
                EXPORT INT4_IRQHandler           [WEAK]
                EXPORT INT5_IRQHandler           [WEAK]
                EXPORT INT6_IRQHandler           [WEAK]
                EXPORT INT7_IRQHandler           [WEAK]
                EXPORT INT8_IRQHandler           [WEAK]
                EXPORT INT9_IRQHandler           [WEAK]
                EXPORT INTA_IRQHandler           [WEAK]
                EXPORT INTB_IRQHandler           [WEAK]
                EXPORT INTC_IRQHandler           [WEAK]
                EXPORT INTD_IRQHandler           [WEAK]
                EXPORT INTE_IRQHandler           [WEAK]
                EXPORT INTF_IRQHandler           [WEAK]
                EXPORT INTRX0_IRQHandler         [WEAK]
                EXPORT INTTX0_IRQHandler         [WEAK]
                EXPORT INTRX1_IRQHandler         [WEAK]
                EXPORT INTTX1_IRQHandler         [WEAK]
                EXPORT INTRX2_IRQHandler         [WEAK]
                EXPORT INTTX2_IRQHandler         [WEAK]
                EXPORT INTRX3_IRQHandler         [WEAK]
                EXPORT INTTX3_IRQHandler         [WEAK]
                EXPORT INTUART0_IRQHandler       [WEAK]
                EXPORT INTUART1_IRQHandler       [WEAK]
                EXPORT INTI2C0_IRQHandler        [WEAK]
                EXPORT INTI2C1_IRQHandler        [WEAK]
                EXPORT INTI2C2_IRQHandler        [WEAK]
                EXPORT INTSSP0_IRQHandler        [WEAK]
                EXPORT INTSSP1_IRQHandler        [WEAK]
                EXPORT INTSSP2_IRQHandler        [WEAK]
                EXPORT INTADHP_IRQHandler        [WEAK]
                EXPORT INTADM0_IRQHandler        [WEAK]
                EXPORT INTADM1_IRQHandler        [WEAK]
                EXPORT INTAD_IRQHandler          [WEAK]
                EXPORT INTAES_IRQHandler         [WEAK]
                EXPORT INTSHA_IRQHandler         [WEAK]
                EXPORT INTMLA_IRQHandler         [WEAK]
                EXPORT INTESG_IRQHandler         [WEAK]
                EXPORT INTSNFCSEQ_IRQHandler     [WEAK]
                EXPORT INTSNFCPRTAE_IRQHandler   [WEAK]
                EXPORT INTSNFCPRTCE_IRQHandler   [WEAK]
                EXPORT INTSNFCFAIL_IRQHandler    [WEAK]
                EXPORT INTMTEMG0_IRQHandler      [WEAK]
                EXPORT INTMTPTB00_IRQHandler     [WEAK]
                EXPORT INTMTPTB01_IRQHandler     [WEAK]
                EXPORT INTMTCAP00_IRQHandler     [WEAK]
                EXPORT INTMTCAP01_IRQHandler     [WEAK]
                EXPORT INTMTEMG1_IRQHandler      [WEAK]
                EXPORT INTMTPTB10_IRQHandler     [WEAK]
                EXPORT INTMTPTB11_IRQHandler     [WEAK]
                EXPORT INTMTCAP10_IRQHandler     [WEAK]
                EXPORT INTMTCAP11_IRQHandler     [WEAK]
                EXPORT INTMTEMG2_IRQHandler      [WEAK]
                EXPORT INTMTPTB20_IRQHandler     [WEAK]
                EXPORT INTMTTTB21_IRQHandler     [WEAK]
                EXPORT INTMTCAP20_IRQHandler     [WEAK]
                EXPORT INTMTCAP21_IRQHandler     [WEAK]
                EXPORT INTMTEMG3_IRQHandler      [WEAK]
                EXPORT INTMTPTB30_IRQHandler     [WEAK]
                EXPORT INTMTTTB31_IRQHandler     [WEAK]
                EXPORT INTMTCAP30_IRQHandler     [WEAK]
                EXPORT INTMTCAP31_IRQHandler     [WEAK]
                EXPORT INTTB0_IRQHandler         [WEAK]
                EXPORT INTCAP00_IRQHandler       [WEAK]
                EXPORT INTCAP01_IRQHandler       [WEAK]
                EXPORT INTTB1_IRQHandler         [WEAK]
                EXPORT INTCAP10_IRQHandler       [WEAK]
                EXPORT INTCAP11_IRQHandler       [WEAK]
                EXPORT INTTB2_IRQHandler         [WEAK]
                EXPORT INTCAP20_IRQHandler       [WEAK]
                EXPORT INTCAP21_IRQHandler       [WEAK]
                EXPORT INTTB3_IRQHandler         [WEAK]
                EXPORT INTCAP30_IRQHandler       [WEAK]
                EXPORT INTCAP31_IRQHandler       [WEAK]
                EXPORT INTTB4_IRQHandler         [WEAK]
                EXPORT INTCAP40_IRQHandler       [WEAK]
                EXPORT INTCAP41_IRQHandler       [WEAK]
                EXPORT INTTB5_IRQHandler         [WEAK]
                EXPORT INTCAP50_IRQHandler       [WEAK]
                EXPORT INTCAP51_IRQHandler       [WEAK]
                EXPORT INTTB6_IRQHandler         [WEAK]
                EXPORT INTCAP60_IRQHandler       [WEAK]
                EXPORT INTCAP61_IRQHandler       [WEAK]
                EXPORT INTTB7_IRQHandler         [WEAK]
                EXPORT INTCAP70_IRQHandler       [WEAK]
                EXPORT INTCAP71_IRQHandler       [WEAK]
                EXPORT INTRTC_IRQHandler         [WEAK]
                EXPORT INTDMAA_IRQHandler        [WEAK]
                EXPORT INTDMAB_IRQHandler        [WEAK]
                EXPORT INTDMAC_IRQHandler        [WEAK]
                EXPORT INTDMACTC8_IRQHandler     [WEAK]
                EXPORT INTDMACTC9_IRQHandler     [WEAK]
                EXPORT INTDMACTC10_IRQHandler    [WEAK]
                EXPORT INTDMACTC11_IRQHandler    [WEAK]
                EXPORT INTDMAAERR_IRQHandler     [WEAK]
                EXPORT INTDMABERR_IRQHandler     [WEAK]
                EXPORT INTDMACERR_IRQHandler     [WEAK]
                EXPORT INTFLRDY_IRQHandler       [WEAK]

INT0_IRQHandler
INT1_IRQHandler
INT2_IRQHandler
INT3_IRQHandler
INT4_IRQHandler
INT5_IRQHandler
INT6_IRQHandler
INT7_IRQHandler
INT8_IRQHandler
INT9_IRQHandler
INTA_IRQHandler
INTB_IRQHandler
INTC_IRQHandler
INTD_IRQHandler
INTE_IRQHandler
INTF_IRQHandler
INTRX0_IRQHandler
INTTX0_IRQHandler
INTRX1_IRQHandler
INTTX1_IRQHandler
INTRX2_IRQHandler
INTTX2_IRQHandler
INTRX3_IRQHandler
INTTX3_IRQHandler
INTUART0_IRQHandler
INTUART1_IRQHandler
INTI2C0_IRQHandler
INTI2C1_IRQHandler
INTI2C2_IRQHandler
INTSSP0_IRQHandler
INTSSP1_IRQHandler
INTSSP2_IRQHandler
INTADHP_IRQHandler
INTADM0_IRQHandler
INTADM1_IRQHandler
INTAD_IRQHandler
INTAES_IRQHandler
INTSHA_IRQHandler
INTMLA_IRQHandler
INTESG_IRQHandler
INTSNFCSEQ_IRQHandler
INTSNFCPRTAE_IRQHandler
INTSNFCPRTCE_IRQHandler
INTSNFCFAIL_IRQHandler
INTMTEMG0_IRQHandler
INTMTPTB00_IRQHandler
INTMTPTB01_IRQHandler
INTMTCAP00_IRQHandler
INTMTCAP01_IRQHandler
INTMTEMG1_IRQHandler
INTMTPTB10_IRQHandler
INTMTPTB11_IRQHandler
INTMTCAP10_IRQHandler
INTMTCAP11_IRQHandler
INTMTEMG2_IRQHandler
INTMTPTB20_IRQHandler
INTMTTTB21_IRQHandler
INTMTCAP20_IRQHandler
INTMTCAP21_IRQHandler
INTMTEMG3_IRQHandler
INTMTPTB30_IRQHandler
INTMTTTB31_IRQHandler
INTMTCAP30_IRQHandler
INTMTCAP31_IRQHandler
INTTB0_IRQHandler
INTCAP00_IRQHandler
INTCAP01_IRQHandler
INTTB1_IRQHandler
INTCAP10_IRQHandler
INTCAP11_IRQHandler
INTTB2_IRQHandler
INTCAP20_IRQHandler
INTCAP21_IRQHandler
INTTB3_IRQHandler
INTCAP30_IRQHandler
INTCAP31_IRQHandler
INTTB4_IRQHandler
INTCAP40_IRQHandler
INTCAP41_IRQHandler
INTTB5_IRQHandler
INTCAP50_IRQHandler
INTCAP51_IRQHandler
INTTB6_IRQHandler
INTCAP60_IRQHandler
INTCAP61_IRQHandler
INTTB7_IRQHandler
INTCAP70_IRQHandler
INTCAP71_IRQHandler
INTRTC_IRQHandler
INTDMAA_IRQHandler
INTDMAB_IRQHandler
INTDMAC_IRQHandler
INTDMACTC8_IRQHandler
INTDMACTC9_IRQHandler
INTDMACTC10_IRQHandler
INTDMACTC11_IRQHandler
INTDMAAERR_IRQHandler
INTDMABERR_IRQHandler
INTDMACERR_IRQHandler
INTFLRDY_IRQHandler

                B       .

                ENDP
                
                ALIGN
                END
