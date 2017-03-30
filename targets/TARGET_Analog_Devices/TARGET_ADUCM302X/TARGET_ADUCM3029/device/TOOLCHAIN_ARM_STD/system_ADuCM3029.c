/**************************************************************************//**
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

#include <stdint.h>
#include <ADuCM3029.h>
#include <adi_int.h>
#include <adi_pwr.h>

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
 
extern uint32_t __Vectors[];

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

#ifdef ADI_DEBUG
/* only needed in debug mode */
uint32_t lfClock = 0u;    /* "lf_clk" coming out of LF mux             */
#endif

uint32_t hfClock = 0u;    /* "root_clk" output of HF mux               */
uint32_t gpioClock = 0u;  /* external GPIO clock                       */

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/

uint32_t SystemCoreClock = 0u;  /*!< System Clock Frequency (Core Clock)*/

/*----------------------------------------------------------------------------
  Security options
 *----------------------------------------------------------------------------*/

#if defined (__CC_ARM)
  __attribute__ ((at(0x00000180u)))
#elif defined (__GNUC__)
  __attribute__ ((section(".security_options")))
#endif /* __GNUC__ */
  __attribute__ ((weak))
const ADI_ADUCM302X_SECURITY_OPTIONS adi_aducm302x_security_options
  = {
        { 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu },
        0xA79C3203u,
        127u,
        0xFFFFFFFFu,
        0xFFFFFFFFu,
};

#ifdef RELOCATE_IVT

/**
  A relocated IVT is requested.  Provision for IVT relocation
  to RAM during startup.  This allows for dynamic interrupt
  vector patching required by RTOS.  Places the relocated IVT
  at the start of RAM.  Note: the IVT placement is required
  to be next power-of-two of the vector table size.  So the
  IVT includes 61 programmable interrupts, 15 system exception
  vectors and the main stack pointer, therefore we need
  (61 + 15 + 1)*4 = 308 bytes, which rounds up to a 512 (0x200)
  address boundary (which address 0x20000000 satisfies).

  Also note use of the "__no_init" attribute to force the
  relocated IVT into the ".noinit" section.  This prevents
  the CRTL startup sequence from initializing the relocated
  IVT after we have activated it during the reset vector.
  so that the CRTL does not clear it.
*/

#define RELOCATION_ADDRESS    (0x20000000)
#define RELOCATION_ALIGNMENT  (0x80)
#define NUM_VECTORS           (64 + 1 + 15)

#if defined ( __ICCARM__ )
    #pragma data_alignment=RELOCATION_ALIGNMENT  /* IAR */
#elif defined (__CC_ARM)
    __align(RELOCATION_ALIGNMENT)                /* Keil */
#else
    #pragma message("WARNING: NO ALIGNMENT DEFINED FOR IVT RELOCATION")
#endif

/* reserve aligned IVT space at top of RAM */
void (*__Relocated___Vectors[NUM_VECTORS])(void) __attribute__( ( at( RELOCATION_ADDRESS ) ) ) = { 0 };

#endif /* RELOCATE_IVT */

ADI_CRITICAL_REGION_VAR_DEFINE

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
    float32_t   mul2, nDivisor, nMulfactor;

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

            val = ( ( (uint32_t)( ( nMulfactor * ( mul2 + 1.0 ) * (float32_t) val ) / nDivisor ) ) >> div2 );

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

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 *         Initialize the Embedded Flash Interface, the PLL and update the
 *         SystemCoreClock variable.
 *
 * @note   This function should be used only after reset.
 */
