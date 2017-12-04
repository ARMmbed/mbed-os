/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef BLE_PAL_GAP_TYPES_H_
#define BLE_PAL_GAP_TYPES_H_

#include <algorithm>
#include "ble/BLETypes.h"
#include "ble/SafeEnum.h"

namespace ble {
namespace pal {

/**
 * Type of advertising the LE subsystem can use when it advertise.
 */
struct advertising_type_t : SafeEnum<advertising_type_t, uint8_t> {
    enum type {
        /**
         * Connectable and scannable undirected advertising .
         */
        ADV_IND = 0x00,

        /**
         * Connectable high duty cycle directed advertising
         */
        ADV_DIRECT_IND = 0x01,

        /**
         * Scannable undirected advertising
         */
        ADV_SCAN_IND = 0x02,

        /**
         * Non connectable undirected advertising
         */
        ADV_NONCONN_IND = 0x03,

        /**
         * Connectable low duty cycle directed advertising
         */
        ADV_DIRECT_IND_LOW_DUTY_CYCLE = 0x04
    };

    /**
     * Construct a new advertising_type_t value.
     */
    advertising_type_t(type value) :
        SafeEnum<advertising_type_t, uint8_t>(value) { }
};


/**
 * Type used to model the own address used during the following GAP operations:
 * advertising, scanning and initiating
 */
struct own_address_type_t : SafeEnum<own_address_type_t, uint8_t> {
    enum type {
        /**
         * Use the public device address
         */
        PUBLIC_ADDRESS = 0x00,

        /**
         * Use the random device address
         */
        RANDOM_ADDRESS = 0x01,

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
     */
    own_address_type_t(type value) :
        SafeEnum<own_address_type_t, uint8_t>(value) { }
};


/**
 * Type modeling the peer address type during direct advertising.
 */
struct advertising_peer_address_type_t :
    SafeEnum<advertising_peer_address_type_t, uint8_t> {
    enum type {
        /**
         * Public device address or identity address.
         */
        PUBLIC_ADDRESS = 0x00,

        /**
         * Random device address or random (static) identity address.
         */
        RANDOM_ADDRESS = 0x01
    };

    /**
     * Construct a new instance of advertising_peer_address_type_t.
     */
    advertising_peer_address_type_t(type value) :
        SafeEnum<advertising_peer_address_type_t, uint8_t>(value) { }
};


/**
 * Peer address type used during connection initiating.
 */
struct connection_peer_address_type_t :
    SafeEnum<connection_peer_address_type_t, uint8_t> {
    enum type {
        /**
         * Public device address
         */
        PUBLIC_ADDRESS = 0x00,

        /**
         * Random device address
         */
        RANDOM_ADDRESS = 0x01,

        /**
         * Public identity address.
         * @note remove once privacy mode is supported.
         */
        PUBLIC_IDENTITY_ADDRESS = 0x02,

        /**
         * Random (static) identity address.
         * @note remove once privacy mode is supported.
         */
        RANDOM_IDENTITY_ADDRESS = 0x03
    };

    /**
     * Construct a new connection_peer_address_type_t instance.
     */
    connection_peer_address_type_t(type value) :
        SafeEnum<connection_peer_address_type_t, uint8_t>(value) { }
};


/**
 * Address type used in whitelist operations
 */
struct whitelist_address_type_t : SafeEnum<whitelist_address_type_t, uint8_t> {
    enum type {
        PUBLIC_DEVICE_ADDRESS = 0x00,
        RANDOM_DEVICE_ADDRESS = 0x01,
        /* TODO: to be added with bluetooth 5 support:
        ANONYMOUS_ADVERTISEMENT_DEVICE_ADRESS
        */
    };

    /**
     * Construct a new whitelist_address_type_t instance.
     */
    whitelist_address_type_t(type value) :
        SafeEnum<whitelist_address_type_t, uint8_t>(value) { }
};


/**
 * Channel map which can be used during advertising.
 */
struct advertising_channel_map_t : SafeEnum<advertising_channel_map_t, uint8_t> {
    enum type {
        ADVERTISING_CHANNEL_37 = (1 << 0),
        ADVERTISING_CHANNEL_38 = (1 << 1),
        ADVERTISING_CHANNEL_37_AND_38 =
            ADVERTISING_CHANNEL_37 | ADVERTISING_CHANNEL_38,
        ADVERTISING_CHANNEL_39 = (1 << 2),
        ADVERTISING_CHANNEL_37_AND_39 =
            ADVERTISING_CHANNEL_37 | ADVERTISING_CHANNEL_39,
        ADVERTISING_CHANNEL_38_AND_39 =
            ADVERTISING_CHANNEL_38 | ADVERTISING_CHANNEL_39,
        ALL_ADVERTISING_CHANNELS =
            ADVERTISING_CHANNEL_37 | ADVERTISING_CHANNEL_38 | ADVERTISING_CHANNEL_39
    };

