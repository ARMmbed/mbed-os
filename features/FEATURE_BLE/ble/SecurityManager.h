/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#ifndef __SECURITY_MANAGER_H__
#define __SECURITY_MANAGER_H__

#include <stdint.h>

#include "Gap.h"
#include "CallChainOfFunctionPointersWithContext.h"

class SecurityManagerEventHandler;
class LegacySecurityManagerEventHandler;

using ble::connection_handle_t;

class SecurityManager {
public:
    enum Keypress_t {
        KEYPRESS_STARTED, /* Passkey entry started */
        KEYPRESS_ENTERED, /* Passkey digit entered */
        KEYPRESS_ERASED, /* Passkey digit erased */
        KEYPRESS_CLEARED, /* Passkey cleared */
        KEYPRESS_COMPLETED, /* Passkey entry completed */
    };

    enum SecurityMode_t {
        SECURITY_MODE_NO_ACCESS,
        SECURITY_MODE_ENCRYPTION_OPEN_LINK, /**< Require no protection, open link. */
        SECURITY_MODE_ENCRYPTION_NO_MITM,   /**< Require encryption, but no MITM protection. */
        SECURITY_MODE_ENCRYPTION_WITH_MITM, /**< Require encryption and MITM protection. */
        SECURITY_MODE_SIGNED_NO_MITM,       /**< Require signing or encryption, but no MITM protection. */
        SECURITY_MODE_SIGNED_WITH_MITM,     /**< Require signing or encryption, and MITM protection. */
    };

    /**
     * @brief Defines possible security status or states.
     *
     * @details Defines possible security status or states of a link when requested by getLinkSecurity().
     */
    enum LinkSecurityStatus_t {
        NOT_ENCRYPTED,          /**< The link is not secured. */
        ENCRYPTION_IN_PROGRESS, /**< Link security is being established.*/
        ENCRYPTED               /**< The link is secure.*/
    };

    enum SecurityIOCapabilities_t {
      IO_CAPS_DISPLAY_ONLY     = 0x00,   /**< Display only. */
      IO_CAPS_DISPLAY_YESNO    = 0x01,   /**< Display and yes/no entry. */
      IO_CAPS_KEYBOARD_ONLY    = 0x02,   /**< Keyboard only. */
      IO_CAPS_NONE             = 0x03,   /**< No I/O capabilities. */
      IO_CAPS_KEYBOARD_DISPLAY = 0x04,   /**< Keyboard and display. */
    };

    enum SecurityCompletionStatus_t {
        SEC_STATUS_SUCCESS              = 0x00,  /**< Procedure completed with success. */
        SEC_STATUS_TIMEOUT              = 0x01,  /**< Procedure timed out. */
        SEC_STATUS_PDU_INVALID          = 0x02,  /**< Invalid PDU received. */
        SEC_STATUS_PASSKEY_ENTRY_FAILED = 0x81,  /**< Passkey entry failed (user cancelled or other). */
        SEC_STATUS_OOB_NOT_AVAILABLE    = 0x82,  /**< Out of Band Key not available. */
        SEC_STATUS_AUTH_REQ             = 0x83,  /**< Authentication requirements not met. */
        SEC_STATUS_CONFIRM_VALUE        = 0x84,  /**< Confirm value failed. */
        SEC_STATUS_PAIRING_NOT_SUPP     = 0x85,  /**< Pairing not supported.  */
        SEC_STATUS_ENC_KEY_SIZE         = 0x86,  /**< Encryption key size. */
        SEC_STATUS_SMP_CMD_UNSUPPORTED  = 0x87,  /**< Unsupported SMP command. */
        SEC_STATUS_UNSPECIFIED          = 0x88,  /**< Unspecified reason. */
        SEC_STATUS_REPEATED_ATTEMPTS    = 0x89,  /**< Too little time elapsed since last attempt. */
        SEC_STATUS_INVALID_PARAMS       = 0x8A,  /**< Invalid parameters. */
        SEC_STATUS_DHKEY_CHECK_FAILED   = 0x8B,  /**< DHKey received doesn’t match locally calculated one. */
        SEC_STATUS_COMPARISON_FAILED    = 0x8C,  /**< Values in the numeric comparison protocol do not match. */
    };

