/*********************************************************************************************************************
 * @file     system_XMC4500.c
 * @brief    CMSIS Cortex-M4 Device Peripheral Access Layer Header File for the Infineon XMC4500 Device Series
 * @version  V3.1.2
 * @date     09. Feb 2017
 *
 * @cond
 *********************************************************************************************************************
 * Copyright (c) 2014-2017, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with
 * Infineon Technologies AG dave@infineon.com).
 *********************************************************************************************************************
 *
 ********************** Version History ***************************************
 * V3.1.0, Dec 2014, Added options to configure clock settings
 * V3.1.1, 01. Jun 2016, Fix masking of OSCHPCTRL value 
 * V3.1.2, 09. Feb 2017, Fix activation of USBPLL when SDMMC clock is enabled
 ******************************************************************************
 * @endcond
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
#include <string.h>

#include <XMC4500.h>
#include "system_XMC4500.h"

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define CHIPID_LOC ((uint8_t *)0x20000000UL)

/* Define WEAK attribute */
#if !defined(__WEAK)
#if defined ( __CC_ARM )
#define __WEAK __attribute__ ((weak))
#elif defined ( __ICCARM__ )
#define __WEAK __weak
#elif defined ( __GNUC__ )
#define __WEAK __attribute__ ((weak))
#elif defined ( __TASKING__ )
#define __WEAK __attribute__ ((weak))
#endif
#endif

#define PMU_FLASH_WS          (0x3U)

#define FPLL_FREQUENCY        (120000000U)
#define FOSCREF               (2500000U)
#define DELAY_CNT_50US_50MHZ  (2500UL)
#define DELAY_CNT_150US_50MHZ (7500UL)
#define DELAY_CNT_50US_60MHZ  (3000UL)
#define DELAY_CNT_50US_90MHZ  (4500UL)
#define DELAY_CNT_50US_120MHZ (6000UL)

#define SCU_PLL_PLLSTAT_OSC_USABLE  (SCU_PLL_PLLSTAT_PLLHV_Msk | \
                                     SCU_PLL_PLLSTAT_PLLLV_Msk | \
                                     SCU_PLL_PLLSTAT_PLLSP_Msk)

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*
// <h> Clock configuration
*/

/*
//    <o> External crystal frequency [Hz]
//       <8000000=> 8MHz
//       <12000000=> 12MHz
//       <16000000=> 16MHz
//    <i> Defines external crystal frequency
//    <i> Default: 8MHz
*/
#define OSCHP_FREQUENCY (12000000U)

#if OSCHP_FREQUENCY == 8000000U
#define USB_PDIV (1U)
#define USB_NDIV (95U)
#define USB_DIV (3U)

#elif OSCHP_FREQUENCY == 12000000U
#define USB_PDIV (1U)
#define USB_NDIV (63U)
#define USB_DIV (3U)

#elif OSCHP_FREQUENCY == 16000000U
#define USB_PDIV (1U)
#define USB_NDIV (47U)
#define USB_DIV (3U)

#else
#error "External crystal frequency not supported"

#endif

/*
//    <o> System clock (fSYS) source selection
//       <0=> Backup clock (24MHz)
//       <1=> Maximum clock frequency using PLL (120MHz)
//    <i> Default: Maximum clock frequency using PLL (120MHz)
*/
#define SYS_CLOCK_SRC 1
#define SYS_CLOCK_SRC_OFI 0
#define SYS_CLOCK_SRC_PLL 1

/*
//    <o> Backup clock calibration mode
//       <0=> Factory calibration
//       <1=> Automatic calibration
//    <i> Default: Automatic calibration
*/
#define FOFI_CALIBRATION_MODE 1
#define FOFI_CALIBRATION_MODE_FACTORY 0
#define FOFI_CALIBRATION_MODE_AUTOMATIC 1

/*
//    <o> Standby clock (fSTDBY) source selection
//       <0=> Internal slow oscillator (32768Hz)
//       <1=> External crystal (32768Hz)
//    <i> Default: Internal slow oscillator (32768Hz)
*/
#define STDBY_CLOCK_SRC 0
#define STDBY_CLOCK_SRC_OSI 0
#define STDBY_CLOCK_SRC_OSCULP 1

