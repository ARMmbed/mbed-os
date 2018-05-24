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
    virtual ble_error_t initialize() {
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t terminate() {
        return BLE_ERROR_NONE;
    }

    virtual address_t get_device_address() {
        return address_t(HciGetBdAddr());
    }

    virtual address_t get_random_address() {
        return device_random_address;
    }

    virtual ble_error_t set_random_address(const address_t& address) {
        device_random_address = address;
        DmDevSetRandAddr(const_cast<uint8_t*>(address.data()));
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t set_advertising_parameters(
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

    virtual ble_error_t set_advertising_data(
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

    virtual ble_error_t set_scan_response_data(
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

    virtual ble_error_t advertising_enable(bool enable) {
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

    virtual ble_error_t set_scan_parameters(
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

    virtual ble_error_t scan_enable(
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

    virtual ble_error_t cancel_connection_creation() {
        DmConnClose(
            DM_CLIENT_ID_APP,
            /* connection handle - invalid */ DM_CONN_ID_NONE,
            /* reason - invalid (use success) */ 0x00
        );
        return BLE_ERROR_NONE;
    }

    virtual uint8_t read_white_list_capacity() {
        return HciGetWhiteListSize();
    }

    virtual ble_error_t clear_whitelist() {
        DmDevWhiteListClear();
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t add_device_to_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    ) {
        DmDevWhiteListAdd(
            address_type.value(),
            const_cast<uint8_t*>(address.data())
        );
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t remove_device_from_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    ) {
        DmDevWhiteListRemove(
            address_type.value(),
            const_cast<uint8_t*>(address.data())
        );
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t connection_parameters_update(
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

    virtual ble_error_t accept_connection_parameter_request(
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

    virtual ble_error_t reject_connection_parameter_request(
        connection_handle_t connection_handle,
        hci_error_code_t rejection_reason
    ) {
        DmRemoteConnParamReqNegReply(
            connection_handle,
            rejection_reason.value()
        );
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t disconnect(
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

    virtual bool is_privacy_supported() {
        // We only support controller-based privacy, so return whether the controller supports it
        return HciLlPrivacySupported();
    }
    
    virtual ble_error_t set_address_resolution(
        bool enable
    ) {
        DmPrivSetAddrResEnable(enable);
        return BLE_ERROR_NONE;
    }

    // singleton of the ARM Cordio client
    static Gap& get_gap() {
        static Gap _gap;
        return _gap;
    }

private:
    typedef bool (*event_handler_t)(const wsfMsgHdr_t* msg);

public:
    /**
     * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
     */
    static void gap_handler(const wsfMsgHdr_t* msg) {
        if (msg == NULL) {
            return;
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

private:
    /**
     * T shall define a can_convert and convert function and a type
     */
    template<typename T>
    static bool event_handler(const wsfMsgHdr_t* msg) {
        if (T::can_convert(msg)) {
            get_gap().emit_gap_event(T::convert((const typename T::type*)msg));
            return true;
        }
        return false;
    }

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
};

} // cordio
} // vendor
} // pal
} // ble

#endif /* CORDIO_PAL_GAP_ */
