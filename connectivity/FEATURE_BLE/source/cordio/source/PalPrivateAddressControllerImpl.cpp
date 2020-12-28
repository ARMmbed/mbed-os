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

namespace ble {
namespace impl {

ble_error_t PalPrivateAddressController::initialize()
{
    DmPrivInit();
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::terminate()
{
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::generate_resolvable_private_address(const irk_t& local_irk)
{
    if (_generating_rpa) {
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
    return address;
}

ble_error_t PalPrivateAddressController::resolve_private_address(
    const address_t &address,
    const irk_t& irk
)
{
    if (_resolving_rpa) {
        return BLE_ERROR_INVALID_STATE;
    }

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
    DmPrivSetAddrResEnable(enable);
    return BLE_ERROR_NONE;
}

ble_error_t PalPrivateAddressController::set_ll_resolvable_private_address_timeout(
    resolvable_address_timeout_t timeout
)
{
    if (HciLlPrivacySupported() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
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
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
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
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
    if (msg == nullptr) {
        return false;
    }

    auto* handler = instance()._event_handler;

    switch (msg->event) {
        case DM_PRIV_GENERATE_ADDR_IND: {
            instance()._generating_rpa = false;

            if (!handler) {
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
            if (!handler) {
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
