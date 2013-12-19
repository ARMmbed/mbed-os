/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
 
/**
  @defgroup nrf_soc_api SoC Library API
  @{
  
  @brief APIs for the SoC library.
  
*/

#ifndef NRF_SOC_H__
#define NRF_SOC_H__

#include <stdint.h>
#include <stdbool.h>
#include "nrf_svc.h"
#include "nrf51.h"
#include "nrf51_bitfields.h"
#include "nrf_error_soc.h"

/** @addtogroup NRF_SOC_DEFINES Defines
 * @{ */

/**@brief The number of the lowest SVC number reserved for the SoC library. */
#define SOC_SVC_BASE 0x20

/**@brief Guranteed time for application to process radio inactive notification. */
#define NRF_RADIO_NOTIFICATION_INACTIVE_GUARANTEED_TIME_US   (62)

#define SOC_ECB_KEY_LENGTH                   (16)                       /**< ECB key length. */
#define SOC_ECB_CLEARTEXT_LENGTH             (16)                       /**< ECB cleartext length. */
#define SOC_ECB_CIPHERTEXT_LENGTH            (SOC_ECB_CLEARTEXT_LENGTH) /**< ECB ciphertext length. */

#define SD_EVENT_IRQn                 (SWI2_IRQn)       /**< SoftDevice Event IRQ number. Used for both protocol events and SoC events. */
#define SD_EVENT_IRQHandler           (SWI2_IRQHandler) /**< SoftDevice Event IRQ handler. Used for both protocol events and SoC events. */
#define RADIO_NOTIFICATION_IRQn       (SWI1_IRQn)       /**< The radio notification IRQ number. */
#define RADIO_NOTIFICATION_IRQHandler (SWI1_IRQHandler) /**< The radio notification IRQ handler. */

/** @} */

/** @addtogroup NRF_SOC_TYPES Types
 * @{ */

/**@brief The SVC numbers used by the SVC functions in the SoC library. */
enum NRF_SOC_SVCS
{
  SD_MUTEX_NEW = SOC_SVC_BASE,
  SD_MUTEX_ACQUIRE,
  SD_MUTEX_RELEASE,
  SD_NVIC_ENABLEIRQ,
  SD_NVIC_DISABLEIRQ,
  SD_NVIC_GETPENDINGIRQ,
  SD_NVIC_SETPENDINGIRQ,
  SD_NVIC_CLEARPENDINGIRQ,
  SD_NVIC_SETPRIORITY,
  SD_NVIC_GETPRIORITY,
  SD_NVIC_SYSTEMRESET,
  SD_NVIC_CRITICAL_REGION_ENTER,
  SD_NVIC_CRITICAL_REGION_EXIT,
  SD_RAND_APPLICATION_POOL_CAPACITY,
  SD_RAND_APPLICATION_BYTES_AVAILABLE,
  SD_RAND_APPLICATION_GET_VECTOR,
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
  SD_APP_EVENT_WAIT,
  SD_CLOCK_HFCLK_REQUEST,
  SD_CLOCK_HFCLK_RELEASE,
  SD_CLOCK_HFCLK_IS_RUNNING,
  SD_PPI_CHANNEL_ENABLE_GET,
  SD_PPI_CHANNEL_ENABLE_SET,
  SD_PPI_CHANNEL_ENABLE_CLR,
  SD_PPI_CHANNEL_ASSIGN,
  SD_PPI_GROUP_TASK_ENABLE,
  SD_PPI_GROUP_TASK_DISABLE,
  SD_PPI_GROUP_ASSIGN,
  SD_PPI_GROUP_GET,
  SD_RADIO_NOTIFICATION_CFG_SET,
  SD_ECB_BLOCK_ENCRYPT,
  SD_EVENT_GET,

  SVC_SOC_LAST
};

/**@brief Possible values of a ::nrf_mutex_t. */
enum NRF_MUTEX_VALUES
{
  NRF_MUTEX_FREE,
  NRF_MUTEX_TAKEN
};

/**@brief Possible values of ::nrf_app_irq_priority_t. */
enum NRF_APP_PRIORITIES
{
  NRF_APP_PRIORITY_HIGH = 1,
  NRF_APP_PRIORITY_LOW = 3
};

/**@brief Possible values of ::nrf_power_mode_t. */
enum NRF_POWER_MODES
{
  NRF_POWER_MODE_CONSTLAT,  /**< Constant latency mode. See power management in the reference manual. */
  NRF_POWER_MODE_LOWPWR     /**< Low power mode. See power management in the reference manual. */
};


