/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef BLE_GAP_TYPES_H
#define BLE_GAP_TYPES_H

#include "ble/common/Duration.h"
#include "ble/common/Bounded.h"
#include "ble/SafeEnum.h"

namespace ble {

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/* BLE units, using microseconds as the common denominator */

/**
 * Time interval between two advertisements.
 *
 * The duration is in unit of 625µs and ranges from 0x20 to 0xFFFFFF .
 *
 * @note before Bluetooth 5 range was 0x20 to 0xFFFF.
 */
typedef Duration<uint32_t, 625, Range<0x20, 0xFFFFFF> > adv_interval_t;

/**
 * Advertising process duration.
 *
 * The duration is in unit of 10ms and ranges from 0x0001 to 0xFFFF. The special
 * value 0x0000 means the advertising process never ends; it is accessible with
 * adv_duration_t::forever().
 */
typedef Duration<uint16_t, 10000, Range<0x00, 0xFFFF>, /* forever */ Value<uint16_t, 0x0000> > adv_duration_t;

/**
 * Scan process duration.
 *
 * The duration is in unit of 10ms and ranges from 0x0001 to 0xFFFF. The special
 * value 0x0000 means the scan process never ends; it is accessible with
 * scan_duration_t::forever().
 */
typedef Duration<uint16_t, 10000, Range<0x00, 0xFFFF>, /* forever */ Value<uint16_t, 0x0000> > scan_duration_t;

/**
 * Time interval between two scan processes.
 *
 * The duration is in unit of 1.28s and ranges from 0x0001 to 0xFFFF. The special
 * value 0x0000 is used to indicate that scan_period_t is not used.
 */
typedef Duration<uint16_t, 1280000, Range<0x00, 0xFFFF> > scan_period_t;

/**
 * Time interval between two scans.
 *
 * The duration is in unit of 625µs and ranges from 0x04 to 0xFFFF.
 */
typedef Duration<uint16_t, 625, Range<0x04, 0xFFFF> > scan_interval_t;

/**
 * Duration of a scan.
 *
 * The duration is in unit of 625µs and ranges from 0x04 to 0xFFFF.
 */
typedef Duration<uint16_t, 625, Range<0x04, 0xFFFF> > scan_window_t;

/**
 * Time interval between two connection events.
 *
 * The interval is in unit of 1.250 milliseconds and ranges from 0x06 to 0xC80.
 */
typedef Duration<uint16_t, 1250, Range<0x06, 0x0C80> > conn_interval_t;

/**
 * Time after which a connection is loss of devices have not exchanged data.
 *
 * The duration is in unit of 10 milliseconds and ranges from 0x0A to 0xC80.
 *
 * @note this time should be no larger than (1 + ConnLatency) * ConnIntervalMax * 2
 */
typedef Duration<uint16_t, 10000, Range<0x0A, 0x0C80> > supervision_timeout_t;

/**
 * Duration of a connection event.
 *
 * The duration is in unit of 625µs and ranges from 0x0 to 0xFFFF .
 */
typedef Duration<uint16_t, 625, Range<0, 0xFFFF> > conn_event_length_t;

/**
 * Time after which a periodic sync link is considered lost if the receiver hasn't
 * received anything from the advertiser.
 *
 * The duration is in unit of 10 milliseconds and ranges from 0x0A to 0x4000.
 */
typedef Duration<uint16_t, 10000, Range<0x0A, 0x4000> > sync_timeout_t;

/**
 * Interval between two periodic advertising events.
 *
 * The duration is in unit of 1.250ms and ranges from 0x06 to 0xFFFF.
 */
typedef Duration<uint16_t, 1250, Range<0x06, 0xFFFF> > periodic_interval_t;

/**
 * Number of connection events that can be skipped by the slave.
 *
 * It ranges from 0 to 0x1F3.
 */
typedef Bounded<uint16_t, 0, 0x01F3> slave_latency_t;

/**
 * Handle of an advertising set.
 *
 * @note Range of valid handle is comprised between 0x00 and 0xEF.
 */
typedef uint8_t advertising_handle_t;

/**
 * Handle of a sync representing a periodic advertiser.
 *
 * @note Range of valid handle is comprised between 0x0000 and 0xFFFF.
 */
typedef uint16_t periodic_sync_handle_t;

/**
 * Encapsulates the peripheral advertising modes.
 *
 * It determines how the device appears to other scanner and peripheral
 * devices in the scanning range.
 */
struct advertising_type_t : SafeEnum<advertising_type_t, uint8_t> {
    /// enumeration of advertising_type_t values
    enum type {
        /**
         * Device is connectable, scannable and doesn't expect connection from a
         * specific peer.
         * @note Cannot carry extended advertising payload, only legacy PDUs.
         * Use CONNECTABLE_NON_SCANNABLE_UNDIRECTED for non-legacy payload.
         *
         * @see Vol 3, Part C, Section 9.3.4 and Vol 6, Part B, Section 2.3.1.1.
         */
        CONNECTABLE_UNDIRECTED = 0x00,

