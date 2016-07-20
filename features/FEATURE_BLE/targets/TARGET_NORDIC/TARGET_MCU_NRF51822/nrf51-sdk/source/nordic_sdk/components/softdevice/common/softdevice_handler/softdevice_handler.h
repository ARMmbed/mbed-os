/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
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

#define SOFTDEVICE_SCHED_EVT_SIZE       0                                                 /**< Size of button events being passed through the scheduler (is to be used for computing the maximum size of scheduler events). For SoftDevice events, this size is 0, since the events are being pulled in the event handler. */
#define SYS_EVT_MSG_BUF_SIZE            sizeof(uint32_t)                                  /**< Size of System (SOC) event message buffer. */

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
 * @param[in] CLOCK_SOURCE     Low frequency clock source and accuracy (type nrf_clock_lfclksrc_t,
 *                             see sd_softdevice_enable() for details).
 * @param[in] EVT_HANDLER      scheduler/RTOS event handler function.
 *
 * @note      Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *            several times as long as it is from the same location, that is to do a
 *            reinitialization).
 */
/*lint -emacro(506, SOFTDEVICE_HANDLER_INIT) */ /* Suppress "Constant value Boolean */
#define SOFTDEVICE_HANDLER_INIT(CLOCK_SOURCE,                                                      \
                                EVT_HANDLER)                                                     \
    do                                                                                             \
    {                                                                                              \
        static uint32_t BLE_EVT_BUFFER[CEIL_DIV(BLE_STACK_EVT_MSG_BUF_SIZE, sizeof(uint32_t))];    \
        uint32_t ERR_CODE;                                                                         \
        ERR_CODE = softdevice_handler_init((CLOCK_SOURCE),                                         \
                                           BLE_EVT_BUFFER,                                         \
                                           sizeof(BLE_EVT_BUFFER),                                 \
                                           EVT_HANDLER);      \
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
 * @param[in]  clock_source        Low frequency clock source to be used by the SoftDevice.
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
uint32_t softdevice_handler_init(nrf_clock_lfclksrc_t              clock_source,
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


// Functions for connecting the Stack Event Handler to the scheduler:
/**@cond NO_DOXYGEN */
void intern_softdevice_events_execute(void);


/**@endcond */

#endif // SOFTDEVICE_HANDLER_H__

/** @} */