/**@brief Possible values of ::nrf_power_failure_threshold_t */
enum NRF_POWER_THRESHOLDS
{
  NRF_POWER_THRESHOLD_V21,  /**< 2.1 Volts power failure threshold. */
  NRF_POWER_THRESHOLD_V23,  /**< 2.3 Volts power failure threshold. */
  NRF_POWER_THRESHOLD_V25,  /**< 2.5 Volts power failure threshold. */ 
  NRF_POWER_THRESHOLD_V27   /**< 2.7 Volts power failure threshold. */
};


/**@brief Possible values of ::nrf_power_dcdc_mode_t. */
enum NRF_POWER_DCDC_MODES
{
  NRF_POWER_DCDC_MODE_OFF,          /**< The DCDC is always off. */
  NRF_POWER_DCDC_MODE_ON,           /**< The DCDC is always on. */
  NRF_POWER_DCDC_MODE_AUTOMATIC     /**< The DCDC is automatically managed. */
};

/**@brief Possible values of ::nrf_radio_notification_distance_t. */
enum NRF_RADIO_NOTIFICATION_DISTANCES
{
  NRF_RADIO_NOTIFICATION_DISTANCE_NONE = 0, /**< The event does not have a notification. */
  NRF_RADIO_NOTIFICATION_DISTANCE_800US,    /**< The distance from the active notification to start of radio activity. */
  NRF_RADIO_NOTIFICATION_DISTANCE_1740US,   /**< The distance from the active notification to start of radio activity. */
  NRF_RADIO_NOTIFICATION_DISTANCE_2680US,   /**< The distance from the active notification to start of radio activity. */
  NRF_RADIO_NOTIFICATION_DISTANCE_3620US,   /**< The distance from the active notification to start of radio activity. */
  NRF_RADIO_NOTIFICATION_DISTANCE_4560US,   /**< The distance from the active notification to start of radio activity. */
  NRF_RADIO_NOTIFICATION_DISTANCE_5500US    /**< The distance from the active notification to start of radio activity. */
};


/**@brief Possible values of ::nrf_radio_notification_type_t. */
enum NRF_RADIO_NOTIFICATION_TYPES
{
  NRF_RADIO_NOTIFICATION_TYPE_NONE = 0,        /**< The event does not have a radio notification signal. */
  NRF_RADIO_NOTIFICATION_TYPE_INT_ON_ACTIVE,   /**< Using interrupt for notification when the radio will be enabled. */
  NRF_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE, /**< Using interrupt for notification when the radio has been disabled. */
  NRF_RADIO_NOTIFICATION_TYPE_INT_ON_BOTH,     /**< Using interrupt for notification both when the radio will be enabled and disabled. */
};

/**@brief SoC Events. */
enum NRF_SOC_EVENTS
{
  NRF_EVENT_HFCLKSTARTED,                       /**< Event indicating that the HFCLK has started. */
  NRF_EVENT_POWER_FAILURE_WARNING,              /**< Event indicating that a power failure warning has occurred. */
  NRF_EVENT_NUMBER_OF_EVENTS
};

/** @} */

/** @addtogroup NRF_SOC_TYPES Types
 * @{ */

/**@brief Represents a mutex for use with the nrf_mutex functions.
 * @note Accessing the value directly is not safe, use the mutex functions!
 */
typedef volatile uint8_t nrf_mutex_t;

/**@brief The interrupt priorities available to the application while the softdevice is active. */
typedef uint8_t nrf_app_irq_priority_t;

/**@brief Represents a power mode, used in power mode functions */
typedef uint8_t nrf_power_mode_t;

/**@brief Represents a power failure threshold value. */
typedef uint8_t nrf_power_failure_threshold_t;

/**@brief Represents a DCDC mode value. */
typedef uint32_t nrf_power_dcdc_mode_t;

/**@brief Radio notification distances. */
typedef uint8_t nrf_radio_notification_distance_t;

/**@brief Radio notification types. */
typedef uint8_t nrf_radio_notification_type_t;


/**@brief AES ECB data structure */
typedef struct
{
  uint8_t key[SOC_ECB_KEY_LENGTH];                /**< Encryption key. */
  uint8_t cleartext[SOC_ECB_CLEARTEXT_LENGTH];    /**< Clear Text data. */
  uint8_t ciphertext[SOC_ECB_CIPHERTEXT_LENGTH];  /**< Cipher Text data. */
} nrf_ecb_hal_data_t;

/** @} */

/** @addtogroup NRF_SOC_FUNCTIONS Functions
 * @{ */

