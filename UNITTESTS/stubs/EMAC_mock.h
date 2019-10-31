/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef EMACMOCK_H
#define EMACMOCK_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "features/netsocket/EMAC.h"

class MockEMAC : public EMAC {
public:
    MOCK_METHOD0(power_up, bool());
    MOCK_METHOD0(power_down, void());
    MOCK_CONST_METHOD0(get_mtu_size, uint32_t());
    MOCK_CONST_METHOD0(get_align_preference, uint32_t());
    MOCK_CONST_METHOD2(get_ifname, void(char *name, uint8_t size));
    MOCK_CONST_METHOD0(get_hwaddr_size, uint8_t());
    MOCK_CONST_METHOD1(get_hwaddr, bool(uint8_t *addr));
    MOCK_METHOD1(set_hwaddr, void(const uint8_t *));
    MOCK_METHOD1(link_out, bool(emac_mem_buf_t *buf));
    MOCK_METHOD1(set_link_input_cb, void(emac_link_input_cb_t input_cb));
    MOCK_METHOD1(set_link_state_cb, void(emac_link_state_change_cb_t state_cb));
    MOCK_METHOD1(add_multicast_group, void(const uint8_t *address));
    MOCK_METHOD1(remove_multicast_group, void(const uint8_t *address));
    MOCK_METHOD1(set_all_multicast, void(bool all));
    MOCK_METHOD1(set_memory_manager, void(EMACMemoryManager &mem_mngr));

    static MockEMAC &get_instance()
    {
        static MockEMAC emacMock1;
        return emacMock1;
    }
};

MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return MockEMAC::get_instance();
}

#endif // EMACMOCK_H
