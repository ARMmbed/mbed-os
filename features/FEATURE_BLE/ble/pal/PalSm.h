/*
 * PalSM.h
 *
 *  Created on: 12 Dec 2017
 *      Author: pawszc02
 */

#ifndef MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_
#define MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_

#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "ble/BLETypes.h"
#include "ble/SafeEnum.h"
#include "ble/BLEProtocol.h"
#include "ble/SecurityManager.h"

namespace ble {
namespace pal {

using SecurityManager::SecurityIOCapabilities_t;
using SecurityManager::SecurityCompletionStatus_t;
using SecurityManager::SecurityMode_t;
using SecurityManager::Passkey_t;
using SecurityManager::SecurityManagerEventBlock;

using BLEProtocol::AddressBytes_t;
using BLEProtocol::Address_t;


typedef uint8_t irk_t[16];
typedef uint8_t csrk_t[16];
typedef uint8_t ltk_t[16];
typedef uint8_t ediv_t[8];
typedef uint8_t rand_t[2];
typedef uint8_t passkey_t[4];
typedef uint8_t c192_t[16];
typedef uint8_t r192_t[16];
typedef uint8_t c256_t[16];
typedef uint8_t r256_t[16];

enum keypress_t {
    KEYPRESS_STARTED, /* Passkey entry started */
    KEYPRESS_ENTERED, /* Passkey digit entered */
    KEYPRESS_ERASED, /* Passkey digit erased */
    KEYPRESS_CLEARED, /* Passkey cleared */
    KEYPRESS_COMPLETED, /* Passkey entry completed */
};

struct bonded_list_entry_t {
    Address_t peer_address;
    ediv_t ediv;
    rand_t rand;
    ltk_t ltk;
};

struct resolving_list_entry_t {
    Address_t peer_address;
    irk_t peer_irk;
    irk_t local_irk;
};

/** Representation of a resolving list. */
struct resolving_list_t {
    resolving_list_entry_t *entries; /**< pointer to array storing the entries */
    uint8_t size; /**< actual number of entries */
    uint8_t capacity;  /**< number of entries that can be stored */
};

/** Representation of a bonded list. */
struct boonded_list_t {
    bonded_list_entry_t *entries; /**< pointer to array storing the entries */
    uint8_t size; /**< actual number of entries */
    uint8_t capacity;  /**< number of entries that can be stored */
};


class SecurityManagerPal : private mbed::NonCopyable<SecurityManagerPal> {
public:
    SecurityManagerPal(SecurityManagerEventHandler& sme) : _sme(sme) { };

    virtual ble_error_t initialize() = 0;
    virtual ble_error_t terminate() = 0;
    virtual ble_error_t reset() = 0;

    /* for persistence */

    virtual ble_error_t get_bonded_list(boonded_list_t *list) = 0;
    virtual ble_error_t set_bonded_list(boonded_list_t *list) = 0;

    virtual ble_error_t get_resolving_list(resolving_list_t *list) = 0;
    virtual ble_error_t set_resolving_list(resolving_list_t *list) = 0;

    /* security settings */

    virtual ble_error_t set_private_address_timeout(uint16_t timeout /*s*/) = 0;

    virtual ble_error_t refresh_encryption_key(connection_handle_t) = 0;

    virtual ble_error_t set_authentication_timeout(connection_handle_t, uint16_t timeout /*x10 ms*/) = 0;
    virtual ble_error_t get_authentication_timeout(connection_handle_t, uint16_t *timeout /*x10 ms*/) = 0;

    virtual ble_error_t set_pin_code(uint8_t pin_length, uint8_t *pin_code, bool variable_pin = true) = 0;

    /* feature support */

    virtual ble_error_t get_simple_pairing_mode(bool *enabled) = 0;
    virtual ble_error_t set_simple_pairing_mode(bool enabled) = 0;

    virtual ble_error_t set_secure_connections_support(bool enabled) = 0;
    virtual ble_error_t get_secure_connections_support(bool *enabled) = 0;

    virtual ble_error_t get_encryption_key_size(connection_handle_t, uint8_t *size) = 0;

    /* security level */

    virtual ble_error_t set_link_security(connection_handle_t handle, bool bondable, bool mitm, SecurityIOCapabilities_t iocaps) = 0;
    virtual ble_error_t get_link_security(connection_handle_t handle, bool *bondable, bool *mitm, SecurityIOCapabilities_t *iocaps) = 0;

    /* MITM */

    virtual ble_error_t use_oob(AddressBytes_t handle, bool enabled) = 0;

    virtual ble_error_t confirmation_entered(AddressBytes_t address, bool confirmation) = 0;
    virtual ble_error_t passkey_entered(AddressBytes_t, passkey_t passkey) = 0;
    virtual ble_error_t send_keypress_notification(AddressBytes_t, keypress_t keypress) = 0;

    virtual ble_error_t set_link_oob(AddressBytes_t handle, c192_t*, r192_t*) = 0;
    virtual ble_error_t set_link_extended_oob(AddressBytes_t handle, c192_t*, r192_t*,c256_t*, r256_t*) = 0;
    virtual ble_error_t get_local_oob_data(AddressBytes_t handle, c192_t*, r192_t*) = 0;
    virtual ble_error_t get_local_extended_oob_data(AddressBytes_t handle, c192_t*, r192_t*,c256_t*, r256_t*) = 0;

    /* Entry points for the underlying stack to report events back to the user. */
public:
    void process_valid_mic_timeout(connection_handle_t handle) = 0;
    void process_keypress(AddressBytes_t handle, keypress_t keypress) = 0;
    void process_oob_request(AddressBytes_t handle, bool extended = false) = 0;
    void process_pin_request(AddressBytes_t handle) = 0;
    void process_passkey_request(AddressBytes_t handle) = 0;
    void process_confirmation_request(AddressBytes_t handle) = 0;

    void process_link_key_failure(connection_handle_t handle) = 0;

    void process_security_setup_initiated_event(connection_handle_t handle, bool allow_bonding, bool require_mitm, SecurityIOCapabilities_t iocaps) {
        _evt.securitySetupInitiatedCallback(handle, allow_bonding, require_mitm, iocaps);
    }

    void process_security_setupCompleted_event(connection_handle_t handle, SecurityCompletionStatus_t status) {
        _evt.securitySetupCompletedCallback(handle, status);
    }

    void process_link_secured_event(connection_handle_t handle, SecurityMode_t security_mode) {
        _evt.linkSecuredCallback(handle, security_mode);
    }

    void process_security_context_stored_event(connection_handle_t handle) {
        _evt.securityContextStoredCallback(handle);
    }

    void process_Passkey_display_event(connection_handle_t handle, const Passkey_t passkey) {
        _evt.passkeyDisplayCallback(handle, passkey);
    }

private:
    SecurityManagerEventBlock& _evt;

};

}
}

#endif /* MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_ */
