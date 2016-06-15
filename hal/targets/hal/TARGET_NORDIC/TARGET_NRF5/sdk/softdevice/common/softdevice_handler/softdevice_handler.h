/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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

/** @file
 *
 * @defgroup softdevice_handler SoftDevice Event Handler
 * @{
 * @ingroup  app_common
 * @brief    API for initializing and disabling the SoftDevice
 *
 * @details  This API contains the functions and defines exposed by the @ref lib_softdevice_handler.
 *           For more information on the library and how the application should use it, please refer
 *           @ref lib_softdevice_handler.
 *
 * @note     Use the USE_SCHEDULER parameter of the SOFTDEVICE_HANDLER_INIT() macro to select if
 *           the @ref app_scheduler is to be used or not.
 *
 * @note     Even if the scheduler is not used, softdevice_handler.h will include app_scheduler.h.
 *           So when compiling, app_scheduler.h must be available in one of the compiler include
 *           paths.
 */

#ifndef SOFTDEVICE_HANDLER_H__
#define SOFTDEVICE_HANDLER_H__

#include <stdlib.h>
#include "nordic_common.h"
#include "nrf_sdm.h"
#include "app_error.h"
#include "app_util.h"
#include "ble_stack_handler_types.h"
#include "ant_stack_handler_types.h"
#if defined(BLE_STACK_SUPPORT_REQD)
    #include "ble.h"
#endif
#include "app_ram_base.h"
#define SOFTDEVICE_SCHED_EVT_SIZE       0                                                 /**< Size of button events being passed through the scheduler (is to be used for computing the maximum size of scheduler events). For SoftDevice events, this size is 0, since the events are being pulled in the event handler. */
#define SYS_EVT_MSG_BUF_SIZE            sizeof(uint32_t)                                  /**< Size of System (SOC) event message buffer. */


#define CHECK_RAM_START_ADDR_INTERN(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT)              \
    do{                                                                                     \
        uint32_t app_ram_start_addr = APP_RAM_BASE_CENTRAL_LINKS_##CENTRAL_LINK_COUNT##_PERIPH_LINKS_##PERIPHERAL_LINK_COUNT##_SEC_COUNT_0_MID_BW; \
        err_code = sd_check_ram_start(app_ram_start_addr);                                  \
        APP_ERROR_CHECK(err_code);                                                          \
    } while (0)

/** @brief Macro for checking the RAM requirement of the SoftDevice */
#define CHECK_RAM_START_ADDR(C_LINK_CNT, P_LINK_CNT) CHECK_RAM_START_ADDR_INTERN(C_LINK_CNT, P_LINK_CNT)


/**@brief     Function for checking the RAM requirement of the SoftDevice.
 *
 * @details   Call this function to check if the project settings have the correct RAM start address in respect to what the SoftDevice requires.
 *
 * @note      This function is called using the CHECK_RAM_START_ADDR_INTERN macro and should not be called directly.
 */
uint32_t sd_check_ram_start(uint32_t sd_req_ram_start);

/**@brief Type of function for passing events from the stack handler module to the scheduler. */
typedef uint32_t (*softdevice_evt_schedule_func_t) (void);

/**@brief Application System (SOC) event handler type. */
typedef void (*sys_evt_handler_t) (uint32_t evt_id);


/**@brief     Macro for initializing the stack event handler.
 *
 * @details   It will handle dimensioning and allocation of the memory buffer required for reading
 *            events from the stack, making sure the buffer is correctly aligned. It will also
 *            connect the stack event handler to the scheduler/RTOS (if specified).
 *
 * @param[in] CLOCK_SOURCE     Low frequency clock source and accuracy (type nrf_clock_lf_cfg_t_t,
 *                             see sd_softdevice_enable() for details).
 * @param[in] EVT_HANDLER      scheduler/RTOS event handler function.
 *
 * @note      Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *            several times as long as it is from the same location, that is to do a
 *            reinitialization).
 */
/*lint -emacro(506, SOFTDEVICE_HANDLER_INIT) */ /* Suppress "Constant value Boolean */
#define SOFTDEVICE_HANDLER_INIT(CLOCK_SOURCE,                                                      \
                                EVT_HANDLER)                                                       \
    do                                                                                             \
    {                                                                                              \
        static uint32_t BLE_EVT_BUFFER[CEIL_DIV(BLE_STACK_EVT_MSG_BUF_SIZE, sizeof(uint32_t))];    \
        uint32_t ERR_CODE;                                                                         \
        ERR_CODE = softdevice_handler_init((CLOCK_SOURCE),                                         \
                                           BLE_EVT_BUFFER,                                         \
                                           sizeof(BLE_EVT_BUFFER),                                 \
                                           EVT_HANDLER);                                           \
        APP_ERROR_CHECK(ERR_CODE);                                                                 \
    } while (0)

/**
 * @brief Function for retrieving the information about SD state
 *
 * The information about current state of softdevice.
 * @retval false SD is not initialized and SD commands should not be called.
 * @retval true  SD is already initialized
 */
