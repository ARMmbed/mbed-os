/*
 * Copyright (c) 2018-2019 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "WisunInterface.h"
#include "NanostackRfPhy.h"
#include "include/wisun_tasklet.h"
#include "callback_handler.h"
#include "NanostackLockGuard.h"
#include "mesh_system.h"
#include "randLIB.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "ws_management_api.h"

#include "ns_trace.h"
#define TRACE_GROUP "WSIn"

class Nanostack::WisunInterface : public Nanostack::MeshInterface {
public:
    virtual nsapi_error_t bringup(bool dhcp, const char *ip,
                                  const char *netmask, const char *gw,
                                  nsapi_ip_stack_t stack = IPV6_STACK,
                                  bool blocking = true);
    virtual nsapi_error_t bringdown();
    virtual char *get_gateway(char *buf, nsapi_size_t buflen);

    friend class Nanostack;
    friend class ::WisunInterface;
private:
    WisunInterface(NanostackRfPhy &phy) : MeshInterface(phy) { }
    mesh_error_t init();
    mesh_error_t mesh_connect();
    mesh_error_t mesh_disconnect();
};

Nanostack::WisunInterface *WisunInterface::get_interface() const
{
    return static_cast<Nanostack::WisunInterface *>(_interface);
}

nsapi_error_t WisunInterface::do_initialize()
{
    if (!_interface) {
        _interface = new (std::nothrow) Nanostack::WisunInterface(*_phy);
        if (!_interface) {
            return NSAPI_ERROR_NO_MEMORY;
        }
        _interface->attach(_connection_status_cb);
    }

    // Apply mbed configuration to Wi-SUN
    configure();

    return NSAPI_ERROR_OK;
}

nsapi_error_t WisunInterface::configure()
{
    int status;

    if (_configured) {
        // Already configured
        return NSAPI_ERROR_OK;
    }

    _configured = true;
#ifdef MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME
    char network_name[] = {MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME};
    status = set_network_name((char *) &network_name);
    if (status < 0) {
        tr_error("Failed to set network name!");
        return NSAPI_ERROR_PARAMETER;
    }
#endif

#if (MBED_CONF_MBED_MESH_API_WISUN_REGULATORY_DOMAIN != 255) || (MBED_CONF_MBED_MESH_API_WISUN_OPERATING_CLASS != 255) || (MBED_CONF_MBED_MESH_API_WISUN_OPERATING_MODE != 255)
    status = set_network_regulatory_domain(MBED_CONF_MBED_MESH_API_WISUN_REGULATORY_DOMAIN,
                                           MBED_CONF_MBED_MESH_API_WISUN_OPERATING_CLASS,
                                           MBED_CONF_MBED_MESH_API_WISUN_OPERATING_MODE);
    if (status < 0) {
        tr_error("Failed to set regulatory domain!");
        return NSAPI_ERROR_PARAMETER;
    }
#endif

#if (MBED_CONF_MBED_MESH_API_WISUN_UC_CHANNEL_FUNCTION != 255)
    status = set_unicast_channel_function(static_cast<mesh_channel_function_t>(MBED_CONF_MBED_MESH_API_WISUN_UC_CHANNEL_FUNCTION),
                                          MBED_CONF_MBED_MESH_API_WISUN_UC_FIXED_CHANNEL,
                                          MBED_CONF_MBED_MESH_API_WISUN_UC_DWELL_INTERVAL);
    if (status < 0) {
        tr_error("Failed to set unicast channel function configuration");
        return NSAPI_ERROR_PARAMETER;
    }
#endif

#if (MBED_CONF_MBED_MESH_API_WISUN_BC_CHANNEL_FUNCTION != 255) || (MBED_CONF_MBED_MESH_API_WISUN_BC_DWELL_INTERVAL != 0) || (MBED_CONF_MBED_MESH_API_WISUN_BC_INTERVAL != 0)
    status = set_broadcast_channel_function(static_cast<mesh_channel_function_t>(MBED_CONF_MBED_MESH_API_WISUN_BC_CHANNEL_FUNCTION),
                                            MBED_CONF_MBED_MESH_API_WISUN_BC_FIXED_CHANNEL,
                                            MBED_CONF_MBED_MESH_API_WISUN_BC_DWELL_INTERVAL,
                                            MBED_CONF_MBED_MESH_API_WISUN_BC_INTERVAL);
    if (status < 0) {
        tr_error("Failed to set broadcast channel function configuration");
        return NSAPI_ERROR_PARAMETER;
    }
#endif

    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::WisunInterface::bringup(bool dhcp, const char *ip,
                                                 const char *netmask, const char *gw,
                                                 nsapi_ip_stack_t stack, bool blocking)
{
    nanostack_lock();

    if (register_phy() < 0) {
        nanostack_unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _blocking = blocking;

    // After the RF is up, we can seed the random from it.
    randLIB_seed_random();

    mesh_error_t status = init();
    if (status != MESH_ERROR_NONE) {
        nanostack_unlock();
        return map_mesh_error(status);
    }

    status = mesh_connect();
    if (status != MESH_ERROR_NONE) {
        nanostack_unlock();
        return map_mesh_error(status);
    }

    // Release mutex before blocking
    nanostack_unlock();

    if (blocking) {
        // wait connection for ever
        connect_semaphore.acquire();
    }
    return 0;

}

nsapi_error_t Nanostack::WisunInterface::bringdown()
{
    NanostackLockGuard lock;

    mesh_error_t status = mesh_disconnect();

    return map_mesh_error(status);
}

mesh_error_t Nanostack::WisunInterface::init()
{
    wisun_tasklet_init();
    __mesh_handler_set_callback(this);
    interface_id = wisun_tasklet_network_init(_device_id);

    if (interface_id == -2) {
        return MESH_ERROR_PARAM;
    } else if (interface_id == -3) {
        return MESH_ERROR_MEMORY;
    } else if (interface_id < 0) {
        return MESH_ERROR_UNKNOWN;
    }
    return MESH_ERROR_NONE;
}

mesh_error_t Nanostack::WisunInterface::mesh_connect()
{
    int8_t status = -9; // init to unknown error
    tr_debug("connect()");

    status = wisun_tasklet_connect(&__mesh_handler_c_callback, interface_id);

    if (status >= 0) {
        return MESH_ERROR_NONE;
    } else if (status == -1) {
        return MESH_ERROR_PARAM;
    } else if (status == -2) {
        return MESH_ERROR_MEMORY;
    } else if (status == -3) {
        return MESH_ERROR_STATE;
    } else {
        return MESH_ERROR_UNKNOWN;
    }
}

mesh_error_t Nanostack::WisunInterface::mesh_disconnect()
{
    int8_t status = -1;

    status = wisun_tasklet_disconnect(true);

    if (status >= 0) {
        return MESH_ERROR_NONE;
    }

    return MESH_ERROR_UNKNOWN;
}

char *Nanostack::WisunInterface::get_gateway(char *buf, nsapi_size_t buflen)
{
    NanostackLockGuard lock;
    if (wisun_tasklet_get_router_ip_address(buf, buflen) == 0) {
        return buf;
    }
    return NULL;
}

bool WisunInterface::getRouterIpAddress(char *address, int8_t len)
{
    SocketAddress sock_addr;
    if (_interface->get_gateway(&sock_addr) == NSAPI_ERROR_OK) {
        strncpy(address, sock_addr.get_ip_address(), len);
        return true;
    }
    return false;
}

mesh_error_t WisunInterface::set_network_name(char *network_name)
{
    int status = ws_management_network_name_set(get_interface_id(), network_name);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::get_network_name(char *network_name)
{
    int status = ws_management_network_name_get(get_interface_id(), network_name);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::validate_network_name(char *network_name)
{
    int status = ws_management_network_name_validate(get_interface_id(), network_name);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::set_network_regulatory_domain(uint8_t regulatory_domain, uint8_t operating_class, uint8_t operating_mode)
{
    int status = ws_management_regulatory_domain_set(get_interface_id(), regulatory_domain, operating_class, operating_mode);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::get_network_regulatory_domain(uint8_t *regulatory_domain, uint8_t *operating_class, uint8_t *operating_mode)
{
    int status = ws_management_regulatory_domain_get(get_interface_id(), regulatory_domain, operating_class, operating_mode);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::validate_network_regulatory_domain(uint8_t regulatory_domain, uint8_t operating_class, uint8_t operating_mode)
{
    int status = ws_management_regulatory_domain_validate(get_interface_id(), regulatory_domain, operating_class, operating_mode);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::set_network_size(uint8_t network_size)
{
    if (network_size == 0xff) {
        // Size 0xff is internal API
        network_size = 0xfe;
    }

    int status = ws_management_network_size_set(get_interface_id(), network_size);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::get_network_size(uint8_t *network_size)
{
    int status = ws_management_network_size_get(get_interface_id(), network_size);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::validate_network_size(uint8_t network_size)
{
    if (network_size == 0xff) {
        // Size 0xff is internal API
        network_size = 0xfe;
    }

    int status = ws_management_network_size_validate(get_interface_id(), network_size);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::set_channel_mask(uint32_t channel_mask[8])
{
    int status = ws_management_channel_mask_set(get_interface_id(), channel_mask);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::get_channel_mask(uint32_t *channel_mask)
{
    int status = ws_management_channel_mask_get(get_interface_id(), channel_mask);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::validate_channel_mask(uint32_t channel_mask[8])
{
    int status = ws_management_channel_mask_validate(get_interface_id(), channel_mask);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::set_unicast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel, uint8_t dwell_interval)
{
    int status = ws_management_fhss_unicast_channel_function_configure(get_interface_id(), channel_function, fixed_channel, dwell_interval);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::get_unicast_channel_function(mesh_channel_function_t *channel_function, uint16_t *fixed_channel, uint8_t *dwell_interval)
{
    uint8_t ch_function;
    int status = ws_management_fhss_unicast_channel_function_get(get_interface_id(), &ch_function, fixed_channel, dwell_interval);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }
    *channel_function = static_cast<mesh_channel_function_t>(ch_function);

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::validate_unicast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel, uint8_t dwell_interval)
{
    int status = ws_management_fhss_unicast_channel_function_validate(get_interface_id(), channel_function, fixed_channel, dwell_interval);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::set_broadcast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel, uint8_t dwell_interval, uint32_t broadcast_interval)
{
    int status = ws_management_fhss_broadcast_channel_function_configure(get_interface_id(), channel_function, fixed_channel, dwell_interval, broadcast_interval);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::get_broadcast_channel_function(mesh_channel_function_t *channel_function, uint16_t *fixed_channel, uint8_t *dwell_interval, uint32_t *broadcast_interval)
{
    uint8_t ch_function;
    int status = ws_management_fhss_broadcast_channel_function_get(get_interface_id(), &ch_function, fixed_channel, dwell_interval, broadcast_interval);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }
    *channel_function = static_cast<mesh_channel_function_t>(ch_function);

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::validate_broadcast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel, uint8_t dwell_interval, uint32_t broadcast_interval)
{
    int status = ws_management_fhss_broadcast_channel_function_validate(get_interface_id(), channel_function, fixed_channel, dwell_interval, broadcast_interval);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::set_timing_parameters(uint16_t disc_trickle_imin, uint16_t disc_trickle_imax, uint8_t disc_trickle_k, uint16_t pan_timeout)
{
    int status = ws_management_timing_parameters_set(get_interface_id(), disc_trickle_imin, disc_trickle_imax, disc_trickle_k, pan_timeout);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::get_timing_parameters(uint16_t *disc_trickle_imin, uint16_t *disc_trickle_imax, uint8_t *disc_trickle_k, uint16_t *pan_timeout)
{
    int status = ws_management_timing_parameters_get(get_interface_id(), disc_trickle_imin, disc_trickle_imax, disc_trickle_k, pan_timeout);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::validate_timing_parameters(uint16_t disc_trickle_imin, uint16_t disc_trickle_imax, uint8_t disc_trickle_k, uint16_t pan_timeout)
{
    int status = ws_management_timing_parameters_validate(get_interface_id(), disc_trickle_imin, disc_trickle_imax, disc_trickle_k, pan_timeout);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunInterface::set_own_certificate(uint8_t *cert, uint16_t cert_len, uint8_t *cert_key, uint16_t cert_key_len)
{
    mesh_error_t ret_val = MESH_ERROR_NONE;
    int status =  wisun_tasklet_set_own_certificate(cert, cert_len, cert_key, cert_key_len);
    if (status == -1) {
        ret_val = MESH_ERROR_MEMORY;
    } else if (status == -2) {
        ret_val = MESH_ERROR_STATE;
    }

    return ret_val;
}

mesh_error_t WisunInterface::remove_own_certificates(void)
{
    mesh_error_t ret_val = MESH_ERROR_NONE;
    int status =  wisun_tasklet_remove_own_certificates();
    if (status == -1) {
        ret_val = MESH_ERROR_MEMORY;
    } else if (status == -2) {
        ret_val = MESH_ERROR_STATE;
    }

    return ret_val;
}

mesh_error_t WisunInterface::set_trusted_certificate(uint8_t *cert, uint16_t cert_len)
{
    mesh_error_t ret_val = MESH_ERROR_NONE;
    int status =  wisun_tasklet_set_trusted_certificate(cert, cert_len);
    if (status == -1) {
        ret_val = MESH_ERROR_MEMORY;
    } else if (status == -2) {
        ret_val = MESH_ERROR_STATE;
    }

    return ret_val;
}

mesh_error_t WisunInterface::remove_trusted_certificates(void)
{
    mesh_error_t ret_val = MESH_ERROR_NONE;
    int status =  wisun_tasklet_remove_trusted_certificates();
    if (status == -1) {
        ret_val = MESH_ERROR_MEMORY;
    } else if (status == -2) {
        ret_val = MESH_ERROR_STATE;
    }

    return ret_val;
}

mesh_error_t WisunInterface::enable_statistics(void)
{
    mesh_error_t ret_val = MESH_ERROR_NONE;
    int status = wisun_tasklet_statistics_start();
    if (status < 0) {
        ret_val = MESH_ERROR_UNKNOWN;
    }
    return ret_val;
}

mesh_error_t WisunInterface::read_nw_statistics(mesh_nw_statistics_t *statistics)
{
    mesh_error_t ret_val = MESH_ERROR_NONE;
    int status = wisun_tasklet_statistics_nw_read(statistics);
    if (status < 0) {
        ret_val = MESH_ERROR_UNKNOWN;
    }
    return ret_val;
}

mesh_error_t WisunInterface::read_mac_statistics(mesh_mac_statistics_t *statistics)
{
    mesh_error_t ret_val = MESH_ERROR_NONE;
    int status = wisun_tasklet_statistics_mac_read(statistics);
    if (status < 0) {
        ret_val = MESH_ERROR_UNKNOWN;
    }
    return ret_val;
}

#define WISUN 0x2345
#if MBED_CONF_NSAPI_DEFAULT_MESH_TYPE == WISUN && DEVICE_802_15_4_PHY
MBED_WEAK MeshInterface *MeshInterface::get_target_default_instance()
{
    static bool inited;
    static WisunInterface interface;
    singleton_lock();
    if (!inited) {
        nsapi_error_t result = interface.initialize(&NanostackRfPhy::get_default_instance());
        if (result != 0) {
            tr_error("Wi-SUN initialize failed: %d", result);
            singleton_unlock();
            return NULL;
        }
        inited = true;
    }
    singleton_unlock();
    return &interface;
}
#endif
