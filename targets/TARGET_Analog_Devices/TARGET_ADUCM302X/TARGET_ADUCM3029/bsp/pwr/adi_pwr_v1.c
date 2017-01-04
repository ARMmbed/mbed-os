/*!
 *****************************************************************************
 * @file:    adi_pwr_v1.c
 * @brief:   Power Management service implementation.
 * @version: $Revision: 35861 $
 * @date:    $Date: 2016-11-22 09:07:07 +0000 (Tue, 22 Nov 2016) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2014 Analog Devices, Inc.

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

/*! \addtogroup Power_Service Dynamic Power Management Service
 *  @{
 */

#ifndef _ADI_PWR_V1_C_
/* \cond PRIVATE */
#define _ADI_PWR_V1_C_

#include <stdlib.h>     /* for 'NULL' */
#include <services/int/adi_int.h>
#include <adi_types.h>

#include <ssdd_common/common_def.h>
#include "adi_pwr_def_v1.h"
#include <adi_pwr_config.h>
#include <services/pwr/adi_pwr.h>
#include <services/gpio/adi_gpio.h>

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   This isn't a header as such.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
*   Needed for null expansion of ADI_INSTALL_HANDLER and others.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm140
#endif /* __ICCARM__ */

/*----------------------------------------------------------------------------
  Internal Clock Variables. The external ones are defined in system.c
 *---------------------------------------------------------------------------*/
#ifdef ADI_DEBUG
/* not needed unless its debug mode */
extern uint32_t lfClock;    /* "lf_clk" coming out of LF mux             */
#endif
extern uint32_t hfClock;    /* "root_clk" output of HF mux               */
extern uint32_t gpioClock;  /* external GPIO clock                       */

static ADI_CALLBACK  pfCallbackFunction;
static void *pPowcbParam = NULL;
static bool_t bLowPowerIntOccFlag = false;
extern uint32_t SystemCoreClock;

extern void SystemCoreClockUpdate (void);

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
    pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK;
#if (ADI_PWR_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)    
    pADI_CLKG0_OSC->CTL = OSCCTRL_CONFIG_VALUE;    
    pADI_CLKG0_OSC->CTL |= (uint32_t)ADI_PWR_LF_CLOCK_MUX;
#else
    pADI_CLKG0_OSC->CTL |= 1U << BITP_CLKG_OSC_CTL_HFOSCEN;
#endif

    pfCallbackFunction = NULL;  
    pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK;
    pADI_CLKG0_OSC->CTL |= BITM_CLKG_OSC_CTL_HFOSCEN;
    /* wait for HF OSC to stabilize */
    while ((pADI_CLKG0_OSC->CTL & (1U << BITP_CLKG_OSC_CTL_HFOSCOK)) == 0u)
    {
    }

    /* Switch over to the internal HF oscillator */
    pADI_CLKG0_CLK->CTL0 &= ~(BITM_CLKG_CLK_CTL0_CLKMUX);
#if (ADI_PWR_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
    /* complete remaining reset sequence */
    pADI_CLKG0_CLK->CTL0 = CLOCKN0_CONFIG_VALUE;
    pADI_CLKG0_CLK->CTL1 = CLOCKN1_CONFIG_VALUE;
    /* no CLKCON2 */
    pADI_CLKG0_CLK->CTL3 = CLOCKN3_CONFIG_VALUE | (uint32_t)ADI_PWR_SPLL_ENABLE;
    pADI_CLKG0_CLK->CTL5 = CLOCKN5_CONFIG_VALUE;
    pADI_PMG0->IEN  = PWM_INTERRUPT_CONFIG;
    

#endif    

    /* Change the PLL input source from the default HFOSC to HFXTAL */
    /* to satisfy the majority of use cases. It has no effect if    */
    /* no PLL is used.                                              */
    adi_pwr_SetRootClockMux(ADI_CLOCK_MUX_SPLL_HF_XTAL);

    /* disable external HF crystal oscillator */
    /* (don't disable LF crystal or the RTC will lose time */
    pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK;
    pADI_CLKG0_OSC->CTL &= ~BITM_CLKG_OSC_CTL_HFXTALEN;
    
    ADI_ENABLE_INT(PMG0_VREG_OVR_IRQn);
    ADI_ENABLE_INT(PMG0_BATT_RANGE_IRQn);
    ADI_INSTALL_HANDLER(PMG0_VREG_OVR_IRQn,Vreg_over_Int_Handler);
    ADI_INSTALL_HANDLER(PMG0_BATT_RANGE_IRQn,Battery_Voltage_Int_Handler);
    
    ADI_ENABLE_INT(CLKG_XTAL_OSC_EVT_IRQn);
    ADI_ENABLE_INT(CLKG_PLL_EVT_IRQn);
    ADI_INSTALL_HANDLER(CLKG_XTAL_OSC_EVT_IRQn,Crystal_osc_Int_Handler);
    ADI_INSTALL_HANDLER(CLKG_PLL_EVT_IRQn,PLL_Int_Handler);
    
    /* compute new internal clocks based on the newly reset controller */
    SystemCoreClockUpdate();
    
    return(ADI_PWR_SUCCESS);
}


/**
 * @brief  Updates the internal SystemCoreClocks with current core
 * Clock retrieved from cpu registers.
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Updated core system core clock variables.
 *
 * Updates the internal SystemCoreClocks with current core
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
 * @brief  To install the call back function to notify the user. 
 *
 * @param[in]  pfCallback: Callback function pointer.
 *
 * @param[in]  pcbParam:  Callback parameter.
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully installed the callback function.
 *    - #ADI_PWR_FAILURE : Failed to install the callback function since the call back function pointer is NULL.
 *
 */
ADI_PWR_RESULT adi_pwr_RegisterCallback(
               ADI_CALLBACK pfCallback,
	       void *pcbParam
	       )
{
#ifdef ADI_DEBUG
   if(pfCallback == NULL)
   {
      return(ADI_PWR_FAILURE);
   }
#endif  
    pfCallbackFunction = pfCallback;
    pPowcbParam = pcbParam;
    return ADI_PWR_SUCCESS;
}

/**
 * @brief  Sets the system external clock frequency
 *
 * @param[in]  ExtClkFreq: External clock frequency in Hz

 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully set the external clock as source. 
 *    - #ADI_PWR_INVALID_CLOCK_SPEED :  Specified clock is out of range. 
 * @sa adi_pwr_GetClockFrequency ()
 */
ADI_PWR_RESULT adi_pwr_SetExtClkFreq (uint32_t ExtClkFreq)
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
 * @brief  Set the input clock for PLL multiplexer bits.
 *
 * @param[in]  eClockID: Clock ID
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully set the PLL multiplexer.
 *    - #ADI_PWR_INVALID_CLOCK_ID : Specified clock ID is invalid.
 *
 * @sa adi_pwr_GetClockMux()
 * @sa adi_pwr_SetLFClockMux()
 * @sa adi_pwr_GetClockMux()
 */
