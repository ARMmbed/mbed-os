/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright (c) 2017 ARM Limited
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "mbed_interface.h"
#include "netsocket/nsapi_types.h"
#include "Cellular_driver_L3IP.h"

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

void Cellular_driver_L3IP::add_ipv4_multicast_group(const char *address)
{

}

void Cellular_driver_L3IP::add_ipv6_multicast_group(const char *address)
{

}

void Cellular_driver_L3IP::remove_ipv4_multicast_group(const char *address)
{

}

void Cellular_driver_L3IP::remove_ipv6_multicast_group(const char *address)
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


Cellular_driver_L3IP &Cellular_driver_L3IP::get_instance() {
    static Cellular_driver_L3IP l3ip_test_driver;
    return l3ip_test_driver;
}

// Weak so a module can override
MBED_WEAK L3IP &L3IP::get_default_instance() {
    return Cellular_driver_L3IP::get_instance();
}

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