        /**
         * Device is connectable and expects connection from a specific peer.
         * (3.75 ms or smaller Advertising Interval)
         * @see Vol 3, Part C, Section 9.3.3 and Vol 6, Part B, Section 2.3.1.2.
         */
        CONNECTABLE_DIRECTED = 0x01,

        /**
         * Device is scannable but not connectable.
         *
         * @see Vol 6, Part B, Section 2.3.1.4.
         */
        SCANNABLE_UNDIRECTED = 0x02,

        /**
         * Device is not connectable and not scannable.
         *
         * @see Vol 3, Part C, Section 9.3.2 and Vol 6, Part B, Section 2.3.1.3.
         */
        NON_CONNECTABLE_UNDIRECTED = 0x03,

        /**
         * Device is connectable and expects connection from a specific peer (sent at long user set intervals).
         */
        CONNECTABLE_DIRECTED_LOW_DUTY = 0x04,

        /**
         * Device is connectable, but not scannable and doesn't expect connection from a specific peer.
         * @note Only for use with extended advertising payload, will not allow legacy PDUs
         * (use CONNECTABLE_UNDIRECTED for legacy PDU).
         */
        CONNECTABLE_NON_SCANNABLE_UNDIRECTED = 0x05,

#if !defined(DOXYGEN_ONLY)
        // used by the PAL; naming in line with the the spec.
        ADV_IND = 0x00,
        ADV_DIRECT_IND = 0x01,
        ADV_SCAN_IND = 0x02,
        ADV_NONCONN_IND = 0x03,
        ADV_DIRECT_IND_LOW_DUTY_CYCLE = 0x04,
        ADV_NONSCAN_IND = 0x05
#endif
    };

    /**
     * Construct a new advertising_type_t value.
     *
     * @param value The value of the advertising_type_t created.
     */
    advertising_type_t(type value) : SafeEnum(value)
    {
    }
};


/** Used to indicate if the packet is complete and if it's truncated.
 */
struct advertising_data_status_t : SafeEnum<advertising_data_status_t, uint8_t> {
    /// enumeration of advertising_data_status_t values
    enum type {
        COMPLETE = 0x00, /// Advertising payload complete.
        INCOMPLETE_MORE_DATA = 0x01, /// Partial advertising payload, more to come.
        INCOMPLETE_DATA_TRUNCATED = 0x02 /// Advertising payload incomplete, and no more is coming.
    };

    /**
     * Construct a new advertising_data_status_t value.
     *
     * @param value The value of the advertising_data_status_t created.
     */
    advertising_data_status_t(type value) : SafeEnum(value)
    {
    }

#if !defined(DOXYGEN_ONLY)

    /**
     * Explicit constructor from a raw value.
     */
    explicit advertising_data_status_t(uint8_t raw_value) :
        SafeEnum(raw_value)
    {
    }

#endif

};

/** Properties of an advertising event.
 */
struct advertising_event_t {

private:
    // Implementation note: The object is constructed from the field Event_Type
    // of an LE Extended Advertising.
    // Indexes and functions of bits are defined in BLUETOOTH SPECIFICATION
    // Version 5.0 | Vol 2, Part E -
    // Section 7.7.65.13 LE Extended Advertising report Event
    enum {
        CONNECTABLE_BIT = 0,
        SCANNABLE_ADVERTISING_BIT = 1,
        DIRECTED_ADVERTISING_BIT = 2,
        SCAN_RESPONSE_BIT = 3,
        LEGACY_PDU_BIT = 4,
        DATA_STATUS_INDEX = 5,
        DATA_STATUS_MASK = 0x03
    };

public:

#if !defined(DOXYGEN_ONLY)

