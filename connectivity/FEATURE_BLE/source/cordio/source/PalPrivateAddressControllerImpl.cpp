/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#if BLE_FEATURE_PRIVACY

#include "PalPrivateAddressControllerImpl.h"
#include "dm_api.h"
#include "mbed-trace/mbed_trace.h"
#include "common/ble_trace_helpers.h"

#define TRACE_GROUP "BLPR"

namespace ble {
namespace impl {

ble_error_t PalPrivateAddressController::initialize()
{
    tr_info("Initialize privacy PAL");
    DmPrivInit();
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::terminate()
{
    tr_info("Terminate privacy PAL");
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::generate_resolvable_private_address(const irk_t& local_irk)
{
    tr_info("PAL start generation of RPA from local irk: %s", to_string(local_irk));
    if (_generating_rpa) {
        tr_error("PAL can't generate RPA, it is busy handling a previous request");
        return BLE_ERROR_INVALID_STATE;
    }

    DmPrivGenerateAddr(const_cast<uint8_t*>(local_irk.data()), 0);
    _generating_rpa = true;
    return BLE_ERROR_NONE;
}

address_t PalPrivateAddressController::generate_non_resolvable_private_address()
{
    address_t address;
    SecRand(address.data(), address.size());
    DM_RAND_ADDR_SET(address, DM_RAND_ADDR_NONRESOLV);
    tr_info("Non resolvable private address generated: %s", to_string(address));
    return address;
}

ble_error_t PalPrivateAddressController::resolve_private_address(
    const address_t &address,
    const irk_t& irk
)
{
    if (_resolving_rpa) {
        tr_error("Failed to start resolution of RPA: Handling a previous request");
        return BLE_ERROR_INVALID_STATE;
    }

    tr_debug("Start resolution of private address: address=%s, irk=%s", to_string(address), to_string(irk));
    DmPrivResolveAddr(
        const_cast<uint8_t*>(address.data()),
        const_cast<uint8_t*>(irk.data()),
        0
    );
    return BLE_ERROR_NONE;
}

bool PalPrivateAddressController::is_ll_privacy_supported()
{
    return HciLlPrivacySupported();
}

ble_error_t PalPrivateAddressController::set_ll_address_resolution(bool enable)
{
    tr_info("Enable LL private address resolution");
    DmPrivSetAddrResEnable(enable);
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::set_ll_resolvable_private_address_timeout(
    resolvable_address_timeout_t timeout
)
{
    if (HciLlPrivacySupported() == false) {
        tr_error("Operation not supported by LL: RPA generation");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    tr_info("Set LL resolvable private address generation timeout: %d s", timeout.value());
    DmPrivSetResolvablePrivateAddrTimeout(timeout.value());
    return BLE_ERROR_NONE;
}

uint8_t PalPrivateAddressController::read_resolving_list_capacity()
{
    return HciGetResolvingListSize();
}

ble_error_t PalPrivateAddressController::add_device_to_resolving_list(
    target_peer_address_type_t peer_address_type,
    const address_t &peer_identity_address,
    const irk_t& peer_irk,
    const irk_t& local_irk
)
{
    if (is_ll_privacy_supported() == false) {
        tr_error("Operation not supported by LL: RPA resolution");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    tr_info("Add RPA to LL resolving list: peer address=%s, type=%s, peer irk=%s, local irk=%s",
        to_string(peer_identity_address), to_string(peer_address_type), to_string(peer_irk), to_string(local_irk));
    DmPrivAddDevToResList(
        peer_address_type.value(),
        peer_identity_address.data(),
        const_cast<uint8_t*>(peer_irk.data()),
        const_cast<uint8_t*>(local_irk.data()),
        false,
        0
    );
    DmPrivSetPrivacyMode(
        peer_address_type.value(),
        peer_identity_address.data(),
        DM_PRIV_MODE_DEVICE
    );
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::remove_device_from_resolving_list(
    target_peer_address_type_t peer_address_type,
    const address_t &peer_identity_address
)
{
    if (is_ll_privacy_supported() == false) {
        tr_error("Operation not supported by LL: RPA resolution");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    tr_info("Remove RPA from LL resolving list: peer address=%s, type=%s",
        to_string(peer_identity_address), to_string(peer_address_type));
    DmPrivRemDevFromResList(peer_address_type.value(), peer_identity_address.data(), 0);
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::set_peer_privacy_mode(
    target_peer_address_type_t peer_address_type,
    const address_t &peer_address,
    privacy_mode_t privacy_mode
)
{
    if (is_ll_privacy_supported() == false) {
        tr_error("Operation not supported by LL: privacy");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
    tr_info("Set privacy mode: peer address=%s, type=%s, mode=%s",
        to_string(peer_address), to_string(peer_address_type), to_string(privacy_mode));
    DmPrivSetPrivacyMode(
        peer_address_type.value(),
        peer_address.data(),
        privacy_mode.value()
    );
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::clear_resolving_list()
{
    if (is_ll_privacy_supported() == false) {
        tr_error("Operation not supported by LL: privacy resolving list");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    tr_info("Clear LL resolving list");
    DmPrivClearResList();
    return BLE_ERROR_NONE;
}

void PalPrivateAddressController::set_event_handler(EventHandler *handler)
{
    _event_handler = handler;
}

PalPrivateAddressController& PalPrivateAddressController::instance()
{
    static impl::PalPrivateAddressController self;
    return self;
}

bool PalPrivateAddressController::cordio_handler(const wsfMsgHdr_t *msg)
{
    MBED_ASSERT(msg);

    auto* handler = instance()._event_handler;

    switch (msg->event) {
        case DM_PRIV_GENERATE_ADDR_IND: {
            tr_info("Privacy handling: DM_PRIV_GENERATE_ADDR_IND");
            instance()._generating_rpa = false;

            if (!handler) {
                tr_warning("No user handler registered for PAL privacy");
                return true;
            }

            const auto *evt = (const dmPrivGenAddrIndEvt_t*) msg;
            if (evt->hdr.status == HCI_SUCCESS) {
                handler->on_resolvable_private_address_generated(evt->addr);
            } else {
                handler->on_resolvable_private_address_generated(address_t{});
            }
            return true;
        }

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
        case DM_PRIV_RESOLVED_ADDR_IND: {
            instance()._resolving_rpa = false;

            if (!handler) {
                return true;
            }

            handler->on_private_address_resolved(msg->status == HCI_SUCCESS);
            return true;
        }
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

        case DM_PRIV_ADD_DEV_TO_RES_LIST_IND: // Device added to resolving list
        case DM_PRIV_REM_DEV_FROM_RES_LIST_IND: // Device removed from resolving list
        case DM_PRIV_CLEAR_RES_LIST_IND: // Resolving list cleared
        {
            tr_info("Privacy handling: %s",
                msg->event == DM_PRIV_ADD_DEV_TO_RES_LIST_IND ?
                    "DM_PRIV_ADD_DEV_TO_RES_LIST_IND" :
                    msg->event == DM_PRIV_ADD_DEV_TO_RES_LIST_IND ?
                        "DM_PRIV_REM_DEV_FROM_RES_LIST_IND" :
                        "DM_PRIV_CLEAR_RES_LIST_IND"
            );
            if (!handler) {
                tr_warning("No user handler registered for PAL privacy");
                return true;
            }

            // Previous command completed, we can move to the next control block
            handler->on_resolving_list_action_complete();
            return true;
        }

        default:
            return false;
    }
}

} // namespace impl
} // namespace ble

#endif // BLE_FEATURE_PRIVACY
