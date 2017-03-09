#include "samr21.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M0+ core handlers */
void NMI_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void PM_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SYSCTRL_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NVMCTRL_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DMAC_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef USB_IRQn
void USB_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
void EVSYS_Handler           ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM0_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM1_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM2_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SERCOM3_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef SERCOM4_IRQn
void SERCOM4_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
#ifdef SERCOM5_IRQn
void SERCOM5_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
void TCC0_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC1_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TCC2_Handler            ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC3_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC4_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC5_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef TC6_IRQn
void TC6_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
#ifdef TC7_IRQn
void TC7_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
#ifdef ADC_IRQn
void ADC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
#ifdef AC_IRQn
void AC_Handler              ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
#ifdef DAC_IRQn
void DAC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
#ifdef PTC_IRQn
void PTC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif
void I2S_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

    /* Configure Initial Stack Pointer, using linker-generated symbols */
    (void*) (&_estack),

    (void*) Reset_Handler,
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
#ifdef USB_IRQn
    (void*) USB_Handler,            /*  7 Universal Serial Bus */
#else
    (void*) (0UL), /* Reserved */
#endif
    (void*) EVSYS_Handler,          /*  8 Event System Interface */
    (void*) SERCOM0_Handler,        /*  9 Serial Communication Interface 0 */
    (void*) SERCOM1_Handler,        /* 10 Serial Communication Interface 1 */
    (void*) SERCOM2_Handler,        /* 11 Serial Communication Interface 2 */
    (void*) SERCOM3_Handler,        /* 12 Serial Communication Interface 3 */
#ifdef SERCOM4_IRQn
    (void*) SERCOM4_Handler,        /* 13 Serial Communication Interface 4 */
#else
    (void*) (0UL), /* Reserved */
#endif
#ifdef SERCOM5_IRQn
    (void*) SERCOM5_Handler,        /* 14 Serial Communication Interface 5 */
#else
    (void*) (0UL), /* Reserved */
#endif
    (void*) TCC0_Handler,           /* 15 Timer Counter Control 0 */
    (void*) TCC1_Handler,           /* 16 Timer Counter Control 1 */
    (void*) TCC2_Handler,           /* 17 Timer Counter Control 2 */
    (void*) TC3_Handler,            /* 18 Basic Timer Counter 0 */
    (void*) TC4_Handler,            /* 19 Basic Timer Counter 1 */
    (void*) TC5_Handler,            /* 20 Basic Timer Counter 2 */
#ifdef TC6_IRQn
    (void*) TC6_Handler,            /* 21 Basic Timer Counter 3 */
#else
    (void*) (0UL), /* Reserved */
#endif
#ifdef TC7_IRQn
    (void*) TC7_Handler,            /* 22 Basic Timer Counter 4 */
#else
    (void*) (0UL), /* Reserved */
#endif
#ifdef ADC_IRQn
    (void*) ADC_Handler,            /* 23 Analog Digital Converter */
#else
    (void*) (0UL), /* Reserved */
#endif
#ifdef AC_IRQn
    (void*) AC_Handler,             /* 24 Analog Comparators */
#else
    (void*) (0UL), /* Reserved */
#endif
#ifdef DAC_IRQn
    (void*) DAC_Handler,            /* 25 Digital Analog Converter */
#else
    (void*) (0UL), /* Reserved */
#endif
#ifdef PTC_IRQn
    (void*) PTC_Handler,            /* 26 Peripheral Touch Controller */
#else
    (void*) (0UL), /* Reserved */
#endif
    (void*) I2S_Handler             /* 27 Inter-IC Sound Interface */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;

    /* Initialize the relocate segment */
    pSrc = &_etext;
    pDest = &_srelocate;

    if (pSrc != pDest) {
        for (; pDest < &_erelocate;) {
            *pDest++ = *pSrc++;
        }
    }

    /* Clear the zero segment */
    for (pDest = &_szero; pDest < &_ezero;) {
        *pDest++ = 0;
    }

    /* Set the vector table base address */
    pSrc = (uint32_t *) & _sfixed;
    SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

    /* Initialize the C library */
    __libc_init_array();

    /* Branch to main function */
    main();

    /* Infinite loop */
    while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
    while (1) {
    }
}
