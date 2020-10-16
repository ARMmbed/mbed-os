
/** \addtogroup netsocket */
/** @{*/
/* UDPSocket
 * Copyright (c) 2015 ARM Limited
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

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "netsocket/InternetSocket.h"
#include "netsocket/NetworkStack.h"
#include "netsocket/NetworkInterface.h"
#include "netsocket/InternetDatagramSocket.h"
#include "rtos/EventFlags.h"
#include "ICMPSocket.h"


/** UDP socket implementation.
 */
class UDPSocket : public InternetDatagramSocket {
public:
    /** Create an uninitialized socket.
     *
     *  @note Must call open to initialize the socket on a network stack.
     */
    UDPSocket();

#if !defined(DOXYGEN_ONLY)

protected:
    nsapi_protocol_t get_proto() override;

#endif //!defined(DOXYGEN_ONLY)

};


#endif

/** @}*/