    /** Create based on a raw value.
     *
     * @param value
     */
    explicit advertising_event_t(uint8_t value) : value(value)
    {
    }

#endif

    /** Is advertising connectable.
     *
     * @return True if connectable.
     */
    bool connectable() const
    {
        return static_cast<bool>(value & (1 << CONNECTABLE_BIT));
    }

    /** Is advertising scannable.
     *
     * @return True if scannable
     */
    bool scannable_advertising() const
    {
        return static_cast<bool>(value & (1 << SCANNABLE_ADVERTISING_BIT));
    }

    /** Is advertising directed.
     *
     * @return True if directed.
     */
    bool directed_advertising() const
    {
        return static_cast<bool>(value & (1 << DIRECTED_ADVERTISING_BIT));
    }

    /** Is this a scan response.
     *
     * @return True if scan response.
     */
    bool scan_response() const
    {
        return static_cast<bool>(value & (1 << SCAN_RESPONSE_BIT));
    }

    /** Is this legacy advertising.
     *
     * @return True if legacy.
     */
    bool legacy_advertising() const
    {
        return static_cast<bool>(value & (1 << LEGACY_PDU_BIT));
    }

    /** Payload completeness status.
     *
     * @return @see advertising_data_status_t for details.
     */
    advertising_data_status_t data_status() const
    {
        return static_cast<advertising_data_status_t::type>(
            (value >> DATA_STATUS_INDEX) & DATA_STATUS_MASK
        );
    }

    /** Is payload complete.
     *
     * @return True if payload is coplete.
     */
    bool complete() const
    {
        return data_status() == advertising_data_status_t::COMPLETE;
    }

    /** Is there more data coming.
     *
     * @return True if more data coming.
     */
    bool more_data_to_come() const
    {
        return data_status() == advertising_data_status_t::INCOMPLETE_MORE_DATA;
    }

    /** Is the payload truncated.
     *
     * @return True if no more data coming.
     */
    bool truncated() const
    {
        return data_status() == advertising_data_status_t::INCOMPLETE_DATA_TRUNCATED;
    }

private:
    uint8_t value;
};

/**
 * Identify an advertising SID. Range: [0x00, 0x0F]
 */
typedef uint8_t advertising_sid_t;

/** Received signal strength. Range <-127, 20>.
 *  @note Special value 127 means RSSI is not available. */
typedef int8_t rssi_t;

/**
 * Describe the advertising power.
 *
 * Values between -127 and +126 are considered power values in dBm while
 * the special value 127 can be used as a wildcard to indicate that the host
 * has no preference or if the power information is not available.
 */
typedef int8_t advertising_power_t;

/**
 * Advertising policy filter modes.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.2.
 */
struct advertising_filter_policy_t : SafeEnum<advertising_filter_policy_t, uint8_t> {
    /// enumeration of advertising_filter_policy_t values
    enum type {
        /**
         * Process connection and scan requests from all devices. The whitelist is
         * not used.
         */
        NO_FILTER = 0x00,

        /**
         * Process connection requests from all devices but filter out scan requests
         * of devices that are not in the whitelist.
         */
        FILTER_SCAN_REQUESTS = 0x01,

        /**
         * Process scan requests from all devices but filter out connection requests
         * of devices that are not in the whitelist.
         */
        FILTER_CONNECTION_REQUEST = 0x02,

        /**
         * Filter out scan or connection requests of devices that are not in the
         * whitelist.
         */
        FILTER_SCAN_AND_CONNECTION_REQUESTS = 0x03
    };

