/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
 
#ifdef DEVICE_ANALOGIN
#include "hal/analogin_api.h"

#include "pinmap.h"
#include "PeripheralPins.h"

#include "nrf_drv_saadc.h"

#define ADC_12BIT_RANGE 0x0FFF
#define ADC_16BIT_RANGE 0xFFFF

/* Unused event handler but driver requires one. */
static void analog_in_event_handler(nrf_drv_saadc_evt_t const *p_event)
{
    (void) p_event;
}

/* Interrupt handler implemented in nrf_drv_saadc.c. */
void SAADC_IRQHandler(void);

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin)
{    
    MBED_ASSERT(obj);

    /* Only initialize SAADC on first pin. */
    static bool first_init = true;

    if (first_init) {

        first_init = false;

        /* Use configuration from sdk_config.h.
         * Default is: 
         *  - 12 bit.
         *  - No oversampling.
         *  - Priority 7 (lowest).
         *  - No low power mode.
         */
        nrf_drv_saadc_config_t adc_config = {
            .resolution         = (nrf_saadc_resolution_t)SAADC_CONFIG_RESOLUTION,
            .oversample         = (nrf_saadc_oversample_t)SAADC_CONFIG_OVERSAMPLE,
            .interrupt_priority = SAADC_CONFIG_IRQ_PRIORITY,
            .low_power_mode     = SAADC_CONFIG_LP_MODE
        };

        ret_code_t result = nrf_drv_saadc_init(&adc_config, analog_in_event_handler);
        MBED_ASSERT(result == NRF_SUCCESS);

        /* Register interrupt handler in vector table. */
        NVIC_SetVector(SAADC_IRQn, (uint32_t)SAADC_IRQHandler);
    }

    /* Use pinmap function to get associated channel. */
    uint32_t channel = pinmap_function(pin, PinMap_ADC);
    MBED_ASSERT(channel != (uint32_t) NC);

    /* Account for an off-by-one in Channel definition and Input definition. */
    nrf_saadc_input_t input = channel + 1;

    /* Configure channel and pin:
     *  - the 1/4 gain and VDD/4 makes the reference voltage VDD.
     */
    nrf_saadc_channel_config_t channel_config = {
        .resistor_p = NRF_SAADC_RESISTOR_DISABLED,
        .resistor_n = NRF_SAADC_RESISTOR_DISABLED,
        .gain       = NRF_SAADC_GAIN1_4,
        .reference  = NRF_SAADC_REFERENCE_VDD4,
        .acq_time   = NRF_SAADC_ACQTIME_10US,
        .mode       = NRF_SAADC_MODE_SINGLE_ENDED,
        .burst      = NRF_SAADC_BURST_DISABLED,
        .pin_p      = input,
        .pin_n      = NRF_SAADC_INPUT_DISABLED
    };

    ret_code_t result = nrf_drv_saadc_channel_init(channel, &channel_config);
    MBED_ASSERT(result == NRF_SUCCESS);

    /* Store channel in ADC object. */
    obj->channel = channel;
}


/** Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param obj The analogin object
 * @return A floating value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj)
{    
    MBED_ASSERT(obj);

    /* Default return value is 0. */
    uint16_t retval = 0;
    
    /* Read single channel, blocking. */
    nrf_saadc_value_t value = { 0 };
    ret_code_t result = nrf_drv_saadc_sample_convert(obj->channel, &value);

    /* nrf_saadc_value_t is a signed integer. Only take the absolute value. */
    if ((result == NRF_SUCCESS) && (value > 0)) {

        /* Normalize 12 bit ADC value to 16 bit Mbed ADC range. */
        uint32_t normalized = value;
        retval = (normalized * ADC_16BIT_RANGE) / ADC_12BIT_RANGE;
    }

    return retval;
}

/** Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16bit value representing the current input voltage
 */
float analogin_read(analogin_t *obj)
{
    MBED_ASSERT(obj);

    /* Read 16 bit ADC value (using Mbed API) and convert to [0;1] range float. */
    uint16_t value = analogin_read_u16(obj);
    float result = ((float) value / (float) ADC_16BIT_RANGE);

    return result;
}

#endif // DEVICE_ANALOGIN