    /**
     * Construct a new advertising_channel_map_t instance.
     */
    advertising_channel_map_t(type value) :
        SafeEnum<advertising_channel_map_t, uint8_t>(value) { }
};


/**
 * HCI Error codes.
 */
struct hci_error_code_t : SafeEnum<hci_error_code_t, uint8_t> {
    enum type {
        SUCCESS = 0x00,
        UNKNOWN_HCI_COMMAND = 0x01,
        UNKNOWN_CONNECTION_IDENTIFIER = 0x02,
        HARDWARE_FAILLURE = 0x03,
        PAGE_TIMEOUT = 0x04,
        AUTHENTICATION_FAILLURE = 0x05,
        PIN_OR_KEY_MISSING = 0x06,
        MEMORY_CAPACITY_EXCEEDED = 0x07,
        CONNECTION_TIMEOUT = 0x08,
        CONNECTION_LIMIT_EXCEEDED = 0x09,
        SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED = 0x0A,
        CONNECTION_ALREADY_EXIST = 0x0B,
        COMMAND_DISALLOWED = 0x0C,
        CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES = 0x0D,
        CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS = 0x0E,
        CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR = 0x0F,
        CONNECTION_ACCEPT_TIMEOUT_EXCEEDED = 0x10,
        UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE = 0x11,
        INVALID_HCI_COMMAND_PARAMETERS = 0x12,
        REMOTE_USER_TERMINATED_CONNECTION = 0x13,
        REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES = 0x14,
        REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF = 0x15,
        CONNECTION_TERMINATED_BY_LOCAL_HOST = 0x16,
        REPEATED_ATTEMPTS = 0x17,
        PAIRING_NOT_ALLOWED = 0x18,
        UNKNOWN_LMP_PDU = 0x19,
        UNSUPPORTED_REMOTE_FEATURE = 0x1A,
        UNSUPPORTED_LMP_FEATURE = 0x1A,
        SCO_OFFSET_REJECTED = 0x1B,
        SCO_INTERVAL_REJECTED = 0x1C,
        SCO_AIR_MODE_REJECTED = 0x1D,
        INVALID_LMP_PARAMETERS = 0x1E,
        INVALID_LL_PARAMETERS = 0x1E,
        UNSPECIFIED_ERROR = 0x1F,
        UNSUPPORTED_LMP_PARAMETER_VALUE = 0x20,
        UNSUPPORTED_LL_PARAMETER_VALUE = 0x20,
        ROLE_CHANGE_NOT_ALLOWED = 0x21,
        LMP_RESPONSE_TIMEOUT = 0x22,
        LL_RESPONSE_TIMEOUT = 0x22,
        LMP_ERROR_TRANSACTION_COLLISION = 0x23,
        LL_PROCEDURE_COLLISION = 0x23,
        LMP_PDU_NOT_ALLOWED = 0x24,
        ENCRYPTION_MODE_NOT_ACCEPTABLE = 0x25,
        LINK_KEY_CANNOT_BE_CHANGED = 0x26,
        REQUESTED_QOS_NOT_SUPPORTED = 0x27,
        INSTANT_PASSED = 0x28,
        PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29,
        DIFFERENT_TRANSACTION_COLLISION = 0x2A,
        RESERVED_FOR_FUTURE_USE = 0x2B,
        QOS_UNACCEPTABLE_PARAMETER = 0x2C,
        QOS_REJECTED = 0x2D,
        CHANNEL_CLASSIFICATION_NOT_SUPPORTED = 0x2E,
        INSUFFICIENT_SECURITY = 0x2F,
        PARAMETER_OUT_OF_MANDATORY_RANGE = 0x30,
        //RESERVED_FOR_FUTURE_USE = 0x31,
        ROLE_SWITCH_PENDING = 0x32,
        //RESERVED_FOR_FUTURE_USE = 0x33,
        RESERVED_SLOT_VIOLATION = 0x34,
        ROLE_SWITCH_FAILED = 0x35,
        EXTENDED_INQUIRY_RESPONSE_TOO_LARGE = 0x36,
        SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST = 0x37,
        HOST_BUSY_PAIRING = 0x38,
        CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND = 0x39,
        CONTROLLER_BUSY = 0x3A,
        UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B,
        ADVERTISING_TIMEOUT = 0x3C,
        CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE = 0x3D,
        CONNECTION_FAILED_TO_BE_ESTABLISHED = 0x3E,
        MAC_CONNECTION_FAILED = 0x3F,
        COARSE_CLOCK_ADJUSTMENT_REJECTED_BUT_WILL_TRY_TO_ADJUST_USING_CLOCK_DRAGGING = 0x40,
        TYPE0_SUBMAP_NOT_DEFINED = 0x41,
        UNKNOWN_ADVERTISING_IDENTIFIER = 0x42,
        LIMIT_REACHED = 0x43,
        OPERATION_CANCELLED_BY_HOST = 0x44
    };

