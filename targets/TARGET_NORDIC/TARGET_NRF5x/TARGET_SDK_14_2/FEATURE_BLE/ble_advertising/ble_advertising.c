/**
 * Copyright (c) 2015 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include  "sdk_common.h"
#if NRF_MODULE_ENABLED(BLE_ADVERTISING)
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "nrf_soc.h"
#include "nrf_log.h"
#include "nrf_fstorage.h"
#include "sdk_errors.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"

#define BLE_ADV_MODES (5) /**< Total number of possible advertising modes. */


#if (NRF_SD_BLE_API_VERSION <= 2)

    static bool whitelist_has_entries(ble_advertising_t * const p_advertising)
    {
        return ((p_advertising->whitelist.addr_count != 0) || (p_advertising->whitelist.irk_count != 0));
    }

#else

    static bool whitelist_has_entries(ble_advertising_t * const p_advertising)
    {
        return p_advertising->whitelist_in_use;
    }

#endif



/**@brief Function for checking if an address is valid.
 */
static bool addr_is_valid(uint8_t const * const addr)
{
    for (uint32_t i = 0; i < BLE_GAP_ADDR_LEN; i++)
    {
        if (addr[i] != 0)
        {
            return true;
        }
    }
    return false;
}


static ble_adv_mode_t adv_mode_next_get(ble_adv_mode_t adv_mode)
{
    return (ble_adv_mode_t)((adv_mode + 1) % BLE_ADV_MODES);
}


/**@brief Function for handling the Connected event.
 *
 * @param[in] p_ble_evt Event received from the BLE stack.
 */
static void on_connected(ble_advertising_t * const p_advertising, ble_evt_t const * p_ble_evt)
{
    if (p_ble_evt->evt.gap_evt.params.connected.role == BLE_GAP_ROLE_PERIPH)
    {
        p_advertising->current_slave_link_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
    }
}


/**@brief Function for handling the Disconnected event.
 *
 * @param[in] p_advertising Advertising module instance.
 * @param[in] p_ble_evt Event received from the BLE stack.
 */
static void on_disconnected(ble_advertising_t * const p_advertising, ble_evt_t const * p_ble_evt)
{
    uint32_t ret;

    p_advertising->whitelist_temporarily_disabled = false;

    if (p_ble_evt->evt.gap_evt.conn_handle == p_advertising->current_slave_link_conn_handle &&
        p_advertising->adv_modes_config.ble_adv_on_disconnect_disabled == false)
    {
       ret = ble_advertising_start(p_advertising, BLE_ADV_MODE_DIRECTED);
       if ((ret != NRF_SUCCESS) && (p_advertising->error_handler != NULL))
       {
           p_advertising->error_handler(ret);
       }
    }
}


/**@brief Function for handling the Timeout event.
 *
 * @param[in] p_advertising Advertising module instance.
 * @param[in] p_ble_evt Event received from the BLE stack.
 */
static void on_timeout(ble_advertising_t * const p_advertising, ble_evt_t const * p_ble_evt)
{
    ret_code_t ret;

    if (p_ble_evt->evt.gap_evt.params.timeout.src != BLE_GAP_TIMEOUT_SRC_ADVERTISING)
    {
        // Nothing to do.
        return;
    }

    // Start advertising in the next mode.
    ret = ble_advertising_start(p_advertising, adv_mode_next_get(p_advertising->adv_mode_current));

    if ((ret != NRF_SUCCESS) && (p_advertising->error_handler != NULL))
    {
        p_advertising->error_handler(ret);
    }
}


/** @brief Function to determine if a flash write operation in in progress.
 *
 * @return true if a flash operation is in progress, false if not.
 */
static bool flash_access_in_progress()
{
    return nrf_fstorage_is_busy(NULL);
}


/**@brief Get the next available advertising mode.
 *
 * @param[in] p_advertising Advertising module instance.
 * @param[in] adv_mode Requested advertising mode.
 *
 * @returns adv_mode if possible, or the best available mode if not.
 */
