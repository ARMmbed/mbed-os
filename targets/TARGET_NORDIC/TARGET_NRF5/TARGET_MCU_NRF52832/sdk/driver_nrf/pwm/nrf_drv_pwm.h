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

/**@file
 * @addtogroup nrf_pwm PWM HAL and driver
 * @ingroup    nrf_drivers
 * @brief      @tagAPI52 Pulse Width Modulation (PWM) module APIs.
 *
 * @defgroup   nrf_drv_pwm PWM driver
 * @{
 * @ingroup    nrf_pwm
 * @brief      @tagAPI52 Pulse Width Modulation (PWM) module driver.
 */


#ifndef NRF_DRV_PWM_H__
#define NRF_DRV_PWM_H__

#include "nordic_common.h"
#include "nrf_drv_config.h"
#include "nrf_pwm.h"
#include "sdk_errors.h"


/**
 * @brief PWM driver instance data structure.
 */
typedef struct
{
    NRF_PWM_Type * p_registers;  ///< Pointer to the structure with PWM peripheral instance registers.
    uint8_t        drv_inst_idx; ///< Driver instance index.
} nrf_drv_pwm_t;

/**
 * @brief Macro for creating a PWM driver instance.
 */
#define NRF_DRV_PWM_INSTANCE(id)                        \
{                                                       \
    .p_registers  = CONCAT_2(NRF_PWM, id),              \
    .drv_inst_idx = CONCAT_3(PWM, id, _INSTANCE_INDEX), \
}


/**
 * @brief This value can be provided instead of a pin number for any channel
 *        to specify that its output is not used and therefore does not need
 *        to be connected to a pin.
 */
#define NRF_DRV_PWM_PIN_NOT_USED    0xFF

/**
 * @brief This value can be added to a pin number to inverse its polarity 
 *        (set idle state = 1).
 */
#define NRF_DRV_PWM_PIN_INVERTED    0x80

/**
 * @brief PWM driver configuration structure.
 */
typedef struct
{
    uint8_t output_pins[NRF_PWM_CHANNEL_COUNT]; ///< Pin numbers for individual output channels (optional). 
                                                /**< Use @ref NRF_DRV_PWM_PIN_NOT_USED
                                                 *   if a given output channel is not needed. */
    uint8_t            irq_priority; ///< Interrupt priority.
    nrf_pwm_clk_t      base_clock;   ///< Base clock frequency.
    nrf_pwm_mode_t     count_mode;   ///< Operating mode of the pulse generator counter.
    uint16_t           top_value;    ///< Value up to which the pulse generator counter counts.
    nrf_pwm_dec_load_t load_mode;    ///< Mode of loading sequence data from RAM.
    nrf_pwm_dec_step_t step_mode;    ///< Mode of advancing the active sequence.
} nrf_drv_pwm_config_t;

/**
 * @brief PWM driver default configuration.
 */
#define NRF_DRV_PWM_DEFAULT_CONFIG(id)                       \
{                                                            \
    .output_pins  = { CONCAT_3(PWM, id, _CONFIG_OUT0_PIN),   \
                      CONCAT_3(PWM, id, _CONFIG_OUT1_PIN),   \
                      CONCAT_3(PWM, id, _CONFIG_OUT2_PIN),   \
                      CONCAT_3(PWM, id, _CONFIG_OUT3_PIN) }, \
    .irq_priority = CONCAT_3(PWM, id, _CONFIG_IRQ_PRIORITY), \
    .base_clock   = CONCAT_3(PWM, id, _CONFIG_BASE_CLOCK),   \
    .count_mode   = CONCAT_3(PWM, id, _CONFIG_COUNT_MODE),   \
    .top_value    = CONCAT_3(PWM, id, _CONFIG_TOP_VALUE),    \
    .load_mode    = CONCAT_3(PWM, id, _CONFIG_LOAD_MODE),    \
    .step_mode    = CONCAT_3(PWM, id, _CONFIG_STEP_MODE),    \
}


/**
 * @brief PWM flags providing additional playback options.
 */
typedef enum
{
    NRF_DRV_PWM_FLAG_STOP = 0x01, /**< When the requested playback is finished,
                                       the peripheral should be stopped.
                                       @note The STOP task is triggered when
                                       the last value of the final sequence is
                                       loaded from RAM, and the peripheral stops
                                       at the end of the current PWM period.
                                       For sequences with configured repeating
                                       of duty cycle values, this might result in
                                       less than the requested number of repeats
                                       of the last value. */
    NRF_DRV_PWM_FLAG_LOOP = 0x02, /**< When the requested playback is finished,
                                       it should be started from the beginning.
                                       This flag is ignored if used together
                                       with @ref NRF_DRV_PWM_FLAG_STOP. */
    NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ0 = 0x04, /**< The event handler should be
                                                  called when the last value
                                                  from sequence 0 is loaded. */
    NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ1 = 0x08, /**< The event handler should be
                                                  called when the last value
                                                  from sequence 1 is loaded. */
    NRF_DRV_PWM_FLAG_NO_EVT_FINISHED = 0x10, /**< The playback finished event
                                                  (enabled by default) should be
                                                  suppressed. */
} nrf_drv_pwm_flag_t;


