/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "sai_api.h"

#if DEVICE_SAI

// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include "mbed_assert.h"

#include <string.h>

#include "cmsis.h"
#include "pinmap.h"
#include "fsl_sai.h"
#include "peripheral_clock_defines.h"
#include "PeripheralPins.h"
#include "fsl_clock_config.h"

static I2S_Type *const sai_addrs[] = I2S_BASE_PTRS;

bool sai_receiver_initialized = false;
bool sai_transmitter_initialized = false;

sai_result_t sai_init(sai_t *obj, sai_init_t *init) {
    int32_t base = NC;
    int32_t tx = NC;
    int32_t rx = NC;

    if ((obj == NULL) || !sai_check_sanity(init)) {
        return SAI_RESULT_INVALID_PARAM;
    }
    if ((init->is_receiver && sai_receiver_initialized) ||
        (!init->is_receiver && sai_transmitter_initialized)) {
        return SAI_RESULT_ALREADY_INITIALIZED;
    }
    if ((memcmp(&(init->format), &sai_mode_i2s16w32, sizeof(sai_format_t)) != 0) &&
        (memcmp(&(init->format), &sai_mode_i2s16, sizeof(sai_format_t)) != 0)) {
        // we only support 1 format so far
        return SAI_RESULT_CONFIG_UNSUPPORTED;
    }
    if ((init->mclk_source != SAI_CLOCK_SOURCE_INTERNAL) || (init->input_mclk_frequency != 0)) {
        return SAI_RESULT_CONFIG_UNSUPPORTED;
    }
    uint32_t mclk_freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    obj->is_receiver = init->is_receiver;

    // validate pins & fetch base peripheral address
    int32_t mclk = pinmap_peripheral(init->mclk, PinMap_SAI_MCLK);
    if (obj->is_receiver) {
        int32_t sd = pinmap_peripheral(init->sd, PinMap_SAI_RXSD);
        int32_t bclk = pinmap_peripheral(init->bclk, PinMap_SAI_RXBCLK);
        int32_t wclk = pinmap_peripheral(init->wclk, PinMap_SAI_RXWCLK);
        MBED_ASSERT((sd != NC) && (bclk != NC) && (wclk != NC));
        rx = pinmap_merge(pinmap_merge(sd, bclk), wclk);
        MBED_ASSERT(rx != NC);
    } else {
        int32_t sd = pinmap_peripheral(init->sd, PinMap_SAI_TXSD);
        int32_t bclk = pinmap_peripheral(init->bclk, PinMap_SAI_TXBCLK);
        int32_t wclk = pinmap_peripheral(init->wclk, PinMap_SAI_TXWCLK);
        MBED_ASSERT((sd != NC) && (bclk != NC) && (wclk != NC));
        tx = pinmap_merge(pinmap_merge(sd, bclk), wclk);
        MBED_ASSERT(tx != NC);
    }
    base = pinmap_merge(pinmap_merge(tx, rx), mclk);
    MBED_ASSERT(base != NC);

    obj->base = sai_addrs[base];

    // enable recv
    sai_config_t config;
    sai_transfer_format_t format;

    if (init->mclk != NC) {
        pinmap_pinout(init->mclk, PinMap_SAI_MCLK);
    }
    if (obj->is_receiver) {
        if (init->sd != NC) {
            pinmap_pinout(init->sd, PinMap_SAI_RXSD);
        }
        if (init->bclk != NC) {
            pinmap_pinout(init->bclk, PinMap_SAI_RXBCLK);
        }
        if (init->wclk != NC) {
            pinmap_pinout(init->wclk, PinMap_SAI_RXWCLK);
        }

        /* Initialize SAI Rx */
        SAI_RxGetDefaultConfig(&config);
    } else {
        if (init->sd != NC) {
            pinmap_pinout(init->sd, PinMap_SAI_TXSD);
        }
        if (init->bclk != NC) {
            pinmap_pinout(init->bclk, PinMap_SAI_TXBCLK);
        }
        if (init->wclk != NC) {
            pinmap_pinout(init->wclk, PinMap_SAI_TXWCLK);
        }

        SAI_TxGetDefaultConfig(&config);
    }

    obj->channel = 0U; // we should get it from the sd pin

    config.protocol = kSAI_BusI2S;

    if (init->is_receiver) {
        config.masterSlave = kSAI_Slave;
        SAI_RxInit(obj->base, &config);
    } else {
        SAI_TxInit(obj->base, &config);
    }

    /* Configure the audio format */
    format.bitWidth = init->format.data_length;
    format.channel = obj->channel;
    format.sampleRate_Hz = init->sample_rate;
    format.masterClockHz = init->output_mclk_frequency;

    format.protocol = config.protocol;
    format.stereo = kSAI_Stereo;
    // FSL_FEATURE_SAI_FIFO_COUNT = 8 on MK66F18
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;

    if (obj->is_receiver) {
        SAI_RxSetFormat(obj->base, &format, mclk_freq, format.masterClockHz);
        sai_receiver_initialized = true;
    } else {
        SAI_TxSetFormat(obj->base, &format, mclk_freq, format.masterClockHz);
        sai_transmitter_initialized = true;
    }

    return SAI_RESULT_OK;
}

bool sai_transfer(sai_t *obj, uint32_t *sample) {
    if (obj == NULL) {
        return false;
    }

    bool ret = false;
    if (obj->is_receiver) {
        uint32_t flags = SAI_RxGetStatusFlag(obj->base);
        if ((flags & I2S_RCSR_RE_MASK) == 0) {
            SAI_RxEnable(obj->base, true);
        }
        ret = (flags & I2S_RCSR_FRF_MASK) == I2S_RCSR_FRF_MASK;
        uint32_t tmpsample = 0;
        if (ret) {
            tmpsample = SAI_ReadData(obj->base, obj->channel);

            if (sample != NULL) {
                *sample = tmpsample;
            }
        }
        if ((flags & I2S_RCSR_FEF_MASK) == I2S_RCSR_FEF_MASK) {
            SAI_RxClearStatusFlags(obj->base, I2S_RCSR_FEF_MASK);
        }
    } else {
        uint32_t tmp_sample = 0;
        if (sample != NULL) { tmp_sample = *sample; }

        uint32_t flags = SAI_TxGetStatusFlag(obj->base);
        if ((flags & I2S_TCSR_TE_MASK) == 0) {
            SAI_TxEnable(obj->base, true);
        }
        if ((flags & I2S_TCSR_FEF_MASK) == I2S_TCSR_FEF_MASK) {
            SAI_TxClearStatusFlags(obj->base, I2S_TCSR_FEF_MASK);
        }
        ret = (flags & I2S_TCSR_FWF_MASK) == I2S_TCSR_FWF_MASK;
        if (ret) {
            SAI_WriteData(obj->base, obj->channel, tmp_sample);
        }
    }
    return ret;
}

/** Releases & de-initialize the referenced peripherals. */
void sai_free(sai_t *obj) {
    if (obj == NULL) {
        return;
    }

    if (obj->is_receiver) {
        SAI_RxEnable(obj->base, false);
        sai_receiver_initialized = false;
    } else {
        SAI_TxEnable(obj->base, false);
        sai_transmitter_initialized = false;
    }
    // Should it also unclock the periph ?
}
#endif