ADI_PWR_RESULT  adi_pwr_SetPLLClockMux(ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;
    switch (eClockID) 
    {

        case ADI_CLOCK_MUX_SPLL_HF_OSC:
        case ADI_CLOCK_MUX_SPLL_HF_XTAL:
            break;
      /* Any other clock ID is not valid since we are configuring the Low frequency clock multiplexer.
       * Only valid input clock to the multiplexer is HFOSC, HFXTL */ 
	default:
	    return(ADI_PWR_INVALID_CLOCK_ID);     
 	    
    }
    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();
    
    tmp = (pADI_CLKG0_CLK->CTL0 & ~BITM_CLKG_CLK_CTL0_SPLLIPSEL);
    tmp |=(((uint32_t)eClockID-ADI_CLOCK_MUX_SPLL_HF_OSC)<<BITP_CLKG_CLK_CTL0_SPLLIPSEL);    
    pADI_CLKG0_CLK->CTL0 = tmp;  	    

    ADI_EXIT_CRITICAL_REGION();
    return(ADI_PWR_SUCCESS);
} 

/**
 * @brief  Set the input clock for low frequency clock multiplexer.
 *
 * @param[in]  eClockID: Clock ID
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully LF clock multiplexer.
 *    - #ADI_PWR_INVALID_CLOCK_ID : Specified clock ID is invalid.
 *
 * @sa adi_pwr_GetClockMux()
 * @sa adi_pwr_SetRootClockMux()
 * @sa adi_pwr_SetPLLClockMux()
 */
ADI_PWR_RESULT  adi_pwr_SetLFClockMux(ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;
    switch (eClockID) 
    {

        case ADI_CLOCK_MUX_LFCLK_LFOSC:
        case ADI_CLOCK_MUX_LFCLK_LFXTAL:
            break;
      /* Any other clock ID is not valid since we are configuring the Low frequency clock multiplexer.
       * Only valid input clock to the multiplexer is LFOSC, LFXTL */ 

	default:
	    return(ADI_PWR_INVALID_CLOCK_ID);     
 	    
    }
    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();
    pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK;    
    tmp = (pADI_CLKG0_OSC->CTL & ~BITM_CLKG_CLK_CTL0_LFXTALIE);
    tmp |=(((uint32_t)eClockID-ADI_CLOCK_MUX_LFCLK_LFOSC)<<BITP_CLKG_CLK_CTL0_LFXTALIE);    
    pADI_CLKG0_OSC->CTL = tmp;  	    

    ADI_EXIT_CRITICAL_REGION();
    return(ADI_PWR_SUCCESS);
} 

/**
 * @brief  Set the Reference clock multiplexer.
 *
 * @param[in]  eClockID: Clock ID
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully set the reference clock to multiplexer.
 *    - #ADI_PWR_INVALID_CLOCK_ID : Specified clock ID is invalid.
 *
 * @sa adi_pwr_GetClockMux()
 * @sa adi_pwr_SetLFClockMux()
 * @sa adi_pwr_SetRootClockMux()
 * @sa adi_pwr_SetPLLClockMux()
 *
 */

ADI_PWR_RESULT  adi_pwr_SetRefClockMux(ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;
    uint32_t nMux;
#ifdef ADI_DEBUG  
    switch (eClockID) 
    {

        case ADI_CLOCK_MUX_REF_HFXTL_CLK:
        case ADI_CLOCK_MUX_REF_HFOSC_CLK:
            break;
      /* Any other clock ID is not valid since we are configuring the out clock multiplexer.*/

	default:
	    return(ADI_PWR_INVALID_CLOCK_ID);     
 	    
    }
#endif    
    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();
    nMux =((uint32_t)eClockID - ADI_CLOCK_MUX_REF_HFOSC_CLK)<<1;
    tmp = (pADI_CLKG0_CLK->CTL0 & ~BITM_CLKG_CLK_CTL0_RCLKMUX);
    tmp |=((nMux)<<BITP_CLKG_CLK_CTL0_RCLKMUX);    
    pADI_CLKG0_CLK->CTL0 = tmp;  	    

    ADI_EXIT_CRITICAL_REGION();
    return(ADI_PWR_SUCCESS);
} 

/**
 * @brief  Set the root clock multiplexer.
 *
 * @param[in]  eClockID: Clock ID
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully set the root clock multiplexer.
 *    - #ADI_PWR_INVALID_CLOCK_ID : Specified clock ID is invalid.
 *
 * @sa adi_pwr_GetClockMux()
 * @sa adi_pwr_SetLFClockMux()
  * @sa adi_pwr_SetPLLClockMux()
 * 
 */
ADI_PWR_RESULT  adi_pwr_SetRootClockMux(ADI_CLOCK_MUX_ID eClockID)
{
    uint32_t tmp;

    switch (eClockID) 
    {
       case ADI_CLOCK_MUX_ROOT_HFOSC:
       case ADI_CLOCK_MUX_ROOT_HFXTAL:
       case ADI_CLOCK_MUX_ROOT_SPLL:
       case ADI_CLOCK_MUX_ROOT_GPIO:
            break;
      /* Any other clock ID is not valid since we are configuring the root clock multiplexer.
       * Only valid input clock to the multiplexer is HFOSC, HFXTL, SPLL, GPIO */ 
	default:
	    return(ADI_PWR_INVALID_CLOCK_ID);     
    }
    /* update the mux setting inside a critical region */
    ADI_ENTER_CRITICAL_REGION();
    /* read the register */ 
    tmp = (pADI_CLKG0_CLK->CTL0 & ~BITM_CLKG_CLK_CTL0_CLKMUX);
    /* Set the mux value */     
    tmp |=(((uint32_t)eClockID-ADI_CLOCK_MUX_ROOT_HFOSC)<<BITP_CLKG_CLK_CTL0_CLKMUX);    
    /* write back */
    pADI_CLKG0_CLK->CTL0 = tmp;  	    

    ADI_EXIT_CRITICAL_REGION();
    return(ADI_PWR_SUCCESS);
} 
/**
 * @brief  Retrieve the system clock multiplexer value.
 *
 * @param[in ]  eMuxId : multiplexer group identifier.
 * @param[out]  peMuxID : pointer to variable to store the clock multiplexer bits.   
 *
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully returning the system clock multiplexer information.
 *
 * @sa adi_pwr_SetClockMux()
 */
