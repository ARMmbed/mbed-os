;* ------------------------------------------------------------------------- 
;*  @file:    startup_MIMXRT1176_cm7.s                                       
;*  @purpose: CMSIS Cortex-M7 Core Device Startup File                       
;*            MIMXRT1176_cm7                                                 
;*  @version: 1.0                                                            
;*  @date:    2020-12-29                                                     
;*  @build:   b210203                                                        
;* ------------------------------------------------------------------------- 
;*                                                                           
;* Copyright 1997-2016 Freescale Semiconductor, Inc.                         
;* Copyright 2016-2021 NXP                                                   
;* All rights reserved.                                                      
;*                                                                           
;* SPDX-License-Identifier: BSD-3-Clause                                     
;****************************************************************************
;* Version: GCC for ARM Embedded Processors                                  
;***************************************************************************

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors   DCD   |Image$$ARM_LIB_STACK$$ZI$$Limit|                ; Top of Stack 
            DCD   Reset_Handler                                   ; Reset Handler 
            DCD   NMI_Handler                                     ; NMI Handler
            DCD   HardFault_Handler                               ; Hard Fault Handler
            DCD   MemManage_Handler                               ; MPU Fault Handler
            DCD   BusFault_Handler                                ; Bus Fault Handler
            DCD   UsageFault_Handler                              ; Usage Fault Handler
            DCD   0                                               ; Reserved
            DCD   0                                               ; Reserved
            DCD   0                                               ; Reserved
            DCD   0                                               ; Reserved
            DCD   SVC_Handler                                     ; SVCall Handler
            DCD   DebugMon_Handler                                ; Debug Monitor Handler
            DCD   0                                               ; Reserved
            DCD   PendSV_Handler                                  ; PendSV Handler
            DCD   SysTick_Handler                                 ; SysTick Handler

                                                            ; External Interrupts
            DCD   DMA0_DMA16_IRQHandler                           ; DMA channel 0;16 transfer complete
            DCD   DMA1_DMA17_IRQHandler                           ; DMA channel 1;17 transfer complete
            DCD   DMA2_DMA18_IRQHandler                           ; DMA channel 2;18 transfer complete
            DCD   DMA3_DMA19_IRQHandler                           ; DMA channel 3;19 transfer complete
            DCD   DMA4_DMA20_IRQHandler                           ; DMA channel 4;20 transfer complete
            DCD   DMA5_DMA21_IRQHandler                           ; DMA channel 5;21 transfer complete
            DCD   DMA6_DMA22_IRQHandler                           ; DMA channel 6;22 transfer complete
            DCD   DMA7_DMA23_IRQHandler                           ; DMA channel 7;23 transfer complete
            DCD   DMA8_DMA24_IRQHandler                           ; DMA channel 8;24 transfer complete
            DCD   DMA9_DMA25_IRQHandler                           ; DMA channel 9;25 transfer complete
            DCD   DMA10_DMA26_IRQHandler                          ; DMA channel 10;26 transfer complete
            DCD   DMA11_DMA27_IRQHandler                          ; DMA channel 11;27 transfer complete
            DCD   DMA12_DMA28_IRQHandler                          ; DMA channel 12;28 transfer complete
            DCD   DMA13_DMA29_IRQHandler                          ; DMA channel 13;29 transfer complete
            DCD   DMA14_DMA30_IRQHandler                          ; DMA channel 14;30 transfer complete
            DCD   DMA15_DMA31_IRQHandler                          ; DMA channel 15;31 transfer complete
            DCD   DMA_ERROR_IRQHandler                            ; DMA error interrupt channels 0-15 ; 16-31
            DCD   CTI_TRIGGER_OUT0_IRQHandler                     ; CTI_TRIGGER_OUT0
            DCD   CTI_TRIGGER_OUT1_IRQHandler                     ; CTI_TRIGGER_OUT1
            DCD   CORE_IRQHandler                                 ; CorePlatform exception IRQ
            DCD   LPUART1_IRQHandler                              ; LPUART1 TX interrupt and RX interrupt
            DCD   LPUART2_IRQHandler                              ; LPUART2 TX interrupt and RX interrupt
            DCD   LPUART3_IRQHandler                              ; LPUART3 TX interrupt and RX interrupt
            DCD   LPUART4_IRQHandler                              ; LPUART4 TX interrupt and RX interrupt
            DCD   LPUART5_IRQHandler                              ; LPUART5 TX interrupt and RX interrupt
            DCD   LPUART6_IRQHandler                              ; LPUART6 TX interrupt and RX interrupt
            DCD   LPUART7_IRQHandler                              ; LPUART7 TX interrupt and RX interrupt
            DCD   LPUART8_IRQHandler                              ; LPUART8 TX interrupt and RX interrupt
            DCD   LPUART9_IRQHandler                              ; LPUART9 TX interrupt and RX interrupt
            DCD   LPUART10_IRQHandler                             ; LPUART10 TX interrupt and RX interrupt
            DCD   LPUART11_IRQHandler                             ; LPUART11 TX interrupt and RX interrupt
            DCD   LPUART12_IRQHandler                             ; LPUART12 TX interrupt and RX interrupt
            DCD   LPI2C1_IRQHandler                               ; LPI2C1 interrupt
            DCD   LPI2C2_IRQHandler                               ; LPI2C2 interrupt
            DCD   LPI2C3_IRQHandler                               ; LPI2C3 interrupt
            DCD   LPI2C4_IRQHandler                               ; LPI2C4 interrupt
            DCD   LPI2C5_IRQHandler                               ; LPI2C5 interrupt
            DCD   LPI2C6_IRQHandler                               ; LPI2C6 interrupt
            DCD   LPSPI1_IRQHandler                               ; LPSPI1 interrupt request line to the core
            DCD   LPSPI2_IRQHandler                               ; LPSPI2 interrupt request line to the core
            DCD   LPSPI3_IRQHandler                               ; LPSPI3 interrupt request line to the core
            DCD   LPSPI4_IRQHandler                               ; LPSPI4 interrupt request line to the core
            DCD   LPSPI5_IRQHandler                               ; LPSPI5 interrupt request line to the core
            DCD   LPSPI6_IRQHandler                               ; LPSPI6 interrupt request line to the core
            DCD   CAN1_IRQHandler                                 ; CAN1 interrupt
            DCD   CAN1_ERROR_IRQHandler                           ; CAN1 error interrupt
            DCD   CAN2_IRQHandler                                 ; CAN2 interrupt
            DCD   CAN2_ERROR_IRQHandler                           ; CAN2 error interrupt
            DCD   CAN3_IRQHandler                                 ; CAN3 interrupt
            DCD   CAN3_ERROR_IRQHandler                           ; CAN3 erro interrupt
            DCD   FLEXRAM_IRQHandler                              ; FlexRAM address out of range Or access hit IRQ
            DCD   KPP_IRQHandler                                  ; Keypad nterrupt
            DCD   Reserved68_IRQHandler                           ; Reserved interrupt
            DCD   GPR_IRQ_IRQHandler                              ; GPR interrupt
            DCD   eLCDIF_IRQHandler                               ; eLCDIF interrupt
            DCD   LCDIFv2_IRQHandler                              ; LCDIFv2 interrupt
            DCD   CSI_IRQHandler                                  ; CSI interrupt
            DCD   PXP_IRQHandler                                  ; PXP interrupt
            DCD   MIPI_CSI_IRQHandler                             ; MIPI_CSI interrupt
            DCD   MIPI_DSI_IRQHandler                             ; MIPI_DSI interrupt
            DCD   GPU2D_IRQHandler                                ; GPU2D interrupt
            DCD   GPIO6_Combined_0_15_IRQHandler                  ; Combined interrupt indication for GPIO6 signal 0 throughout 15
            DCD   GPIO6_Combined_16_31_IRQHandler                 ; Combined interrupt indication for GPIO6 signal 16 throughout 31
            DCD   DAC_IRQHandler                                  ; DAC interrupt
            DCD   KEY_MANAGER_IRQHandler                          ; PUF interrupt
            DCD   WDOG2_IRQHandler                                ; WDOG2 interrupt
            DCD   SNVS_HP_NON_TZ_IRQHandler                       ; SRTC Consolidated Interrupt. Non TZ
            DCD   SNVS_HP_TZ_IRQHandler                           ; SRTC Security Interrupt. TZ
            DCD   SNVS_PULSE_EVENT_IRQHandler                     ; ON-OFF button press shorter than 5 secs (pulse event)
            DCD   CAAM_IRQ0_IRQHandler                            ; CAAM interrupt queue for JQ0
            DCD   CAAM_IRQ1_IRQHandler                            ; CAAM interrupt queue for JQ1
            DCD   CAAM_IRQ2_IRQHandler                            ; CAAM interrupt queue for JQ2
            DCD   CAAM_IRQ3_IRQHandler                            ; CAAM interrupt queue for JQ3
            DCD   CAAM_RECORVE_ERRPR_IRQHandler                   ; CAAM interrupt for recoverable error
            DCD   CAAM_RTIC_IRQHandler                            ; CAAM interrupt for RTIC
            DCD   CDOG_IRQHandler                                 ; CDOG interrupt
            DCD   SAI1_IRQHandler                                 ; SAI1 interrupt
            DCD   SAI2_IRQHandler                                 ; SAI1 interrupt
            DCD   SAI3_RX_IRQHandler                              ; SAI3 interrupt
            DCD   SAI3_TX_IRQHandler                              ; SAI3 interrupt
            DCD   SAI4_RX_IRQHandler                              ; SAI4 interrupt
            DCD   SAI4_TX_IRQHandler                              ; SAI4 interrupt
            DCD   SPDIF_IRQHandler                                ; SPDIF interrupt
            DCD   ANATOP_TEMP_INT_IRQHandler                      ; ANATOP interrupt
            DCD   ANATOP_TEMP_LOW_HIGH_IRQHandler                 ; ANATOP interrupt
            DCD   ANATOP_TEMP_PANIC_IRQHandler                    ; ANATOP interrupt
            DCD   ANATOP_LP8_BROWNOUT_IRQHandler                  ; ANATOP interrupt
            DCD   ANATOP_LP0_BROWNOUT_IRQHandler                  ; ANATOP interrupt
            DCD   ADC1_IRQHandler                                 ; ADC1 interrupt
            DCD   ADC2_IRQHandler                                 ; ADC2 interrupt
            DCD   USBPHY1_IRQHandler                              ; USBPHY1 interrupt
            DCD   USBPHY2_IRQHandler                              ; USBPHY2 interrupt
            DCD   RDC_IRQHandler                                  ; RDC interrupt
            DCD   GPIO13_Combined_0_31_IRQHandler                 ; Combined interrupt indication for GPIO13 signal 0 throughout 31
            DCD   Reserved110_IRQHandler                          ; Reserved interrupt
            DCD   DCIC1_IRQHandler                                ; DCIC1 interrupt
            DCD   DCIC2_IRQHandler                                ; DCIC2 interrupt
            DCD   ASRC_IRQHandler                                 ; ASRC interrupt
            DCD   FLEXRAM_ECC_IRQHandler                          ; FlexRAM ECC fatal interrupt
            DCD   CM7_GPIO2_3_IRQHandler                          ; CM7_GPIO2,CM7_GPIO3 interrupt
            DCD   GPIO1_Combined_0_15_IRQHandler                  ; Combined interrupt indication for GPIO1 signal 0 throughout 15
            DCD   GPIO1_Combined_16_31_IRQHandler                 ; Combined interrupt indication for GPIO1 signal 16 throughout 31
            DCD   GPIO2_Combined_0_15_IRQHandler                  ; Combined interrupt indication for GPIO2 signal 0 throughout 15
            DCD   GPIO2_Combined_16_31_IRQHandler                 ; Combined interrupt indication for GPIO2 signal 16 throughout 31
            DCD   GPIO3_Combined_0_15_IRQHandler                  ; Combined interrupt indication for GPIO3 signal 0 throughout 15
            DCD   GPIO3_Combined_16_31_IRQHandler                 ; Combined interrupt indication for GPIO3 signal 16 throughout 31
            DCD   GPIO4_Combined_0_15_IRQHandler                  ; Combined interrupt indication for GPIO4 signal 0 throughout 15
            DCD   GPIO4_Combined_16_31_IRQHandler                 ; Combined interrupt indication for GPIO4 signal 16 throughout 31
            DCD   GPIO5_Combined_0_15_IRQHandler                  ; Combined interrupt indication for GPIO5 signal 0 throughout 15
            DCD   GPIO5_Combined_16_31_IRQHandler                 ; Combined interrupt indication for GPIO5 signal 16 throughout 31
            DCD   FLEXIO1_IRQHandler                              ; FLEXIO1 interrupt
            DCD   FLEXIO2_IRQHandler                              ; FLEXIO2 interrupt
            DCD   WDOG1_IRQHandler                                ; WDOG1 interrupt
            DCD   RTWDOG3_IRQHandler                              ; RTWDOG3 interrupt
            DCD   EWM_IRQHandler                                  ; EWM interrupt
            DCD   OCOTP_READ_FUSE_ERROR_IRQHandler                ; OCOTP read fuse error interrupt
            DCD   OCOTP_READ_DONE_ERROR_IRQHandler                ; OCOTP read fuse done interrupt
            DCD   GPC_IRQHandler                                  ; GPC interrupt
            DCD   MUA_IRQHandler                                  ; MUA interrupt
            DCD   GPT1_IRQHandler                                 ; GPT1 interrupt
            DCD   GPT2_IRQHandler                                 ; GPT2 interrupt
            DCD   GPT3_IRQHandler                                 ; GPT3 interrupt
            DCD   GPT4_IRQHandler                                 ; GPT4 interrupt
            DCD   GPT5_IRQHandler                                 ; GPT5 interrupt
            DCD   GPT6_IRQHandler                                 ; GPT6 interrupt
            DCD   PWM1_0_IRQHandler                               ; PWM1 capture 0, compare 0, or reload 0 interrupt
            DCD   PWM1_1_IRQHandler                               ; PWM1 capture 1, compare 1, or reload 0 interrupt
            DCD   PWM1_2_IRQHandler                               ; PWM1 capture 2, compare 2, or reload 0 interrupt
            DCD   PWM1_3_IRQHandler                               ; PWM1 capture 3, compare 3, or reload 0 interrupt
            DCD   PWM1_FAULT_IRQHandler                           ; PWM1 fault or reload error interrupt
            DCD   FLEXSPI1_IRQHandler                             ; FlexSPI1 interrupt
            DCD   FLEXSPI2_IRQHandler                             ; FlexSPI2 interrupt
            DCD   SEMC_IRQHandler                                 ; SEMC interrupt
            DCD   USDHC1_IRQHandler                               ; USDHC1 interrupt
            DCD   USDHC2_IRQHandler                               ; USDHC2 interrupt
            DCD   USB_OTG2_IRQHandler                             ; USBO2 USB OTG2
            DCD   USB_OTG1_IRQHandler                             ; USBO2 USB OTG1
            DCD   ENET_IRQHandler                                 ; ENET interrupt
            DCD   ENET_1588_Timer_IRQHandler                      ; ENET_1588_Timer interrupt
            DCD   ENET_MAC0_Tx_Rx_Done_0_IRQHandler               ; ENET 1G MAC0 transmit;receive done 0
            DCD   ENET_MAC0_Tx_Rx_Done_1_IRQHandler               ; ENET 1G MAC0 transmit;receive done 1
            DCD   ENET_1G_IRQHandler                              ; ENET 1G interrupt
            DCD   ENET_1G_1588_Timer_IRQHandler                   ; ENET_1G_1588_Timer interrupt
            DCD   XBAR1_IRQ_0_1_IRQHandler                        ; XBAR1 interrupt
            DCD   XBAR1_IRQ_2_3_IRQHandler                        ; XBAR1 interrupt
            DCD   ADC_ETC_IRQ0_IRQHandler                         ; ADCETC IRQ0 interrupt
            DCD   ADC_ETC_IRQ1_IRQHandler                         ; ADCETC IRQ1 interrupt
            DCD   ADC_ETC_IRQ2_IRQHandler                         ; ADCETC IRQ2 interrupt
            DCD   ADC_ETC_IRQ3_IRQHandler                         ; ADCETC IRQ3 interrupt
            DCD   ADC_ETC_ERROR_IRQ_IRQHandler                    ; ADCETC Error IRQ interrupt
            DCD   Reserved166_IRQHandler                          ; Reserved interrupt
            DCD   Reserved167_IRQHandler                          ; Reserved interrupt
            DCD   Reserved168_IRQHandler                          ; Reserved interrupt
            DCD   Reserved169_IRQHandler                          ; Reserved interrupt
            DCD   Reserved170_IRQHandler                          ; Reserved interrupt
            DCD   PIT1_IRQHandler                                 ; PIT1 interrupt
            DCD   PIT2_IRQHandler                                 ; PIT2 interrupt
            DCD   ACMP1_IRQHandler                                ; ACMP interrupt
            DCD   ACMP2_IRQHandler                                ; ACMP interrupt
            DCD   ACMP3_IRQHandler                                ; ACMP interrupt
            DCD   ACMP4_IRQHandler                                ; ACMP interrupt
            DCD   Reserved177_IRQHandler                          ; Reserved interrupt
            DCD   Reserved178_IRQHandler                          ; Reserved interrupt
            DCD   Reserved179_IRQHandler                          ; Reserved interrupt
            DCD   Reserved180_IRQHandler                          ; Reserved interrupt
            DCD   ENC1_IRQHandler                                 ; ENC1 interrupt
            DCD   ENC2_IRQHandler                                 ; ENC2 interrupt
            DCD   ENC3_IRQHandler                                 ; ENC3 interrupt
            DCD   ENC4_IRQHandler                                 ; ENC4 interrupt
            DCD   Reserved185_IRQHandler                          ; Reserved interrupt
            DCD   Reserved186_IRQHandler                          ; Reserved interrupt
            DCD   TMR1_IRQHandler                                 ; TMR1 interrupt
            DCD   TMR2_IRQHandler                                 ; TMR2 interrupt
            DCD   TMR3_IRQHandler                                 ; TMR3 interrupt
            DCD   TMR4_IRQHandler                                 ; TMR4 interrupt
            DCD   SEMA4_CP0_IRQHandler                            ; SEMA4 CP0 interrupt
            DCD   SEMA4_CP1_IRQHandler                            ; SEMA4 CP1 interrupt
            DCD   PWM2_0_IRQHandler                               ; PWM2 capture 0, compare 0, or reload 0 interrupt
            DCD   PWM2_1_IRQHandler                               ; PWM2 capture 1, compare 1, or reload 0 interrupt
            DCD   PWM2_2_IRQHandler                               ; PWM2 capture 2, compare 2, or reload 0 interrupt
            DCD   PWM2_3_IRQHandler                               ; PWM2 capture 3, compare 3, or reload 0 interrupt
            DCD   PWM2_FAULT_IRQHandler                           ; PWM2 fault or reload error interrupt
            DCD   PWM3_0_IRQHandler                               ; PWM3 capture 0, compare 0, or reload 0 interrupt
            DCD   PWM3_1_IRQHandler                               ; PWM3 capture 1, compare 1, or reload 0 interrupt
            DCD   PWM3_2_IRQHandler                               ; PWM3 capture 2, compare 2, or reload 0 interrupt
            DCD   PWM3_3_IRQHandler                               ; PWM3 capture 3, compare 3, or reload 0 interrupt
            DCD   PWM3_FAULT_IRQHandler                           ; PWM3 fault or reload error interrupt
            DCD   PWM4_0_IRQHandler                               ; PWM4 capture 0, compare 0, or reload 0 interrupt
            DCD   PWM4_1_IRQHandler                               ; PWM4 capture 1, compare 1, or reload 0 interrupt
            DCD   PWM4_2_IRQHandler                               ; PWM4 capture 2, compare 2, or reload 0 interrupt
            DCD   PWM4_3_IRQHandler                               ; PWM4 capture 3, compare 3, or reload 0 interrupt
            DCD   PWM4_FAULT_IRQHandler                           ; PWM4 fault or reload error interrupt
            DCD   Reserved208_IRQHandler                          ; Reserved interrupt
            DCD   Reserved209_IRQHandler                          ; Reserved interrupt
            DCD   Reserved210_IRQHandler                          ; Reserved interrupt
            DCD   Reserved211_IRQHandler                          ; Reserved interrupt
            DCD   Reserved212_IRQHandler                          ; Reserved interrupt
            DCD   Reserved213_IRQHandler                          ; Reserved interrupt
            DCD   Reserved214_IRQHandler                          ; Reserved interrupt
            DCD   Reserved215_IRQHandler                          ; Reserved interrupt
            DCD   HWVAD_EVENT_IRQHandler                          ; HWVAD event interrupt
            DCD   HWVAD_ERROR_IRQHandler                          ; HWVAD error interrupt
            DCD   PDM_EVENT_IRQHandler                            ; PDM event interrupt
            DCD   PDM_ERROR_IRQHandler                            ; PDM error interrupt
            DCD   EMVSIM1_IRQHandler                              ; EMVSIM1 interrupt
            DCD   EMVSIM2_IRQHandler                              ; EMVSIM2 interrupt
            DCD   MECC1_INT_IRQHandler                            ; MECC1 int
            DCD   MECC1_FATAL_INT_IRQHandler                      ; MECC1 fatal int
            DCD   MECC2_INT_IRQHandler                            ; MECC2 int
            DCD   MECC2_FATAL_INT_IRQHandler                      ; MECC2 fatal int
            DCD   XECC_FLEXSPI1_INT_IRQHandler                    ; XECC int
            DCD   XECC_FLEXSPI1_FATAL_INT_IRQHandler              ; XECC fatal int
            DCD   XECC_FLEXSPI2_INT_IRQHandler                    ; XECC int
            DCD   XECC_FLEXSPI2_FATAL_INT_IRQHandler              ; XECC fatal int
            DCD   XECC_SEMC_INT_IRQHandler                        ; XECC int
            DCD   XECC_SEMC_FATAL_INT_IRQHandler                  ; XECC fatal int
            DCD   ENET_QOS_IRQHandler                             ; ENET_QOS interrupt
            DCD   ENET_QOS_PMT_IRQHandler                         ; ENET_QOS_PMT interrupt
            DCD   DefaultISR                                      ; 234
            DCD   DefaultISR                                      ; 235
            DCD   DefaultISR                                      ; 236
            DCD   DefaultISR                                      ; 237
            DCD   DefaultISR                                      ; 238
            DCD   DefaultISR                                      ; 239
            DCD   DefaultISR                                      ; 240
            DCD   DefaultISR                                      ; 241
            DCD   DefaultISR                                      ; 242
            DCD   DefaultISR                                      ; 243
            DCD   DefaultISR                                      ; 244
            DCD   DefaultISR                                      ; 245
            DCD   DefaultISR                                      ; 246
            DCD   DefaultISR                                      ; 247
            DCD   DefaultISR                                      ; 248
            DCD   DefaultISR                                      ; 249
            DCD   DefaultISR                                      ; 250
            DCD   DefaultISR                                      ; 251
            DCD   DefaultISR                                      ; 252
            DCD   DefaultISR                                      ; 253
            DCD   DefaultISR                                      ; 254
            DCD   0xFFFFFFFF                                      ; Reserved for user TRIM value
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                CPSID   I               ; Mask interrupts
                LDR     R0, =0xE000ED08
                LDR     R1, =__Vectors
                STR     R1, [R0]
                LDR     R2, [R1]
                MSR     MSP, R2
                LDR     R0, =SystemInit
                BLX     R0
                CPSIE   i               ; Unmask interrupts
                LDR     R0, =__main
                BX      R0
                ENDP

  
