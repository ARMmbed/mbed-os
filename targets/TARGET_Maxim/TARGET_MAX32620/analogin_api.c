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
#include "clkman_regs.h"
#include "pwrman_regs.h"
#include "trim_regs.h"
#include "PeripheralPins.h"

#define PGA_TRK_CNT     0x1F
#define ADC_ACT_CNT     0x1
#define ADC_PGA_CNT     0x1
#define ADC_ACQ_CNT     0x1
#define ADC_SLP_CNT     0x1

#define ADC_FULL_SCALE          0x3FF
#define ADC_EXTERNAL_LAST_INPUT 3

// Only allow initialization once
static int initialized = 0;

//******************************************************************************
void analogin_init(analogin_t *obj, PinName pin)
{
    // Make sure pin is an analog pin we can use for ADC
    MBED_ASSERT((ADCName)pinmap_peripheral(pin, PinMap_ADC) != (ADCName)NC);

    // Set the object pointer
    obj->adc = MXC_ADC;
    obj->adc_pin = pin;

    if (initialized == 0) {
        // Enable AFE power
        MXC_PWRMAN->pwr_rst_ctrl |= MXC_F_PWRMAN_PWR_RST_CTRL_AFE_POWERED;

        // Enable the clock
        MXC_CLKMAN->clk_ctrl |= MXC_F_CLKMAN_CLK_CTRL_ADC_CLOCK_ENABLE;

        // Enable clock gate
        MXC_CLKMAN->clk_gate_ctrl2 |= MXC_F_CLKMAN_CLK_GATE_CTRL2_ADC_CLK_GATER;

        // Enable interface clock
        obj->adc->ctrl |= MXC_F_ADC_CTRL_ADC_CLK_EN;

        if ((MXC_TRIM->reg11_adc_trim0 == 0xFFFFFFFF) && (MXC_TRIM->reg12_adc_trim1 == 0xFFFFFFFF)) {
            // Set default trim for untrimmed parts.
            MXC_TRIM->reg11_adc_trim0 = 0x02000200;
            MXC_TRIM->reg12_adc_trim1 = 0x02000200;
        }

        // Clear ADC ready interrupt (wite 1 to clr)
        obj->adc->intr = (obj->adc->intr & 0xFFFF) | MXC_F_ADC_INTR_ADC_REF_READY_IF;

        // Using internal reference of 1.20V

        // Enable ADC power bypass the buffer
        obj->adc->ctrl |= (MXC_F_ADC_CTRL_ADC_PU | MXC_F_ADC_CTRL_ADC_REFBUF_PU |
                           MXC_F_ADC_CTRL_ADC_CHGPUMP_PU | MXC_F_ADC_CTRL_BUF_BYPASS);

        // Wait for ADC ready
        while (!(obj->adc->intr & MXC_F_ADC_INTR_ADC_REF_READY_IF));

        initialized = 1;
    }
}

//******************************************************************************
float analogin_read(analogin_t *obj)
{
    // Convert integer to float
    return (((float)analogin_read_u16(obj)/(float)ADC_FULL_SCALE));
}

//******************************************************************************
uint16_t analogin_read_u16(analogin_t *obj)
{
    // Set the pin to take readings from
    uint32_t adc_input = PINNAME_TO_PIN(obj->adc_pin);

    // Select the channel
    obj->adc->ctrl &= ~MXC_F_ADC_CTRL_ADC_CHSEL;
    obj->adc->ctrl |= (adc_input << MXC_F_ADC_CTRL_ADC_CHSEL_POS) & MXC_F_ADC_CTRL_ADC_CHSEL;

    // We want unity gain, to get full 0-Vref range
    // So, both ref and adc input scale should be enabled
    obj->adc->ctrl |= MXC_F_ADC_CTRL_ADC_SCALE | MXC_F_ADC_CTRL_ADC_REFSCL;

    // Not using internal buffer, disable anyway
    obj->adc->ctrl &= ~MXC_F_ADC_CTRL_BUF_PU;
    obj->adc->ctrl |= MXC_F_ADC_CTRL_BUF_BYPASS;

    // Normal LSB justified data alignment

    // Not using limits

    // Clear ADC done flag (wite 1 to clr)
    obj->adc->intr = (obj->adc->intr & 0xFFFF) | MXC_F_ADC_INTR_ADC_DONE_IF;

    // Start the conversion
    obj->adc->ctrl |= MXC_F_ADC_CTRL_CPU_ADC_START;

    // Wait for ADC done
    while (!(obj->adc->intr & MXC_F_ADC_INTR_ADC_DONE_IF));

    // Get sample from the fifo
    uint16_t sample = obj->adc->data;

    // Check for overflow, hardware will report overflow as 0
    if (obj->adc->status & MXC_F_ADC_STATUS_ADC_OVERFLOW) {
        sample = (uint16_t)ADC_FULL_SCALE;
    }

    return sample;
}

//******************************************************************************
const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
