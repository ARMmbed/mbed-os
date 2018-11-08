/*
 * Copyright (c) 2000 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @defgroup nrf_soc_api SoC Library API
 * @{
 *
 * @brief APIs for the SoC library.
 *
 */

#ifndef NRF_SOC_H__
#define NRF_SOC_H__

#include <stdint.h>
#include <stdbool.h>
#include "nrf_svc.h"
#include "nrf.h"

#include "nrf_error_soc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@addtogroup NRF_SOC_DEFINES Defines
 * @{ */

/**@brief The number of the lowest SVC number reserved for the SoC library. */
#define SOC_SVC_BASE               (0x20)
#define SOC_SVC_BASE_NOT_AVAILABLE (0x2B)

/**@brief Guranteed time for application to process radio inactive notification. */
#define NRF_RADIO_NOTIFICATION_INACTIVE_GUARANTEED_TIME_US  (62)

/**@brief The minimum allowed timeslot extension time. */
#define NRF_RADIO_MINIMUM_TIMESLOT_LENGTH_EXTENSION_TIME_US (200)

#define SOC_ECB_KEY_LENGTH                (16)                       /**< ECB key length. */
#define SOC_ECB_CLEARTEXT_LENGTH          (16)                       /**< ECB cleartext length. */
#define SOC_ECB_CIPHERTEXT_LENGTH         (SOC_ECB_CLEARTEXT_LENGTH) /**< ECB ciphertext length. */

#ifdef NRF51
#define SD_EVT_IRQn                       (SWI2_IRQn)        /**< SoftDevice Event IRQ number. Used for both protocol events and SoC events. */
#define SD_EVT_IRQHandler                 (SWI2_IRQHandler)  /**< SoftDevice Event IRQ handler. Used for both protocol events and SoC events. */
#define RADIO_NOTIFICATION_IRQn           (SWI1_IRQn)        /**< The radio notification IRQ number. */
#define RADIO_NOTIFICATION_IRQHandler     (SWI1_IRQHandler)  /**< The radio notification IRQ handler. */
#endif
#ifdef NRF52
#define SD_EVT_IRQn                       (SWI2_EGU2_IRQn)        /**< SoftDevice Event IRQ number. Used for both protocol events and SoC events. */
#define SD_EVT_IRQHandler                 (SWI2_EGU2_IRQHandler)  /**< SoftDevice Event IRQ handler. Used for both protocol events and SoC events. */
#define RADIO_NOTIFICATION_IRQn           (SWI1_EGU1_IRQn)        /**< The radio notification IRQ number. */
#define RADIO_NOTIFICATION_IRQHandler     (SWI1_EGU1_IRQHandler)  /**< The radio notification IRQ handler. */
#endif

#define NRF_RADIO_LENGTH_MIN_US           (100)               /**< The shortest allowed radio timeslot, in microseconds. */
#define NRF_RADIO_LENGTH_MAX_US           (100000)            /**< The longest allowed radio timeslot, in microseconds. */

#define NRF_RADIO_DISTANCE_MAX_US         (128000000UL - 1UL) /**< The longest timeslot distance, in microseconds, allowed for the distance parameter (see @ref nrf_radio_request_normal_t) in the request. */

#define NRF_RADIO_EARLIEST_TIMEOUT_MAX_US (128000000UL - 1UL) /**< The longest timeout, in microseconds, allowed when requesting the earliest possible timeslot. */

#define NRF_RADIO_START_JITTER_US         (2)                 /**< The maximum jitter in @ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START relative to the requested start time. */

/**@} */

/**@addtogroup NRF_SOC_ENUMS Enumerations
 * @{ */

/**@brief The SVC numbers used by the SVC functions in the SoC library. */
enum NRF_SOC_SVCS {
    SD_PPI_CHANNEL_ENABLE_GET = SOC_SVC_BASE,
    SD_PPI_CHANNEL_ENABLE_SET,
    SD_PPI_CHANNEL_ENABLE_CLR,
    SD_PPI_CHANNEL_ASSIGN,
    SD_PPI_GROUP_TASK_ENABLE,
    SD_PPI_GROUP_TASK_DISABLE,
    SD_PPI_GROUP_ASSIGN,
    SD_PPI_GROUP_GET,
    SD_FLASH_PAGE_ERASE,
    SD_FLASH_WRITE,
    SD_FLASH_PROTECT,
    SD_MUTEX_NEW = SOC_SVC_BASE_NOT_AVAILABLE,
    SD_MUTEX_ACQUIRE,
    SD_MUTEX_RELEASE,
    SD_RFU_1,
    SD_RFU_2,
    SD_RFU_3,
    SD_RFU_4,
    SD_RFU_5,
    SD_RFU_6,
    SD_RFU_7,
    SD_RFU_8,
    SD_RFU_9,
    SD_RFU_10,
    SD_RAND_APPLICATION_POOL_CAPACITY_GET,
    SD_RAND_APPLICATION_BYTES_AVAILABLE_GET,
    SD_RAND_APPLICATION_VECTOR_GET,
    SD_POWER_MODE_SET,
    SD_POWER_SYSTEM_OFF,
    SD_POWER_RESET_REASON_GET,
    SD_POWER_RESET_REASON_CLR,
    SD_POWER_POF_ENABLE,
    SD_POWER_POF_THRESHOLD_SET,
    SD_POWER_RAMON_SET,
    SD_POWER_RAMON_CLR,
    SD_POWER_RAMON_GET,
    SD_POWER_GPREGRET_SET,
    SD_POWER_GPREGRET_CLR,
    SD_POWER_GPREGRET_GET,
    SD_POWER_DCDC_MODE_SET,
    SD_APP_EVT_WAIT,
    SD_CLOCK_HFCLK_REQUEST,
    SD_CLOCK_HFCLK_RELEASE,
    SD_CLOCK_HFCLK_IS_RUNNING,
    SD_RADIO_NOTIFICATION_CFG_SET,
    SD_ECB_BLOCK_ENCRYPT,
    SD_ECB_BLOCKS_ENCRYPT,
    SD_RADIO_SESSION_OPEN,
    SD_RADIO_SESSION_CLOSE,
    SD_RADIO_REQUEST,
    SD_EVT_GET,
    SD_TEMP_GET,
    SVC_SOC_LAST
};

/**@brief Possible values of a ::nrf_mutex_t. */
enum NRF_MUTEX_VALUES {
    NRF_MUTEX_FREE,
    NRF_MUTEX_TAKEN
};

/**@brief Power modes. */
enum NRF_POWER_MODES {
    NRF_POWER_MODE_CONSTLAT,  /**< Constant latency mode. See power management in the reference manual. */
    NRF_POWER_MODE_LOWPWR     /**< Low power mode. See power management in the reference manual. */
};


/**@brief Power failure thresholds */
enum NRF_POWER_THRESHOLDS {
    NRF_POWER_THRESHOLD_V21,  /**< 2.1 Volts power failure threshold. */
    NRF_POWER_THRESHOLD_V23,  /**< 2.3 Volts power failure threshold. */
    NRF_POWER_THRESHOLD_V25,  /**< 2.5 Volts power failure threshold. */
    NRF_POWER_THRESHOLD_V27   /**< 2.7 Volts power failure threshold. */
};


/**@brief DC/DC converter modes. */
enum NRF_POWER_DCDC_MODES {
    NRF_POWER_DCDC_DISABLE,          /**< The DCDC is disabled. */
    NRF_POWER_DCDC_ENABLE            /**< The DCDC is enabled.  */
};

/**@brief Radio notification distances. */
enum NRF_RADIO_NOTIFICATION_DISTANCES {
    NRF_RADIO_NOTIFICATION_DISTANCE_NONE = 0, /**< The event does not have a notification. */
    NRF_RADIO_NOTIFICATION_DISTANCE_800US,    /**< The distance from the active notification to start of radio activity. */
    NRF_RADIO_NOTIFICATION_DISTANCE_1740US,   /**< The distance from the active notification to start of radio activity. */
    NRF_RADIO_NOTIFICATION_DISTANCE_2680US,   /**< The distance from the active notification to start of radio activity. */
    NRF_RADIO_NOTIFICATION_DISTANCE_3620US,   /**< The distance from the active notification to start of radio activity. */
    NRF_RADIO_NOTIFICATION_DISTANCE_4560US,   /**< The distance from the active notification to start of radio activity. */
    NRF_RADIO_NOTIFICATION_DISTANCE_5500US    /**< The distance from the active notification to start of radio activity. */
};


/**@brief Radio notification types. */
enum NRF_RADIO_NOTIFICATION_TYPES {
    NRF_RADIO_NOTIFICATION_TYPE_NONE = 0,        /**< The event does not have a radio notification signal. */
    NRF_RADIO_NOTIFICATION_TYPE_INT_ON_ACTIVE,   /**< Using interrupt for notification when the radio will be enabled. */
    NRF_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE, /**< Using interrupt for notification when the radio has been disabled. */
    NRF_RADIO_NOTIFICATION_TYPE_INT_ON_BOTH,     /**< Using interrupt for notification both when the radio will be enabled and disabled. */
};

/**@brief The Radio signal callback types. */
enum NRF_RADIO_CALLBACK_SIGNAL_TYPE {
    NRF_RADIO_CALLBACK_SIGNAL_TYPE_START,             /**< This signal indicates the start of the radio timeslot. */
    NRF_RADIO_CALLBACK_SIGNAL_TYPE_TIMER0,            /**< This signal indicates the NRF_TIMER0 interrupt. */
    NRF_RADIO_CALLBACK_SIGNAL_TYPE_RADIO,             /**< This signal indicates the NRF_RADIO interrupt. */
    NRF_RADIO_CALLBACK_SIGNAL_TYPE_EXTEND_FAILED,     /**< This signal indicates extend action failed. */
    NRF_RADIO_CALLBACK_SIGNAL_TYPE_EXTEND_SUCCEEDED   /**< This signal indicates extend action succeeded. */
};

/**@brief The actions requested by the signal callback.
 *
 *  This code gives the SOC instructions about what action to take when the signal callback has
 *  returned.
 */
enum NRF_RADIO_SIGNAL_CALLBACK_ACTION {
    NRF_RADIO_SIGNAL_CALLBACK_ACTION_NONE,            /**< Return without action. */
    NRF_RADIO_SIGNAL_CALLBACK_ACTION_EXTEND,          /**< Request an extension of the current timeslot (maximum execution time for this action is when the extension succeeded). */
    NRF_RADIO_SIGNAL_CALLBACK_ACTION_END,             /**< End the current radio timeslot. */
    NRF_RADIO_SIGNAL_CALLBACK_ACTION_REQUEST_AND_END  /**< Request a new radio timeslot and end the current timeslot. */
};

/**@brief Radio timeslot high frequency clock source configuration. */
enum NRF_RADIO_HFCLK_CFG {
    NRF_RADIO_HFCLK_CFG_XTAL_GUARANTEED, /**< The SoftDevice will guarantee that the high frequency clock source is the
                                           external crystal for the whole duration of the timeslot. This should be the
                                           preferred option for events that use the radio or require high timing accuracy. */
    NRF_RADIO_HFCLK_CFG_NO_GUARANTEE    /**< This configuration allows for earlier and tighter scheduling of timeslots.
                                           The RC oscillator may be the clock source in part or for the whole duration of the timeslot.
                                           The RC oscillator's accuracy must therefore be taken into consideration.
                                           @note If the application will use the radio peripheral in timeslots with this configuration,
                                           it must make sure that the crystal is running and stable before starting the radio. */
};

/**@brief Radio timeslot priorities. */
enum NRF_RADIO_PRIORITY {
    NRF_RADIO_PRIORITY_HIGH,                          /**< High (equal priority as the normal connection priority of the SoftDevice stack(s)). */
    NRF_RADIO_PRIORITY_NORMAL,                        /**< Normal (equal priority as the priority of secondary activites of the SoftDevice stack(s)). */
};

/**@brief Radio timeslot request type. */
enum NRF_RADIO_REQUEST_TYPE {
    NRF_RADIO_REQ_TYPE_EARLIEST,                      /**< Request radio timeslot as early as possible. This should always be used for the first request in a session. */
    NRF_RADIO_REQ_TYPE_NORMAL                         /**< Normal radio timeslot request. */
};

/**@brief SoC Events. */
enum NRF_SOC_EVTS {
    NRF_EVT_HFCLKSTARTED,                         /**< Event indicating that the HFCLK has started. */
    NRF_EVT_POWER_FAILURE_WARNING,                /**< Event indicating that a power failure warning has occurred. */
    NRF_EVT_FLASH_OPERATION_SUCCESS,              /**< Event indicating that the ongoing flash operation has completed successfully. */
    NRF_EVT_FLASH_OPERATION_ERROR,                /**< Event indicating that the ongoing flash operation has timed out with an error. */
    NRF_EVT_RADIO_BLOCKED,                        /**< Event indicating that a radio timeslot was blocked. */
    NRF_EVT_RADIO_CANCELED,                       /**< Event indicating that a radio timeslot was canceled by SoftDevice. */
    NRF_EVT_RADIO_SIGNAL_CALLBACK_INVALID_RETURN, /**< Event indicating that a radio timeslot signal callback handler return was invalid. */
    NRF_EVT_RADIO_SESSION_IDLE,                   /**< Event indicating that a radio timeslot session is idle. */
    NRF_EVT_RADIO_SESSION_CLOSED,                 /**< Event indicating that a radio timeslot session is closed. */
    NRF_EVT_NUMBER_OF_EVTS
};

/**@} */


/**@addtogroup NRF_SOC_STRUCTURES Structures
 * @{ */

/**@brief Represents a mutex for use with the nrf_mutex functions.
 * @note Accessing the value directly is not safe, use the mutex functions!
 */
typedef volatile uint8_t nrf_mutex_t;

/**@brief Parameters for a request for a timeslot as early as possible. */
typedef struct {
    uint8_t       hfclk;                              /**< High frequency clock source, see @ref NRF_RADIO_HFCLK_CFG. */
    uint8_t       priority;                           /**< The radio timeslot priority, see @ref NRF_RADIO_PRIORITY. */
    uint32_t      length_us;                          /**< The radio timeslot length (in the range 100 to 100,000] microseconds). */
    uint32_t      timeout_us;                         /**< Longest acceptable delay until the start of the requested timeslot (up to @ref NRF_RADIO_EARLIEST_TIMEOUT_MAX_US microseconds). */
} nrf_radio_request_earliest_t;

/**@brief Parameters for a normal radio timeslot request. */
typedef struct {
    uint8_t       hfclk;                              /**< High frequency clock source, see @ref NRF_RADIO_HFCLK_CFG. */
    uint8_t       priority;                           /**< The radio timeslot priority, see @ref NRF_RADIO_PRIORITY. */
    uint32_t      distance_us;                        /**< Distance from the start of the previous radio timeslot (up to @ref NRF_RADIO_DISTANCE_MAX_US microseconds). */
    uint32_t      length_us;                          /**< The radio timeslot length (in the range [100..100,000] microseconds). */
} nrf_radio_request_normal_t;

/**@brief Radio timeslot request parameters. */
typedef struct {
    uint8_t                         request_type;     /**< Type of request, see @ref NRF_RADIO_REQUEST_TYPE. */
    union {
        nrf_radio_request_earliest_t  earliest;         /**< Parameters for requesting a radio timeslot as early as possible. */
        nrf_radio_request_normal_t    normal;           /**< Parameters for requesting a normal radio timeslot. */
    } params;
} nrf_radio_request_t;

/**@brief Return parameters of the radio timeslot signal callback. */
typedef struct {
    uint8_t               callback_action;            /**< The action requested by the application when returning from the signal callback, see @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION. */
    union {
        struct {
            nrf_radio_request_t *p_next;                  /**< The request parameters for the next radio timeslot. */
        } request;                                      /**< Additional parameters for return_code @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION_REQUEST_AND_END. */
        struct {
            uint32_t              length_us;              /**< Requested extension of the radio timeslot duration (microseconds) (for minimum time see @ref NRF_RADIO_MINIMUM_TIMESLOT_LENGTH_EXTENSION_TIME_US). */
        } extend;                                       /**< Additional parameters for return_code @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION_EXTEND. */
    } params;
} nrf_radio_signal_callback_return_param_t;

/**@brief The radio timeslot signal callback type.
 *
 * @note In case of invalid return parameters, the radio timeslot will automatically end
 *       immediately after returning from the signal callback and the
 *       @ref NRF_EVT_RADIO_SIGNAL_CALLBACK_INVALID_RETURN event will be sent.
 * @note The returned struct pointer must remain valid after the signal callback
 *       function returns. For instance, this means that it must not point to a stack variable.
 *
 * @param[in] signal_type Type of signal, see @ref NRF_RADIO_CALLBACK_SIGNAL_TYPE.
 *
 * @return Pointer to structure containing action requested by the application.
 */
typedef nrf_radio_signal_callback_return_param_t *(*nrf_radio_signal_callback_t)(uint8_t signal_type);

/**@brief AES ECB parameter typedefs */
typedef uint8_t soc_ecb_key_t[SOC_ECB_KEY_LENGTH];
typedef uint8_t soc_ecb_cleartext_t[SOC_ECB_CLEARTEXT_LENGTH];
typedef uint8_t soc_ecb_ciphertext_t[SOC_ECB_CIPHERTEXT_LENGTH];

/**@brief AES ECB data structure */
typedef struct {
    soc_ecb_key_t        key;            /**< Encryption key. */
    soc_ecb_cleartext_t  cleartext;      /**< Cleartext data. */
    soc_ecb_ciphertext_t ciphertext;     /**< Ciphertext data. */
} nrf_ecb_hal_data_t;

/**@brief AES ECB block. Used to provide multiple blocks in a single call
          to @ref sd_ecb_blocks_encrypt.*/
typedef struct {
    soc_ecb_key_t        *p_key;           /**< Pointer to the Encryption key. */
    soc_ecb_cleartext_t  *p_cleartext;     /**< Pointer to the Cleartext data. */
    soc_ecb_ciphertext_t *p_ciphertext;    /**< Pointer to the Ciphertext data. */
} nrf_ecb_hal_data_block_t;

/**@} */

/**@addtogroup NRF_SOC_FUNCTIONS Functions
 * @{ */

/**@brief Initialize a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to initialize.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_MUTEX_NEW, uint32_t, sd_mutex_new(nrf_mutex_t *p_mutex));

/**@brief Attempt to acquire a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to acquire.
 *
 * @retval ::NRF_SUCCESS The mutex was successfully acquired.
 * @retval ::NRF_ERROR_SOC_MUTEX_ALREADY_TAKEN The mutex could not be acquired.
 */
SVCALL(SD_MUTEX_ACQUIRE, uint32_t, sd_mutex_acquire(nrf_mutex_t *p_mutex));

/**@brief Release a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to release.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_MUTEX_RELEASE, uint32_t, sd_mutex_release(nrf_mutex_t *p_mutex));

/**@brief Query the capacity of the application random pool.
 *
 * @param[out] p_pool_capacity The capacity of the pool.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_RAND_APPLICATION_POOL_CAPACITY_GET, uint32_t, sd_rand_application_pool_capacity_get(uint8_t *p_pool_capacity));

/**@brief Get number of random bytes available to the application.
 *
 * @param[out] p_bytes_available The number of bytes currently available in the pool.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_RAND_APPLICATION_BYTES_AVAILABLE_GET, uint32_t, sd_rand_application_bytes_available_get(uint8_t *p_bytes_available));

/**@brief Get random bytes from the application pool.
 *
 * @param[out]  p_buff  Pointer to unit8_t buffer for storing the bytes.
 * @param[in]   length  Number of bytes to take from pool and place in p_buff.
 *
 * @retval ::NRF_SUCCESS The requested bytes were written to p_buff.
 * @retval ::NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES No bytes were written to the buffer, because there were not enough bytes available.
*/
SVCALL(SD_RAND_APPLICATION_VECTOR_GET, uint32_t, sd_rand_application_vector_get(uint8_t *p_buff, uint8_t length));

/**@brief Gets the reset reason register.
 *
 * @param[out]  p_reset_reason  Contents of the NRF_POWER->RESETREAS register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RESET_REASON_GET, uint32_t, sd_power_reset_reason_get(uint32_t *p_reset_reason));

/**@brief Clears the bits of the reset reason register.
 *
 * @param[in] reset_reason_clr_msk Contains the bits to clear from the reset reason register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RESET_REASON_CLR, uint32_t, sd_power_reset_reason_clr(uint32_t reset_reason_clr_msk));

/**@brief Sets the power mode when in CPU sleep.
 *
 * @param[in] power_mode The power mode to use when in CPU sleep, see @ref NRF_POWER_MODES. @sa sd_app_evt_wait
 *
 * @retval ::NRF_SUCCESS The power mode was set.
 * @retval ::NRF_ERROR_SOC_POWER_MODE_UNKNOWN The power mode was unknown.
 */
SVCALL(SD_POWER_MODE_SET, uint32_t, sd_power_mode_set(uint8_t power_mode));

/**@brief Puts the chip in System OFF mode.
 *
 * @retval ::NRF_ERROR_SOC_POWER_OFF_SHOULD_NOT_RETURN
 */
SVCALL(SD_POWER_SYSTEM_OFF, uint32_t, sd_power_system_off(void));

/**@brief Enables or disables the power-fail comparator.
 *
 * Enabling this will give a softdevice event (NRF_EVT_POWER_FAILURE_WARNING) when the power failure warning occurs.
 * The event can be retrieved with sd_evt_get();
 *
 * @param[in] pof_enable    True if the power-fail comparator should be enabled, false if it should be disabled.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_POF_ENABLE, uint32_t, sd_power_pof_enable(uint8_t pof_enable));

/**@brief Sets the power-fail threshold value.
 *
 * @param[in] threshold The power-fail threshold value to use, see @ref NRF_POWER_THRESHOLDS.
 *
 * @retval ::NRF_SUCCESS The power failure threshold was set.
 * @retval ::NRF_ERROR_SOC_POWER_POF_THRESHOLD_UNKNOWN The power failure threshold is unknown.
 */
SVCALL(SD_POWER_POF_THRESHOLD_SET, uint32_t, sd_power_pof_threshold_set(uint8_t threshold));

/**@brief Sets bits in the NRF_POWER->RAMON register.
 *
 * @param[in] ramon Contains the bits needed to be set in the NRF_POWER->RAMON register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RAMON_SET, uint32_t, sd_power_ramon_set(uint32_t ramon));

/**@brief Clears bits in the NRF_POWER->RAMON register.
 *
 * @param ramon Contains the bits needed to be cleared in the NRF_POWER->RAMON register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RAMON_CLR, uint32_t, sd_power_ramon_clr(uint32_t ramon));

/**@brief Get contents of NRF_POWER->RAMON register, indicates power status of ram blocks.
 *
 * @param[out] p_ramon Content of NRF_POWER->RAMON register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RAMON_GET, uint32_t, sd_power_ramon_get(uint32_t *p_ramon));

/**@brief Set bits in the NRF_POWER->GPREGRET register.
 *
 * @param[in] gpregret_msk Bits to be set in the GPREGRET register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_GPREGRET_SET, uint32_t, sd_power_gpregret_set(uint32_t gpregret_msk));

/**@brief Clear bits in the NRF_POWER->GPREGRET register.
 *
 * @param[in] gpregret_msk Bits to be clear in the GPREGRET register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_GPREGRET_CLR, uint32_t, sd_power_gpregret_clr(uint32_t gpregret_msk));

/**@brief Get contents of the NRF_POWER->GPREGRET register.
 *
 * @param[out] p_gpregret Contents of the GPREGRET register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_GPREGRET_GET, uint32_t, sd_power_gpregret_get(uint32_t *p_gpregret));

/**@brief Sets the DCDC mode.
 *
 * Enable or disable the DCDC peripheral.
 *
 * @param[in] dcdc_mode The mode of the DCDC, see @ref NRF_POWER_DCDC_MODES.
 *
 * @retval ::NRF_SUCCESS
 * @retval ::NRF_ERROR_INVALID_PARAM The DCDC mode is invalid.
 */
SVCALL(SD_POWER_DCDC_MODE_SET, uint32_t, sd_power_dcdc_mode_set(uint8_t dcdc_mode));

/**@brief Request the high frequency crystal oscillator.
 *
 * Will start the high frequency crystal oscillator, the startup time of the crystal varies
 * and the ::sd_clock_hfclk_is_running function can be polled to check if it has started.
 *
 * @see sd_clock_hfclk_is_running
 * @see sd_clock_hfclk_release
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_CLOCK_HFCLK_REQUEST, uint32_t, sd_clock_hfclk_request(void));

/**@brief Releases the high frequency crystal oscillator.
 *
 * Will stop the high frequency crystal oscillator, this happens immediately.
 *
 * @see sd_clock_hfclk_is_running
 * @see sd_clock_hfclk_request
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_CLOCK_HFCLK_RELEASE, uint32_t, sd_clock_hfclk_release(void));

/**@brief Checks if the high frequency crystal oscillator is running.
 *
 * @see sd_clock_hfclk_request
 * @see sd_clock_hfclk_release
 *
 * @param[out] p_is_running 1 if the external crystal oscillator is running, 0 if not.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_CLOCK_HFCLK_IS_RUNNING, uint32_t, sd_clock_hfclk_is_running(uint32_t *p_is_running));

/**@brief Waits for an application event.
 *
 * An application event is either an application interrupt or a pended interrupt when the
 * interrupt is disabled. When the interrupt is enabled it will be taken immediately since
 * this function will wait in thread mode, then the execution will return in the application's
 * main thread. When an interrupt is disabled and gets pended it will return to the application's
 * thread main. The application must ensure that the pended flag is cleared using
 * ::sd_nvic_ClearPendingIRQ in order to sleep using this function. This is only necessary for
 * disabled interrupts, as the interrupt handler will clear the pending flag automatically for
 * enabled interrupts.
 *
 * In order to wake up from disabled interrupts, the SEVONPEND flag has to be set in the Cortex-M0
 * System Control Register (SCR). @sa CMSIS_SCB
 *
 * @note If an application interrupt has happened since the last time sd_app_evt_wait was
 *       called this function will return immediately and not go to sleep. This is to avoid race
 *       conditions that can occur when a flag is updated in the interrupt handler and processed
 *       in the main loop.
 *
 * @post An application interrupt has happened or a interrupt pending flag is set.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_APP_EVT_WAIT, uint32_t, sd_app_evt_wait(void));

/**@brief Get PPI channel enable register contents.
 *
 * @param[out] p_channel_enable The contents of the PPI CHEN register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_CHANNEL_ENABLE_GET, uint32_t, sd_ppi_channel_enable_get(uint32_t *p_channel_enable));

/**@brief Set PPI channel enable register.
 *
 * @param[in] channel_enable_set_msk Mask containing the bits to set in the PPI CHEN register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_CHANNEL_ENABLE_SET, uint32_t, sd_ppi_channel_enable_set(uint32_t channel_enable_set_msk));

/**@brief Clear PPI channel enable register.
 *
 * @param[in] channel_enable_clr_msk Mask containing the bits to clear in the PPI CHEN register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_CHANNEL_ENABLE_CLR, uint32_t, sd_ppi_channel_enable_clr(uint32_t channel_enable_clr_msk));

/**@brief Assign endpoints to a PPI channel.
 *
 * @param[in] channel_num Number of the PPI channel to assign.
 * @param[in] evt_endpoint Event endpoint of the PPI channel.
 * @param[in] task_endpoint Task endpoint of the PPI channel.
 *
 * @retval ::NRF_ERROR_SOC_PPI_INVALID_CHANNEL The channel number is invalid.
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_CHANNEL_ASSIGN, uint32_t, sd_ppi_channel_assign(uint8_t channel_num, const volatile void *evt_endpoint, const volatile void *task_endpoint));

/**@brief Task to enable a channel group.
 *
 * @param[in] group_num Number of the channel group.
 *
 * @retval ::NRF_ERROR_SOC_PPI_INVALID_GROUP The group number is invalid
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_GROUP_TASK_ENABLE, uint32_t, sd_ppi_group_task_enable(uint8_t group_num));

/**@brief Task to disable a channel group.
 *
 * @param[in] group_num Number of the PPI group.
 *
 * @retval ::NRF_ERROR_SOC_PPI_INVALID_GROUP The group number is invalid.
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_GROUP_TASK_DISABLE, uint32_t, sd_ppi_group_task_disable(uint8_t group_num));

/**@brief Assign PPI channels to a channel group.
 *
 * @param[in] group_num Number of the channel group.
 * @param[in] channel_msk Mask of the channels to assign to the group.
 *
 * @retval ::NRF_ERROR_SOC_PPI_INVALID_GROUP The group number is invalid.
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_GROUP_ASSIGN, uint32_t, sd_ppi_group_assign(uint8_t group_num, uint32_t channel_msk));

/**@brief Gets the PPI channels of a channel group.
 *
 * @param[in]   group_num Number of the channel group.
 * @param[out]  p_channel_msk Mask of the channels assigned to the group.
 *
 * @retval ::NRF_ERROR_SOC_PPI_INVALID_GROUP The group number is invalid.
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_GROUP_GET, uint32_t, sd_ppi_group_get(uint8_t group_num, uint32_t *p_channel_msk));

/**@brief Configures the Radio Notification signal.
 *
 * @note
 *      - The notification signal latency depends on the interrupt priority settings of SWI used
 *        for notification signal.
 *      - To ensure that the radio notification signal behaves in a consistent way, always
 *        configure radio notifications when there is no protocol stack or other SoftDevice
 *        activity in progress. It is recommended that the radio notification signal is
 *        configured directly after the SoftDevice has been enabled.
 *      - In the period between the ACTIVE signal and the start of the Radio Event, the SoftDevice
 *        will interrupt the application to do Radio Event preparation.
 *      - Using the Radio Notification feature may limit the bandwidth, as the SoftDevice may have
 *        to shorten the connection events to have time for the Radio Notification signals.
 *
 * @param[in]  type      Type of notification signal, see @ref NRF_RADIO_NOTIFICATION_TYPES.
 *                       @ref NRF_RADIO_NOTIFICATION_TYPE_NONE shall be used to turn off radio
 *                       notification. Using @ref NRF_RADIO_NOTIFICATION_DISTANCE_NONE is
 *                       recommended (but not required) to be used with
 *                       @ref NRF_RADIO_NOTIFICATION_TYPE_NONE.
 *
 * @param[in]  distance  Distance between the notification signal and start of radio activity, see @ref NRF_RADIO_NOTIFICATION_DISTANCES.
 *                       This parameter is ignored when @ref NRF_RADIO_NOTIFICATION_TYPE_NONE or
 *                       @ref NRF_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE is used.
 *
 * @retval ::NRF_ERROR_INVALID_PARAM The group number is invalid.
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_RADIO_NOTIFICATION_CFG_SET, uint32_t, sd_radio_notification_cfg_set(uint8_t type, uint8_t distance));

/**@brief Encrypts a block according to the specified parameters.
 *
 * 128-bit AES encryption.
 *
 * @note:
 *    - The application may set the SEVONPEND bit in the SCR to 1 to make the SoftDevice sleep while
 *      the ECB is running. The SEVONPEND bit should only be cleared (set to 0) from application
 *      main or low interrupt level.
 *
 * @param[in, out] p_ecb_data Pointer to the ECB parameters' struct (two input
 *                            parameters and one output parameter).
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_ECB_BLOCK_ENCRYPT, uint32_t, sd_ecb_block_encrypt(nrf_ecb_hal_data_t *p_ecb_data));

/**@brief Encrypts multiple data blocks provided as an array of data block structures.
 *
 * @details: Performs 128-bit AES encryption on multiple data blocks
 *
 * @note:
 *    - The application may set the SEVONPEND bit in the SCR to 1 to make the SoftDevice sleep while
 *      the ECB is running. The SEVONPEND bit should only be cleared (set to 0) from application
 *      main or low interrupt level.
 *
 * @param[in]     block_count     Count of blocks in the p_data_blocks array.
 * @param[in,out] p_data_blocks   Pointer to the first entry in a contiguous array of
 *                                @ref nrf_ecb_hal_data_block_t structures.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_ECB_BLOCKS_ENCRYPT, uint32_t, sd_ecb_blocks_encrypt(uint8_t block_count, nrf_ecb_hal_data_block_t *p_data_blocks));

/**@brief Gets any pending events generated by the SoC API.
 *
 * The application should keep calling this function to get events, until ::NRF_ERROR_NOT_FOUND is returned.
 *
 * @param[out] p_evt_id Set to one of the values in @ref NRF_SOC_EVTS, if any events are pending.
 *
 * @retval ::NRF_SUCCESS An event was pending. The event id is written in the p_evt_id parameter.
 * @retval ::NRF_ERROR_NOT_FOUND No pending events.
 */
SVCALL(SD_EVT_GET, uint32_t, sd_evt_get(uint32_t *p_evt_id));

/**@brief Get the temperature measured on the chip
 *
 * This function will block until the temperature measurement is done.
 * It takes around 50us from call to return.
 *
 * @param[out] p_temp Result of temperature measurement. Die temperature in 0.25 degrees celsius.
 *
 * @retval ::NRF_SUCCESS A temperature measurement was done, and the temperature was written to temp
 */
SVCALL(SD_TEMP_GET, uint32_t, sd_temp_get(int32_t *p_temp));

/**@brief Flash Write
*
* Commands to write a buffer to flash
*
* If the SoftDevice is enabled:
*  This call initiates the flash access command, and its completion will be communicated to the
*  application with exactly one of the following events:
*      - @ref NRF_EVT_FLASH_OPERATION_SUCCESS - The command was successfully completed.
*      - @ref NRF_EVT_FLASH_OPERATION_ERROR   - The command could not be started.
*
* If the SoftDevice is not enabled no event will be generated, and this call will return @ref NRF_SUCCESS when the
 * write has been completed
*
* @note
*      - This call takes control over the radio and the CPU during flash erase and write to make sure that
*        they will not interfere with the flash access. This means that all interrupts will be blocked
*        for a predictable time (depending on the NVMC specification in nRF51 Series Reference Manual
*        and the command parameters).
*
*
* @param[in]  p_dst Pointer to start of flash location to be written.
* @param[in]  p_src Pointer to buffer with data to be written.
* @param[in]  size  Number of 32-bit words to write. Maximum size is 256 32-bit words for nRF51 and 1024 for nRF52.
*
* @retval ::NRF_ERROR_INVALID_ADDR   Tried to write to a non existing flash address, or p_dst or p_src was unaligned.
* @retval ::NRF_ERROR_BUSY           The previous command has not yet completed.
* @retval ::NRF_ERROR_INVALID_LENGTH Size was 0, or higher than the maximum allowed size.
* @retval ::NRF_ERROR_FORBIDDEN      Tried to write to or read from protected location.
* @retval ::NRF_SUCCESS              The command was accepted.
*/
SVCALL(SD_FLASH_WRITE, uint32_t, sd_flash_write(uint32_t *const p_dst, uint32_t const *const p_src, uint32_t size));


/**@brief Flash Erase page
*
* Commands to erase a flash page
* If the SoftDevice is enabled:
*  This call initiates the flash access command, and its completion will be communicated to the
*  application with exactly one of the following events:
*      - @ref NRF_EVT_FLASH_OPERATION_SUCCESS - The command was successfully completed.
*      - @ref NRF_EVT_FLASH_OPERATION_ERROR   - The command could not be started.
*
* If the SoftDevice is not enabled no event will be generated, and this call will return @ref NRF_SUCCESS when the
* erase has been completed
*
* @note
*      - This call takes control over the radio and the CPU during flash erase and write to make sure that
*        they will not interfere with the flash access. This means that all interrupts will be blocked
*        for a predictable time (depending on the NVMC specification in nRF51 Series Reference Manual
*        and the command parameters).
*
*
* @param[in]  page_number Pagenumber of the page to erase
* @retval ::NRF_ERROR_INTERNAL      If a new session could not be opened due to an internal error.
* @retval ::NRF_ERROR_INVALID_ADDR  Tried to erase to a non existing flash page.
* @retval ::NRF_ERROR_BUSY          The previous command has not yet completed.
* @retval ::NRF_ERROR_FORBIDDEN     Tried to erase a protected page.
* @retval ::NRF_SUCCESS             The command was accepted.
*/
SVCALL(SD_FLASH_PAGE_ERASE, uint32_t, sd_flash_page_erase(uint32_t page_number));


/**@brief Flash Protection set
 *
 * Commands to set the flash protection configuration registers.
   On nRF51 this sets the PROTENSETx registers of the MPU peripheral.
   On nRF52 this sets the CONFIGx registers of the BPROT peripheral.
 *
 * @note To read the values read them directly. They are only write-protected.
 *
 * @param[in]  block_cfg0 Value to be written to the configuration register.
 * @param[in]  block_cfg1 Value to be written to the configuration register.
 * @param[in]  block_cfg2 Value to be written to the configuration register (ignored on nRF51).
 * @param[in]  block_cfg3 Value to be written to the configuration register (ignored on nRF51).
 *
 * @retval ::NRF_ERROR_FORBIDDEN Tried to protect the SoftDevice.
 * @retval ::NRF_SUCCESS Values successfully written to configuration registers.
 */
SVCALL(SD_FLASH_PROTECT, uint32_t, sd_flash_protect(uint32_t block_cfg0, uint32_t block_cfg1, uint32_t block_cfg2, uint32_t block_cfg3));

/**@brief Opens a session for radio timeslot requests.
 *
 * @note Only one session can be open at a time.
 * @note p_radio_signal_callback(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START) will be called when the radio timeslot
 *       starts. From this point the NRF_RADIO and NRF_TIMER0 peripherals can be freely accessed
 *       by the application.
 * @note p_radio_signal_callback(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_TIMER0) is called whenever the NRF_TIMER0
 *       interrupt occurs.
 * @note p_radio_signal_callback(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_RADIO) is called whenever the NRF_RADIO
 *       interrupt occurs.
 * @note p_radio_signal_callback() will be called at ARM interrupt priority level 0. This
 *       implies that none of the sd_* API calls can be used from p_radio_signal_callback().
 *
 * @param[in] p_radio_signal_callback The signal callback.
 *
 * @retval ::NRF_ERROR_INVALID_ADDR p_radio_signal_callback is an invalid function pointer.
 * @retval ::NRF_ERROR_BUSY If session cannot be opened.
 * @retval ::NRF_ERROR_INTERNAL If a new session could not be opened due to an internal error.
 * @retval ::NRF_SUCCESS Otherwise.
 */
SVCALL(SD_RADIO_SESSION_OPEN, uint32_t, sd_radio_session_open(nrf_radio_signal_callback_t p_radio_signal_callback));

/**@brief Closes a session for radio timeslot requests.
 *
 * @note Any current radio timeslot will be finished before the session is closed.
 * @note If a radio timeslot is scheduled when the session is closed, it will be canceled.
 * @note The application cannot consider the session closed until the @ref NRF_EVT_RADIO_SESSION_CLOSED
 *       event is received.
 *
 * @retval ::NRF_ERROR_FORBIDDEN If session not opened.
 * @retval ::NRF_ERROR_BUSY If session is currently being closed.
 * @retval ::NRF_SUCCESS Otherwise.
 */
SVCALL(SD_RADIO_SESSION_CLOSE, uint32_t, sd_radio_session_close(void));

/**@brief Requests a radio timeslot.
 *
 * @note The request type is determined by p_request->request_type, and can be one of @ref NRF_RADIO_REQ_TYPE_EARLIEST
 *       and @ref NRF_RADIO_REQ_TYPE_NORMAL. The first request in a session must always be of type @ref NRF_RADIO_REQ_TYPE_EARLIEST.
 * @note For a normal request (@ref NRF_RADIO_REQ_TYPE_NORMAL), the start time of a radio timeslot is specified by
 *       p_request->distance_us and is given relative to the start of the previous timeslot.
 * @note A too small p_request->distance_us will lead to a @ref NRF_EVT_RADIO_BLOCKED event.
 * @note Timeslots scheduled too close will lead to a @ref NRF_EVT_RADIO_BLOCKED event.
 * @note See the SoftDevice Specification for more on radio timeslot scheduling, distances and lengths.
 * @note If an opportunity for the first radio timeslot is not found before 100ms after the call to this
 *       function, it is not scheduled, and instead a @ref NRF_EVT_RADIO_BLOCKED event is sent.
 *       The application may then try to schedule the first radio timeslot again.
 * @note Successful requests will result in nrf_radio_signal_callback_t(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START).
 *       Unsuccessful requests will result in a @ref NRF_EVT_RADIO_BLOCKED event, see @ref NRF_SOC_EVTS.
 * @note The jitter in the start time of the radio timeslots is +/- @ref NRF_RADIO_START_JITTER_US us.
 * @note The nrf_radio_signal_callback_t(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START) call has a latency relative to the
 *       specified radio timeslot start, but this does not affect the actual start time of the timeslot.
 * @note NRF_TIMER0 is reset at the start of the radio timeslot, and is clocked at 1MHz from the high frequency
 *       (16 MHz) clock source. If p_request->hfclk_force_xtal is true, the high frequency clock is
 *       guaranteed to be clocked from the external crystal.
 * @note The SoftDevice will neither access the NRF_RADIO peripheral nor the NRF_TIMER0 peripheral
 *       during the radio timeslot.
 *
 * @param[in] p_request Pointer to the request parameters.
 *
 * @retval ::NRF_ERROR_FORBIDDEN If session not opened or the session is not IDLE.
 * @retval ::NRF_ERROR_INVALID_ADDR If the p_request pointer is invalid.
 * @retval ::NRF_ERROR_INVALID_PARAM If the parameters of p_request are not valid.
 * @retval ::NRF_SUCCESS Otherwise.
 */
SVCALL(SD_RADIO_REQUEST, uint32_t, sd_radio_request(nrf_radio_request_t *p_request));

/**@} */

#ifdef __cplusplus
}
#endif
#endif // NRF_SOC_H__

/**@} */
