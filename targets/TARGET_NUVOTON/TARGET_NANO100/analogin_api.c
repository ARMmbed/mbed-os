/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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

#include "mbed_wait_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "nu_modutil.h"

static uint32_t adc_modinit_mask = 0;

static const struct nu_modinit_s adc_modinit_tab[] = {
    {ADC_0_0, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_1, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_2, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_3, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_4, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_5, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_6, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_7, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_8, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_9, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_10, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
    {ADC_0_11, ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1), ADC_RST, ADC_IRQn, NULL},
};

void analogin_init(analogin_t *obj, PinName pin)
{
    obj->adc = (ADCName) pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName) NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->adc, adc_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT((ADCName) modinit->modname == obj->adc);

    obj->pin = pin;

    // Wire pinout
    pinmap_pinout(pin, PinMap_ADC);

    ADC_T *adc_base = (ADC_T *) NU_MODBASE(obj->adc);
    uint32_t chn =  NU_MODSUBINDEX(obj->adc);

    // NOTE: All channels (identified by ADCName) share a ADC module. This reset will also affect other channels of the same ADC module.
    if (! adc_modinit_mask) {
        // Select clock source of paired channels
        CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);

        // Enable clock of paired channels
        CLK_EnableModuleClock(modinit->clkidx);

        // Reset this module if no channel enabled
        SYS_ResetModule(modinit->rsetidx);

        // Set operation mode and enable channel N
        ADC_Open(ADC, ADC_INPUT_MODE_SINGLE_END, ADC_OPERATION_MODE_SINGLE_CYCLE, 1 << chn);

        // Set reference voltage to AVDD
        ADC_SET_REF_VOLTAGE(ADC, ADC_REFSEL_POWER);

        // Power on ADC
        ADC_POWER_ON(ADC);
    } else {
        // Just enable channel N
        adc_base->CHEN |= 1 << chn;
    }

    adc_modinit_mask |= 1 << chn;
}

void analogin_free(analogin_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->adc, adc_modinit_tab);
    MBED_ASSERT(modinit->modname == (int) obj->adc);

    /* Module subindex (aka channel) */
    uint32_t chn =  NU_MODSUBINDEX(obj->adc);

    ADC_T *adc_base = (ADC_T *) NU_MODBASE(obj->adc);

    /* Channel-level windup from here */

    /* Mark channel free */
    adc_modinit_mask &= ~(1 << chn);

    adc_base->CHEN &= ~(1 << chn);
    adc_modinit_mask &= ~(1 << chn);

    /* Module-level windup from here */

    /* See analogin_init() for reason */
    if (! adc_modinit_mask) {
        /* Disable ADC module */
        ADC_Close(adc_base);

        // Power off ADC
        ADC_POWER_DOWN(adc_base);

        /* Disable IP clock */
        CLK_DisableModuleClock(modinit->clkidx);
    }

    /* Free up pins */
    gpio_set(obj->pin);
    obj->pin = NC;
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    ADC_T *adc_base = (ADC_T *) NU_MODBASE(obj->adc);
    uint32_t chn =  NU_MODSUBINDEX(obj->adc);

    // Start the A/D conversion
    adc_base->CR |= ADC_CR_ADST_Msk;
    // Wait for conversion finish
    while (! ADC_GET_INT_FLAG(adc_base, ADC_ADF_INT) & ADC_ADF_INT) ;
    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
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

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif
