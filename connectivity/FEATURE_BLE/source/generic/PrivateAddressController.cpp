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

#include "PrivateAddressController.h"
#include "mbed-trace/mbed_trace.h"
#include "common/ble_trace_helpers.h"

#define TRACE_GROUP "BLSM"

namespace ble {

PrivateAddressController::PrivateAddressController(
    PalPrivateAddressController &address_generator,
    PalEventQueue &event_queue,
    ble::resolvable_address_timeout_t rotation_timeout
) :
    _pal(address_generator) ,
    _event_queue(event_queue),
    _rotation_timeout(rotation_timeout)
{
    _pal.initialize();
    _pal.set_event_handler(this);

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    auto** next = &_free_resolution_entries;
    for (auto &entry : _resolution_list) {
        *next = &entry;
        next = &entry.next;
    }
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
}

PrivateAddressController::~PrivateAddressController()
{
    clear_privacy_control_blocks();
    _pal.terminate();
}

void PrivateAddressController::set_local_irk(const irk_t &local_irk)
{
    tr_info("Set local irk: %s", to_string(local_irk));
    _local_irk = local_irk;
    generate_resolvable_address();
}

void PrivateAddressController::set_timeout(ble::resolvable_address_timeout_t rotation_timeout)
{
    tr_info("Set resolvable address timeout: %d s", rotation_timeout.value());
    _rotation_timeout = rotation_timeout;
    _pal.set_ll_resolvable_private_address_timeout(rotation_timeout);
    if (_generation_started) {
        stop_private_address_generation();
        start_private_address_generation();
    }
}

const address_t& PrivateAddressController::get_resolvable_private_address()
{
    return _resolvable_address;
}

const address_t& PrivateAddressController::get_non_resolvable_private_address()
{
    return _non_resolvable_address;
}

void PrivateAddressController::set_event_handler(EventHandler *handler)
{
    _event_handler = handler;
}

void PrivateAddressController::start_private_address_generation()
{
    if (_generation_started) {
        return;
    }

    tr_info("Start private address generation");

    // non resolvable private address generation has been delayed until now,
    // generate it.
    generate_non_resolvable_address();

    _address_rotation_ticker.attach([this] {
        _event_queue.post([this]{
            tr_info("Private address timeout");
            generate_resolvable_address();
            generate_non_resolvable_address();
        });
    }, _rotation_timeout.valueChrono());
    _generation_started = true;
}

void PrivateAddressController::stop_private_address_generation()
{
    if (_generation_started) {
        tr_info("Stop private address generation");
        _address_rotation_ticker.detach();
        _generation_started = false;
    }
}

void PrivateAddressController::generate_resolvable_address()
{
    if (_local_irk != irk_t{}) {
        _pal.generate_resolvable_private_address(_local_irk);
    } else {
        tr_warning("Failed to generate resolvable address, no local irk");
    }
}

void PrivateAddressController::on_resolvable_private_address_generated(const address_t &rpa)
{
    tr_info("Resolvable private address generated: %s", to_string(rpa));
    _resolvable_address = rpa;
    if (_event_handler) {
        _event_handler->on_resolvable_private_addresses_generated(_resolvable_address);
    } else {
        tr_warning("No app handler to receive RPA generated");
    }
}

void PrivateAddressController::generate_non_resolvable_address()
{
    _non_resolvable_address = _pal.generate_non_resolvable_private_address();
    _event_queue.post([this] {
        if (_event_handler) {
            _event_handler->on_non_resolvable_private_addresses_generated(_non_resolvable_address);
        } else {
            tr_warning("No app handler to receive NRPA generated");
        }
    });
}

bool PrivateAddressController::is_controller_privacy_supported()
{
    return _pal.is_ll_privacy_supported();
}

#if !BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
ble_error_t PrivateAddressController::enable_controller_address_resolution(bool enable)
{
    MBED_ASSERT(is_controller_privacy_supported());
    return _pal.set_ll_address_resolution(enable);
}
#endif // !BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

uint8_t PrivateAddressController::read_resolving_list_capacity()
{
#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    return BLE_SECURITY_DATABASE_MAX_ENTRIES;
#else
    if (is_controller_privacy_supported()) {
        return _pal.read_resolving_list_capacity();
    } else {
        return 0;
    }
#endif //BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
}

uint8_t PrivateAddressController::read_resolving_list_size()
{
    return _resolving_list_size;
}

ble_error_t PrivateAddressController::add_device_to_resolving_list(
    target_peer_address_type_t peer_address_type,
    const address_t &peer_identity_address,
    const irk_t& peer_irk
)
{
    tr_info("Add device to resolving list: peer address=%s, type=%s, peer irk=%s",
        to_string(peer_identity_address), to_string(peer_address_type), to_string(peer_irk));
    if (_local_irk == irk_t{}) {
        tr_error("Invalid local IRK: %s", to_string(_local_irk));
        return BLE_ERROR_INVALID_STATE;
    }

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    // ensure an entry is not added twice
    for (auto &entry : _resolving_list) {
        if (entry.populated &&
            entry.peer_address_type == peer_address_type &&
            entry.peer_address == peer_identity_address &&
            entry.peer_irk == peer_irk
        ) {
            return BLE_ERROR_NONE;
        }
    }

    bool entry_added = false;
    for (auto &entry : _resolving_list) {
        if (entry.populated == false) {
            entry.peer_address_type = peer_address_type;
            entry.peer_address = peer_identity_address;
            entry.peer_irk = peer_irk;
            entry.populated = true;
            entry_added = true;
            _resolving_list_size++;
            break;
        }
    }

    if (!entry_added) {
        tr_error("Failed to add address into host resolving list, not enough space");
        return BLE_ERROR_NO_MEM;
    }


    // Remove unresolved entries from the resolved list
    remove_resolution_entry_from_cache(
        [&](resolution_entry_t &entry) {
            return entry.identity == nullptr;
        }
    );

    // reset pending resolution request
    restart_resolution_process_on_host();

    return BLE_ERROR_NO_MEM;
#else
    if (is_controller_privacy_supported()) {
        return queue_add_device_to_resolving_list(
            peer_address_type,
            peer_identity_address,
            peer_irk
        );
    } else {
        tr_error("Privacy used but not supported on the controller nor the host");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
}

ble_error_t PrivateAddressController::remove_device_from_resolving_list(
    target_peer_address_type_t peer_address_type,
    const address_t &peer_identity_address
)
{
    tr_info("Remove device from resolving list: peer address=%s, type=%s",
        to_string(peer_identity_address), to_string(peer_address_type));
#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    for (auto &entry : _resolving_list) {
        if (entry.populated &&
            entry.peer_address_type == peer_address_type &&
            entry.peer_address == peer_identity_address
        ) {
            remove_resolution_entry_from_cache([&](resolution_entry_t& cache_entry) {
                return cache_entry.identity == &entry;
            });

            entry.populated = false;
            _resolving_list_size--;

            restart_resolution_process_on_host();
        }
    }
    return BLE_ERROR_NONE;
#else
    if (is_controller_privacy_supported()) {
        return queue_remove_device_from_resolving_list(peer_address_type, peer_identity_address);
    } else {
        tr_error("Privacy used but not supported on the controller nor the host");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
}

ble_error_t PrivateAddressController::clear_resolving_list()
{
    tr_info("Clear resolving list");
#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    // Remove entry from the resolving list
    for (auto &entry : _resolving_list) {
        entry.populated = false;
    }
    // Remove all resolved entries from the cache
    remove_resolution_entry_from_cache([&](resolution_entry_t& entry) {
        return entry.identity != nullptr;
    });
    _resolving_list_size = 0;

    restart_resolution_process_on_host();

    return BLE_ERROR_NONE;
#else
    if (is_controller_privacy_supported()) {
        return queue_clear_resolving_list();
    } else {
        tr_error("Privacy used but not supported on the controller nor the host");
        return BLE_ERROR_NOT_IMPLEMENTED;
    }
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
}

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

bool PrivateAddressController::resolve_address_in_host_cache(
    const address_t &peer_address,
    target_peer_address_type_t *retrieved_address_type,
    const address_t **retrieved_address
)
{
    // An LRU cache is used, we first traverse the list of resolved address
    // and return any match
    auto *entry = _resolved_list;
    decltype(entry) previous = nullptr;
    while (entry) {
        if (entry->address == peer_address) {
            // The list contains resolved addresses AND unresolved addresses.
            // Fill input parameters accordingly
            if (entry->identity) {
                *retrieved_address = &entry->identity->peer_address;
                *retrieved_address_type = entry->identity->peer_address_type;
            } else {
                *retrieved_address = nullptr;
            }
            // update cache if we're not at the first entry
            if (previous) {
                previous->next = entry->next;
                entry->next = _resolved_list;
                _resolved_list = entry;
            }
            tr_debug("Resolved address from cache: rpa=%s, resolved address=%s, type=%s",
                to_string(peer_address), to_string(entry->identity->peer_address), to_string(entry->identity->peer_address_type));
            return true;
        }
        previous = entry;
        entry = entry->next;
    }

    tr_debug("Address not found in cache: %s", to_string(peer_address));
    return false;
}

ble_error_t PrivateAddressController::resolve_address_on_host(
    const address_t &peer_address,
    bool *resolution_complete,
    target_peer_address_type_t *retrieved_address_type,
    const address_t **retrieved_address
)
{
    *resolution_complete = resolve_address_in_host_cache(peer_address, retrieved_address_type, retrieved_address);

    // In the case the address has not been resolved, we start the resolution
    // process.
    if (*resolution_complete) {
        return BLE_ERROR_NONE;
    } else {
        return queue_resolve_address_on_host(peer_address);
    }
}
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

void PrivateAddressController::on_resolving_list_action_complete()
{
    tr_info("Resolving list action completed");
    process_privacy_control_blocks(true);
}

struct PrivateAddressController::PrivacyControlBlock {
    PrivacyControlBlock() : _next(nullptr)
    {
    }

    virtual ~PrivacyControlBlock() = default;

    // return trie if the handler has completed and false otherwise.
    virtual bool execute(PrivateAddressController& self) = 0;

    void set_next(PrivacyControlBlock *next)
    {
        _next = next;
    }

    PrivacyControlBlock *next() const
    {
        return _next;
    }

private:
    PrivacyControlBlock *_next;
};

#if !BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
struct PrivateAddressController::PrivacyAddDevToResListControlBlock final :
    PrivateAddressController::PrivacyControlBlock {
    PrivacyAddDevToResListControlBlock(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address,
        const irk_t &peer_irk
    ) : PrivacyControlBlock(),
        _peer_identity_address_type(peer_identity_address_type),
        _peer_identity_address(peer_identity_address),
        _peer_irk(peer_irk)
    {
    }

    bool execute(PrivateAddressController& self) final
    {
        // Execute command
        self._pal.add_device_to_resolving_list(
            _peer_identity_address_type,
            _peer_identity_address,
            _peer_irk,
            self._local_irk
        );
        self._resolving_list_size++;
        return false;
    }

private:
    advertising_peer_address_type_t _peer_identity_address_type;
    address_t _peer_identity_address;
    irk_t _peer_irk;
};

ble_error_t PrivateAddressController::queue_add_device_to_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    const address_t &peer_identity_address,
    const irk_t &peer_irk
)
{
    auto *cb = new(std::nothrow) PrivacyAddDevToResListControlBlock(
        peer_identity_address_type,
        peer_identity_address,
        peer_irk
    );
    if (cb == nullptr) {
        tr_error("Failed to create control block to add device to resolving list");
        // Cannot go further
        return BLE_ERROR_NO_MEM;
    }

    queue_privacy_control_block(cb);

    return BLE_ERROR_NONE;
}

struct PrivateAddressController::PrivacyRemoveDevFromResListControlBlock final :
    PrivateAddressController::PrivacyControlBlock {
    PrivacyRemoveDevFromResListControlBlock(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    ) : PrivacyControlBlock(),
        _peer_identity_address_type(peer_identity_address_type),
        _peer_identity_address(peer_identity_address)
    {
    }

    bool execute(PrivateAddressController& self) final
    {
        // Execute command
        self._pal.remove_device_from_resolving_list(
            _peer_identity_address_type,
            _peer_identity_address
        );
        self._resolving_list_size--;
        return false;
    }

private:
    advertising_peer_address_type_t _peer_identity_address_type;
    address_t _peer_identity_address;
};

ble_error_t PrivateAddressController::queue_remove_device_from_resolving_list(
    advertising_peer_address_type_t peer_identity_address_type,
    const address_t &peer_identity_address
)
{
    auto *cb = new(std::nothrow) PrivacyRemoveDevFromResListControlBlock(
        peer_identity_address_type,
        peer_identity_address
    );
    if (cb == nullptr) {
        tr_error("Failed to create control block to remove device from resolving list");
        // Cannot go further
        return BLE_ERROR_NO_MEM;
    }

    queue_privacy_control_block(cb);
    return BLE_ERROR_NONE;
}

struct PrivateAddressController::PrivacyClearResListControlBlock final :
    PrivateAddressController::PrivacyControlBlock {
    PrivacyClearResListControlBlock() : PrivacyControlBlock()
    {
    }

    bool execute(PrivateAddressController& self) final
    {
        // Execute command
        self._pal.clear_resolving_list();
        self._resolving_list_size = 0;
        return false;
    }
};

ble_error_t PrivateAddressController::queue_clear_resolving_list()
{
    // Remove any pending control blocks, there's no point executing them as we're about to queue the list
    clear_privacy_control_blocks();

    auto *cb = new(std::nothrow) PrivacyClearResListControlBlock();
    if (cb == nullptr) {
        tr_error("Failed to create control block to clear resolving list");
        // Cannot go further
        return BLE_ERROR_NO_MEM;
    }

    queue_privacy_control_block(cb);
    return BLE_ERROR_NONE;
}
#endif // !BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
struct PrivateAddressController::PrivacyResolveAddressOnHost final :
    PrivateAddressController::PrivacyControlBlock {
    PrivacyResolveAddressOnHost(const address_t &peer_address) :
        PrivacyControlBlock(),
        peer_address(peer_address)
    {
    }

    bool execute(PrivateAddressController& self) final
    {
        if (!self._resolving_list[resolving_list_index].populated) {
            // no entry at index 0, move to the next
            return start_next_resolution_round(self);
        } else {
            // start the resolution process with the first entry on the list
            start_resolution(self);
            return false;
        }
    }

    bool on_resolution_complete(PrivateAddressController& self, bool resolved)
    {
        if (require_restart) {
            require_restart = false;
            resolving_list_index = 0;
            return execute(self);
        } else if (resolved) {
            notify_completion(
                self, peer_address, resolved,
                &self._resolving_list[resolving_list_index]
            );
            return true;
        } else {
            return start_next_resolution_round(self);
        }
    }

    void invalidate() {
        require_restart = true;
    }

private:
    bool start_next_resolution_round(PrivateAddressController& self) {
        do {
            ++resolving_list_index;
            if (resolving_list_index == BLE_SECURITY_DATABASE_MAX_ENTRIES) {
                notify_completion(self, peer_address,false,nullptr);
                return true;
            }
        } while (!self._resolving_list[resolving_list_index].populated);

        tr_debug("Start resolution with next identity entry: %s", to_string(peer_address));
        start_resolution(self);
        return false;
    }

    void start_resolution(PrivateAddressController& self) {
        self._pal.resolve_private_address(
            peer_address,
            self._resolving_list[resolving_list_index].peer_irk
        );
    }

    void notify_completion(
        PrivateAddressController& self,
        const address_t &peer_resolvable_address,
        bool resolved,
        resolving_list_entry_t* identity
    )
    {
        if (!resolved) {
            tr_debug("Address resolution complete: rpa=%s, resolved=%s", to_string(peer_resolvable_address), to_string(resolved));
        } else {
            tr_debug("Address resolution complete: rpa=%s, resolved=%s, identity address=%s, type=%s",
                to_string(peer_resolvable_address), to_string(resolved), to_string(identity->peer_address), to_string(identity->peer_address_type));
        }

        // First we had the device to the resolution list
        self.add_resolution_entry_to_cache(peer_resolvable_address, identity);

        if (!self._event_handler) {
            return;
        }
        self._event_handler->on_address_resolution_completed(
            peer_resolvable_address,
            resolved,
            identity ? identity->peer_address_type : target_peer_address_type_t::PUBLIC,
            identity ? identity->peer_address : address_t{}
        );
    }

    size_t resolving_list_index = 0;
    address_t peer_address;
    bool require_restart = false;
};

ble_error_t PrivateAddressController::queue_resolve_address_on_host(const address_t &peer_address)
{
    auto *cb = new(std::nothrow) PrivacyResolveAddressOnHost(peer_address);
    if (cb == nullptr) {
        // Cannot go further
        tr_error("Not enough memory to queue host address resolution: %s", to_string(peer_address));
        return BLE_ERROR_NO_MEM;
    }

    _event_queue.post([this, cb] {
        queue_privacy_control_block(cb);
    });

    return BLE_ERROR_NONE;
}

void PrivateAddressController::restart_resolution_process_on_host()
{
    // if processing is active, restart the one running.
    if (_processing_privacy_control_block && _pending_privacy_control_blocks) {
        tr_info("Restart host address resolution process");
        static_cast<PrivacyResolveAddressOnHost*>(_pending_privacy_control_blocks)->invalidate();
    }
}

void PrivateAddressController::on_private_address_resolved(bool success)
{
    if (_pending_privacy_control_blocks == nullptr ||
        _processing_privacy_control_block == false
        ) {
        tr_warning("Received unexpected address resolution event");
        return;
    }

    auto* cb = static_cast<PrivacyResolveAddressOnHost*>(_pending_privacy_control_blocks);
    bool completed = cb->on_resolution_complete(*this, success);
    process_privacy_control_blocks(completed);
}
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

void PrivateAddressController::clear_privacy_control_blocks()
{
    tr_info("Clear privacy control blocks");
    while (_pending_privacy_control_blocks != nullptr) {
        PrivacyControlBlock *next = _pending_privacy_control_blocks->next();
        delete _pending_privacy_control_blocks;
        _pending_privacy_control_blocks = next;
    }
}

void PrivateAddressController::queue_privacy_control_block(PrivacyControlBlock *block)
{
    tr_info("Queue privacy request");
    if (_pending_privacy_control_blocks == nullptr) {
        _pending_privacy_control_blocks = block;
    } else {
        PrivacyControlBlock *node = _pending_privacy_control_blocks;
        while (node->next() != nullptr) {
            node = node->next();
        }
        node->set_next(block);
    }

    process_privacy_control_blocks(false);
}

// If cb_completed is set to true, it means the previous control block has completed

void PrivateAddressController::process_privacy_control_blocks(bool cb_completed)
{
    if ((_processing_privacy_control_block == true) && !cb_completed) {
        // Busy, cannot process next control block for now
        return;
    }

    if (cb_completed && _pending_privacy_control_blocks) {
        auto *next = _pending_privacy_control_blocks->next();
        delete _pending_privacy_control_blocks;
        _pending_privacy_control_blocks = next;
    }

    auto *cb = _pending_privacy_control_blocks;
    if (cb == nullptr) {
        // All control blocks processed
        _processing_privacy_control_block = false;
        return;
    }

    while (cb) {
        bool completed = cb->execute(*this);
        if (completed) {
            auto *next = cb->next();
            delete cb;
            cb = next;
        } else {
            _processing_privacy_control_block = true;
            break;
        }
    }

    _pending_privacy_control_blocks = cb;
}

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

template<typename Pred>
void PrivateAddressController::remove_resolution_entry_from_cache(const Pred &predicate)
{
    resolution_entry_t* previous = nullptr;
    resolution_entry_t* entry = _resolved_list;
    while (entry) {
        if (predicate(*entry)) {
            if (previous) {
                previous->next = entry->next;
            } else {
                _resolved_list = entry->next;
            }
            entry->next = _free_resolution_entries;
            _free_resolution_entries = entry;
            if (previous) {
                entry = previous->next;
            } else {
                entry = _resolved_list;
            }
        } else {
            previous = entry;
            entry = entry->next;
        }
    }
}

void PrivateAddressController::add_resolution_entry_to_cache(
    const address_t& address, resolving_list_entry_t* identity
)
{
    resolution_entry_t* entry = nullptr;
    if (_free_resolution_entries) {
        entry = _free_resolution_entries;
        _free_resolution_entries = entry->next;
    } else {
        // retrieve the last entry in the resolved list
        entry = _resolved_list;
        resolution_entry_t *previous = nullptr;
        while (entry->next) {
            previous = entry;
            entry = entry->next;
        }
        previous->next = nullptr;
    }

    // Update the entry and insert it at the top of the list
    entry->address = address;
    entry->identity = identity;
    entry->next = _resolved_list;
    _resolved_list = entry;
}

#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

} // namespace ble

#endif // BLE_FEATURE_PRIVACY