/**@brief Initialize a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to initialize.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_MUTEX_NEW, uint32_t, sd_mutex_new(nrf_mutex_t * p_mutex));

/**@brief Attempt to acquire a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to acquire.
 *
 * @retval ::NRF_SUCCESS The mutex was successfully acquired.
 * @retval ::NRF_ERROR_SOC_MUTEX_ALREADY_TAKEN The mutex could not be acquired.
 */
SVCALL(SD_MUTEX_ACQUIRE, uint32_t, sd_mutex_acquire(nrf_mutex_t * p_mutex));

/**@brief Release a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to release.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_MUTEX_RELEASE, uint32_t, sd_mutex_release(nrf_mutex_t * p_mutex));

/**@brief Enable External Interrupt.
 * @note Corresponds to NVIC_EnableIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_EnableIRQ documentation in CMSIS.
 *
 * @retval ::NRF_SUCCESS The interrupt was enabled.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE The interrupt is not available for the application.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_PRIORITY_NOT_ALLOWED The interrupt has a priority not available for the application.
 */
SVCALL(SD_NVIC_ENABLEIRQ, uint32_t, sd_nvic_EnableIRQ(IRQn_Type IRQn));

/**@brief  Disable External Interrupt.
 * @note Corresponds to NVIC_DisableIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_DisableIRQ documentation in CMSIS
 *
 * @retval ::NRF_SUCCESS The interrupt was disabled.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE The interrupt is not available for the application.
 */
SVCALL(SD_NVIC_DISABLEIRQ, uint32_t, sd_nvic_DisableIRQ(IRQn_Type IRQn));

/**@brief  Get Pending Interrupt.
 * @note Corresponds to NVIC_GetPendingIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in]   IRQn          See the NVIC_GetPendingIRQ documentation in CMSIS.
 * @param[out]  p_pending_irq Return value from NVIC_GetPendingIRQ.
 *
 * @retval ::NRF_SUCCESS The interrupt is available for the application.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE IRQn is not available for the application.
 */
SVCALL(SD_NVIC_GETPENDINGIRQ, uint32_t, sd_nvic_GetPendingIRQ(IRQn_Type IRQn, uint32_t * p_pending_irq));

/**@brief  Set Pending Interrupt.
 * @note Corresponds to NVIC_SetPendingIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_SetPendingIRQ documentation in CMSIS.
 *
 * @retval ::NRF_SUCCESS The interrupt is set pending.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE IRQn is not available for the application.
 */
SVCALL(SD_NVIC_SETPENDINGIRQ, uint32_t, sd_nvic_SetPendingIRQ(IRQn_Type IRQn));

/**@brief  Clear Pending Interrupt.
 * @note Corresponds to NVIC_ClearPendingIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_ClearPendingIRQ documentation in CMSIS.
 *
 * @retval ::NRF_SUCCESS The interrupt pending flag is cleared.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE IRQn is not available for the application.
 */
SVCALL(SD_NVIC_CLEARPENDINGIRQ, uint32_t, sd_nvic_ClearPendingIRQ(IRQn_Type IRQn));

/**@brief Set Interrupt Priority.
 * @note Corresponds to NVIC_SetPriority in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 * @pre{priority is valid and not reserved by the stack}
 *
 * @param[in] IRQn      See the NVIC_SetPriority documentation in CMSIS.
 * @param[in] priority  A valid IRQ priority for use by the application.
 *
 * @retval ::NRF_SUCCESS The interrupt and priority level is available for the application.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE IRQn is not available for the application.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_PRIORITY_NOT_ALLOWED The interrupt priority is not available for the application.
 */
SVCALL(SD_NVIC_SETPRIORITY, uint32_t, sd_nvic_SetPriority(IRQn_Type IRQn, nrf_app_irq_priority_t priority));

/**@brief Get Interrupt Priority.
 * @note Corresponds to NVIC_GetPriority in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in]  IRQn         See the NVIC_GetPriority documentation in CMSIS.
 * @param[out] p_priority   Return value from NVIC_GetPriority.
 *
 * @retval ::NRF_SUCCESS The interrupt priority is returned in p_priority.
 * @retval ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - IRQn is not available for the application.
 */
SVCALL(SD_NVIC_GETPRIORITY, uint32_t, sd_nvic_GetPriority(IRQn_Type IRQn, nrf_app_irq_priority_t * p_priority));

/**@brief System Reset.
 * @note Corresponds to NVIC_SystemReset in CMSIS.
 *
 * @retval ::NRF_ERROR_SOC_NVIC_SHOULD_NOT_RETURN
 */
SVCALL(SD_NVIC_SYSTEMRESET, uint32_t, sd_nvic_SystemReset(void));

