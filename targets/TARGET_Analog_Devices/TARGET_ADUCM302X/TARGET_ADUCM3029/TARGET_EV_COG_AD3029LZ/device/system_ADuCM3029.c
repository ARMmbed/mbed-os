/******************************************************************************
 * @file     system_ADuCM3029.c
 * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Source File for
 *           Device ADuCM3029
 * @version  V3.10
 * @date     23. November 2012
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

   Portions Copyright (c) 2016 - 2017 Analog Devices, Inc.
   ---------------------------------------------------------------------------*/
#include <cmsis.h>
#include <adi_pwr.h>
#include <adi_gpio.h>
#include <startup_ADuCM3029.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#ifdef ADI_DEBUG
/* only needed in debug mode */
uint32_t lfClock = 0u;    /* "lf_clk" coming out of LF mux             */
#endif

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
/* Note that these variables will be re-initialized to the value set here by the
   LIBC startup code, so if other clock values are required, make sure set them
   here.
*/
uint32_t hfClock = __HFOSC;         /* "root_clk" output of HF mux              */
uint32_t gpioClock = 0;             /* external GPIO clock                      */
uint32_t SystemCoreClock = __HFOSC; /*!< System Clock Frequency (Core Clock)    */


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

/*!
 * Update the clock.
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the variable SystemCoreClock and must be called whenever
 *         the core clock is changed during program execution.
 */
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
    uint32_t    val;
    uint16_t    div2;
    float       mul2, nDivisor, nMulfactor;

#ifdef ADI_DEBUG
    /* "lfclock" is only used during debug checks... */
    /* LF clock is always 32k, whether osc or xtal */
    lfClock = __LFCLK;  /* for beep, wdt and lcd */
    if( lfClock == 0 )
    {
      while( 1 );
    }
#endif

    /* Update Core Clock sources */
    /* update the HF clock */
    switch( pADI_CLKG0_CLK->CTL0 & BITM_CLKG_CLK_CTL0_CLKMUX ) {

        case HFMUX_INTERNAL_OSC_VAL:
          hfClock = __HFOSC;
          break;

        case HFMUX_EXTERNAL_XTAL_VAL:
          hfClock = __HFXTAL;
          break;

        case HFMUX_SYSTEM_SPLL_VAL:
            /* Calculate System PLL output frequency */
            if( pADI_CLKG0_CLK->CTL0 & BITM_CLKG_CLK_CTL0_SPLLIPSEL )
            {
                /* PLL input from HFXTAL */
                val = __HFXTAL;
            }
            else
            {
                /* PLL input from HFOSC */
                val = __HFOSC;
            }

            /* PLL NSEL multiplier */
            nMulfactor = ( ( pADI_CLKG0_CLK->CTL3 &BITM_CLKG_CLK_CTL3_SPLLNSEL ) >> BITP_CLKG_CLK_CTL3_SPLLNSEL );

            /* PLL MSEL divider */
            nDivisor = ( ( pADI_CLKG0_CLK->CTL3 & BITM_CLKG_CLK_CTL3_SPLLMSEL ) >> BITP_CLKG_CLK_CTL3_SPLLMSEL );

            /* PLL NSEL multiplier */
            div2 = ( ( pADI_CLKG0_CLK->CTL3 & BITM_CLKG_CLK_CTL3_SPLLDIV2 ) >> BITP_CLKG_CLK_CTL3_SPLLDIV2 );

            /* PLL MSEL divider */
            mul2 = ( ( pADI_CLKG0_CLK->CTL3 & BITM_CLKG_CLK_CTL3_SPLLMUL2 ) >> BITP_CLKG_CLK_CTL3_SPLLMUL2 );

            val = ( ( (uint32_t)( ( nMulfactor * ( mul2 + 1.0 ) * (float) val ) / nDivisor ) ) >> div2 );

            hfClock = val;
            break;

        case HFMUX_GPIO_VAL:
          hfClock = gpioClock;
          break;

        default:
          return;
    } /* end switch */

    SystemCoreClock = hfClock;
}


/*!
 * Configure the SRAM banks
 *
 * @return none
 *
 * @brief  Setup the SRAM banks.
 *         Initialize the SRAM configuration and retention.
 */
void SramInit(void)
{
    /* SRAM Bank1 and Banck2 are hibernate-preserved */
    adi_system_EnableRetention(ADI_SRAM_BANK_1, true);
    adi_system_EnableRetention(ADI_SRAM_BANK_2, true);
    /* To disable the instruction SRAM and entire 64K of SRAM is used as DSRAM */
    adi_system_EnableISRAM(false);
    /* To enable the 4K instruction cache out of DSRAM */
    adi_system_EnableCache(true);
}