NMI_Handler\
                PROC
                EXPORT  NMI_Handler         [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler\
                PROC
                EXPORT  SVC_Handler         [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler         [WEAK]
                B       .
                ENDP
SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler         [WEAK]
                B       .
                ENDP
DMA0_DMA16_IRQHandler\
                PROC
                EXPORT  DMA0_DMA16_IRQHandler         [WEAK]
                LDR     R0, =DMA0_DMA16_DriverIRQHandler
                BX      R0
                ENDP

DMA1_DMA17_IRQHandler\
                PROC
                EXPORT  DMA1_DMA17_IRQHandler         [WEAK]
                LDR     R0, =DMA1_DMA17_DriverIRQHandler
                BX      R0
                ENDP

DMA2_DMA18_IRQHandler\
                PROC
                EXPORT  DMA2_DMA18_IRQHandler         [WEAK]
                LDR     R0, =DMA2_DMA18_DriverIRQHandler
                BX      R0
                ENDP

DMA3_DMA19_IRQHandler\
                PROC
                EXPORT  DMA3_DMA19_IRQHandler         [WEAK]
                LDR     R0, =DMA3_DMA19_DriverIRQHandler
                BX      R0
                ENDP

DMA4_DMA20_IRQHandler\
                PROC
                EXPORT  DMA4_DMA20_IRQHandler         [WEAK]
                LDR     R0, =DMA4_DMA20_DriverIRQHandler
                BX      R0
                ENDP

DMA5_DMA21_IRQHandler\
                PROC
                EXPORT  DMA5_DMA21_IRQHandler         [WEAK]
                LDR     R0, =DMA5_DMA21_DriverIRQHandler
                BX      R0
                ENDP

DMA6_DMA22_IRQHandler\
                PROC
                EXPORT  DMA6_DMA22_IRQHandler         [WEAK]
                LDR     R0, =DMA6_DMA22_DriverIRQHandler
                BX      R0
                ENDP

DMA7_DMA23_IRQHandler\
                PROC
                EXPORT  DMA7_DMA23_IRQHandler         [WEAK]
                LDR     R0, =DMA7_DMA23_DriverIRQHandler
                BX      R0
                ENDP

DMA8_DMA24_IRQHandler\
                PROC
                EXPORT  DMA8_DMA24_IRQHandler         [WEAK]
                LDR     R0, =DMA8_DMA24_DriverIRQHandler
                BX      R0
                ENDP

DMA9_DMA25_IRQHandler\
                PROC
                EXPORT  DMA9_DMA25_IRQHandler         [WEAK]
                LDR     R0, =DMA9_DMA25_DriverIRQHandler
                BX      R0
                ENDP

DMA10_DMA26_IRQHandler\
                PROC
                EXPORT  DMA10_DMA26_IRQHandler         [WEAK]
                LDR     R0, =DMA10_DMA26_DriverIRQHandler
                BX      R0
                ENDP

DMA11_DMA27_IRQHandler\
                PROC
                EXPORT  DMA11_DMA27_IRQHandler         [WEAK]
                LDR     R0, =DMA11_DMA27_DriverIRQHandler
                BX      R0
                ENDP

DMA12_DMA28_IRQHandler\
                PROC
                EXPORT  DMA12_DMA28_IRQHandler         [WEAK]
                LDR     R0, =DMA12_DMA28_DriverIRQHandler
                BX      R0
                ENDP

DMA13_DMA29_IRQHandler\
                PROC
                EXPORT  DMA13_DMA29_IRQHandler         [WEAK]
                LDR     R0, =DMA13_DMA29_DriverIRQHandler
                BX      R0
                ENDP

DMA14_DMA30_IRQHandler\
                PROC
                EXPORT  DMA14_DMA30_IRQHandler         [WEAK]
                LDR     R0, =DMA14_DMA30_DriverIRQHandler
                BX      R0
                ENDP

DMA15_DMA31_IRQHandler\
                PROC
                EXPORT  DMA15_DMA31_IRQHandler         [WEAK]
                LDR     R0, =DMA15_DMA31_DriverIRQHandler
                BX      R0
                ENDP

DMA_ERROR_IRQHandler\
                PROC
                EXPORT  DMA_ERROR_IRQHandler         [WEAK]
                LDR     R0, =DMA_ERROR_DriverIRQHandler
                BX      R0
                ENDP

LPUART1_IRQHandler\
                PROC
                EXPORT  LPUART1_IRQHandler         [WEAK]
                LDR     R0, =LPUART1_DriverIRQHandler
                BX      R0
                ENDP

LPUART2_IRQHandler\
                PROC
                EXPORT  LPUART2_IRQHandler         [WEAK]
                LDR     R0, =LPUART2_DriverIRQHandler
                BX      R0
                ENDP

LPUART3_IRQHandler\
                PROC
                EXPORT  LPUART3_IRQHandler         [WEAK]
                LDR     R0, =LPUART3_DriverIRQHandler
                BX      R0
                ENDP

LPUART4_IRQHandler\
                PROC
                EXPORT  LPUART4_IRQHandler         [WEAK]
                LDR     R0, =LPUART4_DriverIRQHandler
                BX      R0
                ENDP

LPUART5_IRQHandler\
                PROC
                EXPORT  LPUART5_IRQHandler         [WEAK]
                LDR     R0, =LPUART5_DriverIRQHandler
                BX      R0
                ENDP

LPUART6_IRQHandler\
                PROC
                EXPORT  LPUART6_IRQHandler         [WEAK]
                LDR     R0, =LPUART6_DriverIRQHandler
                BX      R0
                ENDP

LPUART7_IRQHandler\
                PROC
                EXPORT  LPUART7_IRQHandler         [WEAK]
                LDR     R0, =LPUART7_DriverIRQHandler
                BX      R0
                ENDP

LPUART8_IRQHandler\
                PROC
                EXPORT  LPUART8_IRQHandler         [WEAK]
                LDR     R0, =LPUART8_DriverIRQHandler
                BX      R0
                ENDP

LPUART9_IRQHandler\
                PROC
                EXPORT  LPUART9_IRQHandler         [WEAK]
                LDR     R0, =LPUART9_DriverIRQHandler
                BX      R0
                ENDP

LPUART10_IRQHandler\
                PROC
                EXPORT  LPUART10_IRQHandler         [WEAK]
                LDR     R0, =LPUART10_DriverIRQHandler
                BX      R0
                ENDP

LPUART11_IRQHandler\
                PROC
                EXPORT  LPUART11_IRQHandler         [WEAK]
                LDR     R0, =LPUART11_DriverIRQHandler
                BX      R0
                ENDP

LPUART12_IRQHandler\
                PROC
                EXPORT  LPUART12_IRQHandler         [WEAK]
                LDR     R0, =LPUART12_DriverIRQHandler
                BX      R0
                ENDP

LPI2C1_IRQHandler\
                PROC
                EXPORT  LPI2C1_IRQHandler         [WEAK]
                LDR     R0, =LPI2C1_DriverIRQHandler
                BX      R0
                ENDP

LPI2C2_IRQHandler\
                PROC
                EXPORT  LPI2C2_IRQHandler         [WEAK]
                LDR     R0, =LPI2C2_DriverIRQHandler
                BX      R0
                ENDP

LPI2C3_IRQHandler\
                PROC
                EXPORT  LPI2C3_IRQHandler         [WEAK]
                LDR     R0, =LPI2C3_DriverIRQHandler
                BX      R0
                ENDP

LPI2C4_IRQHandler\
                PROC
                EXPORT  LPI2C4_IRQHandler         [WEAK]
                LDR     R0, =LPI2C4_DriverIRQHandler
                BX      R0
                ENDP

LPI2C5_IRQHandler\
                PROC
                EXPORT  LPI2C5_IRQHandler         [WEAK]
                LDR     R0, =LPI2C5_DriverIRQHandler
                BX      R0
                ENDP

LPI2C6_IRQHandler\
                PROC
                EXPORT  LPI2C6_IRQHandler         [WEAK]
                LDR     R0, =LPI2C6_DriverIRQHandler
                BX      R0
                ENDP

LPSPI1_IRQHandler\
                PROC
                EXPORT  LPSPI1_IRQHandler         [WEAK]
                LDR     R0, =LPSPI1_DriverIRQHandler
                BX      R0
                ENDP

LPSPI2_IRQHandler\
                PROC
                EXPORT  LPSPI2_IRQHandler         [WEAK]
                LDR     R0, =LPSPI2_DriverIRQHandler
                BX      R0
                ENDP

LPSPI3_IRQHandler\
                PROC
                EXPORT  LPSPI3_IRQHandler         [WEAK]
                LDR     R0, =LPSPI3_DriverIRQHandler
                BX      R0
                ENDP

LPSPI4_IRQHandler\
                PROC
                EXPORT  LPSPI4_IRQHandler         [WEAK]
                LDR     R0, =LPSPI4_DriverIRQHandler
                BX      R0
                ENDP

LPSPI5_IRQHandler\
                PROC
                EXPORT  LPSPI5_IRQHandler         [WEAK]
                LDR     R0, =LPSPI5_DriverIRQHandler
                BX      R0
                ENDP

LPSPI6_IRQHandler\
                PROC
                EXPORT  LPSPI6_IRQHandler         [WEAK]
                LDR     R0, =LPSPI6_DriverIRQHandler
                BX      R0
                ENDP

CAN1_IRQHandler\
                PROC
                EXPORT  CAN1_IRQHandler         [WEAK]
                LDR     R0, =CAN1_DriverIRQHandler
                BX      R0
                ENDP

CAN1_ERROR_IRQHandler\
                PROC
                EXPORT  CAN1_ERROR_IRQHandler         [WEAK]
                LDR     R0, =CAN1_ERROR_DriverIRQHandler
                BX      R0
                ENDP

CAN2_IRQHandler\
                PROC
                EXPORT  CAN2_IRQHandler         [WEAK]
                LDR     R0, =CAN2_DriverIRQHandler
                BX      R0
                ENDP

CAN2_ERROR_IRQHandler\
                PROC
                EXPORT  CAN2_ERROR_IRQHandler         [WEAK]
                LDR     R0, =CAN2_ERROR_DriverIRQHandler
                BX      R0
                ENDP

CAN3_IRQHandler\
                PROC
                EXPORT  CAN3_IRQHandler         [WEAK]
                LDR     R0, =CAN3_DriverIRQHandler
                BX      R0
                ENDP

CAN3_ERROR_IRQHandler\
                PROC
                EXPORT  CAN3_ERROR_IRQHandler         [WEAK]
                LDR     R0, =CAN3_ERROR_DriverIRQHandler
                BX      R0
                ENDP

CDOG_IRQHandler\
                PROC
                EXPORT  CDOG_IRQHandler         [WEAK]
                LDR     R0, =CDOG_DriverIRQHandler
                BX      R0
                ENDP

SAI1_IRQHandler\
                PROC
                EXPORT  SAI1_IRQHandler         [WEAK]
                LDR     R0, =SAI1_DriverIRQHandler
                BX      R0
                ENDP

SAI2_IRQHandler\
                PROC
                EXPORT  SAI2_IRQHandler         [WEAK]
                LDR     R0, =SAI2_DriverIRQHandler
                BX      R0
                ENDP

SAI3_RX_IRQHandler\
                PROC
                EXPORT  SAI3_RX_IRQHandler         [WEAK]
                LDR     R0, =SAI3_RX_DriverIRQHandler
                BX      R0
                ENDP

SAI3_TX_IRQHandler\
                PROC
                EXPORT  SAI3_TX_IRQHandler         [WEAK]
                LDR     R0, =SAI3_TX_DriverIRQHandler
                BX      R0
                ENDP

SAI4_RX_IRQHandler\
                PROC
                EXPORT  SAI4_RX_IRQHandler         [WEAK]
                LDR     R0, =SAI4_RX_DriverIRQHandler
                BX      R0
                ENDP

SAI4_TX_IRQHandler\
                PROC
                EXPORT  SAI4_TX_IRQHandler         [WEAK]
                LDR     R0, =SAI4_TX_DriverIRQHandler
                BX      R0
                ENDP

SPDIF_IRQHandler\
                PROC
                EXPORT  SPDIF_IRQHandler         [WEAK]
                LDR     R0, =SPDIF_DriverIRQHandler
                BX      R0
                ENDP

ASRC_IRQHandler\
                PROC
                EXPORT  ASRC_IRQHandler         [WEAK]
                LDR     R0, =ASRC_DriverIRQHandler
                BX      R0
                ENDP

FLEXIO1_IRQHandler\
                PROC
                EXPORT  FLEXIO1_IRQHandler         [WEAK]
                LDR     R0, =FLEXIO1_DriverIRQHandler
                BX      R0
                ENDP

FLEXIO2_IRQHandler\
                PROC
                EXPORT  FLEXIO2_IRQHandler         [WEAK]
                LDR     R0, =FLEXIO2_DriverIRQHandler
                BX      R0
                ENDP

FLEXSPI1_IRQHandler\
                PROC
                EXPORT  FLEXSPI1_IRQHandler         [WEAK]
                LDR     R0, =FLEXSPI1_DriverIRQHandler
                BX      R0
                ENDP

FLEXSPI2_IRQHandler\
                PROC
                EXPORT  FLEXSPI2_IRQHandler         [WEAK]
                LDR     R0, =FLEXSPI2_DriverIRQHandler
                BX      R0
                ENDP

USDHC1_IRQHandler\
                PROC
                EXPORT  USDHC1_IRQHandler         [WEAK]
                LDR     R0, =USDHC1_DriverIRQHandler
                BX      R0
                ENDP

USDHC2_IRQHandler\
                PROC
                EXPORT  USDHC2_IRQHandler         [WEAK]
                LDR     R0, =USDHC2_DriverIRQHandler
                BX      R0
                ENDP

ENET_IRQHandler\
                PROC
                EXPORT  ENET_IRQHandler         [WEAK]
                LDR     R0, =ENET_DriverIRQHandler
                BX      R0
                ENDP

ENET_1588_Timer_IRQHandler\
                PROC
                EXPORT  ENET_1588_Timer_IRQHandler         [WEAK]
                LDR     R0, =ENET_1588_Timer_DriverIRQHandler
                BX      R0
                ENDP

ENET_MAC0_Tx_Rx_Done_0_IRQHandler\
                PROC
                EXPORT  ENET_MAC0_Tx_Rx_Done_0_IRQHandler         [WEAK]
                LDR     R0, =ENET_MAC0_Tx_Rx_Done_0_DriverIRQHandler
                BX      R0
                ENDP

ENET_MAC0_Tx_Rx_Done_1_IRQHandler\
                PROC
                EXPORT  ENET_MAC0_Tx_Rx_Done_1_IRQHandler         [WEAK]
                LDR     R0, =ENET_MAC0_Tx_Rx_Done_1_DriverIRQHandler
                BX      R0
                ENDP

ENET_1G_IRQHandler\
                PROC
                EXPORT  ENET_1G_IRQHandler         [WEAK]
                LDR     R0, =ENET_1G_DriverIRQHandler
                BX      R0
                ENDP

ENET_1G_1588_Timer_IRQHandler\
                PROC
                EXPORT  ENET_1G_1588_Timer_IRQHandler         [WEAK]
                LDR     R0, =ENET_1G_1588_Timer_DriverIRQHandler
                BX      R0
                ENDP

HWVAD_EVENT_IRQHandler\
                PROC
                EXPORT  HWVAD_EVENT_IRQHandler         [WEAK]
                LDR     R0, =HWVAD_EVENT_DriverIRQHandler
                BX      R0
                ENDP

HWVAD_ERROR_IRQHandler\
                PROC
                EXPORT  HWVAD_ERROR_IRQHandler         [WEAK]
                LDR     R0, =HWVAD_ERROR_DriverIRQHandler
                BX      R0
                ENDP

PDM_EVENT_IRQHandler\
                PROC
                EXPORT  PDM_EVENT_IRQHandler         [WEAK]
                LDR     R0, =PDM_EVENT_DriverIRQHandler
                BX      R0
                ENDP

PDM_ERROR_IRQHandler\
                PROC
                EXPORT  PDM_ERROR_IRQHandler         [WEAK]
                LDR     R0, =PDM_ERROR_DriverIRQHandler
                BX      R0
                ENDP

XECC_FLEXSPI1_INT_IRQHandler\
                PROC
                EXPORT  XECC_FLEXSPI1_INT_IRQHandler         [WEAK]
                LDR     R0, =XECC_FLEXSPI1_INT_DriverIRQHandler
                BX      R0
                ENDP

XECC_FLEXSPI1_FATAL_INT_IRQHandler\
                PROC
                EXPORT  XECC_FLEXSPI1_FATAL_INT_IRQHandler         [WEAK]
                LDR     R0, =XECC_FLEXSPI1_FATAL_INT_DriverIRQHandler
                BX      R0
                ENDP

XECC_FLEXSPI2_INT_IRQHandler\
                PROC
                EXPORT  XECC_FLEXSPI2_INT_IRQHandler         [WEAK]
                LDR     R0, =XECC_FLEXSPI2_INT_DriverIRQHandler
                BX      R0
                ENDP

XECC_FLEXSPI2_FATAL_INT_IRQHandler\
                PROC
                EXPORT  XECC_FLEXSPI2_FATAL_INT_IRQHandler         [WEAK]
                LDR     R0, =XECC_FLEXSPI2_FATAL_INT_DriverIRQHandler
                BX      R0
                ENDP

ENET_QOS_IRQHandler\
                PROC
                EXPORT  ENET_QOS_IRQHandler         [WEAK]
                LDR     R0, =ENET_QOS_DriverIRQHandler
                BX      R0
                ENDP

ENET_QOS_PMT_IRQHandler\
                PROC
                EXPORT  ENET_QOS_PMT_IRQHandler         [WEAK]
                LDR     R0, =ENET_QOS_PMT_DriverIRQHandler
                BX      R0
                ENDP




Default_Handler\
                PROC
                EXPORT    MemManage_Handler [WEAK]
                EXPORT    BusFault_Handler [WEAK]
                EXPORT    UsageFault_Handler [WEAK]
                EXPORT    DebugMon_Handler [WEAK]
                EXPORT    DMA0_DMA16_DriverIRQHandler [WEAK]
                EXPORT    DMA1_DMA17_DriverIRQHandler [WEAK]
                EXPORT    DMA2_DMA18_DriverIRQHandler [WEAK]
                EXPORT    DMA3_DMA19_DriverIRQHandler [WEAK]
                EXPORT    DMA4_DMA20_DriverIRQHandler [WEAK]
                EXPORT    DMA5_DMA21_DriverIRQHandler [WEAK]
                EXPORT    DMA6_DMA22_DriverIRQHandler [WEAK]
                EXPORT    DMA7_DMA23_DriverIRQHandler [WEAK]
                EXPORT    DMA8_DMA24_DriverIRQHandler [WEAK]
                EXPORT    DMA9_DMA25_DriverIRQHandler [WEAK]
                EXPORT    DMA10_DMA26_DriverIRQHandler [WEAK]
                EXPORT    DMA11_DMA27_DriverIRQHandler [WEAK]
                EXPORT    DMA12_DMA28_DriverIRQHandler [WEAK]
                EXPORT    DMA13_DMA29_DriverIRQHandler [WEAK]
                EXPORT    DMA14_DMA30_DriverIRQHandler [WEAK]
                EXPORT    DMA15_DMA31_DriverIRQHandler [WEAK]
                EXPORT    DMA_ERROR_DriverIRQHandler [WEAK]
                EXPORT    CTI_TRIGGER_OUT0_IRQHandler [WEAK]
                EXPORT    CTI_TRIGGER_OUT1_IRQHandler [WEAK]
                EXPORT    CORE_IRQHandler [WEAK]
                EXPORT    LPUART1_DriverIRQHandler [WEAK]
                EXPORT    LPUART2_DriverIRQHandler [WEAK]
                EXPORT    LPUART3_DriverIRQHandler [WEAK]
                EXPORT    LPUART4_DriverIRQHandler [WEAK]
                EXPORT    LPUART5_DriverIRQHandler [WEAK]
                EXPORT    LPUART6_DriverIRQHandler [WEAK]
                EXPORT    LPUART7_DriverIRQHandler [WEAK]
                EXPORT    LPUART8_DriverIRQHandler [WEAK]
                EXPORT    LPUART9_DriverIRQHandler [WEAK]
                EXPORT    LPUART10_DriverIRQHandler [WEAK]
                EXPORT    LPUART11_DriverIRQHandler [WEAK]
                EXPORT    LPUART12_DriverIRQHandler [WEAK]
                EXPORT    LPI2C1_DriverIRQHandler [WEAK]
                EXPORT    LPI2C2_DriverIRQHandler [WEAK]
                EXPORT    LPI2C3_DriverIRQHandler [WEAK]
                EXPORT    LPI2C4_DriverIRQHandler [WEAK]
                EXPORT    LPI2C5_DriverIRQHandler [WEAK]
                EXPORT    LPI2C6_DriverIRQHandler [WEAK]
                EXPORT    LPSPI1_DriverIRQHandler [WEAK]
                EXPORT    LPSPI2_DriverIRQHandler [WEAK]
                EXPORT    LPSPI3_DriverIRQHandler [WEAK]
                EXPORT    LPSPI4_DriverIRQHandler [WEAK]
                EXPORT    LPSPI5_DriverIRQHandler [WEAK]
                EXPORT    LPSPI6_DriverIRQHandler [WEAK]
                EXPORT    CAN1_DriverIRQHandler [WEAK]
                EXPORT    CAN1_ERROR_DriverIRQHandler [WEAK]
                EXPORT    CAN2_DriverIRQHandler [WEAK]
                EXPORT    CAN2_ERROR_DriverIRQHandler [WEAK]
                EXPORT    CAN3_DriverIRQHandler [WEAK]
                EXPORT    CAN3_ERROR_DriverIRQHandler [WEAK]
                EXPORT    FLEXRAM_IRQHandler [WEAK]
                EXPORT    KPP_IRQHandler [WEAK]
                EXPORT    Reserved68_IRQHandler [WEAK]
                EXPORT    GPR_IRQ_IRQHandler [WEAK]
                EXPORT    eLCDIF_IRQHandler [WEAK]
                EXPORT    LCDIFv2_IRQHandler [WEAK]
                EXPORT    CSI_IRQHandler [WEAK]
                EXPORT    PXP_IRQHandler [WEAK]
                EXPORT    MIPI_CSI_IRQHandler [WEAK]
                EXPORT    MIPI_DSI_IRQHandler [WEAK]
                EXPORT    GPU2D_IRQHandler [WEAK]
                EXPORT    GPIO6_Combined_0_15_IRQHandler [WEAK]
                EXPORT    GPIO6_Combined_16_31_IRQHandler [WEAK]
                EXPORT    DAC_IRQHandler [WEAK]
                EXPORT    KEY_MANAGER_IRQHandler [WEAK]
                EXPORT    WDOG2_IRQHandler [WEAK]
                EXPORT    SNVS_HP_NON_TZ_IRQHandler [WEAK]
                EXPORT    SNVS_HP_TZ_IRQHandler [WEAK]
                EXPORT    SNVS_PULSE_EVENT_IRQHandler [WEAK]
                EXPORT    CAAM_IRQ0_IRQHandler [WEAK]
                EXPORT    CAAM_IRQ1_IRQHandler [WEAK]
                EXPORT    CAAM_IRQ2_IRQHandler [WEAK]
                EXPORT    CAAM_IRQ3_IRQHandler [WEAK]
                EXPORT    CAAM_RECORVE_ERRPR_IRQHandler [WEAK]
                EXPORT    CAAM_RTIC_IRQHandler [WEAK]
                EXPORT    CDOG_DriverIRQHandler [WEAK]
                EXPORT    SAI1_DriverIRQHandler [WEAK]
                EXPORT    SAI2_DriverIRQHandler [WEAK]
                EXPORT    SAI3_RX_DriverIRQHandler [WEAK]
                EXPORT    SAI3_TX_DriverIRQHandler [WEAK]
                EXPORT    SAI4_RX_DriverIRQHandler [WEAK]
                EXPORT    SAI4_TX_DriverIRQHandler [WEAK] 
                EXPORT    SPDIF_DriverIRQHandler [WEAK]
                EXPORT    ANATOP_TEMP_INT_IRQHandler [WEAK]
                EXPORT    ANATOP_TEMP_LOW_HIGH_IRQHandler [WEAK]
                EXPORT    ANATOP_TEMP_PANIC_IRQHandler [WEAK]
                EXPORT    ANATOP_LP8_BROWNOUT_IRQHandler [WEAK]
                EXPORT    ANATOP_LP0_BROWNOUT_IRQHandler [WEAK]
                EXPORT    ADC1_IRQHandler [WEAK]
                EXPORT    ADC2_IRQHandler [WEAK]
                EXPORT    USBPHY1_IRQHandler [WEAK]
                EXPORT    USBPHY2_IRQHandler [WEAK]
                EXPORT    RDC_IRQHandler [WEAK]
                EXPORT    GPIO13_Combined_0_31_IRQHandler [WEAK]
                EXPORT    Reserved110_IRQHandler [WEAK]
                EXPORT    DCIC1_IRQHandler [WEAK]
                EXPORT    DCIC2_IRQHandler [WEAK]
                EXPORT    ASRC_DriverIRQHandler [WEAK]
                EXPORT    FLEXRAM_ECC_IRQHandler [WEAK]
                EXPORT    CM7_GPIO2_3_IRQHandler [WEAK]
                EXPORT    GPIO1_Combined_0_15_IRQHandler [WEAK]
                EXPORT    GPIO1_Combined_16_31_IRQHandler [WEAK]
                EXPORT    GPIO2_Combined_0_15_IRQHandler [WEAK]
                EXPORT    GPIO2_Combined_16_31_IRQHandler [WEAK]
                EXPORT    GPIO3_Combined_0_15_IRQHandler [WEAK]
                EXPORT    GPIO3_Combined_16_31_IRQHandler [WEAK]
                EXPORT    GPIO4_Combined_0_15_IRQHandler [WEAK]
                EXPORT    GPIO4_Combined_16_31_IRQHandler [WEAK]
                EXPORT    GPIO5_Combined_0_15_IRQHandler [WEAK]
                EXPORT    GPIO5_Combined_16_31_IRQHandler [WEAK]
                EXPORT    FLEXIO1_DriverIRQHandler [WEAK]
                EXPORT    FLEXIO2_DriverIRQHandler [WEAK]
                EXPORT    WDOG1_IRQHandler [WEAK]
                EXPORT    RTWDOG3_IRQHandler [WEAK]
                EXPORT    EWM_IRQHandler [WEAK]
                EXPORT    OCOTP_READ_FUSE_ERROR_IRQHandler [WEAK]
                EXPORT    OCOTP_READ_DONE_ERROR_IRQHandler [WEAK]
                EXPORT    GPC_IRQHandler [WEAK]
                EXPORT    MUA_IRQHandler [WEAK]
                EXPORT    GPT1_IRQHandler [WEAK]
                EXPORT    GPT2_IRQHandler [WEAK]
                EXPORT    GPT3_IRQHandler [WEAK]
                EXPORT    GPT4_IRQHandler [WEAK]
                EXPORT    GPT5_IRQHandler [WEAK]
                EXPORT    GPT6_IRQHandler [WEAK]
                EXPORT    PWM1_0_IRQHandler [WEAK]
                EXPORT    PWM1_1_IRQHandler [WEAK]
                EXPORT    PWM1_2_IRQHandler [WEAK]
                EXPORT    PWM1_3_IRQHandler [WEAK]
                EXPORT    PWM1_FAULT_IRQHandler [WEAK]
                EXPORT    FLEXSPI1_DriverIRQHandler [WEAK]
                EXPORT    FLEXSPI2_DriverIRQHandler [WEAK]
                EXPORT    SEMC_IRQHandler [WEAK]
                EXPORT    USDHC1_DriverIRQHandler [WEAK]
                EXPORT    USDHC2_DriverIRQHandler [WEAK]
                EXPORT    USB_OTG2_IRQHandler [WEAK]
                EXPORT    USB_OTG1_IRQHandler [WEAK]
                EXPORT    ENET_DriverIRQHandler [WEAK]
                EXPORT    ENET_1588_Timer_DriverIRQHandler [WEAK]
                EXPORT    ENET_MAC0_Tx_Rx_Done_0_DriverIRQHandler [WEAK]
                EXPORT    ENET_MAC0_Tx_Rx_Done_1_DriverIRQHandler [WEAK]
                EXPORT    ENET_1G_DriverIRQHandler [WEAK]
                EXPORT    ENET_1G_1588_Timer_DriverIRQHandler [WEAK]
                EXPORT    XBAR1_IRQ_0_1_IRQHandler [WEAK]
                EXPORT    XBAR1_IRQ_2_3_IRQHandler [WEAK]
                EXPORT    ADC_ETC_IRQ0_IRQHandler [WEAK]
                EXPORT    ADC_ETC_IRQ1_IRQHandler [WEAK]
                EXPORT    ADC_ETC_IRQ2_IRQHandler [WEAK]
                EXPORT    ADC_ETC_IRQ3_IRQHandler [WEAK]
                EXPORT    ADC_ETC_ERROR_IRQ_IRQHandler [WEAK]
                EXPORT    Reserved166_IRQHandler [WEAK]
                EXPORT    Reserved167_IRQHandler [WEAK]
                EXPORT    Reserved168_IRQHandler [WEAK]
                EXPORT    Reserved169_IRQHandler [WEAK]
                EXPORT    Reserved170_IRQHandler [WEAK]
                EXPORT    PIT1_IRQHandler [WEAK]
                EXPORT    PIT2_IRQHandler [WEAK]
                EXPORT    ACMP1_IRQHandler [WEAK]
                EXPORT    ACMP2_IRQHandler [WEAK]
                EXPORT    ACMP3_IRQHandler [WEAK]
                EXPORT    ACMP4_IRQHandler [WEAK]
                EXPORT    Reserved177_IRQHandler [WEAK]
                EXPORT    Reserved178_IRQHandler [WEAK]
                EXPORT    Reserved179_IRQHandler [WEAK]
                EXPORT    Reserved180_IRQHandler [WEAK]
                EXPORT    ENC1_IRQHandler [WEAK]
                EXPORT    ENC2_IRQHandler [WEAK]
                EXPORT    ENC3_IRQHandler [WEAK]
                EXPORT    ENC4_IRQHandler [WEAK]
                EXPORT    Reserved185_IRQHandler [WEAK]
                EXPORT    Reserved186_IRQHandler [WEAK]
                EXPORT    TMR1_IRQHandler [WEAK]
                EXPORT    TMR2_IRQHandler [WEAK]
                EXPORT    TMR3_IRQHandler [WEAK]
                EXPORT    TMR4_IRQHandler [WEAK]
                EXPORT    SEMA4_CP0_IRQHandler [WEAK]
                EXPORT    SEMA4_CP1_IRQHandler [WEAK]
                EXPORT    PWM2_0_IRQHandler [WEAK]
                EXPORT    PWM2_1_IRQHandler [WEAK]
                EXPORT    PWM2_2_IRQHandler [WEAK]
                EXPORT    PWM2_3_IRQHandler [WEAK]
                EXPORT    PWM2_FAULT_IRQHandler [WEAK]
                EXPORT    PWM3_0_IRQHandler [WEAK]
                EXPORT    PWM3_1_IRQHandler [WEAK]
                EXPORT    PWM3_2_IRQHandler [WEAK]
                EXPORT    PWM3_3_IRQHandler [WEAK]
                EXPORT    PWM3_FAULT_IRQHandler [WEAK]
                EXPORT    PWM4_0_IRQHandler [WEAK]
                EXPORT    PWM4_1_IRQHandler [WEAK]
                EXPORT    PWM4_2_IRQHandler [WEAK]
                EXPORT    PWM4_3_IRQHandler [WEAK]
                EXPORT    PWM4_FAULT_IRQHandler [WEAK]
                EXPORT    Reserved208_IRQHandler [WEAK]
                EXPORT    Reserved209_IRQHandler [WEAK]
                EXPORT    Reserved210_IRQHandler [WEAK]
                EXPORT    Reserved211_IRQHandler [WEAK]
                EXPORT    Reserved212_IRQHandler [WEAK]
                EXPORT    Reserved213_IRQHandler [WEAK]
                EXPORT    Reserved214_IRQHandler [WEAK]
                EXPORT    Reserved215_IRQHandler [WEAK]
                EXPORT    HWVAD_EVENT_DriverIRQHandler [WEAK]
                EXPORT    HWVAD_ERROR_DriverIRQHandler [WEAK]
                EXPORT    PDM_EVENT_DriverIRQHandler [WEAK]
                EXPORT    PDM_ERROR_DriverIRQHandler [WEAK]
                EXPORT    EMVSIM1_IRQHandler [WEAK]
                EXPORT    EMVSIM2_IRQHandler [WEAK]
                EXPORT    MECC1_INT_IRQHandler [WEAK]
                EXPORT    MECC1_FATAL_INT_IRQHandler [WEAK]
                EXPORT    MECC2_INT_IRQHandler [WEAK]
                EXPORT    MECC2_FATAL_INT_IRQHandler [WEAK]
                EXPORT    XECC_FLEXSPI1_INT_DriverIRQHandler [WEAK]
                EXPORT    XECC_FLEXSPI1_FATAL_INT_DriverIRQHandler [WEAK]
                EXPORT    XECC_FLEXSPI2_INT_DriverIRQHandler [WEAK]
                EXPORT    XECC_FLEXSPI2_FATAL_INT_DriverIRQHandler [WEAK]
                EXPORT    XECC_SEMC_INT_IRQHandler [WEAK]
                EXPORT    XECC_SEMC_FATAL_INT_IRQHandler [WEAK]
                EXPORT    ENET_QOS_DriverIRQHandler [WEAK]
                EXPORT    ENET_QOS_PMT_DriverIRQHandler [WEAK]
                EXPORT    DefaultISR         [WEAK]
MemManage_Handler
BusFault_Handler
UsageFault_Handler
DebugMon_Handler
DMA0_DMA16_DriverIRQHandler
DMA1_DMA17_DriverIRQHandler
DMA2_DMA18_DriverIRQHandler
DMA3_DMA19_DriverIRQHandler
DMA4_DMA20_DriverIRQHandler
DMA5_DMA21_DriverIRQHandler
DMA6_DMA22_DriverIRQHandler
DMA7_DMA23_DriverIRQHandler
DMA8_DMA24_DriverIRQHandler
DMA9_DMA25_DriverIRQHandler
DMA10_DMA26_DriverIRQHandler
DMA11_DMA27_DriverIRQHandler
DMA12_DMA28_DriverIRQHandler
DMA13_DMA29_DriverIRQHandler
DMA14_DMA30_DriverIRQHandler
DMA15_DMA31_DriverIRQHandler
DMA_ERROR_DriverIRQHandler
CTI_TRIGGER_OUT0_IRQHandler
CTI_TRIGGER_OUT1_IRQHandler
CORE_IRQHandler
LPUART1_DriverIRQHandler
LPUART2_DriverIRQHandler
LPUART3_DriverIRQHandler
LPUART4_DriverIRQHandler
LPUART5_DriverIRQHandler
LPUART6_DriverIRQHandler
LPUART7_DriverIRQHandler
LPUART8_DriverIRQHandler
LPUART9_DriverIRQHandler
LPUART10_DriverIRQHandler
LPUART11_DriverIRQHandler
LPUART12_DriverIRQHandler
LPI2C1_DriverIRQHandler
LPI2C2_DriverIRQHandler
LPI2C3_DriverIRQHandler
LPI2C4_DriverIRQHandler
LPI2C5_DriverIRQHandler
LPI2C6_DriverIRQHandler
LPSPI1_DriverIRQHandler
LPSPI2_DriverIRQHandler
LPSPI3_DriverIRQHandler
LPSPI4_DriverIRQHandler
LPSPI5_DriverIRQHandler
LPSPI6_DriverIRQHandler
CAN1_DriverIRQHandler
CAN1_ERROR_DriverIRQHandler
CAN2_DriverIRQHandler
CAN2_ERROR_DriverIRQHandler
CAN3_DriverIRQHandler
CAN3_ERROR_DriverIRQHandler
FLEXRAM_IRQHandler
KPP_IRQHandler
Reserved68_IRQHandler
GPR_IRQ_IRQHandler
eLCDIF_IRQHandler
LCDIFv2_IRQHandler
CSI_IRQHandler
PXP_IRQHandler
MIPI_CSI_IRQHandler
MIPI_DSI_IRQHandler
GPU2D_IRQHandler
GPIO6_Combined_0_15_IRQHandler
GPIO6_Combined_16_31_IRQHandler
DAC_IRQHandler
KEY_MANAGER_IRQHandler
WDOG2_IRQHandler
SNVS_HP_NON_TZ_IRQHandler
SNVS_HP_TZ_IRQHandler
SNVS_PULSE_EVENT_IRQHandler
CAAM_IRQ0_IRQHandler
CAAM_IRQ1_IRQHandler
CAAM_IRQ2_IRQHandler
CAAM_IRQ3_IRQHandler
CAAM_RECORVE_ERRPR_IRQHandler
CAAM_RTIC_IRQHandler
CDOG_DriverIRQHandler
SAI1_DriverIRQHandler
SAI2_DriverIRQHandler
SAI3_RX_DriverIRQHandler
SAI3_TX_DriverIRQHandler
SAI4_RX_DriverIRQHandler
SAI4_TX_DriverIRQHandler
SPDIF_DriverIRQHandler
ANATOP_TEMP_INT_IRQHandler
ANATOP_TEMP_LOW_HIGH_IRQHandler
ANATOP_TEMP_PANIC_IRQHandler
ANATOP_LP8_BROWNOUT_IRQHandler
ANATOP_LP0_BROWNOUT_IRQHandler
ADC1_IRQHandler
ADC2_IRQHandler
USBPHY1_IRQHandler
USBPHY2_IRQHandler
RDC_IRQHandler
GPIO13_Combined_0_31_IRQHandler
Reserved110_IRQHandler
DCIC1_IRQHandler
DCIC2_IRQHandler
ASRC_DriverIRQHandler
FLEXRAM_ECC_IRQHandler
CM7_GPIO2_3_IRQHandler
GPIO1_Combined_0_15_IRQHandler
GPIO1_Combined_16_31_IRQHandler
GPIO2_Combined_0_15_IRQHandler
GPIO2_Combined_16_31_IRQHandler
GPIO3_Combined_0_15_IRQHandler
GPIO3_Combined_16_31_IRQHandler
GPIO4_Combined_0_15_IRQHandler
GPIO4_Combined_16_31_IRQHandler
GPIO5_Combined_0_15_IRQHandler
GPIO5_Combined_16_31_IRQHandler
FLEXIO1_DriverIRQHandler
FLEXIO2_DriverIRQHandler
WDOG1_IRQHandler
RTWDOG3_IRQHandler
EWM_IRQHandler
OCOTP_READ_FUSE_ERROR_IRQHandler
OCOTP_READ_DONE_ERROR_IRQHandler
GPC_IRQHandler
MUA_IRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
GPT3_IRQHandler
GPT4_IRQHandler
GPT5_IRQHandler
GPT6_IRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
PWM1_FAULT_IRQHandler
FLEXSPI1_DriverIRQHandler
FLEXSPI2_DriverIRQHandler
SEMC_IRQHandler
USDHC1_DriverIRQHandler
USDHC2_DriverIRQHandler
USB_OTG2_IRQHandler
USB_OTG1_IRQHandler
ENET_DriverIRQHandler
ENET_1588_Timer_DriverIRQHandler
ENET_MAC0_Tx_Rx_Done_0_DriverIRQHandler
ENET_MAC0_Tx_Rx_Done_1_DriverIRQHandler
ENET_1G_DriverIRQHandler
ENET_1G_1588_Timer_DriverIRQHandler
XBAR1_IRQ_0_1_IRQHandler
XBAR1_IRQ_2_3_IRQHandler
ADC_ETC_IRQ0_IRQHandler
ADC_ETC_IRQ1_IRQHandler
ADC_ETC_IRQ2_IRQHandler
ADC_ETC_IRQ3_IRQHandler
ADC_ETC_ERROR_IRQ_IRQHandler
Reserved166_IRQHandler
Reserved167_IRQHandler
Reserved168_IRQHandler
Reserved169_IRQHandler
Reserved170_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
ACMP1_IRQHandler
ACMP2_IRQHandler
ACMP3_IRQHandler
ACMP4_IRQHandler
Reserved177_IRQHandler
Reserved178_IRQHandler
Reserved179_IRQHandler
Reserved180_IRQHandler
ENC1_IRQHandler
ENC2_IRQHandler
ENC3_IRQHandler
ENC4_IRQHandler
Reserved185_IRQHandler
Reserved186_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
TMR4_IRQHandler
SEMA4_CP0_IRQHandler
SEMA4_CP1_IRQHandler
PWM2_0_IRQHandler
PWM2_1_IRQHandler
PWM2_2_IRQHandler
PWM2_3_IRQHandler
PWM2_FAULT_IRQHandler
PWM3_0_IRQHandler
PWM3_1_IRQHandler
PWM3_2_IRQHandler
PWM3_3_IRQHandler
PWM3_FAULT_IRQHandler
PWM4_0_IRQHandler
PWM4_1_IRQHandler
PWM4_2_IRQHandler
PWM4_3_IRQHandler
PWM4_FAULT_IRQHandler
Reserved208_IRQHandler
Reserved209_IRQHandler
Reserved210_IRQHandler
Reserved211_IRQHandler
Reserved212_IRQHandler
Reserved213_IRQHandler
Reserved214_IRQHandler
Reserved215_IRQHandler
HWVAD_EVENT_DriverIRQHandler
HWVAD_ERROR_DriverIRQHandler
PDM_EVENT_DriverIRQHandler
PDM_ERROR_DriverIRQHandler
EMVSIM1_IRQHandler
EMVSIM2_IRQHandler
MECC1_INT_IRQHandler
MECC1_FATAL_INT_IRQHandler
MECC2_INT_IRQHandler
MECC2_FATAL_INT_IRQHandler
XECC_FLEXSPI1_INT_DriverIRQHandler
XECC_FLEXSPI1_FATAL_INT_DriverIRQHandler
XECC_FLEXSPI2_INT_DriverIRQHandler
XECC_FLEXSPI2_FATAL_INT_DriverIRQHandler
XECC_SEMC_INT_IRQHandler
XECC_SEMC_FATAL_INT_IRQHandler
ENET_QOS_DriverIRQHandler
ENET_QOS_PMT_DriverIRQHandler
DefaultISR
                LDR    R0, =DefaultISR
                BX     R0
                ENDP
                  ALIGN


                END
