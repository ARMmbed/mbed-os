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

#ifndef SECURITY_MANAGER_H_
#define SECURITY_MANAGER_H_

#include <stdint.h>

#include "Gap.h"
#include "CallChainOfFunctionPointersWithContext.h"
#include "ble/BLETypes.h"

/**
 * Overview
 *
 * Security Manager is used to provide link security through encryption, signing and authentication
 * which are made possible by pairing and optionally bonding. Pairing is the process of establishing
 * and/or exchanging keys used for the current connection. Bonding means saving this information so that
 * it can later be used after reconnecting without having to pair again. This saves time and power.
 *
 * @par Paring
 *
 * There are several ways to provide different levels of security during pairing depending on your requirements
 * and the facilities provided by the application. The process starts with initialising the SecurityManager
 * with default options for new connections. Some settings can later be changed per link or globally.
 *
 * The important settings in the init() function are the MITM requirement and IO capabilities. Man in the
 * Middle (MITM) protection prevents an attack where one device can impersonate another device by
 * pairing with both devices at the same time. This protection is achieved by sharing some information
 * between the devices through some independent channel. The IO capabilities of both devices dictate
 * what algorithm is used. For details @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 2.3.5.1.
 * You can change the IO capabilities after initialisation with setIoCapability(). This will take effect
 * for all subsequent pairings.
 *
 * @par Out of Band data used in pairing
 *
 * Sharing this information through IO capabilities means user interaction which limits the degree of
 * protection due to the limit of the amount of data that we can expect the user to transfer. Another
 * solution is using OOB (out of band) communication to transfer this data instead which can send much
 * more data making MITM attack even less likely to succeed. OOB data has to be exchanged by the application
 * and provided to the Security Manager. Use setOOBDataUsage() to indicate you want to use it. The same call also
 * allows you to set whether or not the communication channel you are using to transmit the OOB data is
 * itself secure against MITM protection - this will set the level of the link security achieved using pairing
 * that uses this data.
 *
 * The most secure pairing is provided by Secure Connections which relies on Elliptical Curve Cryptography.
 * Support for Secure Connections is dependent on both the stack and controller on both sides supporting
 * it. If either side doesn't support it Legacy Pairing will be used. This is an older standard of pairing.
 * If higher security is required legacy pairing can be disabled by calling allowLegacyPairing(false);
 *
 * @par Signing
 *
 * Applications may require a level of security providing confidence that data transfers are coming
 * from a trusted source. This can be achieved by encrypting the link which also provides added confidentiality.
 * Encryption is a good choice when a device stays connected but introduces latency due to the need of encrypting the
 * link if the device only connects periodically to transfer data. If confidentiality is not required data GATT
 * server may allow writes to happen over an unencrypted link but authenticated by a signature present in each packet.
 * This signature relies on having sent a signing key to the peer during pairing prior to sending any signed packets.
 *
 * @par Persistence of Security information
 *
 * Security Manager stores all the data required for its operation on active links. Depending on resources
 * available on the device it will also attempt to store data for disconnected devices which have bonded to be
 * reused when reconnected.
 *
 * If the application has initialised a filesystem and the Security Manager has been provided with a
 * filepath during the init() call it may also provide data persistence across resets. This must be enabled by
 * calling preserveBondingStateOnReset(). Persistence is not guaranteed and may fail if abnormally terminated.
 * The Security Manager may also fall back to a non-persistent implementation if the resources are too limited.
 *
 * @par How to use
 *
 * First thing you need to do is to initialise the manager by calling init() with your chosen settings.
 *
 * The SecurityManager communicates with your application through events. These will trigger calls in
 * the EventHandler which you must provide by calling the setSecurityManagerEventHandler() function.
 *
 * The most important process is pairing. This may be triggered manually by calling requestPairing() or
 * may be called as a result of the application requiring encryption by calling setLinkEncryption() or
 * as a result of the application requiring MITM protection through requestAuthentication().
 *
 * All these can be implicitly called by using setLinkSecurity() to conveniently set the required
 * security for the link. The SecurityManager will trigger all the process required to achieve the set
 * security level. Security level can only be escalated. Asking the Security Manager for a lower
 * security level than the existing one will not fail but will result in a event informing the
 * application through linkEncryptionResult() of the current level (which remains unchanged).
 *
 * Depending on the IO capabilities and OOB usage settings different pairing algorithms will be chosen.
 * They will produce appropriate events which must be handled by your EventHandler. If your event handler
 * doesn't support all the calls you must not set IO capabilities or set OOB usage in such a way that would
 * trigger them or else the pairing will fail (usually by timing out).
 *
 * The simplest example is a pairing of a device with no IO capabilities and no OOB data available.
 * With such limited pairing capabilities the "just works" method will be employed. This does not provide
 * any MITM protection. The pairing (triggered implicitly or called explicitly) will result in an event
 * being generated on the peer calling pairingRequest(). The event handler must make a decision (either in
 * the application itself or based on user interaction) whether to accept the pairing and call
 * accetPairing() or cancelPairing(). The result will be communicated on both peers through an event calling
 * pairingResult() in the EventHandler.
 *
 * @par Sequence diagrams
 *
 * Sequence diagram "Just Works" pairing
 *
 * \verbatim
 *  /----------- Device 1 --------------\  *------ BLE link ------*  /-------------- Device 2 -------------\
 *
 * App  EventHandler              SecurityManager              SecurityManager           EventHandler     App
 *  |        |                          |                            |                        |            |
 *  |---------------------------> requestPairing()                   |                        |            |
 *  |        |                          |------[pairing start]------>|                        |            |
 *  |        |                          |                            |----------------> pairingRequest() ->|
 *  |        |                          |                        acceptPairing() <------------------------ |
 *  |        |                          |<---[pairing complete]----->|                        |            |
 *  |<- pairingResult() <---------------|                            |----------------> pairingResult() -->|
 *  |        |                          |                            |                        |            |
 * @endverbatim
 * 
 *  @note the requestPairing() call isn't required to trigger pairing. Pairing will also be triggered
 *  if you request encryption and authentication and no bonding information is available. The sequence will
 *  be the same save for the lack of explicit requestPairing() call.
 *
 *
 *  Sequence diagram Encryption request when bonding information is available
 *
 * \verbatim
 *  /----------- Device 1 --------------\  *------ BLE link ------*  /-------------- Device 2 -------------\
 *
 * App  EventHandler              SecurityManager              SecurityManager           EventHandler     App
 *  |       |                           |                            |                        |            |
 *  |---------------------------> setLinkEncryption()                |                        |            |
 *  |       |                           |<-[encryption established]->|                        |            |
 *  |<- linkEncryptionResult() <--------|                            |---------> linkEncryptionResult() -->|
 *  |       |                           |                            |                        |            |
 * @endverbatim
 * 
 * @note if bonding information is not available, pairing will be triggered
 *
 *
 * Sequence diagram for Secure Connections passkey entry pairing with one device having a display only
 * and other a keyboard
 *
 * \verbatim
 *  /----------- Device 1 (keyboard) ---\  *------ BLE link ------*  /-------------- Device 2 (display) ---\
 *
 * App  EventHandler              SecurityManager              SecurityManager           EventHandler     App
 *  |        |                          |                            |                        |            |
 *  |---------------------------> requestPairing()                   |                        |            |
 *  |        |                          |------[pairing start]------>|                        |            |
 *  |        |                          |                            |----------------> pairingRequest() ->|
 *  |        |                          |                        acceptPairing() <------------------------ |
 *  |        |                          |<---[secure con. pairing]-->|                        |            |
 *  |<- passkeyRequest() <--------------|                            |----------------> passkeyDisplay() ->|
 *  |        |                          |                            |                        |            |
 *
 *                       user reads the passkey on Device 2 and inputs it on Device 1
 *
 *  |        |                          |                            |                        |            |
 *  |-------------------------->passkeyEntered()                     |                        |            |
 *  |        |                          |<---[pairing complete]----->|                        |            |
 *  |<- pairingResult() <---------------|                            |----------------> pairingResult() -->|
 *  |        |                          |                            |                        |            |
 * @endverbatim
 * 
 */

