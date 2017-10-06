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
#include "ble_conn_params.h"

#include "btle_gap.h"
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
    #include "fstorage.h"
    #include "fds.h"
    #include "peer_manager.h"
    #include "ble_conn_state.h"
#endif

#include "softdevice_handler.h"
#include "ble_stack_handler_types.h"
}

#include "nrf_ble_hci.h"
#include "btle_discovery.h"

#include "nRF5xGattClient.h"
#include "nRF5xServiceDiscovery.h"
#include "nRF5xCharacteristicDescriptorDiscoverer.h"


bool isEventsSignaled = false;

extern "C" void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name);
void            app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name);
extern "C" void SD_EVT_IRQHandler(void); // export the softdevice event handler for registration by nvic-set-vector.


static void btle_handler(ble_evt_t *p_ble_evt);

#if 0
#define CENTRAL_LINK_COUNT    (YOTTA_CFG_NORDIC_BLE_CENTRAL_LINKS)  /**<number of central links used by the application. When changing this number remember to adjust the RAM settings */
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_PERIPHERAL_LINKS was used, ram settings are adjusted by the yotta target module. */
#define PERIPHERAL_LINK_COUNT (YOTTA_CFG_NORDIC_BLE_PERIPHERAL_LINKS)     /**<number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_CENTRAL_LINKS was used, ram settings are adjusted by the yotta target module. */
#define GATTS_ATTR_TAB_SIZE (YOTTA_CFG_NORDIC_BLE_GATTS_ATTR_TAB_SIZE) /**< GATTS attribite table size. */
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_GATTS_ATTR_TAB_SIZE was used, ram settings are adjusted by the yotta target module. */
#else
#define CENTRAL_LINK_COUNT    3  /**<number of central links used by the application. When changing this number remember to adjust the RAM settings */
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_PERIPHERAL_LINKS was used, ram settings are adjusted by the yotta target module. */
#define PERIPHERAL_LINK_COUNT 1     /**<number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_CENTRAL_LINKS was used, ram settings are adjusted by the yotta target module. */
#define GATTS_ATTR_TAB_SIZE 0x600 /**< GATTS attribite table size. */
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_GATTS_ATTR_TAB_SIZE was used, ram settings are adjusted by the yotta target module. */

#endif

static void sys_evt_dispatch(uint32_t sys_evt)
{
#if (IS_LEGACY_DEVICE_MANAGER_ENABLED)
    pstorage_sys_event_handler(sys_evt);
#else
    // Forward Softdevice events to the fstorage module
    fs_sys_event_handler(sys_evt);
#endif
}

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

    // register softdevice handler vector
    NVIC_SetVector(SD_EVT_IRQn, (uint32_t) SD_EVT_IRQHandler);
    
    // Configure the LF clock according to values provided by btle_clock.h.
    // It is input from the chain of the yotta configuration system.
    clockConfiguration.source        = LFCLK_CONF_SOURCE;
    clockConfiguration.xtal_accuracy = LFCLK_CONF_ACCURACY;
    clockConfiguration.rc_ctiv       = LFCLK_CONF_RC_CTIV;
    clockConfiguration.rc_temp_ctiv  = LFCLK_CONF_RC_TEMP_CTIV;

    SOFTDEVICE_HANDLER_INIT(&clockConfiguration, signalEvent);

    // Enable BLE stack
    /**
     * Using this call, the application can select whether to include the
     * Service Changed characteristic in the GATT Server. The default in all
     * previous releases has been to include the Service Changed characteristic,
     * but this affects how GATT clients behave. Specifically, it requires
     * clients to subscribe to this attribute and not to cache attribute handles
     * between connections unless the devices are bonded. If the application
     * does not need to change the structure of the GATT server attributes at
     * runtime this adds unnecessary complexity to the interaction with peer
     * clients. If the SoftDevice is enabled with the Service Changed
     * Characteristics turned off, then clients are allowed to cache attribute
     * handles making applications simpler on both sides.
     */
    static const bool IS_SRVC_CHANGED_CHARACT_PRESENT = true;

    ble_enable_params_t ble_enable_params;
    uint32_t err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
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

    // Peer Manger must been initialised prior any other call to its API (this file and btle_security_pm.cpp)
    pm_init();

#if  (NRF_SD_BLE_API_VERSION <= 2)
    ble_gap_addr_t addr;
    if (sd_ble_gap_address_get(&addr) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }
    if (sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &addr) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }
#else
    ble_gap_privacy_params_t privacy_params = {0};
    privacy_params.privacy_mode = BLE_GAP_PRIVACY_MODE_OFF;
    pm_privacy_set(&privacy_params);
