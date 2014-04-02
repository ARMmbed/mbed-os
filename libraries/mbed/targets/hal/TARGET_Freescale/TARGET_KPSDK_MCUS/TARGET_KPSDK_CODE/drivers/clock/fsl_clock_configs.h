/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if !defined(__FSL_CLOCK_CONFIGS_H__)
#define __FSL_CLOCK_CONFIGS_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @addtogroup clock_manager*/
/*! @{*/

#if defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FN1M0VMJ15)

#define CLOCK_SETUP     1

/*! @brief Predefined clock setups for K70*/

/* 
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 41.94MHz, BusClock = 41.94MHz
   1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         Reference clock source for MCG module is an external reference clock source 50MHz
         Core clock = 120MHz, BusClock = 60MHz
   2 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power External (BLPE) mode
         Core clock/Bus clock derived directly from an external reference clock source 50MHz with no multiplication
         Core clock = 50MHz, BusClock = 50MHz
*/


/*! @brief   Define clock source values for K70*/

#if (CLOCK_SETUP == 0)
    #define CPU_XTAL0_CLK_HZ                50000000u  /* external crystal or osc clock frequency in Hz connected to System Oscillator 0*/
    #define CPU_XTAL1_CLK_HZ                8000000u   /* external crystal or osc clock frequency in Hz connected to System Oscillator 1*/
    #define CPU_XTAL32k_CLK_HZ              32768u     /* Value of the external 32k crystal or oscillator clock frequency in Hz*/
    #define CPU_INT_SLOW_CLK_HZ             32768u     /* Value of the slow internal oscillator clock frequency in Hz*/
    #define CPU_INT_FAST_CLK_HZ             4000000u   /* Value of the fast internal oscillator clock frequency in Hz*/
    #define DEFAULT_SYSTEM_CLOCK            41943040u  /* Default System clock value*/
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL0_CLK_HZ                50000000u  /* external crystal or osc clock frequency in Hz connected to System Oscillator 0*/
    #define CPU_XTAL1_CLK_HZ                8000000u   /* external crystal or osc clock frequency in Hz connected to System Oscillator 1*/
    #define CPU_XTAL32k_CLK_HZ              32768u     /* Value of the external 32k crystal or oscillator clock frequency in Hz*/
    #define CPU_INT_SLOW_CLK_HZ             32768u     /* Value of the slow internal oscillator clock frequency in Hz*/
    #define CPU_INT_FAST_CLK_HZ             4000000u   /* Value of the fast internal oscillator clock frequency in Hz*/
    #define DEFAULT_SYSTEM_CLOCK            120000000u /* Default System clock value*/
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL0_CLK_HZ                50000000u  /* external crystal or osc clock frequency in Hz connected to System Oscillator 0*/
    #define CPU_XTAL1_CLK_HZ                8000000u   /* external crystal or osc clock frequency in Hz connected to System Oscillator 1*/
    #define CPU_XTAL32k_CLK_HZ              32768u     /* Value of the external 32k crystal or oscillator clock frequency in Hz*/
    #define CPU_INT_SLOW_CLK_HZ             32768u     /* Value of the slow internal oscillator clock frequency in Hz*/
    #define CPU_INT_FAST_CLK_HZ             4000000u   /* Value of the fast internal oscillator clock frequency in Hz*/
    #define DEFAULT_SYSTEM_CLOCK            50000000u  /* Default System clock value*/
#endif /* (CLOCK_SETUP == 2)*/

#elif defined(CPU_MKL25Z128VLK4)

#define CLOCK_SETUP     1

/*! @brief Predefined clock setups for KL25*/

/* 
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 41.94MHz, BusClock = 13.98MHz
   1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         Reference clock source for MCG module is an external crystal 8MHz
         Core clock = 48MHz, BusClock = 24MHz
   2 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power External (BLPE) mode
         Core clock/Bus clock derived directly from an external crystal 8MHz with no multiplication
         Core clock = 8MHz, BusClock = 8MHz
*/


/*! @brief  Define clock source values for KL25*/

#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 8000000u  /* external crystal or oscillator clock frequency in Hz*/
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* slow internal oscillator clock frequency in Hz*/
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* fast internal oscillator clock frequency in Hz*/
    #define DEFAULT_SYSTEM_CLOCK            41943040u /* Default System clock value*/
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 8000000u  /* external crystal or oscillator clock frequency in Hz*/
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* slow internal oscillator clock frequency in Hz*/
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* fast internal oscillator clock frequency in Hz*/
    #define DEFAULT_SYSTEM_CLOCK            48000000u /* Default System clock value*/
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL_CLK_HZ                 8000000u  /* external crystal or oscillator clock frequency in Hz*/
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* slow internal oscillator clock frequency in Hz*/
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* fast internal oscillator clock frequency in Hz*/
    #define DEFAULT_SYSTEM_CLOCK            8000000u  /* Default System clock value*/
#endif /*(CLOCK_SETUP == 2)*/

#elif defined(CPU_MK64FN1M0VMD12) 

#define DISABLE_WDOG    1

#define CLOCK_SETUP     1
/* Predefined clock setups
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 20.97MHz, BusClock = 20.97MHz
   1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         Reference clock source for MCG module is an external clock source 50MHz
         USB clock divider is set for USB to receive 48MHz input clock.
         Core clock = 120MHz, BusClock = 60MHz
   3 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power Internal (BLPI) mode
         Core clock/Bus clock derived directly from an fast internal clock 4MHz with no multiplication
         The clock settings is ready for Very Low Power Run mode.
         Core clock = 4MHz, BusClock = 4MHz
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 50000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define CPU_INT_IRC_CLK_HZ              48000000u /* Value of the 48M internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            20485760u /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 50000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define CPU_INT_IRC_CLK_HZ              48000000u /* Value of the 48M internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            120000000u /* Default System clock value */
#elif (CLOCK_SETUP == 3)
    #define CPU_XTAL_CLK_HZ                 50000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define CPU_INT_IRC_CLK_HZ              48000000u /* Value of the 48M internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            4000000u /* Default System clock value */
#endif /* (CLOCK_SETUP == 3) */

#elif defined(CPU_MK22FN512VDC12)

#define DISABLE_WDOG    1

#define CLOCK_SETUP     1
/* Predefined clock setups
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 41.94MHz, BusClock = 41.94MHz
   1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         Reference clock source for MCG module is an external crystal 8MHz
         Core clock = 100MHz, BusClock = 50MHz
   2 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power External (BLPE) mode
         Core clock/Bus clock derived directly from an external crystal 8MHz with no multiplication
         Core clock = 8MHz, BusClock = 8MHz
   3 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         Reference clock source for MCG module is an external crystal 8MHz
         Core clock = 120MHz, BusClock = 60MHz
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define CPU_INT_IRC_CLK_HZ              48000000u /* Value of the 48M internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            41943040u /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define CPU_INT_IRC_CLK_HZ              48000000u /* Value of the 48M internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            100000000u /* Default System clock value */
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define CPU_INT_IRC_CLK_HZ              48000000u /* Value of the 48M internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            8000000u /* Default System clock value */
#elif (CLOCK_SETUP == 3)
    #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define CPU_INT_IRC_CLK_HZ              48000000u /* Value of the 48M internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            120000000u /* Default System clock value */
#endif /* (CLOCK_SETUP == 3) */

#endif /* CPU types*/

/*! @}*/

#endif /* __FSL_CLOCK_CONFIGS_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

