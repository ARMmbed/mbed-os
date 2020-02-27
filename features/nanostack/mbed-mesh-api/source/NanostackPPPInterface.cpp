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

#include "NanostackPPPInterface.h"
#include "NanostackPPPPhy.h"
#include "PPP.h"
#include "nsdynmemLIB.h"
#include "arm_hal_phy.h"
#include "mbed_interface.h"
#include "mesh_system.h"
#include "callback_handler.h"
#include "enet_tasklet.h"
#include "ip6string.h"

class PPPPhy : public NanostackPPPPhy {
public:
    PPPPhy(NanostackMemoryManager &mem, PPP &m);
    int8_t phy_register() override;

    void phy_power_on();
    void phy_power_off();

    void get_iid64(uint8_t *iid64) override;
    uint16_t get_mtu() override;

    void set_link_state_change_cb(link_state_change_cb_t cb) override;

    int8_t tx(uint8_t *data_ptr, uint16_t data_len, uint8_t tx_handle, data_protocol_e data_flow);

    void ppp_phy_rx(net_stack_mem_buf_t *mem);
    void link_state_cb(bool up);

private:
    NanostackMemoryManager &memory_manager;
    PPP &ppp;
    uint8_t iid64[8];
    link_state_change_cb_t link_state_change_cb = nullptr;
    bool active = false;
    bool powered_up = false;
    int8_t device_id = -1;
    phy_device_driver_s phy;
};

