/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#if defined(S110)
#include "btle.h"

#include "nRF5xn.h"

extern "C" {
#include "pstorage.h"
#include "device_manager.h"
#include "id_manager.h"
}

#include "btle_security.h"

static dm_application_instance_t applicationInstance;
static bool                      initialized = false;
static ret_code_t dm_handler(dm_handle_t const *p_handle, dm_event_t const *p_event, ret_code_t event_result);

// default security parameters. Avoid "holes" between member assigments in order to compile by gcc++11.
static ble_gap_sec_params_t securityParameters = {
    .bond          = true,         /**< Perform bonding. */
    .mitm          = true,         /**< Man In The Middle protection required. */
    .lesc          = false,        /**< Enable LE Secure Connection pairing. */
    .keypress      = false,        /**< Enable generation of keypress notifications. */
    .io_caps       = SecurityManager::IO_CAPS_NONE, /**< IO capabilities, see @ref BLE_GAP_IO_CAPS. */
    .oob           = 0,            /**< Out Of Band data available. */
    .min_key_size  = 16,           /**< Minimum encryption key size in octets between 7 and 16. If 0 then not applicable in this instance. */
    .max_key_size  = 16,           /**< Maximum encryption key size in octets between min_key_size and 16. */
    .kdist_own  = {
      .enc  = 0,                   /**< Long Term Key and Master Identification. */
      .id   = 0,                   /**< Identity Resolving Key and Identity Address Information. */
      .sign = 0,                   /**< Connection Signature Resolving Key. */
      .link = 0                    /**< Derive the Link Key from the LTK. */
    },                             /**< Key distribution bitmap: keys that the local device will distribute. */
    .kdist_peer  = {
      .enc  = 1,                   /**< Long Term Key and Master Identification. */
      .id   = 1,                   /**< Identity Resolving Key and Identity Address Information. */
      .sign = 1,                   /**< Connection Signature Resolving Key. */
      .link = 0                    /**< Derive the Link Key from the LTK. */
    }                              /**< Key distribution bitmap: keys that the peripheral device will distribute. */
};

bool
btle_hasInitializedSecurity(void)
{
    return initialized;
}

ble_error_t
btle_initializeSecurity(bool                                      enableBonding,
                        bool                                      requireMITM,
                        SecurityManager::SecurityIOCapabilities_t iocaps,
                        const SecurityManager::Passkey_t          passkey)
{
    /* guard against multiple initializations */
    if (initialized) {
        return BLE_ERROR_NONE;
    }

    if (pstorage_init() != NRF_SUCCESS) {
        return BLE_ERROR_UNSPECIFIED;
    }

    ret_code_t rc;
    if (passkey) {
        ble_opt_t opts;
        opts.gap_opt.passkey.p_passkey = const_cast<uint8_t *>(passkey);
        if ((rc = sd_ble_opt_set(BLE_GAP_OPT_PASSKEY, &opts)) != NRF_SUCCESS) {
            switch (rc) {
                case BLE_ERROR_INVALID_CONN_HANDLE:
                case NRF_ERROR_INVALID_ADDR:
                case NRF_ERROR_INVALID_PARAM:
                default:
                    return BLE_ERROR_INVALID_PARAM;
                case NRF_ERROR_INVALID_STATE:
                    return BLE_ERROR_INVALID_STATE;
                case NRF_ERROR_BUSY:
                    return BLE_STACK_BUSY;
            }
        }
    }

    dm_init_param_t dm_init_param = {
        .clear_persistent_data = false /* Set to true in case the module should clear all persistent data. */
    };
    if (dm_init(&dm_init_param) != NRF_SUCCESS) {
        return BLE_ERROR_UNSPECIFIED;
    }

    // update default security parameters with function call parameters
    securityParameters.bond = enableBonding;
    securityParameters.mitm = requireMITM;
    securityParameters.io_caps = iocaps;

    const dm_application_param_t dm_param = {
        .evt_handler  = dm_handler,
        .service_type = DM_PROTOCOL_CNTXT_GATT_CLI_ID,
        .sec_param    = securityParameters
    };

    if ((rc = dm_register(&applicationInstance, &dm_param)) != NRF_SUCCESS) {
        switch (rc) {
            case NRF_ERROR_INVALID_STATE:
                return BLE_ERROR_INVALID_STATE;
            case NRF_ERROR_NO_MEM:
                return BLE_ERROR_NO_MEM;
            default:
                return BLE_ERROR_UNSPECIFIED;
        }
    }

    initialized = true;
    return BLE_ERROR_NONE;
}

