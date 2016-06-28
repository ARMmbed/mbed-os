/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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

#include "nrf_adc.h"
#include "nrf_drv_config.h"
#include "sdk_errors.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup nrf_adc ADC HAL and driver
 * @ingroup nrf_drivers
 * @brief Analog-to-digital converter (ADC) APIs.
 * @details The ADC HAL provides basic APIs for accessing the registers of the analog-to-digital converter.
 * The ADC driver provides APIs on a higher level.
 *
 * @defgroup nrf_adc_drv ADC driver
 * @{
 * @ingroup nrf_adc
 * @brief Analog-to-digital converter (ADC) driver. 
 */


/**
 * @brief Driver event types.
 */
typedef enum
{
    NRF_DRV_ADC_EVT_DONE,    ///< Event generated when the buffer is filled with samples.
    NRF_DRV_ADC_EVT_SAMPLE,  ///< Event generated when the requested channel is sampled.
} nrf_drv_adc_evt_type_t;

typedef int16_t nrf_adc_value_t;

/**
 * @brief Analog-to-digital converter driver DONE event.
 */
typedef struct
{
    nrf_adc_value_t *        p_buffer; ///< Pointer to buffer with converted samples.
    uint16_t                 size;     ///< Number of samples in the buffer.
} nrf_drv_adc_done_evt_t;

/**
 * @brief Analog-to-digital converter driver SAMPLE event.
 */
typedef struct
{
    nrf_adc_value_t   sample; ///< Converted sample.
} nrf_drv_adc_sample_evt_t;

/**
 * @brief Analog-to-digital converter driver event.
 */
typedef struct
{
    nrf_drv_adc_evt_type_t type;  ///< Event type.
    union
    {
        nrf_drv_adc_done_evt_t   done;   ///< Data for DONE event.
        nrf_drv_adc_sample_evt_t sample; ///< Data for SAMPLE event.
    } data;
} nrf_drv_adc_evt_t;

/**@brief Macro for initializing the ADC channel with the default configuration. */
#define NRF_DRV_ADC_DEFAULT_CHANNEL(analog_input)          \
 {{{                                                       \
    .resolution = NRF_ADC_CONFIG_RES_10BIT,                \
    .input      = NRF_ADC_CONFIG_SCALING_INPUT_FULL_SCALE, \
    .reference  = NRF_ADC_CONFIG_REF_VBG,                  \
    .ain        = (analog_input)                           \
 }}, NULL}

/**
 * @brief ADC channel configuration.
 *
 * @note The bit fields reflect bit fields in the ADC CONFIG register.
 */
typedef struct 
{
    uint32_t resolution        :2; ///< 8-10 bit resolution.
    uint32_t input             :3; ///< Input selection and scaling.
    uint32_t reference         :2; ///< Reference source.
    uint32_t reserved          :1; ///< Unused bit fields.
    uint32_t ain               :8; ///< Analog input.
    uint32_t external_reference:2; ///< Eternal reference source.
}nrf_drv_adc_channel_config_t;

// Forward declaration of the nrf_drv_adc_channel_t type.
typedef struct nrf_drv_adc_channel_s nrf_drv_adc_channel_t;

/**
 * @brief ADC channel.
 *
 * This structure is defined by the user and used by the driver. Therefore, it should
 * not be defined on the stack as a local variable.
 */
struct nrf_drv_adc_channel_s
{
    union
    {
        nrf_drv_adc_channel_config_t config; ///< Channel configuration.
        uint32_t data;                       ///< Raw value.
    } config;
    nrf_drv_adc_channel_t      * p_next;     ///< Pointer to the next enabled channel (for internal use).
};

/**
 * @brief ADC configuration.
 */
typedef struct 
{
    uint8_t interrupt_priority;              ///< Priority of ADC interrupt.
} nrf_drv_adc_config_t;

/** @brief ADC default configuration. */
#define NRF_DRV_ADC_DEFAULT_CONFIG                \
{                                                 \
    .interrupt_priority = ADC_CONFIG_IRQ_PRIORITY \
}

/**
 * @brief User event handler prototype.
 *
 * This function is called when the requested number of samples has been processed.
 *
 * @param p_event Event.
 */
typedef void (*nrf_drv_adc_event_handler_t)(nrf_drv_adc_evt_t const * p_event);

/**
 * @brief Function for initializing the ADC.
 *
 * If a valid event handler is provided, the driver is initialized in non-blocking mode.
 * If event_handler is NULL, the driver works in blocking mode.
 *
 * @param[in] p_config Driver configuration.
 * @param[in] event_handler Event handler provided by the user.
 *
 * @retval    NRF_SUCCESS If initialization was successful.
 * @retval    NRF_ERROR_INVALID_STATE If the driver is already initialized.
 */
ret_code_t nrf_drv_adc_init(nrf_drv_adc_config_t const * p_config,
                            nrf_drv_adc_event_handler_t  event_handler);

/**
 * @brief Function for uninitializing the ADC.
 *
 * This function stops all ongoing conversions and disables all channels.
 */
void nrf_drv_adc_uninit(void);

/**
 * @brief Function for enabling an ADC channel.
 *
 * This function configures and enables the channel. When @ref nrf_drv_adc_buffer_convert is
 * called, all channels that have been enabled with this function are sampled.
 *
 * @note The channel instance variable @p p_channel is used by the driver as an item
 *       in a list. Therefore, it cannot be an automatic variable, and an assertion fails if it is
 *       an automatic variable (if asserts are enabled).
 */
void nrf_drv_adc_channel_enable(nrf_drv_adc_channel_t * const p_channel);

/**
 * @brief Function for disabling an ADC channel.
 */
void nrf_drv_adc_channel_disable(nrf_drv_adc_channel_t * const p_channel);

/**
 * @brief Function for starting ADC sampling.
 *
 * This function triggers single ADC sampling. If more than one channel is enabled, the driver
 * emulates scanning and all channels are sampled in the order they were enabled.
 */
void nrf_drv_adc_sample(void);

/**
 * @brief Function for executing a single ADC conversion.
 *
 * This function selects the desired input and starts a single conversion. If a valid pointer 
 * is provided for the result, the function blocks until the conversion is completed. Otherwise, the
 * function returns when the conversion is started, and the result is provided in an event (driver
 * must be initialized in non-blocking mode otherwise an assertion will fail). The function will fail if
 * ADC is busy. The channel does not need to be enabled to perform a single conversion.
 *
 * @param[in]  p_channel Channel.
 * @param[out] p_value   Pointer to the location where the result should be placed. Unless NULL is
 *                       provided, the function is blocking.
 *
 * @retval NRF_SUCCESS    If conversion was successful.
 * @retval NRF_ERROR_BUSY If the ADC driver is busy.
 */
ret_code_t nrf_drv_adc_sample_convert(nrf_drv_adc_channel_t const * const p_channel, 
                                      nrf_adc_value_t * p_value);

/**
 * @brief Function for converting data to the buffer.
 *
 * If the driver is initialized in non-blocking mode, this function returns when the first conversion
 * is set up. When the buffer is filled, the application is notified by the event handler. If the
 * driver is initialized in blocking mode, the function returns when the buffer is filled.
 *
 * Conversion is done on all enabled channels, but it is not triggered by this
 * function. This function will prepare the ADC for sampling and then 
 * wait for the SAMPLE task. Sampling can be triggered manually by the @ref 
 * nrf_drv_adc_sample function or by PPI using the @ref NRF_ADC_TASK_START task.
 *
 * @note If more than one channel is enabled, the function emulates scanning, and 
 * a single START task will trigger conversion on all enabled channels. For example:
 * If 3 channels are enabled and the user requests 6 samples, the completion event 
 * handler will be called after 2 START tasks. 
 * @note The application must adjust the sampling frequency. The maximum frequency 
 * depends on the sampling timer and the maximum latency of the ADC interrupt. If 
 * an interrupt is not handled before the next sampling is triggered, the sample 
 * will be lost.
 *
 * @param[in] buffer Result buffer.
 * @param[in] size   Buffer size in samples.
 *
 * @retval NRF_SUCCESS    If conversion was successful.
 * @retval NRF_ERROR_BUSY If the driver is busy.
 */
ret_code_t nrf_drv_adc_buffer_convert(nrf_adc_value_t * buffer, uint16_t size);

/**
 * @brief Function for retrieving the ADC state.
 *
 * @retval true  If the ADC is busy.
 * @retval false If the ADC is ready.
 */
bool nrf_drv_adc_is_busy(void);

/**
 * @brief Function for getting the address of the ADC START task.
 *
 * This function is used to get the address of the START task, which can be used to trigger ADC
 * conversion.
 *
 * @return Start task address.
 */
__STATIC_INLINE uint32_t nrf_drv_adc_start_task_get(void);

/**
 * @brief Function for converting a GPIO pin number to an analog input pin mask to be used in
 *        the ADC channel configuration.
 *
 * @param[in]  pin GPIO pin.
 *
 * @return     Analog input pin mask. The function returns @ref NRF_ADC_CONFIG_INPUT_DISABLED
 *             if the specified pin is not an analog input.
 */
__STATIC_INLINE nrf_adc_config_input_t nrf_drv_adc_gpio_to_ain(uint32_t pin);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE uint32_t nrf_drv_adc_start_task_get(void)
{
    return nrf_adc_task_address_get(NRF_ADC_TASK_START);
}

__STATIC_INLINE nrf_adc_config_input_t nrf_drv_adc_gpio_to_ain(uint32_t pin)
{
    // AIN2 - AIN7
    if (pin >= 1 && pin <= 6)
    {
        return (nrf_adc_config_input_t)(1 << (pin+1));
    }
    // AIN0 - AIN1
    else if (pin >= 26 && pin <= 27)
    {
        return (nrf_adc_config_input_t)(1 <<(pin - 26));
    }
    else
    {
        return NRF_ADC_CONFIG_INPUT_DISABLED;
    }
}

#ifdef __cplusplus
}
#endif

#endif
/** @} */
