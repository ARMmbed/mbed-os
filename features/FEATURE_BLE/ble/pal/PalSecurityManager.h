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
using SecurityManager::IO_CAPS_NONE;
using SecurityManager::SecurityCompletionStatus_t;
using SecurityManager::SecurityMode_t;
using SecurityManager::LinkSecurityStatus_t;
using SecurityManager::Passkey_t;
using SecurityManager::Keypress_t;
using SecurityManager::C192_t;
using SecurityManager::R192_t;
using SecurityManager::C256_t;
using SecurityManager::R256_t;

using BLEProtocol::AddressBytes_t;
using BLEProtocol::Address_t;


typedef uint8_t irk_t[16];
typedef uint8_t csrk_t[16];
typedef uint8_t ltk_t[16];
typedef uint8_t ediv_t[8];
typedef uint8_t rand_t[2];
typedef uint32_t passkey_num_t;

struct bonded_list_entry_t {
    Address_t peer_address;
    ediv_t ediv;
    rand_t rand;
    ltk_t ltk;
    csrk_t csrk;
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
struct bonded_list_t {
    bonded_list_entry_t *entries; /**< pointer to array storing the entries */
    uint8_t size; /**< actual number of entries */
    uint8_t capacity;  /**< number of entries that can be stored */
};


class SecurityManager : private mbed::NonCopyable<SecurityManager> {
public:
    SecurityManager() : _event_handler(NULL) { };
    virtual ~SecurityManager() { };

    virtual ble_error_t initialize() = 0;
    virtual ble_error_t terminate() = 0;
    virtual ble_error_t reset() = 0;

    /* for persistence */

    virtual ble_error_t get_bonded_list(bonded_list_t &list) = 0;
    virtual ble_error_t set_bonded_list(bonded_list_t &list) = 0;

    virtual ble_error_t get_resolving_list(resolving_list_t &list) = 0;
    virtual ble_error_t set_resolving_list(resolving_list_t &list) = 0;

    virtual ble_error_t get_whitelist(Gap::Whitelist_t &addresses) = 0;
    virtual ble_error_t set_whitelist(Gap::Whitelist_t &addresses) = 0;

    /* security settings */

    virtual ble_error_t set_private_address_timeout(uint16_t timeout /*s*/) = 0;

    virtual ble_error_t refresh_encryption_key(connection_handle_t) = 0;

    virtual ble_error_t set_authentication_timeout(connection_handle_t, uint16_t timeout /*x10 ms*/) = 0;
    virtual ble_error_t get_authentication_timeout(connection_handle_t, uint16_t &timeout /*x10 ms*/) = 0;

    virtual ble_error_t set_pin_code(uint8_t pin_length, uint8_t *pin_code, bool static_pin = false) = 0;
    virtual ble_error_t set_passkey(passkey_num_t passkey) = 0;

    /* feature support */

    virtual ble_error_t set_secure_connections_support(bool enabled, bool secure_connections_only = false) = 0;
    virtual ble_error_t get_secure_connections_support(bool &enabled, bool &secure_connections_only) = 0;

    virtual ble_error_t get_encryption_key_size(connection_handle_t, uint8_t &size) = 0;

    /* security level */

    virtual ble_error_t set_security_settings(bool bondable = true,
                                              SecurityIOCapabilities_t iocaps = IO_CAPS_NONE,
                                              bool send_keypresses = false) = 0;

    virtual ble_error_t set_oob_data_usage(connection_handle_t connectionHandle, bool useOOB, bool OOBProvidesMITM) = 0;

    /* triggers pairing if required */
    virtual ble_error_t set_security_mode(connection_handle_t handle,
                                          SecurityMode_t mode) = 0;


    virtual ble_error_t get_security_mode(connection_handle_t handle,
                                          SecurityMode_t &mode) = 0;


    virtual ble_error_t get_encryption_status(connection_handle_t handle,
                                              LinkSecurityStatus_t &mode) = 0;

    virtual ble_error_t request_pairing(bool authentication_required = true) = 0;
    virtual ble_error_t accept_pairing_request(bool accept = true, bool authentication_required = true) = 0;

    virtual ble_error_t request_authentication() = 0;

    virtual ble_error_t set_pairing_request_authorisation(bool required = true) = 0;

    /* MITM */

    virtual ble_error_t confirmation_entered(connection_handle_t address, bool confirmation) = 0;
    virtual ble_error_t passkey_entered(connection_handle_t, Passkey_t passkey) = 0;
    virtual ble_error_t send_keypress_notification(connection_handle_t, Keypress_t keypress) = 0;

    virtual ble_error_t set_oob(connection_handle_t handle, C192_t&, R192_t&) = 0;
    virtual ble_error_t set_extended_oob(connection_handle_t handle, C192_t&, R192_t&, C256_t&, R256_t&) = 0;
    virtual ble_error_t get_local_oob_data(connection_handle_t handle, C192_t&, R192_t&) = 0;
    virtual ble_error_t get_local_extended_oob_data(connection_handle_t handle, C192_t&, R192_t&, C256_t&, R256_t&) = 0;

    /* Entry points for the underlying stack to report events back to the user. */
 public:
    /* guaranteed to return valid pointer */
    SecurityManagerEventHandler get_event_handler() {
        return _event_handler;
    }

    void setSecurityManagerEventHandler(SecurityManagerEventHandler* event_handler) {
        if (event_handler) {
            _event_handler = event_handler;
        }
    }

private:
    SecurityManagerEventHandler* _event_handler;

};

} /* namespace pal */
} /* namespace ble */

#endif /* MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_ */
