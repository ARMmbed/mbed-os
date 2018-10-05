/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
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


#ifndef NRF_DRV_CONFIG_VALIDATION_H
#define NRF_DRV_CONFIG_VALIDATION_H

#ifdef NRF52

#if (!PERIPHERAL_RESOURCE_SHARING_ENABLED) && \
    ((SPI0_ENABLED + SPIS0_ENABLED + TWI0_ENABLED + TWIS0_ENABLED) > 1)
#error "Peripherals overlap. SPI0, SPIS0, TWI0, TWIS0 - only one of these can be enabled."
#endif

#if (!PERIPHERAL_RESOURCE_SHARING_ENABLED) && \
    ((SPI1_ENABLED + SPIS1_ENABLED + TWI1_ENABLED + TWIS1_ENABLED) > 1)
#error "Peripherals overlap. SPI1, SPIS1, TWI1, TWIS1 - only one of these can be enabled."
#endif

#if (!PERIPHERAL_RESOURCE_SHARING_ENABLED) && \
    ((SPI2_ENABLED + SPIS2_ENABLED) > 1)
#error "Peripherals overlap. SPI2, SPIS2 - only one of these can be enabled."
#endif

#if (!PERIPHERAL_RESOURCE_SHARING_ENABLED) && \
    ((COMP_ENABLED + LPCOMP_ENABLED) > 1)
#error "COMP and LPCOMP cannot be enabled together. Peripherals overlap."
#endif

#else //NRF51

#if (TWIS0_ENABLED + TWIS1_ENABLED) > 0
#error "TWIS not present in nRF51."
#endif

#if SPIS0_ENABLED > 0
#error "SPIS0 instance not present in nRF51."
#endif 

#if (SPI2_ENABLED + SPIS2_ENABLED) > 0
#error "SPI2/SPIS2 instance not present in nRF51."
#endif 

#if RTC2_ENABLED
#error "RTC2 not present in NRF51."
#endif

#if (TIMER3_ENABLED + TIMER4_ENABLED) > 0
#error "TIMER3 and TIMER4 not present in nRF51."
#endif

#if (!PERIPHERAL_RESOURCE_SHARING_ENABLED) && \
    ((SPI0_ENABLED + TWI0_ENABLED) > 1)
#error "Peripherals overlap. SPI0, TWI0 - only one of these can be enabled."
#endif

#if (!PERIPHERAL_RESOURCE_SHARING_ENABLED) && \
    ((SPI1_ENABLED + SPIS1_ENABLED + TWI1_ENABLED) > 1)
#error "Peripherals overlap. SPI1, SPIS1, TWI1 - only one of these can be enabled."
#endif

#if SAADC_ENABLED > 0
#error "SAADC not present in nRF51."
#endif

#if I2S_ENABLED > 0
#error "I2S not present in nRF51."
#endif
#if COMP_ENABLED > 0
#error "COMP not present in nRF51."
#endif

#endif //NRF51

#endif // NRF_DRV_CONFIG_VALIDATION_H