#endif

    ASSERT_STATUS( softdevice_ble_evt_handler_set(btle_handler));
    ASSERT_STATUS( softdevice_sys_evt_handler_set(sys_evt_dispatch));

    return btle_gap_init();
}

static void btle_handler(ble_evt_t *p_ble_evt)
{
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

    // Forward BLE events to the Peer Manager
    pm_on_ble_evt(p_ble_evt);
#endif

#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
    bleGattcEventHandler(p_ble_evt);
#endif

    nRF5xn               &ble             = nRF5xn::Instance(BLE::DEFAULT_INSTANCE);
    nRF5xGap             &gap             = (nRF5xGap &) ble.getGap();
    nRF5xGattServer      &gattServer      = (nRF5xGattServer &) ble.getGattServer();
    nRF5xSecurityManager &securityManager = (nRF5xSecurityManager &) ble.getSecurityManager();

    /* Custom event handler */
    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED: {
            Gap::Handle_t handle = p_ble_evt->evt.gap_evt.conn_handle;
#if defined(TARGET_MCU_NRF51_16K_S110) || defined(TARGET_MCU_NRF51_32K_S110)
            /* Only peripheral role is supported by S110 */
            Gap::Role_t role = Gap::PERIPHERAL;
#else
            Gap::Role_t role = static_cast<Gap::Role_t>(p_ble_evt->evt.gap_evt.params.connected.role);
#endif
            gap.setConnectionHandle(handle);
            const Gap::ConnectionParams_t *params = reinterpret_cast<Gap::ConnectionParams_t *>(&(p_ble_evt->evt.gap_evt.params.connected.conn_params));
            const ble_gap_addr_t *peer = &p_ble_evt->evt.gap_evt.params.connected.peer_addr;
#if  (NRF_SD_BLE_API_VERSION <= 2)
            const ble_gap_addr_t *own  = &p_ble_evt->evt.gap_evt.params.connected.own_addr;
        
            gap.processConnectionEvent(handle,
                                       role,
                                       static_cast<BLEProtocol::AddressType_t>(peer->addr_type), peer->addr,
                                       static_cast<BLEProtocol::AddressType_t>(own->addr_type),  own->addr,
                                       params);
#else
            Gap::AddressType_t addr_type;
            Gap::Address_t     own_address;
            gap.getAddress(&addr_type, own_address);

            gap.processConnectionEvent(handle,
                                       role,
                                       static_cast<BLEProtocol::AddressType_t>(peer->addr_type), peer->addr,
                                       addr_type,  own_address,
                                       params);
#endif
            break;
        }

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
            nRF5xGattClient& gattClient = ble.getGattClient();
            gattClient.characteristicDescriptorDiscoverer().terminate(handle, BLE_ERROR_INVALID_STATE);
            gattClient.discovery().terminate(handle);
#endif

            gap.processDisconnectionEvent(handle, reason);
            break;
        }

        case BLE_GAP_EVT_PASSKEY_DISPLAY:
            securityManager.processPasskeyDisplayEvent(p_ble_evt->evt.gap_evt.conn_handle, p_ble_evt->evt.gap_evt.params.passkey_display.passkey);
            break;

        case BLE_GAP_EVT_TIMEOUT:
            gap.processTimeoutEvent(static_cast<Gap::TimeoutSource_t>(p_ble_evt->evt.gap_evt.params.timeout.src));
            break;

        case BLE_GATTC_EVT_TIMEOUT:
        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server and Client timeout events.
            // ASSERT_STATUS_RET_VOID (sd_ble_gap_disconnect(m_conn_handle,
            // BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
            break;

        case BLE_GAP_EVT_ADV_REPORT: {
            const ble_gap_evt_adv_report_t *advReport = &p_ble_evt->evt.gap_evt.params.adv_report;
            gap.processAdvertisementReport(advReport->peer_addr.addr,
                                           advReport->rssi,
                                           advReport->scan_rsp,
                                           static_cast<GapAdvertisingParams::AdvertisingType_t>(advReport->type),
                                           advReport->dlen,
                                           advReport->data);
            break;
        }

        default:
            break;
    }

    gattServer.hwCallback(p_ble_evt);
}

/*! @brief      Callback when an error occurs inside the SoftDevice */
void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name)
{
    ASSERT_TRUE(false, (void) 0);
}

/*!
    @brief      Handler for general errors above the SoftDevice layer.
                Typically we can' recover from this so we do a reset.
*/
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name)
{
    ASSERT_STATUS_RET_VOID( error_code );
    NVIC_SystemReset();
}
