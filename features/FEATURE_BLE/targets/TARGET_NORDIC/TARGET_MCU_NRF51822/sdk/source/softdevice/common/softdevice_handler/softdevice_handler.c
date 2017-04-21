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

#include "softdevice_handler.h"
#include <stdlib.h>
#include "nordic_common.h"
#include "app_error.h"
#include "app_util.h"
#include "nrf_assert.h"
#include "nrf_soc.h"
#include "nrf.h"

#if defined(ANT_STACK_SUPPORT_REQD) && defined(BLE_STACK_SUPPORT_REQD)
    #include "ant_interface.h"
#elif defined(ANT_STACK_SUPPORT_REQD) 
    #include "ant_interface.h"
#elif defined(BLE_STACK_SUPPORT_REQD)
    #include "nrf_ble.h"
#endif

#ifdef NRF51
#define SOFTDEVICE_EVT_IRQ        SD_EVT_IRQn       /**< SoftDevice Event IRQ number. Used for both protocol events and SoC events. */
#define SOFTDEVICE_EVT_IRQHandler SD_EVT_IRQHandler
#elif defined (NRF52)
#define SOFTDEVICE_EVT_IRQ        SWI2_EGU2_IRQn
#define SOFTDEVICE_EVT_IRQHandler SWI2_EGU2_IRQHandler
#endif /* NRF51 */

static softdevice_evt_schedule_func_t m_evt_schedule_func;              /**< Pointer to function for propagating SoftDevice events to the scheduler. */

static volatile bool                  m_softdevice_enabled = false;     /**< Variable to indicate whether the SoftDevice is enabled. */

#ifdef BLE_STACK_SUPPORT_REQD
// The following three definitions is needed only if BLE events are needed to be pulled from the stack.
static uint8_t                      * mp_ble_evt_buffer;                /**< Buffer for receiving BLE events from the SoftDevice. */
static uint16_t                       m_ble_evt_buffer_size;            /**< Size of BLE event buffer. */
static ble_evt_handler_t              m_ble_evt_handler;                /**< Application event handler for handling BLE events. */
#endif

#ifdef ANT_STACK_SUPPORT_REQD
// The following two definition is needed only if ANT events are needed to be pulled from the stack.
static ant_evt_t                      m_ant_evt_buffer;                 /**< Buffer for receiving ANT events from the SoftDevice. */
static ant_evt_handler_t              m_ant_evt_handler;                /**< Application event handler for handling ANT events.  */
#endif

static sys_evt_handler_t              m_sys_evt_handler;                /**< Application event handler for handling System (SOC) events.  */