static ble_adv_mode_t adv_mode_next_avail_get(ble_advertising_t * const p_advertising,
                                              ble_adv_mode_t            adv_mode)
{
    bool peer_addr_is_valid = addr_is_valid(p_advertising->peer_address.addr);

    // If a mode is disabled, continue to the next mode.

    switch (adv_mode)
    {
        case BLE_ADV_MODE_DIRECTED:
            if ((p_advertising->adv_modes_config.ble_adv_directed_enabled) && peer_addr_is_valid)
            {
                return BLE_ADV_MODE_DIRECTED;
            }
            // Fallthrough.

        case BLE_ADV_MODE_DIRECTED_SLOW:
            if ((p_advertising->adv_modes_config.ble_adv_directed_slow_enabled) && peer_addr_is_valid)
            {
                return BLE_ADV_MODE_DIRECTED_SLOW;
            }
            // Fallthrough.

        case BLE_ADV_MODE_FAST:
            if (p_advertising->adv_modes_config.ble_adv_fast_enabled)
            {
                return BLE_ADV_MODE_FAST;
            }
            // Fallthrough.

        case BLE_ADV_MODE_SLOW:
            if (p_advertising->adv_modes_config.ble_adv_slow_enabled)
            {
                return BLE_ADV_MODE_SLOW;
            }
            // Fallthrough.

        default:
            return BLE_ADV_MODE_IDLE;
    }
}


/**@brief Function for starting directed advertising.
 *
 * @param[in]  p_advertising Advertising instance.
 * @param[out] p_adv_params Advertising parameters.
 *
 * @return NRF_SUCCESS
 */
static ret_code_t set_adv_mode_directed(ble_advertising_t * const p_advertising,
                                        ble_gap_adv_params_t    * p_adv_params)
{
    p_advertising->adv_evt  = BLE_ADV_EVT_DIRECTED;

    p_adv_params->p_peer_addr = &(p_advertising->peer_address);
    p_adv_params->type        = BLE_GAP_ADV_TYPE_ADV_DIRECT_IND;
    p_adv_params->timeout     = 0;
    p_adv_params->interval    = 0;

    return NRF_SUCCESS;
}


/**@brief Function for starting directed slow advertising.
 *
 * @param[in]  p_advertising Advertising module instance.
 * @param[out] p_adv_params Advertising parameters.
 *
 * @return NRF_SUCCESS
 */
static ret_code_t set_adv_mode_directed_slow(ble_advertising_t * const p_advertising,
                                             ble_gap_adv_params_t    * p_adv_params)
{
    p_advertising->adv_evt = BLE_ADV_EVT_DIRECTED_SLOW;

    p_adv_params->p_peer_addr = &p_advertising->peer_address;
    p_adv_params->type        = BLE_GAP_ADV_TYPE_ADV_DIRECT_IND;
    p_adv_params->timeout     = p_advertising->adv_modes_config.ble_adv_directed_slow_timeout;
    p_adv_params->interval    = p_advertising->adv_modes_config.ble_adv_directed_slow_interval;

    return NRF_SUCCESS;
}


/**@brief Function for starting fast advertising.
 *
 * @param[in]  p_advertising Advertising module instance.
 * @param[out] p_adv_params Advertising parameters.
 *
 * @return NRF_SUCCESS or an error from @ref ble_advdata_set().
 */
static ret_code_t set_adv_mode_fast(ble_advertising_t * const p_advertising,
                                    ble_gap_adv_params_t    * p_adv_params)
{
    ret_code_t ret;

    p_adv_params->interval = p_advertising->adv_modes_config.ble_adv_fast_interval;
    p_adv_params->timeout  = p_advertising->adv_modes_config.ble_adv_fast_timeout;

    if ((p_advertising->adv_modes_config.ble_adv_whitelist_enabled) &&
        (!p_advertising->whitelist_temporarily_disabled) &&
        (whitelist_has_entries(p_advertising)))
    {
        #if (NRF_SD_BLE_API_VERSION <= 2)
            p_adv_params->p_whitelist = &m_whitelist;
        #endif

        p_adv_params->fp = BLE_GAP_ADV_FP_FILTER_CONNREQ;
        p_advertising->advdata.flags  = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;

        ret = ble_advdata_set(&(p_advertising->advdata), NULL);
        if (ret != NRF_SUCCESS)
        {
            return ret;
        }

        p_advertising->adv_evt = BLE_ADV_EVT_FAST_WHITELIST;
    }
    else
    {
        p_advertising->adv_evt = BLE_ADV_EVT_FAST;
    }

    return NRF_SUCCESS;
}


/**@brief Function for starting slow advertising.
 *
 * @param[in]  p_advertising Advertising module instance.
 * @param[out] p_adv_params Advertising parameters.
 *
 * @return NRF_SUCCESS or an error from @ref ble_advdata_set().
 */
