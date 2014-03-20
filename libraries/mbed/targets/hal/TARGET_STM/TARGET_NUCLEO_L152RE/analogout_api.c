/* mbed Microcontroller Library
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

#define RANGE_12BIT (0xFFF)

static const PinMap PinMap_DAC[] = {
    {PA_4, DAC_1, STM_PIN_DATA(GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0xFF)}, // DAC_OUT1
    {PA_5, DAC_1, STM_PIN_DATA(GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0xFF)}, // DAC_OUT2
    {NC,   NC,    0}
};

void analogout_init(dac_t *obj, PinName pin) {
    DAC_InitTypeDef DAC_InitStructure;
  
    // Get the peripheral name (DAC_1, ...) from the pin and assign it to the object
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);

    if (obj->dac == (DACName)NC) {
        error("DAC pin mapping failed");
    }

    // Configure GPIO
    pinmap_pinout(pin, PinMap_DAC);

    // Save the channel for the write and read functions
    obj->channel = pin;

    // Enable DAC clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    // Configure and enable DAC channel
    DAC_InitStructure.DAC_Trigger                      = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration               = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitStructure.DAC_OutputBuffer                 = DAC_OutputBuffer_Disable;
    
    if (obj->channel == PA_4) {
        DAC_Init(DAC_Channel_1, &DAC_InitStructure);
        DAC_Cmd(DAC_Channel_1, ENABLE);
    }
    if (obj->channel == PA_5) {
        DAC_Init(DAC_Channel_2, &DAC_InitStructure);
        DAC_Cmd(DAC_Channel_2, ENABLE);
    }
      
    analogout_write_u16(obj, 0);
}

void analogout_free(dac_t *obj) {
}

static inline void dac_write(dac_t *obj, uint16_t value) {
    if (obj->channel == PA_4) {
        DAC_SetChannel1Data(DAC_Align_12b_R, value);
    }
    if (obj->channel == PA_5) {
        DAC_SetChannel2Data(DAC_Align_12b_R, value);
    }
}

static inline int dac_read(dac_t *obj) {
    if (obj->channel == PA_4) {
        return (int)DAC_GetDataOutputValue(DAC_Channel_1);
    }
    if (obj->channel == PA_5) {
        return (int)DAC_GetDataOutputValue(DAC_Channel_2);
    }
    return 0;
}

void analogout_write(dac_t *obj, float value) {
    if (value < 0.0f) {
        dac_write(obj, 0); // Min value
    } else if (value > 1.0f) {
        dac_write(obj, (uint16_t)RANGE_12BIT); // Max value
    } else {
        dac_write(obj, (uint16_t)(value * (float)RANGE_12BIT));
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    if (value > (uint16_t)RANGE_12BIT) {
      dac_write(obj, (uint16_t)RANGE_12BIT); // Max value
    }
    else {
      dac_write(obj, value);
    }
}

float analogout_read(dac_t *obj) {
    uint32_t value = dac_read(obj);
    return (float)value * (1.0f / (float)RANGE_12BIT);
}

uint16_t analogout_read_u16(dac_t *obj) {
    return (uint16_t)dac_read(obj);
}

#endif // DEVICE_ANALOGOUT