ADI_PWR_RESULT adi_pwr_GetClockMux(ADI_CLOCK_MUX_GROUP_ID eMuxId, ADI_CLOCK_MUX_ID *peMuxID)
{
    uint32_t mask;
    int32_t position;
    uint16_t offset;
    uint32_t tmp;
    volatile uint32_t *pRegister = &pADI_CLKG0_CLK->CTL0; 

    /* The following logic depends on the enumeration order being correct */
    switch (eMuxId) {

        /* SPLL clock source */
        case ADI_CLOCK_MUX_SPLL:
            mask = BITM_CLKG_CLK_CTL0_SPLLIPSEL;
            position = BITP_CLKG_CLK_CTL0_SPLLIPSEL;
            offset = ADI_CLOCK_MUX_SPLL_HF_OSC;
            break;

        /* lf_clk source */
        case ADI_CLOCK_MUX_LFCLK:
            mask = BITM_CLKG_OSC_CTL_LFCLKMUX;
            position = BITP_CLKG_OSC_CTL_LFCLKMUX;
            offset = ADI_CLOCK_MUX_LFCLK_LFOSC;
	    pRegister = &pADI_CLKG0_OSC->CTL; 
            break;

        /* Clock output clock source */
        case ADI_CLOCK_MUX_REFCLK:
            mask = BITM_CLKG_CLK_CTL0_RCLKMUX;
            position = BITP_CLKG_CLK_CTL0_RCLKMUX;
            offset = ADI_CLOCK_MUX_REF_HFOSC_CLK;
            break;

        /* root_clk source */
        case ADI_CLOCK_MUX_ROOT:
            mask = BITM_CLKG_CLK_CTL0_CLKMUX;
            position = BITP_CLKG_CLK_CTL0_CLKMUX;
            offset = ADI_CLOCK_MUX_ROOT_HFOSC;
            break;
    default:
           return(ADI_PWR_FAILURE); 
    } /* end switch */

    tmp = (*pRegister & mask) >> position;
    tmp += offset;
    *peMuxID = (ADI_CLOCK_MUX_ID)tmp;
    return (ADI_PWR_SUCCESS);
}


/**
 * @brief  Gets the system external clock frequency
 *
 * @param[in ]  pExtClock : Pointer to write the external clock frequency.
 * @return      Status
 *    - #ADI_PWR_SUCCESS : Successfully returning the external clock frequency.
 *
 * Gets the clock frequency of the source connected to the external GPIO clock input source
 */
ADI_PWR_RESULT adi_pwr_GetExtClkFreq (uint32_t *pExtClock)
{
#ifdef ADI_DEBUG
    /* trap here if the app fails to call SystemInit(). */
    if (0u == gpioClock)
    {
        return (ADI_PWR_FAILURE);
    }
#endif
    *pExtClock = gpioClock;
    return  ADI_PWR_SUCCESS;
}