static ret_code_t set_adv_mode_slow(ble_advertising_t * const p_advertising,
                                    ble_gap_adv_params_t    * p_adv_params)
{
    ret_code_t ret;

    p_adv_params->interval = p_advertising->adv_modes_config.ble_adv_slow_interval;
    p_adv_params->timeout  = p_advertising->adv_modes_config.ble_adv_slow_timeout;

    if ((p_advertising->adv_modes_config.ble_adv_whitelist_enabled) &&
        (!p_advertising->whitelist_temporarily_disabled) &&
        (whitelist_has_entries(p_advertising)))
    {
        #if (NRF_SD_BLE_API_VERSION <= 2)
        {
            p_adv_params->p_whitelist = &p_advertising->whitelist;
        }
        #endif

        p_adv_params->fp = BLE_GAP_ADV_FP_FILTER_CONNREQ;
        p_advertising->advdata.flags  = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;

        ret = ble_advdata_set(&(p_advertising->advdata), NULL);
        if (ret != NRF_SUCCESS)
        {
            return ret;
        }

        p_advertising->adv_evt = BLE_ADV_EVT_SLOW_WHITELIST;
    }
    else
    {
        p_advertising->adv_evt = BLE_ADV_EVT_SLOW;
    }

    return NRF_SUCCESS;
}


void ble_advertising_conn_cfg_tag_set(ble_advertising_t * const p_advertising,
                                      uint8_t                   ble_cfg_tag)
{
    p_advertising->conn_cfg_tag = ble_cfg_tag;
}


uint32_t ble_advertising_init(ble_advertising_t            * const p_advertising,
                              ble_advertising_init_t const * const p_init)
{
    uint32_t ret;
    if ((p_init == NULL) || (p_advertising == NULL))
    {
        return NRF_ERROR_NULL;
    }

    p_advertising->initialized                    = true;
    p_advertising->adv_mode_current               = BLE_ADV_MODE_IDLE;
    p_advertising->adv_modes_config               = p_init->config;
    p_advertising->conn_cfg_tag                   = BLE_CONN_CFG_TAG_DEFAULT;
    p_advertising->evt_handler                    = p_init->evt_handler;
    p_advertising->error_handler                  = p_init->error_handler;
    p_advertising->current_slave_link_conn_handle = BLE_CONN_HANDLE_INVALID;

    memset(&p_advertising->peer_address, 0, sizeof(p_advertising->peer_address));
    memset(&p_advertising->advdata,      0, sizeof(p_advertising->advdata));

    // Copy advertising data.
    p_advertising->advdata.name_type          = p_init->advdata.name_type;
    p_advertising->advdata.include_appearance = p_init->advdata.include_appearance;
    p_advertising->advdata.flags              = p_init->advdata.flags;
    p_advertising->advdata.short_name_len     = p_init->advdata.short_name_len;

    p_advertising->advdata.uuids_complete          = p_init->advdata.uuids_complete;
    p_advertising->advdata.uuids_more_available    = p_init->advdata.uuids_more_available;
    p_advertising->advdata.uuids_solicited         = p_init->advdata.uuids_solicited;
    p_advertising->advdata.include_ble_device_addr = p_init->advdata.include_ble_device_addr;

    if (p_init->advdata.p_manuf_specific_data != NULL)
    {
        p_advertising->advdata.p_manuf_specific_data   = &(p_advertising->manuf_specific_data);
        p_advertising->manuf_specific_data.data.p_data = p_advertising->manuf_data_array;
        p_advertising->advdata.p_manuf_specific_data->company_identifier =
            p_init->advdata.p_manuf_specific_data->company_identifier;
        p_advertising->advdata.p_manuf_specific_data->data.size = p_init->advdata.p_manuf_specific_data->data.size;

        for (uint32_t i = 0; i < p_advertising->advdata.p_manuf_specific_data->data.size; i++)
        {
            p_advertising->manuf_data_array[i] = p_init->advdata.p_manuf_specific_data->data.p_data[i];
        }
    }

    if (p_init->advdata.p_service_data_array != NULL)
    {
        p_advertising->service_data.data.p_data                   = p_advertising->service_data_array;
        p_advertising->advdata.p_service_data_array               = &(p_advertising->service_data);
        p_advertising->advdata.p_service_data_array->data.p_data  = p_advertising->service_data_array;
        p_advertising->advdata.p_service_data_array->data.size    = p_init->advdata.p_service_data_array->data.size;
        p_advertising->advdata.p_service_data_array->service_uuid = p_init->advdata.p_service_data_array->service_uuid;

        for (uint32_t i = 0; i < p_advertising->advdata.p_service_data_array->data.size; i++)
        {
            p_advertising->service_data_array[i] = p_init->advdata.p_service_data_array->data.p_data[i];
        }

        p_advertising->advdata.service_data_count = p_init->advdata.service_data_count;
    }

    if (p_init->advdata.p_slave_conn_int != NULL)
    {
        p_advertising->advdata.p_slave_conn_int                    = &(p_advertising->slave_conn_int);
        p_advertising->advdata.p_slave_conn_int->max_conn_interval = p_init->advdata.p_slave_conn_int->max_conn_interval;
        p_advertising->advdata.p_slave_conn_int->min_conn_interval = p_init->advdata.p_slave_conn_int->min_conn_interval;
    }

    if (p_init->advdata.p_tx_power_level != NULL)
    {
        p_advertising->advdata.p_tx_power_level = p_init->advdata.p_tx_power_level;
    }

#if (NRF_SD_BLE_API_VERSION <= 2)
        for (int i = 0; i <BLE_GAP_WHITELIST_ADDR_MAX_COUNT ; i++)
        {
            p_advertising->whitelist.pp_addrs[i] = &p_advertising->whitelist_addrs[i];
        }

        for (int i = 0; i <BLE_GAP_WHITELIST_IRK_MAX_COUNT ; i++)
        {
            p_advertising->whitelist.pp_irks[i] = &p_advertising->whitelist_irks[i];
        }
#endif
    ret = ble_advdata_set(&(p_advertising->advdata), &(p_init->srdata));
    return ret;
}


