/**************************************************************************//**
 * @file     system_ADuCM4050.c
 * @brief    CMSIS Cortex-M4 Device Peripheral Access Layer Source File for
 *           Device ADuCM4x50
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
#include <startup_ADuCM4050.h>

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
uint32_t gpioClock = 0u;            /* external GPIO clock                      */
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
void SystemCoreClockUpdate(void)
{
    uint32_t val, nDivisor, nMulfactor, div2, mul2;

#ifdef ADI_DEBUG
    /* "lfclock" is only used during debug checks... */
    /* LF clock is always 32k, whether osc or xtal */
    lfClock = __LFCLK;  /* for beep, wdt and lcd */
    if (lfClock == 0u) {
        while (1) {}
    }
#endif
    /* Update Core Clock sources */
    /* update the HF clock */
    switch (pADI_CLKG0_CLK->CTL0 & BITM_CLKG_CLK_CTL0_CLKMUX ) {

        case HFMUX_INTERNAL_OSC_VAL:
            hfClock = __HFOSC;
            break;

        case HFMUX_EXTERNAL_XTAL_VAL:
            hfClock = __HFXTAL;
            break;

        case HFMUX_SYSTEM_SPLL_VAL:
            /* Calculate System PLL output frequency */
            if ((pADI_CLKG0_CLK->CTL0 & BITM_CLKG_CLK_CTL0_PLL_IPSEL) != 0u) {
                /* PLL input from HFXTAL */
                val = __HFXTAL;
            } else {
                /* PLL input from HFOSC */
                val = __HFOSC;
            }

            /* PLL NSEL multiplier */
            nMulfactor = (pADI_CLKG0_CLK->CTL3 & BITM_CLKG_CLK_CTL3_SPLLNSEL) >> BITP_CLKG_CLK_CTL3_SPLLNSEL;
            /* PLL MSEL divider */
            nDivisor = (pADI_CLKG0_CLK->CTL3 & BITM_CLKG_CLK_CTL3_SPLLMSEL) >> BITP_CLKG_CLK_CTL3_SPLLMSEL;

            /* PLL NSEL multiplier */
            mul2 = (pADI_CLKG0_CLK->CTL3 & BITM_CLKG_CLK_CTL3_SPLLMUL2) >> BITP_CLKG_CLK_CTL3_SPLLMUL2;
            /* PLL MSEL divider */
            div2 = (pADI_CLKG0_CLK->CTL3 & BITM_CLKG_CLK_CTL3_SPLLDIV2) >> BITP_CLKG_CLK_CTL3_SPLLDIV2;

            val = ((val << mul2) * nMulfactor / nDivisor) >> div2;

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
    /* On reset, there is no SRAM retention.  Any retention has to be explicitly
     * set here. */
    adi_system_EnableRetention(ADI_SRAM_BANK_1 |
                               ADI_SRAM_BANK_3 |
                               ADI_SRAM_BANK_4 |
                               ADI_SRAM_BANK_5 |
                               ADI_SRAM_BANK_6 |
                               ADI_SRAM_BANK_7, true);
    /* To disable the instruction SRAM and entire 64K of SRAM is used as DSRAM */
    adi_system_EnableISRAM(false);
    /* To disable the instruction cache  */
    adi_system_EnableCache(false);
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

#if (__FPU_PRESENT == 1u) && (__FPU_USED == 1u)
    /* the FPU is disabled by default so enable FPU (NEON and VFP)
     * set the System Control Block, Coprocessor Access Control Register bits:
     * CP10 = grant CP10 coprocessor privileges and user mode access (full access)
     * CP11 = grant CP11 coprocessor privileged and user mode access (full access)
     * (CP10 and CP11 MUST be the same or "BEHAVIOR IS UNPREDICTABLE")
     */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 for Full Access */
#endif

    /* Flush instruction and data pipelines to insure assertion of new settings. */
    __ISB();
    __DSB();

    adi_pwr_Init();
    adi_pwr_SetClockDivider(ADI_CLOCK_HCLK,1);
    adi_pwr_SetClockDivider(ADI_CLOCK_PCLK,1);

    /* Set up the LF clock MUX. Currently LFXTAL is unstable so use the
       internal LF Oscillator instead. LFXTAL is still required to be enabled
       as it is required by RTC0. This LFXTAL issue is going to be fixed
       in the next revision of the silicon. */
    adi_pwr_EnableClockSource(ADI_CLOCK_SOURCE_LFXTAL,true);
    adi_pwr_SetLFClockMux(ADI_CLOCK_MUX_LFCLK_LFOSC);
    adi_pwr_EnableClockSource(ADI_CLOCK_SOURCE_LFOSC,true);

    __set_PRIMASK(IntStatus);
}

/*!
 * @brief  Enables or disables the cache.
 * @param  bEnable Specify whether to enable/disable cache.
 *                 - true : Enable cache.
 *                 - false: Disable cache.
 * @return none
 */
void adi_system_EnableCache(bool bEnable)
{
    pADI_FLCC0_CACHE->KEY = CACHE_CONTROLLER_KEY;
    if( bEnable == true ) {
        pADI_FLCC0_CACHE->SETUP |= BITM_FLCC_CACHE_SETUP_ICEN;
    } else {
        pADI_FLCC0_CACHE->SETUP &= ~BITM_FLCC_CACHE_SETUP_ICEN;
    }
}

/*!
 * @brief  This enables or disables instruction SRAM
 *
 * @param bEnable Enable/disable the instruction SRAM.
 *               -  true : Enable instruction SRAM.
 *               -  false : Disable instruction SRAM.
 * @return none
 * @note The appropriate linker file needs to support the configuration.
 */
void adi_system_EnableISRAM(bool bEnable)
{

    if( bEnable == true ) {
        pADI_PMG0_TST->SRAM_CTL |= BITM_PMG_TST_SRAM_CTL_INSTREN;
    } else {
        pADI_PMG0_TST->SRAM_CTL &= ~BITM_PMG_TST_SRAM_CTL_INSTREN;
    }
}

/*!
 * @brief  This enables/disable SRAM retention during the hibernation.
 * @param eBank:   Specify which SRAM banks.  Multiple banks can be set
 /                 using a logical OR of the banks.
 * @param bEnable Enable/disable the  retention for specified SRAM bank.
 *               -  true : Enable retention during hibernation.
 *               -  false: Disable retention during hibernation.
 * @return   ADI_SYS_SUCCESS Configured successfully.
 * @return   ADI_SYS_FAILURE Invalid bank, or banks, specified.  Any incorrect
 *                           or invalid bank options will result in failure and
 *                           no changes will have been applied.
 * @note The appropriate linker file needs to support the configuration.
 *       BANK  0 is always retained.
 *       BANKS 1 can be retained individually.
 *       BANK  2 is never retained.
 *       BANKS 3 and 4 can only be mutually retained.
 *       BANKS 5 can be retained individually.
 *       BANKS 6 and 7 can only be mutually retained.
 */
ADI_SYS_RESULT adi_system_EnableRetention(ADI_SRAM_BANK eBank, bool bEnable)
{
    uint32_t retainBits = 0u;

#ifdef ADI_DEBUG
    if((0u != (eBank & ADI_SRAM_BANK_0)) ||
            (0u != (eBank & ADI_SRAM_BANK_2))) {
        /* Banks 0 and 2 are not selectable */
        return ADI_SYS_FAILURE;
    }

    /* Are banks 3 or 4 selected? */
    if(0u != (eBank & (ADI_SRAM_BANK_3 | ADI_SRAM_BANK_4))) {
        /* If so, the only valid option is for both to be retained. */
        if((eBank & (ADI_SRAM_BANK_3 | ADI_SRAM_BANK_4)) != (ADI_SRAM_BANK_3 | ADI_SRAM_BANK_4)) {
            return ADI_SYS_FAILURE;
        }
    }

    /* Are banks 6 or 7 selected? */
    if(0u != (eBank & (ADI_SRAM_BANK_6 | ADI_SRAM_BANK_7))) {
        /* If so, the only valid option is for both to be retained */
        if((eBank & (ADI_SRAM_BANK_6 | ADI_SRAM_BANK_7)) != (ADI_SRAM_BANK_6 | ADI_SRAM_BANK_7)) {
            return ADI_SYS_FAILURE;
        }
    }
#endif
    if((eBank & ADI_SRAM_BANK_1) != 0u) {
        retainBits |= BITM_PMG_SRAMRET_RET1;
    }
    if((eBank & (ADI_SRAM_BANK_3 | ADI_SRAM_BANK_4)) != 0u) {
        retainBits |= BITM_PMG_SRAMRET_RET2;
    }
    if((eBank & ADI_SRAM_BANK_5) != 0u) {
        retainBits |= BITM_PMG_SRAMRET_RET3;
    }
    if((eBank & (ADI_SRAM_BANK_6 | ADI_SRAM_BANK_7)) != 0u) {
        retainBits |= BITM_PMG_SRAMRET_RET4;
    }

    /* Unlock the SRAMRET register using the PWRKEY.
     * If there is any chance that this sequence can be interrupted then it
     * should be protected by disabling interrupts.   A write to any other
     * register on the APB bus before writing to PMG_SRAMRET will return the
     * protection to the lock state. */
    pADI_PMG0->PWRKEY = PWRKEY_VALUE_KEY;
    if(bEnable) {
        pADI_PMG0->SRAMRET |= retainBits;
    } else {
        pADI_PMG0->SRAMRET &= ~(retainBits);
    }

    return ADI_SYS_SUCCESS;
}
