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
#include "mbed_assert.h"
#include "analogin_api.h"
#include "pinmap.h"

#include "rda_ccfg_api.h"

#define ANALOGIN_MEDIAN_FILTER      1
#define RDA_ADC_RANGE               (0x03FFU)

#if DEVICE_ANALOGIN
static const PinMap PinMap_ADC[] = {
    {PB_6, ADC0_0, 0},
    {PB_7, ADC0_1, 0},
    {PB_8, ADC0_1, 0},
    {PD_9, ADC0_2, 0},
    {NC,   NC,     0}
};
static unsigned char adc1_gp = 0U;
static int adc_inited_cnt = 0;

void analogin_init(analogin_t *obj, PinName pin)
{
    unsigned char gp = (unsigned char)(pin & 0x00FFU);
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    /* Init ADC */
    if (0 == adc_inited_cnt) {
        rda_ccfg_adc_init();
    }
    adc_inited_cnt++;
    if (ADC0_2 == obj->adc) {
        return;
    }
    if ((ADC0_1 == obj->adc) && (0U == adc1_gp)) {
        adc1_gp = (2 >= rda_ccfg_hwver()) ? 7U : 8U;
        MBED_ASSERT(gp == adc1_gp);
    }
    rda_ccfg_gp(gp, 0x00U);
    rda_ccfg_adc_gp(gp, 0x00U);
    rda_ccfg_adc_oenb(obj->adc, 0x01U);

    pinmap_pinout(pin, PinMap_ADC);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t value = rda_ccfg_adc_read((unsigned char)(obj->adc));
    return (value); // 10 bit
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = rda_ccfg_adc_read((unsigned char)(obj->adc));
    return (float)value * (1.0f / (float)RDA_ADC_RANGE);
}

void analogin_free(analogin_t *obj)
{
    unsigned char gp = 6U;
    adc_inited_cnt--;
    if (0 == adc_inited_cnt) {
        rda_ccfg_adc_free();
    }
    if (ADC0_2 == obj->adc) {
        return;
    }
    if (ADC0_1 == obj->adc) {
        if (0U == adc1_gp) {
            return;
        }
        gp = adc1_gp;
    }
    rda_ccfg_adc_gp(gp, 0x01U);
    rda_ccfg_gp(gp, 0x01U);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif /* DEVICE_ANALOGIN */