/**
 * @brief PWM driver event type.
 */
typedef enum
{
    NRF_DRV_PWM_EVT_FINISHED, ///< Sequence playback finished.
    NRF_DRV_PWM_EVT_END_SEQ0, /**< End of sequence 0 reached. Its data can be
                                   safely modified now. */
    NRF_DRV_PWM_EVT_END_SEQ1, /**< End of sequence 1 reached. Its data can be
                                   safely modified now. */
    NRF_DRV_PWM_EVT_STOPPED,  ///< The PWM peripheral has been stopped.
} nrf_drv_pwm_evt_type_t;

/**
 * @brief PWM driver event handler type.
 */
typedef void (* nrf_drv_pwm_handler_t)(nrf_drv_pwm_evt_type_t event_type);


/**
 * @brief Function for initializing the PWM driver.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] p_config   Pointer to the structure with initial configuration.
 *                       If NULL, the default configuration is used.
 * @param[in] handler    Event handler provided by the user. If NULL is passed
 *                       instead, event notifications are not done and PWM
 *                       interrupts are disabled.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 * @retval NRF_ERROR_INVALID_STATE If the driver was already initialized.
 */
ret_code_t nrf_drv_pwm_init(nrf_drv_pwm_t const * const p_instance,
                            nrf_drv_pwm_config_t const * p_config,
                            nrf_drv_pwm_handler_t        handler);

/**
 * @brief Function for uninitializing the PWM driver.
 *
 * If any sequence playback is in progress, it is stopped immediately.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 */
void nrf_drv_pwm_uninit(nrf_drv_pwm_t const * const p_instance);

/**
 * @brief Function for starting a single sequence playback.
 *
 * To take advantage of the looping mechanism in the PWM peripheral, both
 * sequences must be used (single sequence can be played back only once by
 * the peripheral). Therefore, the provided sequence is internally set and
 * played back as both sequence 0 and sequence 1. Consequently, if end of
 * sequence notifications are required, events for both sequences should be
 * used (that means that both the @ref NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ0 flag
 * and the @ref NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ1 flag should be specified and
 * the @ref NRF_DRV_PWM_EVT_END_SEQ0 event and the @ref NRF_DRV_PWM_EVT_END_SEQ1
 * event should be handled in the same way).
 *
 * @note The array containing the duty cycle values for the specified sequence
 *       must be in RAM and cannot be allocated on stack.
 *       For detailed information, see @ref nrf_pwm_sequence_t.
 *
 * @param[in] p_instance     Pointer to the driver instance structure.
 * @param[in] p_sequence     Sequence to be played back.
 * @param[in] playback_count Number of playbacks to be performed (must not be 0).
 * @param[in] flags          Additional options. Pass any combination of
 *                           @ref nrf_drv_pwm_flag_t "playback flags", or 0
 *                           for default settings.
 */
void nrf_drv_pwm_simple_playback(nrf_drv_pwm_t const * const p_instance,
                                 nrf_pwm_sequence_t const * p_sequence,
                                 uint16_t                   playback_count,
                                 uint32_t                   flags);

/**
 * @brief Function for starting a two-sequence playback.
 *
 * @note The array containing the duty cycle values for the specified sequence
 *       must be in RAM and cannot be allocated on stack.
 *       For detailed information, see @ref nrf_pwm_sequence_t.
 *
 * @param[in] p_instance     Pointer to the driver instance structure.
 * @param[in] p_sequence_0   First sequence to be played back.
 * @param[in] p_sequence_1   Second sequence to be played back.
 * @param[in] playback_count Number of playbacks to be performed (must not be 0).
 * @param[in] flags          Additional options. Pass any combination of
 *                           @ref nrf_drv_pwm_flag_t "playback flags", or 0
 *                           for default settings.
 */
void nrf_drv_pwm_complex_playback(nrf_drv_pwm_t const * const p_instance,
                                  nrf_pwm_sequence_t const * p_sequence_0,
                                  nrf_pwm_sequence_t const * p_sequence_1,
                                  uint16_t                   playback_count,
                                  uint32_t                   flags);

/**
 * @brief Function for advancing the active sequence.
 *
 * This function only applies to @ref NRF_PWM_STEP_TRIGGERED mode.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 */
__STATIC_INLINE void nrf_drv_pwm_step(nrf_drv_pwm_t const * const p_instance);

/**
 * @brief Function for stopping the sequence playback.
 *
 * The playback is stopped at the end of the current PWM period.
 * This means that if the active sequence is configured to repeat each duty
 * cycle value for a certain number of PWM periods, the last played value
 * might appear on the output less times than requested.
 *
 * @note This function can be instructed to wait until the playback is stopped
 *       (by setting @p wait_until_stopped to true). Note that, depending on
 *       the length of the PMW period, this might take a significant amount of
 *       time. Alternatively, the @ref nrf_drv_pwm_is_stopped function can be
 *       used to poll the status, or the @ref NRF_DRV_PWM_EVT_STOPPED event can
 *       be used to get the notification when the playback is stopped, provided
 *       the event handler is defined.
 *
 * @param[in] p_instance         Pointer to the driver instance structure.
 * @param[in] wait_until_stopped If true, the function will not return until
 *                               the playback is stopped.
 *
 * @retval true  If the PWM peripheral is stopped.
 * @retval false If the PWM peripheral is not stopped.
 */