/*!
    @brief      Get the specified clock.

    @param[in]  eClockId:       Clock identifier 
    @param[out]  pClock:         Pointer to a location to store the clock frequency.

    @return      Status
        - #ADI_PWR_SUCCESS : Successfully returned the queried clock.

    @details    Obtain individual peripheral clock frequencies.

    @sa         adi_PWR_SetClockDivide
    @sa         adi_PWR_GetClockDivide
    @sa         SystemSetClockDivider
*/
ADI_PWR_RESULT adi_pwr_GetClockFrequency (ADI_CLOCK_ID eClockId,uint32_t *pClock )
{

    uint32_t src, nDiv = 0u;

#ifdef ADI_DEBUG
    /* trap here if the app fails to call SystemInit(). */
    if ((0u == hfClock) || (0u == lfClock))
    {
        while(1) {}
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
    *pClock =  src/nDiv;
    return ADI_PWR_SUCCESS;
}


/*!
    @brief      Enable/disable individual peripheral clocks.

    @param[in]  eClockGateId     Clock identifier
    @param[in]  bEnable          Flag to indicate enable/disable individual clock gate.
                                 'true' to enable and 'false' to disable

    @return     Status
                    - #ADI_PWR_SUCCESS if we have successfully enabled or disabled the clock gate.
                    - #ADI_PWR_INVALID_CLOCK_ID [D] if the specified clock is invalid.

    @details    Manage individual clock gates.
*/
ADI_PWR_RESULT adi_pwr_EnableClock (ADI_CLOCK_GATE_ID eClockGateId, bool_t bEnable)
{
    uint32_t mask;
#ifdef ADI_DEBUG
    /* note: not all clocks are user-gatable... */
    switch (eClockGateId) {
        /* note: bit 6 is reserved */
        case ADI_CLOCK_GATE_GPT0_CLK:
        case ADI_CLOCK_GATE_GPT1_CLK:   
        case ADI_CLOCK_GATE_GPT2_CLK: 
        case ADI_CLOCK_GATE_I2C_CLK:
        case ADI_CLOCK_GATE_PCLK:   
             break;
        default:
            return ADI_PWR_INVALID_CLOCK_ID;
    } /* end switch */
#endif    
    mask = (uint16_t)eClockGateId;
    /* update the Clock Gate register in a critical region */
    ADI_ENTER_CRITICAL_REGION();

        /* NOTE NEGATIVE LOGIC!!! */
        if (bEnable) {
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
    @brief      System set clock divide factor for an individual clock group.

    @param[in]  eClockId   Clock domain identifier
    @param[in]  nDiv       Clock divide value to be set (right-justified uint8_t)

    @return     Status
                    - #ADI_PWR_SUCCESS if successfully set the given clock divide factor.
                    - #ADI_PWR_INVALID_CLOCK_DIVIDER [D] if the divider is out of range.
                    - #ADI_PWR_INVALID_CLOCK_ID [D] if the specified clock is invalid.
                    - #ADI_PWR_INVALID_CLOCK_RATIO [D] if the specified clock ratio invalid.

    @details    Manage individual peripheral clock dividers.

    @sa         adi_PWR_SetClockDivide
    @sa         adi_PWR_GetClockDivide
    @sa         SystemGetClockFrequency
*/
ADI_PWR_RESULT adi_pwr_SetClockDivider (ADI_CLOCK_ID eClockId, uint8_t nDiv)
{
    uint32_t mask  = 0u ;
    uint32_t value = 0u;
    uint32_t tmp;

#ifdef ADI_DEBUG
    uint32_t hdiv, pdiv;

    /* test for max 6-bit divider */
    if (((nDiv & ~0x3Fu) != 0u) || (nDiv > CLOCK_MAX_DIV_VALUE) || (nDiv < CLOCK_MIN_DIV_VALUE))
    {
        return ADI_PWR_INVALID_CLOCK_DIVIDER;
    }
#endif  /*ADI_DEBUG*/

    switch (eClockId) {
        case ADI_CLOCK_HCLK:
#ifdef ADI_DEBUG
            /* verify PCLK freq is <= requested HCLK */
            pdiv = (pADI_CLKG0_CLK->CTL1 & BITM_CLKG_CLK_CTL1_PCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_PCLKDIVCNT;
            hdiv = nDiv;
            if (hdiv > pdiv) {
                return ADI_PWR_INVALID_CLOCK_SPEED;
            }

            /* verify new PDIV:HDIV ratio will be integral */
            if (pdiv % hdiv) {
                return ADI_PWR_INVALID_CLOCK_RATIO;
            }
#endif  /*ADI_DEBUG*/
            mask = BITM_CLKG_CLK_CTL1_HCLKDIVCNT;
            value = (uint32_t)nDiv << BITP_CLKG_CLK_CTL1_HCLKDIVCNT;
            break;
      
        case ADI_CLOCK_PCLK:
#ifdef ADI_DEBUG
            /* verify requested PCLK freq is <= HCLK */
            pdiv = nDiv;
            hdiv = (pADI_CLKG0_CLK->CTL1 & BITM_CLKG_CLK_CTL1_HCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_HCLKDIVCNT;
            if (hdiv > pdiv) {
                return ADI_PWR_INVALID_CLOCK_SPEED;
            }

            /* verify new PDIV:HDIV ratio will be integral */
            if (pdiv % hdiv) {
                return ADI_PWR_INVALID_CLOCK_RATIO;
            }
#endif  /*ADI_DEBUG*/
            mask = BITM_CLKG_CLK_CTL1_PCLKDIVCNT;
            value = (uint32_t)nDiv << BITP_CLKG_CLK_CTL1_PCLKDIVCNT;
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
 *                  - #ADI_PWR_SUCCESS if the clock source powers up successfully.
 *
 *  @details    Enables or disables clock sources without additional checks, by writing a "1" or "0" to the enable bit.
 *              
 *
 */
ADI_PWR_RESULT adi_pwr_EnableClockSource (ADI_CLOCK_SOURCE_ID eClockSource, bool_t bEnable) {
    uint32_t val = 0u;
    volatile uint32_t *pReg = 0;
    uint32_t nMask = 0u;

    /* This switch statement does not handle every value in the ADI_CLOCK_SOURCE_ID enumeration
     * which results on a gcc warning. This is done intentionally:
     * ADI_CLOCK_SOURCE_LFOSC is not checked  because it is enabled always and it cannot be disabled
     * ADI_CLOCK_SOURCE_GPIO is only checked if a specific configuration macro is defined
     */
    switch(eClockSource) {

        case ADI_CLOCK_SOURCE_HFXTAL:
            val = (1u << BITP_CLKG_OSC_CTL_HFXTALEN);
            pReg = &pADI_CLKG0_OSC->CTL;
            nMask =  BITM_CLKG_OSC_CTL_HFXTALOK;
            break;

        case ADI_CLOCK_SOURCE_LFXTAL:
            val = (1u << BITP_CLKG_OSC_CTL_LFXTALEN);
            pReg = &pADI_CLKG0_OSC->CTL;
            nMask =  BITM_CLKG_OSC_CTL_LFXTALOK;
            
            break;

        case ADI_CLOCK_SOURCE_HFOSC:
            val = (1u << BITP_CLKG_OSC_CTL_HFOSCEN);
            pReg = &pADI_CLKG0_OSC->CTL;
            nMask =  BITM_CLKG_OSC_CTL_HFOSCOK;            
            break;

        case ADI_CLOCK_SOURCE_SPLL:
            val = (1u << BITP_CLKG_CLK_CTL3_SPLLEN);
            pReg = &pADI_CLKG0_CLK->CTL3;
            nMask =  BITM_CLKG_CLK_CTL3_SPLLEN;                       
            break;
#if (ADI_PWR_CFG_ENABLE_CLOCK_SOURCE_GPIO == 1)
        case ADI_CLOCK_SOURCE_GPIO:
            if(adi_gpio_PullUpEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_10,false) != ADI_GPIO_SUCCESS)
            {
                return ADI_PWR_FAILURE;
            }
            if(adi_gpio_InputEnable(ADI_GPIO_PORT1,ADI_GPIO_PIN_10,true) != ADI_GPIO_SUCCESS)
            {
                return ADI_PWR_FAILURE;
            }
            return ADI_PWR_SUCCESS;
            break; /* Required by IAR MISRA check. */
#endif   
            
    default:
        return(ADI_PWR_INVALID_PARAM);
    } /* end switch */

    /* critical region */
    ADI_ENTER_CRITICAL_REGION();

        pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK;            
        if (bEnable) {
            *pReg |= val;
        } else {
            *pReg &= ~val;
        }
    /* end critical region */
    ADI_EXIT_CRITICAL_REGION();
    if((nMask !=0u)&&(bEnable == true)) 
    {
        while(0u== (pADI_CLKG0_OSC->CTL&nMask&nMask)){} 
    }
    return (ADI_PWR_SUCCESS);

}

/*!
 * @brief       Return the status of a clock source.
 *
 * @param[in]    eClockSource  : Clock source identifier.
 * @param[out]   peStatus      : Pointer to variable of type  #ADI_CLOCK_SOURCE_STATUS for storing colck source status.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS      if the clock source is disabled.
 *              - #ADI_PWR_INVALID_CLOCK_ID if the clock source identifier is not valid.
 *
 *  @details    Return the status of a clock source.
 *
 */
ADI_PWR_RESULT adi_pwr_GetClockStatus (ADI_CLOCK_SOURCE_ID eClockSource, ADI_CLOCK_SOURCE_STATUS *peStatus) 
{
    uint32_t val = pADI_CLKG0_OSC->CTL;
    *peStatus = ADI_CLOCK_SOURCE_DISABLED;
    switch(eClockSource) {

        case ADI_CLOCK_SOURCE_HFOSC:
            if ((val & BITM_CLKG_OSC_CTL_HFOSCEN) != 0u) {
                /* Clock source enabled, now check for stable */
                if ((val & BITM_CLKG_OSC_CTL_HFOSCOK) != 0u) {
                    *peStatus = ADI_CLOCK_SOURCE_ENABLED_STABLE;
                } else {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
                }
            }
            break;

        case ADI_CLOCK_SOURCE_HFXTAL:
            if ((val & BITM_CLKG_OSC_CTL_HFXTALEN) != 0u) {
                /* Clock source enabled, now check for stable */
                if ((val & BITM_CLKG_OSC_CTL_HFXTALOK) != 0u) {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_STABLE;
                } else {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
                }
            }
            break;

        case ADI_CLOCK_SOURCE_LFXTAL:
            if ((val & BITM_CLKG_OSC_CTL_LFXTALEN) != 0u) {
                /* Clock source enabled, now check for stable */
                if ((val & BITM_CLKG_OSC_CTL_LFXTALOK) != 0u) {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_STABLE;
                } else {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
                }
            }
            break;
            
        case ADI_CLOCK_SOURCE_LFOSC:
                /* Clock source enabled, now check for stable */
                if ((val & BITM_CLKG_OSC_CTL_LFOSCOK) != 0u) {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_STABLE;
                } else {
                    *peStatus  = ADI_CLOCK_SOURCE_ENABLED_NOT_STABLE;
                }
            break;


        case ADI_CLOCK_SOURCE_GPIO:
            break;


        default:
            *peStatus  =  ADI_CLOCK_SOURCE_ID_NOT_VALID;
             break;
    } /* end switch */

    return ADI_PWR_SUCCESS;
}

/*!
 * @brief       To enable the clock interrupt to monitor status of LFXTL,HFXTL and PLL.
 *
 * @param[in]   eIrq    : Specify which interrupt need to be enable/disabled.
   @param[in]   bEnable : Specifies to enable/disable the specified interrupt.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Enabled the specified interrupt.
 *
 * @sa adi_pwr_SetVoltageRange()    
 */

ADI_PWR_RESULT adi_pwr_EnableClockInterrupt(ADI_CLOCK_IRQ eIrq, bool_t bEnable)
{
    uint16_t   nOscInterrupt = (eIrq & (ADI_PWR_OSC_LFXTL_STATUS_IEN | ADI_PWR_OSC_HFXTL_STATUS_IEN ));

    if(bEnable == true)
    {
        if(nOscInterrupt != 0u)  
        {
            pADI_CLKG0_CLK->CTL0  |= (uint32_t)eIrq;
        }
        else
        {
            pADI_CLKG0_CLK->CTL3  |= (uint32_t)eIrq;        
        }
    }
    else
    {
        if(nOscInterrupt != 0u)  
        {
            pADI_CLKG0_CLK->CTL0  &= ~((uint32_t)eIrq);
        }
        else
        {
            pADI_CLKG0_CLK->CTL3  &= ~((uint32_t)eIrq);        
        }
    }
    return(ADI_PWR_SUCCESS);
}

/*!
 * @brief       Program PLL frequency.
 *
 * @param[in]   nDivFactor   PLL divider(M).
 * @param[in]   nMulFactor   PLL Multiplier(N) 
 * @param[in]   bDiv2       PLL DIV2 parameter.
 * @param[in]   bMul2       PLL DIV2 parameter.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS if the PLL has been programmed successfully.
 *              - #ADI_PWR_OPERATION_NOT_ALLOWED if trying to program SPLL and SPLL drives the system clock.
 *              - #ADI_PWR_INVALID_CLOCK_ID if the clock identifier does not match either PLL.
 *
 *  @details    Program PLL frequency (parameters M, N, DIV2) forSystem PLL(SPLL).
 *
 *              SPLL = input clock * ["(N *  (1+ bMul2 )" / "((1+bDiv2)*M)" ]  
 *              where input clock can be HFOSC or HFXTL.
 */
ADI_PWR_RESULT adi_pwr_SetPll(uint8_t  nDivFactor, uint8_t  nMulFactor, bool_t bDiv2,bool_t bMul2) {
    uint32_t                val, cfg=0u;
#ifdef ADI_DEBUG    
    ADI_CLOCK_MUX_ID eMuxType;
    if (((nMulFactor & ~0x3Fu) != 0u) || ((nDivFactor & ~0x3Fu) != 0u))
    {
        return ADI_PWR_INVALID_CLOCK_DIVIDER;
    }
    
    /* Check if the PLL is multipexed in as clock source, parameters should not change in that case */
    if (ADI_PWR_SUCCESS == adi_pwr_GetClockMux(ADI_CLOCK_MUX_ROOT,&eMuxType)) 
    {
         return ADI_PWR_OPERATION_NOT_ALLOWED;
    }
#endif            
    
     if(nDivFactor < MINIMUM_PLL_DIVIDER)       
     {
         nDivFactor = MINIMUM_PLL_DIVIDER;
     }
     if(nMulFactor < MINIMUM_PLL_MULTIPLIER)       
     {
         nDivFactor = MINIMUM_PLL_MULTIPLIER;
     }
     
    /* critical region */
    cfg =  ((uint32_t)nDivFactor <<BITP_CLKG_CLK_CTL3_SPLLMSEL)|((uint32_t)nMulFactor<< BITP_CLKG_CLK_CTL3_SPLLNSEL);
    if(bDiv2 == true)
    {
      cfg |= (1u <<BITP_CLKG_CLK_CTL3_SPLLDIV2);
    }
    if(bMul2 == true)
    {
      cfg |= (1u <<BITP_CLKG_CLK_CTL3_SPLLMUL2);
    }
    
    ADI_ENTER_CRITICAL_REGION();
        val = pADI_CLKG0_CLK->CTL3;
        val &= ~( BITM_CLKG_CLK_CTL3_SPLLMUL2 |BITM_CLKG_CLK_CTL3_SPLLMSEL |BITM_CLKG_CLK_CTL3_SPLLDIV2|BITM_CLKG_CLK_CTL3_SPLLNSEL);
        val |= cfg;
        pADI_CLKG0_CLK->CTL3 = val;
   /* end critical region */
    ADI_EXIT_CRITICAL_REGION();

    return ADI_PWR_SUCCESS;

}
/*!
 * @brief       To enable the power service interrupt..
 *
 * @param[in]   eIrq    : Specify which interrupt need to be enable/disabled.
   @param[in]   bEnable : Specifies to enable/disable  the interrupt     
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Enabled the specified interrupt.
 *              - #ADI_PWR_FAILURE  Enabling the battery monitoring interrupt when range is set to safe range (VBAT > 2.75 ).
 *
 * @note : User should configure the appropriate voltage range before enabling the interrupt for battery voltage range. 
 *
 * @sa adi_pwr_SetVoltageRange()    
 */

ADI_PWR_RESULT adi_pwr_EnableInterrupt(ADI_PWR_IRQ eIrq, bool_t bEnable)
{
#ifdef ADI_DEBUG
   if(((pADI_PMG0->IEN & BITM_PMG_IEN_RANGEBAT) == 0u) || (eIrq != ADI_PWR_BATTERY_VOLTAGE_RANGE_IEN)) 
   {
       return(ADI_PWR_FAILURE);
   }
#endif	
  if(bEnable == true)
  {
    pADI_PMG0->IEN  |= (uint32_t)eIrq;
  }
  else
  {
    pADI_PMG0->IEN  &= ~(uint32_t)(eIrq);
  }
  return(ADI_PWR_SUCCESS);
}
/*!
 * @brief       To enable/disable LFXTL bypass mode.
 *
   @param[in]   bEnable : Specifies to enable/disable  the LFXTL bypass mode     
 *\n                      true: To enable LFXTL bypass mode.
 * \n                     false: To disable LFXTL bypass mode 
 *                     - #ADI_PWR_SUCCESS     Enabled/Disabled LFXTL bypass mode.
 *                     - #ADI_PWR_FAILURE[D]  Failed to Enable/Disable LFXTL bypass mode.
 *
 */
ADI_PWR_RESULT  adi_pwr_EnableLFXTLBypass(bool_t bEnable)
{
    volatile uint32_t nDelay = 0xFFFFFFu;
    if(bEnable == true)
    {
        /* Write the oscillator key */
        pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK;
        /* Disable the LFXTL */     
        pADI_CLKG0_OSC->CTL  &= ~(BITM_CLKG_OSC_CTL_LFXTALEN);
        /* Wait till status de-asserted. */ 
        while(nDelay != 0u)
        {
            if((pADI_CLKG0_OSC->CTL & BITM_CLKG_OSC_CTL_LFXTALOK) == 0u)
            {
                break;
            }
            nDelay=nDelay-1u;
        }
#ifdef ADI_DEBUG
        if(nDelay == 0u) 
        {
            return(ADI_PWR_FAILURE);
        }
#endif     
        pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK; 
        /* Enable the BYPASS mode */
        pADI_CLKG0_OSC->CTL  |= (BITM_CLKG_OSC_CTL_LFXTAL_BYPASS);
        /* Wait till status asserted. */
        nDelay = 0xFFFFFFu;    
        while(nDelay != 0u)
        {
            if(((pADI_CLKG0_OSC->CTL &BITM_CLKG_OSC_CTL_LFXTALOK)!=BITM_CLKG_OSC_CTL_LFXTALOK))
            {
                break;
            }
            nDelay=nDelay-1u;
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
        pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK; 
        /* Disable  the BYPASS mode */     
        pADI_CLKG0_OSC->CTL  &= ~(BITM_CLKG_OSC_CTL_LFXTAL_BYPASS);
        /* Wait till status de-asserted. */      
        while(nDelay != 0u)
        {
            if((pADI_CLKG0_OSC->CTL &BITM_CLKG_OSC_CTL_LFXTALOK) != 0u)
            {
                break;
            }
            nDelay=nDelay-1u;
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
    

/*!
 * @brief       To enable/disable LFXTL monitor interrupt.
 *
   @param[in]   bEnable : Specifies to enable/disable  the LFXTL monitor interrupt     
 *\n                      true: To enable LFXTL monitor interrupt.
 * \n                     false: To disable LFXTL monitor interrupt. 
 *                     - #ADI_PWR_SUCCESS  Enabled/Disabled LFXTL monitor interrupt.
 *
 */
ADI_PWR_RESULT  adi_pwr_EnableLFXTLMonitorInterrupt(bool_t bEnable)
{
  if(bEnable == true)
  {
     /* Write the oscillator key */
     pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK;
     /* Disable the LFXTL */     
     pADI_CLKG0_OSC->CTL  |= (BITM_CLKG_OSC_CTL_LFXTAL_MON_EN);
  }
  else
  {
     /* Write the oscillator key */  
     pADI_CLKG0_OSC->KEY = OSCKEY_UNLOCK; 
     /* Disable  the BYPASS mode */     
     pADI_CLKG0_OSC->CTL  &= ~(BITM_CLKG_OSC_CTL_LFXTAL_MON_EN);
  }
  return(ADI_PWR_SUCCESS);
}

/*!
 * @brief       To enable/disable the HP Buck.
 *
   @param[in]   bEnable : Specifies to enable/disable the HPBuck     
 * @return      Status
 *              - #ADI_PWR_SUCCESS  Enabled Hpbuck successfully.
 *
 */
ADI_PWR_RESULT  adi_pwr_EnableHPBuck(bool_t bEnable)
{
  if(bEnable == true)
  {
    pADI_PMG0->CTL1  |= BITM_PMG_CTL1_HPBUCKEN;
  }
  else
  {
    pADI_PMG0->CTL1  &=~(BITM_PMG_CTL1_HPBUCKEN);
  }
  return(ADI_PWR_SUCCESS);
}

/*!
 * @brief       Function to retrieve the wakeup from shutdown mode status. 
 *
 * @param[in]   peStatus    : Pointer to #ADI_PWR_WAKEUP_STATUS for returning the wakeup status.
 *
 * @return      Status
 *              - #ADI_PWR_SUCCESS: Successfully returned the shutdown status.
 *
 *  @details    
 *
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
 *
 *  @details    
 *
 */

ADI_PWR_RESULT adi_pwr_SetVoltageRange(ADI_PWR_VOLTAGE_RANGE eRange)
{
    pADI_PMG0->IEN |= ((uint32_t)eRange);
    return(ADI_PWR_SUCCESS);
}
/*! \cond PRIVATE */
/*
 * Interrupt handler for PLL interrupts.
 */
ADI_INT_HANDLER(PLL_Int_Handler)
{ 
     ISR_PROLOG();
     uint32_t nStatus = ( pADI_CLKG0_CLK->STAT0 & 
                         (BITM_CLKG_CLK_STAT0_SPLLUNLK |
                          BITM_CLKG_CLK_STAT0_SPLLLK )); 


     if(pfCallbackFunction != NULL)    
     {
         pfCallbackFunction( pPowcbParam,nStatus<<CLOCK_EVENT_OFFSET,(void *)0);
     }
     pADI_CLKG0_CLK->STAT0 |= nStatus;
     ISR_EPILOG();    
}

/*
 * Interrupt handler for oscillator interrupts.
 */
ADI_INT_HANDLER(Crystal_osc_Int_Handler)
{ 
     ISR_PROLOG();
     uint32_t nStatus = ( pADI_CLKG0_CLK->STAT0 & 
                         (BITM_CLKG_CLK_STAT0_HFXTALNOK |
                          BITM_CLKG_CLK_STAT0_HFXTALOK  |
                          BITM_CLKG_CLK_STAT0_LFXTALOK  |
                          BITM_CLKG_CLK_STAT0_LFXTALNOK  )); 


     if(pfCallbackFunction != NULL)    
     {
         pfCallbackFunction( pPowcbParam,nStatus<<CLOCK_EVENT_OFFSET,(void *)0);
     }
     pADI_CLKG0_CLK->STAT0 |= nStatus;
     ISR_EPILOG();    
}

/*
 * Interrupt handler for battery voltage interrupt.
 */
ADI_INT_HANDLER(Battery_Voltage_Int_Handler)
{ 
    ISR_PROLOG(); 
    uint32_t nStatus = pADI_PMG0->PSM_STAT; 
         
     if ((nStatus & BITM_PMG_PSM_STAT_VBATUNDR) != 0u)
     {
	 if(pfCallbackFunction != NULL)    
	 {
             pfCallbackFunction( pPowcbParam,(uint32_t)nStatus,(void *)0);
	 }
         pADI_PMG0->PSM_STAT |= (BITM_PMG_PSM_STAT_VBATUNDR);              	 
     }
    ISR_EPILOG();    
}

/*
 * Interrupt handler for battery voltage interrupt.
 */
ADI_INT_HANDLER(Vreg_over_Int_Handler)
{
    ISR_PROLOG();    
     uint32_t nStatus = pADI_PMG0->PSM_STAT; 
     
     if(pfCallbackFunction != NULL)
     {
        if ((nStatus & BITM_PMG_PSM_STAT_VREGOVR) != 0u)
        {
            pfCallbackFunction(pPowcbParam, (uint32_t)ADI_PWR_EVENT_OVER_VOLTAGE, NULL);
        }
        if ((nStatus & BITM_PMG_PSM_STAT_VREGUNDR) != 0u)
        {
            pfCallbackFunction(pPowcbParam, (uint32_t)ADI_PWR_EVENT_UNDER_VOLTAGE, NULL);
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

    @param[in,out] pbInterruptOccurred
                                   Control parameter selection low-power operation.  Either a NULL pointer
                                   for automatic hardware-based sleeping between interrupts, or a pointer
                                   to bool_t for software looping sleep between interrupts.

                                   If a flag pointer is passed in, the flag must be \b false on entry,
                                   and will be cleared \b false on exit.

                                   When a NULL is passed, it means the application wants the low-power
                                   implementation to use the automatic "sleep-on-exit" hardware sleep
                                   mode in which wakeup interrupts are dispatched and then automaticelly
                                   put the processor back to sleep on exit.  All interrupts execute the
                                   same WFI instruction (no looping) under hardware control, which results
                                   in a faster re-sleep than the software mode.

                                   When a non-NULL value is passed, it is interpreted as a pointer to a
                                   shared bool_t application control veriable allowing the wake-up
                                   interrupts to control whether/when the control loop should re-sleep the
                                   processor as each interrupt exits.  Any interrupt that sets the variable
                                   will cause the sleep loop to exit.  Otherwise, exiting interrupts will
                                   cause the core to re-sleep until the flag is set.  Each interrupt executes
                                   a different WFI instruction inside a software loop (slower re-sleep).

    @param[in]  PriorityMask       A right-justified (unshifted) wakeup interrupt priority mask, corresponding
                                   to the programmable interrupt priority encoding scheme defined by the Cortex
                                   NVIC controller.  The \a PriorityMask value blocks interrupts with an equal
                                   or lower priority than the specified level, such that only higher-priority
                                   interrupts (less in numerical value) than the priority mask awake the
                                   processor.  A zero-valued \a PriorityMask disables interrupt masking.

    @return     Status
                    - #ADI_PWR_SUCCESS                    If successfully put the processor into low power mode.
                    - #ADI_PWR_INVALID_PARAM[D]           PriorityMask contains unimplemented hardware bits.



    Puts the processor into a low-power mode with interrupt-based wakeup(s).  Applications specify the low-power
    mode, a pointer to an application-defined interrupt flag, and an interrupt priority mask controlling the
    interrupt priority level that may awake the processor.

    @par pbInterruptOccurred
    When NULL, the processor is automatically put back to sleep as awaking interrupts exit.  This mode employs
    the hardware "sleep-on-exit" system control register bit: SLEEPONEXIT_BIT in conjunction with the "wait-for-
    interrupt" (WFI) instruction to implement a persistant sleep mode.

    When non-Null, a software strategy is used to control sleeping.  As awakening interrupts are processed, they
    can set the flag true and thereby cause the sleep mode to be exited.  Note that all interrupts share a
    common flag and any interrupt that sets the flag will cause the sleep mode to be exited.

    Use of the \a pbInterruptOccurred parameter provides a mechanism to resolve two potential hibernation trouble
    spots: 1) the inherent race between the intended wakeup interrupt and the execution of the Wait-For-Interrupt
    instruction (WFI) used to sleep the processor, and 2) unrelated interrupts (of sufficient priority)
    that may terminate the wait prematurely.

    In the first case of the race condition, the race is avoided by testing the \a pbInterruptOccurred flag prior
    to the WFI within a common critical section.  This allows the #adi_pwr_EnterLowPowerMode() implementation
    to insure the intended wakeup interrupt has not occurred already and control whether to sleep the processor.
    This insures the intended wakeup interrupt has not already occurred prior to the wait, thereby eliminating the
    race condition otherwise present.

    In the second case of an unrelated interrupt terminating the sleep prematurely, the problem is solved by
    requiring the interrupt handler(s) which is(are) intended to awake the sleeping processor to set the
    application-defined \a pbInterruptOccurred flag in their respective interrupt handler(s).  This insures only those
    interrupts that explicitly set the flag will break the sleeping processor out of the sleep cycle.  Other
    (incidental) interrupts put the processor back to sleep after the interrupt because the flag would not have been set.
    This is why there is a loop around the WFI instruction.

    The \a pbInterruptOccurred flag must be initialized to zero before first use, and this should be done
    prior to enabling any interrupt which may set it (otherwise interrupts may be missed). If this flag is
    a global or a static variable then static initialization to zero or false will be sufficient.

    The flag should only be set, from an interrupt handler, by calling adi_pwr_ExitLowPowerMode() and passing
    the flag variable by reference. The flag should not be assigned to directly, other than for initialization.

    #adi_pwr_EnterLowPowerMode() will always clear the flag again before returning, so the variable does not
    need to be cleared by user code on each use. Explicitly clearing the flag, outside of #adi_pwr_EnterLowPowerMode()
    runs the risk of missing interrupts.

    @par PriorityMask
    A zero-valued \a PriorityMask disables interrupt masking, leaving all interrupts eligible to awake the
    sleeping processor.  This means that zero-valued interrupts cannot be masked.  A non-zero \a PriorityMask
    limits interrupts that may awake the sleeping processor to those with a higher priority level (lower
    numerically) than the specified \a PriorityMask value.

    Each "programmable" peripheral interrupt has an associated priority-level register (which defaults to
    zero) within the Nested Vectored Interrupt Controller (NVIC).  The number of interrupt priority encoding
    bits is defined by constant #__NVIC_PRIO_BITS and is a fixed silicon attribute configured during chip
    design.  The interrupt priority-level registers range in width from 3 to 8 bits.

    The processor uses 3-bit priority encoding, allowing priority levels ranging between 0 (the highest,
    default programmable priority) and 7 (the lowest).  For example, if the \a PriorityMask parameter is
    set to 3, only interrupts with assigned priority 0, 1, and 2 may awake the processor.  Since default
    priority of all programmable interrupts is 0, setting up maskable interrupts requires that they be
    demoted in priority (raised numerically) relative to interrupts that are intended to awake the processor.

    @note    The number of priority levels is uncorrelated with the actual number of interrupts or their position
             in the Interrupt Vector Table (IVT).  Interrupt priorities may be programmed individually.

    @note    The priority levels are actually stored in the core as a left-justified value in an 8-bit field.
             The #adi_pwr_EnterLowPowerMode() API takes care of aligning the passed \a PriorityMask value to the
             core register (BASEPRI).

    @note    The default priority level for all interrupts is zero implies it is impossible to mask interrupts
             with a default zero-level priority encoding.

    @sa      adi_pwr_ExitLowPowerMode
*/
ADI_PWR_RESULT adi_pwr_EnterLowPowerMode ( const ADI_PWR_POWER_MODE PowerMode,
                                           bool_t volatile *        pbInterruptOccurred,
                                           const uint8_t            PriorityMask
                                         )
{
    uint32_t savedPriority;
    uint16_t savedWDT;
    uint32_t scrSetBits = 0u;
    uint32_t scrClrBits = 0u;

#ifdef ADI_DEBUG

    /* verify the requested priority mask bits are right-justified and don't exceed __NVIC_PRIO_BITS in width */
    if ((PriorityMask & ~((1u << __NVIC_PRIO_BITS) - 1u)) != 0u)
    {
        return ADI_PWR_INVALID_PARAM;
    }

#endif  /* ADI_DEBUG */

    /* pre-calculate the sleep-on-exit set/clear bits */
    if(NULL == pbInterruptOccurred) {
        scrSetBits |= SLEEPONEXIT_BIT;

        /* point to private control variable when in hardware mode */
        pbInterruptOccurred = &bLowPowerIntOccFlag;
    }

    /* pre-calculate the deepsleep set/clear bits */
    switch (PowerMode) {
        case ADI_PWR_MODE_ACTIVE:
            return ADI_PWR_SUCCESS;
#ifdef __ICCARM__
            break; /* required by IAR MISRA check */
#endif
        case ADI_PWR_MODE_FLEXI:
        case ADI_PWR_MODE_SHUTDOWN:
            scrClrBits |= SLEEPDEEP_BIT;
            break;

        case ADI_PWR_MODE_HIBERNATE:
            scrSetBits |= SLEEPDEEP_BIT;
            break;

        default:
            return ADI_PWR_INVALID_POWER_MODE;
    } /* end switch */

    /* put all the power mode and system control mods inside a critical section */
    ADI_ENTER_CRITICAL_REGION();

        { /* these three lines must be in a success-checking loop if they are not inside critical section */
            /* Uninterruptable unlock sequence */
            pADI_PMG0->PWRKEY = ADI_PMG_KEY;

            /* Clear the previous mode and set new mode */
            pADI_PMG0->PWRMOD =(uint32_t) ( ( pADI_PMG0->PWRMOD & (uint32_t) (~BITM_PMG_PWRMOD_MODE) ) | PowerMode );
        }

        /* Update the SCR (sleepdeep and sleep-on-exit bits) */
        SCB->SCR = ((SCB->SCR | scrSetBits) & ~scrClrBits);

        /* save current Base Priority Level */
        savedPriority = __get_BASEPRI();

        /* NOTE: the watchdog timer (WDT) of the GlueMicro (ADuCM302x) is reset
           by the core hardware with every exit from low-power mode.  Therefore,
           even though we may have disabled it during startup, it will reset
           itself on exit from every hibernation state.  Therefore, to avoid
           unintended system resets every 30 seconds because of unexpected WDT
           timeouts, we save/restore the WDT control register around
           hibernation entry and exit.
        */

        /* save WDT control register */
        savedWDT = pADI_WDT0->CTL;

        /* Set caller's priority threshold (left-justified) */
        __set_BASEPRI((uint32_t)PriorityMask << (8u -__NVIC_PRIO_BITS));

        /* if we are in the software looping mode, loop on the user's flag until set */
        while (!*pbInterruptOccurred) {

            /* SAR-51938: insure WDT is fully synchronized or hibernate may lock out the sync bits */
            while ((pADI_WDT0->STAT & (BITM_WDT_STAT_COUNTING | BITM_WDT_STAT_LOADING | BITM_WDT_STAT_CLRIRQ)) != 0u)
            {
            }

            __DSB();  /* bus sync to insure register writes from interrupt handlers are always complete before WFI */

			/* NOTE: aggressive compiler optimizations can muck up critical timing here, so reduce if hangs are present */

            /* Wait for interrupt */
            __WFI();

            /* Recycle critical section so that interrupts are dispatched.  This
             * allows *pbInterruptOccurred to be set during interrupt handling.
             */
            ADI_EXIT_CRITICAL_REGION();
            /* nop */	
            ADI_ENTER_CRITICAL_REGION();

        }  /* end while */

        /* ...still within critical section... */

        *pbInterruptOccurred = false; /* reset the completion flag on exit */

        /* Restore previous base priority */
        __set_BASEPRI(savedPriority);

        /* restore WDT control register */
        pADI_WDT0->CTL = savedWDT;

        /* clear sleep-on-exit bit to avoid sleeping on exception return to thread level */
        SCB->SCR &= ~SLEEPONEXIT_BIT;

        __DSB(); /* bus sync before re-enabling interrupts */

    ADI_EXIT_CRITICAL_REGION();

    return ADI_PWR_SUCCESS;
}


/*!
 * Companion function to #adi_pwr_EnterLowPowerMode() that allows interrupts to \n
 * break out of the "FLEXI" mode in which the processor stays in \n
 * sleep while peripherals are active. \n
 
  @param[in,out] pbInterruptOccurred
                                   Control parameter selection low-power operation.  Either a NULL pointer \n
                                   for hardware sleep-on-exit feature, or a pointer  to bool_t for software  \n
                                   looping sleep between interrupts.
    @return     Status
                    - #ADI_PWR_SUCCESS                    If successfully  exit from  into low power mode.
                                   
 * @sa adi_pwr_EnterLowPowerMode
 */
ADI_PWR_RESULT adi_pwr_ExitLowPowerMode(bool_t volatile * pbInterruptOccurred) 
{
    /* Manage the exit depending on pbInterruptOccurred flag convention... */
    /* NULL pointer means we are using the hardware sleep-on-exit feature */
    /* non-NULL pointer means we are using a software looping flag top sleep */

    if (NULL == pbInterruptOccurred) {

        pbInterruptOccurred = &bLowPowerIntOccFlag;  /* point to private control variable in hardware mode */

        /* clear hardware sleep-on-exit feature */
        ADI_ENTER_CRITICAL_REGION();

            SCB->SCR &= ~SLEEPONEXIT_BIT;
            __DSB();  /* bus sync before interrupt exit */

        ADI_EXIT_CRITICAL_REGION();
    }

    /* set control variable (whether hardware or software based) so WFI exits in SystemEnterLowPowerMode() */
    *pbInterruptOccurred = true;
    return ADI_PWR_SUCCESS;
}

/*
** EOF
*/

#endif /* _ADI_PWR_V1_C_ */
/*! @} */
