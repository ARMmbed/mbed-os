/*
 * Copyright (c) 2016-2019 ARM Limited. All rights reserved.
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

#ifndef MESHINTERFACENANOSTACK_H
#define MESHINTERFACENANOSTACK_H

#include "Semaphore.h"
#include "MeshInterface.h"
#include "NanostackRfPhy.h"
#include "Nanostack.h"
#include "mesh_interface_types.h"

class Nanostack::Interface : public OnboardNetworkStack::Interface, private mbed::NonCopyable<Nanostack::Interface> {
public:
    nsapi_error_t get_ip_address(SocketAddress *address) final;
    char *get_mac_address(char *buf, nsapi_size_t buflen) override;
    nsapi_error_t set_mac_address(uint8_t *buf, nsapi_size_t buflen);
    nsapi_error_t get_netmask(SocketAddress *address) final;
    nsapi_error_t get_gateway(SocketAddress *address) override;
    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb) final;
    nsapi_connection_status_t get_connection_status() const final;
    virtual void get_mac_address(uint8_t *buf) const;

    /**
     * \brief Callback from C-layer
     * \param status state of the network
     * */
    void network_handler(mesh_connection_status_t status);

    int8_t get_interface_id() const
    {
        return interface_id;
    }
    int8_t get_driver_id() const
    {
        return _device_id;
    }

private:
    NanostackPhy &interface_phy;
protected:
    Interface(NanostackPhy &phy);
    nsapi_error_t register_phy();
    NanostackPhy &get_phy() const
    {
        return interface_phy;
    }
    int8_t interface_id = -1;
    int8_t _device_id = -1;
    rtos::Semaphore connect_semaphore;
    rtos::Semaphore disconnect_semaphore;

    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
    nsapi_connection_status_t _connect_status = NSAPI_STATUS_DISCONNECTED;
    nsapi_connection_status_t _previous_connection_status = NSAPI_STATUS_DISCONNECTED;
    bool _blocking = true;
};

class Nanostack::MeshInterface : public Nanostack::Interface {
public:
    char *get_interface_name(char *buf);
protected:
    MeshInterface(NanostackRfPhy &phy) : Interface(phy) { }
    NanostackRfPhy &get_phy() const
    {
        return static_cast<NanostackRfPhy &>(Interface::get_phy());
    }
};


class InterfaceNanostack : public virtual NetworkInterface {
public:
    /** Start the interface
     *
     *  @return     0 on success, negative error code on failure
     */
    nsapi_error_t connect() override;

    /** Stop the interface
     *
     *  @return     0 on success, negative error code on failure
     */
    nsapi_error_t disconnect() override;

    /** @copydoc NetworkInterface::get_ip_address */
    nsapi_error_t get_ip_address(SocketAddress *address) override;

    /** Get the internally stored MAC address
    /return     MAC address of the interface
    */
    const char *get_mac_address() override;

    /** @copydoc NetworkInterface::set_mac_address */
    nsapi_error_t set_mac_address(uint8_t *mac_addr, nsapi_size_t addr_len) override;

    /** Register callback for status reporting
     *
     *  The specified status callback function will be called on status changes
     *  on the network. The parameters on the callback are the event type and
     *  event-type dependent reason parameter.
     *
     *  @param status_cb The callback for status changes
     */
    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb) override;

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    nsapi_connection_status_t get_connection_status() const override;

    /** Set blocking status of connect() which by default should be blocking
     *
     *  @param blocking true if connect is blocking
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t set_blocking(bool blocking) override;

    /** Set file system root path.
     *
     *  Set file system root path that stack will use to write and read its data.
     *  Setting root_path to NULL will disable file system usage.
     *
     *  @param  root_path Address to NUL-terminated root-path string or NULL to disable file system usage.
     *  @return MESH_ERROR_NONE on success, MESH_ERROR_MEMORY in case of memory failure, MESH_ERROR_UNKNOWN in case of other error.
     */
    nsapi_error_t set_file_system_root_path(const char *root_path);

    /** Get the interface ID
     *  @return  Interface identifier
     */
    int8_t get_interface_id() const
    {
        return _interface->get_interface_id();
    }

protected:
    InterfaceNanostack() = default;
    Nanostack *get_stack(void) override;
    Nanostack::Interface *get_interface() const
    {
        return _interface;
    }
    virtual nsapi_error_t do_initialize() = 0;

    Nanostack::Interface *_interface = nullptr;

    SocketAddress ip_addr;
    char mac_addr_str[24] {};
    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
    bool _blocking = true;
    bool _configured = false;
};

class MeshInterfaceNanostack : public InterfaceNanostack, public MeshInterface, private mbed::NonCopyable<MeshInterfaceNanostack> {
public:

    /** Attach phy and initialize the mesh
     *
     *  Initializes a mesh interface on the given phy. Not needed if
     *  the phy is passed to the mesh's constructor.
     *
     *  @return     0 on success, negative on failure
     */
    nsapi_error_t initialize(NanostackRfPhy *phy);

protected:
    MeshInterfaceNanostack() = default;
    MeshInterfaceNanostack(NanostackRfPhy *phy) : _phy(phy) { }
    Nanostack::MeshInterface *get_interface() const
    {
        return static_cast<Nanostack::MeshInterface *>(_interface);
    }
    NanostackRfPhy *_phy = nullptr;
};

#endif /* MESHINTERFACENANOSTACK_H */
