/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef QUECTEL_BG96_CONTROLPLANE_NETIF_H_
#define QUECTEL_BG96_CONTROLPLANE_NETIF_H_

#include "AT_ControlPlane_netif.h"

namespace mbed {

class QUECTEL_BG96_ControlPlane_netif: public AT_ControlPlane_netif {
public:
    QUECTEL_BG96_ControlPlane_netif(ATHandler &at, int cid);
    virtual ~QUECTEL_BG96_ControlPlane_netif() {};

// ControlPlane_netif
    nsapi_size_or_error_t send(const void *data, nsapi_size_t size);
    nsapi_size_or_error_t recv(void *buffer, nsapi_size_t size);
};

} /* namespace mbed */

#endif // QUECTEL_BG96_CONTROLPLANE_NETIF_H_