/*
//    <o> PLL clock source selection
//       <0=> External crystal
//       <1=> External direct input
//       <2=> Internal fast oscillator
//    <i> Default: External crystal
*/
#define PLL_CLOCK_SRC 0
#define PLL_CLOCK_SRC_EXT_XTAL 0
#define PLL_CLOCK_SRC_EXT_DIRECT 1
#define PLL_CLOCK_SRC_OFI 2

#if PLL_CLOCK_SRC == PLL_CLOCK_SRC_EXT_XTAL
#if OSCHP_FREQUENCY == 8000000U
#define PLL_PDIV (1U)
#define PLL_NDIV (89U)
#define PLL_K2DIV (2U)

#elif OSCHP_FREQUENCY == 12000000U
#define PLL_PDIV (1U)
#define PLL_NDIV (79U)
#define PLL_K2DIV (3U)

#elif OSCHP_FREQUENCY == 16000000U
#define PLL_PDIV (1U)
#define PLL_NDIV (59U)
#define PLL_K2DIV (3U)

#else
#error "External crystal frequency not supported"

#endif

#define VCO ((OSCHP_FREQUENCY / (PLL_PDIV + 1UL)) * (PLL_NDIV + 1UL))

#else /* PLL_CLOCK_SRC == PLL_CLOCK_SRC_EXT_XTAL */

#define PLL_PDIV (1U)
#define PLL_NDIV (39U)
#define PLL_K2DIV (3U)

#define VCO ((OFI_FREQUENCY / (PLL_PDIV + 1UL)) * (PLL_NDIV + 1UL))

#endif /* PLL_CLOCK_SRC == PLL_CLOCK_SRC_OFI */

#define PLL_K2DIV_0 ((VCO / OFI_FREQUENCY) - 1UL)
#define PLL_K2DIV_1 ((VCO / 60000000U) - 1UL)
#define PLL_K2DIV_2 ((VCO / 90000000U) - 1UL)

#define SCU_CLK_CLKCLR_ENABLE_USBCLK SCU_CLK_CLKCLR_USBCDI_Msk
#define SCU_CLK_CLKCLR_ENABLE_MMCCLK SCU_CLK_CLKCLR_MMCCDI_Msk
#define SCU_CLK_CLKCLR_ENABLE_ETHCLK SCU_CLK_CLKCLR_ETH0CDI_Msk
#define SCU_CLK_CLKCLR_ENABLE_EBUCLK SCU_CLK_CLKCLR_EBUCDI_Msk
#define SCU_CLK_CLKCLR_ENABLE_CCUCLK SCU_CLK_CLKCLR_CCUCDI_Msk
#define SCU_CLK_CLKCLR_ENABLE_WDTCLK SCU_CLK_CLKCLR_WDTCDI_Msk

#define SCU_CLK_USBCLKCR_USBSEL_USBPLL (0U << SCU_CLK_USBCLKCR_USBSEL_Pos)
#define SCU_CLK_USBCLKCR_USBSEL_PLL    (1U << SCU_CLK_USBCLKCR_USBSEL_Pos)

#define SCU_CLK_WDTCLKCR_WDTSEL_OFI      (0U << SCU_CLK_WDTCLKCR_WDTSEL_Pos)
#define SCU_CLK_WDTCLKCR_WDTSEL_STANDBY  (1U << SCU_CLK_WDTCLKCR_WDTSEL_Pos)
#define SCU_CLK_WDTCLKCR_WDTSEL_PLL      (2U << SCU_CLK_WDTCLKCR_WDTSEL_Pos)

#define SCU_CLK_EXTCLKCR_ECKSEL_SYS      (0U << SCU_CLK_EXTCLKCR_ECKSEL_Pos)
#define SCU_CLK_EXTCLKCR_ECKSEL_USBPLL   (2U << SCU_CLK_EXTCLKCR_ECKSEL_Pos)
#define SCU_CLK_EXTCLKCR_ECKSEL_PLL      (3U << SCU_CLK_EXTCLKCR_ECKSEL_Pos)

#define EXTCLK_PIN_P0_8  (0)
#define EXTCLK_PIN_P1_15 (1)