uint32_t ble_advertising_start(ble_advertising_t * const p_advertising,
                               ble_adv_mode_t            advertising_mode)
{
    uint32_t             ret;
    ble_gap_adv_params_t adv_params;

    if (p_advertising->initialized == false)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    p_advertising->adv_mode_current = advertising_mode;

    // Delay starting advertising until the flash operations are complete.
    if (flash_access_in_progress())
    {
        p_advertising->advertising_start_pending = true;
        return NRF_SUCCESS;
    }

   memset(&p_advertising->peer_address, 0, sizeof(p_advertising->peer_address));

    if (  ((p_advertising->adv_modes_config.ble_adv_directed_enabled)      && (p_advertising->adv_mode_current == BLE_ADV_MODE_DIRECTED))
        ||((p_advertising->adv_modes_config.ble_adv_directed_slow_enabled) && (p_advertising->adv_mode_current == BLE_ADV_MODE_DIRECTED))
        ||((p_advertising->adv_modes_config.ble_adv_directed_slow_enabled) && (p_advertising->adv_mode_current == BLE_ADV_MODE_DIRECTED_SLOW))
       )
    {
        if (p_advertising->evt_handler != NULL)
        {
            p_advertising->peer_addr_reply_expected = true;
            p_advertising->evt_handler(BLE_ADV_EVT_PEER_ADDR_REQUEST);
        }
        else
        {
            p_advertising->peer_addr_reply_expected = false;
        }
    }

    p_advertising->adv_mode_current = adv_mode_next_avail_get(p_advertising, advertising_mode);

    // Fetch the whitelist.
    if ((p_advertising->evt_handler != NULL) &&
        (p_advertising->adv_mode_current == BLE_ADV_MODE_FAST || p_advertising->adv_mode_current == BLE_ADV_MODE_SLOW) &&
        (p_advertising->adv_modes_config.ble_adv_whitelist_enabled) &&
        (!p_advertising->whitelist_temporarily_disabled))
    {
        #if (NRF_SD_BLE_API_VERSION >= 3)
            p_advertising->whitelist_in_use = false;
        #endif
        p_advertising->whitelist_reply_expected = true;
        p_advertising->evt_handler(BLE_ADV_EVT_WHITELIST_REQUEST);
    }
    else
    {
        p_advertising->whitelist_reply_expected = false;
    }

    // Initialize advertising parameters with default values.
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.fp   = BLE_GAP_ADV_FP_ANY;

    // Set advertising parameters and events according to selected advertising mode.
    switch (p_advertising->adv_mode_current)
    {
        case BLE_ADV_MODE_DIRECTED:
            ret = set_adv_mode_directed(p_advertising, &adv_params);
            break;

        case BLE_ADV_MODE_DIRECTED_SLOW:
            ret = set_adv_mode_directed_slow(p_advertising, &adv_params);
            break;

        case BLE_ADV_MODE_FAST:
            ret = set_adv_mode_fast(p_advertising, &adv_params);
            break;

        case BLE_ADV_MODE_SLOW:
            ret = set_adv_mode_slow(p_advertising, &adv_params);
            break;

        case BLE_ADV_MODE_IDLE:
            p_advertising->adv_evt = BLE_ADV_EVT_IDLE;
            break;

        default:
            break;
    }

    if (p_advertising->adv_mode_current != BLE_ADV_MODE_IDLE)
    {
        ret = sd_ble_gap_adv_start(&adv_params, p_advertising->conn_cfg_tag);
        if (ret != NRF_SUCCESS)
        {
            return ret;
        }
    }

    if (p_advertising->evt_handler != NULL)
    {
        p_advertising->evt_handler(p_advertising->adv_evt);
    }

    return NRF_SUCCESS;
}