class SecurityManager {
public:
    /** events sent and received when passkey is being entered */
    enum Keypress_t {
        KEYPRESS_STARTED,   /**< Passkey entry started */
        KEYPRESS_ENTERED,   /**< Passkey digit entered */
        KEYPRESS_ERASED,    /**< Passkey digit erased */
        KEYPRESS_CLEARED,   /**< Passkey cleared */
        KEYPRESS_COMPLETED, /**< Passkey entry completed */
    };

    /** level of security required from the link by the application */
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

    /** Input/output capability of the device and application */
    enum SecurityIOCapabilities_t {
        IO_CAPS_DISPLAY_ONLY = 0x00,     /**< Display only. */
        IO_CAPS_DISPLAY_YESNO = 0x01,    /**< Display and yes/no entry. */
        IO_CAPS_KEYBOARD_ONLY = 0x02,    /**< Keyboard only. */
        IO_CAPS_NONE = 0x03,             /**< No I/O capabilities. */
        IO_CAPS_KEYBOARD_DISPLAY = 0x04, /**< Keyboard and display. */
    };

    /** Result of security requests */
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
    typedef uint8_t Passkey_t[PASSKEY_LEN]; /**< 6-digit passkey in ASCII ('0'-'9' digits only). */

    typedef FunctionPointerWithContext<const SecurityManager *> SecurityManagerShutdownCallback_t;
    typedef CallChainOfFunctionPointersWithContext<const SecurityManager *> SecurityManagerShutdownCallbackChain_t;