/*!
 * Initialize the system
 *
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the relocate vector table.
 */
void SystemInit (void)
{
    uint32_t IntStatus;

    /* Turn off Tile 3029 LED */
    pADI_GPIO1->OEN |= ADI_GPIO_PIN_10;
    pADI_GPIO2->OEN |= ADI_GPIO_PIN_2;
    pADI_GPIO1->SET  = ADI_GPIO_PIN_10;
    pADI_GPIO2->SET  = ADI_GPIO_PIN_2;

    IntStatus = __get_PRIMASK();
    __disable_irq();

    /* Set boot ROM IVT. */
    SCB->VTOR = (uint32_t)NVIC_FLASH_VECTOR_ADDRESS;

    /* Set all three (USGFAULTENA, BUSFAULTENA, and MEMFAULTENA) fault enable bits
     * in the System Control Block, System Handler Control and State Register
     * otherwise these faults are handled as hard faults.
     */
    SCB->SHCSR = SCB_SHCSR_USGFAULTENA_Msk |
                 SCB_SHCSR_BUSFAULTENA_Msk |
                 SCB_SHCSR_MEMFAULTENA_Msk ;
    adi_pwr_Init();
    adi_pwr_SetClockDivider(ADI_CLOCK_HCLK,1);
    adi_pwr_SetClockDivider(ADI_CLOCK_PCLK,1);

    /* Set up the LF clock source */
    adi_pwr_SetLFClockMux(ADI_CLOCK_MUX_LFCLK_LFXTAL);
    adi_pwr_EnableClockSource(ADI_CLOCK_SOURCE_LFXTAL,true);

    __set_PRIMASK(IntStatus);
}

/*!
 * @brief  This enables or disables  the cache.
 * \n @param  bEnable : To specify whether to enable/disable cache.
 * \n              true : To enable cache.
 * \n
 * \n              false : To disable cache.
 * \n
 * @return none
 *
 */
void adi_system_EnableCache(bool bEnable)
{
    pADI_FLCC0_CACHE->KEY = CACHE_CONTROLLER_KEY;
    if(bEnable)
    {
        pADI_FLCC0_CACHE->SETUP |= BITM_FLCC_CACHE_SETUP_ICEN;
    }
    else
    {
        pADI_FLCC0_CACHE->SETUP &= ~BITM_FLCC_CACHE_SETUP_ICEN;
    }
}

/*!
 * @brief  This enables or disables instruction SRAM
 *
 * @param bEnable: To enable/disable the instruction SRAM.
 * \n              true : To enable cache.
 * \n
 * \n              false : To disable cache.
 * \n
 * @return none
 * @note:  Please note that respective linker file need to support the configuration.
 */
void adi_system_EnableISRAM(bool bEnable)
{

    if(bEnable)
    {
        pADI_PMG0_TST->SRAM_CTL |= BITM_PMG_TST_SRAM_CTL_INSTREN;
    }
    else
    {
        pADI_PMG0_TST->SRAM_CTL &= ~BITM_PMG_TST_SRAM_CTL_INSTREN;
    }
}

/*!
 * @brief  This enables/disable SRAM retention during the hibernation.
 * @param eBank:   Specify which SRAM bank. Only BANK1 and BANK2 are valid.
 * @param bEnable: To enable/disable the  retention for specified  SRAM bank.
 * \n              true : To enable retention during the hibernation.
 * \n
 * \n              false :To disable retention during the hibernation.
 * \n
 * @return : ADI_SYS_SUCCESS : Configured successfully.
 *           ADI_SYS_FAILURE :  For invalid bank.
 * @note: Please note that respective linker file need to support the configuration. Only BANK-1 and
          BANK-2 of SRAM is valid.
 */
uint32_t adi_system_EnableRetention(ADI_SRAM_BANK eBank,bool bEnable)
{
#ifdef ADI_DEBUG
    if((eBank != ADI_SRAM_BANK_1) && (eBank != ADI_SRAM_BANK_2))
    {
        return ADI_SYS_FAILURE;
    }
#endif
    pADI_PMG0->PWRKEY = PWRKEY_VALUE_KEY;
    if(bEnable)
    {
        pADI_PMG0->SRAMRET |= (uint32_t)eBank>>1;
    }
    else
    {
        pADI_PMG0->SRAMRET &= ~((uint32_t)eBank >> 1);
    }

    return ADI_SYS_SUCCESS;
}
