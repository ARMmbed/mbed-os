/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common/common.h"
#include "nordic_common.h"

#include "btle.h"
#include "btle_clock.h"

#include "ble_flash.h"

#include "custom/custom_helper.h"

#include "ble/GapEvents.h"
#include "nRF5xn.h"

#ifdef S110
    #define IS_LEGACY_DEVICE_MANAGER_ENABLED 1
#elif defined(S130) || defined(S132)
    #define IS_LEGACY_DEVICE_MANAGER_ENABLED 0
#endif

extern "C" {
#if (IS_LEGACY_DEVICE_MANAGER_ENABLED)
    #include "pstorage.h"
    #include "device_manager.h"
#else
    #include "nrf_fstorage.h"
    #include "fds.h"
#endif

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
}

#include "headers/ble_hci.h"

#include "nRF5xPalGattClient.h"

// This is a C++ file, so C11 _Static_assert (works with -std=gnu99 on GCC) won't work
#undef STATIC_ASSERT_SIMPLE
#undef STATIC_ASSERT_MSG

// FIXME : We can't use mbed_assert.h because we're using these macros within functions
#define STATIC_ASSERT_MSG(EXPR, MSG)
#define STATIC_ASSERT_SIMPLE(EXPR)

#warning FIXME : We can't use mbed_assert.h because we're using these within functions


// Make this volatile at it will be set in interrupt context
volatile bool isEventsSignaled = false;

extern "C" void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name);
void            app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name);

// Before SDK 14, the softdevice handler is implemented within the SDK
// In SDK 14+, we have to implement it as we're using the "polling" mode for the SD
extern "C" void SD_EVT_IRQHandler(void); // export the softdevice event handler for registration by nvic-set-vector.

#if NRF_SDK14PLUS_EVENT_HANDLERS
void btle_handler(const ble_evt_t *p_ble_evt, void *p_context);
#else
void btle_handler(ble_evt_t *p_ble_evt);
#endif

#if !NRF_SDK14PLUS_EVENT_HANDLERS
static void sys_evt_dispatch(uint32_t sys_evt)
{
#if (IS_LEGACY_DEVICE_MANAGER_ENABLED)
    pstorage_sys_event_handler(sys_evt);
#else
    // Forward Softdevice events to the fstorage module
    fs_sys_event_handler(sys_evt);
#endif
}
#endif

/**
 * This function is called in interrupt context to handle BLE events; i.e. pull
 * system and user events out of the pending events-queue of the BLE stack. The
 * BLE stack signals the availability of events by the triggering the SWI2
 * interrupt, which forwards the handling to this function.
 *
 * The event processing loop is implemented in intern_softdevice_events_execute().
 *
 * This function will signal to the user code by calling signalEventsToProcess
 * that their is events to process and BLE::processEvents should be called.
 */
static uint32_t signalEvent()
{
    if(isEventsSignaled == false) {
        isEventsSignaled = true;
        nRF5xn::Instance(BLE::DEFAULT_INSTANCE).signalEventsToProcess(BLE::DEFAULT_INSTANCE);
    }
    return NRF_SUCCESS;
}