void SystemInit (void)
{
#ifdef RELOCATE_IVT
    int i;
    uint8_t *pSrc, *pDst;
#endif
    uint32_t IntStatus;

    /* Unlock the PWRMOD register by writing the two keys to the PWRKEY register */
    pADI_PMG0->PWRKEY = PWRKEY_VALUE_KEY;
    pADI_PMG0->SRAMRET &= (uint32_t)( ~( BITM_PMG_SRAMRET_BNK2EN | BITM_PMG_SRAMRET_BNK1EN ) );
    
    /* SRAM Bank1 and Banck2 are hibernate-preserved */
    adi_system_EnableRetention( ADI_SRAM_BANK_1, true );
    adi_system_EnableRetention( ADI_SRAM_BANK_2, true );

    /* To disable the instruction SRAM and entire 64K of SRAM is used as DSRAM */
    adi_system_EnableISRAM( false );

    /* To disable the instruction cache  */
    adi_system_EnableCache( false );

#ifdef RELOCATE_IVT
    /* Copy the IVT from Flash to SRAM (avoid use of memcpy here so it does not become locked into flash) */
    for( i = 0, pSrc = (uint8_t*) __Vectors, pDst = (uint8_t*) __Relocated___Vectors; i < ( NUM_VECTORS * 4 ); i++ )
    {
      *pDst++ = *pSrc++;
    }
#endif

    /* Switch the Interrupt Vector Table Offset Register
     * (VTOR) to point to the relocated IVT in SRAM
     */

    /* Because SystemInit must not use global variables, the following
     * interrupt disabling code should not be replaced with critical region
     * code which uses global variables.
     */
    IntStatus = __get_PRIMASK();
    __disable_irq();

    /* Switch from boot ROM IVT to application's IVT
     * set the System Control Block, Vector Table Offset Register
     */
#ifdef RELOCATE_IVT
    SCB->VTOR = (uint32_t) &__Relocated___Vectors;
#else
    SCB->VTOR = (uint32_t) &__Vectors;
#endif

    /* Set all three (USGFAULTENA, BUSFAULTENA, and MEMFAULTENA) fault enable bits
     * in the System Handler Control and State Register otherwise these faults are
     * handled as hard faults
     */
    SCB->SHCSR = SCB_SHCSR_USGFAULTENA_Msk |
                 SCB_SHCSR_BUSFAULTENA_Msk |
                 SCB_SHCSR_MEMFAULTENA_Msk ;

    /* Flush instruction and data pipelines to ensure assertion of new settings */
    __ISB();
    __DSB();

    __set_PRIMASK(IntStatus);
	adi_pwr_Init();
	adi_pwr_SetClockDivider(ADI_CLOCK_HCLK,1);
	adi_pwr_SetClockDivider(ADI_CLOCK_PCLK,1);
}

/*!
 * @brief  Enables or disables the cache.
 * \n @param  bEnable : To specify whether to enable/disable cache.
 * \n              true : To enable cache.
 * \n              false : To disable cache.
 * \n
 * @return none
 *
 */
void adi_system_EnableCache (bool_t bEnable)
{
  pADI_FLCC0_CACHE->KEY = CACHE_CONTROLLER_KEY;
  if( bEnable == true )
  {
    pADI_FLCC0_CACHE->SETUP |=BITM_FLCC_CACHE_SETUP_ICEN;
  }
  else
  {
    pADI_FLCC0_CACHE->SETUP &=(uint32_t)(~(BITM_FLCC_CACHE_SETUP_ICEN));
  }
}

/*!
 * @brief  Enables or disables instruction SRAM
 *
 * @param bEnable: To enable/disable the instruction SRAM.
 * \n              true : To enable cache.
 * \n              false : To disable cache.
 * \n
 * @return none
 * @note:  Please note that respective linker file need to support the configuration.
 */
void adi_system_EnableISRAM (bool_t bEnable)
{
  if( bEnable == true )
  {
    pADI_PMG0_TST->SRAM_CTL |=BITM_PMG_TST_SRAM_CTL_INSTREN;
  }
  else
  {
    pADI_PMG0_TST->SRAM_CTL &=(uint32_t)(~(BITM_PMG_TST_SRAM_CTL_INSTREN));
  }
}

/*!
 * @brief  Enables/disable SRAM retention during the hibernation.
 * @param eBank:   Specify which SRAM bank. Only BANK1 and BANK2 are valid.
 * @param bEnable: To enable/disable the  retention for specified  SRAM bank.
 * \n              true : To enable retention during the hibernation.
 * \n              false :To disable retention during the hibernation.
 * \n
 * @return : SUCCESS : Configured successfully.
 *           FAILURE :  For invalid bank.
 *
 * @note: Please note that respective linker file need to support the configuration. Only BANK-1 and
 *        BANK-2 of SRAM is valid.
 */
uint32_t adi_system_EnableRetention (ADI_SRAM_BANK eBank, bool_t bEnable)
{
#ifdef ADI_DEBUG
  if( ( eBank != ADI_SRAM_BANK_1 ) && ( eBank != ADI_SRAM_BANK_2 ) )
  {
    return( FAILURE );
  }
#endif

  pADI_PMG0->PWRKEY = PWRKEY_VALUE_KEY;

  if( bEnable == true )
  {
    pADI_PMG0->SRAMRET |= ( eBank >> 1 );
  }
  else
  {
    pADI_PMG0->SRAMRET &= (uint32_t)( ~( ( eBank >> 1 ) ) );
  }
  return( SUCCESS );
}

