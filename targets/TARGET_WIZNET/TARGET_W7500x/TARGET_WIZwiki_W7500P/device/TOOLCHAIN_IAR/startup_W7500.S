;/*******************************************************************************************************************************************************
; * Copyright ¨Ï 2016 <WIZnet Co.,Ltd.> 
; * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ¡°Software¡±), 
; * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
; * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
; *
; * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;
; * THE SOFTWARE IS PROVIDED ¡°AS IS¡±, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
; * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
; * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
; * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;*********************************************************************************************************************************************************/
;/**************************************************************************//**
; * @file     startup_ARMCM0.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM0 Device Series
; * @version  V1.08
; * @date     23. November 2012
; *
; * @note
; *
; ******************************************************************************/
;/* Copyright (c) 2011 - 2012 ARM LIMITED
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met:
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;   ---------------------------------------------------------------------------*/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
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
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
__vector_table_0x1c
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ; Exterval Interrupts
        DCD     SSP0_Handler              ; 16+ 0: SSP 0 Handler                   
        DCD     SSP1_Handler              ; 16+ 1: SSP 1 Handler                   
        DCD     UART0_Handler             ; 16+ 2: UART 0 Handler                  
        DCD     UART1_Handler             ; 16+ 3: UART 1 Handler                  
        DCD     UART2_Handler             ; 16+ 4: UART 2 Handler                  
        DCD     I2C0_Handler              ; 16+ 5: I2C 0 Handler                   
        DCD     I2C1_Handler              ; 16+ 6: I2C 1 Handler                   
        DCD     PORT0_Handler             ; 16+ 7: GPIO Port 0 Combined Handler    
        DCD     PORT1_Handler             ; 16+ 8: GPIO Port 1 Combined Handler    
        DCD     PORT2_Handler             ; 16+ 9: GPIO Port 2 Combined Handler    
        DCD     PORT3_Handler             ; 16+10: GPIO Port 3 Combined Handler    
        DCD     DMA_Handler               ; 16+11: DMA Combined Handler            
        DCD     DUALTIMER0_Handler        ; 16+12: Dual timer 0 handler             
        DCD     DUALTIMER1_Handler        ; 16+13: Dual timer 1 handler            
        DCD     PWM0_Handler              ; 16+14: PWM0 Handler                    
        DCD     PWM1_Handler              ; 16+15: PWM1 Handler                    
        DCD     PWM2_Handler              ; 16+16: PWM2 Handler                    
        DCD     PWM3_Handler              ; 16+17: PWM3 Handler                    
        DCD     PWM4_Handler              ; 16+18: PWM4 Handler                    
        DCD     PWM5_Handler              ; 16+19: PWM5 Handler                    
        DCD     PWM6_Handler              ; 16+20: PWM6 Handler                    
        DCD     PWM7_Handler              ; 16+21: PWM7 Handler                    
        DCD     RTC_Handler               ; 16+22: RTC Handler                     
        DCD     ADC_Handler               ; 16+23: ADC Handler                     
        DCD     WZTOE_Handler             ; 16+24: WZTOE_Handler                   
        DCD     EXTI_Handler             ; 16+25: EXTI_Handler      	
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

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
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK SSP0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP0_Handler
        B SSP0_Handler

        PUBWEAK SSP1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP1_Handler
        B SSP1_Handler

        PUBWEAK UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_Handler
        B UART0_Handler

        PUBWEAK UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_Handler
        B UART1_Handler

        PUBWEAK UART2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_Handler
        B UART2_Handler

        PUBWEAK I2C0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_Handler
        B I2C0_Handler
        
        PUBWEAK I2C1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_Handler
        B I2C1_Handler
        
        PUBWEAK PORT0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_Handler
        B PORT0_Handler

        PUBWEAK PORT1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT1_Handler
        B PORT1_Handler

        PUBWEAK PORT2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT2_Handler
        B PORT2_Handler

        PUBWEAK PORT3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT3_Handler
        B PORT3_Handler

        PUBWEAK DMA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Handler
        B DMA_Handler

        PUBWEAK DUALTIMER0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DUALTIMER0_Handler
        B DUALTIMER0_Handler

        PUBWEAK DUALTIMER1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DUALTIMER1_Handler
        B DUALTIMER1_Handler

        PUBWEAK PWM0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM0_Handler
        B PWM0_Handler

        PUBWEAK PWM1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM1_Handler
        B PWM1_Handler
        
        PUBWEAK PWM2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM2_Handler
        B PWM2_Handler

        PUBWEAK PWM3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM3_Handler
        B PWM3_Handler

        PUBWEAK PWM4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM4_Handler
        B PWM4_Handler
        
        PUBWEAK PWM5_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM5_Handler
        B PWM5_Handler

        PUBWEAK PWM6_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM6_Handler
        B PWM6_Handler
        
        PUBWEAK PWM7_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM7_Handler
        B PWM7_Handler
                
        PUBWEAK RTC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_Handler
        B RTC_Handler
        
        PUBWEAK ADC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_Handler
        B ADC_Handler
        
        PUBWEAK WZTOE_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
WZTOE_Handler
        B WZTOE_Handler
        
        PUBWEAK EXTI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI_Handler
        B EXTI_Handler
        
        END
