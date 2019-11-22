/*
 * Copyright (c) 2019 ARM Limited
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

#if defined(MBED_CONF_RTOS_PRESENT)

#include "mbed_interface.h"
#include "netsocket/nsapi_types.h"
#include "cellular_driver_l3ip.h"

Cellular_driver_L3IP::Cellular_driver_L3IP()
{
}


bool Cellular_driver_L3IP::link_out(net_stack_mem_buf_t *buf)
{

    return true;
}


bool Cellular_driver_L3IP::power_up()
{

    return true;
}

uint32_t Cellular_driver_L3IP::get_mtu_size() const
{
    return 0;
}

uint32_t Cellular_driver_L3IP::get_align_preference() const
{
    return 0;
}

void Cellular_driver_L3IP::get_ifname(char *name, uint8_t size) const
{

}



void Cellular_driver_L3IP::set_link_input_cb(l3ip_link_input_cb_t input_cb)
{
    l3ip_link_input_cb = input_cb;
}

void Cellular_driver_L3IP::set_link_state_cb(l3ip_link_state_change_cb_t state_cb)
{
    l3ip_link_state_cb = state_cb;
}

void Cellular_driver_L3IP::add_ipv4_multicast_group(const SocketAddress &address)
{

}

void Cellular_driver_L3IP::add_ipv6_multicast_group(const SocketAddress &address)
{

}

void Cellular_driver_L3IP::remove_ipv4_multicast_group(const SocketAddress &address)
{

}

void Cellular_driver_L3IP::remove_ipv6_multicast_group(const SocketAddress &address)
{

}

void Cellular_driver_L3IP::set_all_multicast(bool all)
{

}

void Cellular_driver_L3IP::power_down()
{

}

void Cellular_driver_L3IP::set_memory_manager(NetStackMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}


Cellular_driver_L3IP &Cellular_driver_L3IP::get_instance()
{
    static Cellular_driver_L3IP l3ip_test_driver;
    return l3ip_test_driver;
}

// Weak so a module can override
MBED_WEAK L3IP &L3IP::get_default_instance()
{
    return Cellular_driver_L3IP::get_instance();
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