    /**
     * Declaration of type containing a passkey to be used during pairing. This
     * is passed into initializeSecurity() to specify a pre-programmed passkey
     * for authentication instead of generating a random one.
     */
    static const unsigned PASSKEY_LEN = 6;
    typedef uint8_t Passkey_t[PASSKEY_LEN];         /**< 6-digit passkey in ASCII ('0'-'9' digits only). */

    typedef void (*HandleSpecificEvent_t)(Gap::Handle_t handle);
    typedef void (*SecuritySetupInitiatedCallback_t)(Gap::Handle_t, bool allowBonding, bool requireMITM, SecurityIOCapabilities_t iocaps);
    typedef void (*SecuritySetupCompletedCallback_t)(Gap::Handle_t, SecurityCompletionStatus_t status);
    typedef void (*LinkSecuredCallback_t)(Gap::Handle_t handle, SecurityMode_t securityMode);
    typedef void (*PasskeyDisplayCallback_t)(Gap::Handle_t handle, const Passkey_t passkey);

public:

    typedef FunctionPointerWithContext<const SecurityManager *> SecurityManagerShutdownCallback_t;
    typedef CallChainOfFunctionPointersWithContext<const SecurityManager *> SecurityManagerShutdownCallbackChain_t;

public:
    /* subclass to override handlers */
    class SecurityManagerEventHandler {
    public:
        SecurityManagerEventHandler() {};
        virtual ~SecurityManagerEventHandler() {};

        virtual void securitySetupInitiated(connection_handle_t handle, bool allowBonding, bool requireMITM, SecurityManager::SecurityIOCapabilities_t iocaps) {
            (void)handle;
            (void)allowBonding;
            (void)requireMITM;
            (void)iocaps;
        };
        virtual void securitySetupCompleted(connection_handle_t handle, SecurityManager::SecurityCompletionStatus_t status) {
            (void)handle;
            (void)status;
        };
        virtual void linkSecured(connection_handle_t handle, SecurityManager::SecurityMode_t securityMode) {
            (void)handle;
            (void)securityMode;
        };
        virtual void securityContextStored(connection_handle_t handle) {
            (void)handle;
        }
        virtual void passkeyDisplay(connection_handle_t handle, const SecurityManager::Passkey_t passkey) {
            (void)handle;
            (void)passkey;
        };
        virtual void validMicTimeout(connection_handle_t handle) {
            (void)handle;
        };
        virtual void linkKeyFailure(connection_handle_t handle) {
            (void)handle;
        };
        virtual void keypressNotification(connection_handle_t handle, SecurityManager::Keypress_t keypress) {
            (void)handle;
            (void)keypress;
        };
        virtual void legacyPairingOobRequest(connection_handle_t handle) {
            (void)handle;
        };
        virtual void oobRequest(connection_handle_t handle) {
            (void)handle;
        };
        virtual void pinRequest(connection_handle_t handle) {
            (void)handle;
        };
        virtual void passkeyRequest(connection_handle_t handle) {
            (void)handle;
        };
        virtual void confirmationRequest(connection_handle_t handle) {
            (void)handle;
        };
        virtual void acceptPairingRequest(connection_handle_t handle) {
            (void)handle;
        };
    };

private:
    /* legacy compatibility with old callbacks (from both sides, so combination of new and old works) */
    class LegacySecurityManagerEventHandler : public SecurityManagerEventHandler {
    public:
        LegacySecurityManagerEventHandler() :
            securitySetupInitiatedCallback(),
            securitySetupCompletedCallback(),
            linkSecuredCallback(),
            securityContextStoredCallback(),
            passkeyDisplayCallback() { };