/**@brief Enters critical region.
 *
 * @post Application interrupts will be disabled.
 * @sa sd_nvic_critical_region_exit
 *
 * @param[out]  p_is_nested_critical_region  1: If in a nested critical region.
 *                                           0: Otherwise.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_NVIC_CRITICAL_REGION_ENTER, uint32_t, sd_nvic_critical_region_enter(uint8_t * p_is_nested_critical_region));

/**@brief Exit critical region.
 *
 * @pre Application has entered a critical region using ::sd_nvic_critical_region_enter.
 * @post If not in a nested critical region, the application interrupts will restored to the state before ::sd_nvic_critical_region_enter was called. 
 *
 * @param[in] is_nested_critical_region If this is set to 1, the critical region won't be exited. @sa sd_nvic_critical_region_enter.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_NVIC_CRITICAL_REGION_EXIT, uint32_t, sd_nvic_critical_region_exit(uint8_t is_nested_critical_region));

/**@brief Query the capacity of the application random pool.
 *
 * @param[out] p_pool_capacity The capacity of the pool.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_RAND_APPLICATION_POOL_CAPACITY, uint32_t, sd_rand_application_pool_capacity_get(uint8_t * p_pool_capacity));

/**@brief Get number of random bytes available to the application.
 *
 * @param[out] p_bytes_available The number of bytes currently available in the pool.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_RAND_APPLICATION_BYTES_AVAILABLE, uint32_t, sd_rand_application_bytes_available_get(uint8_t * p_bytes_available));

/**@brief Get random bytes from the application pool.

  @param[out]  p_buff  Pointer to unit8_t buffer for storing the bytes.
  @param[in]   length  Number of bytes to take from pool and place in p_buff.

  @retval ::NRF_SUCCESS The requested bytes were written to p_buff.
  @retval ::NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES No bytes were written to the buffer, because there were not enough bytes available.
*/
SVCALL(SD_RAND_APPLICATION_GET_VECTOR, uint32_t, sd_rand_application_vector_get(uint8_t * p_buff, uint8_t length));

/**@brief Gets the reset reason register. 
 *
 * @param[out]  p_reset_reason  Contents of the NRF_POWER->RESETREAS register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RESET_REASON_GET, uint32_t, sd_power_reset_reason_get(uint32_t * p_reset_reason));

/**@brief Clears the bits of the reset reason register. 
 *
 * @param[in] reset_reason_clr_msk Contains the bits to clear from the reset reason register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RESET_REASON_CLR, uint32_t, sd_power_reset_reason_clr(uint32_t reset_reason_clr_msk));

/**@brief Sets the power mode when in CPU sleep.
 *
 * @param[in] power_mode The power mode to use when in CPU sleep. @sa sd_app_event_wait
 *
 * @retval ::NRF_SUCCESS The power mode was set.
 * @retval ::NRF_ERROR_SOC_POWER_MODE_UNKNOWN The power mode was unknown.
 */
SVCALL(SD_POWER_MODE_SET, uint32_t, sd_power_mode_set(nrf_power_mode_t power_mode));

/**@brief Puts the chip in System OFF mode. 
 *
 * @retval ::NRF_ERROR_SOC_POWER_OFF_SHOULD_NOT_RETURN
 */
SVCALL(SD_POWER_SYSTEM_OFF, uint32_t, sd_power_system_off(void));

/**@brief Enables or disables the power-fail comparator.
 *
 * Enabling this will give a softdevice event (NRF_EVENT_POWER_FAILURE_WARNING) when the power failure warning occurs.
 * The event can be retrived with sd_event_get();
 *
 * @param[in] pof_enable    True if the power-fail comparator should be enabled, false if it should be disabled.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_POF_ENABLE, uint32_t, sd_power_pof_enable(uint8_t pof_enable));

/**@brief Sets the power-fail threshold value.
 *
 * @param[in] threshold The power-fail threshold value to use.
 *
 * @retval ::NRF_SUCCESS The power failure threshold was set.
 * @retval ::NRF_ERROR_SOC_POWER_POF_THRESHOLD_UNKNOWN The power failure threshold is unknown.
 */
SVCALL(SD_POWER_POF_THRESHOLD_SET, uint32_t, sd_power_pof_threshold_set(nrf_power_failure_threshold_t threshold));