    /**
     * Construct a advertising_filter_policy_t.
     * @param value The value of the advertising_filter_policy_t created.
     */
    advertising_filter_policy_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Scanning policy filter mode.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.3.
 */
struct scanning_filter_policy_t : SafeEnum<scanning_filter_policy_t, uint8_t> {
    /// enumeration of scanning_filter_policy_t values
    enum type {
        /**
        * Accept all advertising packets except directed advertising packets not
        * addressed to this device.
        */
        NO_FILTER = 0x00,

        /**
        * Accept only advertising packets from devices in the whitelist except
        * directed advertising packets not addressed to this device.
        */
        FILTER_ADVERTISING = 0x01,

        /**
         * Accept all advertising packets except directed advertising packets
         * where the initiator's identity address does not address this device.
         *
         * @note Directed advertising packets where the initiator's address is a
         * resolvable private address that cannot be resolved are also accepted.
         */
        NO_FILTER_INCLUDE_UNRESOLVABLE_DIRECTED = 2,

        /**
         * Accept all advertising packets except:
         * - Advertising packets where the advertiser's
         * identity address is not in the whitelist.
         * - Directed advertising packets where the initiator's identity address
         * does not address this device.
         *
         * @note Directed advertising packets where the initiator's address is a
         * resolvable private address that cannot be resolved are also accepted.
         */
        FILTER_ADVERTISING_INCLUDE_UNRESOLVABLE_DIRECTED = 3
    };

    /**
     * Construct a new instance of scanning_filter_policy_t.
     *
     * @param value The value of the scanning_filter_policy_t created.
     */
    scanning_filter_policy_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Filter policy that you can use during connection initiation.
 */
struct initiator_filter_policy_t : SafeEnum<initiator_filter_policy_t, uint8_t> {
    /// enumeration of initiator_filter_policy_t values.
    enum type {
        /**
        * The whitelist is not used to determine which advertiser to connect to.
        */
        NO_FILTER,

        /**
        * The whitelist is used to determine which advertiser to connect to.
        */
        USE_WHITE_LIST
    };

    /**
     * Construct a initiator_filter_policy_t.
     * @param value The value of the initiator_filter_policy_t.
     */
    initiator_filter_policy_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Scanning policy filter mode.
 *
 * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.3.
 */
struct duplicates_filter_t : SafeEnum<duplicates_filter_t, uint8_t> {
    /// enumeration of duplicates_filter_t values
    enum type {
        /**
         * Disable duplicate filtering.
         */
        DISABLE,

        /**
         * Enable duplicate filtering.
         */
        ENABLE,

        /**
         * Enable duplicate filtering, and reset the cache periodically.
         */
        PERIODIC_RESET
    };

    /**
     * Construct a new duplicates_filter_t value.
     * @param value The value of the duplicates_filter_t created.
     */
    duplicates_filter_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Type used to model the own address used during the following GAP operations:
 * advertising, scanning and initiating
 */
struct own_address_type_t : SafeEnum<own_address_type_t, uint8_t> {
    /// enumeration of own_address_type_t values.
    enum type {
        /**
         * Use the public device address.
         */
        PUBLIC = 0x00,

        /**
         * Use the random device address.
         */
        RANDOM = 0x01,

        /**
         * Generated resolvable private address based on the local IRK from the
         * resolving list. Use the public address if no entry match in the resolving
         * list.
         */
        RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK = 0x02,

        /**
         * Generated resolvable private address based on the local IRK from the
         * resolving list. Use the random address if no entry match in the resolving
         * list.
         */
        RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK = 0x03,
    };

    /**
     * Construct a new instance of own_address_type_t.
     * @param value The value of the own_address_type_t created.
     */
    own_address_type_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Type of an address to connect to.
 *
 * It is used to connect to a device directly with directed advertising.
 */
struct target_peer_address_type_t : SafeEnum<target_peer_address_type_t, uint8_t> {
    /// enumeration of target_peer_address_type_t values.
    enum type {
        PUBLIC = 0x00, /**< Public Device Address or Public Identity Address. */
        RANDOM = 0x01, /**< Random Device Address or Random (static) Identity Address. */
    };

