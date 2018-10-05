/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
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

#include "PeripheralPins.h"

typedef enum {
    NRF_SAADC_CHANNEL_0 = 0,
    NRF_SAADC_CHANNEL_1 = 1,
    NRF_SAADC_CHANNEL_2 = 2,
    NRF_SAADC_CHANNEL_3 = 3,
    NRF_SAADC_CHANNEL_4 = 4,
    NRF_SAADC_CHANNEL_5 = 5,
    NRF_SAADC_CHANNEL_6 = 6,
    NRF_SAADC_CHANNEL_7 = 7,
} nrf_saadc_channel_t;

/************ADC***************/
/* The third "function" value is used to select the correct ADC channel */
#if DEVICE_ANALOGIN
const PinMap PinMap_ADC[] = {
    { p2,  ADC0_0, NRF_SAADC_CHANNEL_0 },
    { p3,  ADC0_0, NRF_SAADC_CHANNEL_1 },
    { p4,  ADC0_0, NRF_SAADC_CHANNEL_2 },
    { p5,  ADC0_0, NRF_SAADC_CHANNEL_3 },
    { p28, ADC0_0, NRF_SAADC_CHANNEL_4 },
    { p29, ADC0_0, NRF_SAADC_CHANNEL_5 },
    { p30, ADC0_0, NRF_SAADC_CHANNEL_6 },
    { p31, ADC0_0, NRF_SAADC_CHANNEL_7 },
    { NC,  NC,     NC                  }
};
#endif
