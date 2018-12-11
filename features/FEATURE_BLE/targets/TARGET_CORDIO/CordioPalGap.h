#ifndef CORDIO_PAL_GAP_
#define CORDIO_PAL_GAP_

#include "ble/pal/PalGap.h"
#include "dm_api.h"

namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

/**
 * Implementation of ble::pal::Gap for the Cordio stack.
 */
class Gap : public ::ble::pal::Gap {
public:
    virtual bool is_feature_supported(
        ble::controller_supported_features_t feature
    );

    virtual ble_error_t initialize();

    virtual ble_error_t terminate();

    virtual address_t get_device_address();

    virtual address_t get_random_address();

    virtual ble_error_t set_random_address(const address_t& address);

    virtual ble_error_t set_advertising_parameters(
        uint16_t advertising_interval_min,
        uint16_t advertising_interval_max,
        advertising_type_t advertising_type,
        own_address_type_t own_address_type,
        advertising_peer_address_type_t peer_address_type,
        const address_t& peer_address,
        advertising_channel_map_t advertising_channel_map,
        advertising_filter_policy_t advertising_filter_policy
    );

    virtual ble_error_t set_advertising_data(
        uint8_t advertising_data_length,
        const advertising_data_t& advertising_data
    );

    virtual ble_error_t set_scan_response_data(
        uint8_t scan_response_data_length,
        const advertising_data_t& scan_response_data
    );

    virtual ble_error_t advertising_enable(bool enable);

    virtual ble_error_t set_scan_parameters(
        bool active_scanning,
        uint16_t scan_interval,
        uint16_t scan_window,
        own_address_type_t own_address_type,
        scanning_filter_policy_t filter_policy
    );

    virtual ble_error_t scan_enable(
        bool enable,
        bool filter_duplicates
    );

    virtual ble_error_t create_connection(
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
    );

    virtual ble_error_t cancel_connection_creation();

    virtual uint8_t read_white_list_capacity();

    virtual ble_error_t clear_whitelist();

    virtual ble_error_t add_device_to_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    );

    virtual ble_error_t remove_device_from_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    );

    virtual ble_error_t connection_parameters_update(
        connection_handle_t connection,
        uint16_t connection_interval_min,
        uint16_t connection_interval_max,
        uint16_t connection_latency,
        uint16_t supervision_timeout,
        uint16_t minimum_connection_event_length,
        uint16_t maximum_connection_event_length
    );

    virtual ble_error_t accept_connection_parameter_request(
        connection_handle_t connection_handle,
        uint16_t interval_min,
        uint16_t interval_max,
        uint16_t latency,
        uint16_t supervision_timeout,
        uint16_t minimum_connection_event_length,
        uint16_t maximum_connection_event_length
    );

    virtual ble_error_t reject_connection_parameter_request(
        connection_handle_t connection_handle,
        hci_error_code_t rejection_reason
    );

    virtual ble_error_t disconnect(
        connection_handle_t connection,
        disconnection_reason_t disconnection_reason
    );

    virtual bool is_privacy_supported();
    
    virtual ble_error_t set_address_resolution(
        bool enable
    );

    virtual ble_error_t read_phy(connection_handle_t connection);

    virtual ble_error_t set_preferred_phys(
        const phy_set_t& tx_phys,
        const phy_set_t& rx_phys
    );

    virtual ble_error_t set_phy(
        connection_handle_t connection,
        const phy_set_t& tx_phys,
        const phy_set_t& rx_phys,
        coded_symbol_per_bit_t coded_symbol
    );

    // singleton of the ARM Cordio client
    static Gap& get_gap();

    /**
     * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
     */
    static void gap_handler(const wsfMsgHdr_t* msg);

    virtual ble_error_t set_advertising_set_random_address(
        advertising_handle_t advertising_handle,
        const address_t &address
    );

    virtual ble_error_t set_extended_advertising_parameters(
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
    );

    virtual ble_error_t set_periodic_advertising_parameters(
        advertising_handle_t advertising_handle,
        periodic_advertising_interval_t periodic_advertising_min,
        periodic_advertising_interval_t periodic_advertising_max,
        bool advertise_power
    );

    virtual ble_error_t set_extended_advertising_data(
        advertising_handle_t advertising_handle,
        advertising_fragment_description_t operation,
        bool minimize_fragmentation,
        uint8_t advertising_data_size,
        const uint8_t *advertising_data
    );

    virtual ble_error_t set_periodic_advertising_data(
        advertising_handle_t advertising_handle,
        advertising_fragment_description_t fragment_description,
        uint8_t advertising_data_size,
        const uint8_t *advertising_data
    );

    virtual ble_error_t set_extended_scan_response_data(
        advertising_handle_t advertising_handle,
        advertising_fragment_description_t operation,
        bool minimize_fragmentation,
        uint8_t scan_response_data_size,
        const uint8_t *scan_response_data
    );

    virtual ble_error_t extended_advertising_enable(
        bool enable,
        uint8_t number_of_sets,
        const advertising_handle_t *handles,
        const uint16_t *durations,
        const uint8_t *max_extended_advertising_events
    );

    virtual ble_error_t periodic_advertising_enable(
        bool enable,
        advertising_handle_t advertising_handle
    );

    virtual uint16_t get_maximum_advertising_data_length();

    virtual uint16_t get_maximum_connectable_advertising_data_length();

    virtual uint8_t get_max_hci_advertising_data_length();

    virtual uint8_t get_max_number_of_advertising_sets();

    virtual ble_error_t remove_advertising_set(
        advertising_handle_t advertising_handle
    );

    virtual ble_error_t clear_advertising_sets();

    virtual ble_error_t set_extended_scan_parameters(
        own_address_type_t own_address_type,
        scanning_filter_policy_t filter_policy,
        phy_set_t scanning_phys,
        const bool *active_scanning,
        const uint16_t *scan_interval,
        const uint16_t *scan_window
    );

    virtual ble_error_t extended_scan_enable(
        bool enable,
        duplicates_filter_t filter_duplicates,
        uint16_t duration,
        uint16_t period
    );

    virtual ble_error_t periodic_advertising_create_sync(
        bool use_periodic_advertiser_list,
        uint8_t advertising_sid,
        peer_address_type_t peer_address_type,
        const address_t &peer_address,
        uint16_t allowed_skip,
        uint16_t sync_timeout
    );

    virtual ble_error_t cancel_periodic_advertising_create_sync();

    virtual ble_error_t periodic_advertising_terminate_sync(
        sync_handle_t sync_handle
    );

    virtual ble_error_t add_device_to_periodic_advertiser_list(
        advertising_peer_address_type_t advertiser_address_type,
        const address_t &advertiser_address,
        uint8_t advertising_sid
    );

    virtual ble_error_t remove_device_from_periodic_advertiser_list(
        advertising_peer_address_type_t advertiser_address_type,
        const address_t &advertiser_address,
        uint8_t advertising_sid
    );

    virtual ble_error_t clear_periodic_advertiser_list();

    virtual uint8_t read_periodic_advertiser_list_size();

    virtual ble_error_t extended_create_connection(
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
    );

