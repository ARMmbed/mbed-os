#include "samd21.h"

void __iar_program_start(void);
int __low_level_init(void);

void Dummy_Handler(void);
void Reset_Handler(void);

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
    while (1) {
    }
}

/* Cortex-M0+ core handlers */
void NMI_Handler              ( void );
void HardFault_Handler        ( void );
void SVC_Handler              ( void );
void PendSV_Handler           ( void );
void SysTick_Handler          ( void );

/* Peripherals handlers */
void PM_Handler               ( void );
void SYSCTRL_Handler          ( void );
void WDT_Handler              ( void );
void RTC_Handler              ( void );
void EIC_Handler              ( void );
void NVMCTRL_Handler          ( void );
void DMAC_Handler             ( void );
void USB_Handler              ( void );
void EVSYS_Handler            ( void );
void SERCOM0_Handler          ( void );
void SERCOM1_Handler          ( void );
void SERCOM2_Handler          ( void );
void SERCOM3_Handler          ( void );
void SERCOM4_Handler          ( void );
void SERCOM5_Handler          ( void );
void TCC0_Handler             ( void );
void TCC1_Handler             ( void );
void TCC2_Handler             ( void );
void TC3_Handler              ( void );
void TC4_Handler              ( void );
void TC5_Handler              ( void );
void TC6_Handler              ( void );
void TC7_Handler              ( void );
void ADC_Handler              ( void );
void AC_Handler               ( void );
void DAC_Handler              ( void );
void PTC_Handler              ( void );
void I2S_Handler              ( void );

/* Cortex-M0+ core handlers */
#pragma weak NMI_Handler              = Dummy_Handler
#pragma weak HardFault_Handler        = Dummy_Handler
#pragma weak SVC_Handler              = Dummy_Handler
#pragma weak PendSV_Handler           = Dummy_Handler
#pragma weak SysTick_Handler          = Dummy_Handler

/* Peripherals handlers */
#pragma weak PM_Handler               = Dummy_Handler
#pragma weak SYSCTRL_Handler          = Dummy_Handler
#pragma weak WDT_Handler              = Dummy_Handler
#pragma weak RTC_Handler              = Dummy_Handler
#pragma weak EIC_Handler              = Dummy_Handler
#pragma weak NVMCTRL_Handler          = Dummy_Handler
#pragma weak DMAC_Handler             = Dummy_Handler
#pragma weak USB_Handler              = Dummy_Handler
#pragma weak EVSYS_Handler            = Dummy_Handler
#pragma weak SERCOM0_Handler          = Dummy_Handler
#pragma weak SERCOM1_Handler          = Dummy_Handler
#pragma weak SERCOM2_Handler          = Dummy_Handler
#pragma weak SERCOM3_Handler          = Dummy_Handler
#pragma weak SERCOM4_Handler          = Dummy_Handler
#pragma weak SERCOM5_Handler          = Dummy_Handler
#pragma weak TCC0_Handler             = Dummy_Handler
#pragma weak TCC1_Handler             = Dummy_Handler
#pragma weak TCC2_Handler             = Dummy_Handler
#pragma weak TC3_Handler              = Dummy_Handler
#pragma weak TC4_Handler              = Dummy_Handler
#pragma weak TC5_Handler              = Dummy_Handler
#pragma weak TC6_Handler              = Dummy_Handler
#pragma weak TC7_Handler              = Dummy_Handler
#pragma weak ADC_Handler              = Dummy_Handler
#pragma weak AC_Handler               = Dummy_Handler
#pragma weak DAC_Handler              = Dummy_Handler
#pragma weak PTC_Handler              = Dummy_Handler
#pragma weak I2S_Handler              = Dummy_Handler

/* Exception Table */
#pragma language=extended
#pragma segment="CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
//! [startup_vector_table]
const DeviceVectors __vector_table[] = {
    __sfe("CSTACK"),
    (void*) __iar_program_start,
    (void*) NMI_Handler,
    (void*) HardFault_Handler,
    (void*) (0UL), /* Reserved */
    (void*) (0UL), /* Reserved */
    (void*) (0UL), /* Reserved */
    (void*) (0UL), /* Reserved */
    (void*) (0UL), /* Reserved */
    (void*) (0UL), /* Reserved */
    (void*) (0UL), /* Reserved */
    (void*) SVC_Handler,
    (void*) (0UL), /* Reserved */
    (void*) (0UL), /* Reserved */
    (void*) PendSV_Handler,
    (void*) SysTick_Handler,

    /* Configurable interrupts */
    (void*) PM_Handler,             /*  0 Power Manager */
    (void*) SYSCTRL_Handler,        /*  1 System Control */
    (void*) WDT_Handler,            /*  2 Watchdog Timer */
    (void*) RTC_Handler,            /*  3 Real-Time Counter */
    (void*) EIC_Handler,            /*  4 External Interrupt Controller */
    (void*) NVMCTRL_Handler,        /*  5 Non-Volatile Memory Controller */
    (void*) DMAC_Handler,           /*  6 Direct Memory Access Controller */
    (void*) USB_Handler,            /*  7 Universal Serial Bus */
    (void*) EVSYS_Handler,          /*  8 Event System Interface */
    (void*) SERCOM0_Handler,        /*  9 Serial Communication Interface 0 */
    (void*) SERCOM1_Handler,        /* 10 Serial Communication Interface 1 */
    (void*) SERCOM2_Handler,        /* 11 Serial Communication Interface 2 */
    (void*) SERCOM3_Handler,        /* 12 Serial Communication Interface 3 */
    (void*) SERCOM4_Handler,        /* 13 Serial Communication Interface 4 */
    (void*) SERCOM5_Handler,        /* 14 Serial Communication Interface 5 */
    (void*) TCC0_Handler,           /* 15 Timer Counter Control 0 */
    (void*) TCC1_Handler,           /* 16 Timer Counter Control 1 */
    (void*) TCC2_Handler,           /* 17 Timer Counter Control 2 */
    (void*) TC3_Handler,            /* 18 Basic Timer Counter 0 */
    (void*) TC4_Handler,            /* 19 Basic Timer Counter 1 */
    (void*) TC5_Handler,            /* 20 Basic Timer Counter 2 */
    (void*) TC6_Handler,            /* 21 Basic Timer Counter 3 */
    (void*) TC7_Handler,            /* 22 Basic Timer Counter 4 */
    (void*) ADC_Handler,            /* 23 Analog Digital Converter */
    (void*) AC_Handler,             /* 24 Analog Comparators */
    (void*) DAC_Handler,            /* 25 Digital Analog Converter */
    (void*) PTC_Handler,            /* 26 Peripheral Touch Controller */
    (void*) I2S_Handler             /* 27 Inter-IC Sound Interface */
};
//! [startup_vector_table]

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
    uint32_t *pSrc = __section_begin(".intvec");

    SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

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
