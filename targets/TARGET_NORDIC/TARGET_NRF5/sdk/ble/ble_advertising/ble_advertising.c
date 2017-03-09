/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
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



#include "ble_advdata.h"
#include "ble_advertising.h"
#include "nrf_soc.h"
#include "nrf_log.h"
#include "pstorage.h"
#include "fstorage.h"
#include "sdk_common.h"


#define ADV_LOG(...)

static bool                            m_advertising_start_pending = false; /**< Flag to keep track of ongoing operations on persistent memory. */

static ble_gap_addr_t                  m_peer_address;     /**< Address of the most recently connected peer, used for direct advertising. */
static ble_advdata_t                   m_advdata;          /**< Used by the initialization function to set name, appearance, and UUIDs and advertising flags visible to peer devices. */
static ble_adv_evt_t                   m_adv_evt;          /**< Advertising event propogated to the main application. The event is either a transaction to a new advertising mode, or a request for whitelist or peer address.. */
static ble_advertising_evt_handler_t   m_evt_handler;      /**< Handler for the advertising events. Can be initialized as NULL if no handling is implemented on in the main application. */
static ble_advertising_error_handler_t m_error_handler;    /**< Handler for the advertising error events. */

static ble_adv_mode_t                  m_adv_mode_current; /**< Variable to keep track of the current advertising mode. */
static ble_adv_modes_config_t          m_adv_modes_config; /**< Struct to keep track of disabled and enabled advertising modes, as well as time-outs and intervals.*/

static ble_gap_whitelist_t             m_whitelist;                                         /**< Struct that points to whitelisted addresses. */
static ble_gap_addr_t                * mp_whitelist_addr[BLE_GAP_WHITELIST_ADDR_MAX_COUNT]; /**< Pointer to a list of addresses. Pointed to by the whitelist */
static ble_gap_irk_t                 * mp_whitelist_irk[BLE_GAP_WHITELIST_IRK_MAX_COUNT];   /**< Pointer to a list of Identity Resolving Keys (IRK). Pointed to by the whitelist */
static bool                            m_whitelist_temporarily_disabled = false;            /**< Flag to keep track of temporary disabling of the whitelist. */
static bool                            m_whitelist_reply_expected = false;                  /**< Flag to verify that whitelist is only set when it is requested. */
static bool                            m_peer_addr_reply_expected = false;                  /**< Flag to verify that peer address is only set when requested. */

static ble_advdata_manuf_data_t        m_manuf_specific_data;                      /**< Manufacturer specific data structure*/
static uint8_t                         m_manuf_data_array[BLE_GAP_ADV_MAX_SIZE];   /**< Array to store the Manufacturer specific data*/
static ble_advdata_service_data_t      m_service_data;                             /**< Service data structure. */
static uint8_t                         m_service_data_array[BLE_GAP_ADV_MAX_SIZE]; /**< Array to store the service data. */
static ble_advdata_conn_int_t          m_slave_conn_int;                           /**< Connection interval range structure.*/
static int8_t                          m_tx_power_level;                           /**< TX power level*/


/**@brief Function for checking that the whitelist has entries.
 */
static bool whitelist_has_entries(ble_gap_whitelist_t const * whitelist)
{
    if ((whitelist->addr_count != 0) || (whitelist->irk_count != 0))
    {
        return true;
    }
    return false;
}


/**@brief Function for setting the stored peer address back to zero.
 */
static void ble_advertising_peer_address_clear()
{
    memset(&m_peer_address, 0, sizeof(m_peer_address));
}


/**@brief Function for checking if an address is non-zero. Used to determine if 
 */
static bool peer_address_exists(uint8_t const * address)
{
    uint32_t i;

    for (i = 0; i < BLE_GAP_ADDR_LEN; i++)
    {
        if (address[i] != 0)
        {
            return true;
        }
    }
    return false;
}