error_t btle_init(void)
{
    nrf_clock_lf_cfg_t clockConfiguration;
    ret_code_t err_code;

    // register softdevice handler vector
    NVIC_SetVector(SD_EVT_IRQn, (uint32_t) SD_EVT_IRQHandler);

#if  (NRF_SD_BLE_API_VERSION >= 5)
    err_code = nrf_sdh_enable_request();
    ASSERT_STATUS(err_code);
#else
    // Configure the LF clock according to values provided by btle_clock.h.
    // It is input from the chain of the yotta configuration system.
    clockConfiguration.source        = LFCLK_CONF_SOURCE;
    clockConfiguration.xtal_accuracy = LFCLK_CONF_ACCURACY;
    clockConfiguration.rc_ctiv       = LFCLK_CONF_RC_CTIV;
    clockConfiguration.rc_temp_ctiv  = LFCLK_CONF_RC_TEMP_CTIV;

    SOFTDEVICE_HANDLER_INIT(&clockConfiguration, signalEvent);
#endif

    // Enable BLE stack
    #if  (NRF_SD_BLE_API_VERSION >= 5)
    // Configure softdevice manually
    // We could have used nrf_sdh_ble_default_cfg_set() but it's tightly coupled with the macros defined in sdk_config.h
    ble_cfg_t ble_cfg;
    uint32_t ram_start = 0;

    // Recover start address of application's RAM 
    err_code = nrf_sdh_ble_app_ram_start_get(&ram_start);
    ASSERT_STATUS(err_code);

    // First configure GAP parameters, including the maximum number of connections
    memset(&ble_cfg, 0, sizeof(ble_cfg_t));
    ble_cfg.conn_cfg.conn_cfg_tag = NRF_CONNECTION_TAG;
    ble_cfg.conn_cfg.params.gap_conn_cfg.conn_count   = TOTAL_LINK_COUNT;
    ble_cfg.conn_cfg.params.gap_conn_cfg.event_length = NRF_SDH_BLE_GAP_EVENT_LENGTH; // FIXME?

    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GAP, &ble_cfg, ram_start);
    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    // GAP - Configure the number of peripheral and central links
    memset(&ble_cfg, 0, sizeof(ble_cfg_t));
    ble_cfg.conn_cfg.conn_cfg_tag = NRF_CONNECTION_TAG;
    ble_cfg.gap_cfg.role_count_cfg.periph_role_count  = PERIPHERAL_LINK_COUNT;
    ble_cfg.gap_cfg.role_count_cfg.central_role_count = CENTRAL_LINK_COUNT;
    ble_cfg.gap_cfg.role_count_cfg.central_sec_count  = CENTRAL_LINK_COUNT ?
                                                        BLE_GAP_ROLE_COUNT_CENTRAL_SEC_DEFAULT : 0;

    err_code = sd_ble_cfg_set(BLE_GAP_CFG_ROLE_COUNT, &ble_cfg, ram_start);
    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    // Configure GATT
    memset(&ble_cfg, 0, sizeof(ble_cfg_t));
    ble_cfg.conn_cfg.conn_cfg_tag                 = NRF_CONNECTION_TAG;
    ble_cfg.conn_cfg.params.gatt_conn_cfg.att_mtu = NRF_SDH_BLE_GATT_MAX_MTU_SIZE;

    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GATT, &ble_cfg, ram_start);
    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    // Number of custom UUIDs
    memset(&ble_cfg, 0, sizeof(ble_cfg_t));
    ble_cfg.common_cfg.vs_uuid_cfg.vs_uuid_count = UUID_TABLE_MAX_ENTRIES;

    err_code = sd_ble_cfg_set(BLE_COMMON_CFG_VS_UUID, &ble_cfg, ram_start);
    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    // GATT Server attribute table size
    memset(&ble_cfg, 0, sizeof(ble_cfg_t));
    ble_cfg.gatts_cfg.attr_tab_size.attr_tab_size = GATTS_ATTR_TAB_SIZE;

    err_code = sd_ble_cfg_set(BLE_GATTS_CFG_ATTR_TAB_SIZE, &ble_cfg, ram_start);
    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    // GATT Server, Service Changed characteristic
    memset(&ble_cfg, 0, sizeof(ble_cfg_t));
    ble_cfg.gatts_cfg.service_changed.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;

    err_code = sd_ble_cfg_set(BLE_GATTS_CFG_SERVICE_CHANGED, &ble_cfg, ram_start);
    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    // Enable BLE stack in softdevice
    err_code = nrf_sdh_ble_enable(&ram_start);
    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }
    #else
    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);

    ble_enable_params.gatts_enable_params.attr_tab_size  = GATTS_ATTR_TAB_SIZE;
    ble_enable_params.gatts_enable_params.service_changed  = IS_SRVC_CHANGED_CHARACT_PRESENT;
    ble_enable_params.common_enable_params.vs_uuid_count = UUID_TABLE_MAX_ENTRIES;

    if(err_code  != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    if (softdevice_enable(&ble_enable_params) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }
    #endif

#if  (NRF_SD_BLE_API_VERSION <= 2)
    ble_gap_addr_t addr;
    if (sd_ble_gap_address_get(&addr) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }
    if (sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &addr) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }
#endif

// From SDK 14 onwards event handlers are registered differently
// http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v14.0.0%2Fmigration.html
#if NRF_SDK14PLUS_EVENT_HANDLERS
    // Register a handler for our BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, 3 /* default priority for user events */, btle_handler, NULL);
#else
    ASSERT_STATUS( softdevice_ble_evt_handler_set(btle_handler));
    ASSERT_STATUS( softdevice_sys_evt_handler_set(sys_evt_dispatch));
#endif

    return ERROR_NONE;
}

#if NRF_SDK14PLUS_EVENT_HANDLERS
void btle_handler(const ble_evt_t *p_ble_evt, void *p_context)
#else
void btle_handler(const ble_evt_t *p_ble_evt)
#endif
{
#if NRF_SDK14PLUS_EVENT_HANDLERS
    (void)p_context; // Keep compiler happy
#endif
    using ble::pal::vendor::nordic::nRF5xGattClient;
    using ble::pal::vendor::nordic::nRF5xSecurityManager;

// In SDK14+, all other modules from the SDK will be registered independently as softdevice events observers
#if !NRF_SDK14PLUS_EVENT_HANDLERS
    /* Library service handlers */
#if SDK_CONN_PARAMS_MODULE_ENABLE
    ble_conn_params_on_ble_evt(p_ble_evt);
#endif

#if (IS_LEGACY_DEVICE_MANAGER_ENABLED)
    dm_ble_evt_handler(p_ble_evt);
#else
    // Forward BLE events to the Connection State module.
    // This must be called before any event handler that uses this module.
    ble_conn_state_on_ble_evt(p_ble_evt);
#endif
#endif

#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
    nRF5xGattClient::handle_events(p_ble_evt);
#endif

    nRF5xn               &ble             = nRF5xn::Instance(BLE::DEFAULT_INSTANCE);
    nRF5xGap             &gap             = (nRF5xGap &) ble.getGap();
    nRF5xGattServer      &gattServer      = (nRF5xGattServer &) ble.getGattServer();
    nRF5xSecurityManager &securityManager = nRF5xSecurityManager::get_security_manager();

    /* Custom event handler */
    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            gap.on_connection(
                p_ble_evt->evt.gap_evt.conn_handle,
                p_ble_evt->evt.gap_evt.params.connected
            );
            break;

        case BLE_GAP_EVT_DISCONNECTED: {
            Gap::Handle_t handle = p_ble_evt->evt.gap_evt.conn_handle;
            // Since we are not in a connection and have not started advertising,
            // store bonds
            gap.setConnectionHandle (BLE_CONN_HANDLE_INVALID);

            Gap::DisconnectionReason_t reason;
            switch (p_ble_evt->evt.gap_evt.params.disconnected.reason) {
                case BLE_HCI_LOCAL_HOST_TERMINATED_CONNECTION:
                    reason = Gap::LOCAL_HOST_TERMINATED_CONNECTION;
                    break;
                case BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION:
                    reason = Gap::REMOTE_USER_TERMINATED_CONNECTION;
                    break;
                case BLE_HCI_CONN_INTERVAL_UNACCEPTABLE:
                    reason = Gap::CONN_INTERVAL_UNACCEPTABLE;
                    break;
                default:
                    /* Please refer to the underlying transport library for an
                     * interpretion of this reason's value. */
                    reason = static_cast<Gap::DisconnectionReason_t>(p_ble_evt->evt.gap_evt.params.disconnected.reason);
                    break;
            }

#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
            // Close all pending discoveries for this connection
            nRF5xGattClient::handle_connection_termination(handle);
#endif

            gap.processDisconnectionEvent(handle, reason);
            break;
        }


#if  (NRF_SD_BLE_API_VERSION >= 5)
#ifndef S140
        // Handle PHY upgrade request
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            ble_gap_phys_t const phys =
            {
                /* rx_phys */ BLE_GAP_PHY_AUTO,
                /* tx_phys */ BLE_GAP_PHY_AUTO,
            };
            ASSERT_STATUS_RET_VOID( sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys) );
            break;
        }
