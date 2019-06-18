/*
 * Copyright (c) 2019 ARM Limited. All rights reserved.
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

#ifndef NANOSTACKPPPINTERFACE_H
#define NANOSTACKPPPINTERFACE_H

#include "MeshInterfaceNanostack.h"
#include "PPPInterface.h"
#include "NanostackPPPPhy.h"

class Nanostack::PPPInterface : public Nanostack::Interface {
public:
    virtual nsapi_error_t bringup(bool dhcp, const char *ip,
                                  const char *netmask, const char *gw,
                                  nsapi_ip_stack_t stack = DEFAULT_STACK,
                                  bool blocking = true);
    virtual nsapi_error_t bringdown();

    typedef mbed::Callback<void (uint8_t up, int8_t device_id)> link_state_cb_t;
    virtual void set_link_state_changed_callback(link_state_cb_t link_state_cb);

private:
    friend class Nanostack;
    PPPInterface(NanostackPhy &phy) : Interface(phy), link_state_up(false), enet_tasklet_connected(false) {}
    nsapi_error_t initialize();
    void link_state_changed(bool up);
    nsapi_error_t connect_enet_tasklet();
    link_state_cb_t link_state_cb;
    bool link_state_up;
    bool enet_tasklet_connected;
};

#endif