ble_error_t
btle_purgeAllBondingState(void)
{
    ret_code_t rc;
    if ((rc = dm_device_delete_all(&applicationInstance)) == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    }

    switch (rc) {
        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;
        case NRF_ERROR_NO_MEM:
            return BLE_ERROR_NO_MEM;
        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

ble_error_t
btle_getLinkSecurity(Gap::Handle_t connectionHandle, SecurityManager::LinkSecurityStatus_t *securityStatusP)
{
    ret_code_t rc;
    dm_handle_t dmHandle = {
        .appl_id = applicationInstance,
    };
    if ((rc = dm_handle_get(connectionHandle, &dmHandle)) != NRF_SUCCESS) {
        if (rc == NRF_ERROR_NOT_FOUND) {
            return BLE_ERROR_INVALID_PARAM;
        } else {
            return BLE_ERROR_UNSPECIFIED;
        }
    }

    if ((rc = dm_security_status_req(&dmHandle, reinterpret_cast<dm_security_status_t *>(securityStatusP))) != NRF_SUCCESS) {
        switch (rc) {
            case NRF_ERROR_INVALID_STATE:
                return BLE_ERROR_INVALID_STATE;
            case NRF_ERROR_NO_MEM:
                return BLE_ERROR_NO_MEM;
            default:
                return BLE_ERROR_UNSPECIFIED;
        }
    }

    return BLE_ERROR_NONE;
}

ble_error_t
btle_setLinkSecurity(Gap::Handle_t connectionHandle, SecurityManager::SecurityMode_t securityMode)
{
    // use default and updated parameters as starting point
    // and modify structure based on security mode.
    ble_gap_sec_params_t params = securityParameters;

    switch (securityMode) {
        case SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK:
            /**< Require no protection, open link. */
            securityParameters.bond = false;
            securityParameters.mitm = false;
            break;

        case SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM:
            /**< Require encryption, but no MITM protection. */
            securityParameters.bond = true;
            securityParameters.mitm = false;
            break;

        // not yet implemented security modes
        case SecurityManager::SECURITY_MODE_NO_ACCESS:
        case SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM:
            /**< Require encryption and MITM protection. */
        case SecurityManager::SECURITY_MODE_SIGNED_NO_MITM:
            /**< Require signing or encryption, but no MITM protection. */
        case SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM:
            /**< Require signing or encryption, and MITM protection. */
        default:
            return BLE_ERROR_NOT_IMPLEMENTED;
    }

    // update security settings for given connection
    uint32_t result = sd_ble_gap_authenticate(connectionHandle, &params);

    if (result == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_UNSPECIFIED;
    }
}

ret_code_t
dm_handler(dm_handle_t const *p_handle, dm_event_t const *p_event, ret_code_t event_result)
{
    nRF5xn               &ble             = nRF5xn::Instance(BLE::DEFAULT_INSTANCE);
    nRF5xSecurityManager &securityManager = (nRF5xSecurityManager &) ble.getSecurityManager();

    switch (p_event->event_id) {
        case DM_EVT_SECURITY_SETUP: /* started */ {
            const ble_gap_sec_params_t *peerParams = &p_event->event_param.p_gap_param->params.sec_params_request.peer_params;
            securityManager.processSecuritySetupInitiatedEvent(p_event->event_param.p_gap_param->conn_handle,
                                                                                   peerParams->bond,
                                                                                   peerParams->mitm,
                                                                                   (SecurityManager::SecurityIOCapabilities_t)peerParams->io_caps);
            break;
        }
        case DM_EVT_SECURITY_SETUP_COMPLETE:
            securityManager.
                processSecuritySetupCompletedEvent(p_event->event_param.p_gap_param->conn_handle,
                                                   (SecurityManager::SecurityCompletionStatus_t)(p_event->event_param.p_gap_param->params.auth_status.auth_status));
            break;
        case DM_EVT_LINK_SECURED: {
            unsigned securityMode                    = p_event->event_param.p_gap_param->params.conn_sec_update.conn_sec.sec_mode.sm;
            unsigned level                           = p_event->event_param.p_gap_param->params.conn_sec_update.conn_sec.sec_mode.lv;
            SecurityManager::SecurityMode_t resolvedSecurityMode = SecurityManager::SECURITY_MODE_NO_ACCESS;
            switch (securityMode) {
                case 1:
                    switch (level) {
                        case 1:
                            resolvedSecurityMode = SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK;
                            break;
                        case 2:
                            resolvedSecurityMode = SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM;
                            break;
                        case 3:
                            resolvedSecurityMode = SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM;
                            break;
                    }
                    break;
                case 2:
                    switch (level) {
                        case 1:
                            resolvedSecurityMode = SecurityManager::SECURITY_MODE_SIGNED_NO_MITM;
                            break;
                        case 2:
                            resolvedSecurityMode = SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM;
                            break;
                    }
                    break;
            }

            securityManager.processLinkSecuredEvent(p_event->event_param.p_gap_param->conn_handle, resolvedSecurityMode);
            break;
        }
        case DM_EVT_DEVICE_CONTEXT_STORED:
            securityManager.processSecurityContextStoredEvent(p_event->event_param.p_gap_param->conn_handle);
            break;
        default:
            break;
    }

    return NRF_SUCCESS;
}

ble_error_t
btle_createWhitelistFromBondTable(ble_gap_whitelist_t *p_whitelist)
{
    if (!btle_hasInitializedSecurity()) {
        return BLE_ERROR_INITIALIZATION_INCOMPLETE;
    }
    ret_code_t err = dm_whitelist_create(&applicationInstance, p_whitelist);
    if (err == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    } else if (err == NRF_ERROR_NULL) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    } else {
        return BLE_ERROR_INVALID_STATE;
    }
}


bool
btle_matchAddressAndIrk(ble_gap_addr_t const * p_addr, ble_gap_irk_t const * p_irk)
{
    /*
     * Use a helper function from the Nordic SDK to test whether the BLE
     * address can be generated using the IRK.
     */
    return im_address_resolve(p_addr, p_irk);
}

void
btle_generateResolvableAddress(const ble_gap_irk_t &irk, ble_gap_addr_t &address)
{
    /* Set type to resolvable */
    address.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE;

    /*
     * Assign a random number to the most significant 3 bytes
     * of the address.
     */
    address.addr[BLE_GAP_ADDR_LEN - 3] = 0x8E;
    address.addr[BLE_GAP_ADDR_LEN - 2] = 0x4F;
    address.addr[BLE_GAP_ADDR_LEN - 1] = 0x7C;

    /* Calculate the hash and store it in the top half of the address */
    ah(irk.irk, &address.addr[BLE_GAP_ADDR_LEN - 3], address.addr);
}
#endif
