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

#include "ble_flash.h"
#include "ble_conn_params.h"

#include "btle_gap.h"
#include "btle_advertising.h"
#include "custom/custom_helper.h"

#include "ble/GapEvents.h"
#include "nRF5xn.h"

extern "C" {
#include "pstorage.h"
#include "device_manager.h"
#include "softdevice_handler.h"
#include "ble_stack_handler_types.h"
}

#include "ble_hci.h"
#include "btle_discovery.h"

#include "nRF5xGattClient.h"
#include "nRF5xServiceDiscovery.h"
#include "nRF5xCharacteristicDescriptorDiscoverer.h"

bool isEventsSignaled = false;

extern "C" void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name);
void            app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name);

static void btle_handler(ble_evt_t *p_ble_evt);

static void sys_evt_dispatch(uint32_t sys_evt)
{
    pstorage_sys_event_handler(sys_evt);
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
    nrf_clock_lfclksrc_t clockSource;
    if (NRF_CLOCK->LFCLKSRC & (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos)) {
        clockSource = NRF_CLOCK_LFCLKSRC_XTAL_20_PPM;
    } else {
        clockSource = NRF_CLOCK_LFCLKSRC_RC_250_PPM_4000MS_CALIBRATION;
    }
    SOFTDEVICE_HANDLER_INIT(clockSource, signalEvent);

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
    ble_enable_params_t enableParams = {
        .gatts_enable_params = {
            .service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT
        }
    };
    if (sd_ble_enable(&enableParams) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

    ble_gap_addr_t addr;
    if (sd_ble_gap_address_get(&addr) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }
    if (sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &addr) != NRF_SUCCESS) {
        return ERROR_INVALID_PARAM;
    }

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

    dm_ble_evt_handler(p_ble_evt);

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
            const ble_gap_addr_t *own  = &p_ble_evt->evt.gap_evt.params.connected.own_addr;
            gap.processConnectionEvent(handle,
                                                           role,
                                                           static_cast<BLEProtocol::AddressType_t>(peer->addr_type), peer->addr,
                                                           static_cast<BLEProtocol::AddressType_t>(own->addr_type),  own->addr,
                                                           params);
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
                                           advReport->data,
                                           static_cast<BLEProtocol::AddressType_t>(advReport->peer_addr.addr_type));
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
