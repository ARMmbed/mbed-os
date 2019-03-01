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
#include "dm_api.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

namespace {
bool dummy_gap_event_handler(const wsfMsgHdr_t *msg)
{
    return false;
}
}

template<class EventHandler>
bool Gap<EventHandler>::is_feature_supported_(
    ble::controller_supported_features_t feature
)
{
#if !BLE_FEATURE_PHY_MANAGEMENT
    if (feature == ble::controller_supported_features_t::LE_CODED_PHY ||
        feature == ble::controller_supported_features_t::LE_2M_PHY
    ) {
        return false;
    }
#endif

#if !BLE_FEATURE_EXTENDED_ADVERTISING
    if (feature == ble::controller_supported_features_t::LE_EXTENDED_ADVERTISING) {
        return false;
    }
#endif

#if !BLE_FEATURE_PERIODIC_ADVERTISING
    if (feature == ble::controller_supported_features_t::LE_PERIODIC_ADVERTISING) {
        return false;
    }
#endif

#if !BLE_FEATURE_PRIVACY
    if (feature == ble::controller_supported_features_t::LL_PRIVACY) {
        return false;
    }
#endif

    return (HciGetLeSupFeat() & (1 << feature.value()));
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::initialize_()
{
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::terminate_()
{
    return BLE_ERROR_NONE;
}

template<class EventHandler>
address_t Gap<EventHandler>::get_device_address_()
{
    return address_t(HciGetBdAddr());
}

template<class EventHandler>
address_t Gap<EventHandler>::get_random_address_()
{
    return device_random_address;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_random_address_(const address_t &address)
{
    device_random_address = address;
    DmDevSetRandAddr(const_cast<uint8_t *>(address.data()));
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_advertising_parameters_(
    uint16_t advertising_interval_min,
    uint16_t advertising_interval_max,
    advertising_type_t advertising_type,
    own_address_type_t own_address_type,
    advertising_peer_address_type_t peer_address_type,
    const address_t &peer_address,
    advertising_channel_map_t advertising_channel_map,
    advertising_filter_policy_t advertising_filter_policy
)
{
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

#if BLE_FEATURE_WHITELIST
    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_ADV,
        advertising_filter_policy.value()
    );
#endif // BLE_FEATURE_WHITELIST

    DmAdvConfig(
        DM_ADV_HANDLE_DEFAULT,
        advertising_type.value(),
        peer_address_type.value(),
        const_cast<uint8_t *>(peer_address.data())
    );

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_advertising_data_(
    uint8_t advertising_data_length,
    const advertising_data_t &advertising_data
)
{
    DmAdvSetData(
        DM_ADV_HANDLE_DEFAULT,
        HCI_ADV_DATA_OP_COMP_FRAG,
        DM_DATA_LOC_ADV,
        advertising_data_length,
        const_cast<uint8_t *>(advertising_data.data())
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_scan_response_data_(
    uint8_t scan_response_data_length,
    const advertising_data_t &scan_response_data
)
{
    DmAdvSetData(
        DM_ADV_HANDLE_DEFAULT,
        HCI_ADV_DATA_OP_COMP_FRAG,
        DM_DATA_LOC_SCAN,
        scan_response_data_length,
        const_cast<uint8_t *>(scan_response_data.data())
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::advertising_enable_(bool enable)
{
    if (enable) {
        uint8_t adv_handles[] = {DM_ADV_HANDLE_DEFAULT};
        uint16_t adv_durations[] = { /* infinite */ 0};
        uint8_t max_ea_events[] = {0};
        DmAdvStart(1, adv_handles, adv_durations, max_ea_events);
    } else {
        uint8_t adv_handles[] = {DM_ADV_HANDLE_DEFAULT};
        DmAdvStop(1, adv_handles);
    }
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_scan_parameters_(
    bool active_scanning,
    uint16_t scan_interval,
    uint16_t scan_window,
    own_address_type_t own_address_type,
    scanning_filter_policy_t filter_policy
)
{
    use_active_scanning = active_scanning;
    DmScanSetInterval(HCI_INIT_PHY_LE_1M_BIT, &scan_interval, &scan_window);
    DmScanSetAddrType(own_address_type.value());
#if BLE_FEATURE_WHITELIST
    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_SCAN,
        filter_policy.value()
    );
#endif // BLE_FEATURE_WHITELIST
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::scan_enable_(
    bool enable,
    bool filter_duplicates
)
{
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

template<class EventHandler>
ble_error_t Gap<EventHandler>::create_connection_(
    uint16_t scan_interval,
    uint16_t scan_window,
    initiator_policy_t initiator_policy,
    connection_peer_address_type_t peer_address_type,
    const address_t &peer_address,
    own_address_type_t own_address_type,
    uint16_t connection_interval_min,
    uint16_t connection_interval_max,
    uint16_t connection_latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
)
{
    DmConnSetScanInterval(scan_interval, scan_window);
#if BLE_FEATURE_WHITELIST
    DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, initiator_policy.value());
#endif // BLE_FEATURE_WHITELIST
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
        const_cast<uint8_t *>(peer_address.data())
    );

    if (connection_id == DM_CONN_ID_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::cancel_connection_creation_()
{
    DmConnClose(
        DM_CLIENT_ID_APP,
        /* connection handle - invalid */ DM_CONN_ID_NONE,
        /* reason - invalid (use success) */ 0x00
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
uint8_t Gap<EventHandler>::read_white_list_capacity_()
{
    return HciGetWhiteListSize();
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::clear_whitelist_()
{
    DmDevWhiteListClear();
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::add_device_to_whitelist_(
    whitelist_address_type_t address_type,
    address_t address
)
{
    DmDevWhiteListAdd(
        address_type.value(),
        const_cast<uint8_t *>(address.data())
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::remove_device_from_whitelist_(
    whitelist_address_type_t address_type,
    address_t address
)
{
    DmDevWhiteListRemove(
        address_type.value(),
        const_cast<uint8_t *>(address.data())
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::connection_parameters_update_(
    connection_handle_t connection,
    uint16_t connection_interval_min,
    uint16_t connection_interval_max,
    uint16_t connection_latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
)
{
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

template<class EventHandler>
ble_error_t Gap<EventHandler>::accept_connection_parameter_request_(
    connection_handle_t connection_handle,
    uint16_t interval_min,
    uint16_t interval_max,
    uint16_t latency,
    uint16_t supervision_timeout,
    uint16_t minimum_connection_event_length,
    uint16_t maximum_connection_event_length
)
{
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

template<class EventHandler>
ble_error_t Gap<EventHandler>::reject_connection_parameter_request_(
    connection_handle_t connection_handle,
    hci_error_code_t rejection_reason
)
{
    DmRemoteConnParamReqNegReply(
        connection_handle,
        rejection_reason.value()
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::disconnect_(
    connection_handle_t connection,
    disconnection_reason_t disconnection_reason
)
{
    DmConnClose(
        DM_CLIENT_ID_APP,
        connection,
        disconnection_reason.value()
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
bool Gap<EventHandler>::is_privacy_supported_()
{
    // We only support controller-based privacy, so return whether the controller supports it
    return HciLlPrivacySupported();
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_address_resolution_(
    bool enable
)
{
    DmPrivSetAddrResEnable(enable);
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::read_phy_(connection_handle_t connection)
{
    if (Base::is_feature_supported(controller_supported_features_t::LE_2M_PHY)
        || Base::is_feature_supported(controller_supported_features_t::LE_CODED_PHY)) {
        DmReadPhy(connection);
        return BLE_ERROR_NONE;
    }
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_preferred_phys_(
    const phy_set_t &tx_phys,
    const phy_set_t &rx_phys
)
{
    DmSetDefaultPhy(
        Base::create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value()
    );

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_phy_(
    connection_handle_t connection,
    const phy_set_t &tx_phys,
    const phy_set_t &rx_phys,
    coded_symbol_per_bit_t coded_symbol
)
{
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
        Base::create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value(),
        coded_symbol.value()
    );

    return BLE_ERROR_NONE;
}

// singleton of the ARM Cordio client
template<class EventHandler>
Gap<EventHandler> &Gap<EventHandler>::get_gap()
{
    static Gap _gap;
    return _gap;
}

/**
 * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
 */
template<class EventHandler>
void Gap<EventHandler>::gap_handler(const wsfMsgHdr_t *msg)
{
    typedef bool (*event_handler_t)(const wsfMsgHdr_t *msg);

    if (msg == NULL) {
        return;
    }

    connection_handle_t handle = (connection_handle_t) msg->param;
    EventHandler *handler = get_gap()._pal_event_handler;


    switch (msg->event) {
#if BLE_FEATURE_PHY_MANAGEMENT
        case DM_PHY_READ_IND: {
            if (!handler) {
                break;
            }
            const hciLeReadPhyCmdCmplEvt_t *evt = (const hciLeReadPhyCmdCmplEvt_t *) msg;

            handler->on_read_phy(
                (hci_error_code_t::type) msg->status,
                handle,
                (ble::phy_t::type) evt->txPhy,
                (ble::phy_t::type) evt->rxPhy
            );
        }
        break;

        case DM_PHY_UPDATE_IND: {
            if (!handler) {
                break;
            }

            const hciLePhyUpdateEvt_t *evt = (const hciLePhyUpdateEvt_t *) msg;

            handler->on_phy_update_complete(
                (hci_error_code_t::type) msg->status,
                handle,
                (ble::phy_t::type) evt->txPhy,
                (ble::phy_t::type) evt->rxPhy
            );
        }
        break;
#endif // BLE_FEATURE_PHY_MANAGEMENT
#if BLE_FEATURE_PERIODIC_ADVERTISING
        case DM_PER_ADV_SYNC_EST_IND: {
            if (!handler) {
                break;
            }

            const hciLePerAdvSyncEstEvt_t *evt = (const hciLePerAdvSyncEstEvt_t *) msg;

            handler->on_periodic_advertising_sync_established(
                hci_error_code_t(evt->status),
                evt->syncHandle,
                evt->advSid,
                connection_peer_address_type_t(evt->advAddrType),
                evt->advAddr,
                phy_t(evt->advPhy),
                evt->perAdvInterval,
                clock_accuracy_t(evt->clockAccuracy)
            );
        }
        break;

        case DM_PER_ADV_REPORT_IND: {
            if (!handler) {
                break;
            }

            const hciLePerAdvReportEvt_t *evt = (const hciLePerAdvReportEvt_t *) msg;

            handler->on_periodic_advertising_report(
                evt->syncHandle,
                evt->txPower,
                evt->rssi,
                advertising_data_status_t(evt->status),
                evt->len,
                evt->pData
            );
        }
        break;

        case DM_PER_ADV_SYNC_LOST_IND: {
            if (!handler) {
                break;
            }

            const hciLePerAdvSyncLostEvt_t *evt = (const hciLePerAdvSyncLostEvt_t *) msg;
            handler->on_periodic_advertising_sync_loss(evt->syncHandle);
        }
        break;
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_FEATURE_EXTENDED_ADVERTISING && BLE_ROLE_BROADCASTER
        case DM_SCAN_REQ_RCVD_IND: {
            if (!handler) {
                break;
            }

            const hciLeScanReqRcvdEvt_t *evt = (const hciLeScanReqRcvdEvt_t *) msg;
            handler->on_scan_request_received(
                evt->advHandle,
                connection_peer_address_type_t(evt->scanAddrType),
                evt->scanAddr
            );
        }
        break;

        case DM_ADV_SET_STOP_IND: {
            if (!handler) {
                break;
            }

            const hciLeAdvSetTermEvt_t *evt = (const hciLeAdvSetTermEvt_t *) msg;
            handler->on_advertising_set_terminated(
                hci_error_code_t(evt->status),
                evt->advHandle,
                evt->handle,
                evt->numComplEvts
            );
        }
        break;
#endif //  BLE_FEATURE_EXTENDED_ADVERTISING && BLE_ROLE_BROADCASTER

#if BLE_FEATURE_EXTENDED_ADVERTISING && BLE_ROLE_OBSERVER
        case DM_EXT_SCAN_STOP_IND: {
            if (!handler) {
                break;
            }

            //const hciLeScanTimeoutEvt_t *evt = (const hciLeScanTimeoutEvt_t *) msg;
            handler->on_scan_timeout();
        }
        break;

        case DM_EXT_SCAN_REPORT_IND: {
            if (!handler) {
                break;
            }

            const hciLeExtAdvReportEvt_t *evt = (const hciLeExtAdvReportEvt_t *) msg;
            connection_peer_address_type_t addr_type(evt->addrType);
            phy_t sec_phy(evt->secPhy);

            handler->on_extended_advertising_report(
                advertising_event_t(evt->eventType),
                (evt->addrType == HCI_ADDR_TYPE_ANONYMOUS) ? NULL : &addr_type,
                evt->addr,
                phy_t(evt->priPhy),
                evt->secPhy == HCI_ADV_RPT_PHY_SEC_NONE ? NULL : &sec_phy,
                evt->advSid,
                evt->txPower,
                evt->rssi,
                evt->perAdvInter,
                direct_address_type_t(evt->directAddrType),
                evt->directAddr,
                evt->len,
                evt->pData
            );
        }
        break;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING && BLE_ROLE_OBSERVER

#if BLE_ROLE_CENTRAL || BLE_ROLE_PERIPHERAL
        case DM_CONN_UPDATE_IND: {
            if (!handler) {
                break;
            }

            const hciLeConnUpdateCmplEvt_t *evt = (const hciLeConnUpdateCmplEvt_t *) msg;
            handler->on_connection_update_complete(
                (hci_error_code_t::type) evt->status,
                evt->hdr.param,
                evt->connInterval,
                evt->connLatency,
                evt->supTimeout
            );
        }
        break;

        case DM_REM_CONN_PARAM_REQ_IND: {
            if (!handler) {
                break;
            }

            const hciLeRemConnParamReqEvt_t *evt = (const hciLeRemConnParamReqEvt_t *) msg;
            handler->on_remote_connection_parameter(
                evt->hdr.param,
                evt->intervalMin,
                evt->intervalMax,
                evt->latency,
                evt->timeout
            );
        }
        break;
#endif // BLE_ROLE_CENTRAL || BLE_ROLE_PERIPHERAL
    }

    // all handlers are stored in a static array
    static const event_handler_t handlers[] = {
#if BLE_ROLE_OBSERVER
        &event_handler<GapAdvertisingReportMessageConverter>,
#endif // BLE_ROLE_OBSERVER
#if BLE_ROLE_CENTRAL || BLE_ROLE_PERIPHERAL
        &event_handler<ConnectionCompleteMessageConverter>,
        &event_handler<DisconnectionMessageConverter>,
        &event_handler<ConnectionUpdateMessageConverter>,
        &event_handler<RemoteConnectionParameterRequestMessageConverter>,
#endif // BLE_ROLE_CENTRAL || BLE_ROLE_PERIPHERAL
        &dummy_gap_event_handler
    };

    // event->hdr.param: connection handle

    // traverse all handlers and execute them with the event in input.
    // exit if an handler has handled the event.
    for (size_t i = 0; i < (sizeof(handlers) / sizeof(handlers[0])); ++i) {
        if (handlers[i](msg)) {
            return;
        }
    }
}

/**
 * T shall define a can_convert and convert function and a type
 */
template<class EventHandler>
template<typename T>
bool Gap<EventHandler>::event_handler(const wsfMsgHdr_t *msg)
{
    if (T::can_convert(msg)) {
        get_gap().emit_gap_event(T::convert((const typename T::type *) msg));
        return true;
    }
    return false;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_advertising_set_random_address_(
    advertising_handle_t advertising_handle,
    const address_t &address
)
{
    DmAdvSetRandAddr(advertising_handle, address.data());
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_extended_advertising_parameters_(
    advertising_handle_t advertising_handle,
    advertising_event_properties_t event_properties,
    advertising_interval_t primary_advertising_interval_min,
    advertising_interval_t primary_advertising_interval_max,
    advertising_channel_map_t primary_advertising_channel_map,
    own_address_type_t own_address_type,
    advertising_peer_address_type_t peer_address_type,
    const address_t &peer_address,
    advertising_filter_policy_t advertising_filter_policy,
    advertising_power_t advertising_power,
    phy_t primary_advertising_phy,
    uint8_t secondary_advertising_max_skip,
    phy_t secondary_phy,
    uint8_t advertising_sid,
    bool scan_request_notification
)
{
    uint8_t adv_type;

    if (event_properties.use_legacy_pdu) {
        if (event_properties.directed == false) {
            if (event_properties.high_duty_cycle) {
                return BLE_ERROR_INVALID_PARAM;
            }

            if (event_properties.connectable && event_properties.scannable == false) {
                return BLE_ERROR_INVALID_PARAM;
            }

            if (event_properties.connectable && event_properties.scannable) {
                adv_type = DM_ADV_CONN_UNDIRECT;
            } else if (event_properties.scannable) {
                adv_type = DM_ADV_SCAN_UNDIRECT;
            } else  {
                adv_type = DM_ADV_NONCONN_UNDIRECT;
            }
        } else {
            if (event_properties.scannable) {
                return BLE_ERROR_INVALID_PARAM;
            }

            if (event_properties.connectable == false) {
                return BLE_ERROR_INVALID_PARAM;
            }

            if (event_properties.high_duty_cycle) {
                adv_type = DM_ADV_CONN_DIRECT;
            } else {
                adv_type = DM_ADV_CONN_DIRECT_LO_DUTY;
            }
        }
    } else {
        if (event_properties.directed == false) {
            if (event_properties.high_duty_cycle) {
                return BLE_ERROR_INVALID_PARAM;
            }

            if (event_properties.connectable && event_properties.scannable) {
                adv_type = DM_ADV_CONN_UNDIRECT;
            } else if (event_properties.scannable) {
                adv_type = DM_ADV_SCAN_UNDIRECT;
            } else if (event_properties.connectable) {
                adv_type = DM_EXT_ADV_CONN_UNDIRECT;
            } else  {
                adv_type = DM_ADV_NONCONN_UNDIRECT;
            }
        } else {
            // note: not sure how to act with the high duty cycle in scannable
            // and non connectable mode. These cases looks correct from a Bluetooth
            // standpoint

            if (event_properties.connectable && event_properties.scannable) {
                return BLE_ERROR_INVALID_PARAM;
            } else if (event_properties.connectable) {
                if (event_properties.high_duty_cycle) {
                    adv_type = DM_ADV_CONN_DIRECT;
                } else {
                    adv_type = DM_ADV_CONN_DIRECT_LO_DUTY;
                }
            } else if (event_properties.scannable) {
                adv_type = DM_EXT_ADV_SCAN_DIRECT;
            } else {
                adv_type = DM_EXT_ADV_NONCONN_DIRECT;
            }
        }
    }

    DmAdvSetInterval(
        advertising_handle,
        primary_advertising_interval_min,
        primary_advertising_interval_max
    );

    DmAdvSetAddrType(own_address_type.value());

    DmAdvSetChannelMap(
        advertising_handle,
        primary_advertising_channel_map.value()
    );

    DmDevSetExtFilterPolicy(
        advertising_handle,
        DM_FILT_POLICY_MODE_ADV,
        advertising_filter_policy.value()
    );

    DmAdvScanReqNotifEnable(advertising_handle, scan_request_notification);

    DmAdvSetPhyParam(
        advertising_handle,
        primary_advertising_phy.value(),
        secondary_advertising_max_skip,
        secondary_phy.value()
    );

    DmAdvIncTxPwr(
        advertising_handle,
        event_properties.include_tx_power,
        advertising_power
    );

    DmAdvUseLegacyPdu(advertising_handle, event_properties.use_legacy_pdu);
    DmAdvOmitAdvAddr(advertising_handle, event_properties.omit_advertiser_address);

    DmAdvConfig(
        advertising_handle,
        adv_type,
        peer_address_type.value(),
        const_cast<uint8_t *>(peer_address.data())
    );

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_periodic_advertising_parameters_(
    advertising_handle_t advertising_handle,
    periodic_advertising_interval_t periodic_advertising_min,
    periodic_advertising_interval_t periodic_advertising_max,
    bool advertise_power
)
{
    DmPerAdvIncTxPwr(advertising_handle, advertise_power);
    DmPerAdvSetInterval(
        advertising_handle,
        periodic_advertising_min,
        periodic_advertising_max
    );
    DmPerAdvConfig(advertising_handle);

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_extended_advertising_data_(
    advertising_handle_t advertising_handle,
    advertising_fragment_description_t operation,
    bool minimize_fragmentation,
    uint8_t advertising_data_size,
    const uint8_t *advertising_data
)
{
    uint8_t frag_pref = minimize_fragmentation ?
        HCI_ADV_DATA_FRAG_PREF_NO_FRAG :
        HCI_ADV_DATA_FRAG_PREF_FRAG;

    DmAdvSetFragPref(advertising_handle, frag_pref);

    DmAdvSetData(
        advertising_handle,
        operation.value(),
        DM_DATA_LOC_ADV,
        advertising_data_size,
        const_cast<uint8_t *>(advertising_data)
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_periodic_advertising_data_(
    advertising_handle_t advertising_handle,
    advertising_fragment_description_t fragment_description,
    uint8_t advertising_data_size,
    const uint8_t *advertising_data
)
{
    DmPerAdvSetData(
        advertising_handle,
        fragment_description.value(),
        advertising_data_size,
        const_cast<uint8_t *>(advertising_data)
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_extended_scan_response_data_(
    advertising_handle_t advertising_handle,
    advertising_fragment_description_t operation,
    bool minimize_fragmentation,
    uint8_t scan_response_data_size,
    const uint8_t *scan_response_data
)
{
    uint8_t frag_pref = minimize_fragmentation ?
        HCI_ADV_DATA_FRAG_PREF_NO_FRAG :
        HCI_ADV_DATA_FRAG_PREF_FRAG;

    DmAdvSetFragPref(advertising_handle, frag_pref);

    DmAdvSetData(
        advertising_handle,
        operation.value(),
        DM_DATA_LOC_SCAN,
        scan_response_data_size,
        const_cast<uint8_t *>(scan_response_data)
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::extended_advertising_enable_(
    bool enable,
    uint8_t number_of_sets,
    const advertising_handle_t *handles,
    const uint16_t *durations,
    const uint8_t *max_extended_advertising_events
)
{
    if (enable) {
        uint16_t *durations_ms = new uint16_t[number_of_sets];
        for (size_t i = 0; i < number_of_sets; ++i) {
            uint32_t r = durations[i] * 10;
            durations_ms[i] = r > 0xFFFF ? 0xFFFF : r;
        }

        DmAdvStart(
            number_of_sets,
            const_cast<uint8_t *>(handles),
            durations_ms,
            const_cast<uint8_t *>(max_extended_advertising_events)
        );

        delete[] durations_ms;
    } else {
        DmAdvStop(
            number_of_sets,
            const_cast<uint8_t *>(handles)
        );
    }

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::periodic_advertising_enable_(
    bool enable,
    advertising_handle_t advertising_handle
)
{
    if (enable) {
        DmPerAdvStart(advertising_handle);
    } else {
        DmPerAdvStop(advertising_handle);
    }

    return BLE_ERROR_NONE;
}

template<class EventHandler>
uint16_t Gap<EventHandler>::get_maximum_advertising_data_length_()
{
    return HciGetMaxAdvDataLen();
}

template<class EventHandler>
uint16_t Gap<EventHandler>::get_maximum_connectable_advertising_data_length_()
{
    return HCI_EXT_ADV_CONN_DATA_LEN;
}

template<class EventHandler>
uint8_t Gap<EventHandler>::get_maximum_hci_advertising_data_length_()
{
    return HCI_EXT_ADV_DATA_LEN;
}

template<class EventHandler>
uint8_t Gap<EventHandler>::get_max_number_of_advertising_sets_()
{
    return std::min(HciGetNumSupAdvSets(), (uint8_t) DM_NUM_ADV_SETS);
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::remove_advertising_set_(advertising_handle_t advertising_handle)
{
    DmAdvRemoveAdvSet(advertising_handle);
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::clear_advertising_sets_()
{
    DmAdvClearAdvSets();
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::set_extended_scan_parameters_(
    own_address_type_t own_address_type,
    scanning_filter_policy_t filter_policy,
    phy_set_t scanning_phys,
    const bool *active_scanning,
    const uint16_t *scan_interval,
    const uint16_t *scan_window
)
{
    DmScanSetAddrType(own_address_type.value());

    for (size_t i = 0, count = scanning_phys.count(); i < count; ++i) {
        extended_scan_type[i] = active_scanning[i] ?
            DM_SCAN_TYPE_ACTIVE :
            DM_SCAN_TYPE_PASSIVE;
    }

    this->scanning_phys = scanning_phys;

    DmScanSetInterval(
        scanning_phys.value(),
        const_cast<uint16_t *>(scan_interval),
        const_cast<uint16_t *>(scan_window)
    );

#if BLE_FEATURE_WHITELIST
    DmDevSetFilterPolicy(
        DM_FILT_POLICY_MODE_SCAN,
        filter_policy.value()
    );
#endif // BLE_FEATURE_WHITELIST

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::extended_scan_enable_(
    bool enable,
    duplicates_filter_t filter_duplicates,
    uint16_t duration,
    uint16_t period
)
{
    if (enable) {
        uint32_t duration_ms = duration * 10;

        DmScanStart(
            scanning_phys.value(),
            DM_DISC_MODE_NONE,
            extended_scan_type,
            filter_duplicates.value(),
            duration_ms > 0xFFFF ? 0xFFFF : duration_ms,
            period
        );
    } else {
        DmScanStop();
    }

    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::periodic_advertising_create_sync_(
    bool use_periodic_advertiser_list,
    uint8_t advertising_sid,
    peer_address_type_t peer_address_type,
    const address_t &peer_address,
    uint16_t allowed_skip,
    uint16_t sync_timeout
)
{
    DmDevSetExtFilterPolicy(
        DM_ADV_HANDLE_DEFAULT,
        DM_FILT_POLICY_MODE_SYNC,
        use_periodic_advertiser_list ? HCI_FILT_PER_ADV_LIST : HCI_FILT_NONE
    );

    dmSyncId_t sync_id = DmSyncStart(
        advertising_sid,
        peer_address_type.value(),
        peer_address.data(),
        allowed_skip,
        sync_timeout
    );

    if (sync_id == DM_SYNC_ID_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    } else {
        return BLE_ERROR_NONE;
    }
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::cancel_periodic_advertising_create_sync_()
{
    // FIXME: Find a way to use it!
    // Function not directly exposed by the cordio stack.
    return BLE_ERROR_NOT_IMPLEMENTED;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::periodic_advertising_terminate_sync_(sync_handle_t sync_handle)
{
    DmSyncStop(sync_handle);
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::add_device_to_periodic_advertiser_list_(
    advertising_peer_address_type_t advertiser_address_type,
    const address_t &advertiser_address,
    uint8_t advertising_sid
)
{
    DmAddDeviceToPerAdvList(
        advertiser_address_type.value(),
        const_cast<uint8_t *>(advertiser_address.data()),
        advertising_sid
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::remove_device_from_periodic_advertiser_list_(
    advertising_peer_address_type_t advertiser_address_type,
    const address_t &advertiser_address,
    uint8_t advertising_sid
)
{
    DmRemoveDeviceFromPerAdvList(
        advertiser_address_type.value(),
        const_cast<uint8_t *>(advertiser_address.data()),
        advertising_sid
    );
    return BLE_ERROR_NONE;
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::clear_periodic_advertiser_list_()
{
    DmClearPerAdvList();
    return BLE_ERROR_NONE;
}

template<class EventHandler>
uint8_t Gap<EventHandler>::read_periodic_advertiser_list_size_()
{
    return HciGetPerAdvListSize();
}

template<class EventHandler>
ble_error_t Gap<EventHandler>::extended_create_connection_(
    initiator_policy_t initiator_policy,
    own_address_type_t own_address_type,
    peer_address_type_t peer_address_type,
    const address_t &peer_address,
    phy_set_t initiating_phys,
    const uint16_t *scan_intervals,
    const uint16_t *scan_windows,
    const uint16_t *connection_intervals_min,
    const uint16_t *connection_intervals_max,
    const uint16_t *connection_latencies,
    const uint16_t *supervision_timeouts,
    const uint16_t *minimum_connection_event_lengths,
    const uint16_t *maximum_connection_event_lengths
)
{
    DmExtConnSetScanInterval(
        initiating_phys.value(),
        const_cast<uint16_t *>(scan_intervals),
        const_cast<uint16_t *>(scan_windows)
    );
#if BLE_FEATURE_WHITELIST
    DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, initiator_policy.value());
#endif // BLE_FEATURE_WHITELIST
    DmConnSetAddrType(own_address_type.value());

    // At most 3 phys are in used
    hciConnSpec_t conn_specs[3];
    for (size_t i = 0, count = initiating_phys.count(); i < count; ++i) {
        conn_specs[i].connIntervalMin = connection_intervals_min[i];
        conn_specs[i].connIntervalMax = connection_intervals_max[i];
        conn_specs[i].connLatency = connection_latencies[i];
        conn_specs[i].supTimeout = supervision_timeouts[i];
        conn_specs[i].minCeLen = minimum_connection_event_lengths[i];
        conn_specs[i].maxCeLen = maximum_connection_event_lengths[i];
    }

    DmExtConnSetConnSpec(initiating_phys.value(), conn_specs);

    dmConnId_t connection_id = DmConnOpen(
        DM_CLIENT_ID_APP,
        initiating_phys.value(),
        peer_address_type.value(),
        const_cast<uint8_t *>(peer_address.data())
    );

    if (connection_id == DM_CONN_ID_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    return BLE_ERROR_NONE;
}

} // cordio
} // vendor
} // pal
} // ble
