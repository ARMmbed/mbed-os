/**************************************************************************//**
 * @file     system_NUC472_442.c
 * @version  V1.00
 * $Revision: 15 $
 * $Date: 14/05/29 1:13p $
 * @brief    NUC472/NUC442 system clock init code and assert handler
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "NUC472_442.h"
//#include "rtc.h"

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __HSI;               /*!< System Clock Frequency (Core Clock)*/
uint32_t CyclesPerUs      = (__HSI / 1000000);  /*!< Cycles per micro second            */
uint32_t gau32ClkSrcTbl[] = {__HXT, __LXT, 0, __LIRC, 0, 0, 0, __HIRC}; /*!< System clock source table */

#if defined TARGET_NU_XRAM_SUPPORTED
static void nu_ebi_init(void);
#endif

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
    uint32_t u32Freq, u32ClkSrc;
    uint32_t u32HclkDiv;

    u32ClkSrc = CLK->CLKSEL0 & CLK_CLKSEL0_HCLKSEL_Msk;

    if(u32ClkSrc != CLK_CLKSEL0_HCLKSEL_PLL) {
        /* Use the clock sources directly */
        u32Freq = gau32ClkSrcTbl[u32ClkSrc];
    } else {
        /* Use PLL clock */
        u32Freq = CLK_GetPLLClockFreq();
    }

    u32HclkDiv = (CLK->CLKDIV0 & CLK_CLKDIV0_HCLKDIV_Msk) + 1;

    /* Update System Core Clock */
    SystemCoreClock = u32Freq/u32HclkDiv;

    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;
}

/**
 * Initialize the system
 *
 * @return none
 *
 * @brief  Setup the microcontroller system.
 */
void SystemInit (void)
{
    //uint32_t u32RTC_EN_Flag = 0;

    /* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2)  );               /* set CP11 Full Access */
#endif

    /* The code snippet below is for old-version chip and has potential risk, e.g. program reboots and hangs in it with the call to NVIC_SystemReset(). Remove it for new-version chip. */
#if 0
    /* ------------------ Release Tamper pin ---------------------------------*/
    /* Waiting for 10kHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);

    u32RTC_EN_Flag = ((CLK->APBCLK0 & CLK_APBCLK0_RTCCKEN_Msk) >> CLK_APBCLK0_RTCCKEN_Pos);

    if(!u32RTC_EN_Flag) {
        CLK->APBCLK0 |= CLK_APBCLK0_RTCCKEN_Msk; // RTC Clock Enable
    }

    RTC->INIT = RTC_INIT_KEY;
    while(RTC->INIT != 0x1);

    if(!(RTC->TAMPCTL & RTC_TAMPCTL_TIEN_Msk)) {
        RTC->RWEN = RTC_WRITE_KEY;
        while(!(RTC->RWEN & RTC_RWEN_RWENF_Msk));

        RTC->SPRCTL |= RTC_SPRCTL_SPRRWEN_Msk;

        while(!(RTC->SPRCTL & RTC_SPRCTL_SPRRWRDY_Msk));

        RTC->RWEN = RTC_WRITE_KEY;
        while(!(RTC->RWEN & RTC_RWEN_RWENF_Msk));

        RTC->SPR[23] = RTC->SPR[23];
        while(!(RTC->SPRCTL & RTC_SPRCTL_SPRRWRDY_Msk));

        RTC->RWEN = RTC_WRITE_KEY;
        while(!(RTC->RWEN & RTC_RWEN_RWENF_Msk));

        RTC->SPRCTL &= ~RTC_SPRCTL_SPRRWEN_Msk;
        while(!(RTC->SPRCTL & RTC_SPRCTL_SPRRWRDY_Msk));

        RTC->RWEN = RTC_WRITE_KEY;
        while(!(RTC->RWEN & RTC_RWEN_RWENF_Msk));

        RTC->INTSTS = RTC_INTSTS_TICKIF_Msk;
    }

    if(!u32RTC_EN_Flag) {
        CLK->APBCLK0 &= ~CLK_APBCLK0_RTCCKEN_Msk; // RTC Clock Disable
    }
    /*------------------------------------------------------------------------*/
#endif

#if defined TARGET_NU_XRAM_SUPPORTED
    // NOTE: C-runtime not initialized yet. Ensure no static memory (global variable) are accessed in this function.
    nu_ebi_init();
#endif
}

