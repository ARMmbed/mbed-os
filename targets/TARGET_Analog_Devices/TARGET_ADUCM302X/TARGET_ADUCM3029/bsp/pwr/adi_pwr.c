/*
 *****************************************************************************
 * @file:    adi_pwr.c
 * @brief:   Power Management driver implementation.
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/*! \addtogroup Power_Driver Power Driver
 *  @{
 *  @brief Power Management Driver
 *  @note The application must include drivers/pwr/adi_pwr.h to use this driver
 *  @note The API #adi_pwr_EnableClockSource requires the GPIO driver if
 *  #ADI_PWR_CFG_ENABLE_CLOCK_SOURCE_GPIO is set to 1. In that case the
 *  application must include the GPIO driver sources to avoid link errors.
 */


#include <stdlib.h>     /* for 'NULL' */
#include <adi_callback.h>
#include <adi_pwr_config.h>
#include <rtos_map/adi_rtos_map.h>
#include <drivers/pwr/adi_pwr.h>
#include "adi_pwr_def.h"
#include <drivers/gpio/adi_gpio.h>

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): Types which specify sign and size should be used
*   We use bool which is accepted by MISRA but the toolchain does not accept it
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
* Pm057 (rule 15.2): Every non-empty case clause in a switch statement shall be terminated with a break statement.
*   In some cases we have return statement instead of break. It is not valid to both return and break in MISRA 2012.
*/
#pragma diag_suppress=Pm011,Pm073,Pm050,Pm140,Pm143,Pm057
#endif /* __ICCARM__ */

/*! \cond PRIVATE */

/*----------------------------------------------------------------------------
  Internal Clock Variables. The external ones are defined in system.c
 *---------------------------------------------------------------------------*/
#ifdef ADI_DEBUG
/* not needed unless its debug mode */
extern uint32_t lfClock;    /* "lf_clk" coming out of LF mux             */
#endif

extern uint32_t hfClock;    /* "root_clk" output of HF mux               */
extern uint32_t gpioClock;  /* external GPIO clock                       */

static ADI_CALLBACK  gpfCallbackFunction;
static void *gpPowcbParam = NULL;
static uint32_t gnLowPowerIntOccFlag = 0u;

/*! \endcond */

/*----------------------------------------------------------------------------
  Clock functions
 *---------------------------------------------------------------------------*/
/**
 * Initialize the clock configuration register with the default values.
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully initialized the power service.
 */
ADI_PWR_RESULT adi_pwr_Init (void)
{
    /* Enable internal HF oscillators */
    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;

    pADI_CLKG0_OSC->CTL = OSCCTRL_CONFIG_VALUE;

    gpfCallbackFunction = NULL;
    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;

    /* Switch on the internal HF oscillator */
    pADI_CLKG0_OSC->CTL |= BITM_CLKG_OSC_CTL_HFOSC_EN;

    /* wait for HF OSC to stabilize */
    while ((pADI_CLKG0_OSC->CTL & (1U << BITP_CLKG_OSC_CTL_HFOSC_OK)) == 0u)
    {
    }

    /* Switch over to the internal HF oscillator */
    pADI_CLKG0_CLK->CTL0 &= ~(BITM_CLKG_CLK_CTL0_CLKMUX);

    /* complete remaining reset sequence */
    pADI_CLKG0_CLK->CTL0    = CLOCK_CTL0_CONFIG_VALUE;
    pADI_CLKG0_CLK->CTL1    = CLOCK_CTL1_CONFIG_VALUE;
    
#if defined(__ADUCM4x50__)    
    pADI_CLKG0_CLK->CTL2    = CLOCK_CTL2_CONFIG_VALUE;
#endif /*__ADUCM4x50__ */     
    
    pADI_CLKG0_CLK->CTL3    = CLOCK_CTL3_CONFIG_VALUE;
    /* No CLK CTL4 */
    pADI_CLKG0_CLK->CTL5    = CLOCK_CTL5_CONFIG_VALUE;

    /*
     * Configure the power management registers
     */
    pADI_PMG0->IEN          = PWM_INTERRUPT_CONFIG;
    pADI_PMG0->PWRMOD       = PWM_PWRMOD_CONFIG;
    pADI_PMG0->CTL1         = PWM_HPBUCK_CONTROL;
    
    /* disable external HF crystal oscillator */
    /* (don't disable LF crystal or the RTC will lose time */
    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
    pADI_CLKG0_OSC->CTL &= ~BITM_CLKG_OSC_CTL_HFX_EN;

    NVIC_EnableIRQ(PMG0_VREG_OVR_IRQn);
    NVIC_EnableIRQ(PMG0_BATT_RANGE_IRQn);

    NVIC_EnableIRQ(CLKG_XTAL_OSC_EVT_IRQn);
    NVIC_EnableIRQ(CLKG_PLL_EVT_IRQn);

    /* compute new internal clocks based on the newly reset controller */
    SystemCoreClockUpdate();

    return(ADI_PWR_SUCCESS);
}


/**
 * @brief  Updates the internal SystemCoreClock variable with current core
 *         Clock retrieved from cpu registers.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS : Updated core system core clock variables.
 *
 * Updates the internal SystemCoreClock variable with current core
 * Clock retrieved from cpu registers.
 *
 * @sa adi_pwr_GetClockFrequency ()
 */
ADI_PWR_RESULT adi_pwr_UpdateCoreClock (void)
{
    SystemCoreClockUpdate();
    return(ADI_PWR_SUCCESS);
}

/**
 * @brief  Registers or unregister the callback function.
 *
 * @details Application can register or unregister the callback function which
 *          will be called to notify the events from the driver.
 *
 * @param[in]  pfCallback   : Callback function pointer.
 * @param[in]  pcbParam     : Callback parameter.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS            : Successfully installed the callback function.
 *              - #ADI_PWR_NULL_POINTER [D]   : Failed to install the callback function since the call back function pointer is NULL.
 */
ADI_PWR_RESULT adi_pwr_RegisterCallback(
               const ADI_CALLBACK pfCallback,
               void *pcbParam
           )
{

#ifdef ADI_DEBUG
   if(pfCallback == NULL)
   {
      return(ADI_PWR_NULL_POINTER);
   }
#endif

    gpfCallbackFunction = pfCallback;
    gpPowcbParam        = pcbParam;

    return ADI_PWR_SUCCESS;
}

/**
 * @brief  Sets the system external clock frequency
 *
 * @param[in]  ExtClkFreq: External clock frequency in Hz

 * @return      Status
 *              - #ADI_PWR_SUCCESS                : Successfully set the external clock as source.
 *              - #ADI_PWR_INVALID_CLOCK_SPEED [D]: Specified clock is out of range.
 *
 * @sa adi_pwr_GetClockFrequency ()
 */
ADI_PWR_RESULT adi_pwr_SetExtClkFreq (const uint32_t ExtClkFreq)
{
#ifdef ADI_DEBUG
    if(ExtClkFreq > MAXIMUM_EXT_CLOCK)
    {
         return(ADI_PWR_INVALID_CLOCK_SPEED);
    }
#endif
    gpioClock = ExtClkFreq;
    return(ADI_PWR_SUCCESS);
}

/**
 * @brief  Sets the input clock source for PLL multiplexer.
 *
 * @param[in]  eClockID: Clock source to the System PLL multiplexer.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS              : Successfully set the PLL multiplexer clock source.
 *              - #ADI_PWR_INVALID_CLOCK_ID [D] : Specified clock ID is invalid.
 *
 * @sa adi_pwr_SetLFClockMux()
 */
ADI_PWR_RESULT  adi_pwr_SetPLLClockMux(const ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    /* Validate the given clock ID */
    switch (eClockID)
    {
        case ADI_CLOCK_MUX_SPLL_HFOSC:
        case ADI_CLOCK_MUX_SPLL_HFXTAL:
          
#if defined(__ADUCM4x50__)          
        case ADI_CLOCK_MUX_SPLL_GPIO:
#endif /* __ADUCM4x50__ */           
            break;

            
      /* Any other clock ID is not valid since we are configuring the SPLL clock multiplexer.
       * Only valid input clock to the multiplexer is HFOSC, HFXTAL, GPIO */
        default:
            return(ADI_PWR_INVALID_CLOCK_ID);
    }
#endif /* ADI_DEBUG */

    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();
    tmp     =   (pADI_CLKG0_CLK->CTL0 & ~BITM_CLKG_CLK_CTL0_PLL_IPSEL);
    tmp     |=  (( (uint32_t)eClockID - (uint32_t)ADI_CLOCK_MUX_SPLL_HFOSC) << BITP_CLKG_CLK_CTL0_PLL_IPSEL);
    pADI_CLKG0_CLK->CTL0 = tmp;
    ADI_EXIT_CRITICAL_REGION();

    return(ADI_PWR_SUCCESS);
}

/**
 * @brief  Sets the input clock for low frequency clock multiplexer.
 *
 * @param[in]  eClockID: Clock source to the low frequency clock multiplexer.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS                : Successfully LF clock multiplexer clock source.
 *              - #ADI_PWR_INVALID_CLOCK_ID [D]   : Specified clock ID is invalid.
 *
 * @sa adi_pwr_SetRootClockMux()
 * @sa adi_pwr_SetPLLClockMux()
 */