nsapi_error_t Nanostack::PPPInterface::initialize()
{
    nanostack_lock();

    if (register_phy() < 0) {
        nanostack_unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    nanostack_unlock();

    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::PPPInterface::bringup(bool dhcp, const char *ip,
                                               const char *netmask, const char *gw,
                                               nsapi_ip_stack_t stack, bool blocking)
{
    if (stack == IPV4_STACK) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _blocking = blocking;

    if (link_state_up) {
        connect_enet_tasklet();
    }

    if (blocking) {
        uint8_t retries = 10;
        while (_connect_status != NSAPI_STATUS_GLOBAL_UP) {
            int32_t count = connect_semaphore.try_acquire_for(3000);
            if (count <= 0 && retries-- == 0) {
                return NSAPI_ERROR_DHCP_FAILURE; // sort of...
            }
            // Not up until global up
            if (_connect_status == NSAPI_STATUS_LOCAL_UP) {
                _connect_status = NSAPI_STATUS_CONNECTING;
            }
        }
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::PPPInterface::connect_enet_tasklet()
{
    if (enet_tasklet_connected) {
        return NSAPI_ERROR_OK;
    }

    enet_tasklet_connected = true;

    nanostack_lock();

    if (interface_id < 0) {
        enet_tasklet_init();
        __mesh_handler_set_callback(this);
        interface_id = enet_tasklet_ppp_network_init(_device_id);
    }
    int8_t status = -1;
    if (interface_id >= 0) {
        status = enet_tasklet_connect(&__mesh_handler_c_callback, interface_id);
    }

    nanostack_unlock();

    if (status == -1) {
        return NSAPI_ERROR_DEVICE_ERROR;
    } else if (status == -2) {
        return NSAPI_ERROR_NO_MEMORY;
    } else if (status == -3) {
        return NSAPI_ERROR_ALREADY;
    } else if (status != 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::PPPInterface::bringdown()
{
    nanostack_lock();
    int8_t status = enet_tasklet_disconnect(true);
    enet_tasklet_connected = false;
    nanostack_unlock();

    if (status == -1) {
        return NSAPI_ERROR_DEVICE_ERROR;
    } else if (status == -2) {
        return NSAPI_ERROR_NO_MEMORY;
    } else if (status == -3) {
        return NSAPI_ERROR_ALREADY;
    } else if (status != 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (_blocking) {
        int32_t count = disconnect_semaphore.try_acquire_for(30000);

        if (count <= 0) {
            return NSAPI_ERROR_TIMEOUT;
        }
    }
    return NSAPI_ERROR_OK;
}

void Nanostack::PPPInterface::link_state_changed(bool up)
{
    link_state_up = up;

    // If application callback is set
    if (link_state_cb) {
        link_state_cb(up, _device_id);
    } else {
        if (up) {
            connect_enet_tasklet();
        }
    }
}

void Nanostack::PPPInterface::set_link_state_changed_callback(link_state_cb_t new_link_state_cb)
{
    link_state_cb = new_link_state_cb;
}

// GAH! no handles on the callback. Force a single interface
static PPPPhy *single_phy;

extern "C"
{
    static int8_t ppp_phy_interface_state_control(phy_interface_state_e, uint8_t)
    {
        return -1;
    }

    static int8_t ppp_phy_tx(uint8_t *data_ptr, uint16_t data_len, uint8_t tx_handle, data_protocol_e data_flow)
    {
        return single_phy->tx(data_ptr, data_len, tx_handle, data_flow);
    }

    PPPPhy::PPPPhy(NanostackMemoryManager &mem, PPP &m) : memory_manager(mem), ppp(m)
    {
    }

    void PPPPhy::ppp_phy_rx(net_stack_mem_buf_t *mem)
    {
        if (!active) {
            memory_manager.free(mem);
            return;
        }

        const uint8_t *ptr = NULL;
        uint8_t *tmpbuf = NULL;
        uint32_t total_len;

        if (memory_manager.get_next(mem) == NULL) {
            // Easy contiguous case
            ptr = static_cast<const uint8_t *>(memory_manager.get_ptr(mem));
            total_len = memory_manager.get_len(mem);
        } else {
            // Nanostack can't accept chunked data - make temporary contiguous copy
            total_len = memory_manager.get_total_len(mem);
            ptr = tmpbuf = static_cast<uint8_t *>(ns_dyn_mem_temporary_alloc(total_len));
            if (tmpbuf) {
                memory_manager.copy_from_buf(tmpbuf, total_len, mem);
            }
        }

        if (ptr && phy.phy_rx_cb) {
            phy.phy_rx_cb(ptr, total_len, 0xff, 0, device_id);
        }
        ns_dyn_mem_free(tmpbuf);
        memory_manager.free(mem);
    }

} // extern "C"

int8_t PPPPhy::tx(uint8_t *data_ptr, uint16_t data_len, uint8_t tx_handle, data_protocol_e data_flow)
{
    if (!active) {
        return -1;
    }

    net_stack_mem_buf_t *mem = memory_manager.alloc_pool(data_len, 0);
    if (!mem) {
        return -1;
    }
    memory_manager.copy_to_buf(mem, data_ptr, data_len);

    // They take ownership - their responsibility to free
    ppp.link_out(mem, IPV6_STACK);

    return 0;
}

int8_t PPPPhy::phy_register()
{
    active = true;

    if (device_id < 0) {
        phy.PHY_MAC = iid64;
        phy.address_write = NULL;
        phy.driver_description = const_cast<char *>("PPP");
        phy.link_type = PHY_LINK_PPP;
        phy.phy_MTU = 0;
        phy.phy_header_length = 0;
        phy.phy_tail_length = 0;
        phy.state_control = ppp_phy_interface_state_control;
        phy.tx = ppp_phy_tx;
        phy.phy_rx_cb = NULL;
        phy.phy_tx_done_cb = NULL;

        ppp.set_memory_manager(memory_manager);
        ppp.set_link_input_cb(mbed::callback(this, &PPPPhy::ppp_phy_rx));
        if (link_state_change_cb) {
            ppp.set_link_state_cb(mbed::callback(this, &PPPPhy::link_state_cb));
        }

        device_id = arm_net_phy_register(&phy);

        if (device_id < 0) {
            return -1;
        }
    }

    return device_id;
}

void PPPPhy::phy_power_on()
{
    if (!powered_up) {
        if (!ppp.power_up()) {
            return;
        }
        phy.phy_MTU = get_mtu();
        powered_up = true;
        active = true;
    }
}

void PPPPhy::phy_power_off()
{
    if (powered_up) {
        ppp.power_down();
        powered_up = false;
    }
    active = false;
}

void PPPPhy::set_link_state_change_cb(link_state_change_cb_t cb)
{
    link_state_change_cb = cb;
}

void PPPPhy::link_state_cb(bool up)
{
    // Read negotiated parameters from PPP
    if (up) {
        get_iid64(iid64);
    }

    // Call upper level callback if set
    if (link_state_change_cb) {
        link_state_change_cb(up);
    }
}

void PPPPhy::get_iid64(uint8_t *iid64)
{
    if (!iid64) {
        return;
    }

    // Read link local IPv6 address
    const nsapi_addr_t *ipv6_ll_addr = ppp.get_ip_address(NSAPI_IPv6);
    if (ipv6_ll_addr) {
        memcpy(iid64, &ipv6_ll_addr->bytes[8], 8);
    }
}

uint16_t PPPPhy::get_mtu()
{
    return ppp.get_mtu_size();
}

nsapi_error_t Nanostack::add_ppp_interface(PPP &ppp, bool default_if, Nanostack::PPPInterface **interface_out)
{
    if (!single_phy) {
        single_phy = new (std::nothrow) PPPPhy(this->memory_manager, ppp);
        if (!single_phy) {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    static Nanostack::PPPInterface *interface = NULL;

    if (interface == NULL) {
        interface = new (std::nothrow) Nanostack::PPPInterface(*single_phy);
        if (!interface) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        single_phy->set_link_state_change_cb(mbed::callback(interface, &Nanostack::PPPInterface::link_state_changed));
    }

    interface->initialize();

    single_phy->phy_power_on();

    *interface_out = interface;

    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::add_ppp_interface(PPP &ppp, bool default_if, OnboardNetworkStack::Interface **interface_out)
{
    Nanostack::PPPInterface *interface;
    nsapi_error_t err = add_ppp_interface(ppp, default_if, &interface);
    *interface_out = interface;
    return err;
}

nsapi_error_t Nanostack::remove_ppp_interface(OnboardNetworkStack::Interface **interface_out)
{
    single_phy->phy_power_off();

    return NSAPI_ERROR_OK;
}
