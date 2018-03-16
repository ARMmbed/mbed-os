;/**
; *******************************************************************************
; * @file    startup_TMPM46B.s
; * @brief   CMSIS Cortex-M4 Core Device Startup File for the
; *          TOSHIBA 'TMPM46B' Device Series
; * @version V2.0.2.4
; * @date    2015/03/31
; * 
; * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
; * 
; * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
; *******************************************************************************
; */
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; Cortex-M version
;

                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                SECTION .intvec:CODE:NOROOT(2)

                EXTERN  __iar_program_start
                EXTERN  SystemInit
                PUBLIC  __vector_table

                DATA
__vector_table  DCD     sfe(CSTACK)
                DCD     Reset_Handler

                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     INT0_IRQHandler           ; 0:	Interrupt pin 0
                DCD     INT1_IRQHandler           ; 1:	Interrupt pin 1
                DCD     INT2_IRQHandler           ; 2:	Interrupt pin 2
                DCD     INT3_IRQHandler           ; 3:	Interrupt pin 3
                DCD     INT4_IRQHandler           ; 4:	Interrupt pin 4
                DCD     INT5_IRQHandler           ; 5:	Interrupt pin 5
                DCD     INT6_IRQHandler           ; 6:	Interrupt pin 6
                DCD     INT7_IRQHandler           ; 7:	Interrupt pin 7
                DCD     INT8_IRQHandler           ; 8:	Interrupt pin 8
                DCD     INT9_IRQHandler           ; 9:	Interrupt pin 9
                DCD     INTA_IRQHandler           ; 10:	Interrupt pin A
                DCD     INTB_IRQHandler           ; 11:	Interrupt pin B
                DCD     INTC_IRQHandler           ; 12:	Interrupt pin C
                DCD     INTD_IRQHandler           ; 13:	Interrupt pin D
                DCD     INTE_IRQHandler           ; 14:	Interrupt pin E
                DCD     INTF_IRQHandler           ; 15:	Interrupt pin F
                DCD     INTRX0_IRQHandler         ; 16:	Serial0 reception interrupt
                DCD     INTTX0_IRQHandler         ; 17:	Serial0 transmission interrupt
                DCD     INTRX1_IRQHandler         ; 18:	Serial1 reception interrupt
                DCD     INTTX1_IRQHandler         ; 19:	Serial1 transmission interrupt
                DCD     INTRX2_IRQHandler         ; 20:	Serial2 reception interrupt
                DCD     INTTX2_IRQHandler         ; 21:	Serial2 transmission interrupt
                DCD     INTRX3_IRQHandler         ; 22:	Serial3 reception interrupt
                DCD     INTTX3_IRQHandler         ; 23:	Serial3 transmission interrupt
                DCD     INTUART0_IRQHandler       ; 24:	Full UART0 transmission and reception interrupt
                DCD     INTUART1_IRQHandler       ; 25:	Full UART1 transmission and reception interrupt
                DCD     INTI2C0_IRQHandler        ; 26:	I2C0 transmission and reception interrupt
                DCD     INTI2C1_IRQHandler        ; 27:	I2C1 transmission and reception interrupt
                DCD     INTI2C2_IRQHandler        ; 28:	I2C2 transmission and reception interrupt
                DCD     INTSSP0_IRQHandler        ; 29:	SSP(SPI) Serial interface 0 interrupt 
                DCD     INTSSP1_IRQHandler        ; 30:	SSP(SPI) Serial interface 1 interrupt 
                DCD     INTSSP2_IRQHandler        ; 31:	SSP(SPI) Serial interface 2 interrupt 
                DCD     INTADHP_IRQHandler        ; 32:	High Priority AD conversion interrupt
                DCD     INTADM0_IRQHandler        ; 33:	AD conversion monitor interrupt 0
                DCD     INTADM1_IRQHandler        ; 34:	AD conversion monitor interrupt 1
                DCD     INTAD_IRQHandler          ; 35:	AD conversion interrupt 
                DCD     INTAES_IRQHandler         ; 36:	AES completion interrupt
                DCD     INTSHA_IRQHandler         ; 37:	SHA completion interrupt
                DCD     INTMLA_IRQHandler         ; 38:	MLA completion interrupt
                DCD     INTESG_IRQHandler         ; 39:	ESG completion interrupt
                DCD     INTSNFCSEQ_IRQHandler     ; 40:	SNFC command sequence end interrupt
                DCD     INTSNFCPRTAE_IRQHandler   ; 41:	SNFC page lead RAM transfer end interrupt
                DCD     INTSNFCPRTCE_IRQHandler   ; 42:	SNFC decode data RAM transmission end interrupt
                DCD     INTSNFCFAIL_IRQHandler    ; 43:	SNFC decode fail interrupt
                DCD     0                         ; 44:	Reserved
                DCD     0                         ; 45:	Reserved
                DCD     0                         ; 46:	Reserved
                DCD     INTMTEMG0_IRQHandler      ; 47:	MPT0 EMG interrupt
                DCD     INTMTPTB00_IRQHandler     ; 48:	MPT0 compare match0/overflow,IGBT cycle interrupt
                DCD     INTMTPTB01_IRQHandler     ; 49:	MPT0 compare match1/overflow,IGBT cycle interrupt
                DCD     INTMTCAP00_IRQHandler     ; 50:	MPT0 input capture0 interrupt
                DCD     INTMTCAP01_IRQHandler     ; 51:	MPT0 input capture1 interrupt
                DCD     INTMTEMG1_IRQHandler      ; 52:	MPT1 EMG interrupt
                DCD     INTMTPTB10_IRQHandler     ; 53:	MPT1 compare match0/overflow,IGBT cycle interrupt
                DCD     INTMTPTB11_IRQHandler     ; 54:	MPT1 compare match1/overflow,IGBT cycle interrupt
                DCD     INTMTCAP10_IRQHandler     ; 55:	MPT1 input capture0 interrupt
                DCD     INTMTCAP11_IRQHandler     ; 56:	MPT1 input capture1 interrupt
                DCD     INTMTEMG2_IRQHandler      ; 57:	MPT2 EMG interrupt
                DCD     INTMTPTB20_IRQHandler     ; 58:	MPT2 compare match0/overflow,IGBT cycle interrupt
                DCD     INTMTTTB21_IRQHandler     ; 59:	MPT2 compare match1/overflow,IGBT cycle interrupt
                DCD     INTMTCAP20_IRQHandler     ; 60:	MPT2 input capture0 interrupt
                DCD     INTMTCAP21_IRQHandler     ; 61:	MPT2 input capture1 interrupt
                DCD     INTMTEMG3_IRQHandler      ; 62:	MPT3 EMG interrupt
                DCD     INTMTPTB30_IRQHandler     ; 63:	MPT3 compare match0/overflow,IGBT cycle interrupt
                DCD     INTMTTTB31_IRQHandler     ; 64:	MPT3 compare match1/overflow,IGBT cycle interrupt
                DCD     INTMTCAP30_IRQHandler     ; 65:	MPT3 input capture0 interrupt
                DCD     INTMTCAP31_IRQHandler     ; 66:	MPT3 input capture1 interrupt
                DCD     INTTB0_IRQHandler         ; 67:	TMRB0 compare match detection interrupt
                DCD     INTCAP00_IRQHandler       ; 68:	TMRB0 input capture 0 interrupt
                DCD     INTCAP01_IRQHandler       ; 69:	TMRB0 input capture 1 interrupt
                DCD     INTTB1_IRQHandler         ; 70:	TMRB1 compare match detection interrupt
                DCD     INTCAP10_IRQHandler       ; 71:	TMRB1 input capture 0 interrupt
                DCD     INTCAP11_IRQHandler       ; 72:	TMRB1 input capture 1 interrupt
                DCD     INTTB2_IRQHandler         ; 73:	TMRB2 compare match detection interrupt
                DCD     INTCAP20_IRQHandler       ; 74:	TMRB2 input capture 0 interrupt
                DCD     INTCAP21_IRQHandler       ; 75:	TMRB2 input capture 1 interrupt
                DCD     INTTB3_IRQHandler         ; 76:	TMRB3 compare match detection interrupt
                DCD     INTCAP30_IRQHandler       ; 77:	TMRB3 input capture 0 interrupt
                DCD     INTCAP31_IRQHandler       ; 78:	TMRB3 input capture 1 interrupt
                DCD     INTTB4_IRQHandler         ; 79:	TMRB4 compare match detection interrupt
                DCD     INTCAP40_IRQHandler       ; 80:	TMRB4 input capture 0 interrupt
                DCD     INTCAP41_IRQHandler       ; 81:	TMRB4 input capture 1 interrupt
                DCD     INTTB5_IRQHandler         ; 82:	TMRB5 compare match detection interrupt
                DCD     INTCAP50_IRQHandler       ; 83:	TMRB5 input capture 0 interrupt
                DCD     INTCAP51_IRQHandler       ; 84:	TMRB5 input capture 1 interrupt
                DCD     INTTB6_IRQHandler         ; 85:	TMRB6 compare match detection interrupt
                DCD     INTCAP60_IRQHandler       ; 86:	TMRB6 input capture 0 interrupt
                DCD     INTCAP61_IRQHandler       ; 87:	TMRB6 input capture 1 interrupt
                DCD     INTTB7_IRQHandler         ; 88:	TMRB7 compare match detection interrupt
                DCD     INTCAP70_IRQHandler       ; 89:	TMRB7 input capture 0 interrupt
                DCD     INTCAP71_IRQHandler       ; 90:	TMRB7 input capture 1 interrupt
                DCD     INTRTC_IRQHandler         ; 91:	Real time clock interrupt
                DCD     INTDMAA_IRQHandler        ; 92:	DMAC unitA transmission completion interrupt(ch4-31)
                DCD     INTDMAB_IRQHandler        ; 93:	DMAC unitB transmission completion interrupt(ch24-31)
                DCD     INTDMAC_IRQHandler        ; 94:	DMAC unitC transmission completion interrupt(ch12-31)
                DCD     INTDMACTC8_IRQHandler     ; 95:	DMAC unitC transmission completion interrupt(ch8)
                DCD     INTDMACTC9_IRQHandler     ; 96:	DMAC unitC transmission completion interrupt(ch9)
                DCD     INTDMACTC10_IRQHandler    ; 97:	DMAC unitC transmission completion interrupt(ch10)
                DCD     INTDMACTC11_IRQHandler    ; 98:	DMAC unitC transmission completion interrupt(ch11)
                DCD     INTDMAAERR_IRQHandler     ; 99:	DMAC transmission error interrupt(unitA)
                DCD     INTDMABERR_IRQHandler     ; 100:	DMAC transmission error interrupt(unitB)
                DCD     INTDMACERR_IRQHandler     ; 101:	DMAC transmission error interrupt(unitC)
                DCD     INTFLRDY_IRQHandler       ; 102:	Flash Ready interrupt
                THUMB