ADI_PWR_RESULT  adi_pwr_SetLFClockMux(const ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    switch (eClockID)
    {

        case ADI_CLOCK_MUX_LFCLK_LFOSC:
        case ADI_CLOCK_MUX_LFCLK_LFXTAL:
            break;
      /* Any other clock ID is not valid since we are configuring the Low frequency clock multiplexer.
       * Only valid input clock to the multiplexer is LFOSC, LFXTAL */

    default:
        return(ADI_PWR_INVALID_CLOCK_ID);

    }
#endif /* ADI_DEBUG */

    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();

    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
    tmp = (pADI_CLKG0_OSC->CTL & ~BITM_CLKG_OSC_CTL_LFCLK_MUX);
    tmp |=(((uint32_t)eClockID - (uint32_t)ADI_CLOCK_MUX_LFCLK_LFOSC) << BITP_CLKG_OSC_CTL_LFCLK_MUX);
    pADI_CLKG0_OSC->CTL = tmp;

    ADI_EXIT_CRITICAL_REGION();

    return(ADI_PWR_SUCCESS);
}

/**
 * @brief  Sets clock source for the Reference clock multiplexer.
 *
 * @param[in]  eClockID: Clock source to the reference clock multiplexer.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS                : Successfully set the source for reference clock multiplexer.
 *              - #ADI_PWR_INVALID_CLOCK_ID [D]   : Specified clock ID is invalid.
 *
 * @sa adi_pwr_SetLFClockMux()
 * @sa adi_pwr_SetRootClockMux()
 * @sa adi_pwr_SetPLLClockMux()
 */

ADI_PWR_RESULT  adi_pwr_SetRefClockMux(const ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    switch (eClockID)
    {

        case ADI_CLOCK_MUX_REF_HFOSC_CLK:
        case ADI_CLOCK_MUX_REF_HFXTAL_26MHZ_CLK:
        case ADI_CLOCK_MUX_REF_HFXTAL_16MHZ_CLK:
            break;
      /* Any other clock ID is not valid since we are configuring the out clock multiplexer.*/

        default:
            return(ADI_PWR_INVALID_CLOCK_ID);
    }
#endif /* ADI_DEBUG */

    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();

    tmp = (pADI_CLKG0_CLK->CTL0 & ~BITM_CLKG_CLK_CTL0_RCLKMUX);
    tmp |=(((uint32_t)eClockID - (uint32_t)ADI_CLOCK_MUX_REF_HFOSC_CLK) << BITP_CLKG_CLK_CTL0_RCLKMUX);
    pADI_CLKG0_CLK->CTL0 = tmp;

    ADI_EXIT_CRITICAL_REGION();

    return(ADI_PWR_SUCCESS);
}

/**
 * @brief  Sets the source for the root clock multiplexer.
 *
 * @param[in]  eClockID: Clock source to the root clock multiplexer.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS                : Successfully set the source for root clock multiplexer.
 *              - #ADI_PWR_INVALID_CLOCK_ID [D]   : Specified clock ID is invalid.
 *
 * @sa adi_pwr_SetLFClockMux()
 * @sa adi_pwr_SetPLLClockMux()
 */
ADI_PWR_RESULT  adi_pwr_SetRootClockMux(const ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    switch (eClockID)
    {
       case ADI_CLOCK_MUX_ROOT_HFOSC:
       case ADI_CLOCK_MUX_ROOT_HFXTAL:
       case ADI_CLOCK_MUX_ROOT_SPLL:
       case ADI_CLOCK_MUX_ROOT_GPIO:
            break;
		  /* Any other clock ID is not valid since we are configuring the root clock multiplexer.
		   * Only valid input clock to the multiplexer is HFOSC, HFXTAL, SPLL, GPIO */
		default:
        	return(ADI_PWR_INVALID_CLOCK_ID);
    }
#endif /* ADI_DEBUG */

    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();

    tmp =  (pADI_CLKG0_CLK->CTL0 & ~BITM_CLKG_CLK_CTL0_CLKMUX);
    tmp |= (((uint32_t)eClockID - (uint32_t)ADI_CLOCK_MUX_ROOT_HFOSC) << BITP_CLKG_CLK_CTL0_CLKMUX);
    pADI_CLKG0_CLK->CTL0 = tmp;

    ADI_EXIT_CRITICAL_REGION();

    return(ADI_PWR_SUCCESS);
}


/**
 * @brief  Gets the system external clock frequency.
 *         Gets the clock frequency of the source connected to the external GPIO clock input source.
 *
 * @param [in]  pExtClock : Pointer to write the external clock frequency.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS          : Successfully returning the external clock frequency.
 *              - #ADI_PWR_NULL_POINTER [D] : If the given pointer is pointing to NULL.
 *              - #ADI_PWR_FAILURE [D]      : The system is not initialized yet. Call SystemInit before calling this API.
 */
ADI_PWR_RESULT adi_pwr_GetExtClkFreq (uint32_t *pExtClock)
{
#ifdef ADI_DEBUG
    /* Trap here if the app fails to set the external clock frequency. */
    if (0u == gpioClock)
    {
        return (ADI_PWR_FAILURE);
    }

    if(pExtClock == NULL)
    {
        return (ADI_PWR_NULL_POINTER);
    }
#endif
    *pExtClock = gpioClock;
    return  ADI_PWR_SUCCESS;
}