    /* legacy callbacks, please use SecurityManagerEventHandler instead */
    typedef void (*HandleSpecificEvent_t)(ble::connection_handle_t connectionHandle);
    typedef void (*SecuritySetupInitiatedCallback_t)(ble::connection_handle_t, bool allowBonding, bool requireMITM, SecurityIOCapabilities_t iocaps);
    typedef void (*SecuritySetupCompletedCallback_t)(ble::connection_handle_t, SecurityCompletionStatus_t status);
    typedef void (*LinkSecuredCallback_t)(ble::connection_handle_t connectionHandle, SecurityMode_t securityMode);
    typedef void (*PasskeyDisplayCallback_t)(ble::connection_handle_t connectionHandle, const Passkey_t passkey);

    /** The stack will use these functions to signal events to the application,
     *  subclass to override handlers. Use SecurityManager::setSecurityManagerEventHandler
     *  to set the interface implementation to be used. */
    class EventHandler {
    public:
        EventHandler() {};
        virtual ~EventHandler() {};

        ////////////////////////////////////////////////////////////////////////////
        // Pairing
        //

        /**
         * Request application to accept or reject pairing. Application should respond by
         * calling the appropriate function: acceptPairingRequest or cancelPairingRequest
         *
         * @param[in] connectionHandle connection connectionHandle
         */
        virtual void pairingRequest(ble::connection_handle_t connectionHandle) {
            (void)connectionHandle;
        }

        /**
         * Indicate to the application that pairing has completed.
         *
         * @param[in] connectionHandle connection connectionHandle
         * @param[in] result result of the pairing indicating success or reason for failure
         */
        virtual void pairingResult(ble::connection_handle_t connectionHandle, SecurityCompletionStatus_t result) {
            (void)connectionHandle;
            (void)result;
        }

        ////////////////////////////////////////////////////////////////////////////
        // Security
        //

        /**
         * Deliver the requested whitelist to the application.
         *
         * @param[in] whitelist pointer to the whitelist filled with entries based on bonding information
         */
        virtual void whitelistFromBondTable(Gap::Whitelist_t* whitelist) {
            (void)whitelist;
        }

        ////////////////////////////////////////////////////////////////////////////
        // Encryption
        //

