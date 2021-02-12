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

#ifndef IMPL_PAL_GAP_
#define IMPL_PAL_GAP_

#include "source/pal/PalGap.h"
#include "dm_api.h"

namespace ble {
namespace impl {

/**
 * Implementation of ble::PalGap for the Cordio stack.
 */
class PalGap final : public ble::PalGap {
public:
    PalGap() : _pal_event_handler(nullptr), use_active_scanning(false)
    {
    };

    ~PalGap() = default;

    bool is_feature_supported(
        ble::controller_supported_features_t feature
    ) final;

    ble_error_t initialize() final;

    ble_error_t terminate() final;

    address_t get_device_address() final;

    address_t get_random_address() final;

    ble_error_t set_random_address(const address_t &address) final;

#if BLE_ROLE_BROADCASTER
    ble_error_t set_advertising_parameters(
        uint16_t advertising_interval_min,
        uint16_t advertising_interval_max,
        advertising_type_t advertising_type,
        own_address_type_t own_address_type,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_address,
        advertising_channel_map_t advertising_channel_map,
        advertising_filter_policy_t advertising_filter_policy
    ) final;

    ble_error_t set_advertising_data(
        uint8_t advertising_data_length,
        const advertising_data_t &advertising_data
    ) final;

    ble_error_t set_scan_response_data(
        uint8_t scan_response_data_length,
        const advertising_data_t &scan_response_data
    ) final;

    ble_error_t advertising_enable(bool enable) final;
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
    ble_error_t set_scan_parameters(
        bool active_scanning,
        uint16_t scan_interval,
        uint16_t scan_window,
        own_address_type_t own_address_type,
        scanning_filter_policy_t filter_policy
    ) final;

    ble_error_t scan_enable(
        bool enable,
        bool filter_duplicates
    ) final;
#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_CENTRAL
    ble_error_t create_connection(
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
    ) final;

    ble_error_t cancel_connection_creation();
#endif

#if BLE_FEATURE_WHITELIST
    uint8_t read_white_list_capacity() final;

    ble_error_t clear_whitelist() final;

    ble_error_t add_device_to_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    ) final;

    ble_error_t remove_device_from_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    ) final;
#endif // BLE_FEATURE_WHITELIST

#if BLE_FEATURE_CONNECTABLE
    ble_error_t connection_parameters_update(
        connection_handle_t connection,
        uint16_t connection_interval_min,
        uint16_t connection_interval_max,
        uint16_t connection_latency,
        uint16_t supervision_timeout,
        uint16_t minimum_connection_event_length,
        uint16_t maximum_connection_event_length
    ) final;

    ble_error_t accept_connection_parameter_request(
        connection_handle_t connection_handle,
        uint16_t interval_min,
        uint16_t interval_max,
        uint16_t latency,
        uint16_t supervision_timeout,
        uint16_t minimum_connection_event_length,
        uint16_t maximum_connection_event_length
    ) final;

    ble_error_t reject_connection_parameter_request(
        connection_handle_t connection_handle,
        hci_error_code_t rejection_reason
    ) final;

    ble_error_t disconnect(
        connection_handle_t connection,
        local_disconnection_reason_t disconnection_reason
    ) final;
#endif // BLE_FEATURE_CONNECTABLE

#if BLE_FEATURE_PHY_MANAGEMENT
    ble_error_t read_phy(connection_handle_t connection) final;

    ble_error_t set_preferred_phys(
        const phy_set_t &tx_phys,
        const phy_set_t &rx_phys
    ) final;

    ble_error_t set_phy(
        connection_handle_t connection,
        const phy_set_t &tx_phys,
        const phy_set_t &rx_phys,
        coded_symbol_per_bit_t coded_symbol
    ) final;
#endif // BLE_FEATURE_PHY_MANAGEMENT

    // singleton of the ARM Cordio client
    static PalGap &get_gap();