uint32_t ble_advertising_init(ble_advdata_t const                 * p_advdata,
                              ble_advdata_t const                 * p_srdata,
                              ble_adv_modes_config_t const        * p_config,
                              ble_advertising_evt_handler_t const   evt_handler,
                              ble_advertising_error_handler_t const error_handler)
{
    uint32_t err_code;

    VERIFY_PARAM_NOT_NULL(p_advdata);
    VERIFY_PARAM_NOT_NULL(p_config);

    m_adv_mode_current = BLE_ADV_MODE_IDLE;
    m_evt_handler      = evt_handler;
    m_error_handler    = error_handler;
    m_adv_modes_config = *p_config;

    ble_advertising_peer_address_clear();

    // Prepare Whitelist. Address and IRK double pointers point to allocated arrays.
    m_whitelist.pp_addrs = mp_whitelist_addr;
    m_whitelist.pp_irks  = mp_whitelist_irk;

    // Copy and set advertising data.
    memset(&m_advdata, 0, sizeof(m_advdata));

    // Copy advertising data.
    m_advdata.name_type            = p_advdata->name_type;
    m_advdata.include_appearance   = p_advdata->include_appearance;
    m_advdata.flags                = p_advdata->flags;
    m_advdata.short_name_len       = p_advdata->short_name_len;
   /* 
    if(p_advdata->uuids_complete != NULL)
    {
        m_advdata.uuids_complete = p_advdata->uuids_complete;
    }
    */
    m_advdata.uuids_complete       = p_advdata->uuids_complete;
    m_advdata.uuids_more_available = p_advdata->uuids_more_available;
    m_advdata.uuids_solicited      = p_advdata->uuids_solicited;
    
    if(p_advdata->p_manuf_specific_data != NULL)
    {
        m_advdata.p_manuf_specific_data   = &m_manuf_specific_data;
        m_manuf_specific_data.data.p_data = m_manuf_data_array;
        m_advdata.p_manuf_specific_data->company_identifier =
        p_advdata->p_manuf_specific_data->company_identifier;
        m_advdata.p_manuf_specific_data->data.size = p_advdata->p_manuf_specific_data->data.size;
        
        for(uint32_t i = 0; i < m_advdata.p_manuf_specific_data->data.size; i++)
        {
            m_manuf_data_array[i] = p_advdata->p_manuf_specific_data->data.p_data[i];
        }
    }
    
    if(p_advdata->p_service_data_array != NULL)
    {
        m_service_data.data.p_data                   = m_service_data_array;
        m_advdata.p_service_data_array               = &m_service_data;
        m_advdata.p_service_data_array->data.p_data  = m_service_data_array;
        m_advdata.p_service_data_array->data.size    = p_advdata->p_service_data_array->data.size;
        m_advdata.p_service_data_array->service_uuid = p_advdata->p_service_data_array->service_uuid;

        for(uint32_t i = 0; i < m_advdata.p_service_data_array->data.size; i++)
        {
            m_service_data_array[i] = p_advdata->p_service_data_array->data.p_data[i];
        }

        m_advdata.service_data_count = p_advdata->service_data_count;
    }


    if(p_advdata->p_slave_conn_int != NULL)
    {
        m_advdata.p_slave_conn_int                    = &m_slave_conn_int;
        m_advdata.p_slave_conn_int->max_conn_interval = p_advdata->p_slave_conn_int->max_conn_interval;
        m_advdata.p_slave_conn_int->min_conn_interval = p_advdata->p_slave_conn_int->min_conn_interval;
    }
    
    if(p_advdata->p_tx_power_level != NULL)
    {
        m_advdata.p_tx_power_level     = &m_tx_power_level;
        m_advdata.p_tx_power_level     = p_advdata->p_tx_power_level;
    }
    err_code = ble_advdata_set(&m_advdata, p_srdata);
    return err_code;
}