/*
//    <h> Clock tree
//        <o1.0> CPU clock divider
//                   <0=> fCPU = fSYS
//                   <1=> fCPU = fSYS / 2
//        <o2.0>  Peripheral clock divider
//                     <0=> fPB = fCPU
//                     <1=> fPB = fCPU / 2
//        <e.4> Enable CCU clock
//             <o3.0>  CCU clock divider
//                     <0=> fCCU = fCPU
//                     <1=> fCCU = fCPU / 2
//        </e>
//        <e.5> Enable WDT clock
//             <o4.0..7>  WDT clock divider <1-256><#-1>
//             <o4.16..17> WDT clock source <0=> fOFI
//                                          <1=> fSTDBY
//                                          <2=> fPLL
//        </e>
//        <e.3> Enable EBU clock
//             <o5.0..5>  EBU clock divider  <1-64><#-1>
//        </e>
//        <e.2> Enable ETH clock
//        </e>
//        <e.1> Enable MMC clock
//        </e>
//        <e.0> Enable USB clock
//             <o6.16> USB clock source <0=> USBPLL
//                                      <1=> PLL
//        </e>
//        <e7> External Clock configuration
//            <o8.0..1> External Clock Source Selection
//                  <0=> System clock
//                  <2=> USB PLL clock
//                  <3=> PLL clock
//            <o8.16..24> External Clock divider <1-512><#-1>
//            <i> Only valid for USB PLL and PLL clocks
//            <o9.0> External Clock Pin Selection
//                  <0=> P0.8
//                  <1=> P1.15
//        </e>
//    </h>
*/
#define ENABLE_SCUCLK (0U)
#define CPUCLKDIV (0U)
#define PBCLKDIV (0U)
#define CCUCLKDIV (0U)
#define WDTCLKDIV (0U | SCU_CLK_WDTCLKCR_WDTSEL_OFI)
#define EBUCLKDIV (0U)
#define USBCLKDIV (0U | SCU_CLK_USBCLKCR_USBSEL_USBPLL | USB_DIV)

#define ENABLE_EXTCLK (0U)
#define EXTCLKDIV (0U | SCU_CLK_EXTCLKCR_ECKSEL_SYS)
#define EXTCLK_PIN (0U)

#define ENABLE_PLL \
    (SYS_CLOCK_SRC == SYS_CLOCK_SRC_PLL) || \
    ((ENABLE_SCUCLK & SCU_CLK_CLKSET_EBUCEN_Msk) != 0) || \
    (((ENABLE_SCUCLK & SCU_CLK_CLKSET_USBCEN_Msk) != 0) && ((USBCLKDIV & SCU_CLK_USBCLKCR_USBSEL_Msk) == SCU_CLK_USBCLKCR_USBSEL_PLL)) || \
    (((ENABLE_SCUCLK & SCU_CLK_CLKSET_WDTCEN_Msk) != 0) && ((WDTCLKDIV & SCU_CLK_WDTCLKCR_WDTSEL_Msk) == SCU_CLK_WDTCLKCR_WDTSEL_PLL))

#define ENABLE_USBPLL \
    ((((ENABLE_SCUCLK & SCU_CLK_CLKSET_USBCEN_Msk) != 0) && ((USBCLKDIV & SCU_CLK_USBCLKCR_USBSEL_Msk) == SCU_CLK_USBCLKCR_USBSEL_USBPLL)) ||\
     (((ENABLE_SCUCLK & SCU_CLK_CLKCLR_ENABLE_MMCCLK) != 0) && ((USBCLKDIV & SCU_CLK_USBCLKCR_USBSEL_Msk) == SCU_CLK_USBCLKCR_USBSEL_USBPLL)))
/*
// </h>
*/

/*
//-------- <<< end of configuration section >>> ------------------
*/
                                     
/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/
#if defined ( __CC_ARM )
uint32_t SystemCoreClock __attribute__((at(0x2000FFC0)));
uint8_t g_chipid[16] __attribute__((at(0x2000FFC4)));
#elif defined ( __ICCARM__ )
__no_init uint32_t SystemCoreClock;
__no_init uint8_t g_chipid[16];
#elif defined ( __GNUC__ )
uint32_t SystemCoreClock __attribute__((section(".no_init")));
uint8_t g_chipid[16] __attribute__((section(".no_init")));
#elif defined ( __TASKING__ )
uint32_t SystemCoreClock __at( 0x2000FFC0 );
uint8_t g_chipid[16] __at( 0x2000FFC4 );
#endif

extern uint32_t __Vectors;

/*******************************************************************************
 * LOCAL FUNCTIONS
 *******************************************************************************/
static void delay(uint32_t cycles)
{
  volatile uint32_t i;

  for(i = 0UL; i < cycles ;++i)
  {
    __NOP();
  }
}

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

