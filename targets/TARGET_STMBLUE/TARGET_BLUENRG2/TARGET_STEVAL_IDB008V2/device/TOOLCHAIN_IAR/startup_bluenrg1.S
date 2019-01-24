;*******************************************************************************
;*
;* File Name          : startup_stm32f070xb.s
;*
;*******************************************************************************
;
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

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler                  ; Reset Handler

        DCD     NMI_Handler                    ; NMI Handler
        DCD     HardFault_Handler              ; Hard Fault Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     SVC_Handler                    ; SVCall Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     PendSV_Handler                 ; PendSV Handler
        DCD     SysTick_Handler                ; SysTick Handler

        ; External Interrupts
        DCD     GPIO_IRQHandler                ; Reserved
        DCD     FLASH_IRQHandler               ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     UART_IRQHandler                ; Reserved
        DCD     SPI_IRQHandler                 ; Reserved
        DCD     Blue_Handler                   ; Reserved
        DCD     WDG_IRQHandler                 ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     ADC_IRQHandler                 ; Reserved
        DCD     I2C1_IRQHandler                ; Reserved
        DCD     I2C2_IRQHandler                ; Reserved
        DCD     0                              ; Reserved
        DCD     MFT1A_IRQHandler               ; Reserved
        DCD     MFT1B_IRQHandler               ; Reserved
        DCD     MFT2A_IRQHandler               ; Reserved
        DCD     MFT2B_IRQHandler               ; Reserved
        DCD     RTC_IRQHandler                 ; Reserved
        DCD     PKA_Handler                 ; Reserved
        DCD     DMA_Handler                 ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK GPIO_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPIO_IRQHandler
        B GPIO_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK UART_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART_IRQHandler
        B UART_IRQHandler

        PUBWEAK SPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI_IRQHandler
        B SPI_IRQHandler

        PUBWEAK Blue_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Blue_Handler
        B Blue_Handler

        PUBWEAK WDG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WDG_IRQHandler
        B WDG_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_IRQHandler
        B I2C1_IRQHandler

        PUBWEAK I2C2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C2_IRQHandler
        B I2C2_IRQHandler

        PUBWEAK MFT1A_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MFT1A_IRQHandler
        B MFT1A_IRQHandler

        PUBWEAK MFT1B_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MFT1B_IRQHandler
        B MFT1B_IRQHandler

        PUBWEAK MFT2A_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MFT2A_IRQHandler
        B MFT2A_IRQHandler

        PUBWEAK MFT2B_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MFT2B_IRQHandler
        B MFT2B_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK PKA_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PKA_Handler
        B PKA_Handler

        PUBWEAK DMA_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA_Handler
        B DMA_Handler

        END
;*****************************END OF FILE*****
