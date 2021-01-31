/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "source/PalGapImpl.h"
#include "ble/common/BLERoles.h"
#include "hci_api.h"
#include "dm_api.h"
#include "dm_main.h"
#include "dm_conn.h"

namespace ble {
namespace impl {

namespace {
bool dummy_gap_event_handler(const wsfMsgHdr_t *msg)
{
    return false;
}
}


bool PalGap::is_feature_supported(
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


ble_error_t PalGap::initialize()
{
    for (auto & cb : direct_adv_cb) {
        cb = direct_adv_cb_t();
    }
    return BLE_ERROR_NONE;
}


ble_error_t PalGap::terminate()
{
    for (auto & cb : direct_adv_cb) {
        cb = direct_adv_cb_t();
    }
    return BLE_ERROR_NONE;
}


address_t PalGap::get_device_address()
{
    return address_t(HciGetBdAddr());
}


address_t PalGap::get_random_address()
{
    return device_random_address;
}


ble_error_t PalGap::set_random_address(const address_t &address)
{
    device_random_address = address;
    DmDevSetRandAddr(const_cast<uint8_t *>(address.data()));
    return BLE_ERROR_NONE;
}

#if BLE_ROLE_BROADCASTER
ble_error_t PalGap::set_advertising_parameters(
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

    return update_direct_advertising_parameters(
        DM_ADV_HANDLE_DEFAULT,
        advertising_type.value(),
        peer_address,
        peer_address_type
    );
}


ble_error_t PalGap::set_advertising_data(
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


ble_error_t PalGap::set_scan_response_data(
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


ble_error_t PalGap::advertising_enable(bool enable)
{
    if (enable) {
        // The Cordio stack requires to start direct advertising with
        // the function DmConnAccept instead of the function DmAdvStart.
        // First the algorithm retrieves if direct advertising has been
        // configured and depending on the result use the right function.
        direct_adv_cb_t *direct_adv_cb = get_pending_direct_adv_cb(DM_ADV_HANDLE_DEFAULT);
        if (direct_adv_cb) {
            direct_adv_cb->connection_handle = DmConnAccept(
                DM_CLIENT_ID_APP,
                DM_ADV_HANDLE_DEFAULT,
                direct_adv_cb->low_duty_cycle ? DM_ADV_CONN_DIRECT_LO_DUTY : DM_ADV_CONN_DIRECT,
                direct_adv_cb->low_duty_cycle ? 0 : HCI_ADV_DIRECTED_MAX_DURATION,
                0,
                direct_adv_cb->peer_address_type.value(),
                direct_adv_cb->peer_address.data()
            );
            if (direct_adv_cb->connection_handle == DM_CONN_ID_NONE) {
                return BLE_ERROR_INTERNAL_STACK_FAILURE;
            } else {
                direct_adv_cb->state = direct_adv_cb_t::running;
            }
        } else {
            uint8_t adv_handles[] = {DM_ADV_HANDLE_DEFAULT};
            uint16_t adv_durations[] = { /* infinite */ 0};
            uint8_t max_ea_events[] = {0};
            DmAdvStart(1, adv_handles, adv_durations, max_ea_events);
        }
    } else {
        // Functions to call to stop advertising if connectable direct
        // advertising is used or not. DmConnClose is used if direct
        // advertising is started otherwise use DmAdvStop.
        direct_adv_cb_t *direct_adv_cb = get_running_direct_adv_cb(DM_ADV_HANDLE_DEFAULT);
        if (direct_adv_cb) {
            DmConnClose(
                DM_CLIENT_ID_APP,
                direct_adv_cb->connection_handle,
                HCI_ERR_LOCAL_TERMINATED
            );
            direct_adv_cb->state = direct_adv_cb_t::free;
        } else {
            uint8_t adv_handles[] = {DM_ADV_HANDLE_DEFAULT};
            DmAdvStop(1, adv_handles);
        }
    }

    return BLE_ERROR_NONE;
}
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
ble_error_t PalGap::set_scan_parameters(
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


ble_error_t PalGap::scan_enable(
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
#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_CENTRAL
ble_error_t PalGap::create_connection(
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


ble_error_t PalGap::cancel_connection_creation()
{
    ble_error_t error = BLE_ERROR_OPERATION_NOT_PERMITTED;

    for (uint8_t connection_id = 0; connection_id < DM_CONN_MAX; connection_id++) {
        if (dmConnCb.ccb[connection_id].inUse &&
            dmConnCb.ccb[connection_id].state == DM_CONN_SM_ST_CONNECTING) {
            DmConnClose(
                DM_CLIENT_ID_APP,
                /* connection handle */ connection_id + 1 /* connection IDs start at 1 */,
                /* reason - invalid (use success) */ 0x00
            );
        }
        error = BLE_ERROR_NONE;
    }

    return error;
}
#endif // BLE_ROLE_CENTRAL

#if BLE_FEATURE_WHITELIST
uint8_t PalGap::read_white_list_capacity()
{
    return HciGetWhiteListSize();
}


ble_error_t PalGap::clear_whitelist()
{
    DmDevWhiteListClear();
    return BLE_ERROR_NONE;
}


ble_error_t PalGap::add_device_to_whitelist(
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


ble_error_t PalGap::remove_device_from_whitelist(
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
#endif // BLE_FEATURE_WHITELIST

#if BLE_FEATURE_CONNECTABLE
ble_error_t PalGap::connection_parameters_update(
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


ble_error_t PalGap::accept_connection_parameter_request(
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


ble_error_t PalGap::reject_connection_parameter_request(
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


ble_error_t PalGap::disconnect(
    connection_handle_t connection,
    local_disconnection_reason_t disconnection_reason
)
{
    DmConnClose(
        DM_CLIENT_ID_APP,
        connection,
        disconnection_reason.value()
    );
    return BLE_ERROR_NONE;
}

#endif // BLE_FEATURE_CONNECTABLE

#if BLE_FEATURE_PHY_MANAGEMENT
ble_error_t PalGap::read_phy(connection_handle_t connection)
{
    if (is_feature_supported(controller_supported_features_t::LE_2M_PHY)
        || is_feature_supported(controller_supported_features_t::LE_CODED_PHY)) {
        DmReadPhy(connection);
        return BLE_ERROR_NONE;
    }
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t PalGap::set_preferred_phys(
    const phy_set_t &tx_phys,
    const phy_set_t &rx_phys
)
{
    DmSetDefaultPhy(
        create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value()
    );

    return BLE_ERROR_NONE;
}


ble_error_t PalGap::set_phy(
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
        create_all_phys_value(tx_phys, rx_phys),
        tx_phys.value(),
        rx_phys.value(),
        coded_symbol.value()
    );

    return BLE_ERROR_NONE;
}
#endif // BLE_FEATURE_PHY_MANAGEMENT

// singleton of the ARM Cordio client

PalGap &PalGap::get_gap()
{
    static PalGap _gap;
    return _gap;
}

/**
 * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
 */

void PalGap::gap_handler(const wsfMsgHdr_t *msg)
{
    typedef bool (*event_handler_t)(const wsfMsgHdr_t *msg);

    if (msg == nullptr) {
        return;
    }

    connection_handle_t handle = (connection_handle_t) msg->param;
    ble::PalGapEventHandler *handler = get_gap()._pal_event_handler;


    switch (msg->event) {
#if BLE_FEATURE_PHY_MANAGEMENT
        case DM_PHY_READ_IND: {
            if (!handler) {
                break;
            }
            const auto *evt = (const hciLeReadPhyCmdCmplEvt_t *) msg;

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

            const auto *evt = (const hciLePhyUpdateEvt_t *) msg;

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
#if BLE_ROLE_OBSERVER
        case DM_PER_ADV_SYNC_EST_IND: {
            if (!handler) {
                break;
            }

            const auto *evt = (const hciLePerAdvSyncEstEvt_t *) msg;

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

            const auto *evt = (const hciLePerAdvReportEvt_t *) msg;

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

            const auto *evt = (const hciLePerAdvSyncLostEvt_t *) msg;
            handler->on_periodic_advertising_sync_loss(evt->syncHandle);
        }
            break;
#endif // BLE_ROLE_OBSERVER
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_ROLE_BROADCASTER
        case DM_ADV_START_IND:
            if (!handler) {
                break;
            }
            handler->on_legacy_advertising_started();
            break;

        case DM_ADV_STOP_IND:
            if (!handler) {
                break;
            }
            handler->on_legacy_advertising_stopped();
            break;
#endif // BLE_ROLE_BROADCASTER

#if BLE_FEATURE_EXTENDED_ADVERTISING && BLE_ROLE_BROADCASTER
        case DM_SCAN_REQ_RCVD_IND: {
            if (!handler) {
                break;
            }

            const auto *evt = (const hciLeScanReqRcvdEvt_t *) msg;
            handler->on_scan_request_received(
                evt->advHandle,
                connection_peer_address_type_t(evt->scanAddrType),
                evt->scanAddr
            );
        }   break;

        case DM_ADV_SET_START_IND: {
            if (!handler) {
                break;
            }
            const auto *evt = (const dmAdvSetStartEvt_t *) msg;
            handler->on_advertising_set_started({evt->advHandle, evt->numSets});
        }   break;


        case DM_ADV_SET_STOP_IND: {
            const auto *evt = (const hciLeAdvSetTermEvt_t *) msg;

            // cleanup state in direct advertising list. This event is only
            // called for set using extended advertsing when the advertising
            // module is reset.
            direct_adv_cb_t *adv_cb = get_gap().get_running_direct_adv_cb(evt->advHandle);

            if (adv_cb) {
                adv_cb->state = direct_adv_cb_t::free;
            }

            if (!handler) {
                break;
            }

            handler->on_advertising_set_terminated(
                hci_error_code_t(evt->status),
                evt->advHandle,
                evt->handle,
                evt->numComplEvts
            );
        }   break;
#endif //  BLE_FEATURE_EXTENDED_ADVERTISING && BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
        case DM_SCAN_START_IND:
            if (!handler) { break; }
            handler->on_scan_started(msg->status == HCI_SUCCESS);
            break;

        case DM_SCAN_STOP_IND:
            if (!handler) { break; }
            handler->on_scan_stopped(msg->status == HCI_SUCCESS);
            break;

#if BLE_FEATURE_EXTENDED_ADVERTISING
        case DM_EXT_SCAN_START_IND:
            if (!handler) { break; }
            handler->on_scan_started(msg->status == HCI_SUCCESS);
            break;

        case DM_EXT_SCAN_STOP_IND: {
            if (!handler) {
                get_gap().ext_scan_stopping = false;
                break;
            }

            if (get_gap().ext_scan_stopping) {
                get_gap().ext_scan_stopping = false;
                handler->on_scan_stopped(msg->status == HCI_SUCCESS);
            } else {
                handler->on_scan_timeout();
            }

        }   break;

        case DM_EXT_SCAN_REPORT_IND: {
            if (!handler) {
                break;
            }

            const auto *evt = (const hciLeExtAdvReportEvt_t *) msg;
            connection_peer_address_type_t addr_type(evt->addrType);
            phy_t sec_phy(evt->secPhy);

            handler->on_extended_advertising_report(
                advertising_event_t(evt->eventType),
                (evt->addrType == HCI_ADDR_TYPE_ANONYMOUS) ? nullptr : &addr_type,
                evt->addr,
                phy_t(evt->priPhy),
                evt->secPhy == HCI_ADV_RPT_PHY_SEC_NONE ? nullptr : &sec_phy,
                evt->advSid,
                evt->txPower,
                evt->rssi,
                evt->perAdvInter,
                direct_address_type_t(evt->directAddrType),
                evt->directAddr,
                evt->len,
                evt->pData
            );
        }   break;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING
#endif // BLE_ROLE_OBSERVER

#if BLE_FEATURE_CONNECTABLE
        case DM_REM_CONN_PARAM_REQ_IND: {
            if (!handler) {
                break;
            }

            const auto *evt = (const hciLeRemConnParamReqEvt_t *) msg;
            handler->on_remote_connection_parameter(
                evt->hdr.param,
                evt->intervalMin,
                evt->intervalMax,
                evt->latency,
                evt->timeout
            );
        }
            break;

#if BLE_ROLE_PERIPHERAL
        case DM_CONN_CLOSE_IND: {
            // Intercept connection close indication received when direct  advertising timeout.
            // Leave the rest of the processing to the event handlers bellow.
            const auto *evt = (const hciDisconnectCmplEvt_t *) msg;
            if (evt->status == HCI_ERR_ADV_TIMEOUT) {
                direct_adv_cb_t *adv_cb =
                    get_gap().get_running_conn_direct_adv_cb(evt->hdr.param);
                if (adv_cb) {
                    adv_cb->state = direct_adv_cb_t::free;
                    if (handler) {
#if BLE_FEATURE_EXTENDED_ADVERTISING
                        if (get_gap().is_feature_supported(controller_supported_features_t::LE_EXTENDED_ADVERTISING)) {
                            handler->on_advertising_set_terminated(
                                hci_error_code_t(evt->status),
                                adv_cb->advertising_handle,
                                DM_CONN_ID_NONE,
                                0
                            );
                        } else
#endif
                        {
                            handler->on_legacy_advertising_stopped();
                        }
                    }
                }
            }
        }
            break;

        case DM_CONN_OPEN_IND: {
            // Intercept connection open indication received when direct advertising timeout.
            // Leave the rest of the processing to the event handlers bellow.
            // There is no advertising stop event generated for directed connectable advertising.
            const auto *evt = (const hciLeConnCmplEvt_t *) msg;
            direct_adv_cb_t *adv_cb = get_gap().get_running_conn_direct_adv_cb(evt->hdr.param);
            if (adv_cb) {
                adv_cb->state = direct_adv_cb_t::free;
            }
        }
            break;
#endif // BLE_ROLE_PERIPHERAL
#endif // BLE_FEATURE_CONNECTABLE
    }

    // all handlers are stored in a static array
    static const event_handler_t handlers[] = {
#if BLE_ROLE_OBSERVER
        &event_handler<GapAdvertisingReportMessageConverter>,
#endif // BLE_ROLE_OBSERVER
#if BLE_FEATURE_CONNECTABLE
        &event_handler<ConnectionCompleteMessageConverter>,
        &event_handler<DisconnectionMessageConverter>,
        &event_handler<ConnectionUpdateMessageConverter>,
        &event_handler<RemoteConnectionParameterRequestMessageConverter>,
#endif // BLE_FEATURE_CONNECTABLE
        &dummy_gap_event_handler
    };

    // event->hdr.param: connection handle

    // traverse all handlers and execute them with the event in input.
    // exit if an handler has handled the event.
    for (auto handler : handlers) {
        if (handler(msg)) {
            return;
        }
    }
}

/**
 * T shall define a can_convert and convert function and a type
 */

template<typename T>
bool PalGap::event_handler(const wsfMsgHdr_t *msg)
{
    if (T::can_convert(msg)) {
        get_gap().emit_gap_event(T::convert((const typename T::type *) msg));
        return true;
    }
    return false;
}

#if BLE_ROLE_BROADCASTER
#if BLE_FEATURE_EXTENDED_ADVERTISING
ble_error_t PalGap::set_advertising_set_random_address(
    advertising_handle_t advertising_handle,
    const address_t &address
)
{
    DmAdvSetRandAddr(advertising_handle, address.data());
    return BLE_ERROR_NONE;
}

ble_error_t PalGap::set_extended_advertising_parameters(
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
            } else {
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
            } else {
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

    return update_direct_advertising_parameters(
        advertising_handle,
        adv_type,
        peer_address,
        peer_address_type
    );
}
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

#if BLE_FEATURE_PERIODIC_ADVERTISING
ble_error_t PalGap::set_periodic_advertising_parameters(
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
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_FEATURE_EXTENDED_ADVERTISING
ble_error_t PalGap::set_extended_advertising_data(
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
#endif // #if BLE_FEATURE_EXTENDED_ADVERTISING

#if BLE_FEATURE_PERIODIC_ADVERTISING
ble_error_t PalGap::set_periodic_advertising_data(
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
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_FEATURE_EXTENDED_ADVERTISING
ble_error_t PalGap::set_extended_scan_response_data(
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


ble_error_t PalGap::extended_advertising_enable(
    bool enable,
    uint8_t number_of_sets,
    const advertising_handle_t *in_handles,
    const uint16_t *in_durations,
    const uint8_t *in_max_extended_advertising_events
)
{
    if (number_of_sets > DM_NUM_ADV_SETS) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (enable) {
        advertising_handle_t handles[DM_NUM_ADV_SETS];
        uint8_t max_extended_advertising_events[DM_NUM_ADV_SETS];
        uint16_t durations_ms[DM_NUM_ADV_SETS];
        uint8_t non_direct_set_count = 0;

        // Advertising sets broadcasting direct connectable advertisment can't be
        // started with DmAdvStart. Therefore we first start all the advertising
        // sets broadcasting with direct advertising then pass the remaining
        // advertising sets to DmAdvStart.
        for (size_t i = 0; i < number_of_sets; ++i) {
            uint32_t duration = in_durations[i] * 10;
            duration = duration > 0xFFFF ? 0xFFFF : duration;

            direct_adv_cb_t *direct_adv_cb = get_pending_direct_adv_cb(in_handles[i]);
            if (direct_adv_cb) {
                direct_adv_cb->connection_handle = DmConnAccept(
                    DM_CLIENT_ID_APP,
                    in_handles[i],
                    direct_adv_cb->low_duty_cycle ?
                        DM_ADV_CONN_DIRECT_LO_DUTY : DM_ADV_CONN_DIRECT,
                    direct_adv_cb->low_duty_cycle ? duration : HCI_ADV_DIRECTED_MAX_DURATION,
                    0,
                    direct_adv_cb->peer_address_type.value(),
                    direct_adv_cb->peer_address.data()
                );
                if (direct_adv_cb->connection_handle == DM_CONN_ID_NONE) {
                    return BLE_ERROR_INTERNAL_STACK_FAILURE;
                } else {
                    direct_adv_cb->state = direct_adv_cb_t::running;
                }
            } else {
                handles[non_direct_set_count] = in_handles[i];
                max_extended_advertising_events[non_direct_set_count] = in_max_extended_advertising_events[i];
                durations_ms[non_direct_set_count] = duration;
                ++non_direct_set_count;
            }
        }

        DmAdvStart(
            non_direct_set_count,
            const_cast<uint8_t *>(handles),
            durations_ms,
            const_cast<uint8_t *>(max_extended_advertising_events)
        );
    } else {
        // reconstruct list for non directed advertising
        advertising_handle_t handles[DM_NUM_ADV_SETS];
        uint8_t non_direct_set_count = 0;

        // Advertising sets broadcasting direct connectable advertisment can't be
        // stopped with DmAdvStop. Therefore we first stop all the advertising
        // sets broadcasting with direct advertising with DmConnClose then pass
        // the remaining advertising sets to DmAdvStop.
        for (size_t i = 0; i < number_of_sets; ++i) {
            direct_adv_cb_t *direct_adv_cb = get_running_direct_adv_cb(in_handles[i]);
            if (direct_adv_cb) {
                DmConnClose(
                    DM_CLIENT_ID_APP,
                    direct_adv_cb->connection_handle,
                    HCI_ERR_LOCAL_TERMINATED
                );
                direct_adv_cb->state = direct_adv_cb_t::free;
            } else {
                handles[non_direct_set_count] = in_handles[i];
                ++non_direct_set_count;
            }
        }

        DmAdvStop(
            non_direct_set_count,
            const_cast<uint8_t *>(handles)
        );
    }

    return BLE_ERROR_NONE;
}
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

#if BLE_FEATURE_PERIODIC_ADVERTISING
ble_error_t PalGap::periodic_advertising_enable(
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
#endif

uint16_t PalGap::get_maximum_advertising_data_length()
{
#if BLE_FEATURE_EXTENDED_ADVERTISING
    return HciGetMaxAdvDataLen();
#else
    return HCI_ADV_DATA_LEN;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING
}


uint16_t PalGap::get_maximum_connectable_advertising_data_length()
{
    return HCI_EXT_ADV_CONN_DATA_LEN;
}


uint8_t PalGap::get_maximum_hci_advertising_data_length()
{
    return HCI_EXT_ADV_DATA_LEN;
}


uint8_t PalGap::get_max_number_of_advertising_sets()
{
    return std::min(HciGetNumSupAdvSets(), (uint8_t) DM_NUM_ADV_SETS);
}

#if BLE_FEATURE_EXTENDED_ADVERTISING
ble_error_t PalGap::remove_advertising_set(advertising_handle_t advertising_handle)
{
    DmAdvRemoveAdvSet(advertising_handle);
    return BLE_ERROR_NONE;
}


ble_error_t PalGap::clear_advertising_sets()
{
    DmAdvClearAdvSets();
    return BLE_ERROR_NONE;
}
#endif // BLE_FEATURE_EXTENDED_ADVERTISING
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
#if BLE_FEATURE_EXTENDED_ADVERTISING
ble_error_t PalGap::set_extended_scan_parameters(
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


ble_error_t PalGap::extended_scan_enable(
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
        ext_scan_stopping = true;
    }

    return BLE_ERROR_NONE;
}
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

#if BLE_FEATURE_PERIODIC_ADVERTISING
ble_error_t PalGap::periodic_advertising_create_sync(
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


ble_error_t PalGap::cancel_periodic_advertising_create_sync()
{
    // FIXME: Find a way to use it!
    // Function not directly exposed by the cordio stack.
    return BLE_ERROR_NOT_IMPLEMENTED;
}


ble_error_t PalGap::periodic_advertising_terminate_sync(sync_handle_t sync_handle)
{
    DmSyncStop(sync_handle);
    return BLE_ERROR_NONE;
}


ble_error_t PalGap::add_device_to_periodic_advertiser_list(
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


ble_error_t PalGap::remove_device_from_periodic_advertiser_list(
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


ble_error_t PalGap::clear_periodic_advertiser_list()
{
    DmClearPerAdvList();
    return BLE_ERROR_NONE;
}


uint8_t PalGap::read_periodic_advertiser_list_size()
{
    return HciGetPerAdvListSize();
}
#endif // BLE_FEATURE_PERIODIC_ADVERTISING
#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_CENTRAL && BLE_FEATURE_EXTENDED_ADVERTISING
ble_error_t PalGap::extended_create_connection(
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
#endif // BLE_ROLE_CENTRAL && BLE_FEATURE_EXTENDED_ADVERTISING


ble_error_t PalGap::update_direct_advertising_parameters(
    advertising_handle_t advertising_handle,
    uint8_t advertising_type,
    address_t peer_address,
    advertising_peer_address_type_t peer_address_type
)
{
    // The case where a direct advertising is running and parameters are updated
    // is considered to be a programming error. User should stop advertising first.
    direct_adv_cb_t *running = get_running_direct_adv_cb(advertising_handle);
    if (running) {
        return BLE_ERROR_INVALID_STATE;
    }

    // For pending direct advertising, update the configuration data structure
    direct_adv_cb_t *pending = get_pending_direct_adv_cb(DM_ADV_HANDLE_DEFAULT);
    if (pending) {
        // Update existing config
        if (advertising_type == DM_ADV_CONN_DIRECT ||
            advertising_type == DM_ADV_CONN_DIRECT_LO_DUTY) {
            pending->peer_address_type = peer_address_type;
            pending->peer_address = peer_address;
            pending->low_duty_cycle =
                advertising_type == DM_ADV_CONN_DIRECT_LO_DUTY;
        } else {
            // set the advertising cb state to idle
            pending->state = direct_adv_cb_t::free;
        }
        return BLE_ERROR_NONE;
    }

    // If this is the first configuration of direct advertising, acquire a cb
    // then configure it.
    if (advertising_type == DM_ADV_CONN_DIRECT ||
        advertising_type == DM_ADV_CONN_DIRECT_LO_DUTY) {
        direct_adv_cb_t *adv_cb = get_free_direct_adv_cb();
        if (!adv_cb) {
            return BLE_ERROR_INTERNAL_STACK_FAILURE;
        }
        adv_cb->state = direct_adv_cb_t::pending;
        adv_cb->peer_address_type = peer_address_type;
        adv_cb->peer_address = peer_address;
        adv_cb->low_duty_cycle =
            advertising_type == DM_ADV_CONN_DIRECT_LO_DUTY;
    }

    return BLE_ERROR_NONE;
}


template<class Predicate>
typename PalGap::direct_adv_cb_t *
PalGap::get_adv_cb(const Predicate &predicate)
{
    for (size_t i = 0; i < DM_NUM_ADV_SETS; ++i) {
        if (predicate(direct_adv_cb[i])) {
            return direct_adv_cb + i;
        }
    }
    return nullptr;
}


typename PalGap::direct_adv_cb_t *
PalGap::get_running_direct_adv_cb(advertising_handle_t adv_handle)
{
    return get_adv_cb(
        [adv_handle](const direct_adv_cb_t &cb) {
            return cb.state == direct_adv_cb_t::running &&
                cb.advertising_handle == adv_handle;
        }
    );
}


typename PalGap::direct_adv_cb_t *
PalGap::get_running_conn_direct_adv_cb(connection_handle_t conn_handle)
{
    return get_adv_cb(
        [conn_handle](const direct_adv_cb_t &cb) {
            return cb.state == direct_adv_cb_t::running &&
                cb.connection_handle == conn_handle;
        }
    );
}


typename PalGap::direct_adv_cb_t *
PalGap::get_pending_direct_adv_cb(advertising_handle_t adv_handle)
{
    return get_adv_cb(
        [adv_handle](const direct_adv_cb_t &cb) {
            return cb.state == direct_adv_cb_t::pending &&
                cb.advertising_handle == adv_handle;
        }
    );
}


typename PalGap::direct_adv_cb_t *
PalGap::get_free_direct_adv_cb()
{
    return get_adv_cb(
        [](const direct_adv_cb_t &cb) {
            return cb.state == direct_adv_cb_t::free;
        }
    );
}

void PalGap::when_gap_event_received(mbed::Callback<void(const GapEvent &)> cb)
{
    _gap_event_cb = cb;
}

void PalGap::set_event_handler(PalGapEventHandler *event_handler)
{
    _pal_event_handler = event_handler;
}

PalGapEventHandler *PalGap::get_event_handler()
{
    return _pal_event_handler;
}

} // namespace impl
} // ble