    /**
     * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
     */
    static void gap_handler(const wsfMsgHdr_t *msg);

#if BLE_ROLE_BROADCASTER
#if BLE_FEATURE_EXTENDED_ADVERTISING
    ble_error_t set_advertising_set_random_address(
        advertising_handle_t advertising_handle,
        const address_t &address
    ) final;

    ble_error_t set_extended_advertising_parameters(
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
    ) final;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING


#if BLE_FEATURE_PERIODIC_ADVERTISING
    ble_error_t set_periodic_advertising_parameters(
        advertising_handle_t advertising_handle,
        periodic_advertising_interval_t periodic_advertising_min,
        periodic_advertising_interval_t periodic_advertising_max,
        bool advertise_power
    ) final;
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_FEATURE_EXTENDED_ADVERTISING
    ble_error_t set_extended_advertising_data(
        advertising_handle_t advertising_handle,
        advertising_fragment_description_t operation,
        bool minimize_fragmentation,
        uint8_t advertising_data_size,
        const uint8_t *advertising_data
    ) final;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

#if BLE_FEATURE_PERIODIC_ADVERTISING
    ble_error_t set_periodic_advertising_data(
        advertising_handle_t advertising_handle,
        advertising_fragment_description_t fragment_description,
        uint8_t advertising_data_size,
        const uint8_t *advertising_data
    ) final;
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_FEATURE_EXTENDED_ADVERTISING
    ble_error_t set_extended_scan_response_data(
        advertising_handle_t advertising_handle,
        advertising_fragment_description_t operation,
        bool minimize_fragmentation,
        uint8_t scan_response_data_size,
        const uint8_t *scan_response_data
    ) final;

    ble_error_t extended_advertising_enable(
        bool enable,
        uint8_t number_of_sets,
        const advertising_handle_t *handles,
        const uint16_t *durations,
        const uint8_t *max_extended_advertising_events
    ) final;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

#if BLE_FEATURE_PERIODIC_ADVERTISING
    ble_error_t periodic_advertising_enable(
        bool enable,
        advertising_handle_t advertising_handle
    ) final;
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

    uint16_t get_maximum_advertising_data_length() final;

    uint16_t get_maximum_connectable_advertising_data_length() final;

    uint8_t get_maximum_hci_advertising_data_length() final;

    uint8_t get_max_number_of_advertising_sets() final;

#if BLE_FEATURE_EXTENDED_ADVERTISING
    ble_error_t remove_advertising_set(
        advertising_handle_t advertising_handle
    ) final;

    ble_error_t clear_advertising_sets() final;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING
#endif // BLE_ROLE_BROADCASTER

#if BLE_ROLE_OBSERVER
#if BLE_FEATURE_EXTENDED_ADVERTISING
    ble_error_t set_extended_scan_parameters(
        own_address_type_t own_address_type,
        scanning_filter_policy_t filter_policy,
        phy_set_t scanning_phys,
        const bool *active_scanning,
        const uint16_t *scan_interval,
        const uint16_t *scan_window
    ) final;

    ble_error_t extended_scan_enable(
        bool enable,
        duplicates_filter_t filter_duplicates,
        uint16_t duration,
        uint16_t period
    ) final;
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

#if BLE_FEATURE_PERIODIC_ADVERTISING
    ble_error_t periodic_advertising_create_sync(
        bool use_periodic_advertiser_list,
        uint8_t advertising_sid,
        peer_address_type_t peer_address_type,
        const address_t &peer_address,
        uint16_t allowed_skip,
        uint16_t sync_timeout
    ) final;

    ble_error_t cancel_periodic_advertising_create_sync() final;

    ble_error_t periodic_advertising_terminate_sync(
        sync_handle_t sync_handle
    ) final;

    ble_error_t add_device_to_periodic_advertiser_list(
        advertising_peer_address_type_t advertiser_address_type,
        const address_t &advertiser_address,
        uint8_t advertising_sid
    ) final;

    ble_error_t remove_device_from_periodic_advertiser_list(
        advertising_peer_address_type_t advertiser_address_type,
        const address_t &advertiser_address,
        uint8_t advertising_sid
    ) final;