private:
    /**
     * T shall define a can_convert and convert function and a type
     */
    template<typename T>
    static bool event_handler(const wsfMsgHdr_t* msg);

    /**
     * Traits defining can_convert for events.
     */
    template<uint8_t EventID>
    struct MessageConverter {
        static bool can_convert(const wsfMsgHdr_t* msg) {
            if (msg->event == EventID) {
                return true;
            }
            return false;
        }
    };

    struct ConnectionCompleteMessageConverter : public MessageConverter<DM_CONN_OPEN_IND> {
        typedef hciLeConnCmplEvt_t type;

        static GapConnectionCompleteEvent convert(const hciLeConnCmplEvt_t* conn_evt) {
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

    struct GapAdvertisingReportMessageConverter : public MessageConverter<DM_SCAN_REPORT_IND> {
        typedef hciLeAdvReportEvt_t type;

        struct CordioGapAdvertisingReportEvent : public GapAdvertisingReportEvent {
            CordioGapAdvertisingReportEvent(const advertising_t& advertising) :
                GapAdvertisingReportEvent(), advertising(advertising) {
            }

            virtual ~CordioGapAdvertisingReportEvent() { }

            virtual uint8_t size() const {
                return 1;
            }

            virtual advertising_t operator[](uint8_t i) const {
                return advertising;
            }

            advertising_t advertising;
        };

        static CordioGapAdvertisingReportEvent convert(const hciLeAdvReportEvt_t *scan_report) {
            GapAdvertisingReportEvent::advertising_t advertising = {
                (received_advertising_type_t::type) scan_report->eventType,
                (connection_peer_address_type_t::type) scan_report->addrType,
                scan_report->addr,
                make_const_ArrayView(scan_report->pData, scan_report->len),
                scan_report->rssi
            };
            return CordioGapAdvertisingReportEvent(advertising);
        }
    };

    struct DisconnectionMessageConverter : public MessageConverter<DM_CONN_CLOSE_IND> {
        typedef hciDisconnectCmplEvt_t type;

        static GapDisconnectionCompleteEvent convert(const hciDisconnectCmplEvt_t* disc_evt) {
            return GapDisconnectionCompleteEvent(
                disc_evt->status,
                // note the usage of the stack handle, not the HCI handle
                disc_evt->hdr.param,
                disc_evt->reason
            );
        }
    };

    struct ConnectionUpdateMessageConverter : public MessageConverter<DM_CONN_UPDATE_IND> {
        typedef hciLeConnUpdateCmplEvt_t type;

        static GapConnectionUpdateEvent convert(const hciLeConnUpdateCmplEvt_t* evt) {
            return GapConnectionUpdateEvent(
                evt->status,
                evt->hdr.param,
                evt->connInterval,
                evt->connLatency,
                evt->supTimeout
            );
        }
    };

    struct RemoteConnectionParameterRequestMessageConverter : public MessageConverter<DM_REM_CONN_PARAM_REQ_IND> {
        typedef hciLeRemConnParamReqEvt_t type;

        static GapRemoteConnectionParameterRequestEvent convert(const hciLeRemConnParamReqEvt_t* evt) {
            return GapRemoteConnectionParameterRequestEvent(
                evt->hdr.param,
                evt->intervalMin,
                evt->intervalMax,
                evt->latency,
                evt->timeout
            );
        }
    };

private:
    address_t device_random_address;
    bool use_active_scanning;
    uint8_t extended_scan_type[3];
    phy_set_t scanning_phys;
};

} // cordio
} // vendor
} // pal
} // ble

#endif /* CORDIO_PAL_GAP_ */