__WEAK void SystemInit(void)
{
  memcpy(g_chipid, CHIPID_LOC, 16);

  SystemCoreSetup();
  SystemCoreClockSetup();
}

__WEAK void SystemCoreSetup(void)
{
  uint32_t temp;

  /* relocate vector table */
  __disable_irq();
  SCB->VTOR = (uint32_t)(&__Vectors);
  __DSB();
  __enable_irq();
    
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                 (3UL << 11*2)  );               /* set CP11 Full Access */
#endif

  /* Enable unaligned memory access - SCB_CCR.UNALIGN_TRP = 0 */
  SCB->CCR &= ~(SCB_CCR_UNALIGN_TRP_Msk);

  temp = FLASH0->FCON;
  temp &= ~FLASH_FCON_WSPFLASH_Msk;
  temp |= PMU_FLASH_WS;
  FLASH0->FCON = temp;
}

__WEAK void SystemCoreClockSetup(void)
{
#if FOFI_CALIBRATION_MODE == FOFI_CALIBRATION_MODE_FACTORY
  /* Enable factory calibration */
  SCU_PLL->PLLCON0 |= SCU_PLL_PLLCON0_FOTR_Msk;
#else
  /* Automatic calibration uses the fSTDBY */

  /* Enable HIB domain */
  /* Power up HIB domain if and only if it is currently powered down */
  if((SCU_POWER->PWRSTAT & SCU_POWER_PWRSTAT_HIBEN_Msk) == 0)
  {
    SCU_POWER->PWRSET |= SCU_POWER_PWRSET_HIB_Msk;

    while((SCU_POWER->PWRSTAT & SCU_POWER_PWRSTAT_HIBEN_Msk) == 0)
    {
      /* wait until HIB domain is enabled */
    }
  }

  /* Remove the reset only if HIB domain were in a state of reset */
  if((SCU_RESET->RSTSTAT) & SCU_RESET_RSTSTAT_HIBRS_Msk)
  {
    SCU_RESET->RSTCLR |= SCU_RESET_RSTCLR_HIBRS_Msk;
    delay(DELAY_CNT_150US_50MHZ);
  }
  
#if STDBY_CLOCK_SRC == STDBY_CLOCK_SRC_OSCULP
  /* Enable OSC_ULP */
  if ((SCU_HIBERNATE->OSCULCTRL & SCU_HIBERNATE_OSCULCTRL_MODE_Msk) != 0UL)
  {
    /*enable OSC_ULP*/
    while (SCU_GENERAL->MIRRSTS & SCU_GENERAL_MIRRSTS_OSCULCTRL_Msk)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    SCU_HIBERNATE->OSCULCTRL &= ~SCU_HIBERNATE_OSCULCTRL_MODE_Msk;

    /* Check if the clock is OK using OSCULP Oscillator Watchdog*/
    while (SCU_GENERAL->MIRRSTS & SCU_GENERAL_MIRRSTS_HDCR_Msk)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    SCU_HIBERNATE->HDCR |= SCU_HIBERNATE_HDCR_ULPWDGEN_Msk;

    /* wait till clock is stable */
    do
    {
      while (SCU_GENERAL->MIRRSTS & SCU_GENERAL_MIRRSTS_HDCLR_Msk)
      {
        /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
      }
      SCU_HIBERNATE->HDCLR |= SCU_HIBERNATE_HDCLR_ULPWDG_Msk;

      delay(DELAY_CNT_50US_50MHZ);

    } while ((SCU_HIBERNATE->HDSTAT & SCU_HIBERNATE_HDSTAT_ULPWDG_Msk) != 0UL);

  }

  /* now OSC_ULP is running and can be used*/
  /* Select OSC_ULP as the clock source for RTC and STDBY*/
  while (SCU_GENERAL->MIRRSTS & SCU_GENERAL_MIRRSTS_HDCR_Msk)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  SCU_HIBERNATE->HDCR |= SCU_HIBERNATE_HDCR_RCS_Msk | SCU_HIBERNATE_HDCR_STDBYSEL_Msk;

#endif /* STDBY_CLOCK_SRC == STDBY_CLOCK_SRC_OSCULP */

  /* Enable automatic calibration of internal fast oscillator */
  SCU_PLL->PLLCON0 |= SCU_PLL_PLLCON0_AOTREN_Msk;
#endif /* FOFI_CALIBRATION_MODE == FOFI_CALIBRATION_MODE_AUTOMATIC */

  delay(DELAY_CNT_50US_50MHZ);

#if ENABLE_PLL

  /* enable PLL */
  SCU_PLL->PLLCON0 &= ~(SCU_PLL_PLLCON0_VCOPWD_Msk | SCU_PLL_PLLCON0_PLLPWD_Msk);

#if PLL_CLOCK_SRC != PLL_CLOCK_SRC_OFI
  /* enable OSC_HP */
  if ((SCU_OSC->OSCHPCTRL & SCU_OSC_OSCHPCTRL_MODE_Msk) != 0U)
  {
    SCU_OSC->OSCHPCTRL &= ~(SCU_OSC_OSCHPCTRL_MODE_Msk | SCU_OSC_OSCHPCTRL_OSCVAL_Msk);
    SCU_OSC->OSCHPCTRL |= ((OSCHP_GetFrequency() / FOSCREF) - 1UL) << SCU_OSC_OSCHPCTRL_OSCVAL_Pos;

    /* select OSC_HP clock as PLL input */
    SCU_PLL->PLLCON2 &= ~SCU_PLL_PLLCON2_PINSEL_Msk;

    /* restart OSC Watchdog */
    SCU_PLL->PLLCON0 &= ~SCU_PLL_PLLCON0_OSCRES_Msk;

    while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_OSC_USABLE) != SCU_PLL_PLLSTAT_OSC_USABLE)
    {
      /* wait till OSC_HP output frequency is usable */
    }
  }
