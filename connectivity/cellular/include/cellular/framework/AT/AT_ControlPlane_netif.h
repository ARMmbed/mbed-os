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

#include "CellularList.h"
#include "ControlPlane_netif.h"
#include "ATHandler.h"
#include "AT_CellularDevice.h"

namespace mbed {

class AT_ControlPlane_netif: public ControlPlane_netif {
public:
    AT_ControlPlane_netif(ATHandler &at, int cid, AT_CellularDevice &device);
    virtual ~AT_ControlPlane_netif();

protected:
// ControlPlane_netif

    /* Sends data using +CSODCP specified in
     *  3GPP 27007 10.1.43: Sending of originating data via the control plane
     */
    virtual nsapi_size_or_error_t send(const void *cpdata, nsapi_size_t cpdata_length);

    /* Receives data using +CRTDCP specified in
     *  3GPP 27007 10.1.44: Reporting of terminating data via the control plane
     */
    virtual nsapi_size_or_error_t recv(void *cpdata, nsapi_size_t cpdata_length);

    virtual void data_received();
    virtual void attach(void (*callback)(void *), void *data);

    // Id of the PDP context that enables the control plane data connection
    int _cid;

private:
    struct packet_t {
        uint8_t *data;
        nsapi_size_t data_len;
        packet_t *next;
    };
    CellularList<packet_t> _packet_list;
    void (*_cb)(void *);
    void *_data;
    // Called on receiving URC: +CRTDCP
    void urc_cp_recv();

protected:
    ATHandler &_at;
    AT_CellularDevice &_device;
};

} //mbed namespace