/**@brief       Callback function for asserts in the SoftDevice.
 *
 * @details     A pointer to this function will be passed to the SoftDevice. This function will be
 *              called if an ASSERT statement in the SoftDevice fails.
 *
 * @param[in]   pc         The value of the program counter when the ASSERT call failed.
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void softdevice_assertion_handler(uint32_t pc, uint16_t line_num, const uint8_t * file_name)
{
    UNUSED_PARAMETER(pc);
    assert_nrf_callback(line_num, file_name);
}


void intern_softdevice_events_execute(void)
{
    if (!m_softdevice_enabled)
    {
        // SoftDevice not enabled. This can be possible if the SoftDevice was enabled by the
        // application without using this module's API (i.e softdevice_handler_init)

        return;
    }

    bool no_more_soc_evts = (m_sys_evt_handler == NULL);
#ifdef BLE_STACK_SUPPORT_REQD
    bool no_more_ble_evts = (m_ble_evt_handler == NULL);
#endif
#ifdef ANT_STACK_SUPPORT_REQD
    bool no_more_ant_evts = (m_ant_evt_handler == NULL);
#endif

    for (;;)
    {
        uint32_t err_code;

        if (!no_more_soc_evts)
        {
            uint32_t evt_id;

            // Pull event from SOC.
            err_code = sd_evt_get(&evt_id);
            
            if (err_code == NRF_ERROR_NOT_FOUND)
            {
                no_more_soc_evts = true;
            }
            else if (err_code != NRF_SUCCESS)
            {
                APP_ERROR_HANDLER(err_code);
            }
            else
            {
                // Call application's SOC event handler.
                m_sys_evt_handler(evt_id);
            }
        }

#ifdef BLE_STACK_SUPPORT_REQD
        // Fetch BLE Events.
        if (!no_more_ble_evts)
        {
            // Pull event from stack
            uint16_t evt_len = m_ble_evt_buffer_size;

            err_code = sd_ble_evt_get(mp_ble_evt_buffer, &evt_len);
            if (err_code == NRF_ERROR_NOT_FOUND)
            {
                no_more_ble_evts = true;
            }
            else if (err_code != NRF_SUCCESS)
            {
                APP_ERROR_HANDLER(err_code);
            }
            else
            {
                // Call application's BLE stack event handler.
                m_ble_evt_handler((ble_evt_t *)mp_ble_evt_buffer);
            }
        }
#endif

#ifdef ANT_STACK_SUPPORT_REQD
        // Fetch ANT Events.
        if (!no_more_ant_evts)
        {
            // Pull event from stack
            err_code = sd_ant_event_get(&m_ant_evt_buffer.channel,
                                        &m_ant_evt_buffer.event,
                                        m_ant_evt_buffer.evt_buffer);
            if (err_code == NRF_ERROR_NOT_FOUND)
            {
                no_more_ant_evts = true;
            }
            else if (err_code != NRF_SUCCESS)
            {
                APP_ERROR_HANDLER(err_code);
            }
            else
            {
                // Call application's ANT stack event handler.
                m_ant_evt_handler(&m_ant_evt_buffer);
            }
        }
#endif

        if (no_more_soc_evts)
        {
            // There are no remaining System (SOC) events to be fetched from the SoftDevice.
#if defined(ANT_STACK_SUPPORT_REQD) && defined(BLE_STACK_SUPPORT_REQD)
            // Check if there are any remaining BLE and ANT events.
            if (no_more_ble_evts && no_more_ant_evts)
            {
                break;
            }
#elif defined(BLE_STACK_SUPPORT_REQD)
            // Check if there are any remaining BLE events.
            if (no_more_ble_evts)
            {
                break;
            }
#elif defined(ANT_STACK_SUPPORT_REQD)
            // Check if there are any remaining ANT events.
            if (no_more_ant_evts)
            {
                break;
            }
#else
            // No need to check for BLE or ANT events since there is no support for BLE and ANT
            // required.
            break;
#endif
        }
    }
}

bool softdevice_handler_isEnabled(void)
{
    return m_softdevice_enabled;
}

uint32_t softdevice_handler_init(nrf_clock_lfclksrc_t           clock_source,
                                 void *                         p_ble_evt_buffer,
                                 uint16_t                       ble_evt_buffer_size,
                                 softdevice_evt_schedule_func_t evt_schedule_func)
{
    uint32_t err_code;

    // Save configuration.
#if defined (BLE_STACK_SUPPORT_REQD)
    // Check that buffer is not NULL.
    if (p_ble_evt_buffer == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    // Check that buffer is correctly aligned.
    if (!is_word_aligned(p_ble_evt_buffer))
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    mp_ble_evt_buffer     = (uint8_t *)p_ble_evt_buffer;
    m_ble_evt_buffer_size = ble_evt_buffer_size;
#else
    // The variables p_ble_evt_buffer and ble_evt_buffer_size is not needed if BLE Stack support
    // is not required.
    UNUSED_PARAMETER(p_ble_evt_buffer);
    UNUSED_PARAMETER(ble_evt_buffer_size);
#endif

    m_evt_schedule_func = evt_schedule_func;

//Enabling FPU for SoftDevice
#ifdef S132
    SCB->CPACR |= (3UL << 20) | (3UL << 22);
    __DSB();
    __ISB();
#endif
    // Initialize SoftDevice.
    err_code = sd_softdevice_enable(clock_source, softdevice_assertion_handler);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
#ifdef S132
    SCB->CPACR = 0;
    __DSB();
    __ISB();
#endif

    m_softdevice_enabled = true;

    // Enable BLE event interrupt (interrupt priority has already been set by the stack).
    return sd_nvic_EnableIRQ(SOFTDEVICE_EVT_IRQ);
}


uint32_t softdevice_handler_sd_disable(void)
{
    uint32_t err_code = sd_softdevice_disable();
 
    m_softdevice_enabled = !(err_code == NRF_SUCCESS);

    return err_code;
}


#ifdef BLE_STACK_SUPPORT_REQD
uint32_t softdevice_ble_evt_handler_set(ble_evt_handler_t ble_evt_handler)
{
    if (ble_evt_handler == NULL)
    {
        return NRF_ERROR_NULL;
    }

    m_ble_evt_handler = ble_evt_handler;

    return NRF_SUCCESS;
}
#endif


#ifdef ANT_STACK_SUPPORT_REQD
uint32_t softdevice_ant_evt_handler_set(ant_evt_handler_t ant_evt_handler)
{
    if (ant_evt_handler == NULL)
    {
        return NRF_ERROR_NULL;
    }

    m_ant_evt_handler = ant_evt_handler;

    return NRF_SUCCESS;
}
#endif


uint32_t softdevice_sys_evt_handler_set(sys_evt_handler_t sys_evt_handler)
{
    if (sys_evt_handler == NULL)
    {
        return NRF_ERROR_NULL;
    }

    m_sys_evt_handler = sys_evt_handler;

    return NRF_SUCCESS;
}


/**@brief   Function for handling the Application's BLE Stack events interrupt.
 *
 * @details This function is called whenever an event is ready to be pulled.
 */
void SOFTDEVICE_EVT_IRQHandler(void)
{
    if (m_evt_schedule_func != NULL)
    {
        uint32_t err_code = m_evt_schedule_func();
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        intern_softdevice_events_execute();
    }
}