bool softdevice_handler_isEnabled(void);

/**@brief      Function for initializing the stack handler module.
 *
 * @details    Enables the SoftDevice and the stack event interrupt handler.
 *
 * @note       This function must be called before calling any function in the SoftDevice API.
 *
 * @note       Normally initialization should be done using the SOFTDEVICE_HANDLER_INIT() macro,
 *             as that will both allocate the event buffer, and also align the buffer correctly.
 *
 * @param[in]  p_clock_lf_cfg      Low frequency clock source to be used by the SoftDevice.
 * @param[in]  p_ble_evt_buffer    Buffer for holding one BLE stack event. Since heap is not being
 *                                 used, this buffer must be provided by the application. The
 *                                 buffer must be large enough to hold the biggest stack event the
 *                                 application is supposed to handle. The buffer must be aligned to
 *                                 a 4 byte boundary. This parameter is unused if BLE stack support 
 *                                 is not required.
 * @param[in]  ble_evt_buffer_size Size of SoftDevice BLE event buffer. This parameter is unused if
 *                                 BLE stack support is not required.
 * @param[in]  evt_schedule_func   Function for passing events to the scheduler. Point to
 *                                 ble_ant_stack_evt_schedule() to connect to the scheduler.
 *                                 Set to NULL to make the stack handler module call the event
 *                                 handler directly from the stack event interrupt handler.
 *
 * @retval     NRF_SUCCESS               Successful initialization.
 * @retval     NRF_ERROR_INVALID_PARAM   Invalid parameter (buffer not aligned to a 4 byte
 *                                       boundary) or NULL.
 */
uint32_t softdevice_handler_init(nrf_clock_lf_cfg_t *              p_clock_lf_cfg,
                                 void *                            p_ble_evt_buffer,
                                 uint16_t                          ble_evt_buffer_size,
                                 softdevice_evt_schedule_func_t    evt_schedule_func);


/**@brief     Function for disabling the SoftDevice.
 *
 * @details   This function will disable the SoftDevice. It will also update the internal state
 *            of this module.
 */
uint32_t softdevice_handler_sd_disable(void);


/**@brief     Function for registering for System (SOC) events.
 *
 * @details   The application should use this function to register for receiving System (SOC)
 *            events from the SoftDevice. If the application does not call this function, then any
 *            System (SOC) events that may be generated by the SoftDevice will NOT be fetched. Once
 *            the application has registered for the events, it is not possible to  possible to
 *            cancel the registration. However, it is possible to register a different function for
 *            handling the events at any point of time.
 *
 * @param[in] sys_evt_handler Function to be called for each received System (SOC) event.
 *
 * @retval    NRF_SUCCESS     Successful registration.
 * @retval    NRF_ERROR_NULL  Null pointer provided as input.
 */
uint32_t softdevice_sys_evt_handler_set(sys_evt_handler_t sys_evt_handler);

#if defined(BLE_STACK_SUPPORT_REQD)
/**@brief     Function for fetching the default enable parameters for the SoftDevice.
 *
 * @details   The default enable parameters will work for most projects in the SDK.
 *            They are not optimized with regards to RAM use. This function is meant as a way to abstract the
 *            details of p_ble_enable_params needed by @ref softdevice_enable. You might want to tweak
 *            the struct returned by this function or fill in the entire ble_enable_params_t
 *            instead of fetching it from this function.
 *
 * @param[in]  central_links_count Number of central links used by the application.
 * @param[in]  periph_links_count Number of peripheral links used by the application.
 * @param[out] p_ble_enable_params Default ble_enable_params_t to be used by @ref softdevice_enable.
 *
 * @retval    NRF_SUCCESS     If the operation was successful.
 */
uint32_t softdevice_enable_get_default_config(uint8_t central_links_count, 
                                              uint8_t periph_links_count,
                                              ble_enable_params_t * p_ble_enable_params);

/**@brief     Function for enabling the SoftDevice.
 *
 * @details   This function calls the @ref sd_ble_enable SVC call. It has been abstracted to give
 *            feedback on the app_ram_base. If the app_ram_base is too low, this function will
 *            return an error. Using a app_ram_base that is too high will not fail, but will
 *            result in RAM that is never used. If the DEBUG macro is enabled, this
 *            function will provide the correct app_ram_base as mandated by the SoftDevice.
 *            This is useful to tweak the RAM use of your application.
 *
 * @param[in]  p_ble_enable_params Parameters for configuring links and bandwidths.
 *
 * @retval    NRF_SUCCESS     If the operation was successful.
 */
uint32_t softdevice_enable(ble_enable_params_t * p_ble_enable_params);
#endif //BLE_STACK_SUPPORT_REQD
// Functions for connecting the Stack Event Handler to the scheduler:
/**@cond NO_DOXYGEN */
void intern_softdevice_events_execute(void);


/**@endcond */

#endif // SOFTDEVICE_HANDLER_H__

/** @} */