        void securitySetupInitiated(connection_handle_t handle, bool allowBonding, bool requireMITM, SecurityManager::SecurityIOCapabilities_t iocaps) {
            if (securitySetupInitiatedCallback) {
                securitySetupInitiatedCallback(handle, allowBonding, requireMITM, iocaps);
            }
        };
        void securitySetupCompleted(connection_handle_t handle, SecurityManager::SecurityCompletionStatus_t status) {
            if (securitySetupCompletedCallback) {
                securitySetupCompletedCallback(handle, status);
            }
        };
        void linkSecured(connection_handle_t handle, SecurityManager::SecurityMode_t securityMode) {
            if (linkSecuredCallback) {
                linkSecuredCallback(handle, securityMode);
            }
        };
        void securityContextStored(connection_handle_t handle) {
            if (securityContextStoredCallback) {
                securityContextStoredCallback(handle);
            }
        }
        void passkeyDisplay(connection_handle_t handle, const SecurityManager::Passkey_t passkey) {
            if (passkeyDisplayCallback) {
                passkeyDisplayCallback(handle, passkey);
            }
        };

        SecurityManager::SecuritySetupInitiatedCallback_t securitySetupInitiatedCallback;
        SecurityManager::SecuritySetupCompletedCallback_t securitySetupCompletedCallback;
        SecurityManager::LinkSecuredCallback_t            linkSecuredCallback;
        SecurityManager::HandleSpecificEvent_t            securityContextStoredCallback;
        SecurityManager::PasskeyDisplayCallback_t         passkeyDisplayCallback;
    };

