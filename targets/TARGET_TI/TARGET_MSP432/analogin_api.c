/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

/* Low-level implementation of ADC functionality for MSP432. The ADC
   is very precise and has a resolution of 14 bits. In the default mode,
   a ADC conversion is triggered when read() or read_u16() is called.
   The program will then wait until the conversion result is available.

   Alternatively, MBED_CONF_TARGET_ADC_AUTO_SCAN can be set, which will
   instruct the ADC to scan and convert all configured ADC channels
   automatically (so the ADC is active all the time). The two read-functions
   will then only read the result register, which will contain the latest
   available conversion result. In this case the program will not have to
   wait for the conversion result and the read-functions are faster!
 */

#if DEVICE_ANALOGIN

#include "analogin_api.h"
#include "mbed_assert.h"
#include <stddef.h>

static uint8_t next_mem_index = 0;

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin)
{
    /* Check if pin supports ADC function */
    MBED_ASSERT(pinmap_peripheral(pin, PinMap_ADC) == ADC14_BASE);
    /* Check if we have used this pin already */
    uint8_t channel = GET_DATA_CHAN(pinmap_function(pin, PinMap_ADC));
    uint8_t index;
    for (index = 0; index < next_mem_index; ++index) {
        if ((ADC14->MCTL[index] & ADC14_MCTLN_INCH_MASK) == channel) {
            /* Pin was used before. Set mem register index and return */
            obj->mem_index = index;
            return;
        }
    }
    /* Configure the GPIO pin */
    pinmap_pinout(pin, PinMap_ADC);
    /* Fill the obj structure */
    obj->mem_index = next_mem_index++;
    /* Reset ENC bit so the ADC can be configured */
    BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 0;
    /* Set up result buffer */
    ADC14->MCTL[obj->mem_index]  = channel;
    ADC14->MCTL[obj->mem_index] |= ADC14_MCTLN_VRSEL_0;
#ifdef MBED_CONF_TARGET_ADC_AUTO_SCAN
    ADC14->MCTL[obj->mem_index] |= ADC14_MCTLN_EOS;
    // Clear the former End-Of-Sequence bit
    if (obj->mem_index) {
        ADC14->MCTL[obj->mem_index - 1] &= ~ADC14_MCTLN_EOS;
    }
#endif
    /* Set up the control registers of ADC14 */
    ADC14->CTL1 = ADC14_CTL1_RES__14BIT   | // 14 bit resolution
                  ADC14_CTL1_PWRMD_0;       // normal power mode
    ADC14->CTL0 = ADC14_CTL0_SSEL__MODCLK | // Use MODCLK (25MHz)
                  ADC14_CTL0_PDIV__1      | // clock pre-divider is 1
                  ADC14_CTL0_DIV__1       | // clock divider 1
                  ADC14_CTL0_SHS_0        | // use ADCSC bit as sample/hold trigger
                  ADC14_CTL0_SHP          | // use sample/hold counter
                  ADC14_CTL0_SHT1__16     | // 16 cycles for sample/hold
                  ADC14_CTL0_SHT0__16     | // 16 cycles for sample/hold
#ifdef MBED_CONF_TARGET_ADC_AUTO_SCAN
                  ADC14_CTL0_CONSEQ_3     | // Repeat sequence of channels
                  ADC14_CTL0_MSC          | // Multiple samples and conversion
                  ADC14_CTL0_ENC          | // Enable conversion
                  ADC14_CTL0_SC           | // Start conversion
#else
                  ADC14_CTL0_CONSEQ_0     | // Single channel, single conversion
#endif
                  ADC14_CTL0_ON;            // switch on ADC module
    /* Disable interrupts */
    ADC14->IER0 = 0;
    ADC14->IER1 = 0;
}

/* Read a 14-bit ADC result from a single channel */
uint16_t adc_read_raw(uint8_t mem_index)
{
#ifndef MBED_CONF_TARGET_ADC_AUTO_SCAN
    /* Reset ENC bit so the ADC can be configured */
    BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 0;
    // set up start channel
    ADC14->CTL1 &= ~ADC14_CTL1_CSTARTADD_MASK;
    ADC14->CTL1 |= (mem_index << ADC14_CTL1_CSTARTADD_OFS);
    // start the conversion process
    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;
    // wait until active conversion is done
    while (BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_BUSY_OFS));
#endif
    return ADC14->MEM[mem_index];
}

/** Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param obj The analogin object
 * @return A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj)
{
    uint16_t value = adc_read_raw(obj->mem_index);
    return (float)value * (1.0f / (float)0x3FFF);
}

/** Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t value = adc_read_raw(obj->mem_index);
    // 14-bit to 16-bit conversion
    value = (value << 2) | (value >> 12);
    return value;
}

/** Get the pins that support analogin
 *
 * Return a PinMap array of pins that support analogin. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *analogin_pinmap(void)
{
    return PinMap_ADC;
}

#endif /* DEVICE_ANALOG_IN */