    ble_error_t clear_periodic_advertiser_list() final;

    uint8_t read_periodic_advertiser_list_size() final;
#endif // BLE_FEATURE_PERIODIC_ADVERTISING
#endif // BLE_ROLE_OBSERVER

#if BLE_ROLE_CENTRAL && BLE_FEATURE_EXTENDED_ADVERTISING
    ble_error_t extended_create_connection(
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
    ) final;
#endif // BLE_ROLE_CENTRAL && BLE_FEATURE_EXTENDED_ADVERTISING

    void when_gap_event_received(mbed::Callback<void(const GapEvent &)> cb) final;

    /**
    * Sets the event handler that us called by the PAL porters to notify the stack of events
    * which will in turn be passed onto the user application when appropriate.
    *
    * @param[in] event_handler the new event handler interface implementation. Memory
    * owned by caller who is responsible for updating this pointer if interface changes.
    */
    void set_event_handler(PalGapEventHandler *event_handler) final;

    PalGapEventHandler *get_event_handler() final;

private:
    /**
     * Called this function whenever the LE subsystem
     * generate a PalGap event.
     *
     * @param gap_event The event to emit to higher layer.
     */
    void emit_gap_event(const GapEvent &gap_event)
    {
        if (_gap_event_cb) {
            _gap_event_cb(gap_event);
        }
    }

    /**
     * T shall define a can_convert and convert function and a type
     */
    template<typename T>
    static bool event_handler(const wsfMsgHdr_t *msg);

    /**
     * Traits defining can_convert for events.
     */
    template<uint8_t EventID>
    struct MessageConverter {
        static bool can_convert(const wsfMsgHdr_t *msg)
        {
            if (msg->event == EventID) {
                return true;
            }
            return false;
        }
    };

    struct ConnectionCompleteMessageConverter final : public MessageConverter<DM_CONN_OPEN_IND> {
        typedef hciLeConnCmplEvt_t type;

        static GapConnectionCompleteEvent convert(const hciLeConnCmplEvt_t *conn_evt)
        {
            return GapConnectionCompleteEvent(
                conn_evt->status,
                // note the usage of the stack handle, not the HCI handle
                conn_evt->hdr.param,
                (connection_role_t::type) conn_evt->role,
                (peer_address_type_t::type) conn_evt->addrType,
                conn_evt->peerAddr,
                conn_evt->connInterval,
                conn_evt->connLatency,
                conn_evt->supTimeout,
                conn_evt->localRpa,
                conn_evt->peerRpa
            );
        }
    };

    struct GapAdvertisingReportMessageConverter final : public MessageConverter<DM_SCAN_REPORT_IND> {
        typedef hciLeAdvReportEvt_t type;

        struct CordioGapAdvertisingReportEvent final : public GapAdvertisingReportEvent {
            CordioGapAdvertisingReportEvent(const advertising_t &advertising) :
                GapAdvertisingReportEvent(), advertising(advertising)
            {
            }

            uint8_t size() const final
            {
                return 1;
            }

            advertising_t operator[](uint8_t i) const final
            {
                return advertising;
            }

            advertising_t advertising;
        };

        static CordioGapAdvertisingReportEvent convert(const hciLeAdvReportEvt_t *scan_report)
        {
            GapAdvertisingReportEvent::advertising_t advertising = {
                (received_advertising_type_t::type) scan_report->eventType,
                (connection_peer_address_type_t::type) scan_report->addrType,
                scan_report->addr,
                make_const_Span(scan_report->pData, scan_report->len),
                scan_report->rssi
            };
            return CordioGapAdvertisingReportEvent(advertising);
        }
    };

    struct DisconnectionMessageConverter final : public MessageConverter<DM_CONN_CLOSE_IND> {
        typedef hciDisconnectCmplEvt_t type;

        static GapDisconnectionCompleteEvent convert(const hciDisconnectCmplEvt_t *disc_evt)
        {
            return GapDisconnectionCompleteEvent(
                disc_evt->status,
                // note the usage of the stack handle, not the HCI handle
                disc_evt->hdr.param,
                disc_evt->reason
            );
        }
    };

