/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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
 
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "nu_modutil.h"

struct nu_adc_var {
    uint32_t    en_msk;
};

static struct nu_adc_var adc0_var = {
    .en_msk = 0
};
static struct nu_adc_var adc1_var = {
    .en_msk = 0
};
static struct nu_adc_var adc2_var = {
    .en_msk = 0
};
static struct nu_adc_var adc3_var = {
    .en_msk = 0
};
static struct nu_adc_var adc4_var = {
    .en_msk = 0
};
static struct nu_adc_var adc5_var = {
    .en_msk = 0
};
static struct nu_adc_var adc6_var = {
    .en_msk = 0
};
static struct nu_adc_var adc7_var = {
    .en_msk = 0
};
static struct nu_adc_var adc8_var = {
    .en_msk = 0
};
static struct nu_adc_var adc9_var = {
    .en_msk = 0
};
static struct nu_adc_var adc10_var = {
    .en_msk = 0
};
static struct nu_adc_var adc11_var = {
    .en_msk = 0
};

static const struct nu_modinit_s adc_modinit_tab[] = {
    {ADC_0_0, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc0_var},
    {ADC_0_1, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc1_var},
    {ADC_0_2, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc2_var},
    {ADC_0_3, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc3_var},
    {ADC_0_4, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc4_var},
    {ADC_0_5, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc5_var},
    {ADC_0_6, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc6_var},
    {ADC_0_7, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc7_var},
    {ADC_0_8, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc8_var},
    {ADC_0_9, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc9_var},
    {ADC_0_10, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc10_var},
    {ADC_0_11, ADC_MODULE, CLK_CLKSEL1_ADCSEL_HIRC, CLK_CLKDIV0_ADC(1), ADC_RST, ADC_IRQn, &adc11_var}
};

void analogin_init(analogin_t *obj, PinName pin)
{   
    obj->adc = (ADCName) pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName) NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->adc, adc_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->adc);
    
    // NOTE: All channels (identified by ADCName) share a ADC module. This reset will also affect other channels of the same ADC module.
    if (! ((struct nu_adc_var *) modinit->var)->en_msk) {
        // Reset this module if no channel enabled
        SYS_ResetModule(modinit->rsetidx);
        
        // Select clock source of paired channels
        CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);
        // Enable clock of paired channels
        CLK_EnableModuleClock(modinit->clkidx);
        
        // Power on ADC
        ADC_POWER_ON(ADC);
    }
    
    ADC_T *adc_base = (ADC_T *) NU_MODBASE(obj->adc);
    uint32_t chn =  NU_MODSUBINDEX(obj->adc);
    
    // Wire pinout
    pinmap_pinout(pin, PinMap_ADC);
    
    // Enable channel 0
    ADC_Open(adc_base,
        ADC_INPUT_MODE_SINGLE_END,
        ADC_OPERATION_MODE_SINGLE,
        1 << chn);  // ADC_CH_0_MASK~ADC_CH_11_MASK
    
    ((struct nu_adc_var *) modinit->var)->en_msk |= 1 << chn;
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    ADC_T *adc_base = (ADC_T *) NU_MODBASE(obj->adc);
    uint32_t chn =  NU_MODSUBINDEX(obj->adc);
    
    ADC_START_CONV(adc_base);
    while (adc_base->CTL & ADC_CTL_SWTRG_Msk);
    uint16_t conv_res_12 = ADC_GET_CONVERSION_DATA(adc_base, chn);
    // Just 12 bits are effective. Convert to 16 bits.
    // conv_res_12: 0000 b11b10b9b8 b7b6b5b4 b3b2b1b0
    // conv_res_16: b11b10b9b8 b7b6b5b4 b3b2b1b0 b11b10b9b8
    uint16_t conv_res_16 = (conv_res_12 << 4) | (conv_res_12 >> 8);
    
    return conv_res_16;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float) value * (1.0f / (float) 0xFFFF);
}

#endif
