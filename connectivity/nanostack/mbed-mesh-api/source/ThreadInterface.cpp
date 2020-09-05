/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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

#include "ThreadInterface.h"
#include "include/thread_tasklet.h"
#include "callback_handler.h"
#include "mesh_system.h"
#include "randLIB.h"

#include "ns_trace.h"
#define TRACE_GROUP "nsth"

class Nanostack::ThreadInterface : public Nanostack::MeshInterface {
public:
    nsapi_error_t bringup(bool dhcp, const char *ip,
                          const char *netmask, const char *gw,
                          nsapi_ip_stack_t stack = IPV6_STACK,
                          bool blocking = true) override;
    nsapi_error_t bringdown() override;
    friend class Nanostack;
    friend class ::ThreadInterface;
private:
    ThreadInterface(NanostackRfPhy &phy) : MeshInterface(phy), eui64_set(false) { }

    /*
     * \brief Initialization of the interface.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM when input parameters are illegal (also in case when RF device is already associated to other interface)
     * \return MESH_ERROR_MEMORY in case of memory error
     * \return MESH_ERROR_UNKNOWN in other error cases
     */
    mesh_error_t init();
    /**
     * \brief Connect interface to the mesh network
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM in case of illegal parameters.
     * \return MESH_ERROR_MEMORY in case of memory error.
     * \return MESH_ERROR_STATE if interface is already connected to network.
     * \return MESH_ERROR_UNKNOWN in case of unspecified error.
     * */
    mesh_error_t mesh_connect();

    /**
     * \brief Disconnect interface from the mesh network
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of error.
     * */
    mesh_error_t mesh_disconnect();

    /**
     * \brief Sets the eui64 for the device configuration.
     * By default this value is read from the radio driver.
     * The value must be set before calling the connect function.
     * */
    void device_eui64_set(const uint8_t *eui64);

    /**
     * \brief Reads the eui64 from the device configuration.
     * */
    void device_eui64_get(uint8_t *eui64);

    /**
     * \brief sets the PSKd for the device configuration.
     * The default value is overwritten, which is defined in the mbed_lib.json file in the mesh-api
     * The value must be set before calling the connect function.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM in case of illegal parameters.
     * \return MESH_ERROR_MEMORY in case of memory error.
     * */

    mesh_error_t device_pskd_set(const char *pskd);

    bool eui64_set;
};

Nanostack::ThreadInterface *ThreadInterface::get_interface() const
{
    return static_cast<Nanostack::ThreadInterface *>(_interface);
}

nsapi_error_t ThreadInterface::do_initialize()
{
    if (!_interface) {
        _interface = new (std::nothrow) Nanostack::ThreadInterface(*_phy);
        if (!_interface) {
            return NSAPI_ERROR_NO_MEMORY;
        }
        _interface->attach(_connection_status_cb);
    }
    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::ThreadInterface::bringup(bool dhcp, const char *ip,
                                                  const char *netmask, const char *gw,
                                                  nsapi_ip_stack_t stack, bool blocking)
{
    if (_connect_status == NSAPI_STATUS_GLOBAL_UP || _connect_status == NSAPI_STATUS_LOCAL_UP) {
        return NSAPI_ERROR_IS_CONNECTED;
    } else if (_connect_status == NSAPI_STATUS_CONNECTING) {
        return NSAPI_ERROR_ALREADY;
    }

    if (stack == IPV4_STACK) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    if (register_phy() < 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    nanostack_lock();

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

    // In Thread wait connection for ever:
    // -routers will create new network and get local connectivity
    // -end devices will get connectivity once attached to existing network
    // -devices without network settings gets connectivity once commissioned and attached to network
    _connect_status = NSAPI_STATUS_CONNECTING;
    if (_blocking) {
        connect_semaphore.acquire();
    }
    return 0;
}

nsapi_error_t Nanostack::ThreadInterface::bringdown()
{
    nanostack_lock();

    mesh_error_t status = mesh_disconnect();

    nanostack_unlock();

    return map_mesh_error(status);
}

mesh_error_t Nanostack::ThreadInterface::init()
{
    thread_tasklet_init();
    __mesh_handler_set_callback(this);
    device_eui64_get(NULL); // Ensure we've selected the EUI-64 - this does it
    interface_id = thread_tasklet_network_init(_device_id);

    if (interface_id == -2) {
        return MESH_ERROR_PARAM;
    } else if (interface_id == -3) {
        return MESH_ERROR_MEMORY;
    } else if (interface_id < 0) {
        return MESH_ERROR_UNKNOWN;
    }
    return MESH_ERROR_NONE;
}

mesh_error_t Nanostack::ThreadInterface::mesh_connect()
{
    int8_t status;
    tr_debug("connect()");

    status = thread_tasklet_connect(&__mesh_handler_c_callback, interface_id);

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

mesh_error_t Nanostack::ThreadInterface::mesh_disconnect()
{
    int8_t status;

    status = thread_tasklet_disconnect(true);

    if (status >= 0) {
        return MESH_ERROR_NONE;
    }

    return MESH_ERROR_UNKNOWN;
}

void ThreadInterface::device_eui64_set(const uint8_t *eui64)
{
    get_interface()->device_eui64_set(eui64);
}

void ThreadInterface::device_eui64_get(uint8_t *eui64)
{
    memset(eui64, 0, 8);
    if (!get_interface()) {
        return;
    }
    get_interface()->device_eui64_get(eui64);
}

void Nanostack::ThreadInterface::device_eui64_set(const uint8_t *eui64)
{
    eui64_set = true;
    thread_tasklet_device_eui64_set(eui64);
}

void Nanostack::ThreadInterface::device_eui64_get(uint8_t *eui64)
{
    if (!eui64_set) {
        uint8_t eui64_buf[8];
        if (register_phy() < 0) {
            return;
        }
        get_phy().get_mac_address(eui64_buf);
        device_eui64_set(eui64_buf);
    }

    if (eui64) {
        thread_tasklet_device_eui64_get(eui64);
    }
}

mesh_error_t ThreadInterface::device_pskd_set(const char *pskd)
{
    return get_interface()->device_pskd_set(pskd);
}

mesh_error_t Nanostack::ThreadInterface::device_pskd_set(const char *pskd)
{
    return (mesh_error_t)thread_tasklet_device_pskd_set(pskd);
}

#define THREAD 0x2345
#if MBED_CONF_NSAPI_DEFAULT_MESH_TYPE == THREAD && DEVICE_802_15_4_PHY

MBED_WEAK MeshInterface *MeshInterface::get_target_default_instance()
{
    static bool inited;
    static ThreadInterface interface;
    singleton_lock();
    if (!inited) {
        nsapi_error_t result = interface.initialize(&NanostackRfPhy::get_default_instance());
        if (result != 0) {
            tr_error("Thread initialize failed: %d", result);
            singleton_unlock();
            return NULL;
        }
        inited = true;
    }
    singleton_unlock();
    return &interface;
}
#endif
