/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**
 * @file
 * @brief ADC HAL implementation
 */

#include "nrf_adc.h"

#ifndef NRF52

/**
 * @brief Function for configuring ADC.
 *
 * This function powers on ADC and configures it. ADC is in DISABLE state after configuration,
 * so it should be enabled before using it.
 *
 * @param[in] config  Requested configuration.
 */
void nrf_adc_configure(nrf_adc_config_t * config)
{
    uint32_t config_reg = 0;

    config_reg |= ((uint32_t)config->resolution << ADC_CONFIG_RES_Pos) & ADC_CONFIG_RES_Msk;
    config_reg |= ((uint32_t)config->scaling << ADC_CONFIG_INPSEL_Pos) & ADC_CONFIG_INPSEL_Msk;
    config_reg |= ((uint32_t)config->reference << ADC_CONFIG_REFSEL_Pos) & ADC_CONFIG_REFSEL_Msk;

    if (config->reference & ADC_CONFIG_EXTREFSEL_Msk)
    {
        config_reg |= config->reference & ADC_CONFIG_EXTREFSEL_Msk;
    }

    /* select input */
    nrf_adc_input_select(NRF_ADC_CONFIG_INPUT_DISABLED);

    /* set new configuration keeping selected input */
    NRF_ADC->CONFIG = config_reg | (NRF_ADC->CONFIG & ADC_CONFIG_PSEL_Msk);
}


/**
 * @brief Blocking function for executing single ADC conversion.
 *
 * This function selects the desired input, starts a single conversion,
 * waits for it to finish, and returns the result.
 * ADC is left in STOP state, the given input is selected.
 * This function does not check if ADC is initialized and powered.
 *
 * @param[in] input Requested input to be selected.
 *
 * @return Conversion result
 */
int32_t nrf_adc_convert_single(nrf_adc_config_input_t input)
{
    int32_t val;

    nrf_adc_input_select(input);
    nrf_adc_start();

    while (!nrf_adc_conversion_finished())
    {
    }
    nrf_adc_conversion_event_clean();
    val = nrf_adc_result_get();
    nrf_adc_stop();
    return val;
}
#endif
