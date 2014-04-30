/* Copyright (c)  2013 Nordic Semiconductor. All Rights Reserved.
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

#include "softdevice_handler.h"
#include <stdlib.h>
#include "nordic_common.h"
#include "app_error.h"
#include "app_util.h"
#include "nrf_assert.h"
#include "nrf_soc.h"
#include "mbed.h"

#if defined(ANT_STACK_SUPPORT_REQD) && defined(BLE_STACK_SUPPORT_REQD)
    #include "ant_interface.h"
#elif defined(ANT_STACK_SUPPORT_REQD) 
    #include "ant_interface.h"
#elif defined(BLE_STACK_SUPPORT_REQD)
    #include "ble.h"
#endif


static softdevice_evt_schedule_func_t m_evt_schedule_func;              /**< Pointer to function for propagating SoftDevice events to the scheduler. */

#if defined (BLE_STACK_SUPPORT_REQD) || defined (ANT_STACK_SUPPORT_REQD)
// The following two definition is needed only if ANT or BLE events are needed to be pulled from the stack.
static uint8_t *                      m_evt_buffer;                     /**< Buffer for receiving events from the SoftDevice. */
#endif

#ifdef BLE_STACK_SUPPORT_REQD
static uint16_t                       m_ble_evt_buffer_size;            /**< Size of BLE event buffer. */
#endif

static volatile bool                  m_softdevice_enabled = false;     /**< Variable to indicate whether the SoftDevice is enabled. */

#ifdef BLE_STACK_SUPPORT_REQD
static ble_evt_handler_t              m_ble_evt_handler;                /**< Application event handler for handling BLE events. */
#endif

#ifdef ANT_STACK_SUPPORT_REQD
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

            err_code = sd_ble_evt_get(m_evt_buffer, &evt_len);
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
                m_ble_evt_handler((ble_evt_t *)m_evt_buffer);
            }
        }
#endif

#ifdef ANT_STACK_SUPPORT_REQD
        // Fetch ANT Events.
        if (!no_more_ant_evts)
        {
            // Pull event from stack
            err_code = sd_ant_event_get(&((ant_evt_t *)m_evt_buffer)->channel,
                                        &((ant_evt_t *)m_evt_buffer)->event,
                                        ((ant_evt_t *)m_evt_buffer)->evt_buffer);
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
                m_ant_evt_handler((ant_evt_t *)m_evt_buffer);
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


uint32_t softdevice_handler_init(nrf_clock_lfclksrc_t           clock_source,
                                 void *                         p_evt_buffer,
                                 uint16_t                       evt_buffer_size,
                                 softdevice_evt_schedule_func_t evt_schedule_func)
{
    uint32_t err_code;

    // Save configuration.
#if defined (BLE_STACK_SUPPORT_REQD) || defined (ANT_STACK_SUPPORT_REQD)
    // Check that buffer is not NULL.
    if (p_evt_buffer == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    // Check that buffer is correctly aligned.
    if (!is_word_aligned(p_evt_buffer))
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    m_evt_buffer = (uint8_t *)p_evt_buffer;
#else
    // The variable p_evt_buffer is not needed if neither BLE Stack nor ANT stack support is 
    // required.
    UNUSED_PARAMETER(p_evt_buffer);
#endif

#if defined (BLE_STACK_SUPPORT_REQD)     
    m_ble_evt_buffer_size = evt_buffer_size;
#else
    // The variable evt_buffer_size is not needed if BLE Stack support is NOT required.
    UNUSED_PARAMETER(evt_buffer_size);
#endif
    
    m_evt_schedule_func = evt_schedule_func;

    // Initialize SoftDevice.
   
    err_code = sd_softdevice_enable(clock_source, softdevice_assertion_handler);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    m_softdevice_enabled = true;
    // Enable BLE event interrupt (interrupt priority has already been set by the stack).
    return sd_nvic_EnableIRQ(SWI2_IRQn);
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
extern "C" void SWI2_IRQHandler(void)
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