        /**
         * Inform the device of the encryption state of a given link.
         *
         * @param[in] connectionHandle connection connectionHandle
         * @param[in] result encryption state of the link
         */
        virtual void linkEncryptionResult(ble::connection_handle_t connectionHandle, ble::link_encryption_t result) {
            (void)connectionHandle;
            (void)result;
        }

        ////////////////////////////////////////////////////////////////////////////
        // MITM
        //

        /**
         * Display the given passkey on the local device.
         *
         * @param[in] connectionHandle connection connectionHandle
         * @param[in] passkey 6 digit passkey to be displayed
         */
        virtual void passkeyDisplay(ble::connection_handle_t connectionHandle, const SecurityManager::Passkey_t passkey) {
            (void)connectionHandle;
            (void)passkey;
        }

        /**
         * Indicate to the application that a confirmation is required. This is used
         * when the device does not have a keyboard but has a yes/no button. The device
         * displays numbers on its display in response to passkeyDisplay and the user
         * checks if they are the same on both devices. The application should proceed
         * by supplying the confirmation using the confirmationEntered function.
         *
         * @param[in] connectionHandle connection connectionHandle
         */
        virtual void confirmationRequest(ble::connection_handle_t connectionHandle) {
            (void)connectionHandle;
        }

        /**
         * Indicate to the application that a passkey is required. The application should
         * proceed by supplying the passkey through the passkeyEntered function.
         *
         * @param[in] connectionHandle connection connectionHandle
         */
        virtual void passkeyRequest(ble::connection_handle_t connectionHandle) {
            (void)connectionHandle;
        }

        /**
         * Notify the application that a key was pressed by the peer during passkey entry.
         *
         * @param[in] connectionHandle connection connectionHandle
         * @param[in] keypress type of keypress event
         */
        virtual void keypressNotification(ble::connection_handle_t connectionHandle, SecurityManager::Keypress_t keypress) {
            (void)connectionHandle;
            (void)keypress;
        }

        /**
         * Indicate to the application it needs to return legacy pairing OOB to the stack.
         *
         * @param[in] connectionHandle connection connectionHandle
         */
        virtual void legacyPairingOobRequest(ble::connection_handle_t connectionHandle) {
            (void)connectionHandle;
        }

        /**
         * Indicate that the application needs to send legacy pairing OOB data to the peer.
         *
         * @param[in] address address that will be used in the pairing
         * @param[in] temporaryKey temporary key to be used in legacy pairing
         */
        virtual void legacyPairingOobGenerated(const ble::address_t *address,
                                               const ble::oob_tk_t *temporaryKey) {
            (void)address;
            (void)temporaryKey;
        }

        /**
         * Indicate that the application needs to send secure connections OOB data to the peer.
         *
         * @param[in] address address that will be used in the pairing
         * @param[in] random random number used to generate the confirmation
         * @param[in] confirm confirmation value to be use for authentication
         *                    in secure connections pairing
         */
        virtual void oobGenerated(const ble::address_t *address,
                                  const ble::oob_lesc_value_t *random,
                                  const ble::oob_confirm_t *confirm) {
            (void)address;
            (void)random;
            (void)confirm;
        }

        ////////////////////////////////////////////////////////////////////////////
        // Keys
        //

