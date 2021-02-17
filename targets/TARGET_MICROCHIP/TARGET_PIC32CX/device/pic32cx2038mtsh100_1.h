/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2021, Microchip Technology Inc.                                */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

#ifndef _PIC32CX2038MTSH100_
#define _PIC32CX2038MTSH100_

/** \addtogroup PIC32CX2038MTSH100_definitions PIC32CX2038MTSH100 definitions
 * This file defines all structures and symbols for PIC32CX2038MTSH100:
 *  - registers and bitfields
 *  - peripheral base address
 *  - peripheral ID
 *  - PIO definitions
 */
/*@{*/

#ifdef __cplusplus
extern "C" {
#endif

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#include <stdint.h>
#endif

/* ************************************************************************** */
/*   CMSIS DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */
/** \addtogroup PIC32CX2038MTSH100_cmsis CMSIS Definitions */
/*@{*/

/**< Interrupt Number Definition */
typedef enum IRQn {
	/******  Cortex-M4 Processor Exceptions Numbers ******************************/
	NonMaskableInt_IRQn   = -14, /**<  2 Non Maskable Interrupt                */
	HardFault_IRQn        = -13, /**<  3 HardFault Interrupt                   */
	MemoryManagement_IRQn = -12, /**<  4 Cortex-M4 Memory Management Interrupt */
	BusFault_IRQn         = -11, /**<  5 Cortex-M4 Bus Fault Interrupt         */
	UsageFault_IRQn       = -10, /**<  6 Cortex-M4 Usage Fault Interrupt       */
	SVCall_IRQn           = -5, /**< 11 Cortex-M4 SV Call Interrupt           */
	DebugMonitor_IRQn     = -4, /**< 12 Cortex-M4 Debug Monitor Interrupt     */
	PendSV_IRQn           = -2, /**< 14 Cortex-M4 Pend SV Interrupt           */
	SysTick_IRQn          = -1, /**< 15 Cortex-M4 System Tick Interrupt       */
	/******  PIC32CX2038MTSH100 specific Interrupt Numbers *********************************/

	SUPC_IRQn            =  0, /**<  0 Supply Controller (SUPC) */
	RSTC_IRQn            =  1, /**<  1 Reset Controller (RSTC) */
	RTC_IRQn             =  2, /**<  2 Real Time Clock (RTC) */
	RTT_IRQn             =  3, /**<  3 Real Time Timer (RTT) */
	DWDT_IRQn            =  4, /**<  4 Dual Watchdog Timer (DWDT) */
	PMC_IRQn             =  6, /**<  6 Power Management Controller (PMC) */
	SEFC0_IRQn           =  7, /**<  7 Embedded Flash Controller 0 (SEFC0) */
	SEFC1_IRQn           =  8, /**<  8 Embedded Flash Controller 1 (SEFC1) */
	FLEXCOM0_IRQn        =  9, /**<  9 FLEXCOM 0 (USART0/SPI0/TWI0) (FLEXCOM0) */
	FLEXCOM1_IRQn        = 10, /**< 10 FLEXCOM 1 (USART1/SPI1/TWI1) (FLEXCOM1) */
	FLEXCOM2_IRQn        = 11, /**< 11 FLEXCOM 2 (USART2/SPI2/TWI2) (FLEXCOM2) */
	FLEXCOM3_IRQn        = 12, /**< 12 FLEXCOM 3 (USART3/SPI3/TWI3) (FLEXCOM3) */
	FLEXCOM4_IRQn        = 13, /**< 13 FLEXCOM 4 (USART4/SPI4/TWI4) (FLEXCOM4) */
	FLEXCOM5_IRQn        = 14, /**< 14 FLEXCOM 5 (USART5/SPI5/TWI5) (FLEXCOM5) */
	FLEXCOM6_IRQn        = 15, /**< 15 FLEXCOM 6 (USART6/SPI6/TWI6) (FLEXCOM6) */
	FLEXCOM7_IRQn        = 16, /**< 16 FLEXCOM 7 (USART7/SPI7/TWI7) (FLEXCOM7) */
	PIOA_IRQn            = 17, /**< 17 Application Core Parallel I/O Controller A (PIOA) (PIOA) */
	PIOA_SEC_IRQn        = 18, /**< 18 Application Core Parallel I/O Controller A (PIOA) Secure Event Interrupt (PIOA_SEC) */
	PIOB_IRQn            = 19, /**< 19 Application Core Parallel I/O Controller (B PIOB) (PIOB) */
	PIOB_SEC_IRQn        = 20, /**< 20 Application Core Parallel I/O Controller (B PIOB) Secure Event Interrupt (PIOB_SEC) */
	PIOC_IRQn            = 21, /**< 21 Application Core Parallel I/O Controller C (PIOC) (PIOC) */
	PIOC_SEC_IRQn        = 22, /**< 22 Application Core Parallel I/O Controller C (PIOC) Secure Event Interrupt (PIOC_SEC) */
	QSPI_IRQn            = 23, /**< 23 Quad IO Serial Peripheral Interface (QSPI) */
	ADC_IRQn             = 24, /**< 24 Analog To Digital Converter (ADC) */
	ACC_IRQn             = 25, /**< 25 Analog Comparator (ACC) */
	ARM0_FPU_IRQn        = 26, /**< 26 Application Core (CORE0) Floating Point Unit except IXC (ARM0_FPU) */
	ARM0_IXC_IRQn        = 27, /**< 27 Application Core (CORE0) FPU Interrupt IXC associated with FPU cumulative exception bit (ARM0_IXC) */
	IPC0_IRQn            = 28, /**< 28 Application Core Interprocessor communication (IPC0) (IPC0) */
	SLCDC_IRQn           = 29, /**< 29 Segment LCD Controller (SLCDC) */
	MEM2MEM0_IRQn        = 30, /**< 30 Application Core Memory to Memory Transfer Controller (MEM2MEM0) (MEM2MEM0) */
	TC0_CHANNEL0_IRQn    = 31, /**< 31 Timer Counter 0, Channel 0 (TC0_CHANNEL0) */
	TC0_CHANNEL1_IRQn    = 32, /**< 32 Timer Counter 0, Channel 1 (TC0_CHANNEL1) */
	TC0_CHANNEL2_IRQn    = 33, /**< 33 Timer Counter 0, Channel 2 (TC0_CHANNEL2) */
	TC1_CHANNEL0_IRQn    = 34, /**< 34 Timer Counter 1, Channel 0 (TC1_CHANNEL0) */
	TC1_CHANNEL1_IRQn    = 35, /**< 35 Timer Counter 1, Channel 1 (TC1_CHANNEL1) */
	TC1_CHANNEL2_IRQn    = 36, /**< 36 Timer Counter 1, Channel 2 (TC1_CHANNEL2) */
	TC2_CHANNEL0_IRQn    = 37, /**< 37 Timer Counter 2, Channel 0 (TC2_CHANNEL0) */
	TC2_CHANNEL1_IRQn    = 38, /**< 38 Timer Counter 2, Channel 1 (TC2_CHANNEL1) */
	TC2_CHANNEL2_IRQn    = 39, /**< 39 Timer Counter 2, Channel 2 (TC2_CHANNEL2) */
	TC0_C0SEC_IRQn       = 40, /**< 40 Timer Counter 0, Channel 0, Secure IRQ (TC0_C0SEC) */
	TC0_C1SEC_IRQn       = 41, /**< 41 Timer Counter 0, Channel 1, Secure IRQ (TC0_C1SEC) */
	TC0_C2SEC_IRQn       = 42, /**< 42 Timer Counter 0, Channel 2, Secure IRQ (TC0_C2SEC) */
	TC1_C0SEC_IRQn       = 43, /**< 43 Timer Counter 1, Channel 0, Secure IRQ (TC1_C0SEC) */
	TC1_C1SEC_IRQn       = 44, /**< 44 Timer Counter 1, Channel 1, Secure IRQ (TC1_C1SEC) */
	TC1_C2SEC_IRQn       = 45, /**< 45 Timer Counter 1, Channel 2, Secure IRQ (TC1_C2SEC) */
	TC2_C0SEC_IRQn       = 46, /**< 46 Timer Counter 2, Channel 0, Secure IRQ (TC2_C0SEC) */
	TC2_C1SEC_IRQn       = 47, /**< 47 Timer Counter 2, Channel 1, Secure IRQ (TC2_C1SEC) */
	TC2_C2SEC_IRQn       = 48, /**< 48 Timer Counter 2, Channel 2, Secure IRQ (TC2_C2SEC) */
	AES_IRQn             = 49, /**< 49 Advanced Enhanced Standard (AES) */
	AES_AESSEC_IRQn      = 50, /**< 50 AES Secure Event Interrupt (AES_AESSEC) */
	AESB_IRQn            = 51, /**< 51 AES Bridge (AESB) */
	AESB_AESBSEC_IRQn    = 52, /**< 52 AES Bridge Secure Interrupt (AESB_AESBSEC) */
	SHA_IRQn             = 53, /**< 53 Secure Hash Algorithm (SHA) */
	SHA_SHASEC_IRQn      = 54, /**< 54 SHA Secure Event Interrupt (SHA_SHASEC) */
	TRNG_IRQn            = 55, /**< 55 True Random Generator (TRNG) */
	TRNG_TRNGSEC_IRQn    = 56, /**< 56 TRNG Secure Event Interrupt (TRNG_TRNGSEC) */
	ICM_IRQn             = 57, /**< 57 Integrity Check Module (ICM) */
	ICM_ICMSEC_IRQn      = 58, /**< 58 Integrity Check Module (ICM_ICMSEC) */
	CPKCC_IRQn           = 59, /**< 59 Public Key Cryptography Controller (CPKCC) */
	MATRIX0_IRQn         = 60, /**< 60 High-Speed Application Core Matrix (HSACM) (MATRIX0) (MATRIX0) */
	MATRIX1_IRQn         = 61, /**< 61 Low-Speed Application Core Matrix (LSACM) (MATRIX1) (MATRIX1) */
	SUPC_WKUP3_IRQn      = 62, /**< 62 External interrupt 3 (SUPC_WKUP3) */
	SUPC_WKUP4_IRQn      = 63, /**< 63 External interrupt 4 (SUPC_WKUP4) */
	SUPC_WKUP5_IRQn      = 64, /**< 64 External interrupt 5 (SUPC_WKUP5) */
	SUPC_WKUP6_IRQn      = 65, /**< 65 External interrupt 6 (SUPC_WKUP6) */
	SUPC_WKUP7_IRQn      = 66, /**< 66 External interrupt 7 (SUPC_WKUP7) */
	SUPC_WKUP8_IRQn      = 67, /**< 67 External interrupt 8 (SUPC_WKUP8) */
	SUPC_WKUP9_IRQn      = 68, /**< 68 External interrupt 9 (SUPC_WKUP9) */
	SUPC_WKUP10_IRQn     = 69, /**< 69 External interrupt 10 (SUPC_WKUP10) */
	SUPC_WKUP11_IRQn     = 70, /**< 70 External interrupt 11 (SUPC_WKUP11) */
	SUPC_WKUP12_IRQn     = 71, /**< 71 External interrupt 12 (SUPC_WKUP12) */
	SUPC_WKUP13_IRQn     = 72, /**< 72 External interrupt 13 (SUPC_WKUP13) */
	SUPC_WKUP14_IRQn     = 73, /**< 73 External interrupt 14 (SUPC_WKUP14) */
	SUPC_WKUP15_IRQn     = 74, /**< 74 External interrupt 15 (SUPC_WKUP15) */
	SEAFEC_SLINK_IRQn    = 76, /**< 76 Smart Energy Analog Front-End Controller Serial Link Interrupt (SEAFEC_SLINK) */
	SEAFEC_DATA_IRQn     = 77, /**< 77 Smart Energy Analog Front-End Controller Data Ready Interrupt (SEAFEC_DATA) */
	MEM2MEM1_IRQn        = 78, /**< 78 Metrology Core Memory to Memory Transfer Controller 1 (MEM2MEM1) (MEM2MEM1) */
	TC3_CHANNEL0_IRQn    = 79, /**< 79 Timer Counter 3, Channel 0 (TC3_CHANNEL0) */
	TC3_CHANNEL1_IRQn    = 80, /**< 80 Timer Counter 3, Channel 1 (TC3_CHANNEL1) */
	TC3_CHANNEL2_IRQn    = 81, /**< 81 Timer Counter 3, Channel 2 (TC3_CHANNEL2) */
	TC3_C0SEC_IRQn       = 82, /**< 82 Timer Counter 3, Channel 0, Secure IRQ (TC3_C0SEC) */
	TC3_C1SEC_IRQn       = 83, /**< 83 Timer Counter 3, Channel 1, Secure IRQ (TC3_C1SEC) */
	TC3_C2SEC_IRQn       = 84, /**< 84 Timer Counter 3, Channel 2, Secure IRQ (TC3_C2SEC) */
	PIOD_IRQn            = 85, /**< 85 Metrology Core Parallel I/O Controller D (PIOD) (PIOD) */
	PIOD_SEC_IRQn        = 86, /**< 86 Metrology Core Parallel I/O Controller A Secure Interupt (named as PIOD) (PIOD_SEC) */
	UART_IRQn            = 87, /**< 87 Optical UART (UART) */
	IPC1_IRQn            = 88, /**< 88 Metrology Core Interprocessor communication (IPC1) (IPC1) */
	MCSPI_IRQn           = 89, /**< 89 Multi-Channel SPI (MCSPI) */
	PWM_IRQn             = 90, /**< 90 Pulse Width Modulation (PWM) */
	ARM1_FPU_IRQn        = 92, /**< 92 Metrology Core (CORE1) Floating Point Unit except IXC (ARM1_FPU) */
	ARM1_IXC_IRQn        = 93, /**< 93 Metrology Core (CORE1) FPU Interrupt IXC associated with FPU cumulative exception bit (ARM1_IXC) */
	MATRIX2_IRQn         = 94, /**< 94 High-Speed Metrology Core Matrix (HSMCM) (Formely MATRIX2) (MATRIX2) */
	MATRIX3_IRQn         = 95, /**< 95 Low-Speed Metrology Core Matrix (LSMCM) (Formely MATRIX3) (MATRIX3) */

	PERIPH_COUNT_IRQn    = 97 /**< Number of peripheral IDs */
} IRQn_Type;

typedef struct _DeviceVectors {
	/* Stack pointer */
	void *pvStack;

	/* Cortex-M handlers */
	void *pfnReset_Handler;
	void *pfnNMI_Handler;
	void *pfnHardFault_Handler;
	void *pfnMemManage_Handler;
	void *pfnBusFault_Handler;
	void *pfnUsageFault_Handler;
	void *pfnReserved1_Handler;
	void *pfnReserved2_Handler;
	void *pfnReserved3_Handler;
	void *pfnReserved4_Handler;
	void *pfnSVC_Handler;
	void *pfnDebugMon_Handler;
	void *pfnReserved5_Handler;
	void *pfnPendSV_Handler;
	void *pfnSysTick_Handler;

	/* Peripheral handlers */
	void *pfnSUPC_Handler;   /*  0 Supply Controller */
	void *pfnRSTC_Handler;   /*  1 Reset Controller */
	void *pfnRTC_Handler;    /*  2 Real Time Clock */
	void *pfnRTT_Handler;    /*  3 Real Time Timer */
	void *pfnDWDT_Handler;   /*  4 Dual Watchdog Timer */
	void *pvReserved5;
	void *pfnPMC_Handler;    /*  6 Power Management Controller */
	void *pfnSEFC0_Handler;  /*  7 Embedded Flash Controller 0 */
	void *pfnSEFC1_Handler;  /*  8 Embedded Flash Controller 1 */
	void *pfnFLEXCOM0_Handler; /*  9 FLEXCOM 0 (USART0/SPI0/TWI0) */
	void *pfnFLEXCOM1_Handler; /* 10 FLEXCOM 1 (USART1/SPI1/TWI1) */
	void *pfnFLEXCOM2_Handler; /* 11 FLEXCOM 2 (USART2/SPI2/TWI2) */
	void *pfnFLEXCOM3_Handler; /* 12 FLEXCOM 3 (USART3/SPI3/TWI3) */
	void *pfnFLEXCOM4_Handler; /* 13 FLEXCOM 4 (USART4/SPI4/TWI4) */
	void *pfnFLEXCOM5_Handler; /* 14 FLEXCOM 5 (USART5/SPI5/TWI5) */
	void *pfnFLEXCOM6_Handler; /* 15 FLEXCOM 6 (USART6/SPI6/TWI6) */
	void *pfnFLEXCOM7_Handler; /* 16 FLEXCOM 7 (USART7/SPI7/TWI7) */
	void *pfnPIOA_Handler;   /* 17 Application Core Parallel I/O Controller A (PIOA) */
	void *pfnPIOA_SEC_Handler; /* 18 Application Core Parallel I/O Controller A (PIOA) Secure Event Interrupt */
	void *pfnPIOB_Handler;   /* 19 Application Core Parallel I/O Controller (B PIOB) */
	void *pfnPIOB_SEC_Handler; /* 20 Application Core Parallel I/O Controller (B PIOB) Secure Event Interrupt */
	void *pfnPIOC_Handler;   /* 21 Application Core Parallel I/O Controller C (PIOC) */
	void *pfnPIOC_SEC_Handler; /* 22 Application Core Parallel I/O Controller C (PIOC) Secure Event Interrupt */
	void *pfnQSPI_Handler;   /* 23 Quad IO Serial Peripheral Interface */
	void *pfnADC_Handler;    /* 24 Analog To Digital Converter */
	void *pfnACC_Handler;    /* 25 Analog Comparator */
	void *pfnARM0_FPU_Handler; /* 26 Application Core (CORE0) Floating Point Unit except IXC */
	void *pfnARM0_IXC_Handler; /* 27 Application Core (CORE0) FPU Interrupt IXC associated with FPU cumulative exception bit */
	void *pfnIPC0_Handler;   /* 28 Application Core Interprocessor communication (IPC0) */
	void *pfnSLCDC_Handler;  /* 29 Segment LCD Controller */
	void *pfnMEM2MEM0_Handler; /* 30 Application Core Memory to Memory Transfer Controller (MEM2MEM0) */
	void *pfnTC0_CHANNEL0_Handler; /* 31 Timer Counter 0, Channel 0 */
	void *pfnTC0_CHANNEL1_Handler; /* 32 Timer Counter 0, Channel 1 */
	void *pfnTC0_CHANNEL2_Handler; /* 33 Timer Counter 0, Channel 2 */
	void *pfnTC1_CHANNEL0_Handler; /* 34 Timer Counter 1, Channel 0 */
	void *pfnTC1_CHANNEL1_Handler; /* 35 Timer Counter 1, Channel 1 */
	void *pfnTC1_CHANNEL2_Handler; /* 36 Timer Counter 1, Channel 2 */
	void *pfnTC2_CHANNEL0_Handler; /* 37 Timer Counter 2, Channel 0 */
	void *pfnTC2_CHANNEL1_Handler; /* 38 Timer Counter 2, Channel 1 */
	void *pfnTC2_CHANNEL2_Handler; /* 39 Timer Counter 2, Channel 2 */
	void *pfnTC0_C0SEC_Handler; /* 40 Timer Counter 0, Channel 0, Secure IRQ */
	void *pfnTC0_C1SEC_Handler; /* 41 Timer Counter 0, Channel 1, Secure IRQ */
	void *pfnTC0_C2SEC_Handler; /* 42 Timer Counter 0, Channel 2, Secure IRQ */
	void *pfnTC1_C0SEC_Handler; /* 43 Timer Counter 1, Channel 0, Secure IRQ */
	void *pfnTC1_C1SEC_Handler; /* 44 Timer Counter 1, Channel 1, Secure IRQ */
	void *pfnTC1_C2SEC_Handler; /* 45 Timer Counter 1, Channel 2, Secure IRQ */
	void *pfnTC2_C0SEC_Handler; /* 46 Timer Counter 2, Channel 0, Secure IRQ */
	void *pfnTC2_C1SEC_Handler; /* 47 Timer Counter 2, Channel 1, Secure IRQ */
	void *pfnTC2_C2SEC_Handler; /* 48 Timer Counter 2, Channel 2, Secure IRQ */
	void *pfnAES_Handler;    /* 49 Advanced Enhanced Standard */
	void *pfnAES_AESSEC_Handler; /* 50 AES Secure Event Interrupt */
	void *pfnAESB_Handler;   /* 51 AES Bridge */
	void *pfnAESB_AESBSEC_Handler; /* 52 AES Bridge Secure Interrupt */
	void *pfnSHA_Handler;    /* 53 Secure Hash Algorithm */
	void *pfnSHA_SHASEC_Handler; /* 54 SHA Secure Event Interrupt */
	void *pfnTRNG_Handler;   /* 55 True Random Generator */
	void *pfnTRNG_TRNGSEC_Handler; /* 56 TRNG Secure Event Interrupt */
	void *pfnICM_Handler;    /* 57 Integrity Check Module */
	void *pfnICM_ICMSEC_Handler; /* 58 Integrity Check Module */
	void *pfnCPKCC_Handler;  /* 59 Public Key Cryptography Controller */
	void *pfnMATRIX0_Handler; /* 60 High-Speed Application Core Matrix (HSACM) (MATRIX0) */
	void *pfnMATRIX1_Handler; /* 61 Low-Speed Application Core Matrix (LSACM) (MATRIX1) */
	void *pfnSUPC_WKUP3_Handler; /* 62 External interrupt 3 */
	void *pfnSUPC_WKUP4_Handler; /* 63 External interrupt 4 */
	void *pfnSUPC_WKUP5_Handler; /* 64 External interrupt 5 */
	void *pfnSUPC_WKUP6_Handler; /* 65 External interrupt 6 */
	void *pfnSUPC_WKUP7_Handler; /* 66 External interrupt 7 */
	void *pfnSUPC_WKUP8_Handler; /* 67 External interrupt 8 */
	void *pfnSUPC_WKUP9_Handler; /* 68 External interrupt 9 */
	void *pfnSUPC_WKUP10_Handler; /* 69 External interrupt 10 */
	void *pfnSUPC_WKUP11_Handler; /* 70 External interrupt 11 */
	void *pfnSUPC_WKUP12_Handler; /* 71 External interrupt 12 */
	void *pfnSUPC_WKUP13_Handler; /* 72 External interrupt 13 */
	void *pfnSUPC_WKUP14_Handler; /* 73 External interrupt 14 */
	void *pfnSUPC_WKUP15_Handler; /* 74 External interrupt 15 */
	void *pvReserved75;
	void *pfnSEAFEC_SLINK_Handler; /* 76 Smart Energy Analog Front-End Controller Serial Link Interrupt */
	void *pfnSEAFEC_DATA_Handler; /* 77 Smart Energy Analog Front-End Controller Data Ready Interrupt */
	void *pfnMEM2MEM1_Handler; /* 78 Metrology Core Memory to Memory Transfer Controller 1 (MEM2MEM1) */
	void *pfnTC3_CHANNEL0_Handler; /* 79 Timer Counter 3, Channel 0 */
	void *pfnTC3_CHANNEL1_Handler; /* 80 Timer Counter 3, Channel 1 */
	void *pfnTC3_CHANNEL2_Handler; /* 81 Timer Counter 3, Channel 2 */
	void *pfnTC3_C0SEC_Handler; /* 82 Timer Counter 3, Channel 0, Secure IRQ */
	void *pfnTC3_C1SEC_Handler; /* 83 Timer Counter 3, Channel 1, Secure IRQ */
	void *pfnTC3_C2SEC_Handler; /* 84 Timer Counter 3, Channel 2, Secure IRQ */
	void *pfnPIOD_Handler;   /* 85 Metrology Core Parallel I/O Controller D (PIOD) */
	void *pfnPIOD_SEC_Handler; /* 86 Metrology Core Parallel I/O Controller A Secure Interupt (named as PIOD) */
	void *pfnUART_Handler;   /* 87 Optical UART */
	void *pfnIPC1_Handler;   /* 88 Metrology Core Interprocessor communication (IPC1) */
	void *pfnMCSPI_Handler;  /* 89 Multi-Channel SPI */
	void *pfnPWM_Handler;    /* 90 Pulse Width Modulation */
	void *pvReserved91;
	void *pfnARM1_FPU_Handler; /* 92 Metrology Core (CORE1) Floating Point Unit except IXC */
	void *pfnARM1_IXC_Handler; /* 93 Metrology Core (CORE1) FPU Interrupt IXC associated with FPU cumulative exception bit */
	void *pfnMATRIX2_Handler; /* 94 High-Speed Metrology Core Matrix (HSMCM) (Formely MATRIX2) */
	void *pfnMATRIX3_Handler; /* 95 Low-Speed Metrology Core Matrix (LSMCM) (Formely MATRIX3) */
	void* pvReserved96;
} DeviceVectors;

/* Cortex-M4 core handlers */
void Reset_Handler        ( void );
void NMI_Handler          ( void );
void HardFault_Handler    ( void );
void MemManage_Handler    ( void );
void BusFault_Handler     ( void );
void UsageFault_Handler   ( void );
void SVC_Handler          ( void );
void DebugMon_Handler     ( void );
void PendSV_Handler       ( void );
void SysTick_Handler      ( void );

/* Peripherals handlers */
void ACC_Handler          ( void );
void ADC_Handler          ( void );
void AES_Handler          ( void );
void AES_AESSEC_Handler   ( void );
void AESB_Handler         ( void );
void AESB_AESBSEC_Handler ( void );
void ARM0_FPU_Handler     ( void );
void ARM0_IXC_Handler     ( void );
void ARM1_FPU_Handler     ( void );
void ARM1_IXC_Handler     ( void );
void CPKCC_Handler        ( void );
void DWDT_Handler         ( void );
void FLEXCOM0_Handler     ( void );
void FLEXCOM1_Handler     ( void );
void FLEXCOM2_Handler     ( void );
void FLEXCOM3_Handler     ( void );
void FLEXCOM4_Handler     ( void );
void FLEXCOM5_Handler     ( void );
void FLEXCOM6_Handler     ( void );
void FLEXCOM7_Handler     ( void );
void ICM_Handler          ( void );
void ICM_ICMSEC_Handler   ( void );
void IPC0_Handler         ( void );
void IPC1_Handler         ( void );
void MATRIX0_Handler      ( void );
void MATRIX1_Handler      ( void );
void MATRIX2_Handler      ( void );
void MATRIX3_Handler      ( void );
void MCSPI_Handler        ( void );
void MEM2MEM0_Handler     ( void );
void MEM2MEM1_Handler     ( void );
void PIOA_Handler         ( void );
void PIOA_SEC_Handler     ( void );
void PIOB_Handler         ( void );
void PIOB_SEC_Handler     ( void );
void PIOC_Handler         ( void );
void PIOC_SEC_Handler     ( void );
void PIOD_Handler         ( void );
void PIOD_SEC_Handler     ( void );
void PMC_Handler          ( void );
void PWM_Handler          ( void );
void QSPI_Handler         ( void );
void RSTC_Handler         ( void );
void RTC_Handler          ( void );
void RTT_Handler          ( void );
void SEAFEC_DATA_Handler  ( void );
void SEAFEC_SLINK_Handler ( void );
void SEFC0_Handler        ( void );
void SEFC1_Handler        ( void );
void SHA_Handler          ( void );
void SHA_SHASEC_Handler   ( void );
void SLCDC_Handler        ( void );
void SUPC_Handler         ( void );
void SUPC_WKUP3_Handler   ( void );
void SUPC_WKUP4_Handler   ( void );
void SUPC_WKUP5_Handler   ( void );
void SUPC_WKUP6_Handler   ( void );
void SUPC_WKUP7_Handler   ( void );
void SUPC_WKUP8_Handler   ( void );
void SUPC_WKUP9_Handler   ( void );
void SUPC_WKUP10_Handler  ( void );
void SUPC_WKUP11_Handler  ( void );
void SUPC_WKUP12_Handler  ( void );
void SUPC_WKUP13_Handler  ( void );
void SUPC_WKUP14_Handler  ( void );
void SUPC_WKUP15_Handler  ( void );
void TC0_CHANNEL0_Handler ( void );
void TC0_CHANNEL1_Handler ( void );
void TC0_CHANNEL2_Handler ( void );
void TC0_C0SEC_Handler    ( void );
void TC0_C1SEC_Handler    ( void );
void TC0_C2SEC_Handler    ( void );
void TC1_CHANNEL0_Handler ( void );
void TC1_CHANNEL1_Handler ( void );
void TC1_CHANNEL2_Handler ( void );
void TC1_C0SEC_Handler    ( void );
void TC1_C1SEC_Handler    ( void );
void TC1_C2SEC_Handler    ( void );
void TC2_CHANNEL0_Handler ( void );
void TC2_CHANNEL1_Handler ( void );
void TC2_CHANNEL2_Handler ( void );
void TC2_C0SEC_Handler    ( void );
void TC2_C1SEC_Handler    ( void );
void TC2_C2SEC_Handler    ( void );
void TC3_CHANNEL0_Handler ( void );
void TC3_CHANNEL1_Handler ( void );
void TC3_CHANNEL2_Handler ( void );
void TC3_C0SEC_Handler    ( void );
void TC3_C1SEC_Handler    ( void );
void TC3_C2SEC_Handler    ( void );
void TRNG_Handler         ( void );
void TRNG_TRNGSEC_Handler ( void );
void UART_Handler         ( void );

/**
 * \brief Configuration of the Cortex-M4 Processor and Core Peripherals 
 */

#define __CM4_REV              0x0001 /**< PIC32CX2038MTSH100 core revision number ([15:8] revision number, [7:0] patch number) */
#define __MPU_PRESENT          1      /**< PIC32CX2038MTSH100 does provide a MPU */
#define __FPU_PRESENT          1      /**< PIC32CX2038MTSH100 does provide a FPU */
#define __NVIC_PRIO_BITS       4      /**< PIC32CX2038MTSH100 uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig 0      /**< Set to 1 if different SysTick Config is used */

/*
 * \brief CMSIS includes
 */

#include <core_cm4.h>
#if !defined DONT_USE_CMSIS_INIT
#include "system_pic32cx.h"
#endif /* DONT_USE_CMSIS_INIT */

/*@}*/

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */
/** \addtogroup PIC32CX2038MTSH100_api Peripheral Software API */
/*@{*/

#include "component/acc.h"
#include "component/adc.h"
#include "component/aes.h"
#include "component/aesb.h"
#include "component/chipid.h"
#include "component/cmcc.h"
#include "component/cpkcc.h"
#include "component/dwdt.h"
#include "component/emafelpfif.h"
#include "component/flexcom.h"
#include "component/gpbr.h"
#include "component/icm.h"
#include "component/ipc.h"
#include "component/matrix.h"
#include "component/mcspi.h"
#include "component/mem2mem.h"
#include "component/pdc.h"
#include "component/pio.h"
#include "component/pmc.h"
#include "component/pwm.h"
#include "component/qspi.h"
#include "component/rstc.h"
#include "component/rtc.h"
#include "component/rtt.h"
#include "component/sefc.h"
#include "component/sfr.h"
#include "component/sfrbu.h"
#include "component/sha.h"
#include "component/slcdc.h"
#include "component/spi.h"
#include "component/supc.h"
#include "component/syscwp.h"
#include "component/tc.h"
#include "component/trng.h"
#include "component/twi.h"
#include "component/uart.h"
#include "component/usart.h"
/*@}*/

/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */
/** \addtogroup PIC32CX2038MTSH100_reg Registers Access Definitions */
/*@{*/

#include "instance/flexcom0.h"
#include "instance/flexcom1.h"
#include "instance/flexcom2.h"
#include "instance/flexcom3.h"
#include "instance/flexcom4.h"
#include "instance/flexcom5.h"
#include "instance/flexcom6.h"
#include "instance/flexcom7.h"
#include "instance/usart0.h"
#include "instance/usart1.h"
#include "instance/usart2.h"
#include "instance/usart3.h"
#include "instance/usart4.h"
#include "instance/usart5.h"
#include "instance/usart6.h"
#include "instance/usart7.h"
#include "instance/spi0.h"
#include "instance/spi1.h"
#include "instance/spi2.h"
#include "instance/spi3.h"
#include "instance/spi4.h"
#include "instance/spi5.h"
#include "instance/spi6.h"
#include "instance/spi7.h"
#include "instance/twi0.h"
#include "instance/twi1.h"
#include "instance/twi2.h"
#include "instance/twi3.h"
#include "instance/twi4.h"
#include "instance/twi5.h"
#include "instance/twi6.h"
#include "instance/twi7.h"
#include "instance/qspi.h"
#include "instance/adc.h"
#include "instance/acc.h"
#include "instance/ipc0.h"
#include "instance/slcdc.h"
#include "instance/mem2mem0.h"
#include "instance/tc0.h"
#include "instance/tc1.h"
#include "instance/tc2.h"
#include "instance/matrix1.h"
#include "instance/pio.h"
#include "instance/chipid.h"
#include "instance/sfr.h"
#include "instance/sfrbu.h"
#include "instance/dwdt.h"
#include "instance/rstc.h"
#include "instance/rtt.h"
#include "instance/gpbr.h"
#include "instance/rtc.h"
#include "instance/syscwp.h"
#include "instance/supc.h"
#include "instance/aes.h"
#include "instance/aesb.h"
#include "instance/sha.h"
#include "instance/trng.h"
#include "instance/icm.h"
#include "instance/cpkcc.h"
#include "instance/matrix0.h"
#include "instance/cmcc0.h"
#include "instance/cmcc1.h"
#include "instance/sefc0.h"
#include "instance/sefc1.h"
#include "instance/pmc.h"
#include "instance/mem2mem1.h"
#include "instance/tc3.h"
#include "instance/uart.h"
#include "instance/ipc1.h"
#include "instance/mcspi.h"
#include "instance/pwm.h"
#include "instance/matrix3.h"
#include "instance/matrix2.h"
/*@}*/

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */
/** \addtogroup PIC32CX2038MTSH100_id Peripheral Ids Definitions */
/*@{*/

#define ID_SUPC         ( 0) /**< \brief Supply Controller (SUPC) */
#define ID_RSTC         ( 1) /**< \brief Reset Controller (RSTC) */
#define ID_RTC          ( 2) /**< \brief Real Time Clock (RTC) */
#define ID_RTT          ( 3) /**< \brief Real Time Timer (RTT) */
#define ID_DWDT         ( 4) /**< \brief Dual Watchdog Timer (DWDT) */
#define ID_PMC          ( 6) /**< \brief Power Management Controller (PMC) */
#define ID_SEFC0        ( 7) /**< \brief Embedded Flash Controller 0 (SEFC0) */
#define ID_SEFC1        ( 8) /**< \brief Embedded Flash Controller 1 (SEFC1) */
#define ID_FLEXCOM0     ( 9) /**< \brief FLEXCOM 0 (USART0/SPI0/TWI0) (FLEXCOM0) */
#define ID_FLEXCOM1     (10) /**< \brief FLEXCOM 1 (USART1/SPI1/TWI1) (FLEXCOM1) */
#define ID_FLEXCOM2     (11) /**< \brief FLEXCOM 2 (USART2/SPI2/TWI2) (FLEXCOM2) */
#define ID_FLEXCOM3     (12) /**< \brief FLEXCOM 3 (USART3/SPI3/TWI3) (FLEXCOM3) */
#define ID_FLEXCOM4     (13) /**< \brief FLEXCOM 4 (USART4/SPI4/TWI4) (FLEXCOM4) */
#define ID_FLEXCOM5     (14) /**< \brief FLEXCOM 5 (USART5/SPI5/TWI5) (FLEXCOM5) */
#define ID_FLEXCOM6     (15) /**< \brief FLEXCOM 6 (USART6/SPI6/TWI6) (FLEXCOM6) */
#define ID_FLEXCOM7     (16) /**< \brief FLEXCOM 7 (USART7/SPI7/TWI7) (FLEXCOM7) */
#define ID_PIOA         (17) /**< \brief Application Core Parallel I/O Controller A (PIOA) (PIOA) */
#define ID_PIOA_SEC     (18) /**< \brief Application Core Parallel I/O Controller A (PIOA) Secure Event Interrupt (PIOA_SEC) */
#define ID_PIOB         (19) /**< \brief Application Core Parallel I/O Controller (B PIOB) (PIOB) */
#define ID_PIOB_SEC     (20) /**< \brief Application Core Parallel I/O Controller (B PIOB) Secure Event Interrupt (PIOB_SEC) */
#define ID_PIOC         (21) /**< \brief Application Core Parallel I/O Controller C (PIOC) (PIOC) */
#define ID_PIOC_SEC     (22) /**< \brief Application Core Parallel I/O Controller C (PIOC) Secure Event Interrupt (PIOC_SEC) */
#define ID_QSPI         (23) /**< \brief Quad IO Serial Peripheral Interface (QSPI) */
#define ID_ADC          (24) /**< \brief Analog To Digital Converter (ADC) */
#define ID_ACC          (25) /**< \brief Analog Comparator (ACC) */
#define ID_ARM0_FPU     (26) /**< \brief Application Core (CORE0) Floating Point Unit except IXC (ARM0_FPU) */
#define ID_ARM0_IXC     (27) /**< \brief Application Core (CORE0) FPU Interrupt IXC associated with FPU cumulative exception bit (ARM0_IXC) */
#define ID_IPC0         (28) /**< \brief Application Core Interprocessor communication (IPC0) (IPC0) */
#define ID_SLCDC        (29) /**< \brief Segment LCD Controller (SLCDC) */
#define ID_MEM2MEM0     (30) /**< \brief Application Core Memory to Memory Transfer Controller (MEM2MEM0) (MEM2MEM0) */
#define ID_TC0_CHANNEL0 (31) /**< \brief Timer Counter 0, Channel 0 (TC0_CHANNEL0) */
#define ID_TC0_CHANNEL1 (32) /**< \brief Timer Counter 0, Channel 1 (TC0_CHANNEL1) */
#define ID_TC0_CHANNEL2 (33) /**< \brief Timer Counter 0, Channel 2 (TC0_CHANNEL2) */
#define ID_TC1_CHANNEL0 (34) /**< \brief Timer Counter 1, Channel 0 (TC1_CHANNEL0) */
#define ID_TC1_CHANNEL1 (35) /**< \brief Timer Counter 1, Channel 1 (TC1_CHANNEL1) */
#define ID_TC1_CHANNEL2 (36) /**< \brief Timer Counter 1, Channel 2 (TC1_CHANNEL2) */
#define ID_TC2_CHANNEL0 (37) /**< \brief Timer Counter 2, Channel 0 (TC2_CHANNEL0) */
#define ID_TC2_CHANNEL1 (38) /**< \brief Timer Counter 2, Channel 1 (TC2_CHANNEL1) */
#define ID_TC2_CHANNEL2 (39) /**< \brief Timer Counter 2, Channel 2 (TC2_CHANNEL2) */
#define ID_TC0_C0SEC    (40) /**< \brief Timer Counter 0, Channel 0, Secure IRQ (TC0_C0SEC) */
#define ID_TC0_C1SEC    (41) /**< \brief Timer Counter 0, Channel 1, Secure IRQ (TC0_C1SEC) */
#define ID_TC0_C2SEC    (42) /**< \brief Timer Counter 0, Channel 2, Secure IRQ (TC0_C2SEC) */
#define ID_TC1_C0SEC    (43) /**< \brief Timer Counter 1, Channel 0, Secure IRQ (TC1_C0SEC) */
#define ID_TC1_C1SEC    (44) /**< \brief Timer Counter 1, Channel 1, Secure IRQ (TC1_C1SEC) */
#define ID_TC1_C2SEC    (45) /**< \brief Timer Counter 1, Channel 2, Secure IRQ (TC1_C2SEC) */
#define ID_TC2_C0SEC    (46) /**< \brief Timer Counter 2, Channel 0, Secure IRQ (TC2_C0SEC) */
#define ID_TC2_C1SEC    (47) /**< \brief Timer Counter 2, Channel 1, Secure IRQ (TC2_C1SEC) */
#define ID_TC2_C2SEC    (48) /**< \brief Timer Counter 2, Channel 2, Secure IRQ (TC2_C2SEC) */
#define ID_AES          (49) /**< \brief Advanced Enhanced Standard (AES) */
#define ID_AES_AESSEC   (50) /**< \brief AES Secure Event Interrupt (AES_AESSEC) */
#define ID_AESB         (51) /**< \brief AES Bridge (AESB) */
#define ID_AESB_AESBSEC (52) /**< \brief AES Bridge Secure Interrupt (AESB_AESBSEC) */
#define ID_SHA          (53) /**< \brief Secure Hash Algorithm (SHA) */
#define ID_SHA_SHASEC   (54) /**< \brief SHA Secure Event Interrupt (SHA_SHASEC) */
#define ID_TRNG         (55) /**< \brief True Random Generator (TRNG) */
#define ID_TRNG_TRNGSEC (56) /**< \brief TRNG Secure Event Interrupt (TRNG_TRNGSEC) */
#define ID_ICM          (57) /**< \brief Integrity Check Module (ICM) */
#define ID_ICM_ICMSEC   (58) /**< \brief Integrity Check Module (ICM_ICMSEC) */
#define ID_CPKCC        (59) /**< \brief Public Key Cryptography Controller (CPKCC) */
#define ID_MATRIX0      (60) /**< \brief High-Speed Application Core Matrix (HSACM) (MATRIX0) (MATRIX0) */
#define ID_MATRIX1      (61) /**< \brief Low-Speed Application Core Matrix (LSACM) (MATRIX1) (MATRIX1) */
#define ID_SUPC_WKUP3   (62) /**< \brief External interrupt 3 (SUPC_WKUP3) */
#define ID_SUPC_WKUP4   (63) /**< \brief External interrupt 4 (SUPC_WKUP4) */
#define ID_SUPC_WKUP5   (64) /**< \brief External interrupt 5 (SUPC_WKUP5) */
#define ID_SUPC_WKUP6   (65) /**< \brief External interrupt 6 (SUPC_WKUP6) */
#define ID_SUPC_WKUP7   (66) /**< \brief External interrupt 7 (SUPC_WKUP7) */
#define ID_SUPC_WKUP8   (67) /**< \brief External interrupt 8 (SUPC_WKUP8) */
#define ID_SUPC_WKUP9   (68) /**< \brief External interrupt 9 (SUPC_WKUP9) */
#define ID_SUPC_WKUP10  (69) /**< \brief External interrupt 10 (SUPC_WKUP10) */
#define ID_SUPC_WKUP11  (70) /**< \brief External interrupt 11 (SUPC_WKUP11) */
#define ID_SUPC_WKUP12  (71) /**< \brief External interrupt 12 (SUPC_WKUP12) */
#define ID_SUPC_WKUP13  (72) /**< \brief External interrupt 13 (SUPC_WKUP13) */
#define ID_SUPC_WKUP14  (73) /**< \brief External interrupt 14 (SUPC_WKUP14) */
#define ID_SUPC_WKUP15  (74) /**< \brief External interrupt 15 (SUPC_WKUP15) */
#define ID_SEAFEC       (75) /**< \brief Smart Energy Analog Front-End Controller (SEAFEC) */
#define ID_SEAFEC_SLINK (76) /**< \brief Smart Energy Analog Front-End Controller Serial Link Interrupt (SEAFEC_SLINK) */
#define ID_SEAFEC_DATA  (77) /**< \brief Smart Energy Analog Front-End Controller Data Ready Interrupt (SEAFEC_DATA) */
#define ID_MEM2MEM1     (78) /**< \brief Metrology Core Memory to Memory Transfer Controller 1 (MEM2MEM1) (MEM2MEM1) */
#define ID_TC3_CHANNEL0 (79) /**< \brief Timer Counter 3, Channel 0 (TC3_CHANNEL0) */
#define ID_TC3_CHANNEL1 (80) /**< \brief Timer Counter 3, Channel 1 (TC3_CHANNEL1) */
#define ID_TC3_CHANNEL2 (81) /**< \brief Timer Counter 3, Channel 2 (TC3_CHANNEL2) */
#define ID_TC3_C0SEC    (82) /**< \brief Timer Counter 3, Channel 0, Secure IRQ (TC3_C0SEC) */
#define ID_TC3_C1SEC    (83) /**< \brief Timer Counter 3, Channel 1, Secure IRQ (TC3_C1SEC) */
#define ID_TC3_C2SEC    (84) /**< \brief Timer Counter 3, Channel 2, Secure IRQ (TC3_C2SEC) */
#define ID_PIOD         (85) /**< \brief Metrology Core Parallel I/O Controller D (PIOD) (PIOD) */
#define ID_PIOD_SEC     (86) /**< \brief Metrology Core Parallel I/O Controller A Secure Interupt (named as PIOD) (PIOD_SEC) */
#define ID_UART         (87) /**< \brief Optical UART (UART) */
#define ID_IPC1         (88) /**< \brief Metrology Core Interprocessor communication (IPC1) (IPC1) */
#define ID_MCSPI        (89) /**< \brief Multi-Channel SPI (MCSPI) */
#define ID_PWM          (90) /**< \brief Pulse Width Modulation (PWM) */
#define ID_FLEXRAM1     (91) /**< \brief Metrology Core RAM (FLEXRAM 1 & 2) (FLEXRAM1) */
#define ID_ARM1_FPU     (92) /**< \brief Metrology Core (CORE1) Floating Point Unit except IXC (ARM1_FPU) */
#define ID_ARM1_IXC     (93) /**< \brief Metrology Core (CORE1) FPU Interrupt IXC associated with FPU cumulative exception bit (ARM1_IXC) */
#define ID_MATRIX2      (94) /**< \brief High-Speed Metrology Core Matrix (HSMCM) (Formely MATRIX2) (MATRIX2) */
#define ID_MATRIX3      (95) /**< \brief Low-Speed Metrology Core Matrix (LSMCM) (Formely MATRIX3) (MATRIX3) */

#define ID_PERIPH_COUNT (97) /**< \brief Number of peripheral IDs */
/*@}*/

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */
/** \addtogroup PIC32CX2038MTSH100_base Peripheral Base Address Definitions */
/*@{*/

#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define FLEXCOM0     (0x40000000U) /**< \brief (FLEXCOM0    ) Base Address */
#define PDC_FLEXCOM0 (0x40000100U) /**< \brief (PDC_FLEXCOM0) Base Address */
#define USART0       (0x40000200U) /**< \brief (USART0      ) Base Address */
#define PDC_USART0   (0x40000300U) /**< \brief (PDC_USART0  ) Base Address */
#define SPI0         (0x40000400U) /**< \brief (SPI0        ) Base Address */
#define PDC_SPI0     (0x40000500U) /**< \brief (PDC_SPI0    ) Base Address */
#define TWI0         (0x40000600U) /**< \brief (TWI0        ) Base Address */
#define PDC_TWI0     (0x40000700U) /**< \brief (PDC_TWI0    ) Base Address */
#define FLEXCOM1     (0x40004000U) /**< \brief (FLEXCOM1    ) Base Address */
#define PDC_FLEXCOM1 (0x40004100U) /**< \brief (PDC_FLEXCOM1) Base Address */
#define USART1       (0x40004200U) /**< \brief (USART1      ) Base Address */
#define PDC_USART1   (0x40004300U) /**< \brief (PDC_USART1  ) Base Address */
#define SPI1         (0x40004400U) /**< \brief (SPI1        ) Base Address */
#define PDC_SPI1     (0x40004500U) /**< \brief (PDC_SPI1    ) Base Address */
#define TWI1         (0x40004600U) /**< \brief (TWI1        ) Base Address */
#define PDC_TWI1     (0x40004700U) /**< \brief (PDC_TWI1    ) Base Address */
#define FLEXCOM2     (0x40008000U) /**< \brief (FLEXCOM2    ) Base Address */
#define PDC_FLEXCOM2 (0x40008100U) /**< \brief (PDC_FLEXCOM2) Base Address */
#define USART2       (0x40008200U) /**< \brief (USART2      ) Base Address */
#define PDC_USART2   (0x40008300U) /**< \brief (PDC_USART2  ) Base Address */
#define SPI2         (0x40008400U) /**< \brief (SPI2        ) Base Address */
#define PDC_SPI2     (0x40008500U) /**< \brief (PDC_SPI2    ) Base Address */
#define TWI2         (0x40008600U) /**< \brief (TWI2        ) Base Address */
#define PDC_TWI2     (0x40008700U) /**< \brief (PDC_TWI2    ) Base Address */
#define FLEXCOM3     (0x4000C000U) /**< \brief (FLEXCOM3    ) Base Address */
#define PDC_FLEXCOM3 (0x4000C100U) /**< \brief (PDC_FLEXCOM3) Base Address */
#define USART3       (0x4000C200U) /**< \brief (USART3      ) Base Address */
#define PDC_USART3   (0x4000C300U) /**< \brief (PDC_USART3  ) Base Address */
#define SPI3         (0x4000C400U) /**< \brief (SPI3        ) Base Address */
#define PDC_SPI3     (0x4000C500U) /**< \brief (PDC_SPI3    ) Base Address */
#define TWI3         (0x4000C600U) /**< \brief (TWI3        ) Base Address */
#define PDC_TWI3     (0x4000C700U) /**< \brief (PDC_TWI3    ) Base Address */
#define FLEXCOM4     (0x40010000U) /**< \brief (FLEXCOM4    ) Base Address */
#define PDC_FLEXCOM4 (0x40010100U) /**< \brief (PDC_FLEXCOM4) Base Address */
#define USART4       (0x40010200U) /**< \brief (USART4      ) Base Address */
#define PDC_USART4   (0x40010300U) /**< \brief (PDC_USART4  ) Base Address */
#define SPI4         (0x40010400U) /**< \brief (SPI4        ) Base Address */
#define PDC_SPI4     (0x40010500U) /**< \brief (PDC_SPI4    ) Base Address */
#define TWI4         (0x40010600U) /**< \brief (TWI4        ) Base Address */
#define PDC_TWI4     (0x40010700U) /**< \brief (PDC_TWI4    ) Base Address */
#define FLEXCOM5     (0x40014000U) /**< \brief (FLEXCOM5    ) Base Address */
#define PDC_FLEXCOM5 (0x40014100U) /**< \brief (PDC_FLEXCOM5) Base Address */
#define USART5       (0x40014200U) /**< \brief (USART5      ) Base Address */
#define PDC_USART5   (0x40014300U) /**< \brief (PDC_USART5  ) Base Address */
#define SPI5         (0x40014400U) /**< \brief (SPI5        ) Base Address */
#define PDC_SPI5     (0x40014500U) /**< \brief (PDC_SPI5    ) Base Address */
#define TWI5         (0x40014600U) /**< \brief (TWI5        ) Base Address */
#define PDC_TWI5     (0x40014700U) /**< \brief (PDC_TWI5    ) Base Address */
#define FLEXCOM6     (0x40018000U) /**< \brief (FLEXCOM6    ) Base Address */
#define PDC_FLEXCOM6 (0x40018100U) /**< \brief (PDC_FLEXCOM6) Base Address */
#define USART6       (0x40018200U) /**< \brief (USART6      ) Base Address */
#define PDC_USART6   (0x40018300U) /**< \brief (PDC_USART6  ) Base Address */
#define SPI6         (0x40018400U) /**< \brief (SPI6        ) Base Address */
#define PDC_SPI6     (0x40018500U) /**< \brief (PDC_SPI6    ) Base Address */
#define TWI6         (0x40018600U) /**< \brief (TWI6        ) Base Address */
#define PDC_TWI6     (0x40018700U) /**< \brief (PDC_TWI6    ) Base Address */
#define FLEXCOM7     (0x4001C000U) /**< \brief (FLEXCOM7    ) Base Address */
#define PDC_FLEXCOM7 (0x4001C100U) /**< \brief (PDC_FLEXCOM7) Base Address */
#define USART7       (0x4001C200U) /**< \brief (USART7      ) Base Address */
#define PDC_USART7   (0x4001C300U) /**< \brief (PDC_USART7  ) Base Address */
#define SPI7         (0x4001C400U) /**< \brief (SPI7        ) Base Address */
#define PDC_SPI7     (0x4001C500U) /**< \brief (PDC_SPI7    ) Base Address */
#define TWI7         (0x4001C600U) /**< \brief (TWI7        ) Base Address */
#define PDC_TWI7     (0x4001C700U) /**< \brief (PDC_TWI7    ) Base Address */
#define QSPI0        (0x40020000U) /**< \brief (QSPI0       ) Base Address */
#define PDC_QSPI     (0x40020100U) /**< \brief (PDC_QSPI    ) Base Address */
#define ADC          (0x40024000U) /**< \brief (ADC         ) Base Address */
#define PDC_ADC      (0x40024100U) /**< \brief (PDC_ADC     ) Base Address */
#define ACC          (0x40028000U) /**< \brief (ACC         ) Base Address */
#define IPC0         (0x4002C000U) /**< \brief (IPC0        ) Base Address */
#define SLCDC        (0x40030000U) /**< \brief (SLCDC       ) Base Address */
#define MEM2MEM0     (0x40034000U) /**< \brief (MEM2MEM0    ) Base Address */
#define PDC_MEM2MEM0 (0x40034100U) /**< \brief (PDC_MEM2MEM0) Base Address */
#define TC0          (0x40038000U) /**< \brief (TC0         ) Base Address */
#define PDC_TC0      (0x40038100U) /**< \brief (PDC_TC0     ) Base Address */
#define TC1          (0x4003C000U) /**< \brief (TC1         ) Base Address */
#define PDC_TC1      (0x4003C100U) /**< \brief (PDC_TC1     ) Base Address */
#define TC2          (0x40040000U) /**< \brief (TC2         ) Base Address */
#define PDC_TC2      (0x40040100U) /**< \brief (PDC_TC2     ) Base Address */
#define MATRIX1      (0x40044000U) /**< \brief (MATRIX1     ) Base Address */
#define PIOA         (0x40048000U) /**< \brief (PIOA        ) Base Address */
#define PIOB         (0x40048040U) /**< \brief (PIOB        ) Base Address */
#define PIOC         (0x40048080U) /**< \brief (PIOC        ) Base Address */
#define CHIPID       (0x40050200U) /**< \brief (CHIPID      ) Base Address */
#define SFR          (0x40050400U) /**< \brief (SFR         ) Base Address */
#define SFRBU        (0x40050600U) /**< \brief (SFRBU       ) Base Address */
#define DWDT         (0x40052000U) /**< \brief (DWDT        ) Base Address */
#define RSTC         (0x40053000U) /**< \brief (RSTC        ) Base Address */
#define RTT          (0x40053020U) /**< \brief (RTT         ) Base Address */
#define GPBR         (0x40053060U) /**< \brief (GPBR        ) Base Address */
#define RTC          (0x40053100U) /**< \brief (RTC         ) Base Address */
#define SYSCWP       (0x400531A0U) /**< \brief (SYSCWP      ) Base Address */
#define SUPC         (0x400531D0U) /**< \brief (SUPC        ) Base Address */
#define AES          (0x44000000U) /**< \brief (AES         ) Base Address */
#define PDC_AES      (0x44000100U) /**< \brief (PDC_AES     ) Base Address */
#define AESB         (0x44004000U) /**< \brief (AESB        ) Base Address */
#define SHA          (0x44008000U) /**< \brief (SHA         ) Base Address */
#define PDC_SHA      (0x44008100U) /**< \brief (PDC_SHA     ) Base Address */
#define TRNG         (0x4400C000U) /**< \brief (TRNG        ) Base Address */
#define ICM          (0x44010000U) /**< \brief (ICM         ) Base Address */
#define CPKCC        (0x46000000U) /**< \brief (CPKCC       ) Base Address */
#define MATRIX0      (0x46004000U) /**< \brief (MATRIX0     ) Base Address */
#define CMCC0        (0x46008000U) /**< \brief (CMCC0       ) Base Address */
#define CMCC1        (0x4600C000U) /**< \brief (CMCC1       ) Base Address */
#define SEFC0        (0x460E0000U) /**< \brief (SEFC0       ) Base Address */
#define SEFC1        (0x460E0200U) /**< \brief (SEFC1       ) Base Address */
#define PMC          (0x46800000U) /**< \brief (PMC         ) Base Address */
#define EMAFE        (0x48000000U) /**< \brief (EMAFE       ) Base Address */
#define MEM2MEM1     (0x48004000U) /**< \brief (MEM2MEM1    ) Base Address */
#define PDC_MEM2MEM1 (0x48004100U) /**< \brief (PDC_MEM2MEM1) Base Address */
#define TC3          (0x48008000U) /**< \brief (TC3         ) Base Address */
#define PDC_TC3      (0x48008100U) /**< \brief (PDC_TC3     ) Base Address */
#define PIOD         (0x4800C000U) /**< \brief (PIOD        ) Base Address */
#define UART         (0x48010000U) /**< \brief (UART        ) Base Address */
#define PDC_UART     (0x48010100U) /**< \brief (PDC_UART    ) Base Address */
#define IPC1         (0x48014000U) /**< \brief (IPC1        ) Base Address */
#define MCSPI        (0x48018000U) /**< \brief (MCSPI       ) Base Address */
#define PDC_MCSPI    (0x48018100U) /**< \brief (PDC_MCSPI   ) Base Address */
#define PWM          (0x4801C000U) /**< \brief (PWM         ) Base Address */
#define PDC_PWM      (0x4801C100U) /**< \brief (PDC_PWM     ) Base Address */
#define MATRIX3      (0x48020000U) /**< \brief (MATRIX3     ) Base Address */
#define MATRIX2      (0x4A000000U) /**< \brief (MATRIX2     ) Base Address */
#else
#define FLEXCOM0     ((Flexcom    *)0x40000000U) /**< \brief (FLEXCOM0    ) Base Address */
#define PDC_FLEXCOM0 ((Pdc        *)0x40000100U) /**< \brief (PDC_FLEXCOM0) Base Address */
#define USART0       ((Usart      *)0x40000200U) /**< \brief (USART0      ) Base Address */
#define PDC_USART0   ((Pdc        *)0x40000300U) /**< \brief (PDC_USART0  ) Base Address */
#define SPI0         ((Spi        *)0x40000400U) /**< \brief (SPI0        ) Base Address */
#define PDC_SPI0     ((Pdc        *)0x40000500U) /**< \brief (PDC_SPI0    ) Base Address */
#define TWI0         ((Twi        *)0x40000600U) /**< \brief (TWI0        ) Base Address */
#define PDC_TWI0     ((Pdc        *)0x40000700U) /**< \brief (PDC_TWI0    ) Base Address */
#define FLEXCOM1     ((Flexcom    *)0x40004000U) /**< \brief (FLEXCOM1    ) Base Address */
#define PDC_FLEXCOM1 ((Pdc        *)0x40004100U) /**< \brief (PDC_FLEXCOM1) Base Address */
#define USART1       ((Usart      *)0x40004200U) /**< \brief (USART1      ) Base Address */
#define PDC_USART1   ((Pdc        *)0x40004300U) /**< \brief (PDC_USART1  ) Base Address */
#define SPI1         ((Spi        *)0x40004400U) /**< \brief (SPI1        ) Base Address */
#define PDC_SPI1     ((Pdc        *)0x40004500U) /**< \brief (PDC_SPI1    ) Base Address */
#define TWI1         ((Twi        *)0x40004600U) /**< \brief (TWI1        ) Base Address */
#define PDC_TWI1     ((Pdc        *)0x40004700U) /**< \brief (PDC_TWI1    ) Base Address */
#define FLEXCOM2     ((Flexcom    *)0x40008000U) /**< \brief (FLEXCOM2    ) Base Address */
#define PDC_FLEXCOM2 ((Pdc        *)0x40008100U) /**< \brief (PDC_FLEXCOM2) Base Address */
#define USART2       ((Usart      *)0x40008200U) /**< \brief (USART2      ) Base Address */
#define PDC_USART2   ((Pdc        *)0x40008300U) /**< \brief (PDC_USART2  ) Base Address */
#define SPI2         ((Spi        *)0x40008400U) /**< \brief (SPI2        ) Base Address */
#define PDC_SPI2     ((Pdc        *)0x40008500U) /**< \brief (PDC_SPI2    ) Base Address */
#define TWI2         ((Twi        *)0x40008600U) /**< \brief (TW20        ) Base Address */
#define PDC_TWI2     ((Pdc        *)0x40008700U) /**< \brief (PDC_TWI2    ) Base Address */
#define FLEXCOM3     ((Flexcom    *)0x4000C000U) /**< \brief (FLEXCOM3    ) Base Address */
#define PDC_FLEXCOM3 ((Pdc        *)0x4000C100U) /**< \brief (PDC_FLEXCOM3) Base Address */
#define USART3       ((Usart      *)0x4000C200U) /**< \brief (USART3      ) Base Address */
#define PDC_USART3   ((Pdc        *)0x4000C300U) /**< \brief (PDC_USART3  ) Base Address */
#define SPI3         ((Spi        *)0x4000C400U) /**< \brief (SPI3        ) Base Address */
#define PDC_SPI3     ((Pdc        *)0x4000C500U) /**< \brief (PDC_SPI3    ) Base Address */
#define TWI3         ((Twi        *)0x4000C600U) /**< \brief (TWI3        ) Base Address */
#define PDC_TWI3     ((Pdc        *)0x4000C700U) /**< \brief (PDC_TWI3    ) Base Address */
#define FLEXCOM4     ((Flexcom    *)0x40010000U) /**< \brief (FLEXCOM4    ) Base Address */
#define PDC_FLEXCOM4 ((Pdc        *)0x40010100U) /**< \brief (PDC_FLEXCOM4) Base Address */
#define USART4       ((Usart      *)0x40010200U) /**< \brief (USART4      ) Base Address */
#define PDC_USART4   ((Pdc        *)0x40010300U) /**< \brief (PDC_USART4  ) Base Address */
#define SPI4         ((Spi        *)0x40010400U) /**< \brief (SPI4        ) Base Address */
#define PDC_SPI4     ((Pdc        *)0x40010500U) /**< \brief (PDC_SPI4    ) Base Address */
#define TWI4         ((Twi        *)0x40010600U) /**< \brief (TWI4        ) Base Address */
#define PDC_TWI4     ((Pdc        *)0x40010700U) /**< \brief (PDC_TWI4    ) Base Address */
#define FLEXCOM5     ((Flexcom    *)0x40014000U) /**< \brief (FLEXCOM5    ) Base Address */
#define PDC_FLEXCOM5 ((Pdc        *)0x40014100U) /**< \brief (PDC_FLEXCOM5) Base Address */
#define USART5       ((Usart      *)0x40014200U) /**< \brief (USART5      ) Base Address */
#define PDC_USART5   ((Pdc        *)0x40014300U) /**< \brief (PDC_USART5  ) Base Address */
#define SPI5         ((Spi        *)0x40014400U) /**< \brief (SPI5        ) Base Address */
#define PDC_SPI5     ((Pdc        *)0x40014500U) /**< \brief (PDC_SPI5    ) Base Address */
#define TWI5         ((Twi        *)0x40014600U) /**< \brief (TWI5        ) Base Address */
#define PDC_TWI5     ((Pdc        *)0x40014700U) /**< \brief (PDC_TWI5    ) Base Address */
#define FLEXCOM6     ((Flexcom    *)0x40018000U) /**< \brief (FLEXCOM6    ) Base Address */
#define PDC_FLEXCOM6 ((Pdc        *)0x40018100U) /**< \brief (PDC_FLEXCOM6) Base Address */
#define USART6       ((Usart      *)0x40018200U) /**< \brief (USART6      ) Base Address */
#define PDC_USART6   ((Pdc        *)0x40018300U) /**< \brief (PDC_USART6  ) Base Address */
#define SPI6         ((Spi        *)0x40018400U) /**< \brief (SPI6        ) Base Address */
#define PDC_SPI6     ((Pdc        *)0x40018500U) /**< \brief (PDC_SPI6    ) Base Address */
#define TWI6         ((Twi        *)0x40018600U) /**< \brief (TWI6        ) Base Address */
#define PDC_TWI6     ((Pdc        *)0x40018700U) /**< \brief (PDC_TWI6    ) Base Address */
#define FLEXCOM7     ((Flexcom    *)0x4001C000U) /**< \brief (FLEXCOM7    ) Base Address */
#define PDC_FLEXCOM7 ((Pdc        *)0x4001C100U) /**< \brief (PDC_FLEXCOM7) Base Address */
#define USART7       ((Usart      *)0x4001C200U) /**< \brief (USART7      ) Base Address */
#define PDC_USART7   ((Pdc        *)0x4001C300U) /**< \brief (PDC_USART7  ) Base Address */
#define SPI7         ((Spi        *)0x4001C400U) /**< \brief (SPI7        ) Base Address */
#define PDC_SPI7     ((Pdc        *)0x4001C500U) /**< \brief (PDC_SPI7    ) Base Address */
#define TWI7         ((Twi        *)0x4001C600U) /**< \brief (TWI7        ) Base Address */
#define PDC_TWI7     ((Pdc        *)0x4001C700U) /**< \brief (PDC_TWI7    ) Base Address */
#define QSPI0        ((Qspi       *)0x40020000U) /**< \brief (QSPI0       ) Base Address */
#define PDC_QSPI     ((Pdc        *)0x40020100U) /**< \brief (PDC_QSPI    ) Base Address */
#define ADC          ((Adc        *)0x40024000U) /**< \brief (ADC         ) Base Address */
#define PDC_ADC      ((Pdc        *)0x40024100U) /**< \brief (PDC_ADC     ) Base Address */
#define ACC          ((Acc        *)0x40028000U) /**< \brief (ACC         ) Base Address */
#define IPC0         ((Ipc        *)0x4002C000U) /**< \brief (IPC0        ) Base Address */
#define SLCDC        ((Slcdc      *)0x40030000U) /**< \brief (SLCDC       ) Base Address */
#define MEM2MEM0     ((Mem2mem    *)0x40034000U) /**< \brief (MEM2MEM0    ) Base Address */
#define PDC_MEM2MEM0 ((Pdc        *)0x40034100U) /**< \brief (PDC_MEM2MEM0) Base Address */
#define TC0          ((Tc         *)0x40038000U) /**< \brief (TC0         ) Base Address */
#define PDC_TC0      ((Pdc        *)0x40038100U) /**< \brief (PDC_TC0     ) Base Address */
#define TC1          ((Tc         *)0x4003C000U) /**< \brief (TC1         ) Base Address */
#define PDC_TC1      ((Pdc        *)0x4003C100U) /**< \brief (PDC_TC1     ) Base Address */
#define TC2          ((Tc         *)0x40040000U) /**< \brief (TC2         ) Base Address */
#define PDC_TC2      ((Pdc        *)0x40040100U) /**< \brief (PDC_TC2     ) Base Address */
#define MATRIX1      ((Matrix     *)0x40044000U) /**< \brief (MATRIX1     ) Base Address */
#define PIOA         ((PioGroup   *)0x40048000U) /**< \brief (PIOA        ) Base Address */
#define PIOB         ((PioGroup   *)0x40048040U) /**< \brief (PIOB        ) Base Address */
#define PIOC         ((PioGroup   *)0x40048080U) /**< \brief (PIOC        ) Base Address */
#define CHIPID       ((Chipid     *)0x40050200U) /**< \brief (CHIPID      ) Base Address */
#define SFR          ((Sfr        *)0x40050400U) /**< \brief (SFR         ) Base Address */
#define SFRBU        ((Sfrbu      *)0x40050600U) /**< \brief (SFRBU       ) Base Address */
#define DWDT         ((Dwdt       *)0x40052000U) /**< \brief (DWDT        ) Base Address */
#define RSTC         ((Rstc       *)0x40053000U) /**< \brief (RSTC        ) Base Address */
#define RTT          ((Rtt        *)0x40053020U) /**< \brief (RTT         ) Base Address */
#define GPBR         ((Gpbr       *)0x40053060U) /**< \brief (GPBR        ) Base Address */
#define RTC          ((Rtc        *)0x40053100U) /**< \brief (RTC         ) Base Address */
#define SYSCWP       ((Syscwp     *)0x400531A0U) /**< \brief (SYSCWP      ) Base Address */
#define SUPC         ((Supc       *)0x400531D0U) /**< \brief (SUPC        ) Base Address */
#define AES          ((Aes        *)0x44000000U) /**< \brief (AES         ) Base Address */
#define PDC_AES      ((Pdc        *)0x44000100U) /**< \brief (PDC_AES     ) Base Address */
#define AESB         ((Aesb       *)0x44004000U) /**< \brief (AESB        ) Base Address */
#define SHA          ((Sha        *)0x44008000U) /**< \brief (SHA         ) Base Address */
#define PDC_SHA      ((Pdc        *)0x44008100U) /**< \brief (PDC_SHA     ) Base Address */
#define TRNG         ((Trng       *)0x4400C000U) /**< \brief (TRNG        ) Base Address */
#define ICM          ((Icm        *)0x44010000U) /**< \brief (ICM         ) Base Address */
#define CPKCC        ((Cpkcc      *)0x46000000U) /**< \brief (CPKCC       ) Base Address */
#define MATRIX0      ((Matrix     *)0x46004000U) /**< \brief (MATRIX0     ) Base Address */
#define CMCC0        ((Cmcc       *)0x46008000U) /**< \brief (CMCC0       ) Base Address */
#define CMCC1        ((Cmcc       *)0x4600C000U) /**< \brief (CMCC1       ) Base Address */
#define SEFC0        ((Sefc       *)0x460E0000U) /**< \brief (SEFC0       ) Base Address */
#define SEFC1        ((Sefc       *)0x460E0200U) /**< \brief (SEFC1       ) Base Address */
#define PMC          ((Pmc        *)0x46800000U) /**< \brief (PMC         ) Base Address */
#define EMAFE        ((Emafelpfif *)0x48000000U) /**< \brief (EMAFE       ) Base Address */
#define MEM2MEM1     ((Mem2mem    *)0x48004000U) /**< \brief (MEM2MEM1    ) Base Address */
#define PDC_MEM2MEM1 ((Pdc        *)0x48004100U) /**< \brief (PDC_MEM2MEM1) Base Address */
#define TC3          ((Tc         *)0x48008000U) /**< \brief (TC3         ) Base Address */
#define PDC_TC3      ((Pdc        *)0x48008100U) /**< \brief (PDC_TC3     ) Base Address */
#define PIOD         ((PioGroup   *)0x4800C000U) /**< \brief (PIOD        ) Base Address */
#define UART         ((Uart       *)0x48010000U) /**< \brief (UART        ) Base Address */
#define PDC_UART     ((Pdc        *)0x48010100U) /**< \brief (PDC_UART    ) Base Address */
#define IPC1         ((Ipc        *)0x48014000U) /**< \brief (IPC1        ) Base Address */
#define MCSPI        ((Mcspi      *)0x48018000U) /**< \brief (MCSPI       ) Base Address */
#define PDC_MCSPI    ((Pdc        *)0x48018100U) /**< \brief (PDC_MCSPI   ) Base Address */
#define PWM          ((Pwm        *)0x4801C000U) /**< \brief (PWM         ) Base Address */
#define PDC_PWM      ((Pdc        *)0x4801C100U) /**< \brief (PDC_PWM     ) Base Address */
#define MATRIX3      ((Matrix     *)0x48020000U) /**< \brief (MATRIX3     ) Base Address */
#define MATRIX2      ((Matrix     *)0x4A000000U) /**< \brief (MATRIX2     ) Base Address */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/*@}*/

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */
/** \addtogroup PIC32CX2038MTSH100_pio Peripheral Pio Definitions */
/*@{*/

#include "pio/pic32cx2038mtsh100.h"
/*@}*/

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */

#define IFLASH0_PAGE_SIZE        (512u)
#define IFLASH0_LOCK_REGION_SIZE (8192u)
#define IFLASH0_NB_OF_PAGES      (2048u)
#define IFLASH0_NB_OF_LOCK_BITS  (256u)
#define IFLASH1_PAGE_SIZE        (512u)
#define IFLASH1_LOCK_REGION_SIZE (8192u)
#define IFLASH1_NB_OF_PAGES      (2048u)
#define IFLASH1_NB_OF_LOCK_BITS  (256u)
#define IRAM1_SIZE               (0x8000u)
#define IRAM2_SIZE               (0x4000u)
#define IRAM_SIZE                (IRAM1_SIZE + IRAM2_SIZE)

#define IFLASH0_CNC_ADDR        (0x01000000u) /**< Internal Flash (Code - Non Cached) base address */
#if defined IFLASH0_SIZE
#define IFLASH1_CNC_ADDR        (IFLASH0_CNC_ADDR + IFLASH0_SIZE) /**< Internal Flash (Code - Non Cached) base address */
#endif
#define CPKCC_ROM_ADDR          (0x02020000u) /**< CPKCC ROM base address */
#define CPKCC_RAM_ADDR          (0x02031000u) /**< CPKCC RAM base address */
#define QSPI_MEM_CNC_ADDR       (0x04000000u) /**< QSPI Memory (Code - Non Cached) base address */
#define QSPI_AESB_CNC_ADDR      (0x06000000u) /**< QPSI Memory crypted with AESB (Code - Non Cached) base address */
#define IFLASH0_CC_ADDR         (0x11000000u) /**< Internal Flash (Code - Cached) base address */
#if defined IFLASH0_SIZE
#define IFLASH1_CC_ADDR         (IFLASH0_CC_ADDR + IFLASH0_SIZE) /**< Internal Flash (Code - Cached) base address */
#endif
#define QSPI_MEM_CC_ADDR        (0x14000000u) /**< QSPI Memory (Code - Cached) base address */
#define QSPI_AESB_CC_ADDR       (0x16000000u) /**< QPSI Memory crypted with AESB (Code - Cached) base address */
#define DTCM_ADDR               (0x1FFFA000u) /**< Data Tightly Coupled Memory base address */
#define ITCM_ADDR               (0x1FFFC000u) /**< Instruction Tightly Coupled Memory base address */
#define IRAM1_ADDR              (0x20080000u) /**< Internal RAM 1 base address */
#define IRAM2_ADDR              (0x20088000u) /**< Internal RAM 2 base address */
#define IFLASH_W_ADDR           (0xA1000000u) /**< Internal Flash Write base address */
#define CORTEXM_PRIVATE_PB_ADDR (0xE0000000u) /**< Cortex-M Private Peripheral Bus base address */

/* ************************************************************************** */
/*   MISCELLANEOUS DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */

#define CHIP_CIDR   (0x2C220EE0UL)
#define CHIP_EXID   (0x00000000UL)

/* ************************************************************************** */
/*   ELECTRICAL DEFINITIONS FOR PIC32CX2038MTSH100 */
/* ************************************************************************** */

/* Device characteristics */
#define CHIP_FREQ_SLCK_RC               (31700UL)
#define CHIP_FREQ_MAINCK_RC_12MHZ       (12110000UL)
#define CHIP_FREQ_CPU_MAX               (200000000UL)
#define CHIP_FREQ_XTAL_32K              (32768UL)

/* Embedded Flash Write Wait State */
#define CHIP_FLASH_WRITE_WAIT_STATE     (6U)

/* Embedded Flash Read Wait State (VDDCORE set at 1.20V / VDDIO set between 2.7V to 3.6V / @ 85°C) */
#define CHIP_FREQ_FWS_0                 (21000000UL)  /**< \brief Maximum operating frequency when FWS is 0 */
#define CHIP_FREQ_FWS_1                 (42000000UL)  /**< \brief Maximum operating frequency when FWS is 1 */
#define CHIP_FREQ_FWS_2                 (63000000UL)  /**< \brief Maximum operating frequency when FWS is 2 */
#define CHIP_FREQ_FWS_3                 (85000000UL)  /**< \brief Maximum operating frequency when FWS is 3 */
#define CHIP_FREQ_FWS_4                 (106000000UL) /**< \brief Maximum operating frequency when FWS is 4 */
#define CHIP_FREQ_FWS_5                 (121000000UL) /**< \brief Maximum operating frequency when FWS is 5 */

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* _PIC32CX2038MTSH100_ */