/** @brief Function to determine if a flash access in in progress. If it is the case, we can not
*          start advertising until it is finished. attempted restart
*          in @ref ble_advertising_on_sys_evt
*
* @return true if a flash access is in progress, false if not.
*/
static bool flash_access_in_progress()
{
    uint32_t err_code;
    uint32_t count = 0;

    err_code = pstorage_access_status_get(&count);
    if ((err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_SUCCESS))
    {
        ADV_LOG("[ADV]: pstorage_access_status_get returned %d.\r\n", err_code);
        return true;
    }

    if (err_code == NRF_ERROR_INVALID_STATE)
    {
        err_code = fs_queued_op_count_get(&count);
        if (err_code != FS_SUCCESS)
        {
            return false;
        }
        ADV_LOG("[ADV]: fs_queued_op_count_get gives count %d.\r\n", count);
    }

    if(count != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint32_t ble_advertising_start(ble_adv_mode_t advertising_mode)
{
    uint32_t             err_code;
    ble_gap_adv_params_t adv_params;

    m_adv_mode_current = advertising_mode;

    // Verify if there are any pending flash operations. If so, delay starting advertising until
    // the flash operations are complete.
    if(flash_access_in_progress())
    {
        m_advertising_start_pending = true;
        return NRF_SUCCESS;
    }

    ADV_LOG("[ADV]: no flash operations in progress, prepare advertising.\r\n");
    // Fetch the peer address.
    ble_advertising_peer_address_clear();

    if (  ((m_adv_modes_config.ble_adv_directed_enabled)      && (m_adv_mode_current == BLE_ADV_MODE_DIRECTED))
        ||((m_adv_modes_config.ble_adv_directed_slow_enabled) && (m_adv_mode_current == BLE_ADV_MODE_DIRECTED))
        ||((m_adv_modes_config.ble_adv_directed_slow_enabled) && (m_adv_mode_current == BLE_ADV_MODE_DIRECTED_SLOW))
       )
    {
        if (m_evt_handler != NULL)
        {
            m_peer_addr_reply_expected = true;
            m_evt_handler(BLE_ADV_EVT_PEER_ADDR_REQUEST);
        }
        else
        {
            m_peer_addr_reply_expected = false;
        }
    }

    // If a mode is disabled, continue to the next mode. I.e fast instead of direct, slow instead of fast, idle instead of slow.
    if (  (m_adv_mode_current == BLE_ADV_MODE_DIRECTED)
        &&(!m_adv_modes_config.ble_adv_directed_enabled || !peer_address_exists(m_peer_address.addr)))
    {
        m_adv_mode_current = BLE_ADV_MODE_DIRECTED_SLOW;
    }
    if (  (m_adv_mode_current == BLE_ADV_MODE_DIRECTED_SLOW)
        &&(!m_adv_modes_config.ble_adv_directed_slow_enabled || !peer_address_exists(m_peer_address.addr)))
    {
        m_adv_mode_current = BLE_ADV_MODE_FAST;
    }
    if (!m_adv_modes_config.ble_adv_fast_enabled && m_adv_mode_current == BLE_ADV_MODE_FAST)
    {
        m_adv_mode_current = BLE_ADV_MODE_SLOW;
    }
    if (!m_adv_modes_config.ble_adv_slow_enabled && m_adv_mode_current == BLE_ADV_MODE_SLOW)
    {
        m_adv_mode_current = BLE_ADV_MODE_IDLE;
        m_adv_evt          = BLE_ADV_EVT_IDLE;
    }

    // Fetch the whitelist.
    if (   (m_evt_handler != NULL)
        && (m_adv_mode_current == BLE_ADV_MODE_FAST || m_adv_mode_current == BLE_ADV_MODE_SLOW)
        && (m_adv_modes_config.ble_adv_whitelist_enabled)
        && (!m_whitelist_temporarily_disabled))
    {
        m_whitelist_reply_expected = true;
        m_evt_handler(BLE_ADV_EVT_WHITELIST_REQUEST);
    }
    else
    {
        m_whitelist_reply_expected = false;
    }

    // Initialize advertising parameters with default values.
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.p_whitelist = NULL;

    // Set advertising parameters and events according to selected advertising mode.
    switch (m_adv_mode_current)
    {
        case BLE_ADV_MODE_DIRECTED:
            ADV_LOG("[ADV]: Starting direct advertisement.\r\n");
            adv_params.p_peer_addr = &m_peer_address; // Directed advertising.
            adv_params.type        = BLE_GAP_ADV_TYPE_ADV_DIRECT_IND;
            adv_params.timeout     = 0;
            adv_params.interval    = 0;
            m_adv_evt              = BLE_ADV_EVT_DIRECTED;
            break;

        case BLE_ADV_MODE_DIRECTED_SLOW:
            ADV_LOG("[ADV]: Starting direct advertisement.\r\n");
            adv_params.p_peer_addr = &m_peer_address; // Directed advertising.
            adv_params.type        = BLE_GAP_ADV_TYPE_ADV_DIRECT_IND;
            adv_params.timeout     = m_adv_modes_config.ble_adv_directed_slow_timeout;
            adv_params.interval    = m_adv_modes_config.ble_adv_directed_slow_interval;
            m_adv_evt              = BLE_ADV_EVT_DIRECTED_SLOW;
            break;

        case BLE_ADV_MODE_FAST:
            adv_params.timeout  = m_adv_modes_config.ble_adv_fast_timeout;
            adv_params.interval = m_adv_modes_config.ble_adv_fast_interval;

            if (   whitelist_has_entries(&m_whitelist)
                && m_adv_modes_config.ble_adv_whitelist_enabled
                && !m_whitelist_temporarily_disabled)
            {
                adv_params.fp          = BLE_GAP_ADV_FP_FILTER_CONNREQ;
                adv_params.p_whitelist = &m_whitelist;
                m_advdata.flags        = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
                err_code               = ble_advdata_set(&m_advdata, NULL);
                VERIFY_SUCCESS(err_code);

                m_adv_evt = BLE_ADV_EVT_FAST_WHITELIST;
                ADV_LOG("[ADV]: Starting fast advertisement with whitelist.\r\n");
            }
            else
            {
                m_adv_evt = BLE_ADV_EVT_FAST;
                ADV_LOG("[ADV]: Starting fast advertisement.\r\n");
            }
            break;

        case BLE_ADV_MODE_SLOW:
            adv_params.interval = m_adv_modes_config.ble_adv_slow_interval;
            adv_params.timeout  = m_adv_modes_config.ble_adv_slow_timeout;

            if (   whitelist_has_entries(&m_whitelist)
                && m_adv_modes_config.ble_adv_whitelist_enabled
                && !m_whitelist_temporarily_disabled)
            {
                adv_params.fp          = BLE_GAP_ADV_FP_FILTER_CONNREQ;
                adv_params.p_whitelist = &m_whitelist;
                m_advdata.flags        = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
                err_code               = ble_advdata_set(&m_advdata, NULL);
                VERIFY_SUCCESS(err_code);

                m_adv_evt = BLE_ADV_EVT_SLOW_WHITELIST;
                ADV_LOG("[ADV]: Starting slow advertisement with whitelist.\r\n");
            }
            else
            {
                m_adv_evt = BLE_ADV_EVT_SLOW;
                ADV_LOG("[ADV]: Starting slow advertisement.\r\n");
            }
            break;

        default:
            break;
    }
    if (m_adv_mode_current != BLE_ADV_MODE_IDLE)
    {
        err_code = sd_ble_gap_adv_start(&adv_params);
        VERIFY_SUCCESS(err_code);
    }
    if (m_evt_handler != NULL)
    {
        m_evt_handler(m_adv_evt);
    }

    return NRF_SUCCESS;
}


void ble_advertising_on_ble_evt(ble_evt_t const * p_ble_evt)
{
    static uint16_t current_slave_link_conn_handle = BLE_CONN_HANDLE_INVALID;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            if (p_ble_evt->evt.gap_evt.params.connected.role == BLE_GAP_ROLE_PERIPH)
            {
                current_slave_link_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            }
            break;

        // Upon disconnection, whitelist will be activated and direct advertising is started.
        case BLE_GAP_EVT_DISCONNECTED:
        {
            uint32_t err_code;
            m_whitelist_temporarily_disabled = false;

            if (p_ble_evt->evt.gap_evt.conn_handle == current_slave_link_conn_handle)
            {
               err_code = ble_advertising_start(BLE_ADV_MODE_DIRECTED);
               if ((err_code != NRF_SUCCESS) && (m_error_handler != NULL))
               {
                   m_error_handler(err_code);
               }
            }
            break;
        }
        // Upon time-out, the next advertising mode is started, i.e. go from fast to slow or from slow to idle.
        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING)
            {
                switch (m_adv_mode_current)
                {
                    case BLE_ADV_MODE_DIRECTED:
                        ADV_LOG("[ADV]: Timed out from directed advertising.\r\n");
                        {
                            uint32_t err_code;
                            err_code = ble_advertising_start(BLE_ADV_MODE_DIRECTED_SLOW);
                            if ((err_code != NRF_SUCCESS) && (m_error_handler != NULL))
                            {
                                m_error_handler(err_code);
                            }
                        }
                        break;
                    case BLE_ADV_MODE_DIRECTED_SLOW:
                        ADV_LOG("[ADV]: Timed out from directed slow advertising.\r\n");
                        {
                            uint32_t err_code;
                            err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
                            if ((err_code != NRF_SUCCESS) && (m_error_handler != NULL))
                            {
                                m_error_handler(err_code);
                            }
                        }
                        break;
                    case BLE_ADV_MODE_FAST:
                    {
                        uint32_t err_code;
                        m_adv_evt = BLE_ADV_EVT_FAST;
                        ADV_LOG("[ADV]: Timed out from fast advertising, starting slow advertising.\r\n");
                        err_code = ble_advertising_start(BLE_ADV_MODE_SLOW);
                        if ((err_code != NRF_SUCCESS) && (m_error_handler != NULL))
                        {
                            m_error_handler(err_code);
                        }
                        break;
                    }
                    case BLE_ADV_MODE_SLOW:
                        m_adv_evt = BLE_ADV_EVT_IDLE;
                        ADV_LOG("[ADV]: Timed out from slow advertising, stopping advertising.\r\n");
                        if (m_evt_handler != NULL)
                        {
                            m_evt_handler(m_adv_evt);
                        }
                        break;

                    default:
                        // No implementation needed.
                        break;
                }
            }
            break;

        default:
            // No implementation needed.
            break;
    }
}
void ble_advertising_on_sys_evt(uint32_t sys_evt)
{
    uint32_t err_code = NRF_SUCCESS;
    switch (sys_evt)
    {

        case NRF_EVT_FLASH_OPERATION_SUCCESS:
        // Fall through.

        //When a flash operation finishes, advertising no longer needs to be pending.
        case NRF_EVT_FLASH_OPERATION_ERROR:
            if (m_advertising_start_pending)
            {
                m_advertising_start_pending = false;
                err_code = ble_advertising_start(m_adv_mode_current);
                if ((err_code != NRF_SUCCESS) && (m_error_handler != NULL))
                {
                    m_error_handler(err_code);
                }
            }
            break;

        default:
            // No implementation needed.
            break;
    }
}

