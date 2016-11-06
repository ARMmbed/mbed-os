/* 
 * Copyright (c) 2016 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
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
 * 
 */

#ifndef _BTLE_CLOCK_H_
#define _BTLE_CLOCK_H_

#include "nrf5x_lf_clk_helper.h"

/**
 * @brief Module that generates settings for the low-frequency (LF) clock configuration.
 *
 * This module provides macros that are generated from the mbed config system macros.
 *
 *
 *
 * As a result, this module provides the following: @n
 * - literal value LFCLK_CONF_SOURCE @n
 * - literal value LFCLK_CONF_ACCURACY @n
 * - literal value LFCLK_CONF_RC_CTIV @n
 * - literal value LFCLK_CONF_RC_TEMP_CTIV
 */



#include "nrf_sdm.h"

#define DEFAULT_LFCLK_CONF_ACCURACY NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM

#ifdef NRF52
    #define MAX_LFCLK_CONF_RC_CTIV    32
#else
    #define MAX_LFCLK_CONF_RC_CTIV    64
#endif

#define MAX_LFCLK_CONF_RC_TEMP_CTIV   33

#define DEFAULT_LFCLK_CONF_RC_CTIV 16     // Check temperature every 16 * 250ms.
#define DEFAULT_LFCLK_CONF_RC_TEMP_CTIV 1 // Only calibrate if temperature has changed.

#define NRF_LF_SRC_XTAL  2 
#define NRF_LF_SRC_SYNTH 3 
#define NRF_LF_SRC_RC    4

#if MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_RC
    #define LFCLK_CONF_SOURCE       NRF_CLOCK_LF_SRC_RC
    
    #ifdef MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_TIMER_INTERVAL
        #define LFCLK_CONF_RC_CTIV MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_TIMER_INTERVAL
    #else
        #define LFCLK_CONF_RC_CTIV DEFAULT_LFCLK_CONF_RC_CTIV
    #endif
    
    #ifdef MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_MODE_CONFIG
        #define LFCLK_CONF_RC_TEMP_CTIV MBED_CONF_NORDIC_NRF_LF_CLOCK_CALIB_MODE_CONFIG
    #else
        #define LFCLK_CONF_RC_TEMP_CTIV DEFAULT_LFCLK_CONF_RC_TEMP_CTIV
    #endif
    
    #if (LFCLK_CONF_RC_CTIV < 1) || (LFCLK_CONF_RC_CTIV > MAX_LFCLK_CONF_RC_CTIV)
        #error Calibration timer interval out of range!
    #endif
    
    #if (LFCLK_CONF_RC_TEMP_CTIV < 0 ) || (LFCLK_CONF_RC_TEMP_CTIV > 33)
        #error Number/mode of LF RC calibration intervals out of range!
    #endif
    
#elif MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_SYNTH
    #define LFCLK_CONF_SOURCE       NRF_CLOCK_LF_SRC_SYNTH
    #define LFCLK_CONF_RC_CTIV      0 // Must be 0 if source is not NRF_CLOCK_LF_SRC_RC.
    #define LFCLK_CONF_RC_TEMP_CTIV 0 // Must be 0 if source is not NRF_CLOCK_LF_SRC_RC.
    
    #ifdef MBED_CONF_NORDIC_LF_CLOCK_HF_SYNTH_ACCURACY
        #define LFCLK_CONF_ACCURACY MBED_CONF_NORDIC_LF_CLOCK_HF_SYNTH_ACCURACY
    #endif
    
#else //  default is NRF_LF_SRC_SYNTH
    #define LFCLK_CONF_SOURCE NRF_CLOCK_LF_SRC_XTAL
    #define LFCLK_CONF_RC_CTIV      0 // Must be 0 if source is not NRF_CLOCK_LF_SRC_RC.
    #define LFCLK_CONF_RC_TEMP_CTIV 0 // Must be 0 if source is not NRF_CLOCK_LF_SRC_RC.
    
    #ifdef MBED_CONF_NORDIC_LF_CLOCK_XTAL_ACCURACY
        #define LFCLK_CONF_ACCURACY MBED_CONF_NORDIC_LF_CLOCK_XTAL_ACCURACY
    #endif

#endif

#ifndef LFCLK_CONF_ACCURACY
    #define LFCLK_CONF_ACCURACY DEFAULT_LFCLK_CONF_ACCURACY
#endif

#if (LFCLK_CONF_ACCURACY > NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM) || (LFCLK_CONF_ACCURACY < NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM)
    #error Low frequency clock accuracy out of range!
#endif


#endif //_BTLE_CLOCK_H_