bool nrf_drv_pwm_stop(nrf_drv_pwm_t const * const p_instance,
                      bool wait_until_stopped);

/**
 * @brief Function for checking the status of the PWM peripheral.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 *
 * @retval true  If the PWM peripheral is stopped.
 * @retval false If the PWM peripheral is not stopped.
 */
bool nrf_drv_pwm_is_stopped(nrf_drv_pwm_t const * const p_instance);

/**
 * @brief Function for updating the sequence data during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] p_sequence Pointer to the new sequence definition.
 */
__STATIC_INLINE void nrf_drv_pwm_sequence_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t                    seq_id,
                                        nrf_pwm_sequence_t const * p_sequence);

/**
 * @brief Function for updating the pointer to the duty cycle values
 *        in the specified sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] values     New pointer to the duty cycle values.
 */
__STATIC_INLINE void nrf_drv_pwm_sequence_values_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t          seq_id,
                                        nrf_pwm_values_t values);

/**
 * @brief Function for updating the number of duty cycle values
 *        in the specified sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] length     New number of the duty cycle values.
 */
__STATIC_INLINE void nrf_drv_pwm_sequence_length_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t  seq_id,
                                        uint16_t length);

/**
 * @brief Function for updating the number of repeats for duty cycle values
 *        in specified sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] repeats    New number of repeats.
 */
__STATIC_INLINE void nrf_drv_pwm_sequence_repeats_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t  seq_id,
                                        uint32_t repeats);

/**
 * @brief Function for updating the additional delay after the specified
 *        sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] end_delay  New end delay value (in PWM periods).
 */
__STATIC_INLINE void nrf_drv_pwm_sequence_end_delay_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t  seq_id,
                                        uint32_t end_delay);

/**
 * @brief Function for returning the address of a specified PWM task that can
 *        be used in PPI module.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] task       Requested task.
 *
 * @return Task address.
 */
__STATIC_INLINE uint32_t nrf_drv_pwm_task_address_get(
                                        nrf_drv_pwm_t const * const p_instance,
                                        nrf_pwm_task_t task);

/**@brief Function for returning the address of a specified PWM event that can
 *        be used in PPI module.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] event      Requested event.
 *
 * @return Event address.
 */
__STATIC_INLINE uint32_t nrf_drv_pwm_event_address_get(
                                        nrf_drv_pwm_t const * const p_instance,
                                        nrf_pwm_event_t event);


#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE void nrf_drv_pwm_step(nrf_drv_pwm_t const * const p_instance)
{
    nrf_pwm_task_trigger(p_instance->p_registers, NRF_PWM_TASK_NEXTSTEP);
}

__STATIC_INLINE void nrf_drv_pwm_sequence_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t                    seq_id,
                                        nrf_pwm_sequence_t const * p_sequence)
{
    nrf_pwm_sequence_set(p_instance->p_registers, seq_id, p_sequence);
}

__STATIC_INLINE void nrf_drv_pwm_sequence_values_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t          seq_id,
                                        nrf_pwm_values_t values)
{
    nrf_pwm_seq_ptr_set(p_instance->p_registers, seq_id, values.p_raw);
}

__STATIC_INLINE void nrf_drv_pwm_sequence_length_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t  seq_id,
                                        uint16_t length)
{
    nrf_pwm_seq_cnt_set(p_instance->p_registers, seq_id, length);
}

__STATIC_INLINE void nrf_drv_pwm_sequence_repeats_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t  seq_id,
                                        uint32_t repeats)
{
    nrf_pwm_seq_refresh_set(p_instance->p_registers, seq_id, repeats);
}

__STATIC_INLINE void nrf_drv_pwm_sequence_end_delay_update(
                                        nrf_drv_pwm_t const * const p_instance,
                                        uint8_t  seq_id,
                                        uint32_t end_delay)
{
    nrf_pwm_seq_end_delay_set(p_instance->p_registers, seq_id, end_delay);
}

__STATIC_INLINE uint32_t nrf_drv_pwm_task_address_get(
                                        nrf_drv_pwm_t const * const p_instance,
                                        nrf_pwm_task_t task)
{
    return nrf_pwm_task_address_get(p_instance->p_registers, task);
}

__STATIC_INLINE uint32_t nrf_drv_pwm_event_address_get(
                                        nrf_drv_pwm_t const * const p_instance,
                                        nrf_pwm_event_t event)
{
    return nrf_pwm_event_address_get(p_instance->p_registers, event);
}

#endif // SUPPRESS_INLINE_IMPLEMENTATION

#endif // NRF_DRV_PWM_H__

/** @} */