uint32_t ble_advertising_peer_addr_reply(ble_gap_addr_t * p_peer_address)
{
    if(m_peer_addr_reply_expected == false)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    m_peer_address.addr_type = p_peer_address->addr_type;

    for (int i = 0; i < BLE_GAP_ADDR_LEN; i++)
    {
        m_peer_address.addr[i] = p_peer_address->addr[i];
    }

    m_peer_addr_reply_expected = false;
    return NRF_SUCCESS;
}


uint32_t ble_advertising_whitelist_reply(ble_gap_whitelist_t * p_whitelist)
{
    uint32_t i;

    if(m_whitelist_reply_expected == false)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    m_whitelist.addr_count = p_whitelist->addr_count;
    m_whitelist.irk_count  = p_whitelist->irk_count;

    for (i = 0; i < m_whitelist.irk_count; i++)
    {
        mp_whitelist_irk[i] = p_whitelist->pp_irks[i];
    }

    for (i = 0; i < m_whitelist.addr_count; i++)
    {
        mp_whitelist_addr[i] = p_whitelist->pp_addrs[i];
    }

    m_whitelist_reply_expected = false;
    return NRF_SUCCESS;
}


uint32_t ble_advertising_restart_without_whitelist(void)
{
    uint32_t err_code;

    if(     m_adv_modes_config.ble_adv_whitelist_enabled == BLE_ADV_WHITELIST_ENABLED
        && !m_whitelist_temporarily_disabled)
    {
        if (m_adv_mode_current != BLE_ADV_MODE_IDLE)
        {
            err_code = sd_ble_gap_adv_stop();
            VERIFY_SUCCESS(err_code);
        }
        m_whitelist_temporarily_disabled = true;
        m_advdata.flags                  = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
        err_code                         = ble_advdata_set(&m_advdata, NULL);
        VERIFY_SUCCESS(err_code);

        err_code = ble_advertising_start(m_adv_mode_current);
        if ((err_code != NRF_SUCCESS) && (m_error_handler != NULL))
        {
            m_error_handler(err_code);
        }
    }
    return NRF_SUCCESS;
}