#endif

        	// Handle Data length negotiation request
        case BLE_GAP_EVT_DATA_LENGTH_UPDATE_REQUEST:
        {
            ble_gap_evt_t       const * p_gap_evt = &p_ble_evt->evt.gap_evt;
            uint8_t const data_length_peer =
                p_gap_evt->params.data_length_update_request.peer_params.max_tx_octets;

            const uint8_t max_data_length = NRF_SDH_BLE_GATT_MAX_MTU_SIZE + 4 /* L2CAP header size */;

            uint8_t const data_length = MIN(max_data_length, data_length_peer);

            ble_gap_data_length_params_t const dlp =
            {
                /* max_rx_octets */ data_length,
                /* max_tx_octets */ data_length
            };

            ASSERT_STATUS_RET_VOID(sd_ble_gap_data_length_update(p_gap_evt->conn_handle, &dlp, NULL));
        		break;
        }

        	// Handle MTU exchange request
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
        {
        		// Respond with the server MTU
            uint16_t   conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;
            ASSERT_STATUS_RET_VOID(sd_ble_gatts_exchange_mtu_reply(conn_handle, NRF_SDH_BLE_GATT_MAX_MTU_SIZE));
        		break;
        }
#endif
        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST: {
            Gap::Handle_t connection = p_ble_evt->evt.gap_evt.conn_handle;
            const ble_gap_evt_conn_param_update_request_t *update_request =
                &p_ble_evt->evt.gap_evt.params.conn_param_update_request;

            sd_ble_gap_conn_param_update(connection, &update_request->conn_params);
            break;
        }

        case BLE_GAP_EVT_TIMEOUT:
            gap.processTimeoutEvent(static_cast<Gap::TimeoutSource_t>(p_ble_evt->evt.gap_evt.params.timeout.src));
            break;

        case BLE_GATTC_EVT_TIMEOUT:
        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server and Client timeout events.
            // ASSERT_STATUS_RET_VOID (sd_ble_gap_disconnect(m_conn_handle,
            // BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
            break;

        case BLE_GAP_EVT_ADV_REPORT:
            gap.on_advertising_packet(p_ble_evt->evt.gap_evt.params.adv_report);
            break;

        default:
            break;
    }

    // Process security manager events
    securityManager.sm_handler(p_ble_evt);

    gattServer.hwCallback(p_ble_evt);
}

/*! @brief      Callback when an error occurs inside the SoftDevice or ASSERT in debug*/
void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name)
{
    error("nrf failure at %s:%d", p_file_name, line_num);
}

#if NRF_SD_BLE_API_VERSION >= 5
/*!
    @brief      Handler for general errors above the SoftDevice layer.
                Typically we can' recover from this so we do a reset.
                This implementation will override the default weak symbol generated by the Nordic SDK
*/
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    ASSERT_STATUS_RET_VOID( id );
    NVIC_SystemReset();
}
#else
/*!
    @brief      Handler for general errors above the SoftDevice layer.
                Typically we can' recover from this so we do a reset.
*/
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name)
{
    ASSERT_STATUS_RET_VOID( error_code );
    NVIC_SystemReset();
}
#endif

#if NRF_SDK14PLUS_EVENT_HANDLERS
/*!
    @brief      Handler of Softdevice events.
                This signals that the softdevie has events that need to be processed.
*/
extern "C" void SD_EVT_IRQHandler(void)
{
    ASSERT_STATUS_RET_VOID(signalEvent());
}
#endif