    /**
     * Construct a new hci_error_code_t instance.
     */
    hci_error_code_t(type value) :
        SafeEnum<hci_error_code_t, uint8_t>(value) { }
};


/**
 * Reasons which can be used to end a connection.
 */
struct disconnection_reason_t : SafeEnum<disconnection_reason_t, uint8_t> {
    enum type {
        AUTHENTICATION_FAILLURE = 0x05,
        REMOTE_USER_TERMINATED_CONNECTION = 0x13,
        REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES = 0x14,
        REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF = 0x15,
        UNSUPPORTED_REMOTE_FEATURE = 0x1A,
        PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29,
        UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B
    };

    /**
     * Construct a new disconnection_reason_t instance.
     */
    disconnection_reason_t(type value) :
        SafeEnum<disconnection_reason_t, uint8_t>(value) { }
};


/**
 * Filter policy which can be used during advertising.
 */
struct advertising_filter_policy_t :
    SafeEnum<advertising_filter_policy_t, uint8_t> {
    enum type {
        /**
         * Process connection and scan requests from all devices. The whitelist is
         * not used.
         */
        NO_FILTER = 0x00,

        /**
         * Process connection requests from all devices but filter out scan requests
         * of devices which are not in the whitelist.
         */
        FILTER_SCAN_REQUESTS = 0x01,

        /**
         * Process scan requests from all devices but filter out connection requests
         * of devices which are not in the whitelist.
         */
        FILTER_CONNECTION_REQUEST = 0x02,

        /**
         * Filter out scan or connection requests of devices which are not in the
         * whitelist.
         */
        FILTER_SCAN_AND_CONNECTION_REQUESTS = 0x03
    };

    /**
     * Construct a new instance of advertising_filter_policy_t.
     */
    advertising_filter_policy_t(type value) :
        SafeEnum<advertising_filter_policy_t, uint8_t>(value) { }
};


/**
 * Filter policy which can be used during a scan.
 */
struct scanning_filter_policy_t : SafeEnum<scanning_filter_policy_t, uint8_t> {
    enum type {
        /**
        * Accept all advertising packets except directed advertising packet not
        * addressed to this device.
        */
        NO_FILTER = 0x00,

        /**
        * Accept only advertising packets from devices in the whitelist except
        * directed advertising packet not addressed to this device.
        */
        FILTER_ADVERTISING = 0x01

        // EXTENDED ADVERTISING FILTER POLICY (accept private resolvable direct advertising)
    };

    /**
     * Construct a new instance of scanning_filter_policy_t.
     */
    scanning_filter_policy_t(type value) :
        SafeEnum<scanning_filter_policy_t, uint8_t>(value) { }

};


/**
 * Filter policy which can be used during connection initiation.
 */
struct initiator_policy_t : SafeEnum<initiator_policy_t, uint8_t> {
    enum type {
        /**
        * The whitelist is not used to determine which advertiser to connect to.
        */
        NO_FILTER,

        /**
        * Whitelist is used to determine which advertiser to connect to.
        */
        USE_WHITE_LIST
    };

    initiator_policy_t(type value) :
        SafeEnum<initiator_policy_t, uint8_t>(value) { }
};


/**
 * MAC address data type.
 */
struct address_t {
    /**
     * Create an invalid mac address, equal to FF:FF:FF:FF:FF:FF
     */
    address_t() {
        memset(value, 0xFF, sizeof(value));
    }