#else /* PLL_CLOCK_SRC != PLL_CLOCK_SRC_OFI */

  /* select backup clock as PLL input */
  SCU_PLL->PLLCON2 |= SCU_PLL_PLLCON2_PINSEL_Msk;
#endif

  /* Go to bypass the Main PLL */
  SCU_PLL->PLLCON0 |= SCU_PLL_PLLCON0_VCOBYP_Msk;

  /* disconnect Oscillator from PLL */
  SCU_PLL->PLLCON0 |= SCU_PLL_PLLCON0_FINDIS_Msk;

  /* Setup divider settings for main PLL */
  SCU_PLL->PLLCON1 = ((PLL_NDIV << SCU_PLL_PLLCON1_NDIV_Pos) |
                      (PLL_K2DIV_0 << SCU_PLL_PLLCON1_K2DIV_Pos) |
                      (PLL_PDIV << SCU_PLL_PLLCON1_PDIV_Pos));

  /* Set OSCDISCDIS */
  SCU_PLL->PLLCON0 |= SCU_PLL_PLLCON0_OSCDISCDIS_Msk;

  /* connect Oscillator to PLL */
  SCU_PLL->PLLCON0 &= ~SCU_PLL_PLLCON0_FINDIS_Msk;

  /* restart PLL Lock detection */
  SCU_PLL->PLLCON0 |= SCU_PLL_PLLCON0_RESLD_Msk;

  while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOLOCK_Msk) == 0U)
  {
    /* wait for PLL Lock */
  }

  /* Disable bypass- put PLL clock back */
  SCU_PLL->PLLCON0 &= ~SCU_PLL_PLLCON0_VCOBYP_Msk;
  while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOBYST_Msk) != 0U)
  {
    /* wait for normal mode */
  }
#endif /* ENABLE_PLL */

#if (SYS_CLOCK_SRC == SYS_CLOCK_SRC_PLL)
  /* Switch system clock to PLL */
  SCU_CLK->SYSCLKCR |= SCU_CLK_SYSCLKCR_SYSSEL_Msk;
#else
  /* Switch system clock to backup clock */
  SCU_CLK->SYSCLKCR &= ~SCU_CLK_SYSCLKCR_SYSSEL_Msk;
#endif

  /* Before scaling to final frequency we need to setup the clock dividers */
  SCU_CLK->PBCLKCR = PBCLKDIV;
  SCU_CLK->CPUCLKCR = CPUCLKDIV;
  SCU_CLK->CCUCLKCR = CCUCLKDIV;
  SCU_CLK->WDTCLKCR = WDTCLKDIV;
  SCU_CLK->EBUCLKCR = EBUCLKDIV;
  SCU_CLK->USBCLKCR = USBCLKDIV;