        /**
         * Deliver the signing key to the application.
         *
         * @param[in] connectionHandle connection connectionHandle
         * @param[in] csrk signing key, pointer only valid during call
         * @param[in] authenticated indicates if the signing key is authenticated
         */
        virtual void signingKey(ble::connection_handle_t connectionHandle, const ble::csrk_t *csrk, bool authenticated) {
            (void)connectionHandle;
            (void)csrk;
            (void)authenticated;
        }
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
     * @param[in]  signing       Generate and distribute signing key during pairing
     * @param[in]  dbFilepath    Path to the file used to store keys in the filesystem,
     *                           if NULL keys will be only stored in memory
     *
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t init(bool                     enableBonding = true,
                             bool                     requireMITM   = true,
                             SecurityIOCapabilities_t iocaps        = IO_CAPS_NONE,
                             const Passkey_t          passkey       = NULL,
                             bool                     signing       = true,
                             const char              *dbFilepath    = NULL) {
        /* Avoid compiler warnings about unused variables. */
        (void)enableBonding;
        (void)requireMITM;
        (void)iocaps;
        (void)passkey;
        (void)dbFilepath;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Change the file used for the security database. If path is invalid or a NULL is passed
     * keys will only be stored in memory.
     *
     * @note This operation is only allowed with no active connections.
     *
     * @param[in]  dbFilepath    Path to the file used to store keys in the filesystem,
     *                           if NULL keys will be only stored in memory
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setDatabaseFilepath(const char *dbFilepath = NULL) {
        (void)dbFilepath;
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
        eventHandler = &defaultEventHandler;

        return BLE_ERROR_NONE;
    }

    /**
     * Normally all bonding information is lost when device is reset, this requests that the stack
     * attempts to save the information and reload it during initialisation. This is not guaranteed.
     *
     * @param[in] enable if true the stack will attempt to preserve bonding information on reset.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t preserveBondingStateOnReset(bool enable) {
        /* Avoid compiler warnings about unused variables */
        (void) enable;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
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
     * Create a list of addresses from all peers in the bond table and generate
     * an event which returns it as a whitelist. Pass in the container for the whitelist.
     * This will be returned by the event.
     *
     * @param[in] whitelist Preallocated whitelist which will be filled up to its capacity.
     *                      If whitelist already contains entries this will be appended to.
     *                      Do not access the whitelist until callback has been called,
     *                      returning the filled whitelist.
     *
     * @retval BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    virtual ble_error_t generateWhitelistFromBondTable(Gap::Whitelist_t *whitelist) const {
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    /**
     * Request pairing with the peer. Called by the master.
     * @note Slave can call requestAuthentication or setLinkEncryption to achieve security.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t requestPairing(ble::connection_handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Accept the pairing request. Called as a result of pairingRequest being called
     * on the event handler.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t acceptPairingRequest(ble::connection_handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Reject pairing request if the local device is the slave or cancel an outstanding
     * pairing request if master.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t cancelPairingRequest(ble::connection_handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Tell the stack whether the application needs to authorise pairing requests or should
     * they be automatically accepted.
     *
     * @param[in] required If set to true, pairingRequest in the event handler will
     *                     will be called and will require an action from the application
     *                     to continue with pairing by calling acceptPairingRequest
     *                     or cancelPairingRequest if the user wishes to reject it.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setPairingRequestAuthorisation(bool required = true) {
        (void) required;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    /**
     * Allow of disallow the use of legacy pairing in case the application only wants
     * to force the use of Secure Connections. If legacy pairing is disallowed and either
     * side doesn't support Secure Connections the pairing will fail.
     *
     * @param[out] allow If true legacy pairing will be used if either side doesn't support Secure Connections.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t allowLegacyPairing(bool allow = true) {
        (void) allow;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Check if the Secure Connections feature is supported by the stack and controller.
     *
     * @param[out] enabled true if SC are supported
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t getSecureConnectionsSupport(bool *enabled) {
        (void) enabled;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    /**
     * Set the IO capability of the local device.
     *
     * @param[in] iocaps type of IO capabilities available on the local device
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setIoCapability(SecurityIOCapabilities_t iocaps) {
        (void) iocaps;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Set the passkey that is displayed on the local device instead of using
     * a randomly generated one
     *
     * @param[in] passkey ASCII string of 6 digits
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setDisplayPasskey(const Passkey_t passkey) {
        (void) passkey;
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
    virtual ble_error_t setLinkSecurity(ble::connection_handle_t connectionHandle, SecurityMode_t securityMode) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)securityMode;

        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set whether or not we want to send and receive keypress notifications
     * during passkey entry.
     *
     * @param[in] enabled if true pairing will try to enable keypress notifications
     * (dependent on other side supporting it)
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setKeypressNotification(bool enabled = true) {
        (void)enabled;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Request generation and exchange of signing keys so that packet signing can be utilised
     * on this connection.
     * @note This does not generate a signingKey event. Use getSigningKey for that.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] enabled          If set to true, signing keys will be exchanged
     *                             during subsequent pairing.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t enableSigning(ble::connection_handle_t connectionHandle, bool enabled = true) {
        (void) enabled;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Give a hint to the stack that the master/slave role might change in the future.
     *
     * @param[in] enable If set to true it hints the roles are likely to swap in the future.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setHintFutureRoleReversal(bool enable = true) {
        (void)enable;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * Current state of encryption on the link.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[out] encryption
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t getLinkEncryption(ble::connection_handle_t connectionHandle, ble::link_encryption_t *encryption) {
        (void)connectionHandle;
        (void)encryption;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Enabled or disable encryption on the link. The result of this request will be indicated
     * by a call to linkEncryptionResult in the event handler when the action is completed.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] encryption encryption state requested
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setLinkEncryption(ble::connection_handle_t connectionHandle, ble::link_encryption_t encryption) {
        (void)connectionHandle;
        (void)encryption;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Set the requirements for encryption key size. If the peer cannot comply with the requirements
     * paring will fail.
     *
     * @param[in] minimumByteSize Smallest allowed encryption key size in bytes. (no smaller than 7)
     * @param[in] maximumByteSize Largest allowed encryption key size in bytes. (no larger than 16)
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setEncryptionKeyRequirements(uint8_t minimumByteSize, uint8_t maximumByteSize) {
        (void) minimumByteSize;
        (void) maximumByteSize;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * Request that the link be authenticated (keys with MITM protection). This might trigger encryption
     * or pairing/re-pairing. The success will be indicated through an event indicating security level change.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t requestAuthentication(ble::connection_handle_t connectionHandle) {
        (void) connectionHandle;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * Generate OOB data with the given address. If Secure Connections is supported this will
     * also generate Secure Connections OOB data on top of legacy pairing OOB data. This can be used
     * to generate such data before the connection takes place.
     *
     * In this model the OOB exchange takes place before the devices connect. Devices should establish
     * communication over another channel and exchange the OOB data. The address provided will be used
     * by the peer to associate the received data with the address of the device it will then connect
     * to over BLE.
     *
     * @param[in] address The local address you will use in the connection using this OOB data. This
     *                    address will be returned along with the rest of the OOB data when generation
     *                    is complete. Using an invalid address is illegal.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t generateOOB(const ble::address_t *address) {
        /* Avoid compiler warnings about unused variables */
        (void) address;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Enable OOB data usage during paring. If Secure Connections is supported enabling useOOB will
     * generate Secure Connections OOB data through oobGenerated() on top of legacy pairing OOB data.
     *
     * You do not have to call this function to return received OOB data. Use legacyPairingOobReceived
     * or oobReceived to hand it in. This will allow the stack to use it if possible. You only need to
     * call this function to attempt legacy OOB data exchange after pairing start and to inform
     * the stack OOB data does not provide MITM protection (by default it is set to provide this).
     *
     * In this model the OOB exchange takes places after the devices have connected but possibly
     * prior to pairing. For secure connections pairing must not be started until after the OOB
     * data has been sent and/or received. The address in the OOB data generated will match
     * the original address used to establish the connection and will be used by the peer to
     * identify which connection the OOB data belongs to.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] useOOB If set to true, authenticate using OOB data.
     * @param[in] OOBProvidesMITM If set to true keys exchanged during pairing using OOB data
     *                            will provide Man-in-the-Middle protection. This indicates that
     *                            the form of exchange used by the OOB data itself provides MITM
     *                            protection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t setOOBDataUsage(ble::connection_handle_t connectionHandle, bool useOOB, bool OOBProvidesMITM = true) {
        /* Avoid compiler warnings about unused variables */
        (void) connectionHandle;
        (void) useOOB;
        (void) OOBProvidesMITM;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Report to the stack if the passkey matches or not. Used during pairing to provide MITM protection.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] confirmation True value indicates the passkey displayed matches.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t confirmationEntered(ble::connection_handle_t connectionHandle, bool confirmation) {
        (void) connectionHandle;
        (void) confirmation;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Supply the stack with the user entered passkey.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] passkey ASCII string of digits entered by the user.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t passkeyEntered(ble::connection_handle_t connectionHandle, Passkey_t passkey) {
        (void) connectionHandle;
        (void) passkey;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Send a notification to the peer that the user pressed a key on the local device.
     * @note This will only be delivered if the keypress notifications have been enabled during pairing.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] keypress Type of keypress event.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t sendKeypressNotification(ble::connection_handle_t connectionHandle, Keypress_t keypress) {
        (void) connectionHandle;
        (void) keypress;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Supply the stack with the OOB data for legacy connections.
     *
     * @param[in] address address of the peer device this data comes from
     * @param[in] tk pointer to out of band data received containing the temporary key.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t legacyPairingOobReceived(const ble::address_t *address, const ble::oob_tk_t *tk) {
        (void) address;
        (void) tk;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    /**
     * Supply the stack with the OOB data for secure connections.
     *
     * @param[in] address address of the peer device this data comes from
     * @param[in] random random number used to generate the confirmation
     * @param[in] confirm confirmation value to be use for authentication
     *                    in secure connections pairing
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t oobReceived(const ble::address_t *address, const ble::oob_lesc_value_t *random, const ble::oob_confirm_t *confirm) {
        (void) address;
        (void) random;
        (void) confirm;
        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if security is supported. */
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /**
     * Retrieves a signing key through a signingKey event.
     * If a signing key is not present, pairing/authentication will be attempted.
     * @note This will attempt to retrieve the key even if enableSigning hasn't been called prior to pairing.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] authenticated    Whether the signing key needs to be authenticated
     *                             (provide MITM protection).
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual ble_error_t getSigningKey(ble::connection_handle_t connectionHandle, bool authenticated) {
        (void)connectionHandle;
        (void)authenticated;
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
     * Provide access to the callchain of shutdown event callbacks.
     * It is possible to register callbacks using onShutdown().add(callback).
     * It is possible to unregister callbacks using onShutdown().detach(callback).
     *
     * @return The shutdown event callbacks chain
     */
    SecurityManagerShutdownCallbackChain_t& onShutdown() {
        return shutdownCallChain;
    }

    /**
     * Assign the event handler implementation that will be used by the stack to signal events
     * back to the application.
     *
     * @param[in] handler Event Handler interface implementation.
     */
    virtual void setSecurityManagerEventHandler(EventHandler* handler) {
        if (handler) {
            eventHandler = handler;
        } else {
            eventHandler = &defaultEventHandler;
        }
    }

protected:
    SecurityManager() {
        eventHandler = &defaultEventHandler;
    }

    virtual ~SecurityManager() { };

public:
    /**
     * @deprecated use generateWhitelistFromBondTable instead
     *
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

    /**
     * @deprecated
     *
     * Get the security status of a connection.
     *
     * @param[in]  connectionHandle Handle to identify the connection.
     * @param[out] securityStatus   Security status.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
     ble_error_t getLinkSecurity(ble::connection_handle_t connectionHandle, LinkSecurityStatus_t *securityStatus) {
        ble::link_encryption_t encryption(ble::link_encryption_t::NOT_ENCRYPTED);
        ble_error_t err = getLinkEncryption(connectionHandle, &encryption);
        if (err) {
            return err;
        }

        switch (encryption.value()) {
            case ble::link_encryption_t::NOT_ENCRYPTED:
                *securityStatus = NOT_ENCRYPTED;
                break;
            case ble::link_encryption_t::ENCRYPTION_IN_PROGRESS:
                *securityStatus = ENCRYPTION_IN_PROGRESS;
                break;
            case ble::link_encryption_t::ENCRYPTED:
            case ble::link_encryption_t::ENCRYPTED_WITH_MITM:
            case ble::link_encryption_t::ENCRYPTED_WITH_SC_AND_MITM:
                *securityStatus = ENCRYPTED;
                break;
            default:
                // should never happen
                MBED_ASSERT(false);
                *securityStatus = NOT_ENCRYPTED;
                break;
        }

        return BLE_ERROR_NONE;
    }

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
    void processSecuritySetupInitiatedEvent(ble::connection_handle_t connectionHandle, bool allowBonding, bool requireMITM, SecurityIOCapabilities_t iocaps) {
        if (defaultEventHandler.securitySetupInitiatedCallback) {
            defaultEventHandler.securitySetupInitiatedCallback(connectionHandle, allowBonding, requireMITM, iocaps);
        }
    }
    /** @deprecated */
    void processSecuritySetupCompletedEvent(ble::connection_handle_t connectionHandle, SecurityCompletionStatus_t status) {
        eventHandler->pairingResult(connectionHandle, status);
    }
    /** @deprecated */
    void processLinkSecuredEvent(ble::connection_handle_t connectionHandle, SecurityMode_t securityMode) {
        if (securityMode == SECURITY_MODE_ENCRYPTION_NO_MITM) {
            eventHandler->linkEncryptionResult(connectionHandle, ble::link_encryption_t::ENCRYPTED);
        } else {
            eventHandler->linkEncryptionResult(connectionHandle, ble::link_encryption_t::NOT_ENCRYPTED);
        }
    }
    /** @deprecated */
    void processSecurityContextStoredEvent(ble::connection_handle_t connectionHandle) {
        if (defaultEventHandler.securityContextStoredCallback) {
            defaultEventHandler.securityContextStoredCallback(connectionHandle);
        }
    }
    /** @deprecated */
    void processPasskeyDisplayEvent(ble::connection_handle_t connectionHandle, const Passkey_t passkey) {
        eventHandler->passkeyDisplay(connectionHandle, passkey);
    }

private:
    /* Legacy compatibility with old callbacks (from both sides so any
     * combination of new and old works) */
    class LegacyEventHandler : public EventHandler {
    public:
        LegacyEventHandler() :
            securitySetupInitiatedCallback(),
            securitySetupCompletedCallback(),
            linkSecuredCallback(),
            securityContextStoredCallback(),
            passkeyDisplayCallback() { };

        virtual void pairingResult(ble::connection_handle_t connectionHandle, SecurityCompletionStatus_t result) {
            if (securitySetupCompletedCallback) {
                securitySetupCompletedCallback(connectionHandle, result);
            }
        }

        virtual void linkEncryptionResult(ble::connection_handle_t connectionHandle, ble::link_encryption_t result) {
            if (linkSecuredCallback) {
                SecurityManager::SecurityMode_t securityMode;
                if (result == ble::link_encryption_t::ENCRYPTED) {
                    securityMode = SECURITY_MODE_ENCRYPTION_NO_MITM;
                } else if (
                    result == ble::link_encryption_t::ENCRYPTED_WITH_MITM ||
                    result == ble::link_encryption_t::ENCRYPTED_WITH_SC_AND_MITM
                ) {
                    securityMode = SECURITY_MODE_ENCRYPTION_WITH_MITM;
                } else {
                    securityMode = SECURITY_MODE_ENCRYPTION_OPEN_LINK;
                }
                linkSecuredCallback(connectionHandle, securityMode);
            }
        };

        virtual void passkeyDisplay(ble::connection_handle_t connectionHandle, const SecurityManager::Passkey_t passkey) {
            if (passkeyDisplayCallback) {
                passkeyDisplayCallback(connectionHandle, passkey);
            }
        };

        SecurityManager::SecuritySetupInitiatedCallback_t securitySetupInitiatedCallback;
        SecurityManager::SecuritySetupCompletedCallback_t securitySetupCompletedCallback;
        SecurityManager::LinkSecuredCallback_t            linkSecuredCallback;
        SecurityManager::HandleSpecificEvent_t            securityContextStoredCallback;
        SecurityManager::PasskeyDisplayCallback_t         passkeyDisplayCallback;
    };

private:
    SecurityManagerShutdownCallbackChain_t shutdownCallChain;

protected:
    EventHandler*      eventHandler;
    LegacyEventHandler defaultEventHandler;
};

#endif /*SECURITY_MANAGER_H_*/