#if defined TARGET_NU_XRAM_SUPPORTED
void nu_ebi_init(void)
{
    /* Enable IP clock */
    CLK_EnableModuleClock(EBI_MODULE);
    
    /* Configure EBI multi-function pins */
    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA10MFP_Msk) ) | SYS_GPA_MFPH_PA10MFP_EBI_A20;    /* A20. =   PA10 */
    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA9MFP_Msk) ) | SYS_GPA_MFPH_PA9MFP_EBI_A19;      /* A19. =   PA9 */
    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA8MFP_Msk) ) | SYS_GPA_MFPH_PA8MFP_EBI_A18;      /* A18. =   PA8 */
    SYS->GPA_MFPL = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA7MFP_Msk) ) | SYS_GPA_MFPL_PA7MFP_EBI_A17;      /* A17. =   PA7 */
    SYS->GPA_MFPL = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA6MFP_Msk) ) | SYS_GPA_MFPL_PA6MFP_EBI_A16;      /* A16. =   PA6 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB13MFP_Msk) ) | SYS_GPB_MFPH_PB13MFP_EBI_AD15;   /* AD15 =   PB13 */

    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB12MFP_Msk) ) | SYS_GPB_MFPH_PB12MFP_EBI_AD14;   /* AD14 =   PB12 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB11MFP_Msk) ) | SYS_GPB_MFPH_PB11MFP_EBI_AD13;   /* AD13 =   PB11 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB10MFP_Msk) ) | SYS_GPB_MFPH_PB10MFP_EBI_AD12;   /* AD12 =   PB10 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB9MFP_Msk) ) | SYS_GPB_MFPH_PB9MFP_EBI_AD11;     /* AD11 =   PB9 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB8MFP_Msk) ) | SYS_GPB_MFPH_PB8MFP_EBI_AD10;     /* AD10 =   PB8 */

    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB7MFP_Msk) ) | SYS_GPB_MFPL_PB7MFP_EBI_AD9;      /* AD9 =    PB7 */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB6MFP_Msk) ) | SYS_GPB_MFPL_PB6MFP_EBI_AD8;      /* AD8 =    PB6 */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB5MFP_Msk) ) | SYS_GPB_MFPL_PB5MFP_EBI_AD7;      /* AD7 =    PB5 */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB4MFP_Msk) ) | SYS_GPB_MFPL_PB4MFP_EBI_AD6;      /* AD6 =    PB4 */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB3MFP_Msk) ) | SYS_GPB_MFPL_PB3MFP_EBI_AD5;      /* AD5 =    PB3 */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & (~SYS_GPB_MFPL_PB2MFP_Msk) ) | SYS_GPB_MFPL_PB2MFP_EBI_AD4;      /* AD4 =    PB2 */

    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA14MFP_Msk) ) | SYS_GPA_MFPH_PA14MFP_EBI_AD3;    /* AD3. =   PA14 */
    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA13MFP_Msk) ) | SYS_GPA_MFPH_PA13MFP_EBI_AD2;    /* AD2. =   PA13 */
    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA12MFP_Msk) ) | SYS_GPA_MFPH_PA12MFP_EBI_AD1;    /* AD1. =   PA12 */
    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA11MFP_Msk) ) | SYS_GPA_MFPH_PA11MFP_EBI_AD0;    /* AD0. =   PA11 */

    SYS->GPE_MFPL = (SYS->GPE_MFPL & (~SYS_GPE_MFPL_PE6MFP_Msk) ) | SYS_GPE_MFPL_PE6MFP_EBI_nWR;      /* PE.6 =   nWR */
    SYS->GPE_MFPL = (SYS->GPE_MFPL & (~SYS_GPE_MFPL_PE7MFP_Msk) ) | SYS_GPE_MFPL_PE7MFP_EBI_nRD;      /* PE.7 =   nRD */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & (~SYS_GPE_MFPH_PE8MFP_Msk) ) | SYS_GPE_MFPH_PE8MFP_EBI_ALE;      /* PE.8 =   ALE */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & (~SYS_GPE_MFPH_PE9MFP_Msk) ) | SYS_GPE_MFPH_PE9MFP_EBI_nWRH;     /* PE.9 =   WRH */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & (~SYS_GPE_MFPH_PE10MFP_Msk) ) | SYS_GPE_MFPH_PE10MFP_EBI_nWRL;   /* PE.10 =  WRL */

    SYS->GPE_MFPH = (SYS->GPE_MFPH & (~SYS_GPE_MFPH_PE11MFP_Msk) ) | SYS_GPE_MFPH_PE11MFP_EBI_nCS0;   /* PE.11 = nCS0 */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & (~SYS_GPE_MFPH_PE12MFP_Msk) ) | SYS_GPE_MFPH_PE12MFP_EBI_nCS1;   /* PE.12 = nCS1 */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & (~SYS_GPE_MFPH_PE13MFP_Msk) ) | SYS_GPE_MFPH_PE13MFP_EBI_nCS2;   /* PE.13 = nCS2 */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & (~SYS_GPE_MFPH_PE14MFP_Msk) ) | SYS_GPE_MFPH_PE14MFP_EBI_nCS3;   /* PE.14 = nCS3 */
    
    const uint32_t u32Timing = 0x21C;

    /* Open EBI interface */
    EBI_Open(EBI_BANK0, EBI_BUSWIDTH_16BIT, EBI_TIMING_FAST, EBI_SEPARATEMODE_DISABLE, EBI_CS_ACTIVE_LOW);
    EBI_Open(EBI_BANK1, EBI_BUSWIDTH_16BIT, EBI_TIMING_FAST, EBI_SEPARATEMODE_DISABLE, EBI_CS_ACTIVE_LOW);
    EBI_Open(EBI_BANK2, EBI_BUSWIDTH_16BIT, EBI_TIMING_FAST, EBI_SEPARATEMODE_DISABLE, EBI_CS_ACTIVE_LOW);
    EBI_Open(EBI_BANK3, EBI_BUSWIDTH_16BIT, EBI_TIMING_FAST, EBI_SEPARATEMODE_DISABLE, EBI_CS_ACTIVE_LOW);

    /* Configure EBI timing */
    EBI_SetBusTiming(EBI_BANK0, u32Timing, EBI_MCLKDIV_2);
    EBI_SetBusTiming(EBI_BANK1, u32Timing, EBI_MCLKDIV_2);
    EBI_SetBusTiming(EBI_BANK2, u32Timing, EBI_MCLKDIV_2);
    EBI_SetBusTiming(EBI_BANK3, u32Timing, EBI_MCLKDIV_2);
}
#endif
/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
