/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include "EndpointResolver.h"

static uint32_t logical_to_index(uint32_t logical, bool in_not_out)
{
    return (logical << 1) | (in_not_out ? 1 : 0);
}

static uint32_t index_to_logical(uint32_t index)
{
    return index >> 1;
}


EndpointResolver::EndpointResolver(const usb_ep_table_t *table) : _table(table), _cost(0), _used(0), _valid(true)
{
    // Do nothing
}

EndpointResolver::~EndpointResolver()
{
    // Do nothing
}

void EndpointResolver::endpoint_ctrl(uint32_t size)
{
    endpoint_in(USB_EP_TYPE_CTRL, size);
    endpoint_out(USB_EP_TYPE_CTRL, size);
}

usb_ep_t EndpointResolver::next_free_endpoint(bool in_not_out, usb_ep_type_t type, uint32_t size)
{
    int index = next_index(type, in_not_out);
    if (index < 0) {
        _valid = false;
        return 0;
    }

    const usb_ep_entry_t &entry = _table->table[index_to_logical(index)];
    _cost += entry.base_cost + entry.byte_cost * size;
    _used |= 1 << index;

    return index_to_endpoint(index);

}
usb_ep_t EndpointResolver::endpoint_in(usb_ep_type_t type, uint32_t size)
{
    return next_free_endpoint(true, type, size);
}

usb_ep_t EndpointResolver::endpoint_out(usb_ep_type_t type, uint32_t size)
{
    return next_free_endpoint(false, type, size);
}

bool EndpointResolver::valid()
{
    return _valid && (_cost <= _table->resources);
}

void EndpointResolver::reset()
{
    _cost = 0;
    _used = 0;
    _valid = true;
}

usb_ep_t EndpointResolver::index_to_endpoint(int index)
{
    return index_to_logical(index) | ((index & 1) ? 0x80 : 0);
}

int EndpointResolver::next_index(usb_ep_type_t type, bool in_not_out)
{
    for (int logical = 0; logical < (int)(sizeof(_table->table) / sizeof(_table->table[0])); logical++) {
        uint32_t index = logical_to_index(logical, in_not_out);
        uint32_t other = logical_to_index(logical, !in_not_out);
        const usb_ep_entry_t &entry = _table->table[logical];

        usb_ep_attr_t dir = entry.attributes & USB_EP_ATTR_DIR_MASK;
        bool in_allowed = dir != USB_EP_ATTR_DIR_OUT;
        bool out_allowed = dir != USB_EP_ATTR_DIR_IN;
        bool shared = dir == USB_EP_ATTR_DIR_IN_OR_OUT;

        if (!(entry.attributes & (1 << type))) {
            // This type is not supported
            continue;
        }

        if (in_not_out && !in_allowed) {
            // In endpoint not supported
            continue;
        }

        if (!in_not_out && !out_allowed) {
            // Out endpoint not supported
            continue;
        }

        if (_used & (1 << index)) {
            // This endpoint is in use
            continue;
        }

        if (shared && (_used & (1 << other))) {
            // This endpoint can only be one direction at a time and is in
            // use by the other direction
            continue;
        }

        return index;
    }

    // Not found
    return -1;
}