/**@brief Sets bits in the NRF_POWER->RAMON register.
 *
 * @param[in] ramon Contains the bits needed to be set in the NRF_POWER->RAMON register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_POWER_RAMON_SET, uint32_t, sd_power_ramon_set(uint32_t ramon));

/** @brief Clears bits in the NRF_POWER->RAMON register.
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
SVCALL(SD_POWER_RAMON_GET, uint32_t, sd_power_ramon_get(uint32_t * p_ramon));

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
 * Depending on the internal state of the SoftDevice, the mode change may not happen immediately.
 * The DCDC mode switch will be blocked when occuring in close proximity to radio transmissions. When
 * the radio transmission is done, the last mode will be used.
 *
 * @param[in] dcdc_mode The mode of the DCDC.
 *
 * @retval ::NRF_SUCCESS
 * @retval ::NRF_ERROR_INVALID_PARAM The DCDC mode is invalid.
 */
SVCALL(SD_POWER_DCDC_MODE_SET, uint32_t, sd_power_dcdc_mode_set(nrf_power_dcdc_mode_t dcdc_mode));

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
SVCALL(SD_CLOCK_HFCLK_IS_RUNNING, uint32_t, sd_clock_hfclk_is_running(uint32_t * p_is_running));

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
 * @note If an application interrupt has happened since the last time sd_app_event_wait was
 *       called this function will return immediately and not go to sleep. This is to avoid race
 *       conditions that can occur when a flag is updated in the interrupt handler and processed
 *       in the main loop.
 *
 * @post An application interrupt has happened or a interrupt pending flag is set.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_APP_EVENT_WAIT, uint32_t, sd_app_event_wait(void));

/**@brief Get PPI channel enable register contents.
 *
 * @param[out] p_channel_enable The contents of the PPI CHEN register.
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_CHANNEL_ENABLE_GET, uint32_t, sd_ppi_channel_enable_get(uint32_t * p_channel_enable));

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
 * @param[in] event_endpoint Event endpoint of the PPI channel.
 * @param[in] task_endpoint Task endpoint of the PPI channel.
 *
 * @retval ::NRF_ERROR_SOC_PPI_INVALID_CHANNEL The channel number is invalid.
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_PPI_CHANNEL_ASSIGN, uint32_t, sd_ppi_channel_assign(uint8_t channel_num, const volatile void * event_endpoint, const volatile void * task_endpoint));

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
SVCALL(SD_PPI_GROUP_GET, uint32_t, sd_ppi_group_get(uint8_t group_num, uint32_t * p_channel_msk));

/**@brief Configures the Radio Notification signal.
 *
 * @note
 *      - The notification signal latency depends on the interrupt priority settings of SWI used
 *        for notification signal.
 *      - In the period between the ACTIVE signal and the start of the Radio Event, the SoftDevice
 *        will interrupt the application to do Radio Event preparation.
 *      - Using the Radio Notification feature may limit the bandwidth, as the SoftDevice may have
 *        to shorten the connection events to have time for the Radio Notification signals.
 *
 * @param[in]  type      Type of notification signal.
 *                       @ref NRF_RADIO_NOTIFICATION_TYPE_NONE shall be used to turn off radio
 *                       notification. Using @ref NRF_RADIO_NOTIFICATION_DISTANCE_NONE is
 *                       recommended (but not required) to be used with
 *                       @ref NRF_RADIO_NOTIFICATION_TYPE_NONE.
 *
 * @param[in]  distance  Distance between the notification signal and start of radio activity.
 *                       This parameter is ignored when @ref NRF_RADIO_NOTIFICATION_TYPE_NONE or 
 *                       @ref NRF_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE is used.
 *
 * @retval ::NRF_ERROR_INVALID_PARAM The group number is invalid.
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_RADIO_NOTIFICATION_CFG_SET, uint32_t, sd_radio_notification_cfg_set(nrf_radio_notification_type_t type, nrf_radio_notification_distance_t distance));

/**@brief Encrypts a block according to the specified parameters.
 *
 * 128-bit AES encryption.
 *
 * @param[in, out] p_ecb_data Pointer to the ECB paramters' struct (two input
 *                            parameters and one output parameter).
 *
 * @retval ::NRF_SUCCESS
 */
SVCALL(SD_ECB_BLOCK_ENCRYPT, uint32_t, sd_ecb_block_encrypt(nrf_ecb_hal_data_t * p_ecb_data));

/**@brief Gets any pending events generated by the SoC API.
 *
 * The application should keep calling this function to get events, until ::NRF_ERROR_NOT_FOUND is returned.
 *
 * @param[out] p_event Set to the event number, if any events are pending.
 *
 * @retval ::NRF_SUCCESS An event was pending. The event number is written in the p_event paramer.
 * @retval ::NRF_ERROR_NOT_FOUND No pending events. 
 */
SVCALL(SD_EVENT_GET, uint32_t, sd_event_get(uint32_t *p_event));

/** @} */

#endif // NRF_SOC_H__

/**
  @}
 */