; Dummy Exception Handlers (infinite loops which can be modified)

                PUBWEAK Reset_Handler
                SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0

                PUBWEAK NMI_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
                B       NMI_Handler

                PUBWEAK HardFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
                B       HardFault_Handler

                PUBWEAK MemManage_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
                B       MemManage_Handler

                PUBWEAK BusFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
                B       BusFault_Handler

                PUBWEAK UsageFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
                B       UsageFault_Handler

                PUBWEAK SVC_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK DebugMon_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
                B       DebugMon_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
                B       SysTick_Handler

                PUBWEAK INT0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT0_IRQHandler
                B       INT0_IRQHandler

                PUBWEAK INT1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT1_IRQHandler
                B       INT1_IRQHandler

                PUBWEAK INT2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT2_IRQHandler
                B       INT2_IRQHandler

                PUBWEAK INT3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT3_IRQHandler
                B       INT3_IRQHandler

                PUBWEAK INT4_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT4_IRQHandler
                B       INT4_IRQHandler

                PUBWEAK INT5_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT5_IRQHandler
                B       INT5_IRQHandler

                PUBWEAK INT6_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT6_IRQHandler
                B       INT6_IRQHandler

                PUBWEAK INT7_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT7_IRQHandler
                B       INT7_IRQHandler

                PUBWEAK INT8_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT8_IRQHandler
                B       INT8_IRQHandler

                PUBWEAK INT9_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT9_IRQHandler
                B       INT9_IRQHandler

                PUBWEAK INTA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTA_IRQHandler
                B       INTA_IRQHandler

                PUBWEAK INTB_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTB_IRQHandler
                B       INTB_IRQHandler

                PUBWEAK INTC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTC_IRQHandler
                B       INTC_IRQHandler

                PUBWEAK INTD_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTD_IRQHandler
                B       INTD_IRQHandler

                PUBWEAK INTE_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTE_IRQHandler
                B       INTE_IRQHandler

                PUBWEAK INTF_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTF_IRQHandler
                B       INTF_IRQHandler

                PUBWEAK INTRX0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRX0_IRQHandler
                B       INTRX0_IRQHandler

                PUBWEAK INTTX0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTX0_IRQHandler
                B       INTTX0_IRQHandler

                PUBWEAK INTRX1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRX1_IRQHandler
                B       INTRX1_IRQHandler

                PUBWEAK INTTX1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTX1_IRQHandler
                B       INTTX1_IRQHandler

                PUBWEAK INTRX2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRX2_IRQHandler
                B       INTRX2_IRQHandler

                PUBWEAK INTTX2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTX2_IRQHandler
                B       INTTX2_IRQHandler

                PUBWEAK INTRX3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRX3_IRQHandler
                B       INTRX3_IRQHandler

                PUBWEAK INTTX3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTX3_IRQHandler
                B       INTTX3_IRQHandler

                PUBWEAK INTUART0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART0_IRQHandler
                B       INTUART0_IRQHandler

                PUBWEAK INTUART1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART1_IRQHandler
                B       INTUART1_IRQHandler

                PUBWEAK INTI2C0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C0_IRQHandler
                B       INTI2C0_IRQHandler

                PUBWEAK INTI2C1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C1_IRQHandler
                B       INTI2C1_IRQHandler

                PUBWEAK INTI2C2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C2_IRQHandler
                B       INTI2C2_IRQHandler

                PUBWEAK INTSSP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSSP0_IRQHandler
                B       INTSSP0_IRQHandler

                PUBWEAK INTSSP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSSP1_IRQHandler
                B       INTSSP1_IRQHandler

                PUBWEAK INTSSP2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSSP2_IRQHandler
                B       INTSSP2_IRQHandler

                PUBWEAK INTADHP_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADHP_IRQHandler
                B       INTADHP_IRQHandler

                PUBWEAK INTADM0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADM0_IRQHandler
                B       INTADM0_IRQHandler

                PUBWEAK INTADM1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADM1_IRQHandler
                B       INTADM1_IRQHandler

                PUBWEAK INTAD_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTAD_IRQHandler
                B       INTAD_IRQHandler

                PUBWEAK INTAES_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTAES_IRQHandler
                B       INTAES_IRQHandler

                PUBWEAK INTSHA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSHA_IRQHandler
                B       INTSHA_IRQHandler

                PUBWEAK INTMLA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMLA_IRQHandler
                B       INTMLA_IRQHandler

                PUBWEAK INTESG_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTESG_IRQHandler
                B       INTESG_IRQHandler

                PUBWEAK INTSNFCSEQ_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSNFCSEQ_IRQHandler
                B       INTSNFCSEQ_IRQHandler

                PUBWEAK INTSNFCPRTAE_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSNFCPRTAE_IRQHandler
                B       INTSNFCPRTAE_IRQHandler

                PUBWEAK INTSNFCPRTCE_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSNFCPRTCE_IRQHandler
                B       INTSNFCPRTCE_IRQHandler

                PUBWEAK INTSNFCFAIL_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSNFCFAIL_IRQHandler
                B       INTSNFCFAIL_IRQHandler

                PUBWEAK INTMTEMG0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTEMG0_IRQHandler
                B       INTMTEMG0_IRQHandler

                PUBWEAK INTMTPTB00_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTPTB00_IRQHandler
                B       INTMTPTB00_IRQHandler

                PUBWEAK INTMTPTB01_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTPTB01_IRQHandler
                B       INTMTPTB01_IRQHandler

                PUBWEAK INTMTCAP00_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP00_IRQHandler
                B       INTMTCAP00_IRQHandler

                PUBWEAK INTMTCAP01_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP01_IRQHandler
                B       INTMTCAP01_IRQHandler

                PUBWEAK INTMTEMG1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTEMG1_IRQHandler
                B       INTMTEMG1_IRQHandler

                PUBWEAK INTMTPTB10_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTPTB10_IRQHandler
                B       INTMTPTB10_IRQHandler

                PUBWEAK INTMTPTB11_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTPTB11_IRQHandler
                B       INTMTPTB11_IRQHandler

                PUBWEAK INTMTCAP10_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP10_IRQHandler
                B       INTMTCAP10_IRQHandler

                PUBWEAK INTMTCAP11_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP11_IRQHandler
                B       INTMTCAP11_IRQHandler

                PUBWEAK INTMTEMG2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTEMG2_IRQHandler
                B       INTMTEMG2_IRQHandler

                PUBWEAK INTMTPTB20_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTPTB20_IRQHandler
                B       INTMTPTB20_IRQHandler

                PUBWEAK INTMTTTB21_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTTTB21_IRQHandler
                B       INTMTTTB21_IRQHandler

                PUBWEAK INTMTCAP20_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP20_IRQHandler
                B       INTMTCAP20_IRQHandler

                PUBWEAK INTMTCAP21_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP21_IRQHandler
                B       INTMTCAP21_IRQHandler

                PUBWEAK INTMTEMG3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTEMG3_IRQHandler
                B       INTMTEMG3_IRQHandler

                PUBWEAK INTMTPTB30_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTPTB30_IRQHandler
                B       INTMTPTB30_IRQHandler

                PUBWEAK INTMTTTB31_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTTTB31_IRQHandler
                B       INTMTTTB31_IRQHandler

                PUBWEAK INTMTCAP30_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP30_IRQHandler
                B       INTMTCAP30_IRQHandler

                PUBWEAK INTMTCAP31_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTMTCAP31_IRQHandler
                B       INTMTCAP31_IRQHandler

                PUBWEAK INTTB0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB0_IRQHandler
                B       INTTB0_IRQHandler

                PUBWEAK INTCAP00_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP00_IRQHandler
                B       INTCAP00_IRQHandler

                PUBWEAK INTCAP01_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP01_IRQHandler
                B       INTCAP01_IRQHandler

                PUBWEAK INTTB1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB1_IRQHandler
                B       INTTB1_IRQHandler

                PUBWEAK INTCAP10_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP10_IRQHandler
                B       INTCAP10_IRQHandler

                PUBWEAK INTCAP11_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP11_IRQHandler
                B       INTCAP11_IRQHandler

                PUBWEAK INTTB2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB2_IRQHandler
                B       INTTB2_IRQHandler

                PUBWEAK INTCAP20_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP20_IRQHandler
                B       INTCAP20_IRQHandler

                PUBWEAK INTCAP21_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP21_IRQHandler
                B       INTCAP21_IRQHandler

                PUBWEAK INTTB3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB3_IRQHandler
                B       INTTB3_IRQHandler

                PUBWEAK INTCAP30_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP30_IRQHandler
                B       INTCAP30_IRQHandler

                PUBWEAK INTCAP31_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP31_IRQHandler
                B       INTCAP31_IRQHandler

                PUBWEAK INTTB4_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB4_IRQHandler
                B       INTTB4_IRQHandler

                PUBWEAK INTCAP40_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP40_IRQHandler
                B       INTCAP40_IRQHandler

                PUBWEAK INTCAP41_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP41_IRQHandler
                B       INTCAP41_IRQHandler

                PUBWEAK INTTB5_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB5_IRQHandler
                B       INTTB5_IRQHandler

                PUBWEAK INTCAP50_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP50_IRQHandler
                B       INTCAP50_IRQHandler

                PUBWEAK INTCAP51_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP51_IRQHandler
                B       INTCAP51_IRQHandler

                PUBWEAK INTTB6_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB6_IRQHandler
                B       INTTB6_IRQHandler

                PUBWEAK INTCAP60_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP60_IRQHandler
                B       INTCAP60_IRQHandler

                PUBWEAK INTCAP61_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP61_IRQHandler
                B       INTCAP61_IRQHandler

                PUBWEAK INTTB7_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB7_IRQHandler
                B       INTTB7_IRQHandler

                PUBWEAK INTCAP70_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP70_IRQHandler
                B       INTCAP70_IRQHandler

                PUBWEAK INTCAP71_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTCAP71_IRQHandler
                B       INTCAP71_IRQHandler

                PUBWEAK INTRTC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRTC_IRQHandler
                B       INTRTC_IRQHandler

                PUBWEAK INTDMAA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAA_IRQHandler
                B       INTDMAA_IRQHandler

                PUBWEAK INTDMAB_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAB_IRQHandler
                B       INTDMAB_IRQHandler

                PUBWEAK INTDMAC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAC_IRQHandler
                B       INTDMAC_IRQHandler

                PUBWEAK INTDMACTC8_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMACTC8_IRQHandler
                B       INTDMACTC8_IRQHandler

                PUBWEAK INTDMACTC9_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMACTC9_IRQHandler
                B       INTDMACTC9_IRQHandler

                PUBWEAK INTDMACTC10_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMACTC10_IRQHandler
                B       INTDMACTC10_IRQHandler

                PUBWEAK INTDMACTC11_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMACTC11_IRQHandler
                B       INTDMACTC11_IRQHandler

                PUBWEAK INTDMAAERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAAERR_IRQHandler
                B       INTDMAAERR_IRQHandler

                PUBWEAK INTDMABERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMABERR_IRQHandler
                B       INTDMABERR_IRQHandler

                PUBWEAK INTDMACERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMACERR_IRQHandler
                B       INTDMACERR_IRQHandler

                PUBWEAK INTFLRDY_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTFLRDY_IRQHandler
                B       INTFLRDY_IRQHandler

                END