    /**
     * Create a new target_peer_address_type_t.
     * @param value The value of the target_peer_address_type_t created.
     */
    target_peer_address_type_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Accuracy of the master clock.
 */
struct clock_accuracy_t : SafeEnum<clock_accuracy_t, uint8_t> {
    /// enumeration of clock_accuracy_t values.
    enum type {
        /**
         * 500 PPM
         */
        PPM_500 = 0x00,

        /**
         * 250 PPM
         */
        PPM_250 = 0x01,

        /**
         * 150 PPM
         */
        PPM_150 = 0x02,

        /**
         * 100 PPM
         */
        PPM_100 = 0x03,

        /**
         * 75 PPM
         */
        PPM_75 = 0x04,

        /**
         * 50 PPM
         */
        PPM_50 = 0x05,

        /**
         * 30 PPM
         */
        PPM_30 = 0x06,

        /**
         * 20 PPM
         */
        PPM_20 = 0x07
    };

    /** Get clock accuracy.
     *
     * @return Parts per million as a number.
     */
    uint16_t get_ppm()
    {
        switch (value()) {
            case PPM_500:
                return 500;
            case PPM_250:
                return 250;
            case PPM_150:
                return 150;
            case PPM_100:
                return 100;
            case PPM_75:
                return 75;
            case PPM_50:
                return 50;
            case PPM_30:
                return 30;
            case PPM_20:
                return 20;
            default:
                return 0;
        }
    }

    /**
     * Construct a new clock_accuracy_t value.
     * @param value The value of the clock_accuracy_t created.
     */
    clock_accuracy_t(type value) : SafeEnum(value)
    {
    }

#if !defined(DOXYGEN_ONLY)

    /**
     * Construct a new clock_accuracy_t value from a raw value.
     * @param raw_value The value of the clock accuracy.
     */
    explicit clock_accuracy_t(uint8_t raw_value) : SafeEnum(raw_value)
    {
    }

#endif
};

/**
 * Enumeration of GAP roles.
 *
 * @note The BLE API does not express the broadcaster and scanner roles.
 *
 * @attention A device can fulfill different roles concurrently.
 */
struct connection_role_t : SafeEnum<connection_role_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        /**
         * Central Role.
         *
         * The device can scan and initiate connection to peripherals. It
         * acts as the master when a connection is established.
         *
         * @note A central is a scanner.
         */
        CENTRAL = 0x00,

        /**
         * Peripheral Role.
         *
         * The device can advertise, and you can connect it by a central. It
         * acts as a slave when connected.
         *
         * @note A peripheral is a broadcaster.
         */
        PERIPHERAL = 0x01
    };

    /**
     * Construct a new instance of role_t.
     *
     * @param value The value of the role_t created.
     */
    connection_role_t(type value) : SafeEnum(value)
    {
    }

#if !defined(DOXYGEN_ONLY)

    /**
     * Explicit constructor from a raw value.
     * @param raw_value The role.
     */
    explicit connection_role_t(uint8_t raw_value) : SafeEnum(raw_value)
    {
    }

#endif
};

/**
 * Enumeration of disconnection reasons that should be transmited to the peer.
 */
struct local_disconnection_reason_t : SafeEnum<local_disconnection_reason_t, uint8_t> {
    /// enumeration of local_disconnection_reason_t values.
    enum type {
        /**
         * GAP or GATT failed to authenticate the peer.
         */
        AUTHENTICATION_FAILURE = 0x05,

        /**
         * Connection terminated by the user.
         */
        USER_TERMINATION = 0x13,

        /**
         * Connection termination due to low resources.
         */
        LOW_RESOURCES = 0x14,

        /**
         * Connection termination due to power off.
         */
        POWER_OFF = 0x15,

        /**
         * Remote feature not supported.
         */
        UNSUPPORTED_REMOTE_FEATURE = 0x1A,

        /**
         * Not possible to pai with a unit key.
         */
        PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29,

        /**
         * Connection parameters were unacceptable.
         */
        UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B
    };

    /**
     * Construct a new instance of disconnection_reason_t.
     *
     * @param value The value of the local_disconnection_reason_t created.
     */
    local_disconnection_reason_t(type value) : SafeEnum(value)
    {
    }
};


/**
 * Enumeration of disconnection reasons received in a disconnection event.
 */
struct disconnection_reason_t : SafeEnum<disconnection_reason_t, uint8_t> {
    /// enumeration of disconnection_reason_t values.
    enum type {
        /**
         * GAP or GATT failed to authenticate the peer.
         */
        AUTHENTICATION_FAILURE = 0x05,