    /**
     * Initialize a mac address from an array of bytes.
     *
     * @param input_value value of the MAC address.
     */
    address_t(const uint8_t (&input_value)[6]) {
        memcpy(value, input_value, sizeof(value));
    }

    /**
     * Initialize a mac address from a pointer to a buffer.
     *
     * @param input_value Buffer containing the mac address. It shall be at
     * least 6 long.
     *
     * @param tag Tag used to select this constructor. The value does not matter.
     */
    address_t(const uint8_t* input_value, bool tag) {
        memcpy(value, input_value, sizeof(value));
    }

    /**
     * Equal operator between two addresses.
     */
    friend bool operator==(const address_t& lhs, const address_t& rhs) {
        return memcmp(lhs.value, rhs.value, sizeof(lhs.value)) == 0;
    }

    /**
     * Non equal operator between two addresses.
     */
    friend bool operator!=(const address_t& lhs, const address_t& rhs) {
        return !(lhs == rhs);
    }

    /**
     * Subscript operator to access mac address content
     */
    uint8_t operator[](uint8_t i) const {
        return value[i];
    }

    /**
     * Return the pointer to the buffer holding mac address.
     */
    const uint8_t* data() const {
        return value;
    }

    /**
     * Size in byte of a mac address.
     */
    static uint8_t size() {
        return sizeof(value);
    }

private:
    uint8_t value[6];
};


/**
 * Hold advertising data.
 */
struct advertising_data_t {
    /**
     * Construct advertising data from an array.
     *
     * @param input_value Reference to the array containing the advertising data
     */
    advertising_data_t(const uint8_t (&input_value)[31]) {
        memcpy(value, input_value, sizeof(value));
    }

    /**
     * Construct advertising data from a pointer to a buffer.
     *
     * @param input_value Pointer to the buffer containing the advertising data.
     *
     * @param len Length of the buffer.
     */
    advertising_data_t(const uint8_t* input_value, size_t len) {
        const size_t actual_len = std::min(len, sizeof(value));
        memcpy(value, input_value, actual_len);
        memset(value + actual_len, 0x00, sizeof(value) - actual_len);
    }

    /**
     * Equal operator between two advertising data.
     */
    friend bool operator==(
        const advertising_data_t& lhs, const advertising_data_t& rhs
    ) {
        return memcmp(lhs.value, rhs.value, sizeof(lhs.value)) == 0;
    }

    /**
     * Non equal operator between two advertising data.
     */
    friend bool operator!=(
        const advertising_data_t& lhs, const advertising_data_t& rhs
    ) {
        return !(lhs == rhs);
    }

    /**
     * Subscript operator used to access the content of the advertising data.
     */
    uint8_t operator[](uint8_t i) const {
        return value[i];
    }

    /**
     * Return a pointer to the advertising data buffer.
     */
    const uint8_t* data() const {
        return value;
    }

    /**
     * Return (fixed) size of advertising data.
     */
    static uint8_t size() {
        return sizeof(value);
    }

private:
    uint8_t value[31];
};


/**
 * Type of advertising the LE subsystem can use when it advertise.
 */
struct received_advertising_type_t :
    SafeEnum<received_advertising_type_t, uint8_t> {
    enum type {
        /**
         * Connectable and scannable undirected advertising .
         */
        ADV_IND = 0x00,

        /**
         * Connectable high duty cycle directed advertising
         */
        ADV_DIRECT_IND = 0x01,

        /**
         * Scannable undirected advertising
         */
        ADV_SCAN_IND = 0x02,

        /**
         * Non connectable undirected advertising
         */
        ADV_NONCONN_IND = 0x03,

        /**
         * Response to a scan request.
         */
        SCAN_RESPONSE = 0x04
    };

    /**
     * Construct a new received_advertising_type_t value.
     */
    received_advertising_type_t(type value) :
        SafeEnum<received_advertising_type_t, uint8_t>(value) { }
};


/**
 * Model connection role. Used in GapConnectionCompleteEvent.
 */
struct connection_role_t : SafeEnum<connection_role_t, uint8_t> {
    enum type {
        MASTER,
        SLAVE
    };

    connection_role_t(type value) : SafeEnum<connection_role_t, uint8_t>(value) { }
};

} // namespace pal
} // namespace ble

#endif /* BLE_PAL_GAP_TYPES_H_ */