#if ENABLE_PLL
  /* PLL frequency stepping...*/
  /* Reset OSCDISCDIS */
  SCU_PLL->PLLCON0 &= ~SCU_PLL_PLLCON0_OSCDISCDIS_Msk;

  SCU_PLL->PLLCON1 = ((PLL_NDIV << SCU_PLL_PLLCON1_NDIV_Pos) |
                        (PLL_K2DIV_1 << SCU_PLL_PLLCON1_K2DIV_Pos) |
                        (PLL_PDIV << SCU_PLL_PLLCON1_PDIV_Pos));


  delay(DELAY_CNT_50US_60MHZ);
  while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOLOCK_Msk) == 0U)
  {
    /* wait for PLL Lock */
  }

  SCU_PLL->PLLCON1 = ((PLL_NDIV << SCU_PLL_PLLCON1_NDIV_Pos) |
                        (PLL_K2DIV_2 << SCU_PLL_PLLCON1_K2DIV_Pos) |
                        (PLL_PDIV << SCU_PLL_PLLCON1_PDIV_Pos));


  delay(DELAY_CNT_50US_90MHZ);
  while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOLOCK_Msk) == 0U)
  {
    /* wait for PLL Lock */
  }

  SCU_PLL->PLLCON1 = ((PLL_NDIV << SCU_PLL_PLLCON1_NDIV_Pos) |
                        (PLL_K2DIV << SCU_PLL_PLLCON1_K2DIV_Pos) |
                        (PLL_PDIV << SCU_PLL_PLLCON1_PDIV_Pos));


  delay(DELAY_CNT_50US_120MHZ);
  while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOLOCK_Msk) == 0U)
  {
    /* wait for PLL Lock */
  }

  SCU_TRAP->TRAPCLR = SCU_TRAP_TRAPCLR_SOSCWDGT_Msk | SCU_TRAP_TRAPCLR_SVCOLCKT_Msk;
#endif /* ENABLE_PLL */

#if ENABLE_USBPLL
  /* enable USB PLL first */
  SCU_PLL->USBPLLCON &= ~(SCU_PLL_USBPLLCON_VCOPWD_Msk | SCU_PLL_USBPLLCON_PLLPWD_Msk);

  /* USB PLL uses as clock input the OSC_HP */
  /* check and if not already running enable OSC_HP */
  if ((SCU_OSC->OSCHPCTRL & SCU_OSC_OSCHPCTRL_MODE_Msk) != 0U)
  {
    /* check if Main PLL is switched on for OSC WDG*/
    if ((SCU_PLL->PLLCON0 &(SCU_PLL_PLLCON0_VCOPWD_Msk | SCU_PLL_PLLCON0_PLLPWD_Msk)) != 0UL)
    {
      /* enable PLL first */
      SCU_PLL->PLLCON0 &= ~(SCU_PLL_PLLCON0_VCOPWD_Msk | SCU_PLL_PLLCON0_PLLPWD_Msk);
    }

    SCU_OSC->OSCHPCTRL &= ~(SCU_OSC_OSCHPCTRL_MODE_Msk | SCU_OSC_OSCHPCTRL_OSCVAL_Msk);
    SCU_OSC->OSCHPCTRL |= ((OSCHP_GetFrequency() / FOSCREF) - 1UL) << SCU_OSC_OSCHPCTRL_OSCVAL_Pos;

    /* restart OSC Watchdog */
    SCU_PLL->PLLCON0 &= ~SCU_PLL_PLLCON0_OSCRES_Msk;

    while ((SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_OSC_USABLE) != SCU_PLL_PLLSTAT_OSC_USABLE)
    {
      /* wait till OSC_HP output frequency is usable */
    }
  }

  /* Setup USB PLL */
  /* Go to bypass the USB PLL */
  SCU_PLL->USBPLLCON |= SCU_PLL_USBPLLCON_VCOBYP_Msk;

  /* disconnect Oscillator from USB PLL */
  SCU_PLL->USBPLLCON |= SCU_PLL_USBPLLCON_FINDIS_Msk;

  /* Setup Divider settings for USB PLL */
  SCU_PLL->USBPLLCON = ((USB_NDIV << SCU_PLL_USBPLLCON_NDIV_Pos) |
                        (USB_PDIV << SCU_PLL_USBPLLCON_PDIV_Pos));

  /* Set OSCDISCDIS */
  SCU_PLL->USBPLLCON |= SCU_PLL_USBPLLCON_OSCDISCDIS_Msk;

  /* connect Oscillator to USB PLL */
  SCU_PLL->USBPLLCON &= ~SCU_PLL_USBPLLCON_FINDIS_Msk;

  /* restart PLL Lock detection */
  SCU_PLL->USBPLLCON |= SCU_PLL_USBPLLCON_RESLD_Msk;

  while ((SCU_PLL->USBPLLSTAT & SCU_PLL_USBPLLSTAT_VCOLOCK_Msk) == 0U)
  {
    /* wait for PLL Lock */
  }
