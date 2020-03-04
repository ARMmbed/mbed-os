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
#ifndef CONTROLPLANE_NETIF_H_
#define CONTROLPLANE_NETIF_H_

#include "nsapi_types.h"

/** @file ControlPlane_netif.h
 * @brief Implements support for data transfer using Control Plane CIoT EPS optimization.
 *
 */

namespace mbed {

/* Length of the buffer storing data received over control plane */
#ifndef MBED_CONF_CELLULAR_MAX_CP_DATA_RECV_LEN
#define MBED_CONF_CELLULAR_MAX_CP_DATA_RECV_LEN 1358
#endif


/**
 * @addtogroup cellular
 * @{
 */

/** Implements support for data transfer using
 *  Control Plane CIoT EPS optimization specified in 3GPP 23.401(4.10), 3GPP 23.682(4.5.14).
 */
class ControlPlane_netif {
public:
    virtual ~ControlPlane_netif() = default;

protected:
    friend class CellularNonIPSocket;
    friend class CellularContext;

    /** Send data over cellular control plane
    *
    *  @param cpdata            Buffer of data to be sent over control plane connection
    *  @param cpdata_length     Length of data in bytes
    *  @return                  Number of sent bytes on success, negative error
    *                           code on failure.
    */
    virtual nsapi_size_or_error_t send(const void *cpdata, nsapi_size_t cpdata_length) = 0;

    /** Receive data over cellular control plane
    *
    *  @param cpdata            Destination buffer for data received from control plane connection
    *  @param cpdata_length     Length of data in bytes
    *  @return                  Number of received bytes on success, negative error
    *                           code on failure.
    */
    virtual nsapi_size_or_error_t recv(void *cpdata, nsapi_size_t cpdata_length) = 0;

    /** Receives data from the control plane PDP context
    *
    *  This function is called by cellular PDP context when data
    *  is received from network. It will invoke the callback set
    *  by the above attach.
    *
    */
    virtual void data_received() = 0;

    /** Register a callback on state change of the socket
    *
    *  The specified callback will be called on state changes such as when
    *  the socket can recv/send successfully and on when an error
    *  occurs. The callback may also be called spuriously without reason.
    *
    *  The callback may be called in an interrupt context and should not
    *  perform expensive operations such as recv/send calls.
    *
    *  @param callback     Function to call on state change
    *  @param data         Argument to pass to callback
    */
    virtual void attach(void (*callback)(void *), void *data) = 0;
};

/**
 * @}
 */

} // mbed namespace
#endif