    struct ConnectionUpdateMessageConverter final : public MessageConverter<DM_CONN_UPDATE_IND> {
        typedef hciLeConnUpdateCmplEvt_t type;

        static GapConnectionUpdateEvent convert(const hciLeConnUpdateCmplEvt_t *evt)
        {
            return GapConnectionUpdateEvent(
                evt->status,
                evt->hdr.param,
                evt->connInterval,
                evt->connLatency,
                evt->supTimeout
            );
        }
    };

    struct RemoteConnectionParameterRequestMessageConverter final : public MessageConverter<DM_REM_CONN_PARAM_REQ_IND> {
        typedef hciLeRemConnParamReqEvt_t type;

        static GapRemoteConnectionParameterRequestEvent convert(const hciLeRemConnParamReqEvt_t *evt)
        {
            return GapRemoteConnectionParameterRequestEvent(
                evt->hdr.param,
                evt->intervalMin,
                evt->intervalMax,
                evt->latency,
                evt->timeout
            );
        }
    };

    /* Cordio requires the call to DmConnAccept to properly catch direct advertising timeout.
     * This call returns a connection ID and expect the peer address and the peer address type.
     * This PAL following HCI expectations, these information are not present when the higher
     * level calls the function to enable advertising.
     * This data structure caches these informations and use them in case of direct advertising.
     * Similarly, direct advertising timeout doesn't trigger an advertising timeout instead it
     * triggers a disconnection with the status 60. Information on running direct advertising
     * is stored here.
     */
    struct direct_adv_cb_t {
        direct_adv_cb_t() :
            peer_address_type(advertising_peer_address_type_t::PUBLIC),
            state(free)
        {
        }

        address_t peer_address;
        advertising_peer_address_type_t peer_address_type;
        advertising_handle_t advertising_handle;
        connection_handle_t connection_handle;
        bool low_duty_cycle;
        enum {
            free = 0,
            pending,
            running
        } state;
    };

    /* Predicate expect a const reference to a direct_adv_cb_t
     * It must returns true if the criteria are met and false otherwise. */
    template<typename Predicate>
    direct_adv_cb_t *get_adv_cb(const Predicate &predicate);

    direct_adv_cb_t *get_running_direct_adv_cb(advertising_handle_t adv_handle);

    direct_adv_cb_t *get_running_conn_direct_adv_cb(connection_handle_t adv_handle);

    direct_adv_cb_t *get_pending_direct_adv_cb(advertising_handle_t adv_handle);

    direct_adv_cb_t *get_free_direct_adv_cb();

    ble_error_t update_direct_advertising_parameters(
        advertising_handle_t advertising_handle,
        uint8_t advertising_type,
        address_t peer_address,
        advertising_peer_address_type_t peer_address_type
    );

    /**
     * Create an ALL_PHYS parameter used in LE Set PHY Command
     * and LE Set Default PHY Command.
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E - 7.8.49
     */
    static uint8_t create_all_phys_value(
        const phy_set_t &tx_phys,
        const phy_set_t &rx_phys
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
        return all_phys;
    }

private:
#if BLE_FEATURE_PERIODIC_ADVERTISING && BLE_ROLE_OBSERVER
    /* must be static because is needed in a static handler, there can only be one sync in progress */
    static sync_handle_t _pending_periodic_sync_handle;
#endif
    PalGapEventHandler *_pal_event_handler;
    address_t device_random_address;
    bool use_active_scanning;
    uint8_t extended_scan_type[3];
    phy_set_t scanning_phys;
    direct_adv_cb_t direct_adv_cb[DM_NUM_ADV_SETS];
    bool ext_scan_stopping = false;

    /**
     * Callback called when an event is emitted by the LE subsystem.
     */
    mbed::Callback<void(const GapEvent &)> _gap_event_cb;
};

} // namespace impl
} // ble

#endif /* IMPL_PAL_GAP_ */
