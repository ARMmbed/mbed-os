/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

#include "CordioPalGap.h"
#include "hci_api.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

bool Gap::is_feature_supported(
    Gap::ControllerSupportedFeatures_t feature
) {
    return (HciGetLeSupFeat() & (1 << feature.value()));
}

ble_error_t Gap::initialize() {
    return BLE_ERROR_NONE;
}

ble_error_t Gap::terminate() {
    return BLE_ERROR_NONE;
}

address_t Gap::get_device_address() {
    return address_t(HciGetBdAddr());
}

address_t Gap::get_random_address() {
    return device_random_address;
}

ble_error_t Gap::set_random_address(const address_t& address) {
    device_random_address = address;
    DmDevSetRandAddr(const_cast<uint8_t*>(address.data()));
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_advertising_parameters(
    uint16_t advertising_interval_min,
    uint16_t advertising_interval_max,
    advertising_type_t advertising_type,
    own_address_type_t own_address_type,
    advertising_peer_address_type_t peer_address_type,
    const address_t& peer_address,
    advertising_channel_map_t advertising_channel_map,
    advertising_filter_policy_t advertising_filter_policy
) {
    DmAdvSetInterval(
        DM_ADV_HANDLE_DEFAULT,
        advertising_interval_min,
        advertising_interval_max
    );

    DmAdvSetAddrType(own_address_type.value());

    DmAdvSetChannelMap(
        DM_ADV_HANDLE_DEFAULT,
        advertising_channel_map.value()
    );

    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_ADV,
        advertising_filter_policy.value()
    );

    DmAdvConfig(
        DM_ADV_HANDLE_DEFAULT,
        advertising_type.value(),
        peer_address_type.value(),
        const_cast<uint8_t*>(peer_address.data())
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_advertising_data(
    uint8_t advertising_data_length,
    const advertising_data_t& advertising_data
) {
    DmAdvSetData(
        DM_ADV_HANDLE_DEFAULT,
        HCI_ADV_DATA_OP_COMP_FRAG,
        DM_DATA_LOC_ADV,
        advertising_data_length,
        const_cast<uint8_t*>(advertising_data.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_scan_response_data(
    uint8_t scan_response_data_length,
    const advertising_data_t& scan_response_data
) {
    DmAdvSetData(
        DM_ADV_HANDLE_DEFAULT,
        HCI_ADV_DATA_OP_COMP_FRAG,
        DM_DATA_LOC_SCAN,
        scan_response_data_length,
        const_cast<uint8_t*>(scan_response_data.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::advertising_enable(bool enable) {
    if (enable) {
        uint8_t adv_handles[] = { DM_ADV_HANDLE_DEFAULT };
        uint16_t adv_durations[] = { /* infinite */ 0 };
        uint8_t max_ea_events[] = { 0 };
        DmAdvStart(1, adv_handles, adv_durations, max_ea_events);
    } else {
        uint8_t adv_handles[] = { DM_ADV_HANDLE_DEFAULT };
        DmAdvStop(1, adv_handles);
    }
    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_scan_parameters(
    bool active_scanning,
    uint16_t scan_interval,
    uint16_t scan_window,
    own_address_type_t own_address_type,
    scanning_filter_policy_t filter_policy
) {
    use_active_scanning = active_scanning;
    DmScanSetInterval(HCI_INIT_PHY_LE_1M_BIT, &scan_interval, &scan_window);
    DmScanSetAddrType(own_address_type.value());
    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_SCAN,
        filter_policy.value()
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::scan_enable(
    bool enable,
    bool filter_duplicates
) {
    if (enable) {
        uint8_t scanType = use_active_scanning ? DM_SCAN_TYPE_ACTIVE : DM_SCAN_TYPE_PASSIVE;
        DmScanStart(
            HCI_SCAN_PHY_LE_1M_BIT,
            DM_DISC_MODE_NONE,
            &scanType,
            filter_duplicates,
            0,
            0
        );
    } else {
        DmScanStop();
    }
    return BLE_ERROR_NONE;
}

ble_error_t Gap::create_connection(
    uint16_t scan_interval,
    uint16_t scan_window,
    initiator_policy_t initiator_policy,
    connection_peer_address_type_t peer_address_type,
    const address_t& peer_address,
    own_address_type_t own_address_type,
    uint16_t connection_interval_min,
    uint16_t connection_interval_max,
    uint16_t connection_latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
) {
    DmConnSetScanInterval(scan_interval, scan_window);
    DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, initiator_policy.value());
    DmConnSetAddrType(own_address_type.value());

    hciConnSpec_t conn_spec = {
        connection_interval_min,
        connection_interval_max,
        connection_latency,
        supervision_timeout,
        minimum_connection_event_length,
        maximum_connection_event_length
    };
    DmConnSetConnSpec(&conn_spec);

    dmConnId_t connection_id = DmConnOpen(
        DM_CLIENT_ID_APP,
        HCI_INIT_PHY_LE_1M_BIT,
        peer_address_type.value(),
        const_cast<uint8_t*>(peer_address.data())
    );

    if (connection_id == DM_CONN_ID_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    return BLE_ERROR_NONE;
}

ble_error_t Gap::cancel_connection_creation() {
    DmConnClose(
        DM_CLIENT_ID_APP,
        /* connection handle - invalid */ DM_CONN_ID_NONE,
        /* reason - invalid (use success) */ 0x00
    );
    return BLE_ERROR_NONE;
}

uint8_t Gap::read_white_list_capacity() {
    return HciGetWhiteListSize();
}

ble_error_t Gap::clear_whitelist() {
    DmDevWhiteListClear();
    return BLE_ERROR_NONE;
}

ble_error_t Gap::add_device_to_whitelist(
    whitelist_address_type_t address_type,
    address_t address
) {
    DmDevWhiteListAdd(
        address_type.value(),
        const_cast<uint8_t*>(address.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::remove_device_from_whitelist(
    whitelist_address_type_t address_type,
    address_t address
) {
    DmDevWhiteListRemove(
        address_type.value(),
        const_cast<uint8_t*>(address.data())
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::connection_parameters_update(
    connection_handle_t connection,
    uint16_t connection_interval_min,
    uint16_t connection_interval_max,
    uint16_t connection_latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
) {
    if (DmConnCheckIdle(connection) != 0) {
        return BLE_ERROR_INVALID_STATE;
    }

    hciConnSpec_t connection_spec = {
        connection_interval_min,
        connection_interval_max,
        connection_latency,
        supervision_timeout,
        minimum_connection_event_length,
        maximum_connection_event_length
    };
    DmConnUpdate(
        connection,
        &connection_spec
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::accept_connection_parameter_request(
    connection_handle_t connection_handle,
    uint16_t interval_min,
    uint16_t interval_max,
    uint16_t latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
) {
    hciConnSpec_t connection_spec = {
        interval_min,
        interval_max,
        latency,
        supervision_timeout,
        minimum_connection_event_length,
        maximum_connection_event_length
    };
    DmRemoteConnParamReqReply(connection_handle, &connection_spec);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::reject_connection_parameter_request(
    connection_handle_t connection_handle,
    hci_error_code_t rejection_reason
) {
    DmRemoteConnParamReqNegReply(
        connection_handle,
        rejection_reason.value()
    );
    return BLE_ERROR_NONE;
}

ble_error_t Gap::disconnect(
    connection_handle_t connection,
    disconnection_reason_t disconnection_reason
) {
    DmConnClose(
        DM_CLIENT_ID_APP,
        connection,
        disconnection_reason.value()
    );
    return BLE_ERROR_NONE;
}

bool Gap::is_privacy_supported() {
    // We only support controller-based privacy, so return whether the controller supports it
    return HciLlPrivacySupported();
}

ble_error_t Gap::set_address_resolution(
    bool enable
) {
    DmPrivSetAddrResEnable(enable);
    return BLE_ERROR_NONE;
}

ble_error_t Gap::read_phy(connection_handle_t connection) {
    if (is_feature_supported(ControllerSupportedFeatures_t::LE_2M_PHY)
        || is_feature_supported(ControllerSupportedFeatures_t::LE_CODED_PHY)) {
        DmReadPhy(connection);
        return BLE_ERROR_NONE;
    }
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t Gap::set_preferred_phys(
    const phy_set_t& tx_phys,
    const phy_set_t& rx_phys
) {
    DmSetDefaultPhy(
        create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value()
    );

    return BLE_ERROR_NONE;
}

ble_error_t Gap::set_phy(
    connection_handle_t connection,
    const phy_set_t& tx_phys,
    const phy_set_t& rx_phys,
    coded_symbol_per_bit_t coded_symbol
) {
    /* if phy set is empty set corresponding all_phys bit to 1 */
    uint8_t all_phys = 0;
    if (tx_phys.value() == 0) {
        all_phys |= 0x01;
    }
    if (rx_phys.value() == 0) {
        all_phys |= 0x02;
    }

    DmSetPhy(
        connection,
        create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value(),
        coded_symbol.value()
    );

    return BLE_ERROR_NONE;
}

// singleton of the ARM Cordio client
Gap& Gap::get_gap() {
    static Gap _gap;
    return _gap;
}

/**
 * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
 */
void Gap::gap_handler(const wsfMsgHdr_t* msg) {
    typedef bool (*event_handler_t)(const wsfMsgHdr_t* msg);

    if (msg == NULL) {
        return;
    }

    connection_handle_t handle = (connection_handle_t)msg->param;

    switch(msg->event) {
        case DM_PHY_READ_IND:
            if (get_gap()._pal_event_handler) {
                const hciLeReadPhyCmdCmplEvt_t* evt = (const hciLeReadPhyCmdCmplEvt_t*)msg;

                get_gap()._pal_event_handler->on_read_phy(
                    (hci_error_code_t::type)msg->status,
                    handle,
                    (ble::phy_t::type)evt->txPhy,
                    (ble::phy_t::type)evt->rxPhy
                );
            }
            break;
        case DM_PHY_UPDATE_IND:
            if (get_gap()._pal_event_handler) {
                const hciLePhyUpdateEvt_t* evt = (const hciLePhyUpdateEvt_t*)msg;

                get_gap()._pal_event_handler->on_phy_update_complete(
                    (hci_error_code_t::type)msg->status,
                    handle,
                    (ble::phy_t::type)evt->txPhy,
                    (ble::phy_t::type)evt->rxPhy
                );
            }
            break;
    }

    // all handlers are stored in a static array
    static const event_handler_t handlers[] = {
        &event_handler<ConnectionCompleteMessageConverter>,
        &event_handler<GapAdvertisingReportMessageConverter>,
        &event_handler<DisconnectionMessageConverter>,
        &event_handler<ConnectionUpdateMessageConverter>,
        &event_handler<RemoteConnectionParameterRequestMessageConverter>
    };

    // event->hdr.param: connection handle

    // traverse all handlers and execute them with the event in input.
    // exit if an handler has handled the event.
    for(size_t i = 0; i < (sizeof(handlers)/sizeof(handlers[0])); ++i) {
        if (handlers[i](msg)) {
            return;
        }
    }
}

/**
 * T shall define a can_convert and convert function and a type
 */
template<typename T>
bool Gap::event_handler(const wsfMsgHdr_t* msg) {
    if (T::can_convert(msg)) {
        get_gap().emit_gap_event(T::convert((const typename T::type*)msg));
        return true;
    }
    return false;
}


} // cordio
} // vendor
} // pal
} // ble