        /**
         * The connection timed out.
         */
        CONNECTION_TIMEOUT = 0x08,

        /**
         * Connection terminated by the user.
         */
        REMOTE_USER_TERMINATED_CONNECTION = 0x13,

        /**
         * Remote device terminated connection due to low resources.
         */
        REMOTE_DEV_TERMINATION_DUE_TO_LOW_RESOURCES = 0x14,

        /**
         * Remote device terminated connection due to power off.
         */
        REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF = 0x15,

        /**
         * Indicate that the local user or the internal
         * Bluetooth subsystem terminated the connection.
         */
        LOCAL_HOST_TERMINATED_CONNECTION = 0x16,

        /**
         * Connection parameters were unacceptable.
         */
        UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B
    };

    /**
     * Construct a new instance of disconnection_reason_t.
     *
     * @param value The value of the disconnection_reason_t created.
     */
    disconnection_reason_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * Privacy Configuration of the peripheral role.
 *
 * @note This configuration also applies to the broadcaster role configuration.
 */
struct peripheral_privacy_configuration_t {
    /**
     * Indicates if nonresolvable random address should be used when the
     * peripheral advertises nonconnectable packets.
     *
     * Resolvable random address continues to be used for connectable packets.
     */
    bool use_non_resolvable_random_address;

    /**
     * Resolution strategy for initiator resolvable addresses when a
     * connection request is received.
     */
    enum resolution_strategy_t {
        /**
         * Do not resolve the address of the initiator and accept the
         * connection request.
         */
        DO_NOT_RESOLVE,

        /**
         * If a bond is present in the secure database and the address
         * resolution fails, then reject the connection request with the error
         * code AUTHENTICATION_FAILLURE.
         */
        REJECT_NON_RESOLVED_ADDRESS,

        /**
         * Perform the pairing procedure if the initiator resolvable
         * address failed the resolution process.
         */
        PERFORM_PAIRING_PROCEDURE,

        /**
         * Perform the authentication procedure if the initiator resolvable
         * address failed the resolution process.
         */
        PERFORM_AUTHENTICATION_PROCEDURE
    };

    MBED_DEPRECATED_SINCE("mbed-os-5.11", "Use resolution_strategy_t instead.")
    typedef resolution_strategy_t ResolutionStrategy;

    /**
     * Connection strategy to use when a connection request contains a
     * private resolvable address.
     */
    resolution_strategy_t resolution_strategy;
};

/**
 * Privacy configuration of the central role.
 *
 * @note This configuration is also used when the local device operates as
 * an observer.
 */
struct central_privay_configuration_t {
    /**
     * Indicates if nonresolvable random address should be used when the
     * central or observer sends scan request packets.
     *
     * Resolvable random address continues to be used for connection requests.
     */
    bool use_non_resolvable_random_address;


    /**
     * Resolution strategy of resolvable addresses received in advertising
     * packets.
     */
    enum resolution_strategy_t {
        /**
         * Do not resolve the address received in advertising packets.
         */
        DO_NOT_RESOLVE,

        /**
         * Resolve the resolvable addresses in the advertising packet and
         * forward advertising packet to the application independently of
         * the address resolution procedure result.
         */
        RESOLVE_AND_FORWARD,

        /**
         * Filter out packets containing a resolvable address that cannot be resolved
         * by this device.
         *
         * @note Filtering is applied if the local device contains at least
         * one bond.
         */
        RESOLVE_AND_FILTER
    };

    MBED_DEPRECATED_SINCE("mbed-os-5.11", "Use resolution_strategy_t instead.")
    typedef resolution_strategy_t ResolutionStrategy;

    /**
     * Resolution strategy applied to advertising packets received by the
     * local device.
     */
    resolution_strategy_t resolution_strategy;
};


/**
 * @}
 * @}
 */

} // namespace ble

#endif //BLE_GAP_TYPES_H
