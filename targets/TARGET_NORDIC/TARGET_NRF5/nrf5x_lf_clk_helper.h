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

#ifndef __NRF5X_LF_CLK_HELPER_H_
    
#ifndef MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC
    #define MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC (NRF_LF_SRC_XTAL)
    #warning No configuration for LF clock source. Xtal source will be used as a default configuration.
#endif



#define NRF_LF_SRC_XTAL  2 
#define NRF_LF_SRC_SYNTH 3 
#define NRF_LF_SRC_RC    4

#if MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_SYNTH
    #define CLOCK_LFCLKSRC_SRC_TO_USE (CLOCK_LFCLKSRC_SRC_Synth)
#elif MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_XTAL
    #define CLOCK_LFCLKSRC_SRC_TO_USE (CLOCK_LFCLKSRC_SRC_Xtal)
#elif MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_RC
    #define CLOCK_LFCLKSRC_SRC_TO_USE (CLOCK_LFCLKSRC_SRC_RC)
#else
    #error Bad LFCLK configuration. Declare proper source through mbed configuration.
#endif

#undef NRF_LF_SRC_XTAL
#undef NRF_LF_SRC_SYNTH
#undef NRF_LF_SRC_RC

#endif