/*!
 * @brief      Get the frequency of the given clock.
 *             Obtain individual peripheral clock frequencies
 *
 * @param[in]  eClockId : Clock identifier
 * @param[out] pClock   : Pointer to a location to store the clock frequency.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS                      : Successfully returned the queried clock.
 *              - #ADI_PWR_SYSTEM_NOT_INITIALIZED [D]   : The system is not initialized yet. Call SystemInit before calling this API.
 *
 * @sa adi_PWR_SetClockDivide
 * @sa SystemSetClockDivider
*/
ADI_PWR_RESULT adi_pwr_GetClockFrequency (const ADI_CLOCK_ID eClockId, uint32_t *pClock )
{
    uint32_t src, nDiv;

#ifdef ADI_DEBUG
    /* trap here if the app fails to call SystemInit(). */
    if ((0u == hfClock) || (0u == lfClock))
    {
        return ADI_PWR_SYSTEM_NOT_INITIALIZED;
    }
#endif

    /* refresh internal clock variables */
    SystemCoreClockUpdate();
    src = hfClock;

    switch (eClockId) {

        /* HCLOCK domain */
        case ADI_CLOCK_HCLK:
            nDiv = (pADI_CLKG0_CLK->CTL1 & BITM_CLKG_CLK_CTL1_HCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_HCLKDIVCNT;
            break;

        /* PCLOCK domain */
        case ADI_CLOCK_PCLK:
            nDiv = (pADI_CLKG0_CLK->CTL1 & BITM_CLKG_CLK_CTL1_PCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_PCLKDIVCNT;
            break;

        default:
            return ADI_PWR_INVALID_CLOCK_ID;
    } /* end switch */

    if(nDiv == 0u)
    {
        nDiv = 1u;
    }

    *pClock =  (src/nDiv);

    return ADI_PWR_SUCCESS;
}


/*!
    @brief      Enable/disable individual peripheral clocks.

    @param[in]  eClockGate       Clock identifier
    @param[in]  bEnable          Flag to indicate whether to enable/disable individual clock.
                                 true  - to enable individual clock.
                                 false - to disable individual clock.

    @return     Status
                    - #ADI_PWR_SUCCESS  if we have successfully enabled or disabled the clock.

    @details    Manage individual peripheral clock gates to enable or disable the clocks to the peripheral.
*/
ADI_PWR_RESULT adi_pwr_EnableClock (const ADI_CLOCK_GATE eClockGate, const bool bEnable)
{
    uint32_t mask;
    ADI_INT_STATUS_ALLOC();

    mask = (uint16_t)eClockGate;
    /* update the Clock Gate register in a critical region */
    ADI_ENTER_CRITICAL_REGION();

        /* NOTE NEGATIVE LOGIC!!! */
        if (bEnable == true) {

        /* clear disable bit */
            pADI_CLKG0_CLK->CTL5 &= ~mask;
        } else {
            /* set disable bit */
            pADI_CLKG0_CLK->CTL5 |= mask;
        }

    /* end critical region */
    ADI_EXIT_CRITICAL_REGION();

    return ADI_PWR_SUCCESS;
}


/*!
    @brief      Sets the clock divide factor for an individual clock group.

    @param[in]  eClockId   Clock domain identifier.
    @param[in]  nDiv       Clock divide value to be set (right-justified uint16_t).

    @return     Status
                    - #ADI_PWR_SUCCESS                      if successfully set the given clock divide factor.
                    - #ADI_PWR_INVALID_CLOCK_DIVIDER [D]    if the divider is out of range.
                    - #ADI_PWR_INVALID_CLOCK_ID [D]         if the given clock is invalid.
                    - #ADI_PWR_INVALID_CLOCK_RATIO [D]      if the given clock ratio invalid.

    @details    Manage individual peripheral clock dividers.

    @sa         SystemGetClockFrequency
*/
ADI_PWR_RESULT adi_pwr_SetClockDivider (const ADI_CLOCK_ID eClockId, const uint16_t nDiv)
{
    uint32_t mask;
    uint32_t value;
    uint32_t tmp;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    uint32_t hdiv, pdiv;
#endif  /*ADI_DEBUG*/

    switch (eClockId)
    {
        case ADI_CLOCK_HCLK:
#ifdef ADI_DEBUG
            /* Verify the divide factor is within the range */
            if ((nDiv > CLOCK_MAX_DIV_VALUE) || (nDiv < CLOCK_MIN_DIV_VALUE))
            {
                return ADI_PWR_INVALID_CLOCK_DIVIDER;
            }

            /* verify PCLK freq is <= requested HCLK */
            pdiv = (pADI_CLKG0_CLK->CTL1 & BITM_CLKG_CLK_CTL1_PCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_PCLKDIVCNT;
            hdiv = nDiv;
            if (hdiv > pdiv) {
                return ADI_PWR_INVALID_CLOCK_SPEED;
            }

            /* verify new PDIV:HDIV ratio will be integral */
            if ((pdiv % hdiv) != 0u) 
            {
                return ADI_PWR_INVALID_CLOCK_RATIO;
            }
#endif  /*ADI_DEBUG*/

            mask = BITM_CLKG_CLK_CTL1_HCLKDIVCNT;
            value = (uint32_t)nDiv << BITP_CLKG_CLK_CTL1_HCLKDIVCNT;
            break;

        case ADI_CLOCK_PCLK:
#ifdef ADI_DEBUG

            /* Verify the divide factor is within the range */
            if ((nDiv > CLOCK_MAX_DIV_VALUE) || (nDiv < CLOCK_MIN_DIV_VALUE))
            {
                return ADI_PWR_INVALID_CLOCK_DIVIDER;
            }

            /* verify requested PCLK freq is <= HCLK */
            pdiv = nDiv;
            hdiv = (pADI_CLKG0_CLK->CTL1 & BITM_CLKG_CLK_CTL1_HCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_HCLKDIVCNT;
            if (hdiv > pdiv) {
                return ADI_PWR_INVALID_CLOCK_SPEED;
            }

            /* verify new PDIV:HDIV ratio will be integral */
            if ((pdiv % hdiv) != 0u) 
            {
                return ADI_PWR_INVALID_CLOCK_RATIO;
            }
#endif  /*ADI_DEBUG*/
            mask = BITM_CLKG_CLK_CTL1_PCLKDIVCNT;
            value = (uint32_t)nDiv << BITP_CLKG_CLK_CTL1_PCLKDIVCNT;
            break;

        case ADI_CLOCK_ACLK:
#ifdef ADI_DEBUG
            /* Verify the divide factor is within the range */
            if ((nDiv > ACLK_MAX_DIV_VALUE) || (nDiv < ACLK_MIN_DIV_VALUE))
            {
                return ADI_PWR_INVALID_CLOCK_DIVIDER;
            }

            /* verify requested ACLK freq is <= HCLK */
            pdiv = nDiv;
            hdiv = (pADI_CLKG0_CLK->CTL1 & BITM_CLKG_CLK_CTL1_HCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_HCLKDIVCNT;
            if (hdiv > pdiv) {
                return ADI_PWR_INVALID_CLOCK_SPEED;
            }

            /* verify new PDIV:HDIV ratio will be integral */
            if ((pdiv % hdiv) != 0u)
            {
                return ADI_PWR_INVALID_CLOCK_RATIO;
            }
#endif  /*ADI_DEBUG*/

            mask = BITM_CLKG_CLK_CTL1_ACLKDIVCNT;
            value = (uint32_t)nDiv << BITP_CLKG_CLK_CTL1_ACLKDIVCNT;
            break;


        default:
            return ADI_PWR_INVALID_CLOCK_ID;
    } /* end switch */

    /* critical region */
    ADI_ENTER_CRITICAL_REGION();

    /* read-modify-write without any interrupts */
    /* change in a tmp variable and write entire new value all at once */
    tmp = pADI_CLKG0_CLK->CTL1;
    tmp &= ~mask;                       /* blank the field */
    tmp |= value;                       /* set the new value */
    pADI_CLKG0_CLK->CTL1 = tmp;         /* write the new value */

    /* end critical region */
    ADI_EXIT_CRITICAL_REGION();

    /* refresh internal clock variables */
    SystemCoreClockUpdate();

    return ADI_PWR_SUCCESS;
}

/*!
 * @brief       To Enable/disable clock sources.
 *
 * @param[in]   eClockSource :    Clock source identifier.
 * @param[in]   bEnable      :    Enable (true) or disable (false) the clock source.
 *
 * @return      Status
 *                  - #ADI_PWR_SUCCESS          if the clock source powers up successfully.
 *                  - #ADI_PWR_INVALID_PARAM    if the clock source is not valid.
 *
 * @details    Enables or disables clock sources without additional checks, by writing a "1" or "0" to the enable bit.
 *
 */
ADI_PWR_RESULT adi_pwr_EnableClockSource (const ADI_CLOCK_SOURCE_ID eClockSource, const bool bEnable)
{
    uint32_t val = 0u;
    volatile uint32_t *pReg = NULL;
    uint32_t nMask = 0u;
    ADI_INT_STATUS_ALLOC();

    /* This switch statement does not handle every value in the ADI_CLOCK_SOURCE_ID enumeration
     * which results on a gcc warning. This is done intentionally:
     * ADI_CLOCK_SOURCE_LFOSC is not checked  because it is enabled always and it cannot be disabled
     * ADI_CLOCK_SOURCE_GPIO is only checked if a specific configuration macro is defined
     */
    switch(eClockSource)
    {
        case ADI_CLOCK_SOURCE_HFXTAL:
            val     =   (1u << BITP_CLKG_OSC_CTL_HFX_EN);
            pReg    =   &pADI_CLKG0_OSC->CTL;
            nMask   =   BITM_CLKG_OSC_CTL_HFX_OK;
            break;

        case ADI_CLOCK_SOURCE_LFXTAL:
            val     =   (1u << BITP_CLKG_OSC_CTL_LFX_EN);
            pReg    =   &pADI_CLKG0_OSC->CTL;
            nMask   =   BITM_CLKG_OSC_CTL_LFX_OK;
            break;

        case ADI_CLOCK_SOURCE_HFOSC:
            val     =   (1u << BITP_CLKG_OSC_CTL_HFOSC_EN);
            pReg    =   &pADI_CLKG0_OSC->CTL;
            nMask   =   BITM_CLKG_OSC_CTL_HFOSC_OK;
            break;

        case ADI_CLOCK_SOURCE_SPLL:
            val     =   (1u << BITP_CLKG_CLK_CTL3_SPLLEN);
            pReg    =   &pADI_CLKG0_CLK->CTL3;
            nMask   =   BITM_CLKG_CLK_CTL3_SPLLEN;
            break;

#if (ADI_PWR_CFG_ENABLE_CLOCK_SOURCE_GPIO == 1)
        case ADI_CLOCK_SOURCE_GPIO:
            if(adi_gpio_PullUpEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_10,false) != ADI_GPIO_SUCCESS)
            {
               return(ADI_PWR_FAILURE);
            }
            if(adi_gpio_InputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_10,true) != ADI_GPIO_SUCCESS)
            {
               return ADI_PWR_SUCCESS;
            }
            break;
#endif

        default:
            return(ADI_PWR_INVALID_PARAM);

    } /* end switch */

    ADI_ENTER_CRITICAL_REGION();

        pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
        if (bEnable == true)
        {
            *pReg |= val;
        }
        else
        {
            *pReg &= ~val;
        }

    ADI_EXIT_CRITICAL_REGION();

    if((nMask !=0u) && (bEnable == true))
    {
        while(0u== (pADI_CLKG0_OSC->CTL & nMask)){}
    }

    return (ADI_PWR_SUCCESS);
}


/*!
 * @brief       Return the status of a clock source.
 *
 * @param[in]    eClockSource  : Clock source identifier.
 * @param[out]   peStatus      : Pointer to variable of type  #ADI_CLOCK_SOURCE_STATUS for storing clock source status.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS               if the clock source is disabled.
 *              - #ADI_PWR_NULL_POINTER [D]      if the given pointer is pointing to NULL.

 *  @details    Return the status of a clock source.
 *
 */
ADI_PWR_RESULT adi_pwr_GetClockStatus (const ADI_CLOCK_SOURCE_ID eClockSource, ADI_CLOCK_SOURCE_STATUS *peStatus)
{
    uint32_t val = pADI_CLKG0_OSC->CTL;

#ifdef ADI_DEBUG
    if(peStatus == NULL)
    {
        return ADI_PWR_NULL_POINTER;
    }
#endif /* ADI_DEBUG */

    *peStatus = ADI_CLOCK_SOURCE_DISABLED;

    switch(eClockSource)
    {
        case ADI_CLOCK_SOURCE_HFOSC:
            if ((val & BITM_CLKG_OSC_CTL_HFOSC_EN) != 0u)
            {
                /* Clock source enabled, now check for stable */
                if ((val & BITM_CLKG_OSC_CTL_HFOSC_OK) != 0u)
                {
                    *peStatus = ADI_CLOCK_SOURCE_ENABLED_STABLE;
                }
                else
                {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
                }
            }
            break;

        case ADI_CLOCK_SOURCE_HFXTAL:
            if ((val & BITM_CLKG_OSC_CTL_HFX_EN) != 0u)
            {
                /* Clock source enabled, now check for stable */
                if ((val & BITM_CLKG_OSC_CTL_HFX_OK) != 0u)
                {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_STABLE;
                }
                else
                {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
                }
            }
            break;

        case ADI_CLOCK_SOURCE_LFXTAL:
            if ((val & BITM_CLKG_OSC_CTL_LFX_EN) != 0u)
            {
                /* Clock source enabled, now check for stable */
                if ((val & BITM_CLKG_OSC_CTL_LFX_OK) != 0u)
                {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_STABLE;
                }
                else
                {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
                }
            }
            break;

        case ADI_CLOCK_SOURCE_LFOSC:
            /* Clock source enabled, now check for stable */
            if ((val & BITM_CLKG_OSC_CTL_LFOSC_OK) != 0u)
            {
                *peStatus  = ADI_CLOCK_SOURCE_ENABLED_STABLE;
            }
            else
            {
                *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
            }
            break;

        /* Since the clock through GPIO is supplied externally we cannot get
           the clock status for GPIO */
        case ADI_CLOCK_SOURCE_GPIO:
        default:
            *peStatus  =  ADI_CLOCK_SOURCE_ID_NOT_VALID;
             break;

    } /* end switch */

    return ADI_PWR_SUCCESS;
}

/*!
 * @brief       Enable/Disable the clock interrupt to monitor status of LFXTAL, HFXTAL and PLL.
 *
 * @param[in]   eIrq    : Specify which interrupt need to be enable/disabled.
   @param[in]   bEnable : Specifies to enable/disable the specified interrupt.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Enabled the specified interrupt.
 *
 * @sa adi_pwr_SetVoltageRange()
 */

ADI_PWR_RESULT adi_pwr_EnableClockInterrupt(const ADI_PWR_CLOCK_IRQ eIrq, const bool bEnable)
{
    ADI_INT_STATUS_ALLOC();
    volatile uint32_t *pReg = NULL;
    uint32_t tmp;

    switch(eIrq)
    {
#if defined(__ADUCM4x50__)      
        /*! Interrupt for root clock monitor and Clock Fail */
        case ADI_PWR_ROOT_CLOCK_MON_IEN:
            pReg = &pADI_CLKG0_OSC->CTL;
            break;
#endif /* __ADUCM4x50__ */
            
        /*! Interrupt for LFXTAL clock monitor and Clock Fail */
        case ADI_PWR_LFXTAL_CLOCK_MON_IEN:
            pReg = &pADI_CLKG0_OSC->CTL;
            break;

        /*! Interrupt when LFXTAL clock becomes stable/unstable */
        case ADI_PWR_LFXTAL_STATUS_IEN:
            pReg = &pADI_CLKG0_CLK->CTL0;
            break;

        /*! Interrupt when HFXTAL clock becomes stable/unstable */
        case ADI_PWR_HFXTAL_STATUS_IEN:
            pReg = &pADI_CLKG0_CLK->CTL0;
            break;

        /*! Interrupt when PLL-LOCK/PLL-UNLOCK  */
        case ADI_PWR_PLL_STATUS_IEN:
            pReg = &pADI_CLKG0_CLK->CTL3;
            break;

        default:
            break;
    }

    ADI_ENTER_CRITICAL_REGION();

    tmp = *pReg;

    if(bEnable == true)
    {
        tmp |= (uint32_t)eIrq;
    }
    else
    {
        tmp &= ~((uint32_t)eIrq);
    }

    /* If we have to write to oscillator control register unlock it */
    if(pReg == &pADI_CLKG0_OSC->CTL)
    {
        pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
    }
    *pReg = tmp;

    ADI_EXIT_CRITICAL_REGION();

    return(ADI_PWR_SUCCESS);
}


/*!
 * @brief       Program PLL frequency.
 *
 * @param[in]   nDivFactor   PLL divider(M).
 * @param[in]   nMulFactor   PLL Multiplier(N)
 * @param[in]   bDiv2        PLL DIV2 parameter.
 * @param[in]   bMul2        PLL DIV2 parameter.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS                    if the PLL has been programmed successfully.
 *              - #ADI_PWR_OPERATION_NOT_ALLOWED [D]  if trying to program SPLL and SPLL drives the system clock.
 *              - #ADI_PWR_INVALID_CLOCK_ID [D]       if the clock identifier does not match either PLL.
 *
 *  @details    Program PLL frequency (parameters M, N, DIV2) forSystem PLL(SPLL).
 *
 *              SPLL = input clock * ["(N *  (1+ bMul2 )" / "((1+bDiv2)*M)" ]
 *              where input clock can be HFOSC or HFXTAL.
 */
ADI_PWR_RESULT adi_pwr_SetPll(uint8_t  nDivFactor, const uint8_t  nMulFactor, const bool bDiv2, const bool bMul2)
{
    uint32_t                val, cfg = 0u;
    uint8_t nTempDivFactor = nDivFactor, nTempMulFactor = nMulFactor;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    /* Check if multiplication factor and division factor is more than 6 bits */
    if (((nMulFactor & ~0x3Fu) != 0u) || ((nDivFactor & ~0x3Fu) != 0u))
    {
        return ADI_PWR_INVALID_CLOCK_DIVIDER;
    }

    /* Check if the PLL is multipexed in as root clock source, parameters should not change in that case */
    if((pADI_CLKG0_CLK->CTL0 & BITM_CLKG_CLK_CTL0_CLKMUX) ==
       ((uint32_t)((ADI_CLOCK_MUX_ROOT_SPLL - ADI_CLOCK_MUX_ROOT_HFOSC) << BITP_CLKG_CLK_CTL0_CLKMUX))) 
    {
         return ADI_PWR_OPERATION_NOT_ALLOWED;
    }
#endif

     if(nTempDivFactor < MINIMUM_PLL_DIVIDER)
     {
         nTempDivFactor = MINIMUM_PLL_DIVIDER;
     }
     if(nTempMulFactor < MINIMUM_PLL_MULTIPLIER)
     {
         nTempMulFactor = MINIMUM_PLL_MULTIPLIER;
     }

    cfg =  (((uint32_t)nTempDivFactor) << BITP_CLKG_CLK_CTL3_SPLLMSEL)|( ((uint32_t) nTempMulFactor) << BITP_CLKG_CLK_CTL3_SPLLNSEL);

    if(bDiv2 == true)
    {
      cfg |= (1u <<BITP_CLKG_CLK_CTL3_SPLLDIV2);
    }
    if(bMul2 == true)
    {
      cfg |= (1u <<BITP_CLKG_CLK_CTL3_SPLLMUL2);
    }

    /* critical region */
    ADI_ENTER_CRITICAL_REGION();

    val = pADI_CLKG0_CLK->CTL3;
    val &= ~( BITM_CLKG_CLK_CTL3_SPLLMUL2 | BITM_CLKG_CLK_CTL3_SPLLMSEL | BITM_CLKG_CLK_CTL3_SPLLDIV2 | BITM_CLKG_CLK_CTL3_SPLLNSEL);
    val |= cfg;
    pADI_CLKG0_CLK->CTL3 = val;

   /* end critical region */
    ADI_EXIT_CRITICAL_REGION();

    return ADI_PWR_SUCCESS;
}


/*!
 * @brief       Enable/Disable the power management interrupt.
 *
 * @param[in]   eIrq    : Specify which interrupt need to be enable/disabled.
   @param[in]   bEnable : Specifies to enable/disable  the interrupt.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS      Enabled the specified interrupt.
 *              - #ADI_PWR_FAILURE [D]  Enabling the battery monitoring interrupt when range is set to safe range (VBAT > 2.75 ).
 *
 * @note : User should configure the appropriate voltage range before enabling the interrupt for battery voltage range.
 *
 * @sa adi_pwr_SetVoltageRange()
 */
ADI_PWR_RESULT adi_pwr_EnablePMGInterrupt(const ADI_PWR_PMG_IRQ eIrq, const bool bEnable)
{
   ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
   if(((pADI_PMG0->IEN & BITM_PMG_IEN_RANGEBAT) == 0u) || (eIrq != ADI_PWR_BATTERY_VOLTAGE_RANGE_IEN))
   {
       return(ADI_PWR_FAILURE);
   }
#endif

  ADI_ENTER_CRITICAL_REGION();
  if(bEnable == true)
  {
    pADI_PMG0->IEN  |= (uint32_t)eIrq;
  }
  else
  {
    pADI_PMG0->IEN  &= ~(uint32_t)(eIrq);
  }
  ADI_EXIT_CRITICAL_REGION();

  return(ADI_PWR_SUCCESS);
}



/*!
 * @brief       Enable/disable LFXTAL bypass mode.
 *
   @param[in]   bEnable : Specifies to enable/disable  the LFXTAL bypass mode
 *\n                      true: To enable LFXTAL bypass mode.
 * \n                     false: To disable LFXTAL bypass mode.
 * @return      Status
 *                     - #ADI_PWR_SUCCESS     Enabled/Disabled LFXTAL bypass mode.
 *                     - #ADI_PWR_FAILURE[D]  Failed to Enable/Disable LFXTAL bypass mode.
 *
 */
ADI_PWR_RESULT  adi_pwr_EnableLFXTALBypass(const bool bEnable)
{
    volatile uint32_t nDelay = 0xFFFFFFu;
    if(bEnable == true)
    {
        /* Write the oscillator key */
        pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
        /* Disable the LFXTAL */
        pADI_CLKG0_OSC->CTL  &= ~(BITM_CLKG_OSC_CTL_LFX_EN);
        /* Wait till status de-asserted. */
        while(nDelay != 0u)
        {
            if((pADI_CLKG0_OSC->CTL & BITM_CLKG_OSC_CTL_LFX_OK) == 0u)
            {
                break;
            }
            nDelay--;
        }
#ifdef ADI_DEBUG
        if(nDelay == 0u)
        {
            return(ADI_PWR_FAILURE);
        }
#endif
        pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
        /* Enable the BYPASS mode */
        pADI_CLKG0_OSC->CTL  |= (BITM_CLKG_OSC_CTL_LFX_BYP);
        /* Wait till status asserted. */
        nDelay = 0xFFFFFFu;
        while(nDelay != 0u)
        {
            if(((pADI_CLKG0_OSC->CTL & BITM_CLKG_OSC_CTL_LFX_OK)== BITM_CLKG_OSC_CTL_LFX_OK))
            {
                break;
            }
            nDelay--;
        }
#ifdef ADI_DEBUG
        if(nDelay == 0u)
        {
            return(ADI_PWR_FAILURE);
        }
#endif

    }
    else
    {
        /* Write the oscillator key */
        pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
        /* Disable  the BYPASS mode */
        pADI_CLKG0_OSC->CTL  &= ~(BITM_CLKG_OSC_CTL_LFX_BYP);
        /* Wait till status de-asserted. */
        while(nDelay != 0u)
        {
            if((pADI_CLKG0_OSC->CTL & BITM_CLKG_OSC_CTL_LFX_OK) == 0u)
            {
                break;
            }
            nDelay--;
        }
#ifdef ADI_DEBUG
        if(nDelay == 0u)
        {
            return(ADI_PWR_FAILURE);
        }
#endif
    }

    return(ADI_PWR_SUCCESS);
}


#if defined(__ADUCM4x50__)
/*!
 * @brief       Enables or disables the LFXTAL Robust mode.
 *              The Robust mode enables the LFXTAL oscillator to work also when an additional resistive
 *              load is placed between the crystal pins and GND. This feature is capable of tolerating
 *              the presence of impurities on the PCB board, where these impurities allow a high-resistance
 *              leakage path from the crystal pins to ground, which can cause problems to the circuit operation
 *
 *  @param[in]  bEnable  :  Flag which indicates whether to enable or disable LFXTAL Robust mode.
                            true    - Enable Robust mode.
                            false   - Disable Robust mode.
 *  @return     Status
 *                      - #ADI_PWR_SUCCESS     Enabled/Disabled LFXTAL Robust mode.
 *
 * @sa adi_pwr_SetLFXTALRobustModeLoad()
 */
ADI_PWR_RESULT adi_pwr_EnableLFXTALRobustMode( const bool bEnable )
{
    /* Write the oscillator key */
    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;

    if(bEnable == true)
    {
        pADI_CLKG0_OSC->CTL |= BITM_CLKG_OSC_CTL_LFX_ROBUST_EN;
    }
    else
    {
        pADI_CLKG0_OSC->CTL &= ~(BITM_CLKG_OSC_CTL_LFX_ROBUST_EN);
    }

    return(ADI_PWR_SUCCESS);
}

/*!
 * @brief       Enable/Disable the LFXTAL Fail Auto switch.
 *              Enables/Disable automatic Switching of the LF Mux to LF OSC on LF XTAL Failure.
 *
 *  @param[in]  bEnable   :  Flag which indicates whether to enable/disable LFXTAL Auto switch.
 *                           true  - Enable LFXTAL Auto switch.
 *                           false - Disable LFXTAL Auto switch.
 *  @return     Status
 *                - #ADI_PWR_SUCCESS     Enabled/Disabled LFXTAL Auto switch mode.
 */
ADI_PWR_RESULT adi_pwr_EnableLFXTALFailAutoSwitch( const bool  bEnable )
{
    /* Write the oscillator key */
    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;

    if(bEnable == true)
    {
        pADI_CLKG0_OSC->CTL  |= BITM_CLKG_OSC_CTL_LFX_AUTSW_EN;
    }
    else
    {
        pADI_CLKG0_OSC->CTL  &= ~(BITM_CLKG_OSC_CTL_LFX_AUTSW_EN);
    }
    return(ADI_PWR_SUCCESS);
}


/*!
 * @brief       Sets the LFXT Robust Mode Load.
 *              Selects the amount of loading tolerated when LFXTAL robust mode is enabled.
 *
 *  @param[in]  eLoad   :  Amount of loading tolerance required.
 *  @return     Status
 *                 - #ADI_PWR_SUCCESS     Successfully set the load tolerance for LFXTAL Robust mode.
 *
 * @sa adi_pwr_EnableLFXTALRobustMode()
 */
ADI_PWR_RESULT adi_pwr_SetLFXTALRobustModeLoad( const ADI_PWR_LFXTAL_LOAD eLoad )
{
    uint32_t tmp;

    tmp =   pADI_CLKG0_OSC->CTL & ~BITM_CLKG_OSC_CTL_LFX_ROBUST_LD;
    tmp |=  ((uint32_t)eLoad) << BITP_CLKG_OSC_CTL_LFX_ROBUST_LD;

    /* Write the oscillator key */
    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;
    pADI_CLKG0_OSC->CTL = tmp;

    return(ADI_PWR_SUCCESS);
}

/*!
 * @brief       To enable/disable auto switching of root clock to HFOSC upon detection of Root clock failure.
 *              This feature is valid only when the ROOT clock monitor is enabled. The root clock monitoring
 *              can be enabled by using the API #adi_pwr_EnableClockInterrupt.
 *
 *  @param[in]  bEnable   :  Flag which indicates whether to enable or disable Root clock auto switch.
 *                           true  - Enable Root clock auto switch.
                             false - Disable Root clock auto switch.
 *  @return     Status
 *                 - #ADI_PWR_SUCCESS     Successfully set the load tolerance for LFXTAL Robust mode.
 *
 * @sa adi_pwr_EnableClockInterrupt()
 */
ADI_PWR_RESULT adi_pwr_EnableRootClockFailAutoSwitch( const bool bEnable )
{
    /* Write the oscillator key */
    pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;

    if(bEnable == true)
    {
        pADI_CLKG0_OSC->CTL |= BITM_CLKG_OSC_CTL_ROOT_AUTSW_EN;
    }
    else
    {
        pADI_CLKG0_OSC->CTL &= ~(BITM_CLKG_OSC_CTL_ROOT_AUTSW_EN);
    }

    return(ADI_PWR_SUCCESS);
}


/*!
 * @brief       Sets the HF Oscillator divide factor.
 *
 *              Sets the divide factor for the clocks derived from the HF oscillator clock.
 *
 * @param[in]  eDivFactor :  HF Clock divide factor to be set.
 *
 * @return     Status
 *                 - #ADI_PWR_SUCCESS   Successfully set the clock divide factor for HF Oscillator.
 *
 * @note        When the HF Oscillator auto divide by 1 is set, the divide factor set is automatically
 *              changed to 1 when coming out of Flexi mode. Application should set it back to the
 *              required divide after coming out of Flexi mode.
 *
 * @sa adi_pwr_EnableHFOscAutoDivBy1()
 */
ADI_PWR_RESULT adi_pwr_SetHFOscDivFactor( const ADI_PWR_HFOSC_DIV  eDivFactor )
{
    uint32_t tmp;

    tmp =  (pADI_CLKG0_CLK->CTL2 & ~BITM_CLKG_CLK_CTL2_HFOSCDIVCLKSEL);
    tmp |= ((uint32_t) eDivFactor << BITP_CLKG_CLK_CTL2_HFOSCDIVCLKSEL);
    pADI_CLKG0_CLK->CTL2 = tmp;

    return(ADI_PWR_SUCCESS);
}


/*!
 * @brief       Enable or disable the HF oscillator automatic divide by 1 during wakeup from Flexi mode.
 *
 *              This is used to enable/disable the fast wakeup from Flexi power mode. When the fast wakeup
 *              from Flexi mode is enabled, the frequency undivided 26MHz HF oscillator clock itself will
 *              be used during the wake up. The undivided HFOSC clock is selected automatically by setting
 *              the HF oscillator divide factor to 1. This updated divided by 1 clock selection will remain
 *              same until the new divider value is set.
 *
 *              When disabled the HF Oscillator divide factor will remain unchanged during the wakeup.
 *
 * @param[in]  bEnable :  Flag which indicates whether HF oscillator automatic divide by 1 is enabled/disabled.
 *                        'true'  - To enable automatic divide by 1.
 *                        'false' - To disable automatic divide by 1.
 *
 * @return     Status
 *                 - #ADI_PWR_SUCCESS   Successfully enable/disabled HF Oscillator automatic divide by 1.
 *
 * @sa adi_pwr_SetHFOscDivFactor()
 */
ADI_PWR_RESULT adi_pwr_EnableHFOscAutoDivBy1( const bool bEnable )
{
    if(bEnable == true)
    {
        pADI_CLKG0_CLK->CTL2 |= BITM_CLKG_CLK_CTL2_HFOSCAUTODIV_EN;
    }
    else
    {
        pADI_CLKG0_CLK->CTL2 &= ~(BITM_CLKG_CLK_CTL2_HFOSCAUTODIV_EN);
    }

    return(ADI_PWR_SUCCESS);
}


/*!
 * @brief       Set the clock output through the GPIO.
 *
 * @param[in]   eClockOutput : Clock to be output through the GPIO pin.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Successfully set the GPIO clock output.
 */
ADI_PWR_RESULT  adi_pwr_SetGPIOClockOutput( const ADI_CLOCK_OUTPUT_ID eClockOutput )
{
    uint32_t tmp;

    tmp =  (pADI_CLKG0_CLK->CTL0 & ~BITM_CLKG_CLK_CTL0_CLKOUT);
    tmp |= ((uint32_t)eClockOutput << BITP_CLKG_CLK_CTL0_CLKOUT);
    pADI_CLKG0_CLK->CTL0 = tmp;

    return(ADI_PWR_SUCCESS);
}

/*!
 * @brief       Enable or disable the HPBuck Low Power mode.
 *              The HPBUCK Low Power mode can be selected, when the Chip is in Flexi Power mode
 *              and low power modules such as Timer, Beeper only are enabled.
 *
 * @param[in]   bEnable : Flag which indicates whether to enable or disable HPBuck low power mode.
 *                        'true'  - Enable HPBuck low power mode.
 *                        'false' - Disable HPBuck low power mode.
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Successfully enabled or disabled the HPBuck low power mode.
 */
ADI_PWR_RESULT adi_pwr_EnableHPBuckLowPowerMode( const bool bEnable )
{
  if(bEnable == true)
  {
    pADI_PMG0->CTL1  |= BITM_PMG_CTL1_HPBUCK_LOWPWR_MODE;
  }
  else
  {
    pADI_PMG0->CTL1  &= ~(BITM_PMG_CTL1_HPBUCK_LOWPWR_MODE);
  }

  return(ADI_PWR_SUCCESS);
}

/*!
 * @brief  Set the HP Buck load mode.
 *
 *         HP Buck load mode can be set based on the system load.
 *         The low load mode can be set when the system is running below 26Mhz.
 *         The High load mode can be set when the system is running at greater than 26Mhz.
 *
 * @param[in]   eLoadMode : Load mode to be set.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Successfully set the load mode.
 */
ADI_PWR_RESULT adi_pwr_SetHPBuckLoadMode( const ADI_PWR_HPBUCK_LD_MODE eLoadMode )
{
  if(eLoadMode == ADI_PWR_HPBUCK_LD_MODE_HIGH)
  {
    pADI_PMG0->CTL1  |= BITM_PMG_CTL1_HPBUCK_LD_MODE;
  }
  else
  {
    pADI_PMG0->CTL1  &= ~(BITM_PMG_CTL1_HPBUCK_LD_MODE);
  }

  return(ADI_PWR_SUCCESS);
}
#endif /* ADUCM4x50 */

/*!
 * @brief       Enables or disables the HP Buck.
 *
 * @param[in]   bEnable : Flag which indicates whether to enable or disable HPBuck
 *                        'true'  - To enable HPBuck.
 *                        'false' - To disable HPBuck.
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Successfully enabled or disabled HPBUCK successfully.
 */
ADI_PWR_RESULT  adi_pwr_EnableHPBuck(const bool bEnable)
{
  if(bEnable == true)
  {
    pADI_PMG0->CTL1  |= BITM_PMG_CTL1_HPBUCKEN;
  }
  else
  {
    pADI_PMG0->CTL1  &= ~(BITM_PMG_CTL1_HPBUCKEN);
  }

  return(ADI_PWR_SUCCESS);
}

/*!
 * @brief       Function to retrieve the wakeup from shut down mode status.
 *
 * @param[in]   peStatus    : Pointer to #ADI_PWR_WAKEUP_STATUS for returning the wakeup status.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS: Successfully returned the shut down status.
 */
ADI_PWR_RESULT adi_pwr_GetWakeUpStatus(ADI_PWR_WAKEUP_STATUS  *peStatus)
{
    *peStatus =(ADI_PWR_WAKEUP_STATUS) pADI_PMG0->SHDN_STAT;
    return(ADI_PWR_SUCCESS);
}


/*!
 * @brief      To  Monitor voltage range of battery.
 *
 * @param[in]   eRange    : Specify the voltage range for the battery.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS: Successfully programmed battery range.
 *  @details
 *
 */
ADI_PWR_RESULT adi_pwr_SetVoltageRange(const ADI_PWR_VOLTAGE_RANGE eRange)
{
    uint32_t tmp;

    tmp = (pADI_PMG0->IEN & ~BITM_PMG_IEN_RANGEBAT);
    tmp |= ((uint32_t)eRange << BITP_PMG_IEN_RANGEBAT);
    pADI_PMG0->IEN = tmp;

    return(ADI_PWR_SUCCESS);
}

/*! \cond PRIVATE */

/*
 * Interrupt handler for PLL interrupts.
 */
void PLL_Int_Handler(void)
{
     ISR_PROLOG();

    /* As the same status word is shared between two interrupts
       Crystal_osc_Int_Handler and PLL_Int_Handler
       check and clear status bits handled in this handler */
     uint32_t nStatus = (pADI_CLKG0_CLK->STAT0 &
                        (BITM_CLKG_CLK_STAT0_SPLLUNLK | BITM_CLKG_CLK_STAT0_SPLLLK));

     /* If a callback is registered notify the events */
     if(gpfCallbackFunction != NULL)
     {
        if((nStatus & BITM_CLKG_CLK_STAT0_SPLLUNLK ) != 0u)
        {
            /* PLL unlock event */
            gpfCallbackFunction( gpPowcbParam, ADI_PWR_EVENT_PLLC_UNLOCK,(void *)0);
        }
        else if((nStatus & BITM_CLKG_CLK_STAT0_SPLLLK) != 0u)
        {
            /* PLL lock event */
            gpfCallbackFunction( gpPowcbParam, ADI_PWR_EVENT_PLLC_LOCK,(void *)0);
        }
        else
        {
           /* Do nothing */
        }
     }

     /* Clear the status bits */
     pADI_CLKG0_CLK->STAT0 = nStatus;

     ISR_EPILOG();
}

/*
 * Interrupt handler for oscillator interrupts.
 */
void Crystal_osc_Int_Handler(void)
{
     ISR_PROLOG();

     /* As the same status word is shared between two interrupts
        Crystal_osc_Int_Handler and PLL_Int_Handler
        check and clear status bits handled in this handler */
     uint32_t nClkStatus = (pADI_CLKG0_CLK->STAT0 &
                         (BITM_CLKG_CLK_STAT0_HFXTALNOK |
                          BITM_CLKG_CLK_STAT0_HFXTALOK  |
                          BITM_CLKG_CLK_STAT0_LFXTALOK  |
                          BITM_CLKG_CLK_STAT0_LFXTALNOK));
#if defined(__ADUCM4x50__)
     /* Check if the interrupt was generated due to failure in Root Clock or LFXTAL */
     uint32_t nOscStatus = (pADI_CLKG0_OSC->CTL &   (BITM_CLKG_OSC_CTL_LFX_FAIL_STA   |
                                                     BITM_CLKG_OSC_CTL_ROOT_FAIL_STA  |
                                                     BITM_CLKG_OSC_CTL_ROOT_AUTSW_STA |
                                                     BITM_CLKG_OSC_CTL_LFX_AUTSW_STA ));
#endif /* __ADUCM4x50__ */

     uint32_t nEvent = 0u;


     if(gpfCallbackFunction != NULL)
     {
         /* Is the interrupt caused due to HFXTAL or LFXTAL status */
         if(nClkStatus != 0u)
         {
                  if ((nClkStatus & BITM_CLKG_CLK_STAT0_HFXTALNOK) != 0u) { nEvent |= ADI_PWR_EVENT_OSC_HFXTAL_CLOCK_NO_OK;  }
             else if ((nClkStatus & BITM_CLKG_CLK_STAT0_HFXTALOK)  != 0u) { nEvent |= ADI_PWR_EVENT_OSC_HFXTAL_CLOCK_OK; }
             else if ((nClkStatus & BITM_CLKG_CLK_STAT0_LFXTALOK)  != 0u) { nEvent |= ADI_PWR_EVENT_OSC_LFXTAL_CLOCK_OK; }
             else if ((nClkStatus & BITM_CLKG_CLK_STAT0_LFXTALNOK) != 0u) { nEvent |= ADI_PWR_EVENT_OSC_LFXTAL_CLOCK_NO_OK; }
             else { /* do nothing */ }

             if(nEvent != 0u)  { gpfCallbackFunction( gpPowcbParam, nEvent, (void *)0u); }

         }
#if defined(__ADUCM4x50__)         
         /* Or is the interrupt caused due to Root Clock or LFXTAL failure status */
         else if(nOscStatus != 0u)
         {
            /* Did the LFXTAL failed */
            if( (nOscStatus & BITM_CLKG_OSC_CTL_LFX_FAIL_STA) != 0u)
            {
                /* Notifiy LFXTAL failure */
                gpfCallbackFunction( gpPowcbParam, ADI_PWR_EVENT_OSC_LFXTAL_MON_FAIL, (void *)0u);

                /* Did the HW auto switched to LFOSC due to LFXTAL failure */
                if((nOscStatus & BITM_CLKG_OSC_CTL_LFX_AUTSW_STA) != 0u)
                {
                   /* Notify about the auto switch to LFOSC */
                   gpfCallbackFunction( gpPowcbParam, ADI_PWR_EVENT_OSC_LFXTAL_AUTO_SWITCH, (void *)0u);
                }
            }
            /* Did the root clock failed */
            else if((nOscStatus & BITM_CLKG_OSC_CTL_ROOT_FAIL_STA) != 0u)
            {
               /* Indicate about the root clock failure */
               gpfCallbackFunction( gpPowcbParam, ADI_PWR_EVENT_OSC_ROOT_CLOCK_MON_FAIL, (void *)0u);

               /* Did the HW auto switched to HFOSC due to root clock failure */
               if((nOscStatus & BITM_CLKG_OSC_CTL_ROOT_AUTSW_STA) != 0u)
               {
                  /* Notify about auto switch to HFOSC */
                  gpfCallbackFunction( gpPowcbParam, ADI_PWR_EVENT_OSC_ROOT_CLOCK_FAIL_AUTO_SWITCH, (void *)0u);
               }
            }
            else
            {
               /* Do nothing */
            }
         }
         else
         {
            /* Do nothing */
         }
#endif /* __ADUCM4x50__ */         
     }

    /* Clear the staus bits */
    if(nClkStatus != 0u)
    {
      pADI_CLKG0_CLK->STAT0 = nClkStatus;
    }
#if defined(__ADUCM4x50__)    
    else if(nOscStatus  != 0u)
    {
        /* Write the oscillator key to clear the status bits */
        pADI_CLKG0_OSC->KEY = ADI_OSC_KEY;

        /* Clear only status bits */
        pADI_CLKG0_OSC->CTL |= nOscStatus;
    }
    else
    {
        /* Do nothing */
    }
#endif /* __ADUCM4x50__ */

    ISR_EPILOG();
}

/*
 * Interrupt handler for battery voltage interrupt.
 */
void Battery_Voltage_Int_Handler(void)
{
    ISR_PROLOG();
    uint32_t nStatus = pADI_PMG0->PSM_STAT;

     if ((nStatus & BITM_PMG_PSM_STAT_VBATUNDR) != 0u)
     {
     if(gpfCallbackFunction != NULL)
     {
             gpfCallbackFunction( gpPowcbParam, (uint32_t)nStatus, (void *)0);
     }
         pADI_PMG0->PSM_STAT |= (BITM_PMG_PSM_STAT_VBATUNDR);
     }
    ISR_EPILOG();
}

/*
 * Interrupt handler for battery voltage interrupt.
 */
void Vreg_over_Int_Handler(void)
{
    ISR_PROLOG();
     uint32_t nStatus = pADI_PMG0->PSM_STAT;

     if(gpfCallbackFunction != NULL)
     {
        if ((nStatus & BITM_PMG_PSM_STAT_VREGOVR) != 0u)
        {
            gpfCallbackFunction(gpPowcbParam, (uint32_t)ADI_PWR_EVENT_VREG_OVER_VOLTAGE, NULL);
        }
        if ((nStatus & BITM_PMG_PSM_STAT_VREGUNDR) != 0u)
        {
            gpfCallbackFunction(gpPowcbParam, (uint32_t)ADI_PWR_EVENT_VREG_UNDER_VOLTAGE, NULL);
        }
     }
     pADI_PMG0->PSM_STAT |= (nStatus &(BITM_PMG_PSM_STAT_VREGOVR | BITM_PMG_PSM_STAT_VREGUNDR));
    ISR_EPILOG();
}

/*! \endcond */
/*!
    @brief      Puts the processor into given low power mode.

    @param[in]  PowerMode          One of the ADI_PWR_POWER_MODE enum values, defining the specific
                                   low-power modes to use.

    @param[in,out] pnInterruptOccurred
                                   Control parameter selection low-power operation.  Either a NULL pointer
                                   for automatic hardware-based sleeping between interrupts, or a pointer
                                   to uint32_t for software looping sleep between interrupts.

                                   If a pointer to uint32_t is passed in, the integer must be \b 0 on entry,
                                   and will be set to \b 0 on exit.

                                   When a NULL is passed, it means the application wants the low-power
                                   implementation to use the automatic "sleep-on-exit" hardware sleep
                                   mode in which wakeup interrupts are dispatched and then automatically
                                   put the processor back to sleep on exit.  All interrupts execute the
                                   same WFI instruction (no looping) under hardware control, which results
                                   in a faster re-sleep than the software mode.

                                   When a non-NULL value is passed, it is interpreted as a pointer to a
                                   shared integer application control variable allowing the wake-up
                                   interrupts to control whether/when the control loop should re-sleep the
                                   processor as each interrupt exits.  Any interrupt that sets the variable
                                   will cause the sleep loop to exit.  Otherwise, exiting interrupts will
                                   cause the core to re-sleep until the variable is set.  Each interrupt executes
                                   a different WFI instruction inside a software loop (slower re-sleep).

    @param[in]  PriorityMask       A right-justified (un shifted) wakeup interrupt priority mask, corresponding
                                   to the programmable interrupt priority encoding scheme defined by the Cortex
                                   NVIC controller.  The \a PriorityMask value blocks interrupts with an equal
                                   or lower priority than the specified level, such that only higher-priority
                                   interrupts (less in numerical value) than the priority mask awake the
                                   processor.  A zero-valued \a PriorityMask disables interrupt masking.

    @return     Status
                    - #ADI_PWR_SUCCESS                    If successfully put the processor into low power mode.
                    - #ADI_PWR_INVALID_PARAM[D]           PriorityMask contains unimplemented hardware bits.



    Puts the processor into a low-power mode with interrupt-based wakeup(s).  Applications specify the low-power
    mode, a pointer to an application-defined interrupt variable, and an interrupt priority mask controlling the
    interrupt priority level that may awake the processor.

    @par pnInterruptOccurred
    When NULL, the processor is automatically put back to sleep as awaking interrupts exit.  This mode employs
    the hardware "sleep-on-exit" system control register bit: SLEEPONEXIT_BIT in conjunction with the "wait-for-
    interrupt" (WFI) instruction to implement a persistent sleep mode.

    When non-Null, a software strategy is used to control sleeping.  As awakening interrupts are processed, they
    can increment the interrupt controlling variable and thereby cause the sleep mode to be exited.  Note that all 
    interrupts share a common variable and any interrupt that sets the variable will cause the sleep mode to be 
    exited.

    Use of the \a pnInterruptOccurred parameter provides a mechanism to resolve two potential hibernation trouble
    spots: 1) the inherent race between the intended wakeup interrupt and the execution of the Wait-For-Interrupt
    instruction (WFI) used to sleep the processor, and 2) unrelated interrupts (of sufficient priority)
    that may terminate the wait prematurely.

    In the first case of the race condition, the race is avoided by testing the \a pnInterruptOccurred variable prior
    to the WFI within a common critical section.  This allows the #adi_pwr_EnterLowPowerMode() implementation
    to insure the intended wakeup interrupt has not occurred already and control whether to sleep the processor.
    This insures the intended wakeup interrupt has not already occurred prior to the wait, thereby eliminating the
    race condition otherwise present.

    In the second case of an unrelated interrupt terminating the sleep prematurely, the problem is solved by
    requiring the interrupt handler(s) which is(are) intended to awake the sleeping processor to set the
    application-defined \a pnInterruptOccurred variable in their respective interrupt handler(s).  This insures only those
    interrupts that explicitly set the variable will break the sleeping processor out of the sleep cycle.  Other
    (incidental) interrupts put the processor back to sleep after the interrupt because the variable would not have been set.
    This is why there is a loop around the WFI instruction.

    The \a pnInterruptOccurred variable must be initialized to zero before first use, and this should be done
    prior to enabling any interrupt which may set it (otherwise interrupts may be missed). If this variable is
    global or static then static initialization to zero or false will be sufficient.

    The variable should only be set, from an interrupt handler, by calling adi_pwr_ExitLowPowerMode() and passing
    the variable by reference. The variable should not be assigned to directly, other than for initialization.

    #adi_pwr_EnterLowPowerMode() will always clear the variable again before returning, so it does not
    need to be cleared by user code on each use. Explicitly clearing the variable, outside of #adi_pwr_EnterLowPowerMode()
    runs the risk of missing interrupts.

    @par PriorityMask
    A zero-valued \a PriorityMask disables interrupt masking, leaving all interrupts eligible to awake the
    sleeping processor.  This means that zero-valued interrupts cannot be masked.  A non-zero \a PriorityMask
    limits interrupts that may awake the sleeping processor to those with a higher priority level (lower
    numerically) than the specified \a PriorityMask value.

    Each "programmable" peripheral interrupt has an associated priority-level register (which defaults to
    zero) within the Nested Vectored Interrupt Controller (NVIC).  The number of interrupt priority encoding
    bits is defined by constant __NVIC_PRIO_BITS and is a fixed silicon attribute configured during chip
    design.  The interrupt priority-level registers range in width from 3 to 8 bits.

    This processor uses 3-bit priority encoding, allowing priority levels ranging between 0 (the highest,
    default programmable priority) and 7 (the lowest).  For example, if the \a PriorityMask parameter is
    set to 3, only interrupts with assigned priority 0, 1, and 2 may awake the processor.  Since default
    priority of all programmable interrupts is 0, setting up maskable interrupts requires that they be
    demoted in priority (raised numerically) relative to interrupts that are intended to awake the processor.

    @note    The number of priority levels is uncorrelated with the actual number of interrupts or their position
             in the Interrupt Vector Table (IVT).  Interrupt priorities may be programmed individually.\n\n

    @note    The priority levels are actually stored in the core as a left-justified value in an 8-bit field.
             The #adi_pwr_EnterLowPowerMode() API takes care of aligning the passed \a PriorityMask value to the
             core register (BASEPRI).\n\n

    @note    The default priority level for all interrupts is zero, which implies it is impossible to mask interrupts
             with a default zero-level priority encoding.  All interrupt priorities must be managed to create meaningful
             interrupt masks for low-power wakeups, as described above.\n\n

    @warning Do not modify the BASEPRI register (used for masking interrupt priority) during interrupts that take
             the core out of low-power mode momentarily.  The BASEPRI register is saved/restored on low-power mode
             entry/exit to honor user priority requests.  Interrupt-level changes to BASEPRI will be clobbered on
             low-power exit as the saved value is restored.\n\n

    @sa      adi_pwr_ExitLowPowerMode
*/
ADI_PWR_RESULT adi_pwr_EnterLowPowerMode ( const ADI_PWR_POWER_MODE PowerMode,
                                           uint32_t volatile *      pnInterruptOccurred,
                                           const uint8_t            PriorityMask
                                         )
{
    uint32_t savedPriority;
    uint32_t scrSetBits = 0u;
    uint32_t scrClrBits = 0u;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG

    /* verify the requested priority mask bits are right-justified and don't exceed __NVIC_PRIO_BITS in width */
    if ((PriorityMask & ~((1u << __NVIC_PRIO_BITS) - 1u)) != 0u)
    {
        return ADI_PWR_INVALID_PARAM;
    }

#endif  /* ADI_DEBUG */

    /* pre-calculate the sleep-on-exit set/clear bits */
    if(NULL == pnInterruptOccurred) {
        scrSetBits |= SCB_SCR_SLEEPONEXIT_Msk;

        /* point to private control variable when in hardware (sleep-on-exit) mode */
        pnInterruptOccurred = &gnLowPowerIntOccFlag;
    }

    /* pre-calculate the deepsleep and sleep-on-exit set/clear bits */
    switch (PowerMode) {

        case ADI_PWR_MODE_ACTIVE:    /* Note: this value is a "reserved" PWRMODE register code. */
            return ADI_PWR_SUCCESS;  /* avoids the reserved value "1" being written to PWRMODE. */

        case ADI_PWR_MODE_FLEXI:  /* wfi without deepsleep or sleep-on-exit */
            scrClrBits |= (uint32_t)(BITM_NVIC_INTCON0_SLEEPDEEP | BITM_NVIC_INTCON0_SLEEPONEXIT);
            break;

        case ADI_PWR_MODE_HIBERNATE:  /* wfi with deepsleep and sleep-on-exit per pnInterruptOccurred setting */
            scrSetBits |= BITM_NVIC_INTCON0_SLEEPDEEP;

            break;

        case ADI_PWR_MODE_SHUTDOWN:  /* wfi with both deepsleep and sleep-on-exit */
            /* Note: sleep-on-exit causes WFI to never exit and wakeup is only through system reset. */
            scrSetBits |= (uint32_t)(BITM_NVIC_INTCON0_SLEEPDEEP | BITM_NVIC_INTCON0_SLEEPONEXIT);
            break;

        default:
            return ADI_PWR_INVALID_POWER_MODE;

    } /* end switch */

    /* put the power mode and system control mods, as well as the WFI loop inside a critical section */
    ADI_ENTER_CRITICAL_REGION();

        { /* these lines must be in a success-checking loop if they are not inside critical section */
            /* Uninterruptable unlock sequence */
            pADI_PMG0->PWRKEY = ADI_PMG_KEY;

            /* Clear the previous mode and set new mode */
            pADI_PMG0->PWRMOD = (uint32_t) ( ( pADI_PMG0->PWRMOD & (uint32_t) (~BITM_PMG_PWRMOD_MODE) ) | PowerMode );
        }

        /* Update the SCR (sleepdeep and sleep-on-exit bits) */
        SCB->SCR = ((SCB->SCR | scrSetBits) & ~scrClrBits);

        /* save/restore current Base Priority Level */
        savedPriority = __get_BASEPRI();

        /* assert caller's priority threshold (left-justified) */
        __set_BASEPRI((uint32_t)PriorityMask << (8u -__NVIC_PRIO_BITS));
        
        /* if we are in the software looping mode, loop on the user's variable until set */
        while (0u == *pnInterruptOccurred) {

            __DSB();  /* bus sync to insure register writes from interrupt handlers are always complete before WFI */

            /* NOTE: aggressive compiler optimizations can muck up critical timing here, so reduce if hangs are present */

            /* The WFI loop MUST reside in a critical section because we need to insure that the interrupt
               that is planned to take us out of WFI (via a call to adi_pwr_ExitLowPowerMode()) is not
               dispatched until we get into the WFI.  If that interrupt sneaks in prior to our getting to the
               WFI, then we may end up waiting (potentially forever) for an interrupt that has already occurred.
            */
            __WFI();

            /* Recycle the critical section so that other (non-wakeup) interrupts are dispatched.
               This allows *pnInterruptOccurred to be set from any interrupt context.
             */
            ADI_EXIT_CRITICAL_REGION();
            /* nop */
            ADI_ENTER_CRITICAL_REGION();

        }  /* end while */

        /* ...still within critical section... */

        (*pnInterruptOccurred)--;       /* decrement the completion variable on exit */

        /* Restore previous base priority */
        __set_BASEPRI(savedPriority);

        /* clear sleep-on-exit bit to avoid sleeping on exception return to thread level */
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

        __DSB(); /* bus sync before re-enabling interrupts */

    ADI_EXIT_CRITICAL_REGION();

    return ADI_PWR_SUCCESS;
}


/*!
 * Companion function to #adi_pwr_EnterLowPowerMode() that allows interrupts to \n
 * break out of the "FLEXI" mode in which the processor stays in \n
 * sleep while peripherals are active. \n

  @param[in,out] pnInterruptOccurred
                                   Control parameter selection low-power operation. Either a NULL pointer \n
                                   for hardware sleep-on-exit feature, or a pointer to uint32_t for software  \n
                                   looping sleep between interrupts.
    @return     Status
                    - #ADI_PWR_SUCCESS                    If successfully exited from low power mode.

 * @sa adi_pwr_EnterLowPowerMode
 */
ADI_PWR_RESULT adi_pwr_ExitLowPowerMode(uint32_t volatile * pnInterruptOccurred)
{
    ADI_INT_STATUS_ALLOC();

    /* Manage the exit depending on pnInterruptOccurred convention... */
    /* NULL pointer means we are using the hardware sleep-on-exit feature */
    /* non-NULL pointer means we are using a software looping variable top sleep */

    if (NULL == pnInterruptOccurred) {

        pnInterruptOccurred = &gnLowPowerIntOccFlag;  /* point to private control variable in hardware mode */

        /* clear hardware sleep-on-exit feature */
        ADI_ENTER_CRITICAL_REGION();

            SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
            __DSB();  /* bus sync before interrupt exit */

        ADI_EXIT_CRITICAL_REGION();
    }

    /* set control variable (whether hardware or software based) so WFI exits in SystemEnterLowPowerMode() */
    (*pnInterruptOccurred)++;
    return ADI_PWR_SUCCESS;
}

/*
** EOF
*/

/*! @} */
