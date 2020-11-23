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

#include "pic32cx.h"

typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);
int __tcm_initialization(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M4 core handlers */
#pragma weak NMI_Handler=Dummy_Handler
#pragma weak HardFault_Handler=Dummy_Handler
#pragma weak MemManage_Handler=Dummy_Handler
#pragma weak BusFault_Handler=Dummy_Handler
#pragma weak UsageFault_Handler=Dummy_Handler
#pragma weak SVC_Handler=Dummy_Handler
#pragma weak DebugMon_Handler=Dummy_Handler
#pragma weak PendSV_Handler=Dummy_Handler
#pragma weak SysTick_Handler=Dummy_Handler

/* Peripherals handlers */
#pragma weak SUPC_Handler=Dummy_Handler          
#pragma weak RSTC_Handler=Dummy_Handler          
#pragma weak RTC_Handler=Dummy_Handler           
#pragma weak RTT_Handler=Dummy_Handler           
#pragma weak DWDT_Handler=Dummy_Handler          
#pragma weak PMC_Handler=Dummy_Handler           
#pragma weak SEFC0_Handler=Dummy_Handler         
#pragma weak SEFC1_Handler=Dummy_Handler         
#pragma weak FLEXCOM0_Handler=Dummy_Handler      
#pragma weak FLEXCOM1_Handler=Dummy_Handler      
#pragma weak FLEXCOM2_Handler=Dummy_Handler      
#pragma weak FLEXCOM3_Handler=Dummy_Handler      
#pragma weak FLEXCOM4_Handler=Dummy_Handler      
#pragma weak FLEXCOM5_Handler=Dummy_Handler      
#pragma weak FLEXCOM6_Handler=Dummy_Handler      
#pragma weak FLEXCOM7_Handler=Dummy_Handler      
#pragma weak PIOA_Handler=Dummy_Handler          
#pragma weak PIOA_SEC_Handler=Dummy_Handler      
#pragma weak PIOB_Handler=Dummy_Handler          
#pragma weak PIOB_SEC_Handler=Dummy_Handler      
#pragma weak PIOC_Handler=Dummy_Handler          
#pragma weak PIOC_SEC_Handler=Dummy_Handler      
#pragma weak QSPI_Handler=Dummy_Handler          
#pragma weak ADC_Handler=Dummy_Handler           
#pragma weak ACC_Handler=Dummy_Handler           
#pragma weak ARM0_FPU_Handler=Dummy_Handler      
#pragma weak ARM0_IXC_Handler=Dummy_Handler      
#pragma weak IPC0_Handler=Dummy_Handler          
#pragma weak SLCDC_Handler=Dummy_Handler         
#pragma weak MEM2MEM0_Handler=Dummy_Handler      
#pragma weak TC0_CHANNEL0_Handler=Dummy_Handler  
#pragma weak TC0_CHANNEL1_Handler=Dummy_Handler  
#pragma weak TC0_CHANNEL2_Handler=Dummy_Handler  
#pragma weak TC1_CHANNEL0_Handler=Dummy_Handler  
#pragma weak TC1_CHANNEL1_Handler=Dummy_Handler  
#pragma weak TC1_CHANNEL2_Handler=Dummy_Handler  
#pragma weak TC2_CHANNEL0_Handler=Dummy_Handler  
#pragma weak TC2_CHANNEL1_Handler=Dummy_Handler  
#pragma weak TC2_CHANNEL2_Handler=Dummy_Handler  
#pragma weak TC0_C0SEC_Handler=Dummy_Handler     
#pragma weak TC0_C1SEC_Handler=Dummy_Handler     
#pragma weak TC0_C2SEC_Handler=Dummy_Handler     
#pragma weak TC1_C0SEC_Handler=Dummy_Handler     
#pragma weak TC1_C1SEC_Handler=Dummy_Handler     
#pragma weak TC1_C2SEC_Handler=Dummy_Handler     
#pragma weak TC2_C0SEC_Handler=Dummy_Handler     
#pragma weak TC2_C1SEC_Handler=Dummy_Handler     
#pragma weak TC2_C2SEC_Handler=Dummy_Handler     
#pragma weak AES_Handler=Dummy_Handler           
#pragma weak AES_AESSEC_Handler=Dummy_Handler    
#pragma weak AESB_Handler=Dummy_Handler          
#pragma weak AESB_AESBSEC_Handler=Dummy_Handler  
#pragma weak SHA_Handler=Dummy_Handler           
#pragma weak SHA_SHASEC_Handler=Dummy_Handler    
#pragma weak TRNG_Handler=Dummy_Handler          
#pragma weak TRNG_TRNGSEC_Handler=Dummy_Handler  
#pragma weak ICM_Handler=Dummy_Handler           
#pragma weak ICM_ICMSEC_Handler=Dummy_Handler    
#pragma weak CPKCC_Handler=Dummy_Handler         
#pragma weak MATRIX0_Handler=Dummy_Handler       
#pragma weak MATRIX1_Handler=Dummy_Handler       
#pragma weak SUPC_WKUP3_Handler =Dummy_Handler   
#pragma weak SUPC_WKUP4_Handler =Dummy_Handler   
#pragma weak SUPC_WKUP5_Handler =Dummy_Handler   
#pragma weak SUPC_WKUP6_Handler =Dummy_Handler   
#pragma weak SUPC_WKUP7_Handler =Dummy_Handler   
#pragma weak SUPC_WKUP8_Handler =Dummy_Handler   
#pragma weak SUPC_WKUP9_Handler =Dummy_Handler   
#pragma weak SUPC_WKUP10_Handler=Dummy_Handler   
#pragma weak SUPC_WKUP11_Handler=Dummy_Handler   
#pragma weak SUPC_WKUP12_Handler=Dummy_Handler   
#pragma weak SUPC_WKUP13_Handler=Dummy_Handler   
#pragma weak SUPC_WKUP14_Handler=Dummy_Handler   
#pragma weak SUPC_WKUP15_Handler=Dummy_Handler   
#pragma weak SEAFEC_SLINK_Handler=Dummy_Handler  
#pragma weak SEAFEC_DATA_Handler=Dummy_Handler   
#pragma weak MEM2MEM1_Handler=Dummy_Handler      
#pragma weak TC3_CHANNEL0_Handler=Dummy_Handler  
#pragma weak TC3_CHANNEL1_Handler=Dummy_Handler  
#pragma weak TC3_CHANNEL2_Handler=Dummy_Handler  
#pragma weak TC3_C0SEC_Handler=Dummy_Handler     
#pragma weak TC3_C1SEC_Handler=Dummy_Handler     
#pragma weak TC3_C2SEC_Handler=Dummy_Handler     
#pragma weak PIOD_Handler=Dummy_Handler          
#pragma weak PIOD_SEC_Handler=Dummy_Handler      
#pragma weak UART_Handler=Dummy_Handler          
#pragma weak IPC1_Handler=Dummy_Handler          
#pragma weak MCSPI_Handler=Dummy_Handler         
#pragma weak PWM_Handler=Dummy_Handler           
#pragma weak ARM1_FPU_Handler=Dummy_Handler      
#pragma weak ARM1_IXC_Handler=Dummy_Handler      
#pragma weak MATRIX2_Handler=Dummy_Handler       
#pragma weak MATRIX3_Handler=Dummy_Handler       

/* Exception Table */
#pragma language = extended
#pragma segment = "CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
const DeviceVectors __vector_table = {
        (void*) __sfe("CSTACK"),

        .pfnReset_Handler         = (void*) Reset_Handler,
        .pfnNMI_Handler           = (void*) NMI_Handler,
        .pfnHardFault_Handler     = (void*) HardFault_Handler,
        .pfnMemManage_Handler     = (void*) MemManage_Handler,
        .pfnBusFault_Handler      = (void*) BusFault_Handler,
        .pfnUsageFault_Handler    = (void*) UsageFault_Handler,
        .pfnReserved1_Handler     = (void*) (0UL),                  /* Reserved */
        .pfnReserved2_Handler     = (void*) (0UL),                  /* Reserved */
        .pfnReserved3_Handler     = (void*) (0UL),                  /* Reserved */
        .pfnReserved4_Handler     = (void*) (0UL),                  /* Reserved */
        .pfnSVC_Handler           = (void*) SVC_Handler,            
        .pfnDebugMon_Handler      = (void*) DebugMon_Handler,       
        .pfnReserved5_Handler     = (void*) (0UL),                  /* Reserved */
        .pfnPendSV_Handler        = (void*) PendSV_Handler,
        .pfnSysTick_Handler       = (void*) SysTick_Handler,

        /* Configurable interrupts */
        .pfnSUPC_Handler          = (void*) SUPC_Handler,           /*  0 Supply Controller */
        .pfnRSTC_Handler          = (void*) RSTC_Handler,           /*  1 Reset Controller */
        .pfnRTC_Handler           = (void*) RTC_Handler,            /*  2 Real Time Clock */
        .pfnRTT_Handler           = (void*) RTT_Handler,            /*  3 Real Time Timer */
        .pfnDWDT_Handler          = (void*) DWDT_Handler,           /*  4 Dual Watchdog Timer */
        .pvReserved5              = (void*) (0UL),                  /*  5 Reserved */
        .pfnPMC_Handler           = (void*) PMC_Handler,            /*  6 Power Management Controller */
        .pfnSEFC0_Handler         = (void*) SEFC0_Handler,          /*  7 Embedded Flash Controller 0 */
        .pfnSEFC1_Handler         = (void*) SEFC1_Handler,          /*  8 Embedded Flash Controller 1 */
        .pfnFLEXCOM0_Handler      = (void*) FLEXCOM0_Handler,       /*  9 FLEXCOM 0 (USART0/SPI0/TWI0) */
        .pfnFLEXCOM1_Handler      = (void*) FLEXCOM1_Handler,       /* 10 FLEXCOM 1 (USART1/SPI1/TWI1) */
        .pfnFLEXCOM2_Handler      = (void*) FLEXCOM2_Handler,       /* 11 FLEXCOM 2 (USART2/SPI2/TWI2) */
        .pfnFLEXCOM3_Handler      = (void*) FLEXCOM3_Handler,       /* 12 FLEXCOM 3 (USART3/SPI3/TWI3) */
        .pfnFLEXCOM4_Handler      = (void*) FLEXCOM4_Handler,       /* 13 FLEXCOM 4 (USART4/SPI4/TWI4) */
        .pfnFLEXCOM5_Handler      = (void*) FLEXCOM5_Handler,       /* 14 FLEXCOM 5 (USART5/SPI5/TWI5) */
        .pfnFLEXCOM6_Handler      = (void*) FLEXCOM6_Handler,       /* 15 FLEXCOM 6 (USART6/SPI6/TWI6) */
        .pfnFLEXCOM7_Handler      = (void*) FLEXCOM7_Handler,       /* 16 FLEXCOM 7 (USART7/SPI7/TWI7) */
        .pfnPIOA_Handler          = (void*) PIOA_Handler,           /* 17 Application Core Parallel I/O Controller A (PIOA) */
        .pfnPIOA_SEC_Handler      = (void*) PIOA_SEC_Handler,       /* 18 Application Core Parallel I/O Controller A (PIOA) Secure Event Interrupt */
        .pfnPIOB_Handler          = (void*) PIOB_Handler,           /* 19 Application Core Parallel I/O Controller (B PIOB) */
        .pfnPIOB_SEC_Handler      = (void*) PIOB_SEC_Handler,       /* 20 Application Core Parallel I/O Controller (B PIOB) Secure Event Interrupt */
        .pfnPIOC_Handler          = (void*) PIOC_Handler,           /* 21 Application Core Parallel I/O Controller C (PIOC) */
        .pfnPIOC_SEC_Handler      = (void*) PIOC_SEC_Handler,       /* 22 Application Core Parallel I/O Controller C (PIOC) Secure Event Interrupt */
        .pfnQSPI_Handler          = (void*) QSPI_Handler,           /* 23 Quad IO Serial Peripheral Interface */
        .pfnADC_Handler           = (void*) ADC_Handler,            /* 24 Analog To Digital Converter */
        .pfnACC_Handler           = (void*) ACC_Handler,            /* 25 Analog Comparator */
        .pfnARM0_FPU_Handler      = (void*) ARM0_FPU_Handler,       /* 26 Application Core (CORE0) Floating Point Unit except IXC */
        .pfnARM0_IXC_Handler      = (void*) ARM0_IXC_Handler,       /* 27 Application Core (CORE0) FPU Interrupt IXC associated with FPU cumulative exception bit */
        .pfnIPC0_Handler          = (void*) IPC0_Handler,           /* 28 Application Core Interprocessor communication (IPC0) */
        .pfnSLCDC_Handler         = (void*) SLCDC_Handler,          /* 29 Segment LCD Controller */
        .pfnMEM2MEM0_Handler      = (void*) MEM2MEM0_Handler,       /* 30 Application Core Memory to Memory Transfer Controller (MEM2MEM0) */
        .pfnTC0_CHANNEL0_Handler  = (void*) TC0_CHANNEL0_Handler,   /* 31 Timer Counter 0, Channel 0 */
        .pfnTC0_CHANNEL1_Handler  = (void*) TC0_CHANNEL1_Handler,   /* 32 Timer Counter 0, Channel 1 */
        .pfnTC0_CHANNEL2_Handler  = (void*) TC0_CHANNEL2_Handler,   /* 33 Timer Counter 0, Channel 2 */
        .pfnTC1_CHANNEL0_Handler  = (void*) TC1_CHANNEL0_Handler,   /* 34 Timer Counter 1, Channel 0 */
        .pfnTC1_CHANNEL1_Handler  = (void*) TC1_CHANNEL1_Handler,   /* 35 Timer Counter 1, Channel 1 */
        .pfnTC1_CHANNEL2_Handler  = (void*) TC1_CHANNEL2_Handler,   /* 36 Timer Counter 1, Channel 2 */
        .pfnTC2_CHANNEL0_Handler  = (void*) TC2_CHANNEL0_Handler,   /* 37 Timer Counter 2, Channel 0 */
        .pfnTC2_CHANNEL1_Handler  = (void*) TC2_CHANNEL1_Handler,   /* 38 Timer Counter 2, Channel 1 */
        .pfnTC2_CHANNEL2_Handler  = (void*) TC2_CHANNEL2_Handler,   /* 39 Timer Counter 2, Channel 2 */
        .pfnTC0_C0SEC_Handler     = (void*) TC0_C0SEC_Handler,      /* 40 Timer Counter 0, Channel 0, Secure IRQ */
        .pfnTC0_C1SEC_Handler     = (void*) TC0_C1SEC_Handler,      /* 41 Timer Counter 0, Channel 1, Secure IRQ */
        .pfnTC0_C2SEC_Handler     = (void*) TC0_C2SEC_Handler,      /* 42 Timer Counter 0, Channel 2, Secure IRQ */
        .pfnTC1_C0SEC_Handler     = (void*) TC1_C0SEC_Handler,      /* 43 Timer Counter 1, Channel 0, Secure IRQ */
        .pfnTC1_C1SEC_Handler     = (void*) TC1_C1SEC_Handler,      /* 44 Timer Counter 1, Channel 1, Secure IRQ */
        .pfnTC1_C2SEC_Handler     = (void*) TC1_C2SEC_Handler,      /* 45 Timer Counter 1, Channel 2, Secure IRQ */
        .pfnTC2_C0SEC_Handler     = (void*) TC2_C0SEC_Handler,      /* 46 Timer Counter 2, Channel 0, Secure IRQ */
        .pfnTC2_C1SEC_Handler     = (void*) TC2_C1SEC_Handler,      /* 47 Timer Counter 2, Channel 1, Secure IRQ */
        .pfnTC2_C2SEC_Handler     = (void*) TC2_C2SEC_Handler,      /* 48 Timer Counter 2, Channel 2, Secure IRQ */
        .pfnAES_Handler           = (void*) AES_Handler,            /* 49 Advanced Enhanced Standard */
        .pfnAES_AESSEC_Handler    = (void*) AES_AESSEC_Handler,     /* 50 AES Secure Event Interrupt */
        .pfnAESB_Handler          = (void*) AESB_Handler,           /* 51 AES Bridge */
        .pfnAESB_AESBSEC_Handler  = (void*) AESB_AESBSEC_Handler,   /* 52 AES Bridge Secure Interrupt */
        .pfnSHA_Handler           = (void*) SHA_Handler,            /* 53 Secure Hash Algorithm */
        .pfnSHA_SHASEC_Handler    = (void*) SHA_SHASEC_Handler,     /* 54 SHA Secure Event Interrupt */
        .pfnTRNG_Handler          = (void*) TRNG_Handler,           /* 55 True Random Generator */
        .pfnTRNG_TRNGSEC_Handler  = (void*) TRNG_TRNGSEC_Handler,   /* 56 TRNG Secure Event Interrupt */
        .pfnICM_Handler           = (void*) ICM_Handler,            /* 57 Integrity Check Module */
        .pfnICM_ICMSEC_Handler    = (void*) ICM_ICMSEC_Handler,     /* 58 Integrity Check Module */
        .pfnCPKCC_Handler         = (void*) CPKCC_Handler,          /* 59 Public Key Cryptography Controller */
        .pfnMATRIX0_Handler       = (void*) MATRIX0_Handler,        /* 60 High-Speed Application Core Matrix (HSACM) (MATRIX0) */
        .pfnMATRIX1_Handler       = (void*) MATRIX1_Handler,        /* 61 Low-Speed Application Core Matrix (LSACM) (MATRIX1) */
        .pfnSUPC_WKUP3_Handler    = (void*) SUPC_WKUP3_Handler ,    /* 62 External interrupt 3 */
        .pfnSUPC_WKUP4_Handler    = (void*) SUPC_WKUP4_Handler ,    /* 63 External interrupt 4 */
        .pfnSUPC_WKUP5_Handler    = (void*) SUPC_WKUP5_Handler ,    /* 64 External interrupt 5 */
        .pfnSUPC_WKUP6_Handler    = (void*) SUPC_WKUP6_Handler ,    /* 65 External interrupt 6 */
        .pfnSUPC_WKUP7_Handler    = (void*) SUPC_WKUP7_Handler ,    /* 66 External interrupt 7 */
        .pfnSUPC_WKUP8_Handler    = (void*) SUPC_WKUP8_Handler ,    /* 67 External interrupt 8 */
        .pfnSUPC_WKUP9_Handler    = (void*) SUPC_WKUP9_Handler ,    /* 68 External interrupt 9 */
        .pfnSUPC_WKUP10_Handler   = (void*) SUPC_WKUP10_Handler,    /* 69 External interrupt 10 */
        .pfnSUPC_WKUP11_Handler   = (void*) SUPC_WKUP11_Handler,    /* 70 External interrupt 11 */
        .pfnSUPC_WKUP12_Handler   = (void*) SUPC_WKUP12_Handler,    /* 71 External interrupt 12 */
        .pfnSUPC_WKUP13_Handler   = (void*) SUPC_WKUP13_Handler,    /* 72 External interrupt 13 */
        .pfnSUPC_WKUP14_Handler   = (void*) SUPC_WKUP14_Handler,    /* 73 External interrupt 14 */
        .pfnSUPC_WKUP15_Handler   = (void*) SUPC_WKUP15_Handler,    /* 74 External interrupt 15 */
        .pvReserved75             = (void*) (0UL),                  /* 75 Reserved */
        .pfnSEAFEC_SLINK_Handler  = (void*) SEAFEC_SLINK_Handler,   /* 76 Smart Energy Analog Front-End Controller Serial Link Interrupt */
        .pfnSEAFEC_DATA_Handler   = (void*) SEAFEC_DATA_Handler,    /* 77 Smart Energy Analog Front-End Controller Data Ready Interrupt */
        .pfnMEM2MEM1_Handler      = (void*) MEM2MEM1_Handler,       /* 78 Metrology Core Memory to Memory Transfer Controller 1 (MEM2MEM1) */
        .pfnTC3_CHANNEL0_Handler  = (void*) TC3_CHANNEL0_Handler,   /* 79 Timer Counter 3, Channel 0 */
        .pfnTC3_CHANNEL1_Handler  = (void*) TC3_CHANNEL1_Handler,   /* 80 Timer Counter 3, Channel 1 */
        .pfnTC3_CHANNEL2_Handler  = (void*) TC3_CHANNEL2_Handler,   /* 81 Timer Counter 3, Channel 2 */
        .pfnTC3_C0SEC_Handler     = (void*) TC3_C0SEC_Handler,      /* 82 Timer Counter 3, Channel 0, Secure IRQ */
        .pfnTC3_C1SEC_Handler     = (void*) TC3_C1SEC_Handler,      /* 83 Timer Counter 3, Channel 1, Secure IRQ */
        .pfnTC3_C2SEC_Handler     = (void*) TC3_C2SEC_Handler,      /* 84 Timer Counter 3, Channel 2, Secure IRQ */
        .pfnPIOD_Handler          = (void*) PIOD_Handler,           /* 85 Metrology Core Parallel I/O Controller D (PIOD) */
        .pfnPIOD_SEC_Handler      = (void*) PIOD_SEC_Handler,       /* 86 Metrology Core Parallel I/O Controller A Secure Interupt (named as PIOD) */
        .pfnUART_Handler          = (void*) UART_Handler,           /* 87 Optical UART */
        .pfnIPC1_Handler          = (void*) IPC1_Handler,           /* 88 Metrology Core Interprocessor communication (IPC1) */
        .pfnMCSPI_Handler         = (void*) MCSPI_Handler,          /* 89 Multi-Channel SPI */
        .pfnPWM_Handler           = (void*) PWM_Handler,            /* 90 Pulse Width Modulation */
        .pvReserved91             = (void*) (0UL),                  /* 91 Reserved */
        .pfnARM1_FPU_Handler      = (void*) ARM1_FPU_Handler,       /* 92 Metrology Core (CORE1) Floating Point Unit except IXC */
        .pfnARM1_IXC_Handler      = (void*) ARM1_IXC_Handler,       /* 93 Metrology Core (CORE1) FPU Interrupt IXC associated with FPU cumulative exception bit */
        .pfnMATRIX2_Handler       = (void*) MATRIX2_Handler,        /* 94 High-Speed Metrology Core Matrix (HSMCM) (Formely MATRIX2) */
        .pfnMATRIX3_Handler       = (void*) MATRIX3_Handler,        /* 95 Low-Speed Metrology Core Matrix (LSMCM) (Formely MATRIX3) */
        .pvReserved96             = (void*) (0UL),                  /* 96 Reserved */
};

#pragma weak __tcm_initialization=tcm_empty_init

/**
 * \brief Empty method to allow TCM initialization from application´s code.
 * Override the _tcm_low_level_init function if TCM setup is needed.
 */
static int tcm_empty_init(void) 
{
        return 0;
}

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
        uint32_t *pSrc = __section_begin(".intvec");

        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        __tcm_initialization();

        return 1; /* if return 0, the data sections will not be initialized */
}

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
void Reset_Handler(void)
{
        __iar_program_start();
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}
