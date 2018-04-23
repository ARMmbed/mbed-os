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

#if defined(S130) || defined(S132) || defined(S140)
#include "btle.h"

#include "nRF5xn.h"

extern "C" {
#include "peer_manager.h"
#include "id_manager.h"
#include "fds.h"
}

#include "btle_security.h"

static bool                      initialized = false;

static void pm_handler(pm_evt_t const *p_event);
static bool _enc_in_progress = false; // helper flag for distinguish between state of link connected and link connected in progres of encryption establishing.
volatile static uint32_t async_ret_code; // busy loop support variable for asyncronous API.

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
      .sign = 0,                   /**< Connection Signature Resolving Key. */
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

    // update default security parameters with function call parameters
    securityParameters.bond    = enableBonding;
    securityParameters.mitm    = requireMITM;
    securityParameters.io_caps = iocaps;
    
    if (enableBonding) {
        securityParameters.kdist_own.enc = 1;
        securityParameters.kdist_own.id  = 1;
    } else {
        securityParameters.kdist_own.enc = 0;
        securityParameters.kdist_own.id  = 0;
    }
    rc = pm_sec_params_set(&securityParameters);

    if (rc == NRF_SUCCESS) {
        rc = pm_register(pm_handler);
    }

    switch (rc) {
        case NRF_SUCCESS:
            initialized = true;
            return BLE_ERROR_NONE;

        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;

        case NRF_ERROR_INVALID_PARAM:
            return BLE_ERROR_INVALID_PARAM;

        default:
            return BLE_ERROR_UNSPECIFIED;
    }


    initialized = true;
    return BLE_ERROR_NONE;
}