#endif /* (USBCLKDIV & SCU_CLK_USBCLKCR_USBSEL_Msk) */

  /* Enable selected clocks */
  SCU_CLK->CLKSET = ENABLE_SCUCLK;

#if ENABLE_EXTCLK == 1
  /* Configure external clock */
  SCU_CLK->EXTCLKCR = EXTCLKDIV;

#if EXTCLK_PIN == EXTCLK_PIN_P1_15
  /* P1.15 */
  PORT1->PDR1 &= ~PORT1_PDR1_PD15_Msk;
  PORT1->IOCR12 = (PORT1->IOCR12 & ~PORT0_IOCR12_PC15_Msk) | (0x11U << PORT0_IOCR12_PC15_Pos);
#else
  /* P0.8 */
  PORT0->HWSEL &= ~PORT0_HWSEL_HW8_Msk;
  PORT0->PDR1 &= ~PORT0_PDR1_PD8_Msk;
  PORT0->IOCR8 = (PORT0->IOCR8 & ~PORT0_IOCR8_PC8_Msk) | (0x11U << PORT0_IOCR8_PC8_Pos);
#endif

#endif  /* ENABLE_EXTCLK == 1  */

  SystemCoreClockUpdate();
}

__WEAK void SystemCoreClockUpdate(void)
{
  uint32_t pdiv;
  uint32_t ndiv;
  uint32_t kdiv;
  uint32_t temp;

  if (SCU_CLK->SYSCLKCR & SCU_CLK_SYSCLKCR_SYSSEL_Msk)
  {
    /* fPLL is clock source for fSYS */
    if(SCU_PLL->PLLCON2 & SCU_PLL_PLLCON2_PINSEL_Msk)
    {
      /* PLL input clock is the backup clock (fOFI) */
      temp = OFI_FREQUENCY;
    }
    else
    {
      /* PLL input clock is the high performance osicllator (fOSCHP) */
      temp = OSCHP_GetFrequency();
    }

    /* check if PLL is locked */
    if (SCU_PLL->PLLSTAT & SCU_PLL_PLLSTAT_VCOLOCK_Msk)
    {
      /* PLL normal mode */
      /* read back divider settings */
      pdiv = ((SCU_PLL->PLLCON1 & SCU_PLL_PLLCON1_PDIV_Msk) >> SCU_PLL_PLLCON1_PDIV_Pos) + 1;
      ndiv = ((SCU_PLL->PLLCON1 & SCU_PLL_PLLCON1_NDIV_Msk) >> SCU_PLL_PLLCON1_NDIV_Pos) + 1;
      kdiv = ((SCU_PLL->PLLCON1 & SCU_PLL_PLLCON1_K2DIV_Msk) >> SCU_PLL_PLLCON1_K2DIV_Pos) + 1;

      temp = (temp / (pdiv * kdiv)) * ndiv;
    }
    else
    {
      /* PLL prescalar mode */
      /* read back divider settings */
      kdiv  = ((SCU_PLL->PLLCON1 & SCU_PLL_PLLCON1_K1DIV_Msk) >> SCU_PLL_PLLCON1_K1DIV_Pos) + 1;
      
      temp = (temp / kdiv);
    }
  }
  else
  {
    /* fOFI is clock source for fSYS */    
    temp = OFI_FREQUENCY;
  }

  temp = temp / ((SCU_CLK->SYSCLKCR & SCU_CLK_SYSCLKCR_SYSDIV_Msk) + 1);
  temp = temp / ((SCU_CLK->CPUCLKCR & SCU_CLK_CPUCLKCR_CPUDIV_Msk) + 1);

  SystemCoreClock = temp;
}

__WEAK uint32_t OSCHP_GetFrequency(void)
{
  return OSCHP_FREQUENCY;
}
