/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */
#include "mbed_assert.h"
#include "analogin_api.h"
#include "adc.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#define ADC_FULL_SCALE            0x3FFU
#define INT_FULL_SCALE            0xFFFFU
#define FLOAT_FULL_SCALE          1.0f

static int initialized = 0;

//******************************************************************************
void analogin_init(analogin_t *obj, PinName pin)
{
    // Make sure pin is an analog pin we can use for ADC
    MBED_ASSERT((ADCName)pinmap_peripheral(pin, PinMap_ADC) != (ADCName)NC);

    // Set the object pointer and channel encoding
    obj->adc = MXC_ADC;
    obj->channel = pinmap_find_function(pin, PinMap_ADC);

    if (!initialized) {
        MBED_ASSERT(ADC_Init() == E_NO_ERROR);
        initialized = 1;
    }
}

//******************************************************************************
float analogin_read(analogin_t *obj)
{
    uint16_t tmp;
    float result;

    // Start conversion with no input scaling and no input buffer bypass
    ADC_StartConvert(obj->channel, 0, 0);

    if (ADC_GetData(&tmp) == E_OVERFLOW) {
        result = FLOAT_FULL_SCALE;
    } else {
        result = (float)tmp * (FLOAT_FULL_SCALE / (float)ADC_FULL_SCALE);
    }

    return result;
}

//******************************************************************************
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t tmp;
    uint16_t result;

    // Start conversion with no input scaling and no input buffer bypass
    ADC_StartConvert(obj->channel, 0, 0);

    if (ADC_GetData(&tmp) == E_OVERFLOW) {
        result = INT_FULL_SCALE;
    } else {
        result = ((tmp << 6) & 0xFFC0) | ((tmp >> 4) & 0x003F);
    }

    return result;
}