ble_error_t
btle_purgeAllBondingState(void)
{
    ret_code_t rc;
    
    async_ret_code = NRF_ERROR_BUSY;
    
    rc = pm_peers_delete(); // it is asynhronous API
    
    if (rc == NRF_SUCCESS)
    {
        // waiting for respond from pm_handler
        while (async_ret_code == NRF_ERROR_BUSY) {
            // busy-loop
        }
        
        rc = async_ret_code;
    }

    switch (rc) {
        case NRF_SUCCESS:
            return BLE_ERROR_NONE;
            
        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;
            
        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

ble_error_t
btle_getLinkSecurity(Gap::Handle_t connectionHandle, SecurityManager::LinkSecurityStatus_t *securityStatusP)
{
    ret_code_t 			 rc;
    pm_conn_sec_status_t conn_sec_status;

    rc = pm_conn_sec_status_get(connectionHandle, &conn_sec_status);

    if (rc == NRF_SUCCESS)
    {
    	if (conn_sec_status.encrypted) {
    		*securityStatusP = SecurityManager::ENCRYPTED;
    	}
    	else if (conn_sec_status.connected) {
    		if (_enc_in_progress) {
    			*securityStatusP = SecurityManager::ENCRYPTION_IN_PROGRESS;
    		}
    		else {
    			*securityStatusP = SecurityManager::NOT_ENCRYPTED;
    		}
    	}

        return BLE_ERROR_NONE;
    }

    switch (rc) {
        case BLE_ERROR_INVALID_CONN_HANDLE:
            return BLE_ERROR_INVALID_PARAM;

        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;

        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

ble_error_t
btle_setLinkSecurity(Gap::Handle_t connectionHandle, SecurityManager::SecurityMode_t securityMode)
{
    // use default and updated parameters as starting point
    // and modify structure based on security mode.
    ret_code_t rc;

    switch (securityMode) {
        case SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK:
            /**< Require no protection, open link. */
            securityParameters.bond = false;
            securityParameters.mitm = false;
            securityParameters.kdist_own.enc = 0;
            break;

        case SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM:
            /**< Require encryption, but no MITM protection. */
            securityParameters.bond = true;
            securityParameters.mitm = false;
            securityParameters.kdist_own.enc = 1;
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

    rc = pm_sec_params_set(&securityParameters);

    if (rc == NRF_SUCCESS) {
        rc = pm_conn_secure(connectionHandle, false);
    }

    switch (rc) {
        case NRF_SUCCESS:
            initialized = true;
            return BLE_ERROR_NONE;

        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;

        case NRF_ERROR_INVALID_PARAM:
            return BLE_ERROR_INVALID_PARAM;

        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}


void pm_handler(pm_evt_t const *p_event)
{
    nRF5xn               &ble             = nRF5xn::Instance(BLE::DEFAULT_INSTANCE);
    nRF5xSecurityManager &securityManager = (nRF5xSecurityManager &) ble.getSecurityManager();
    ret_code_t            err_code;
    SecurityManager::SecurityMode_t resolvedSecurityMode;

    switch (p_event->evt_id) {
        case PM_EVT_CONN_SEC_START: /* started */ {
            const ble_gap_sec_params_t *peerParams = &securityParameters;
            securityManager.processSecuritySetupInitiatedEvent(p_event->conn_handle,
                                                                                   peerParams->bond,
                                                                                   peerParams->mitm,
                                                                                   (SecurityManager::SecurityIOCapabilities_t)peerParams->io_caps);
            _enc_in_progress = true;
            break;
        }
        case PM_EVT_CONN_SEC_SUCCEEDED:
            // Update the rank of the peer.
            if (p_event->params.conn_sec_succeeded.procedure == PM_LINK_SECURED_PROCEDURE_BONDING)
            {
                err_code = pm_peer_rank_highest(p_event->peer_id);
            }
        
            securityManager.
                processSecuritySetupCompletedEvent(p_event->conn_handle,
                                                    SecurityManager::SEC_STATUS_SUCCESS);// SEC_STATUS_SUCCESS of SecurityCompletionStatus_t
                                                    
            ble_gap_conn_sec_t conn_sec;
            sd_ble_gap_conn_sec_get(p_event->conn_handle, &conn_sec);
            
            resolvedSecurityMode = SecurityManager::SECURITY_MODE_NO_ACCESS;
            
            switch (conn_sec.sec_mode.sm) {
                case 1:
                    switch (conn_sec.sec_mode.lv) {
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
                    switch (conn_sec.sec_mode.lv) {
                        case 1:
                            resolvedSecurityMode = SecurityManager::SECURITY_MODE_SIGNED_NO_MITM;
                            break;
                        case 2:
                            resolvedSecurityMode = SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM;
                            break;
                    }
                    break;
            }

            securityManager.processLinkSecuredEvent(p_event->conn_handle, resolvedSecurityMode);

            _enc_in_progress = false;
            break;
            
        case PM_EVT_CONN_SEC_FAILED:
            SecurityManager::SecurityCompletionStatus_t securityCompletionStatus;
            
            if ((uint32_t)p_event->params.conn_sec_failed.error >= PM_CONN_SEC_ERROR_BASE ) {
                securityCompletionStatus = SecurityManager::SEC_STATUS_UNSPECIFIED;
            } else {
                 securityCompletionStatus = 
                    (SecurityManager::SecurityCompletionStatus_t)p_event->params.conn_sec_failed.error;
            }
                
            securityManager.
                processSecuritySetupCompletedEvent(p_event->conn_handle, securityCompletionStatus);

            _enc_in_progress = false;
            break;
            
        case PM_EVT_BONDED_PEER_CONNECTED:
            pm_peer_rank_highest(p_event->peer_id);
            break;
        
        case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
            if (p_event->params.peer_data_update_succeeded.action == PM_PEER_DATA_OP_UPDATE)
            {
                securityManager.processSecurityContextStoredEvent(p_event->conn_handle);
            }
            break;
            
        case PM_EVT_PEER_DATA_UPDATE_FAILED:
            break;
            
        case PM_EVT_PEERS_DELETE_SUCCEEDED:
            async_ret_code = NRF_SUCCESS;        // respond SUCCESS to the busy-loop in f. btle_purgeAllBondingState
            break;
        
        case PM_EVT_PEERS_DELETE_FAILED:
            async_ret_code = NRF_ERROR_INTERNAL; // respond FAILURE to the busy-loop in f. btle_purgeAllBondingState
            break;

        case PM_EVT_STORAGE_FULL:
            // Run garbage collection on the flash.
            err_code = fds_gc();
            if (err_code == FDS_ERR_BUSY || err_code == FDS_ERR_NO_SPACE_IN_QUEUES)
            {
                // Retry.
            }
            else
            {
                APP_ERROR_CHECK(err_code);
            }
            break;//PM_EVT_STORAGE_FULL
            
        case PM_EVT_CONN_SEC_CONFIG_REQ:{
            // A connected peer (central) is trying to pair, but the Peer Manager already has a bond
            // for that peer. Setting allow_repairing to false rejects the pairing request.
            // If this event is ignored (pm_conn_sec_config_reply is not called in the event
            // handler), the Peer Manager assumes allow_repairing to be false.
            pm_conn_sec_config_t conn_sec_config = {.allow_repairing = true};
            pm_conn_sec_config_reply(p_event->conn_handle, &conn_sec_config);
            }
            break;//PM_EVT_CONN_SEC_CONFIG_REQ
            
        default:
            break;
    }
}

#if  (NRF_SD_BLE_API_VERSION <= 2)
ble_error_t
btle_createWhitelistFromBondTable(ble_gap_whitelist_t *p_whitelist)
{
    if (!btle_hasInitializedSecurity()) {
        return BLE_ERROR_INITIALIZATION_INCOMPLETE;
    }
    ret_code_t err = pm_whitelist_create( NULL, BLE_GAP_WHITELIST_ADDR_MAX_COUNT, p_whitelist);
    if (err == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    } else if (err == NRF_ERROR_NULL) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    } else {
        return BLE_ERROR_INVALID_STATE;
    }
}
#endif

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


#if (NRF_SD_BLE_API_VERSION >= 3)
ble_error_t btle_getAddressesFromBondTable(Gap::Whitelist_t &addrList)
{
    pm_peer_id_t           peer_id;
    ret_code_t             ret;
    pm_peer_data_bonding_t bond_data;

    addrList.size = 0;
    peer_id       = pm_next_peer_id_get(PM_PEER_ID_INVALID);

    /**
     * Fill addresses list:
     * Copy addresses from bond table, or
     * for every private resolvable address in the bond table generate the resolvable address.
     */
    while ((peer_id != PM_PEER_ID_INVALID) && (addrList.capacity > addrList.size)) {
        memset(&bond_data, 0x00, sizeof(bond_data));

        // Read peer data from flash.
        ret = pm_peer_data_bonding_load(peer_id, &bond_data);

        if ((ret == NRF_ERROR_NOT_FOUND) || (ret == NRF_ERROR_INVALID_PARAM)) {
            // Peer data could not be found in flash or peer ID is not valid.
            return BLE_ERROR_UNSPECIFIED;
        }

        if (bond_data.peer_ble_id.id_addr_info.addr_type == BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE) {
            btle_generateResolvableAddress(bond_data.peer_ble_id.id_info,
                (ble_gap_addr_t &) addrList.addresses[addrList.size].address);
        } else {
            memcpy(&addrList.addresses[addrList.size].address,
                   &bond_data.peer_ble_id.id_addr_info.addr,
                   sizeof(addrList.addresses[0].address));
        }

        addrList.addresses[addrList.size].type = static_cast<BLEProtocol::AddressType_t> (bond_data.peer_ble_id.id_addr_info.addr_type);

        addrList.size++;

        // get next peer  id
        peer_id = pm_next_peer_id_get(peer_id);
    }

    return BLE_ERROR_NONE;
}
#endif

#endif // defined(S130) || defined(S132) || defined(S140)
