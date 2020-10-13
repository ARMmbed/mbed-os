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

#ifndef BLE_CORDIO_PAL_PRIVATE_ADDRESS_CONTROLLER_IMPL_H
#define BLE_CORDIO_PAL_PRIVATE_ADDRESS_CONTROLLER_IMPL_H

#if BLE_FEATURE_PRIVACY

#include "source/pal/PalPrivateAddressController.h"
#include "dm_api.h"

namespace ble {
namespace impl {

/**
 * Cordio implementation of a ble::PalPrivateAddressController.
 */
struct PalPrivateAddressController final : public ble::PalPrivateAddressController {

    ble_error_t initialize() final;

    ble_error_t terminate() final;

    ble_error_t generate_resolvable_private_address(const irk_t& local_irk) final;

    address_t generate_non_resolvable_private_address() final;

    ble_error_t resolve_private_address(
        const address_t &address,
        const irk_t& irk
    ) final;

    bool is_ll_privacy_supported() final;

    ble_error_t set_ll_address_resolution(bool enable) final;

    ble_error_t set_ll_resolvable_private_address_timeout(resolvable_address_timeout_t timeout) final;

    uint8_t read_resolving_list_capacity() final;

    ble_error_t add_device_to_resolving_list(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address,
        const irk_t& peer_irk,
        const irk_t& local_irk
    ) final;

    ble_error_t remove_device_from_resolving_list(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    ) final;

    ble_error_t clear_resolving_list() final;

    ble_error_t set_peer_privacy_mode(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_address,
        privacy_mode_t privacy_mode
    ) final;

    void set_event_handler(EventHandler *handler) final;

    static PalPrivateAddressController& instance();

    /**
     * Callback which handle wsfMsgHdr_t and forward them to emit_gap_event.
     */
    static bool cordio_handler(const wsfMsgHdr_t *msg);

private:
    PalPrivateAddressController() = default;

    EventHandler *_event_handler = nullptr;
    bool _generating_rpa = false;
    bool _resolving_rpa = false;
};


} // namespace impl
} // namespace ble

#endif // BLE_FEATURE_PRIVACY

#endif //BLE_CORDIO_PAL_PRIVATE_ADDRESS_CONTROLLER_IMPL_H