void ble_advertising_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_advertising_t * p_advertising = (ble_advertising_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connected(p_advertising, p_ble_evt);
            break;

        // Upon disconnection, whitelist will be activated and direct advertising is started.
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnected(p_advertising, p_ble_evt);
            break;

        // Upon time-out, the next advertising mode is started.
        case BLE_GAP_EVT_TIMEOUT:
            on_timeout(p_advertising, p_ble_evt);
            break;

        default:
            break;
    }
}


void ble_advertising_on_sys_evt(uint32_t evt_id, void * p_context)
{
    ble_advertising_t * p_advertising = (ble_advertising_t *)p_context;

    switch (evt_id)
    {
        //When a flash operation finishes, re-attempt to start advertising operations.
        case NRF_EVT_FLASH_OPERATION_SUCCESS:
        case NRF_EVT_FLASH_OPERATION_ERROR:
        {
            if (p_advertising->advertising_start_pending)
            {
                p_advertising->advertising_start_pending = false;
                ret_code_t ret = ble_advertising_start(p_advertising,
                                                       p_advertising->adv_mode_current);

                if ((ret != NRF_SUCCESS) && (p_advertising->error_handler != NULL))
                {
                    p_advertising->error_handler(ret);
                }
            }
        } break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_advertising_peer_addr_reply(ble_advertising_t * const p_advertising,
                                         ble_gap_addr_t          * p_peer_address)
{
    if (!p_advertising->peer_addr_reply_expected)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    p_advertising->peer_addr_reply_expected = false;

    memcpy(&p_advertising->peer_address, p_peer_address, sizeof(p_advertising->peer_address));

    return NRF_SUCCESS;
}


uint32_t ble_advertising_whitelist_reply(ble_advertising_t * const p_advertising,
                                         ble_gap_addr_t    const * p_gap_addrs,
                                         uint32_t               addr_cnt,
                                         ble_gap_irk_t  const * p_gap_irks,
                                         uint32_t               irk_cnt)
{
    if (!p_advertising->whitelist_reply_expected)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    p_advertising->whitelist_reply_expected = false;

    #if (NRF_SD_BLE_API_VERSION <= 2)

        p_advertising->whitelist.addr_count = addr_cnt;
        p_advertising->whitelist.irk_count  = irk_cnt;

        for (uint32_t i = 0; i < addr_cnt; i++)
        {
            *p_advertising->whitelist.pp_addrs[i] = p_gap_addrs[i];
        }

        for (uint32_t i = 0; i < irk_cnt; i++)
        {
            *p_advertising->whitelist.pp_irks[i] = p_gap_irks[i];
        }

    #else

        p_advertising->whitelist_in_use = ((addr_cnt > 0) || (irk_cnt > 0));

    #endif

    return NRF_SUCCESS;
}


uint32_t ble_advertising_restart_without_whitelist(ble_advertising_t * const p_advertising)
{
    uint32_t ret;

    (void) sd_ble_gap_adv_stop();

    p_advertising->whitelist_temporarily_disabled = true;

    #if (NRF_SD_BLE_API_VERSION >= 3)
        p_advertising->whitelist_in_use = false;
    #endif

    p_advertising->advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    ret = ble_advdata_set(&(p_advertising->advdata), NULL);
    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    ret = ble_advertising_start(p_advertising, p_advertising->adv_mode_current);
    if ((ret != NRF_SUCCESS) && (p_advertising->error_handler != NULL))
    {
        p_advertising->error_handler(ret);
    }

    return NRF_SUCCESS;
}


void ble_advertising_modes_config_set(ble_advertising_t            * const p_advertising,
                                      ble_adv_modes_config_t const * const p_adv_modes_config)
{
    p_advertising->adv_modes_config = *p_adv_modes_config;
}


#endif // NRF_MODULE_ENABLED(BLE_ADVERTISING)
