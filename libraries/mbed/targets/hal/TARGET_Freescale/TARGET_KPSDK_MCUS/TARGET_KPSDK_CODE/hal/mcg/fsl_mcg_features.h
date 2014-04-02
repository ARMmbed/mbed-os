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
#ifndef __FSL_MCG_FEATURES_H__
#define __FSL_MCG_FEATURES_H__

#if (defined(CPU_MKL25Z32VFM4) || defined(CPU_MKL25Z64VFM4) || defined(CPU_MKL25Z128VFM4) || \
     defined(CPU_MKL25Z32VFT4) || defined(CPU_MKL25Z64VFT4) || defined(CPU_MKL25Z128VFT4) || \
     defined(CPU_MKL25Z32VLH4) || defined(CPU_MKL25Z64VLH4) || defined(CPU_MKL25Z128VLH4) || \
     defined(CPU_MKL25Z32VLK4) || defined(CPU_MKL25Z64VLK4) || defined(CPU_MKL25Z128VLK4))
    #define FSL_FEATURE_MCG_PLL_VDIV_BASE  (24)
    #define FSL_FEATURE_MCG_HAS_PLL_EXTRA_DIV (0)
    #define FSL_FEATURE_MCG_HAS_RTC_32K (0)
    #define FSL_FEATURE_MCG_HAS_PLL1 (0)
    #define FSL_FEATURE_MCG_HAS_IRC_48M (0)
    #define FSL_FEATURE_MCG_HAS_OSC1 (0)
    #define FSL_FEATURE_MCG_HAS_FCFTRIM (0)
    #define FSL_FEATURE_MCG_HAS_LOLRE (1)
    #define FSL_FEATURE_MCG_USE_OSCSEL (0)
    #define FSL_FEATURE_MCG_USE_PLLREFSEL (0)
    #define FSL_FEATURE_MCG_USE_SYSTEM_CLOCK (1)
#elif (defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FX512VMJ12) || \
       defined(CPU_MK70FN1M0VMJ15) || defined(CPU_MK70FX512VMJ15))
    #define FSL_FEATURE_MCG_PLL_VDIV_BASE  (16)
    #define FSL_FEATURE_MCG_HAS_PLL_EXTRA_DIV (1)
    #define FSL_FEATURE_MCG_HAS_RTC_32K (1)
    #define FSL_FEATURE_MCG_HAS_PLL1 (1)
    #define FSL_FEATURE_MCG_HAS_IRC_48M (0)
    #define FSL_FEATURE_MCG_HAS_OSC1 (1)
    #define FSL_FEATURE_MCG_HAS_FCFTRIM (0)
    #define FSL_FEATURE_MCG_HAS_LOLRE (0)
    #define FSL_FEATURE_MCG_USE_OSCSEL (1)
    #define FSL_FEATURE_MCG_USE_PLLREFSEL (1)
    #define FSL_FEATURE_MCG_USE_SYSTEM_CLOCK (0)
#elif  (defined(CPU_MK64FN1M0VMD12) || (defined(CPU_MK22FN512VDC12)))
    #define FSL_FEATURE_MCG_PLL_VDIV_BASE  (24)
    #define FSL_FEATURE_MCG_HAS_PLL_EXTRA_DIV (0)
    #define FSL_FEATURE_MCG_HAS_RTC_32K (1)
    #define FSL_FEATURE_MCG_HAS_PLL1 (0)
    #define FSL_FEATURE_MCG_HAS_IRC_48M (1)
    #define FSL_FEATURE_MCG_HAS_OSC1 (0)
    #define FSL_FEATURE_MCG_HAS_FCFTRIM (1)
    #define FSL_FEATURE_MCG_HAS_LOLRE (1)
    #define FSL_FEATURE_MCG_USE_OSCSEL (1)
    #define FSL_FEATURE_MCG_USE_PLLREFSEL (0)
    #define FSL_FEATURE_MCG_USE_SYSTEM_CLOCK (0)
#else
    #error "No valid CPU defined"
#endif

#endif /* __FSL_MCG_FEATURES_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