    /*
     * The following functions are meant to be overridden in the platform-specific sub-class.
     */
public:
    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    /**
     * Enable the BLE stack's Security Manager. The Security Manager implements
     * the actual cryptographic algorithms and protocol exchanges that allow two
     * devices to securely exchange data and privately detect each other.
     * Calling this API is a prerequisite for encryption and pairing (bonding).
     *
     * @param[in]  enableBonding Allow for bonding.
     * @param[in]  requireMITM   Require protection for man-in-the-middle attacks.
     * @param[in]  iocaps        To specify the I/O capabilities of this peripheral,
     *                           such as availability of a display or keyboard, to
     *                           support out-of-band exchanges of security data.
     * @param[in]  passkey       To specify a static passkey.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t init(bool                     enableBonding = true,
                             bool                     requireMITM   = true,
                             SecurityIOCapabilities_t iocaps        = IO_CAPS_NONE,
                             const Passkey_t          passkey       = NULL) {
        /* Avoid compiler warnings about unused variables. */
        (void)enableBonding;
        (void)requireMITM;
        (void)iocaps;
        (void)passkey;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Notify all registered onShutdown callbacks that the SecurityManager is
     * about to be shutdown and clear all SecurityManager state of the
     * associated object.
     *
     * This function is meant to be overridden in the platform-specific
     * sub-class. Nevertheless, the sub-class is only expected to reset its
     * state and not the data held in SecurityManager members. This shall be
     * achieved by a call to SecurityManager::reset() from the sub-class'
     * reset() implementation.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t reset(void) {
        /* Notify that the instance is about to shutdown */
        shutdownCallChain.call(this);
        shutdownCallChain.clear();
        if (eventHandler != &defaultEventHandler) {
            delete eventHandler;
            eventHandler = &defaultEventHandler;
        }

        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    /**
     * Delete all peer device context and all related bonding information from
     * the database within the security manager.
     *
     * @retval BLE_ERROR_NONE             On success, else an error code indicating reason for failure.
     * @retval BLE_ERROR_INVALID_STATE    If the API is called without module initialization or
     *                                    application registration.
     */
    virtual ble_error_t purgeAllBondingState(void) {
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Get a list of addresses from all peers in the bond table.
     *
     * @param[in,out]   addresses
     *                  (on input) addresses.capacity contains the maximum
     *                  number of addresses to be returned.
     *                  (on output) The populated table with copies of the
     *                  addresses in the implementation's whitelist.
     *
     * @retval BLE_ERROR_NONE             On success, else an error code indicating reason for failure.
     * @retval BLE_ERROR_INVALID_STATE    If the API is called without module initialization or
     *                                    application registration.
     */
    virtual ble_error_t getAddressesFromBondTable(Gap::Whitelist_t &addresses) const {
        /* Avoid compiler warnings about unused variables */
        (void) addresses;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    virtual ble_error_t allowLegacyPairing(bool allow = true) {
        (void) allow;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t getSecureConnectionsSupport(bool *enabled) {
        (void) enabled;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    virtual ble_error_t setPasskey(const Passkey_t passkey) {
        (void) passkey;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t setAuthenticationTimeout(connection_handle_t handle, uint32_t timeout_in_ms) {
        (void) handle;
        (void) timeout_in_ms;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t getAuthenticationTimeout(connection_handle_t handle, uint32_t *timeout_in_ms) {
        (void) handle;
        (void) timeout_in_ms;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Set the security mode on a connection. Useful for elevating the security mode
     * once certain conditions are met, e.g., a particular service is found.
     *
     * @param[in]  connectionHandle   Handle to identify the connection.
     * @param[in]  securityMode       Requested security mode.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setLinkSecurity(Gap::Handle_t connectionHandle, SecurityMode_t securityMode) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)securityMode;

        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    virtual ble_error_t getLinkSecurity(Gap::Handle_t handle, SecurityMode_t *securityMode) {
        (void) handle;
        (void) securityMode;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * @deprecated
     *
     * Get the security status of a connection.
     *
     * @param[in]  connectionHandle   Handle to identify the connection.
     * @param[out] securityStatusP    Security status.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t getLinkSecurity(Gap::Handle_t connectionHandle, LinkSecurityStatus_t *securityStatusP) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)securityStatusP;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t getEncryptionKeySize(Gap::Handle_t handle, uint8_t *size) {
        (void) handle;
        (void) size;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    virtual ble_error_t setPrivateAddressTimeout(uint16_t timeout_in_seconds) {
        (void) timeout_in_seconds;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    virtual ble_error_t requestPairing(Gap::Handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t acceptPairingRequest(Gap::Handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t canceltPairingRequest(Gap::Handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t requestAuthentication(Gap::Handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t setPairingRequestAuthorisation(bool required = true) {
        (void) required;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    virtual ble_error_t setOOBDataUsage(Gap::Handle_t connectionHandle, bool useOOB, bool OOBProvidesMITM = false) {
        /* Avoid compiler warnings about unused variables */
        (void) connectionHandle;
        (void) useOOB;
        (void) OOBProvidesMITM;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    virtual ble_error_t confirmationEntered(Gap::Handle_t handle, bool confirmation) {
        (void) handle;
        (void) confirmation;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }
    virtual ble_error_t passkeyEntered(Gap::Handle_t handle, Passkey_t passkey) {
        (void) handle;
        (void) passkey;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }
    virtual ble_error_t sendKeypressNotification(Gap::Handle_t handle, Keypress_t keypress) {
        (void) handle;
        (void) keypress;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /* Event callback handlers. */
public:
    /**
     * Setup a callback to be invoked to notify the user application that the
     * SecurityManager instance is about to shutdown (possibly as a result of a call
     * to BLE::shutdown()).
     *
     * @note  It is possible to chain together multiple onShutdown callbacks
     * (potentially from different modules of an application) to be notified
     * before the SecurityManager is shutdown.
     *
     * @note  It is also possible to set up a callback into a member function of
     * some object.
     *
     * @note It is possible to unregister a callback using onShutdown().detach(callback)
     */
    void onShutdown(const SecurityManagerShutdownCallback_t& callback) {
        shutdownCallChain.add(callback);
    }
    template <typename T>
    void onShutdown(T *objPtr, void (T::*memberPtr)(const SecurityManager *)) {
        shutdownCallChain.add(objPtr, memberPtr);
    }

    /**
     * @brief provide access to the callchain of shutdown event callbacks
     * It is possible to register callbacks using onShutdown().add(callback);
     * It is possible to unregister callbacks using onShutdown().detach(callback)
     * @return The shutdown event callbacks chain
     */
    SecurityManagerShutdownCallbackChain_t& onShutdown() {
        return shutdownCallChain;
    }

    virtual void setSecurityManagerEventHandler(SecurityManagerEventHandler* handler) {
        if (handler) {
            if (eventHandler && (eventHandler != &defaultEventHandler)) {
                delete eventHandler;
            }
            eventHandler = handler;
        }
    }

    virtual ble_error_t preserveBondingStateOnReset(bool enable) {
        /* Avoid compiler warnings about unused variables */
        (void) enable;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

protected:
    SecurityManager() {
        eventHandler = &defaultEventHandler;
    }

    virtual ~SecurityManager() { };

public:
    /**
     * @deprecated
     *
     * To indicate that a security procedure for the link has started.
     */
    virtual void onSecuritySetupInitiated(SecuritySetupInitiatedCallback_t callback) {
        defaultEventHandler.securitySetupInitiatedCallback = callback;
    }

    /**
     * @deprecated
     *
     * To indicate that the security procedure for the link has completed.
     */
    virtual void onSecuritySetupCompleted(SecuritySetupCompletedCallback_t callback) {
        defaultEventHandler.securitySetupCompletedCallback = callback;
    }

    /**
     * @deprecated
     *
     * To indicate that the link with the peer is secured. For bonded devices,
     * subsequent reconnections with a bonded peer will result only in this callback
     * when the link is secured; setup procedures will not occur (unless the
     * bonding information is either lost or deleted on either or both sides).
     */
    virtual void onLinkSecured(LinkSecuredCallback_t callback) {
        defaultEventHandler.linkSecuredCallback = callback;
    }

    /**
     * @deprecated
     *
     * To indicate that device context is stored persistently.
     */
    virtual void onSecurityContextStored(HandleSpecificEvent_t callback) {
        defaultEventHandler.securityContextStoredCallback = callback;
    }

    /** @deprecated
     *
     * To set the callback for when the passkey needs to be displayed on a peripheral with DISPLAY capability.
     */
    virtual void onPasskeyDisplay(PasskeyDisplayCallback_t callback) {
        defaultEventHandler.passkeyDisplayCallback = callback;
    }

    /* Entry points for the underlying stack to report events back to the user. */
public:
    /** @deprecated */
    void processSecuritySetupInitiatedEvent(Gap::Handle_t handle, bool allowBonding, bool requireMITM, SecurityIOCapabilities_t iocaps) {
        eventHandler->securitySetupInitiated(handle, allowBonding, requireMITM, iocaps);
    }
    /** @deprecated */
    void processSecuritySetupCompletedEvent(Gap::Handle_t handle, SecurityCompletionStatus_t status) {
        eventHandler->securitySetupCompleted(handle, status);
    }
    /** @deprecated */
    void processLinkSecuredEvent(Gap::Handle_t handle, SecurityMode_t securityMode) {
        eventHandler->linkSecured(handle, securityMode);
    }
    /** @deprecated */
    void processSecurityContextStoredEvent(Gap::Handle_t handle) {
        eventHandler->securityContextStored(handle);
    }
    /** @deprecated */
    void processPasskeyDisplayEvent(Gap::Handle_t handle, const Passkey_t passkey) {
        eventHandler->passkeyDisplay(handle, passkey);
    }

private:
    SecurityManagerEventHandler*           eventHandler;
    SecurityManagerShutdownCallbackChain_t shutdownCallChain;

protected:
    LegacySecurityManagerEventHandler      defaultEventHandler;
};

#endif /*__SECURITY_MANAGER_H__*/
